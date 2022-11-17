/** @file
  ACPI Debug feature driver implementation.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesLib.h>
#include <Protocol/AcpiTable.h>
#include <IndustryStandard/Acpi.h>

#include <Protocol/SmmBase2.h>
#include <Protocol/SmmEndOfDxe.h>
#include <Protocol/SmmSwDispatch2.h>

#define ACPI_DEBUG_STR      "INTEL ACPI DEBUG"

//
// ASL NAME structure
//
#pragma pack(1)
typedef struct {
  UINT8  NameOp;            // Byte [0]=0x08:NameOp.
  UINT32 NameString;        // Byte [4:1]=Name of object.
  UINT8  DWordPrefix;       // Byte [5]=0x0C:DWord Prefix.
  UINT32 Value;             // 0 ; Value of named object.
} NAME_LAYOUT;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8  Signature[16];     // "INTEL ACPI DEBUG"
  UINT32 BufferSize;        // Total size of Acpi Debug buffer including header structure
  UINT32 Head;              // Current buffer pointer for SMM to print out
  UINT32 Tail;              // Current buffer pointer for ASL to input
  UINT8  SmiTrigger;        // Value to trigger the SMI via B2 port
  UINT8  Wrap;              // If current Tail < Head
  UINT8  SmmVersion;        // If SMM version
  UINT8  Truncate;          // If the input from ASL > MAX_BUFFER_SIZE
} ACPI_DEBUG_HEAD;
#pragma pack()

#define AD_SIZE             sizeof (ACPI_DEBUG_HEAD) // This is 0x20

#define MAX_BUFFER_SIZE     32

UINT32                      mBufferEnd = 0;
ACPI_DEBUG_HEAD             *mAcpiDebug = NULL;

EFI_SMM_SYSTEM_TABLE2       *mSmst = NULL;

/**
  Patch and load ACPI table.

  @param[in] AcpiDebugAddress   Address of Acpi debug memory buffer.
  @param[in] BufferIndex        Index that starts after the Acpi Debug head.
  @param[in] BufferEnd          End of Acpi debug memory buffer.

**/
VOID
PatchAndLoadAcpiTable (
  IN ACPI_DEBUG_HEAD            *AcpiDebugAddress,
  IN UINT32                     BufferIndex,
  IN UINT32                     BufferEnd
  )
{
  EFI_STATUS                    Status;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  UINTN                         Size;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableKey;
  UINT8                         *CurrPtr;
  UINT32                        *Signature;
  NAME_LAYOUT                   *NamePtr;
  UINT8                         UpdateCounter;

  Status = GetSectionFromFv (
             &gEfiCallerIdGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &TableHeader,
             &Size
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // This is Acpi Debug SSDT. Acpi Debug should be enabled if we reach here so load the table.
  //
  ASSERT (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId == SIGNATURE_64 ('A', 'D', 'e', 'b', 'T', 'a', 'b', 'l'));

  //
  // Patch some pointers for the ASL code before loading the SSDT.
  //

  //
  // Count pointer updates, so we can stop after all three pointers are patched.
  //
  UpdateCounter = 1;
  for (CurrPtr = (UINT8 *) TableHeader; CurrPtr <= ((UINT8 *) TableHeader + TableHeader->Length) && UpdateCounter < 4; CurrPtr++) {
    Signature = (UINT32 *) (CurrPtr + 1);
    //
    // patch DPTR (address of Acpi debug memory buffer)
    //
    if ((*CurrPtr == AML_NAME_OP) && *Signature == SIGNATURE_32 ('D', 'P', 'T', 'R')) {
      NamePtr = (NAME_LAYOUT *) CurrPtr;
      NamePtr->Value  = (UINT32) (UINTN) AcpiDebugAddress;
      UpdateCounter++;
    }
    //
    // patch EPTR (end of Acpi debug memory buffer)
    //
    if ((*CurrPtr == AML_NAME_OP) && *Signature == SIGNATURE_32 ('E', 'P', 'T', 'R')) {
      NamePtr = (NAME_LAYOUT *) CurrPtr;
      NamePtr->Value  = BufferEnd;
      UpdateCounter++;
    }
    //
    // patch CPTR (used as an index that starts after the Acpi Debug head)
    //
    if ((*CurrPtr == AML_NAME_OP) && *Signature == SIGNATURE_32 ('C', 'P', 'T', 'R')) {
      NamePtr = (NAME_LAYOUT *) CurrPtr;
      NamePtr->Value  = BufferIndex;
      UpdateCounter++;
    }
  }

  //
  // Add the table
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  TableKey = 0;
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        TableHeader,
                        Size,
                        &TableKey
                        );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

Done:
  gBS->FreePool (TableHeader);
  return ;
}

/**
  Allocate Acpi Debug memory.

  @param[out] BufferSize    Pointer to Acpi debug memory buffer size.

  @return Address of Acpi debug memory buffer.

**/
EFI_PHYSICAL_ADDRESS
AllocateAcpiDebugMemory (
  OUT UINT32           *BufferSize
  )
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS AcpiDebugAddress;
  UINTN                PagesNum;

  AcpiDebugAddress = 0;
  *BufferSize = 0;

  //
  // Reserve memory to store Acpi Debug data.
  //
  AcpiDebugAddress = 0xFFFFFFFF;
  PagesNum = EFI_SIZE_TO_PAGES (PcdGet32 (PcdAcpiDebugBufferSize));
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  PagesNum,
                  &AcpiDebugAddress
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  DEBUG ((DEBUG_INFO, "AcpiDebugAddress - 0x%08x\n", AcpiDebugAddress));

  Status = PcdSet32S (PcdAcpiDebugAddress, (UINT32) AcpiDebugAddress);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    gBS->FreePages (AcpiDebugAddress, PagesNum);
    return 0;
  }

  *BufferSize = PcdGet32 (PcdAcpiDebugBufferSize);

  return AcpiDebugAddress;
}

/**
  Acpi Debug EndOfDxe notification.

  @param[in] Event      Event whose notification function is being invoked.
  @param[in] Context    Pointer to the notification function's context.

**/
VOID
EFIAPI
AcpiDebugEndOfDxeNotification (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINT32        BufferSize;
  UINT32        BufferIndex;

  mAcpiDebug = (ACPI_DEBUG_HEAD *) (UINTN) AllocateAcpiDebugMemory (&BufferSize);
  if (mAcpiDebug != NULL) {
    //
    // Init ACPI DEBUG buffer to lower case 'x'.
    //
    SetMem ((VOID *) mAcpiDebug, BufferSize, 0x78);

    //
    // Clear header of AD_SIZE bytes.
    //
    ZeroMem ((VOID *) mAcpiDebug, AD_SIZE);

    //
    // Write a signature to the first line of the buffer, "INTEL ACPI DEBUG".
    //
    CopyMem ((VOID *) mAcpiDebug, ACPI_DEBUG_STR, sizeof (ACPI_DEBUG_STR) - 1);

    BufferIndex = (UINT32) (UINTN) mAcpiDebug;
    mBufferEnd = BufferIndex + BufferSize;

    //
    // Leave the Index after the Acpi Debug head.
    //
    BufferIndex += AD_SIZE;

    //
    // Patch and Load the SSDT ACPI Tables.
    //
    PatchAndLoadAcpiTable (mAcpiDebug, BufferIndex, mBufferEnd);

    mAcpiDebug->Head = BufferIndex;
    mAcpiDebug->Tail = BufferIndex;
    mAcpiDebug->BufferSize = BufferSize;
  }

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);

  return ;
}

/**
  Initialize ACPI Debug.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The driver initializes correctly.

**/
EFI_STATUS
EFIAPI
InitializeAcpiDebugDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_EVENT             EndOfDxeEvent;

  if (!PcdGetBool (PcdAcpiDebugFeatureActive)) {
    return EFI_SUCCESS;
  }

  //
  // Register EndOfDxe notification
  // that point could ensure the Acpi Debug related PCDs initialized.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiDebugEndOfDxeNotification,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Software SMI callback for ACPI Debug which is called from ACPI method.

  @param[in]      DispatchHandle    The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context           Points to an optional handler context which was specified when the
                                    handler was registered.
  @param[in, out] CommBuffer        A pointer to a collection of data in memory that will
                                    be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize    The size of the CommBuffer.

  @retval EFI_SUCCESS               The interrupt was handled successfully.

**/
EFI_STATUS
EFIAPI
AcpiDebugSmmCallback (
  IN EFI_HANDLE     DispatchHandle,
  IN CONST VOID     *Context,
  IN OUT VOID       *CommBuffer,
  IN OUT UINTN      *CommBufferSize
  )
{
  UINT8             Buffer[MAX_BUFFER_SIZE];

  //
  // Validate the fields in mAcpiDebug to ensure there is no harm to SMI handler.
  // mAcpiDebug is below 4GB and the start address of whole buffer.
  //
  if ((mAcpiDebug->BufferSize != (mBufferEnd - (UINT32) (UINTN) mAcpiDebug)) ||
      (mAcpiDebug->Head < (UINT32) ((UINTN) mAcpiDebug + AD_SIZE)) ||
      (mAcpiDebug->Head > mBufferEnd) ||
      (mAcpiDebug->Tail < (UINT32) ((UINTN) mAcpiDebug + AD_SIZE)) ||
      (mAcpiDebug->Tail > mBufferEnd)) {
    //
    // If some fields in mAcpiDebug are invaid, return directly.
    //
    return EFI_SUCCESS;
  }

  if (!(BOOLEAN)mAcpiDebug->Wrap && ((mAcpiDebug->Head >= (UINT32) ((UINTN) mAcpiDebug + AD_SIZE))
    && (mAcpiDebug->Head < mAcpiDebug->Tail))){
    //
    // If curent ----- buffer + 020
    //                 ...
    //                 ... Head
    //                 ... Data for SMM print
    //                 ... Tail
    //                 ... Vacant for ASL input
    //           ----- buffer end
    //
    // skip NULL block
    //
    while ((*(CHAR8 *) (UINTN) mAcpiDebug->Head == '\0') && (mAcpiDebug->Head < mAcpiDebug->Tail)) {
      mAcpiDebug->Head ++;
    }

    if (mAcpiDebug->Head < mAcpiDebug->Tail) {
      ZeroMem (Buffer, MAX_BUFFER_SIZE);
      AsciiStrnCpyS ((CHAR8 *) Buffer, MAX_BUFFER_SIZE, (CHAR8 *) (UINTN) mAcpiDebug->Head, MAX_BUFFER_SIZE - 1);

      DEBUG ((DEBUG_INFO | DEBUG_ERROR, "%a%a\n", Buffer, (BOOLEAN) mAcpiDebug->Truncate ? "..." : ""));
      mAcpiDebug->Head += MAX_BUFFER_SIZE;

      if (mAcpiDebug->Head >= (mAcpiDebug->Tail)) {
        //
        // When head == tail, we do nothing in handler.
        //
        mAcpiDebug->Head = mAcpiDebug->Tail;
      }
    }
  } else if ((BOOLEAN) mAcpiDebug->Wrap && ((mAcpiDebug->Head > mAcpiDebug->Tail)
    && (mAcpiDebug->Head < (UINT32) ((UINTN) mAcpiDebug + mAcpiDebug->BufferSize)))){
    //
    // If curent ----- buffer + 020
    //                 ... Tail
    //                 ... Vacant for ASL input
    //                 ... Head
    //                 ... Data for SMM print
    //           ----- buffer end
    //
    while ((*(CHAR8 *) (UINTN) mAcpiDebug->Head == '\0') && (mAcpiDebug->Head < (UINT32) ((UINTN) mAcpiDebug + mAcpiDebug->BufferSize))) {
      mAcpiDebug->Head ++;
    }
    if (mAcpiDebug->Head < (UINT32) ((UINTN) mAcpiDebug + mAcpiDebug->BufferSize)){
      ZeroMem (Buffer, MAX_BUFFER_SIZE);
      AsciiStrnCpyS ((CHAR8 *) Buffer, MAX_BUFFER_SIZE, (CHAR8 *) (UINTN) mAcpiDebug->Head, MAX_BUFFER_SIZE - 1);
      DEBUG ((DEBUG_INFO | DEBUG_ERROR, "%a%a\n", Buffer, (BOOLEAN) mAcpiDebug->Truncate ? "..." : ""));
      mAcpiDebug->Head += MAX_BUFFER_SIZE;

      if (mAcpiDebug->Head >= (UINT32) ((UINTN) mAcpiDebug + mAcpiDebug->BufferSize)) {
        //
        // We met end of buffer.
        //
        mAcpiDebug->Wrap = 0;
        mAcpiDebug->Head = (UINT32) ((UINTN) mAcpiDebug + AD_SIZE);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Acpi Debug SmmEndOfDxe notification.

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification runs successfully.

 **/
EFI_STATUS
EFIAPI
AcpiDebugSmmEndOfDxeNotification (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  EFI_STATUS                        Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_HANDLE                        SwHandle;

  AcpiDebugEndOfDxeNotification (NULL, NULL);

  if (mAcpiDebug != NULL) {
    //
    // Get the Sw dispatch protocol and register SMI callback function.
    //
    SwDispatch = NULL;
    Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID **) &SwDispatch);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    SwContext.SwSmiInputValue = (UINTN) -1;
    Status = SwDispatch->Register (SwDispatch, AcpiDebugSmmCallback, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    mAcpiDebug->SmiTrigger = (UINT8) SwContext.SwSmiInputValue;
    mAcpiDebug->SmmVersion = 1;
  }

  return EFI_SUCCESS;
}

/**
  Initialize ACPI Debug.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The driver initializes correctly.

**/
EFI_STATUS
EFIAPI
InitializeAcpiDebugSmm (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  VOID                      *Registration;
  EFI_SMM_BASE2_PROTOCOL    *SmmBase2;
  BOOLEAN                   InSmm;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID **) &SmmBase2);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SmmBase2->InSmm (SmmBase2, &InSmm);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ASSERT (InSmm);

  if (!InSmm) {
    return EFI_UNSUPPORTED;
  }

  Status = SmmBase2->GetSmstLocation (SmmBase2, &mSmst);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Register SmmEndOfDxe notification
  // that point could ensure the Acpi Debug related PCDs initialized.
  //
  Registration = NULL;
  Status = mSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmEndOfDxeProtocolGuid,
                    AcpiDebugSmmEndOfDxeNotification,
                    &Registration
                    );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
