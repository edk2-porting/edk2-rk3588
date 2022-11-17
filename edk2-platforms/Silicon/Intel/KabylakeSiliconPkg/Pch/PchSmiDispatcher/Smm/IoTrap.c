/** @file
  Main implementation source file for the Io Trap SMM driver

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchSmmHelpers.h"
#include <Protocol/PchNvs.h>

#define GENERIC_IOTRAP_SIZE 0x100

//
// Module global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE                      mDriverImageHandle;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE                      mIoTrapHandle;

GLOBAL_REMOVE_IF_UNREFERENCED IO_TRAP_INSTANCE                mIoTrapData;
GLOBAL_REMOVE_IF_UNREFERENCED IO_TRAP_RECORD                  *mIoTrapRecord;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA                    *mPchNvsArea;


static CONST UINT16             mLengthTable[10] = { 1, 2, 3, 4, 8, 16, 32, 64, 128, 256 };

/**
  Helper function that encapsulates IoTrap register access.
  IO trap related register updates must be made in 2 registers, IOTRAP and DMI source decode.

  @param[in] TrapHandlerNum    trap number (0-3)
  @param[in] Value             value to be written in both registers
  @param[in] SaveToBootscript  if true, this register write will be saved to bootscript

**/
VOID
SetIoTrapLowDword (
  IN UINT8   TrapHandlerNum,
  IN UINT32  Value,
  IN BOOLEAN SaveToBootscript
  )
{
  PchPcrWrite32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8, Value);
  PchPcrWrite32 (PID_DMI, R_PCH_PCR_DMI_IOT1 + TrapHandlerNum * 8, Value);
  if (SaveToBootscript) {
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8, 1, &Value);
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_DMI, R_PCH_PCR_DMI_IOT1 + TrapHandlerNum * 8, 1, &Value);
  }
}

/**
  Helper function that encapsulates IoTrap register access.
  IO trap related register updates must be made in 2 registers, IOTRAP and DMI source decode.

  @param[in] TrapHandlerNum    trap number (0-3)
  @param[in] Value             value to be written in both registers
  @param[in] SaveToBootscript  if true, this register write will be saved to bootscript

**/
VOID
SetIoTrapHighDword (
  IN UINT8   TrapHandlerNum,
  IN UINT32  Value,
  IN BOOLEAN SaveToBootscript
  )
{
  PchPcrWrite32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8 + 4, Value);
  PchPcrWrite32 (PID_DMI, R_PCH_PCR_DMI_IOT1 + TrapHandlerNum * 8 + 4, Value);
  if (SaveToBootscript) {
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8 + 4, 1, &Value);
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_DMI, R_PCH_PCR_DMI_IOT1 + TrapHandlerNum * 8 + 4, 1, &Value);
  }
}

/**
  IO resources allocated to IO traps need to be reported to OS so that they don't get reused.
  This function makes IO trap allocation data available to ACPI

  @param[in] TrapHandlerNum  trap number (0-3)
  @param[in] BaseAddress     address of allocated IO resource
  @param[in] Track           TRUE = resource allocated, FALSE = resource freed

**/
VOID
UpdateIoTrapAcpiResources (
  IN UINT8                TrapHandlerNum,
  IN EFI_PHYSICAL_ADDRESS BaseAddress,
  IN BOOLEAN              Track
  )
{

  if (Track == TRUE) {
    mPchNvsArea->IoTrapAddress[TrapHandlerNum] = (UINT16) BaseAddress;
    mPchNvsArea->IoTrapStatus[TrapHandlerNum] = 1;
  } else {
    mPchNvsArea->IoTrapStatus[TrapHandlerNum] = 0;
  }
}

/**
  Register a new IO Trap SMI dispatch function with a parent SMM driver.
  The caller will provide information about the IO trap characteristics via
  the context.  This includes base address, length, read vs. r/w, etc.
  This function will autoallocate IO base address from a common pool if the base address is 0,
  and the RegisterContext Address field will be updated.
  The service will not perform GCD allocation if the base address is non-zero.
  In this case, the caller is responsible for the existence and allocation of the
  specific IO range.
  This function looks for the suitable handler and Register a new IoTrap handler
  if the IO Trap handler is not used. It also enable the IO Trap Range to generate
  SMI.

  @param[in] This                 Pointer to the EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for
                                  this SMI source.
  @param[in, out] RegisterContext Pointer to the dispatch function's context.
                                  The caller fills this context in before calling
                                  the register function to indicate to the register
                                  function the IO trap SMI source for which the dispatch
                                  function should be invoked.  This may not be NULL.
  @param[out] DispatchHandle      Handle of dispatch function, for when interfacing
                                  with the parent SMM driver, will be the address of linked
                                  list link in the call back record.  This may not be NULL.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR        The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources are available
  @retval EFI_INVALID_PARAMETER   Address requested is already in use.
  @retval EFI_ACCESS_DENIED       Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
IoTrapRegister (
  IN CONST  EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL     *This,
  IN        EFI_SMM_HANDLER_ENTRY_POINT2           DispatchFunction,
  IN OUT    EFI_SMM_IO_TRAP_REGISTER_CONTEXT       *RegisterContext,
  OUT       EFI_HANDLE                             *DispatchHandle
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  NextBaseAddress;
  UINT32                NextUsedLength;
  UINT8                 NextTrapHandlerNum;
  EFI_PHYSICAL_ADDRESS  BaseAddress;
  UINT32                UsedLength;
  UINT8                 TrapHandlerNum;
  UINT32                IoTrapRegLowDword;
  UINT32                IoTrapRegHighDword;
  UINT8                 LengthIndex;
  BOOLEAN               TempMergeDisable;

  //
  // Return error if the type is invalid
  //
  if (RegisterContext->Type >= IoTrapTypeMaximum) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Type %0X is invalid! \n", RegisterContext->Type));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Return error if the Length is invalid
  //
  if (RegisterContext->Length < 1 || RegisterContext->Length > GENERIC_IOTRAP_SIZE) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Length %0X is invalid! \n", RegisterContext->Length));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Return error if the address is invalid
  // PCH supports non-aligned address but (Address % 4 + Length) must not be more than 4
  //
  if (((RegisterContext->Length & (RegisterContext->Length - 1)) != 0) && (RegisterContext->Length != 3)) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Length is not power of 2 \n"));
    return EFI_INVALID_PARAMETER;
  }

  if (((RegisterContext->Length >= 4) && (RegisterContext->Address & 0x3)) ||
      ((RegisterContext->Length < 4) && (((RegisterContext->Address & 0x3) + RegisterContext->Length) > 4))) {
    DEBUG ((DEBUG_ERROR, "PCH does not support Dispatch Address %0X and Length %0X combination \n", RegisterContext->Address, RegisterContext->Length));
    return EFI_INVALID_PARAMETER;
  }

  if ((RegisterContext->Length >= 4) && ((RegisterContext->Address & (RegisterContext->Length - 1)) != 0)) {
    DEBUG ((DEBUG_ERROR, "Dispatch Address %0X is not aligned to the Length %0X \n", RegisterContext->Address, RegisterContext->Length));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  if (RegisterContext->Address) {
    TempMergeDisable = TRUE;
  }else {
    TempMergeDisable = FALSE;
  }
  //
  // Loop through the first IO Trap handler, looking for the suitable handler
  //
  for (TrapHandlerNum = 0; TrapHandlerNum < IO_TRAP_HANDLER_NUM; TrapHandlerNum++) {
    //
    // Get information from Io Trap handler register
    //
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8, &IoTrapRegLowDword);

    //
    // Check if the IO Trap handler is not used
    //
    if ((IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD) == 0) {
      //
      //  Search available IO address and allocate it if the IO address is 0
      //
      BaseAddress = RegisterContext->Address;
      if (BaseAddress == 0) {
        //
        // Allocate 256 byte range from GCD for common pool usage
        //
        if ((PcdGet8 (PcdEfiGcdAllocateType) == EfiGcdAllocateMaxAddressSearchBottomUp) || (PcdGet8 (PcdEfiGcdAllocateType) == EfiGcdAllocateMaxAddressSearchTopDown)) {
          BaseAddress = 0xFFFF;
        }
        Status = gDS->AllocateIoSpace (
                        PcdGet8 (PcdEfiGcdAllocateType),
                        EfiGcdIoTypeIo,
                        8,
                        GENERIC_IOTRAP_SIZE,
                        &BaseAddress,
                        mDriverImageHandle,
                        NULL
                        );
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Can't find any available IO address! \n"));
          return EFI_OUT_OF_RESOURCES;
        }

        RegisterContext->Address                    = (UINT16) BaseAddress;
        UsedLength = GENERIC_IOTRAP_SIZE;
        mIoTrapData.TrapUsedLength[TrapHandlerNum] = RegisterContext->Length;
        mIoTrapData.TrackIoResource[TrapHandlerNum] = TRUE;
        UpdateIoTrapAcpiResources (TrapHandlerNum, BaseAddress, TRUE);
      } else {
        BaseAddress &= B_PCH_PCR_PSTH_TRPREG_AD;
        UsedLength = RegisterContext->Length;
      }

      //
      // Register a new IoTrap handler
      //
      Status = PchInternalIoTrapSmiRegister (
                 IoTrapCallback,
                 TrapHandlerNum,
                 &mIoTrapHandle
                 );

      ASSERT_EFI_ERROR (Status);
      mIoTrapData.IoTrapHandle[TrapHandlerNum] = mIoTrapHandle;
      //
      // Fill in the Length, address and Enable the IO Trap SMI
      //
      IoTrapRegLowDword = (UINT32) (((UsedLength - 1) & ~(BIT1 + BIT0)) << 16) |
        (UINT16) BaseAddress |
        B_PCH_PCR_PSTH_TRPREG_TSE;
      if (UsedLength < 4) {
        //
        // The 4 bits is the Byte Enable Mask bits to indicate which byte that are trapped.
        //
        IoTrapRegHighDword  = (((1 << UsedLength) - 1) << ((RegisterContext->Address & 0x3) + N_PCH_PCR_PSTH_TRPREG_BEM)) |
          (UINT32) (RegisterContext->Type << N_PCH_PCR_PSTH_TRPREG_RWIO);
      } else {
        IoTrapRegHighDword  = 0x000000F0 | (UINT32) (RegisterContext->Type << N_PCH_PCR_PSTH_TRPREG_RWIO);
      }
      SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, TRUE);
      SetIoTrapHighDword (TrapHandlerNum, IoTrapRegHighDword, TRUE);
      //
      // Set MergeDisable flag of the registered IoTrap
      //
      mIoTrapData.MergeDisable[TrapHandlerNum] = TempMergeDisable;
    } else {
      //
      // Check next handler if MergeDisable is TRUE or the registered IoTrap if MergeDisable is TRUE
      //
      if ((TempMergeDisable == TRUE) || (mIoTrapData.MergeDisable[TrapHandlerNum] == TRUE)) {
        continue;
      }
      //
      // The IO Trap handler is used, calculate the Length
      //
      UsedLength  = ((IoTrapRegLowDword >> 16) & 0xFC) + 4;
      BaseAddress = IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD;
      //
      //  Assign an addfress from common pool if the caller's address is 0
      //
      if (RegisterContext->Address == 0) {
        //
        //  Check next handler if it's fully used
        //
        if (mIoTrapData.TrapUsedLength[TrapHandlerNum] >= GENERIC_IOTRAP_SIZE) {
          continue;
        }
        //
        // Check next handler if it's not for a common pool
        //
        if (UsedLength < GENERIC_IOTRAP_SIZE) {
          continue;
        }
        //
        // Check next handler if the size is too big
        //
        if (RegisterContext->Length >= (UINT16) GENERIC_IOTRAP_SIZE - mIoTrapData.TrapUsedLength[TrapHandlerNum]) {
          continue;
        }
        //
        // For common pool, we don't need to change the BaseAddress and UsedLength
        //
        RegisterContext->Address = (UINT16) (BaseAddress + mIoTrapData.TrapUsedLength[TrapHandlerNum]);
        mIoTrapData.TrapUsedLength[TrapHandlerNum] += RegisterContext->Length;
      } else {
        //
        // Check next handler if the address is smaller than the IO trap handler's start address
        //
        if (RegisterContext->Address < (UINT16) BaseAddress) {
          continue;
        }
        //
        // Check next handler if the max address is bigger than IO trap handler's range
        //
        if ((RegisterContext->Address + RegisterContext->Length) > (UINT16) (BaseAddress + 256)) {
          continue;
        }
        //
        // If this handler is used for common pool, assert if the caller's address is within the range
        //
        if (mIoTrapData.TrapUsedLength[TrapHandlerNum] != 0) {
          DEBUG ((DEBUG_ERROR, "The Dispatch address %0x is used for common pool! \n", RegisterContext->Address));
          return EFI_INVALID_PARAMETER;
        }
        //
        // Calculate the Length which is maximum use address - start address
        //
        UsedLength = RegisterContext->Address + RegisterContext->Length - (UINT16) BaseAddress;
        //
        // Check the alignment is dword * power of 2 or not
        //
        for (LengthIndex = 0; LengthIndex < sizeof (mLengthTable) / sizeof (UINT16); LengthIndex++) {
          if (UsedLength == mLengthTable[LengthIndex]) {
            break;
          }
        }
        //
        // Check next handler if the alignment is not dword * power of 2
        //
        if (LengthIndex >= sizeof (mLengthTable) / sizeof (UINT16)) {
          continue;
        }

        //
        // Merge the overlap range: remove next Io Trap handler if next Io Trap handler's range is within this handler's range
        //
        for (NextTrapHandlerNum = TrapHandlerNum + 1; NextTrapHandlerNum != TrapHandlerNum; NextTrapHandlerNum++) {
          //
          // Check if NextTrapHandlerNum overflow
          //
          if (NextTrapHandlerNum >= IO_TRAP_HANDLER_NUM) {
            NextTrapHandlerNum = 0;
          }
          //
          // Get information from Io Trap handler register
          //
          PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + NextTrapHandlerNum * 8, &IoTrapRegLowDword);
          //
          // Check next handler if the IO Trap handler is not used
          //
          if ((IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD) == 0) {
            continue;
          }
          //
          // Check if next Io Trap handler's range is within this handler's range
          //
          NextUsedLength  = ((IoTrapRegLowDword >> 16) & 0xFC) + 4;
          NextBaseAddress = IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD;
          if ((BaseAddress > NextBaseAddress) || ((BaseAddress + UsedLength) < (NextBaseAddress + NextUsedLength))) {
            continue;
          }
          //
          // Unregister the IO Trap handler
          //
          mIoTrapHandle = mIoTrapData.IoTrapHandle[NextTrapHandlerNum];
          Status        = PchInternalIoTrapSmiUnRegister (mIoTrapHandle);
          ASSERT_EFI_ERROR (Status);

          //
          // Clear the Io Trap handler register
          //
          SetIoTrapHighDword (NextTrapHandlerNum, 0, TRUE);
          SetIoTrapLowDword (NextTrapHandlerNum, 0, TRUE);
        }

        ///
        /// Update the Length
        ///
        IoTrapRegLowDword = (UINT32) (((UsedLength - 1) & ~(BIT1 + BIT0)) << 16) |
          (UINT16) BaseAddress |
          B_PCH_PCR_PSTH_TRPREG_TSE;
        SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, TRUE);
      }
      //
      // Only set RWM bit when we need both read and write cycles.
      //
      PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8 + 4, &IoTrapRegHighDword);
      if ((IoTrapRegHighDword & B_PCH_PCR_PSTH_TRPREG_RWM) == 0 &&
          (UINT32) ((IoTrapRegHighDword & B_PCH_PCR_PSTH_TRPREG_RWIO) >> N_PCH_PCR_PSTH_TRPREG_RWIO) !=
          (UINT32) RegisterContext->Type) {
        IoTrapRegHighDword = ((IoTrapRegHighDword | B_PCH_PCR_PSTH_TRPREG_RWM) & ~B_PCH_PCR_PSTH_TRPREG_RWIO);
        SetIoTrapHighDword (TrapHandlerNum, IoTrapRegHighDword, TRUE);
      }
    }
    break;
  }

  if (TrapHandlerNum >= IO_TRAP_HANDLER_NUM) {
    DEBUG ((DEBUG_ERROR, "All IO Trap handler is used, no available IO Trap handler! \n"));
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Create database record and add to database
  //
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (IO_TRAP_RECORD),
                    (VOID **) &mIoTrapRecord
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for mIoTrapRecord! \n"));
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Gather information about the registration request
  //
  mIoTrapRecord->Signature  = IO_TRAP_RECORD_SIGNATURE;
  mIoTrapRecord->Context    = *RegisterContext;
  mIoTrapRecord->Callback   = DispatchFunction;

  InsertTailList (&mIoTrapData.CallbackDataBase, &mIoTrapRecord->Link);

  //
  // Child's handle will be the address linked list link in the record
  //
  *DispatchHandle = (EFI_HANDLE) (&mIoTrapRecord->Link);

  DEBUG ((DEBUG_INFO, "RegisterContext->Address:%x! \n", RegisterContext->Address));
  DEBUG ((DEBUG_INFO, "RegisterContext->Length:%x! \n", RegisterContext->Length));

  SmiHandlerProfileRegisterHandler (&gEfiSmmIoTrapDispatch2ProtocolGuid, DispatchFunction, (UINTN)RETURN_ADDRESS (0), RegisterContext, sizeof(*RegisterContext));

  return EFI_SUCCESS;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
  @retval EFI_ACCESS_DENIED       Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
IoTrapUnRegister (
  IN CONST EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL    *This,
  IN EFI_HANDLE                                  DispatchHandle
  )
{
  EFI_STATUS            Status;
  IO_TRAP_RECORD        *RecordToDelete;
  UINT32                IoTrapRegLowDword;
  UINT32                IoTrapRegHighDword;
  EFI_PHYSICAL_ADDRESS  BaseAddress;
  UINT32                UsedLength;
  UINT8                 TrapHandlerNum;
  UINT8                 LengthIndex;

  if (DispatchHandle == 0) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "UnRegister is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  RecordToDelete = IO_TRAP_RECORD_FROM_LINK (DispatchHandle);
  //
  // Take the entry out of the linked list
  //
  if (RecordToDelete->Link.ForwardLink == (LIST_ENTRY *) EFI_BAD_POINTER) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Loop through the first IO Trap handler, looking for the suitable handler
  //
  for (TrapHandlerNum = 0; TrapHandlerNum < IO_TRAP_HANDLER_NUM; TrapHandlerNum++) {
    //
    // Get information from Io Trap handler register
    //
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8, &IoTrapRegLowDword);

    //
    // Check next Io Trap handler if the IO Trap handler is not used
    //
    if ((IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD) == 0) {
      continue;
    }

    UsedLength  = ((IoTrapRegLowDword >> 16) & 0xFC) + 4;
    BaseAddress = IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD;

    //
    // Check if it's the maximum address of the Io Trap handler
    //
    if ((UINTN)(BaseAddress + UsedLength) == (UINTN)(RecordToDelete->Context.Address + RecordToDelete->Context.Length)) {

      if (BaseAddress == RecordToDelete->Context.Address) {
        //
        // Disable the IO Trap handler if it's the only child of the Trap handler
        //
        mIoTrapHandle = mIoTrapData.IoTrapHandle[TrapHandlerNum];
        Status        = PchInternalIoTrapSmiUnRegister (mIoTrapHandle);
        ASSERT_EFI_ERROR (Status);

        mIoTrapData.TrackIoResource[TrapHandlerNum] = FALSE;
        //
        // Clear the Io Trap handler register
        //
        IoTrapRegLowDword   = 0;
        IoTrapRegHighDword  = 0;
        SetIoTrapHighDword (TrapHandlerNum, IoTrapRegHighDword, TRUE);
        if (mIoTrapData.TrackIoResource[TrapHandlerNum] == TRUE) {
          mIoTrapData.TrackIoResource[TrapHandlerNum] = FALSE;
          gDS->FreeIoSpace (BaseAddress, GENERIC_IOTRAP_SIZE);
          UpdateIoTrapAcpiResources (TrapHandlerNum, BaseAddress, FALSE);
        }

      } else {
        //
        // Calculate the new IO Trap handler Length
        //
        UsedLength = UsedLength - RecordToDelete->Context.Length;
        //
        // Check the alignment is dword * power of 2 or not
        //
        for (LengthIndex = 0; LengthIndex < sizeof (mLengthTable) / sizeof (UINT16); LengthIndex++) {
          if (UsedLength == mLengthTable[LengthIndex]) {
            break;
          }
        }
        //
        // Do not decrease the length if the alignment is not dword * power of 2
        //
        if (LengthIndex >= sizeof (mLengthTable) / sizeof (UINT16)) {
          break;
        }
        //
        // Decrease the length to prevent the IO trap SMI
        //
        IoTrapRegLowDword = (UINT32) ((((UsedLength - 1) &~(BIT1 + BIT0)) << 16) | BaseAddress | B_PCH_PCR_PSTH_TRPREG_TSE);
      }

      SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, TRUE);
      break;
    }
  }

  SmiHandlerProfileUnregisterHandler (&gEfiSmmIoTrapDispatch2ProtocolGuid, RecordToDelete->Callback, &RecordToDelete->Context, sizeof(RecordToDelete->Context));

  RemoveEntryList (&RecordToDelete->Link);
  Status = gSmst->SmmFreePool (RecordToDelete);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  This I/O Trap SMI handler invokes the ACPI reference code to handle the SMI.
  It currently assumes it owns all of the IO trap SMI.

  @param[in] DispatchHandle             Not used

**/
VOID
EFIAPI
IoTrapCallback (
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  IO_TRAP_RECORD                            *RecordInDb;
  LIST_ENTRY                                *LinkInDb;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT          CurrentIoTrapRegisterData;
  EFI_SMM_IO_TRAP_CONTEXT                   CurrentIoTrapContextData;
  UINT16                                    BaseAddress;
  UINT16                                    StartAddress;
  UINT16                                    EndAddress;
  UINT32                                    Data32;

  if (!IsListEmpty (&mIoTrapData.CallbackDataBase)) {
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPC, &Data32);
    BaseAddress   = (UINT16) (Data32 & B_PCH_PCR_PSTH_TRPC_IOA);
    StartAddress  = (Data32 & B_PCH_PCR_PSTH_TRPC_AHBE) >> 16;
    //
    // StartAddress and EndAddress will be equal if it's byte access
    //
    EndAddress    = (UINT16) (HighBitSet32 ((UINT32) (StartAddress))) + BaseAddress;
    StartAddress  = (UINT16) (LowBitSet32 ((UINT32) (StartAddress))) + BaseAddress;

    CurrentIoTrapRegisterData.Type     = (BOOLEAN) ((Data32 & B_PCH_PCR_PSTH_TRPC_RW) != 0);
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPD, &Data32);
    CurrentIoTrapContextData.WriteData = Data32;
    LinkInDb                    = GetFirstNode (&mIoTrapData.CallbackDataBase);

    while (!IsNull (&mIoTrapData.CallbackDataBase, LinkInDb)) {
      RecordInDb = IO_TRAP_RECORD_FROM_LINK (LinkInDb);
      if ((RecordInDb->Context.Address <= StartAddress) &&
          (RecordInDb->Context.Address + RecordInDb->Context.Length > EndAddress)) {
        if (RecordInDb->Context.Type == ReadWriteTrap || RecordInDb->Context.Type == CurrentIoTrapRegisterData.Type) {
          //
          // Pass the IO trap context information
          //
          RecordInDb->Callback (&RecordInDb->Link, &CurrentIoTrapContextData,NULL,NULL);
        }
        //
        // Break if the address is match
        //
        break;
      } else {
        LinkInDb = GetNextNode (&mIoTrapData.CallbackDataBase, &RecordInDb->Link);
        if (IsNull (&mIoTrapData.CallbackDataBase, LinkInDb)) {
          //
          // An IO access was trapped that does not have a handler registered.
          // This indicates an error condition.
          //
          ASSERT (FALSE);
        }
      }
    }
  }
}

/**
  Pause IoTrap callback function.

  This function disables the SMI enable of IoTrap according to the DispatchHandle,
  which is returned by IoTrap callback registration. It only supports the DispatchHandle
  with MergeDisable TRUE and address not zero.

  @param[in] This                 Pointer to the PCH_SMM_IO_TRAP_CONTROL_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of the child service to change state.

  @retval EFI_SUCCESS             This operation is complete.
  @retval EFI_INVALID_PARAMETER   The DispatchHandle is invalid.
  @retval EFI_ACCESS_DENIED       The SMI status is alrady PAUSED.
**/
EFI_STATUS
EFIAPI
IoTrapControlPause (
  IN PCH_SMM_IO_TRAP_CONTROL_PROTOCOL   *This,
  IN EFI_HANDLE                         DispatchHandle
  )
{
  IO_TRAP_RECORD                        *IoTrapRecord;
  UINT32                                IoTrapRegLowDword;
  UINT32                                IoTrapRegHighDword;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT32                                UsedLength;
  UINT8                                 TrapHandlerNum;
  BOOLEAN                               TempMergeDisable;

  if (DispatchHandle == 0) {
    return EFI_INVALID_PARAMETER;
  }

  IoTrapRecord = IO_TRAP_RECORD_FROM_LINK (DispatchHandle);

  if (IoTrapRecord->Context.Address) {
    TempMergeDisable =TRUE;
  }else {
    TempMergeDisable = FALSE;
  }

  if ((IoTrapRecord->Signature != IO_TRAP_RECORD_SIGNATURE) ||
      (TempMergeDisable != TRUE)                            ||
      (IoTrapRecord->Context.Address == 0)                  ||
      (IoTrapRecord->Context.Length == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  for (TrapHandlerNum = 0; TrapHandlerNum < IO_TRAP_HANDLER_NUM; TrapHandlerNum++) {
    //
    // This IoTrap register should be merge disabled.
    //
    if (mIoTrapData.MergeDisable[TrapHandlerNum] != TRUE) {
      continue;
    }
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8, &IoTrapRegLowDword);
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8 + 4, &IoTrapRegHighDword);
    //
    // Depending on the usage, we will obtain the UsedLength and BaseAddress differently
    // If the registered trap length is less than 4, we obtain the length from Byte Enable Mask
    // In the other hand, we obtain the length from Address Mask
    //
    if ((IoTrapRegHighDword & 0xF0) != 0xF0) {
      UsedLength = (UINT32) (HighBitSet32 (IoTrapRegHighDword & 0xF0) - LowBitSet32 (IoTrapRegHighDword & 0xF0) + 1);
      BaseAddress = (IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD) + LowBitSet32 ((IoTrapRegHighDword & 0xF0) >> 4);
    } else {
      UsedLength  = ((IoTrapRegLowDword >> 16) & 0xFC) + 4;
      BaseAddress = IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD;
    }

    //
    // The address and length of record matches the IoTrap register's.
    //
    if ((BaseAddress == IoTrapRecord->Context.Address) &&
        (UsedLength  == IoTrapRecord->Context.Length )) {
      //
      // Check if status matched.
      // If this is already Paused, return warning status.
      //
      if ((IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_TSE) == 0) {
        return EFI_ACCESS_DENIED;
      }
      //
      // Clear IoTrap register SMI enable bit
      //
      IoTrapRegLowDword &= (~B_PCH_PCR_PSTH_TRPREG_TSE);
      SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, FALSE);
      return EFI_SUCCESS;
    }
  }
  return EFI_INVALID_PARAMETER;
}

/**
  Resume IoTrap callback function.

  This function enables the SMI enable of IoTrap according to the DispatchHandle,
  which is returned by IoTrap callback registration. It only supports the DispatchHandle
  with MergeDisable TRUE and address not zero.

  @param[in] This                 Pointer to the PCH_SMM_IO_TRAP_CONTROL_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of the child service to change state.

  @retval EFI_SUCCESS             This operation is complete.
  @retval EFI_INVALID_PARAMETER   The DispatchHandle is invalid.
  @retval EFI_ACCESS_DENIED       The SMI status is alrady RESUMED.
**/
EFI_STATUS
EFIAPI
IoTrapControlResume (
  IN PCH_SMM_IO_TRAP_CONTROL_PROTOCOL   *This,
  IN EFI_HANDLE                         DispatchHandle
  )
{
  IO_TRAP_RECORD                        *IoTrapRecord;
  UINT32                                IoTrapRegLowDword;
  UINT32                                IoTrapRegHighDword;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT32                                UsedLength;
  UINT8                                 TrapHandlerNum;
  BOOLEAN                               TempMergeDisable;

  if (DispatchHandle == 0) {
    return EFI_INVALID_PARAMETER;
  }
  IoTrapRecord = IO_TRAP_RECORD_FROM_LINK (DispatchHandle);

  if (IoTrapRecord->Context.Address) {
    TempMergeDisable = TRUE;
  }else {
    TempMergeDisable = FALSE;
  }

  if ((IoTrapRecord->Signature != IO_TRAP_RECORD_SIGNATURE) ||
      (TempMergeDisable != TRUE)          ||
      (IoTrapRecord->Context.Address == 0)                  ||
      (IoTrapRecord->Context.Length == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  for (TrapHandlerNum = 0; TrapHandlerNum < IO_TRAP_HANDLER_NUM; TrapHandlerNum++) {
    //
    // This IoTrap register should be merge disabled.
    //
    if (mIoTrapData.MergeDisable[TrapHandlerNum] != TRUE) {
      continue;
    }
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8, &IoTrapRegLowDword);
    PchPcrRead32 (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0 + TrapHandlerNum * 8 + 4, &IoTrapRegHighDword);
    //
    // Depending on the usage, we will obtain the UsedLength and BaseAddress differently
    // If the registered trap length is less than 4, we obtain the length from Byte Enable Mask
    // In the other hand, we obtain the length from Address Mask
    //
    if ((IoTrapRegHighDword & 0xF0) != 0xF0) {
      UsedLength  = (UINT32) (HighBitSet32 (IoTrapRegHighDword & 0xF0) - LowBitSet32 (IoTrapRegHighDword & 0xF0) + 1);
      BaseAddress = (IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD) + LowBitSet32 ((IoTrapRegHighDword & 0xF0) >> 4);
    } else {
      UsedLength  = ((IoTrapRegLowDword >> 16) & 0xFC) + 4;
      BaseAddress = IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_AD;
    }

    //
    // The address and length of record matches the IoTrap register's.
    //
    if ((BaseAddress == IoTrapRecord->Context.Address) &&
        (UsedLength  == IoTrapRecord->Context.Length )) {
      //
      // Check if status matched.
      // If this is already Resume, return warning status.
      //
      if ((IoTrapRegLowDword & B_PCH_PCR_PSTH_TRPREG_TSE) != 0) {
        return EFI_ACCESS_DENIED;
      }
      //
      // Set IoTrap register SMI enable bit
      //
      IoTrapRegLowDword |= (B_PCH_PCR_PSTH_TRPREG_TSE);
      SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, FALSE);
      return EFI_SUCCESS;
    }
  }
  return EFI_INVALID_PARAMETER;
}

/**
  The IoTrap module abstracts PCH I/O trapping capabilities for other drivers.
  This driver manages the limited I/O trap resources.

  @param[in] ImageHandle                Image handle for this driver image

  @retval EFI_SUCCESS                   Driver initialization completed successfully
**/
EFI_STATUS
EFIAPI
InstallIoTrap (
  IN EFI_HANDLE                         ImageHandle
  )
{
  EFI_STATUS             Status;
  PCH_NVS_AREA_PROTOCOL  *PchNvsAreaProtocol;


  //
  // Initialize the EFI SMM driver library
  //
  mDriverImageHandle = ImageHandle;

  //
  // Initialize the IO TRAP protocol we produce
  //
  mIoTrapData.Signature = IO_TRAP_INSTANCE_SIGNATURE;
  mIoTrapData.EfiSmmIoTrapDispatchProtocol.Register   = IoTrapRegister;
  mIoTrapData.EfiSmmIoTrapDispatchProtocol.UnRegister = IoTrapUnRegister;

  //
  // Initialize the IO TRAP control protocol.
  //
  mIoTrapData.PchSmmIoTrapControlProtocol.Pause       = IoTrapControlPause;
  mIoTrapData.PchSmmIoTrapControlProtocol.Resume      = IoTrapControlResume;

  //
  // Initialize IO TRAP Callback DataBase
  //
  InitializeListHead (&mIoTrapData.CallbackDataBase);

  //
  // Get address of PchNvs structure for later use
  //
  Status = gBS->LocateProtocol (&gPchNvsAreaProtocolGuid, NULL, (VOID **) &PchNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  mPchNvsArea = PchNvsAreaProtocol->Area;

  //
  // Install protocol interface
  //
  mIoTrapData.Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &mIoTrapData.Handle,
                    &gEfiSmmIoTrapDispatch2ProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mIoTrapData.EfiSmmIoTrapDispatchProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmInstallProtocolInterface (
                    &mIoTrapData.Handle,
                    &gPchSmmIoTrapControlGuid,
                    EFI_NATIVE_INTERFACE,
                    &mIoTrapData.PchSmmIoTrapControlProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
