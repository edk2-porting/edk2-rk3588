/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Library/PchP2sbLib.h>
#include <Library/PchSbiAccessLib.h>

/**
  Execute PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
**/
EFI_STATUS
EFIAPI
PchSbiExecution (
  IN     PCH_SBI_PID                    Pid,
  IN     UINT64                         Offset,
  IN     PCH_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  )
{


  return PchSbiExecutionEx ( Pid,
                             Offset,
                             Opcode,
                             Posted,
                             0x000F,
                             0x0000,
                             0x0000,
                             Data32,
                             Response
                             );
}

/**
  Full function for executing PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in] Fbe                        First byte enable
  @param[in] Bar                        Bar
  @param[in] Fid                        Function ID
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
**/
EFI_STATUS
EFIAPI
PchSbiExecutionEx (
  IN     PCH_SBI_PID                    Pid,
  IN     UINT64                         Offset,
  IN     PCH_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fbe,
  IN     UINT16                         Bar,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  )
{
  EFI_STATUS                            Status;
  UINTN                                 P2sbBase;
  BOOLEAN                               P2sbOrgStatus;
  UINTN                                 Timeout;
  UINT16                                SbiStat;

  //
  // Check opcode valid
  //
  switch (Opcode) {
    case PciConfigRead:
    case PciConfigWrite:
    case PrivateControlRead:
    case PrivateControlWrite:
    case GpioLockUnlock:
      break;
    default:
      return EFI_INVALID_PARAMETER;
      break;
  }

  P2sbOrgStatus = FALSE;
  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_P2SB,
                    PCI_FUNCTION_NUMBER_PCH_P2SB
                    );
  PchRevealP2sb (P2sbBase, &P2sbOrgStatus);
  ///
  /// BWG Section 2.2.1
  /// 1. Poll P2SB PCI offset D8h[0] = 0b
  /// Make sure the previous opeartion is completed.
  ///
  Timeout = 0xFFFFFFF;
  while (Timeout > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
    Timeout--;
  }
  if (Timeout == 0) {
    Status = EFI_DEVICE_ERROR;
    goto ExitPchSbiExecutionEx;
  }
  //
  // Initial Response status
  //
  *Response = SBI_INVALID_RESPONSE;
  Status    = EFI_SUCCESS;
  SbiStat   = 0;
  ///
  /// 2. Write P2SB PCI offset D0h[31:0] with Address and Destination Port ID
  ///
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIADDR, (UINT32) ((Pid << 24) | (UINT16)Offset));
  ///
  /// 3. Write P2SB PCI offset DCh[31:0] with extended address, which is expected to be 0 in SKL PCH.
  ///
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIEXTADDR, (UINT32) RShiftU64 (Offset, 16));
  ///
  /// 5. Set P2SB PCI offset D8h[15:8] = 00000110b for read
  ///    Set P2SB PCI offset D8h[15:8] = 00000111b for write
  //
  // Set SBISTAT[15:8] to the opcode passed in
  // Set SBISTAT[7] to the posted passed in
  //
  MmioAndThenOr16 (
    (P2sbBase + R_PCH_P2SB_SBISTAT),
    (UINT16) ~(B_PCH_P2SB_SBISTAT_OPCODE | B_PCH_P2SB_SBISTAT_POSTED),
    (UINT16) ((Opcode << 8) | (Posted << 7))
    );
  ///
  /// 6. Write P2SB PCI offset DAh[15:0] = F000h
  ///
  //
  // Set RID[15:0] = Fbe << 12 | Bar << 8 | Fid
  //
  MmioWrite16 (
    (P2sbBase + R_PCH_P2SB_SBIRID),
    (((Fbe & 0x000F) << 12) | ((Bar & 0x0007) << 8) | (Fid & 0x00FF))
    );

  switch (Opcode) {
    case PciConfigWrite:
    case PrivateControlWrite:
    case GpioLockUnlock:
      ///
      /// 4. Write P2SB PCI offset D4h[31:0] with the intended data accordingly
      ///
      MmioWrite32 ((P2sbBase + R_PCH_P2SB_SBIDATA), *Data32);
      break;
    default:
      ///
      /// 4. Write P2SB PCI offset D4h[31:0] with dummy data such as 0,
      /// because all D0-DFh register range must be touched in SKL PCH
      /// for a successful SBI transaction.
      ///
      MmioWrite32 ((P2sbBase + R_PCH_P2SB_SBIDATA), 0);
      break;
  }
  ///
  /// 7. Set P2SB PCI offset D8h[0] = 1b, Poll P2SB PCI offset D8h[0] = 0b
  ///
  //
  // Set SBISTAT[0] = 1b, trigger the SBI operation
  //
  MmioOr16 (P2sbBase + R_PCH_P2SB_SBISTAT, (UINT16) B_PCH_P2SB_SBISTAT_INITRDY);
  //
  // Poll SBISTAT[0] = 0b, Polling for Busy bit
  //
  Timeout = 0xFFFFFFF;
  while (Timeout > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
    Timeout--;
  }
  if (Timeout == 0) {
    //
    // If timeout, it's fatal error.
    //
    Status = EFI_DEVICE_ERROR;
  } else {
    ///
    /// 8. Check if P2SB PCI offset D8h[2:1] = 00b for successful transaction
    ///
    *Response = (UINT8)((SbiStat & B_PCH_P2SB_SBISTAT_RESPONSE) >> N_PCH_P2SB_SBISTAT_RESPONSE);
    if (*Response == SBI_SUCCESSFUL) {
      switch (Opcode) {
      case PciConfigRead:
      case PrivateControlRead:
        ///
        /// 9. Read P2SB PCI offset D4h[31:0] for SBI data
        ///
        *Data32 = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIDATA);
        break;
      default:
        break;
      }
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

ExitPchSbiExecutionEx:
  if (!P2sbOrgStatus) {
    PchHideP2sb (P2sbBase);
  }
  return Status;
}

/**
  This function saves all PCH SBI registers.
  The save and restore operations must be done while using the PchSbiExecution inside SMM.
  It prevents the racing condition of PchSbiExecution re-entry between POST and SMI.
  Before using this function, make sure the P2SB is not hidden.

  @param[in, out] PchSbiRegister        Structure for saving the registers

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Device is hidden.
**/
EFI_STATUS
EFIAPI
PchSbiRegisterSave (
  IN OUT PCH_SBI_REGISTER_STRUCT        *PchSbiRegister
  )
{
  UINTN                                 P2sbBase;
  UINTN                                 Timeout;
  UINT16                                SbiStat;

  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_P2SB,
                    PCI_FUNCTION_NUMBER_PCH_P2SB
                    );
  if (MmioRead16 (P2sbBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Make sure it's not busy.
  // Poll SBISTAT[0] = 0b
  //
  Timeout = 0xFFFFFFF;
  while ((Timeout--) > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
  }
  if (Timeout == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Save original SBI registers
  //
  PchSbiRegister->SbiAddr    = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIADDR);
  PchSbiRegister->SbiExtAddr = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIEXTADDR);
  PchSbiRegister->SbiData    = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIDATA);
  PchSbiRegister->SbiStat    = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
  PchSbiRegister->SbiRid     = MmioRead16 (P2sbBase + R_PCH_P2SB_SBIRID);

  return EFI_SUCCESS;
}

/**
  This function restores all PCH SBI registers
  The save and restore operations must be done while using the PchSbiExecution inside SMM.
  It prevents the racing condition of PchSbiExecution re-entry between POST and SMI.
  Before using this function, make sure the P2SB is not hidden.

  @param[in] PchSbiRegister             Structure for restoring the registers

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Device is hidden.
**/
EFI_STATUS
EFIAPI
PchSbiRegisterRestore (
  IN PCH_SBI_REGISTER_STRUCT            *PchSbiRegister
  )
{
  UINTN                                 P2sbBase;
  UINTN                                 Timeout;
  UINT16                                SbiStat;

  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_P2SB,
                    PCI_FUNCTION_NUMBER_PCH_P2SB
                    );
  if (MmioRead16 (P2sbBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Make sure it's not busy.
  // Poll SBISTAT[0] = 0b
  //
  Timeout = 0xFFFFFFF;
  while ((Timeout--) > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
  }
  if (Timeout == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Restore original SBI registers
  //
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIADDR   , PchSbiRegister->SbiAddr);
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIEXTADDR, PchSbiRegister->SbiExtAddr);
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIDATA   , PchSbiRegister->SbiData);
  MmioWrite16 (P2sbBase + R_PCH_P2SB_SBISTAT   , PchSbiRegister->SbiStat);
  MmioWrite16 (P2sbBase + R_PCH_P2SB_SBIRID    , PchSbiRegister->SbiRid);

  return EFI_SUCCESS;
}

