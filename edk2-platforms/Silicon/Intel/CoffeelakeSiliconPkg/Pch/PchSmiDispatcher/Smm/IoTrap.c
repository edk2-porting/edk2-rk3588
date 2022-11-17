/** @file
  Main implementation source file for the Io Trap SMM driver

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchSmmHelpers.h"
#include <Private/Protocol/PchNvsArea.h>
#include <Library/SmiHandlerProfileLib.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsPsth.h>
#include <Register/PchRegsDmi.h>

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
  UINT32  BitMask;
  UINT32  BitValue;
  //
  // To provide sequentially consistent programming model for IO trap
  // all pending IO cycles must be flushed before enabling and before disabling a trap.
  // Without this the trap may trigger due to IO cycle issued before the trap is enabled or a cycle issued before the trap is disabled might be missed.
  // a. Issues a MemRd to PSTH IO Trap Enable bit -> This serves to flush all previous IO cycles.
  // b. Then only issues a MemWr to PSTH IO Trap Enable == Value
  // c. Issues another MemRd to PSTH IO Trap Enable bit -> This serves to push the MemWr to PSTH and confirmed that IO Trap is in fact enabled
  //
  PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8);
  PchPcrWrite32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8, Value);
  PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8);

  PchPcrWrite32 (PID_DMI, R_PCH_DMI_PCR_IOT1 + TrapHandlerNum * 8, Value);
  //
  // Read back DMI IOTRAP register to enforce ordering so DMI write is completed before any IO reads
  // from other threads which may occur after this point (after SMI exit).
  //
  PchPcrRead32 (PID_DMI, R_PCH_DMI_PCR_IOT1 + TrapHandlerNum * 8);
  if (SaveToBootscript) {
    //
    // Ignore the value check of PCH_PCR_BOOT_SCRIPT_READ
    //
    BitMask  = 0;
    BitValue = 0;

    PCH_PCR_BOOT_SCRIPT_READ (S3BootScriptWidthUint32, PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8, &BitMask, &BitValue);
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8, 1, &Value);
    PCH_PCR_BOOT_SCRIPT_READ (S3BootScriptWidthUint32, PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8, &BitMask, &BitValue);
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_DMI, R_PCH_DMI_PCR_IOT1 + TrapHandlerNum * 8, 1, &Value);
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
  PchPcrWrite32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8 + 4, Value);
  PchPcrWrite32 (PID_DMI, R_PCH_DMI_PCR_IOT1 + TrapHandlerNum * 8 + 4, Value);
  if (SaveToBootscript) {
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8 + 4, 1, &Value);
    PCH_PCR_BOOT_SCRIPT_WRITE (S3BootScriptWidthUint32, PID_DMI, R_PCH_DMI_PCR_IOT1 + TrapHandlerNum * 8 + 4, 1, &Value);
  }
}

/**
  Clear pending IOTRAP status.
  If IOTRAP status is pending and IOTRAP is disabled, then BIOS will not find a match SMI source
  and will not dispatch any SMI handler for it. The pending status will lead to SMI storm.
  This prevents that IOTRAP gets triggered by pending IO cycles even after it's disabled.

  @param[in] TrapHandlerNum    trap number (0-3)

**/
VOID
ClearPendingIoTrapStatus (
  IN UINT8   TrapHandlerNum
  )
{
  PchPcrWrite32 (PID_PSTH, R_PSTH_PCR_TRPST, (UINT32)(1 << TrapHandlerNum));
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
  Get address from IOTRAP low dword.

  @param[in] IoTrapRegLowDword    IOTRAP register low dword

  @retval                         Address of IOTRAP setting.
**/
STATIC
UINT16
AddressFromLowDword (
  UINT32  IoTrapRegLowDword
  )
{
  return (UINT16) (IoTrapRegLowDword & B_PSTH_PCR_TRPREG_AD);
}

/**
  Get length from IOTRAP low dword.

  @param[in] IoTrapRegLowDword    IOTRAP register low dword

  @retval                         Length of IOTRAP setting.
**/
STATIC
UINT16
LengthFromLowDword (
  UINT32  IoTrapRegLowDword
  )
{
  return (UINT16) (((IoTrapRegLowDword >> 16) & 0xFC) + 4);
}

/**
  Get ByteEnable from IOTRAP high dword.

  @param[in] IoTrapRegHighDword   IOTRAP register high dword

  @retval                         ByteEnable of IOTRAP setting.
**/
STATIC
UINT8
ByteEnableFromHighDword (
  UINT32  IoTrapRegHighDword
  )
{
  return (UINT8) (IoTrapRegHighDword & 0x0F);
}

/**
  Get ByteEnableMask from IOTRAP high dword.

  @param[in] IoTrapRegHighDword   IOTRAP register high dword

  @retval                         ByteEnableMask of IOTRAP setting.
**/
STATIC
UINT8
ByteEnableMaskFromHighDword (
  UINT32  IoTrapRegHighDword
  )
{
  return (UINT8) ((IoTrapRegHighDword & 0xF0) >> 4);
}

/**
  Check the IoTrap register matches the IOTRAP EX content.

  @param[in] IoTrapRecord         IOTRAP registration record structure
  @param[in] IoTrapRegLowDword    IOTRAP register low dword
  @param[in] IoTrapRegHighDword   IOTRAP register high dword

  @retval    TRUE                 Content matched
             FALSE                Content mismatched
**/
STATIC
BOOLEAN
IsIoTrapExContentMatched (
  IO_TRAP_RECORD  *IoTrapRecord,
  UINT32          IoTrapRegLowDword,
  UINT32          IoTrapRegHighDword
  )
{
  if ((IoTrapRecord->Context.Address == AddressFromLowDword (IoTrapRegLowDword))          &&
      (IoTrapRecord->Context.Length == LengthFromLowDword (IoTrapRegLowDword))            &&
      (IoTrapRecord->Context.ByteEnable == ByteEnableFromHighDword (IoTrapRegHighDword))  &&
      (IoTrapRecord->Context.ByteEnableMask == ByteEnableMaskFromHighDword (IoTrapRegHighDword)))
  {
    return TRUE;
  }
  return FALSE;
}


/**
  The helper function for IoTrap callback dispacther

  @param[in] TrapHandlerNum  trap number (0-3)
**/
VOID
IoTrapDispatcherHelper (
  UINTN                                     TrapHandlerNum
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
  UINT8                                     ActiveHighByteEnable;
  BOOLEAN                                   ReadCycle;
  UINT32                                    WriteData;

  if (!IsListEmpty (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase))) {
    Data32 = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPC);
    WriteData = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPD);

    BaseAddress           = (UINT16) (Data32 & B_PSTH_PCR_TRPC_IOA);
    ActiveHighByteEnable  = (UINT8)((Data32 & B_PSTH_PCR_TRPC_AHBE) >> 16);
    ReadCycle             = (BOOLEAN) ((Data32 & B_PSTH_PCR_TRPC_RW) == B_PSTH_PCR_TRPC_RW);
    //
    // StartAddress and EndAddress will be equal if it's byte access
    //
    EndAddress    = (UINT16) (HighBitSet32 ((UINT32) (ActiveHighByteEnable))) + BaseAddress;
    StartAddress  = (UINT16) (LowBitSet32 ((UINT32) (ActiveHighByteEnable))) + BaseAddress;

    CurrentIoTrapRegisterData.Type = (EFI_SMM_IO_TRAP_DISPATCH_TYPE)ReadCycle;
    CurrentIoTrapContextData.WriteData = WriteData;

    LinkInDb = GetFirstNode (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase));

    while (!IsNull (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase), LinkInDb)) {
      RecordInDb = IO_TRAP_RECORD_FROM_LINK (LinkInDb);

      //
      // If MergeDisable is TRUE, no need to check the address range, dispatch the callback function directly.
      //
      if (mIoTrapData.Entry[TrapHandlerNum].MergeDisable) {
        if (RecordInDb->IoTrapCallback != NULL) {
          RecordInDb->IoTrapCallback (&RecordInDb->Link, &CurrentIoTrapContextData, NULL, NULL);
        }
        if (RecordInDb->IoTrapExCallback != NULL) {
          RecordInDb->IoTrapExCallback (BaseAddress, ActiveHighByteEnable, !ReadCycle, WriteData);
        }
        //
        // Expect only one callback available. So break immediately.
        //
        break;
      //
      // If MergeDisable is FALSE, check the address range and trap type.
      //
      } else {
        if ((RecordInDb->Context.Address <= StartAddress) &&
            (RecordInDb->Context.Address + RecordInDb->Context.Length > EndAddress)) {
          if ((RecordInDb->Context.Type == IoTrapExTypeReadWrite) || (RecordInDb->Context.Type == (IO_TRAP_EX_DISPATCH_TYPE) CurrentIoTrapRegisterData.Type)) {
            //
            // Pass the IO trap context information
            //
            RecordInDb->IoTrapCallback (&RecordInDb->Link, &CurrentIoTrapContextData, NULL, NULL);
          }
          //
          // Break if the address is match
          //
          break;
        } else {
          LinkInDb = GetNextNode (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase), &RecordInDb->Link);
          if (IsNull (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase), LinkInDb)) {
            //
            // An IO access was trapped that does not have a handler registered.
            // This indicates an error condition.
            //
            ASSERT (FALSE);
          }
        }
      } // end of if else block
    } // end of while loop
  } // end of if else block
}

/**
  IoTrap dispatcher for IoTrap register 0.

  @param[in] DispatchHandle             Handle of dispatch function
**/
VOID
EFIAPI
IoTrapDispatcher0 (
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  IoTrapDispatcherHelper (0);
}

/**
  IoTrap dispatcher for IoTrap register 1.

  @param[in] DispatchHandle             Handle of dispatch function
**/
VOID
EFIAPI
IoTrapDispatcher1 (
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  IoTrapDispatcherHelper (1);
}

/**
  IoTrap dispatcher for IoTrap register 2.

  @param[in] DispatchHandle             Handle of dispatch function
**/
VOID
EFIAPI
IoTrapDispatcher2 (
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  IoTrapDispatcherHelper (2);
}

/**
  IoTrap dispatcher for IoTrap register 3.

  @param[in] DispatchHandle             Handle of dispatch function
**/
VOID
EFIAPI
IoTrapDispatcher3 (
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  IoTrapDispatcherHelper (3);
}

/**
  IoTrap registratrion helper fucntion.

  @param[in] DispatchHandle             Handle of dispatch function
  @param[in] IoTrapDispatchFunction     Dispatch function of IoTrapDispatch2Protocol.
                                        This could be NULL if it's not from IoTrapDispatch2Protocol.
  @param[in] IoTrapExDispatchFunction   Dispatch function of IoTrapExDispatchProtocol.
                                        This could be NULL if it's not from IoTrapExDispatchProtocol.
  @param[in out] Address                The pointer of IO Address.
                                        If the input Addres is 0, it will return the address assigned
                                        by registration to this caller.
  @param[in] Length                     Length of IO address range.
  @param[in] Type                       Read/Write type of IO trap.
  @param[in] ByteEnable                 Bitmap to enable trap for each byte of every dword alignment address.
  @param[in] ByteEnableMask             ByteEnableMask bitwise to ignore the ByteEnable setting.

  @retval    EFI_INVALID_PARAMETER      If Type is invalid,
                                        If Length is invalid,
                                        If Address is invalid,
             EFI_ACCESS_DENIED          If the SmmReadyToLock event has been triggered,
             EFI_OUT_OF_RESOURCES       If run out of IoTrap register resource,
                                        If run out of SMM memory pool,
             EFI_SUCCESS                IoTrap register successfully.
**/
EFI_STATUS
IoTrapRegisterHelper (
  OUT       EFI_HANDLE                             *DispatchHandle,
  IN        EFI_SMM_HANDLER_ENTRY_POINT2           IoTrapDispatchFunction,
  IN        IO_TRAP_EX_DISPATCH_CALLBACK           IoTrapExDispatchFunction,
  IN OUT    UINT16                                 *Address,
  IN        UINT16                                 Length,
  IN        IO_TRAP_EX_DISPATCH_TYPE               Type,
  IN        UINT8                                  ByteEnable,
  IN        UINT8                                  ByteEnableMask
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  BaseAddress;
  UINT32                UsedLength;
  UINT8                 TrapHandlerNum;
  UINT32                IoTrapRegLowDword;
  UINT32                IoTrapRegHighDword;
  BOOLEAN               TempMergeDisable;

  DEBUG ((DEBUG_INFO, "IoTrapRegisterHelper\n"));
  DEBUG ((DEBUG_INFO, "Address:%x \n", *Address));
  DEBUG ((DEBUG_INFO, "Length:%x \n", Length));
  DEBUG ((DEBUG_INFO, "Type:%x \n", Type));
  DEBUG ((DEBUG_INFO, "ByteEnable:%x \n", ByteEnable));
  DEBUG ((DEBUG_INFO, "ByteEnableMask:%x \n", ByteEnableMask));

  //
  // Return error if the type is invalid
  //
  if (Type >= IoTrapExTypeMaximum) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Type %0X is invalid! \n", Type));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Return error if the Length is invalid
  //
  if (Length < 1 || Length > GENERIC_IOTRAP_SIZE) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Length %0X is invalid! \n", Length));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Return error if the address is invalid
  // PCH supports non-aligned address but (Address % 4 + Length) must not be more than 4
  //
  if (((Length & (Length - 1)) != 0) && (Length != 3)) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Length is not power of 2 \n"));
    return EFI_INVALID_PARAMETER;
  }

  if (((Length >= 4) && (*Address & 0x3)) ||
      ((Length < 4) && (((*Address & 0x3) + Length) > 4))) {
    DEBUG ((DEBUG_ERROR, "PCH does not support Dispatch Address %0X and Length %0X combination \n", *Address, Length));
    return EFI_INVALID_PARAMETER;
  }

  if ((Length >= 4) && ((*Address & (Length - 1)) != 0)) {
    DEBUG ((DEBUG_ERROR, "Dispatch Address %0X is not aligned to the Length %0X \n", *Address, Length));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  if (*Address) {
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
    IoTrapRegLowDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8);

    //
    // Check if the IO Trap handler is not used
    //
    if (AddressFromLowDword (IoTrapRegLowDword) == 0) {
      //
      //  Search available IO address and allocate it if the IO address is 0
      //
      BaseAddress = *Address;
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

        *Address   = (UINT16) BaseAddress;
        UsedLength = GENERIC_IOTRAP_SIZE;
        mIoTrapData.Entry[TrapHandlerNum].TrapUsedLength = Length;
        mIoTrapData.Entry[TrapHandlerNum].ReservedAcpiIoResource = TRUE;
        UpdateIoTrapAcpiResources (TrapHandlerNum, BaseAddress, TRUE);
      } else {
        BaseAddress &= B_PSTH_PCR_TRPREG_AD;
        UsedLength = Length;
      }

      Status = PchInternalIoTrapSmiRegister (
                 mIoTrapData.Entry[TrapHandlerNum].CallbackDispatcher,
                 TrapHandlerNum,
                 &mIoTrapHandle
                 );

      ASSERT_EFI_ERROR (Status);
      mIoTrapData.Entry[TrapHandlerNum].IoTrapHandle = mIoTrapHandle;

      //
      // Fill in the Length, address and Enable the IO Trap SMI
      //
      IoTrapRegLowDword = (UINT32) (((UsedLength - 1) & ~(BIT1 + BIT0)) << 16) |
        (UINT16) BaseAddress |
        B_PSTH_PCR_TRPREG_TSE;

      if (UsedLength < 4) {
        //
        // The 4 bits is the Byte Enable Mask bits to indicate which byte that are trapped.
        // The input ByteEnable and ByteEnableMask are ignored in this case.
        //
        IoTrapRegHighDword  = (((1 << UsedLength) - 1) << ((*Address & 0x3) + (N_PSTH_PCR_TRPREG_BEM - 32))) |
          (UINT32) (Type << N_PSTH_PCR_TRPREG_RWIO);
      } else {
        //
        // Fill in the ByteEnable, ByteEnableMask, and Type of Io Trap register
        //
        IoTrapRegHighDword  = ((ByteEnableMask & 0xF) << (N_PSTH_PCR_TRPREG_BEM - 32)) |
          ((ByteEnable & 0xF) << (N_PSTH_PCR_TRPREG_BE - 32)) |
          (UINT32) (Type << N_PSTH_PCR_TRPREG_RWIO);
      }
      SetIoTrapHighDword (TrapHandlerNum, IoTrapRegHighDword, TRUE);
      SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, TRUE);
      //
      // Set MergeDisable flag of the registered IoTrap
      //
      mIoTrapData.Entry[TrapHandlerNum].MergeDisable = TempMergeDisable;
    } else {
      //
      // Check next handler if MergeDisable is TRUE or the registered IoTrap if MergeDisable is TRUE
      // If the Io Trap register is used by IoTrapEx protocol, then the MergeDisable will be FALSE.
      //
      if ((TempMergeDisable == TRUE) || (mIoTrapData.Entry[TrapHandlerNum].MergeDisable == TRUE)) {
        continue;
      }
      //
      // The IO Trap handler is used, calculate the Length
      //
      UsedLength  = LengthFromLowDword (IoTrapRegLowDword);
      BaseAddress = AddressFromLowDword (IoTrapRegLowDword);
      //
      //  Assign an addfress from common pool if the caller's address is 0
      //
      if (*Address == 0) {
        //
        //  Check next handler if it's fully used
        //
        if (mIoTrapData.Entry[TrapHandlerNum].TrapUsedLength >= GENERIC_IOTRAP_SIZE) {
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
        if (Length >= (UINT16) GENERIC_IOTRAP_SIZE - mIoTrapData.Entry[TrapHandlerNum].TrapUsedLength) {
          continue;
        }
        //
        // For common pool, we don't need to change the BaseAddress and UsedLength
        //
        *Address = (UINT16) (BaseAddress + mIoTrapData.Entry[TrapHandlerNum].TrapUsedLength);
        mIoTrapData.Entry[TrapHandlerNum].TrapUsedLength += Length;
      }
      //
      // Only set RWM bit when we need both read and write cycles.
      //
      IoTrapRegHighDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8 + 4);
      if ((IoTrapRegHighDword & B_PSTH_PCR_TRPREG_RWM) == 0 &&
          (UINT32) ((IoTrapRegHighDword & B_PSTH_PCR_TRPREG_RWIO) >> N_PSTH_PCR_TRPREG_RWIO) !=
          (UINT32) Type) {
        IoTrapRegHighDword = ((IoTrapRegHighDword | B_PSTH_PCR_TRPREG_RWM) & ~B_PSTH_PCR_TRPREG_RWIO);
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
  mIoTrapRecord->Signature               = IO_TRAP_RECORD_SIGNATURE;
  mIoTrapRecord->Context.Address         = *Address;
  mIoTrapRecord->Context.Length          = Length;
  mIoTrapRecord->Context.Type            = Type;
  mIoTrapRecord->Context.ByteEnable      = ByteEnable;
  mIoTrapRecord->Context.ByteEnableMask  = ByteEnableMask;
  mIoTrapRecord->IoTrapCallback          = IoTrapDispatchFunction;
  mIoTrapRecord->IoTrapExCallback        = IoTrapExDispatchFunction;
  mIoTrapRecord->IoTrapNumber            = TrapHandlerNum;

  InsertTailList (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase), &mIoTrapRecord->Link);

  //
  // Child's handle will be the address linked list link in the record
  //
  *DispatchHandle = (EFI_HANDLE) (&mIoTrapRecord->Link);

  DEBUG ((DEBUG_INFO, "Result Address:%x \n", *Address));
  DEBUG ((DEBUG_INFO, "Result Length:%x \n", Length));

  return EFI_SUCCESS;
}

/**
  IoTrap unregistratrion helper fucntion.

  @param[in] DispatchHandle             Handle of dispatch function

  @retval    EFI_INVALID_PARAMETER      If DispatchHandle is invalid,
             EFI_ACCESS_DENIED          If the SmmReadyToLock event has been triggered,
             EFI_SUCCESS                IoTrap unregister successfully.
**/
EFI_STATUS
IoTrapUnRegisterHelper (
  IN EFI_HANDLE                                  DispatchHandle
  )
{
  EFI_STATUS            Status;
  IO_TRAP_RECORD        *RecordToDelete;
  UINT32                IoTrapRegLowDword;
  EFI_PHYSICAL_ADDRESS  BaseAddress;
  UINT32                UsedLength;
  UINT8                 TrapHandlerNum;
  UINT8                 LengthIndex;
  BOOLEAN               RequireToDisableIoTrapHandler;

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

  RequireToDisableIoTrapHandler = FALSE;
  //
  // Loop through the first IO Trap handler, looking for the suitable handler
  //
  TrapHandlerNum = RecordToDelete->IoTrapNumber;

  if (mIoTrapData.Entry[TrapHandlerNum].MergeDisable) {
    //
    // Disable the IO Trap handler if it's the only child of the Trap handler
    //
    RequireToDisableIoTrapHandler = TRUE;
  } else {
    //
    // Get information from Io Trap handler register
    //
    IoTrapRegLowDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8);

    //
    // Check next Io Trap handler if the IO Trap handler is not used
    //
    if (AddressFromLowDword (IoTrapRegLowDword) != 0) {

      UsedLength  = LengthFromLowDword (IoTrapRegLowDword);
      BaseAddress = AddressFromLowDword (IoTrapRegLowDword);

      //
      // Check if it's the maximum address of the Io Trap handler
      //
      if ((UINTN)(BaseAddress + UsedLength) == (UINTN)(RecordToDelete->Context.Address + RecordToDelete->Context.Length)) {

        if (BaseAddress == RecordToDelete->Context.Address) {
          //
          // Disable the IO Trap handler if it's the only child of the Trap handler
          //
          RequireToDisableIoTrapHandler = TRUE;
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
          if (LengthIndex < sizeof (mLengthTable) / sizeof (UINT16)) {
            //
            // Decrease the length to prevent the IO trap SMI
            //
            IoTrapRegLowDword = (UINT32) ((((UsedLength - 1) &~(BIT1 + BIT0)) << 16) | BaseAddress | B_PSTH_PCR_TRPREG_TSE);
          }
          SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, TRUE);
        }
      }
    }
  }

  if (RequireToDisableIoTrapHandler) {
    mIoTrapHandle = mIoTrapData.Entry[TrapHandlerNum].IoTrapHandle;
    Status        = PchInternalIoTrapSmiUnRegister (mIoTrapHandle);
    ASSERT_EFI_ERROR (Status);

    SetIoTrapLowDword (TrapHandlerNum, 0, TRUE);
    SetIoTrapHighDword (TrapHandlerNum, 0, TRUE);
    //
    // Also clear pending IOTRAP status.
    //
    ClearPendingIoTrapStatus (TrapHandlerNum);

    mIoTrapData.Entry[TrapHandlerNum].IoTrapHandle = 0;
    mIoTrapData.Entry[TrapHandlerNum].MergeDisable = FALSE;
    if (mIoTrapData.Entry[TrapHandlerNum].ReservedAcpiIoResource == TRUE) {
      mIoTrapData.Entry[TrapHandlerNum].ReservedAcpiIoResource = FALSE;
      UpdateIoTrapAcpiResources (TrapHandlerNum, 0, FALSE);
    }
  }

  RemoveEntryList (&RecordToDelete->Link);
  Status = gSmst->SmmFreePool (RecordToDelete);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
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
                                  If the registration address is not 0, it's caller's responsibility
                                  to reserve the IO resource in ACPI.
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
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "IoTrapRegister\n"));
  Status = IoTrapRegisterHelper (
             DispatchHandle,
             DispatchFunction,
             NULL,
             &(RegisterContext->Address),
             RegisterContext->Length,
             (IO_TRAP_EX_DISPATCH_TYPE) RegisterContext->Type,
             0x00,
             0x0F);

  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gEfiSmmIoTrapDispatch2ProtocolGuid, DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
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
  IO_TRAP_RECORD *RecordToDelete;

  RecordToDelete = IO_TRAP_RECORD_FROM_LINK (DispatchHandle);
  SmiHandlerProfileUnregisterHandler (&gEfiSmmIoTrapDispatch2ProtocolGuid, RecordToDelete->IoTrapCallback, NULL, 0);
  return IoTrapUnRegisterHelper (DispatchHandle);
}

/**
  Register a new IO Trap Ex SMI dispatch function.

  @param[in] This                 Pointer to the IO_TRAP_EX_DISPATCH_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for
                                  this SMI source.
  @param[in] RegisterContext      Pointer to the dispatch function's context.
                                  The caller fills this context in before calling
                                  the register function to indicate to the register
                                  function the IO trap Ex SMI source for which the dispatch
                                  function should be invoked.  This MUST not be NULL.
  @param[out] DispatchHandle      Handle of dispatch function.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources are available
  @retval EFI_INVALID_PARAMETER   Address requested is already in use.
  @retval EFI_ACCESS_DENIED       Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
IoTrapExRegister (
  IN  IO_TRAP_EX_DISPATCH_PROTOCOL  *This,
  IN  IO_TRAP_EX_DISPATCH_CALLBACK  DispatchFunction,
  IN  IO_TRAP_EX_REGISTER_CONTEXT   *RegisterContext,
  OUT EFI_HANDLE                    *DispatchHandle
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "PchSmmIoTrapExRegister\n"));
  //
  // Return error if length is less than 4 and not power of 2.
  //
  if ((RegisterContext->Length < 4) || ((RegisterContext->Length & (RegisterContext->Length - 1)) != 0)) {
    DEBUG ((DEBUG_ERROR, "The Dispatch Length is not power of 2 \n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = IoTrapRegisterHelper (
             DispatchHandle,
             NULL,
             DispatchFunction,
             &(RegisterContext->Address),
             RegisterContext->Length,
             RegisterContext->Type,
             RegisterContext->ByteEnable,
             RegisterContext->ByteEnableMask);

  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gIoTrapExDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  Unregister a SMI source dispatch function.
  This function is unsupported.

  @param[in] This                 Pointer to the IO_TRAP_EX_DISPATCH_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_UNSUPPORTED         The function is unsupported.
**/
EFI_STATUS
EFIAPI
IoTrapExUnRegister (
  IN IO_TRAP_EX_DISPATCH_PROTOCOL   *This,
  IN EFI_HANDLE                     DispatchHandle
  )
{
  IO_TRAP_RECORD *RecordToDelete;

  RecordToDelete = IO_TRAP_RECORD_FROM_LINK (DispatchHandle);
  SmiHandlerProfileUnregisterHandler (&gIoTrapExDispatchProtocolGuid, RecordToDelete->IoTrapCallback, NULL, 0);
  return IoTrapUnRegisterHelper (DispatchHandle);
}

/**
  Pause IoTrap callback function.

  This function disables the SMI enable of IoTrap according to the DispatchHandle,
  which is returned by IoTrap callback registration. It only supports the DispatchHandle
  with MergeDisable TRUE and address not zero.

  NOTE: This call does not guarantee all pending IO cycles to be synchronized
        and pending IO cycles issued before this call might not be trapped.

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
  BOOLEAN                               DisableIoTrap;

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
    if (mIoTrapData.Entry[TrapHandlerNum].MergeDisable != TRUE) {
      continue;
    }
    IoTrapRegLowDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8);
    IoTrapRegHighDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8 + 4);
    //
    // Depending on the usage, we will obtain the UsedLength and BaseAddress differently
    // If the registered trap length is less than 4, we obtain the length from Byte Enable Mask
    // In the other hand, we obtain the length from Address Mask
    //
    if (ByteEnableMaskFromHighDword (IoTrapRegHighDword) != 0xF) {
      UsedLength = (UINT32) (HighBitSet32 (IoTrapRegHighDword & 0xF0) - LowBitSet32 (IoTrapRegHighDword & 0xF0) + 1);
      BaseAddress = AddressFromLowDword (IoTrapRegLowDword) + LowBitSet32 (ByteEnableMaskFromHighDword (IoTrapRegHighDword));
    } else {
      UsedLength  = LengthFromLowDword (IoTrapRegLowDword);
      BaseAddress = AddressFromLowDword (IoTrapRegLowDword);
    }

    //
    // The address and length of record matches the IoTrap register's.
    //
    DisableIoTrap = FALSE;
    if ((IoTrapRecord->IoTrapExCallback != NULL) &&
        IsIoTrapExContentMatched (IoTrapRecord, IoTrapRegLowDword, IoTrapRegHighDword)) {
      DisableIoTrap = TRUE;
    } else if ((BaseAddress == IoTrapRecord->Context.Address) &&
               (UsedLength  == IoTrapRecord->Context.Length )) {
      DisableIoTrap = TRUE;
    }

    if (DisableIoTrap) {
      //
      // Check if status matched.
      // If this is already Paused, return warning status.
      //
      if ((IoTrapRegLowDword & B_PSTH_PCR_TRPREG_TSE) == 0) {
        return EFI_ACCESS_DENIED;
      }
      //
      // Clear IoTrap register SMI enable bit
      //
      IoTrapRegLowDword &= (~B_PSTH_PCR_TRPREG_TSE);
      SetIoTrapLowDword (TrapHandlerNum, IoTrapRegLowDword, FALSE);
      //
      // Also clear pending IOTRAP status.
      //
      ClearPendingIoTrapStatus (TrapHandlerNum);
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
  BOOLEAN                               EnableIoTrap;

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
    if (mIoTrapData.Entry[TrapHandlerNum].MergeDisable != TRUE) {
      continue;
    }
    IoTrapRegLowDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8);
    IoTrapRegHighDword = PchPcrRead32 (PID_PSTH, R_PSTH_PCR_TRPREG0 + TrapHandlerNum * 8 + 4);
    //
    // Depending on the usage, we will obtain the UsedLength and BaseAddress differently
    // If the registered trap length is less than 4, we obtain the length from Byte Enable Mask
    // In the other hand, we obtain the length from Address Mask
    //
    if (ByteEnableMaskFromHighDword (IoTrapRegHighDword) != 0xF) {
      UsedLength  = (UINT32) (HighBitSet32 (IoTrapRegHighDword & 0xF0) - LowBitSet32 (IoTrapRegHighDword & 0xF0) + 1);
      BaseAddress = AddressFromLowDword (IoTrapRegLowDword) + LowBitSet32 (ByteEnableMaskFromHighDword (IoTrapRegHighDword));
    } else {
      UsedLength  = LengthFromLowDword (IoTrapRegLowDword);
      BaseAddress = AddressFromLowDword (IoTrapRegLowDword);
    }

    //
    // The address and length of record matches the IoTrap register's.
    //
    EnableIoTrap = FALSE;
    if ((IoTrapRecord->IoTrapExCallback != NULL) &&
        IsIoTrapExContentMatched (IoTrapRecord, IoTrapRegLowDword, IoTrapRegHighDword)) {
      EnableIoTrap = TRUE;
    } else if ((BaseAddress == IoTrapRecord->Context.Address) &&
               (UsedLength  == IoTrapRecord->Context.Length )) {
      EnableIoTrap = TRUE;
    }

    if (EnableIoTrap) {
      //
      // Check if status matched.
      // If this is already Resume, return warning status.
      //
      if ((IoTrapRegLowDword & B_PSTH_PCR_TRPREG_TSE) != 0) {
        return EFI_ACCESS_DENIED;
      }
      //
      // Set IoTrap register SMI enable bit
      //
      IoTrapRegLowDword |= (B_PSTH_PCR_TRPREG_TSE);
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
  UINTN                  TrapHandlerNum;

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
  // Initialize the IO TRAP EX protocol
  //
  mIoTrapData.IoTrapExDispatchProtocol.Register       = IoTrapExRegister;
  mIoTrapData.IoTrapExDispatchProtocol.UnRegister     = IoTrapExUnRegister;

  //
  // Initialize the IO TRAP control protocol.
  //
  mIoTrapData.PchSmmIoTrapControlProtocol.Pause       = IoTrapControlPause;
  mIoTrapData.PchSmmIoTrapControlProtocol.Resume      = IoTrapControlResume;

  for (TrapHandlerNum = 0; TrapHandlerNum < IO_TRAP_HANDLER_NUM; TrapHandlerNum++) {
    //
    // Initialize IO TRAP Callback DataBase
    //
    InitializeListHead (&(mIoTrapData.Entry[TrapHandlerNum].CallbackDataBase));
  }
  mIoTrapData.Entry[0].CallbackDispatcher = IoTrapDispatcher0;
  mIoTrapData.Entry[1].CallbackDispatcher = IoTrapDispatcher1;
  mIoTrapData.Entry[2].CallbackDispatcher = IoTrapDispatcher2;
  mIoTrapData.Entry[3].CallbackDispatcher = IoTrapDispatcher3;

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
                    &gIoTrapExDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mIoTrapData.IoTrapExDispatchProtocol
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
