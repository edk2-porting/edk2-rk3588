/** @file
 *
 *  ACPI platform driver
 *
 *  Copyright (c) 2020, Jeremy Linton
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/AcpiAml.h>
#include <IndustryStandard/PeImage.h>
#include <Library/AcpiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <AcpiTables.h>
#include <VarStoreData.h>

STATIC CONST EFI_GUID mAcpiTableFile = {
  0x7E374E25, 0x8E01, 0x4FEE, { 0x87, 0xf2, 0x39, 0x0C, 0x23, 0xC6, 0x06, 0xCD }
};

STATIC EFI_EXIT_BOOT_SERVICES    mOriginalExitBootServices;

STATIC EFI_ACPI_SDT_PROTOCOL     *mAcpiSdtProtocol;

typedef enum {
  AcpiOsUnknown = 0,
  AcpiOsWindows,
} ACPI_OS_BOOT_TYPE;

#define SDT_PATTERN_LEN (AML_NAME_SEG_SIZE + 1)

//
// Simple NameOp integer patcher.
// Does not allocate memory and can be safely used at ExitBootServices.
//
STATIC
EFI_STATUS
AcpiUpdateSdtNameInteger (
  IN  EFI_ACPI_DESCRIPTION_HEADER  *AcpiTable,
  IN  CHAR8                        Name[AML_NAME_SEG_SIZE],
  IN  UINTN                        Value
  )
{
  UINTN   Index;
  CHAR8   Pattern[SDT_PATTERN_LEN];
  UINT8   *SdtPtr;
  UINT32  DataSize;
  UINT32  ValueOffset;

  if (AcpiTable->Length <= SDT_PATTERN_LEN) {
    return EFI_INVALID_PARAMETER;
  }

  SdtPtr = (UINT8 *)AcpiTable;
  //
  // Do a single NameOp variable replacement. These are of the
  // form "08 XXXX SIZE VAL", where SIZE is: 0A=byte, 0B=word, 0C=dword,
  // XXXX is the name and VAL is the value.
  //
  Pattern[0] = AML_NAME_OP;
  CopyMem (Pattern + 1, Name, AML_NAME_SEG_SIZE);

  ValueOffset = SDT_PATTERN_LEN + 1;

  for (Index = 0; Index < (AcpiTable->Length - SDT_PATTERN_LEN); Index++) {
    if (CompareMem (SdtPtr + Index, Pattern, SDT_PATTERN_LEN) == 0) {
      switch (SdtPtr[Index + SDT_PATTERN_LEN]) {
        case AML_QWORD_PREFIX:
          DataSize = sizeof (UINT64);
          break;
        case AML_DWORD_PREFIX:
          DataSize = sizeof (UINT32);
          break;
        case AML_WORD_PREFIX:
          DataSize = sizeof (UINT16);
          break;
        case AML_ONE_OP:
        case AML_ZERO_OP:
          ValueOffset--;
          // Fallthrough
        case AML_BYTE_PREFIX:
          DataSize = sizeof (UINT8);
          break;
        default:
          return EFI_UNSUPPORTED;
      }
      CopyMem (SdtPtr + Index + ValueOffset, &Value, DataSize);
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

STATIC
BOOLEAN
AcpiVerifyUpdateTable (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader
  )
{
  BOOLEAN              Result;

  Result = TRUE;

  switch (AcpiHeader->OemTableId) {
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '3', '4', 'L', '0'):
      Result = FixedPcdGetBool (PcdPcie30Supported)
                && PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED;
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '3', '2', 'L', '0'):
      Result = FALSE; // not supported yet
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '2', '1', 'L', '0'):
      Result = PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_PCIE;
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '2', '1', 'L', '1'):
      Result = PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_PCIE;
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '2', '1', 'L', '2'):
      Result = PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_PCIE;
      break;
    case SIGNATURE_64 ('A', 'H', 'C', 'S', 'A', 'T', 'A', '0'):
      Result = PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_SATA;
      break;
    case SIGNATURE_64 ('A', 'H', 'C', 'S', 'A', 'T', 'A', '1'):
      Result = PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_SATA;
      break;
    case SIGNATURE_64 ('A', 'H', 'C', 'S', 'A', 'T', 'A', '2'):
      Result = PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_SATA;
      break;
  }

  return Result;
}

STATIC
BOOLEAN
AcpiFixupMcfg (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader
  )
{
  RK3588_MCFG_TABLE   *Table;
  UINT32              Seg;

  Table = (RK3588_MCFG_TABLE *) AcpiHeader;

  for (Seg = 0; Seg < ARRAY_SIZE (Table->Entry); Seg++) {
    if ((PcdGet32 (PcdPcieEcamCompliantSegmentsMask) & (1 << Seg)) != 0) {
      Table->Entry[Seg].BaseAddress -= 0x8000;
    }
  }

  return TRUE;
}

STATIC
BOOLEAN
AcpiHandleDynamicNamespace (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader
  )
{
  switch (AcpiHeader->Signature) {
    case SIGNATURE_32 ('D', 'S', 'D', 'T'):
    case SIGNATURE_32 ('S', 'S', 'D', 'T'):
      return AcpiVerifyUpdateTable (AcpiHeader);
    case SIGNATURE_32 ('M', 'C', 'F', 'G'):
      return AcpiFixupMcfg (AcpiHeader);
  }

  return TRUE;
}

STATIC
VOID
EFIAPI
NotifyEndOfDxeEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS    Status;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  (VOID **)&mAcpiSdtProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AcpiPlatform: Couldn't locate gEfiAcpiSdtProtocolGuid! Status=%r\n", Status));
    return;
  }

  Status = LocateAndInstallAcpiFromFvConditional (&mAcpiTableFile, &AcpiHandleDynamicNamespace);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "AcpiPlatform: Failed to install firmware ACPI as config table. Status=%r\n",
            Status));
  }
}

STATIC
VOID
EFIAPI
AcpiPlatformOsBootHandler (
  IN ACPI_OS_BOOT_TYPE OsType
  )
{
  EFI_STATUS                    Status;
  EFI_ACPI_DESCRIPTION_HEADER   *Table;
  UINTN                         TableKey;
  UINTN                         TableIndex;

  if (mAcpiSdtProtocol == NULL) {
    return;
  }

  TableIndex = 0;
  Status = AcpiLocateTableBySignature (
             mAcpiSdtProtocol,
             EFI_ACPI_6_3_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
             &TableIndex,
             &Table,
             &TableKey);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Couldn't locate ACPI DSDT table! Status=%r\n",
            __func__, Status));
    return;
  }

  //
  // Hide EHCI PNP ID for Windows to avoid binding to the inbox driver,
  // which by default uses atomics on uncached memory and would crash
  // the system.
  //
  if (OsType == AcpiOsWindows) {
    AcpiUpdateSdtNameInteger (Table, "EHID", 0);
  }

  AcpiUpdateChecksum ((UINT8 *)Table, Table->Length);
}

STATIC
UINTN
EFIAPI
FindPeImageBase (
  EFI_PHYSICAL_ADDRESS  Base
  )
{
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;

  Base &= ~(EFI_PAGE_SIZE - 1);

  while (Base != 0) {
    DosHdr = (EFI_IMAGE_DOS_HEADER *)Base;
    if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
      Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)(Base + DosHdr->e_lfanew);
      if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE) {
        break;
      }
    }

    Base -= EFI_PAGE_SIZE;
  }

  return Base;
}

STATIC CHAR8 mWinLoadNameStr[] = "winload";
#define PDB_NAME_MAX_LENGTH   256

STATIC
BOOLEAN
IsPeImageWinLoader (
  IN VOID *PeImage
 )
{
  CHAR8  *PdbStr;
  UINTN  Index;

  PdbStr = (CHAR8 *)PeCoffLoaderGetPdbPointer (PeImage);
  if (PdbStr == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < PDB_NAME_MAX_LENGTH && PdbStr[Index] != '\0'; Index++) {
    if (AsciiStrnCmp (PdbStr + Index, mWinLoadNameStr, sizeof (mWinLoadNameStr) - sizeof (CHAR8)) == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

STATIC
EFI_STATUS
EFIAPI
AcpiPlatformExitBootServicesHook (
  IN EFI_HANDLE  ImageHandle,
  IN UINTN       MapKey
  )
{
  UINTN               ReturnAddress;
  UINTN               OsLoaderAddress;
  ACPI_OS_BOOT_TYPE   OsType;

  ReturnAddress = (UINTN)RETURN_ADDRESS (0);

  gBS->ExitBootServices = mOriginalExitBootServices;

  OsType = AcpiOsUnknown;

  OsLoaderAddress = FindPeImageBase (ReturnAddress);
  if (OsLoaderAddress > 0) {
    if (IsPeImageWinLoader ((VOID *)OsLoaderAddress)) {
      OsType = AcpiOsWindows;
    }
  }

  AcpiPlatformOsBootHandler (OsType);

  return gBS->ExitBootServices (ImageHandle, MapKey);
}

EFI_STATUS
EFIAPI
AcpiPlatformDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     Event;

  if ((PcdGet32 (PcdConfigTableMode) & CONFIG_TABLE_MODE_ACPI) == 0) {
    DEBUG ((DEBUG_WARN, "AcpiPlatform: ACPI support is disabled by the settings.\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,                // Type
                    TPL_CALLBACK,                     // NotifyTpl
                    NotifyEndOfDxeEvent,              // NotifyFunction
                    NULL,                             // NotifyContext
                    &gEfiEndOfDxeEventGroupGuid,      // EventGroup
                    &Event                            // Event
                    );
  ASSERT_EFI_ERROR (Status);

  mOriginalExitBootServices = gBS->ExitBootServices;
  gBS->ExitBootServices = AcpiPlatformExitBootServicesHook;

  return EFI_SUCCESS;
}
