/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "GpioLibrary.h"
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiMultiPhase.h>
#include <Library/HobLib.h>

//
// Possible registers to be accessed using GpioReadWriteReg() function
//
typedef enum {
  GpioHostOwnershipRegister = 0,
  GpioGpeEnableRegister,
  GpioSmiEnableRegister,
  GpioNmiEnableRegister,
  GpioPadConfigLockRegister,
  GpioPadLockOutputRegister
} GPIO_REG;

/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[in] Mask                 Mask
  @param[in] Write                Perform read(0) or write(1)
  @param[in,out] ReadWriteValue   Read/Write data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_UNSUPPORTED         Host cannot access this pad
**/
static
EFI_STATUS
GpioReadWritePadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  IN UINT32               Mask,
  IN BOOLEAN              Write,
  IN OUT UINT32           *ReadWriteVal
  )
{
  UINT32               PadCfgReg;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               GroupIndex;
  UINT32               PadNumber;


  GPIO_PAD_OWN         PadOwnVal;


  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pin number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    return EFI_INVALID_PARAMETER;
  }

  if (Write && (DwReg == 1 || (Mask & ~B_PCH_GPIO_TX_STATE) != 0) && GpioIsPadLocked (GroupIndex, PadNumber)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pad is locked (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_WRITE_PROTECTED;
  }

DEBUG_CODE_BEGIN();
  //
  // Check if selected GPIO Pad is not owned by CSME/ISH
  // If GPIO is not owned by Host all access to PadCfg will be dropped
  //
  GpioGetPadOwnership (GpioPad, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_UNSUPPORTED;
  }

  //
DEBUG_CODE_END();

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = 0x8 * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;
  if(DwReg == 1) {
    PadCfgReg += 0x4;
  }

  if (Write) {
    MmioAndThenOr32 (
      (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg),
      (UINT32)(~Mask),
      (UINT32)(*ReadWriteVal & Mask)
    );
  } else {
    *ReadWriteVal = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg));
    *ReadWriteVal &= Mask;
  }

  return EFI_SUCCESS;
}

/**
  This procedure will write or read GPIO register

  @param[in] RegType              GPIO register type
  @param[in] Group                GPIO group
  @param[in] DwNum                Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] GpioPad              GPIO pad
  @param[in] Write                Perform read(0) or write(1)
  @param[in] OnePad               Access whole register(0) or one pad(1)
  @param[in,out] ReadWriteValue   Read/Write data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group, pad or DwNum parameter number
**/
static
EFI_STATUS
GpioReadWriteReg (
  IN GPIO_REG                  RegType,
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN GPIO_PAD                  GpioPad,
  IN BOOLEAN                   Write,
  IN BOOLEAN                   OnePad,
  IN OUT UINT32                *ReadWriteVal
  )
{
  UINT32               Mask;
  UINT32               RegOffset;
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  RegOffset = 0;
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  if (OnePad) {
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
    PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
DEBUG_CODE_BEGIN();
    if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      return EFI_UNSUPPORTED;
    }
DEBUG_CODE_END();
  } else {
    GroupIndex = GpioGetGroupIndexFromGroup (Group);
    PadNumber = 0;
  }
  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return EFI_INVALID_PARAMETER;
  }

  switch (RegType) {
    case GpioHostOwnershipRegister:
      RegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset;
      break;
    case GpioGpeEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeEnOffset;
      break;
    case GpioSmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiEnOffset;
      break;
    case GpioNmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].NmiEnOffset;
      break;
    case GpioPadConfigLockRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockOffset;
      break;
    case GpioPadLockOutputRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockTxOffset;
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  //
  // Check if selected register exists
  //
  if (RegOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Access one GPIO Pad
  //
  if (OnePad) {
    //
    // Check if legal pin number
    //
    if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
      return EFI_INVALID_PARAMETER;
    }
    //
    // For future use. If there are more then 32 pads per group then certain
    // group information would be split into more then one DWord register.
    //
    RegOffset += (PadNumber >> 5) * 0x4;
    //
    // Calculate pad bit position within DWord register
    //
    PadNumber %= 32;
    Mask = BIT0 << PadNumber;

    if (Write) {
      MmioAndThenOr32 (
        (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset),
        (UINT32)(~Mask),
        (UINT32)((*ReadWriteVal << PadNumber) & Mask)
      );
    } else {
      *ReadWriteVal = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset));
      *ReadWriteVal = (*ReadWriteVal & Mask) >> PadNumber;
    }
  //
  // Access whole register
  //
  } else {
    //
    // Check if DwNum argument does not exceed number of DWord registers
    // resulting from available pads for certain group
    //
    if (DwNum > ((GpioGroupInfo[GroupIndex].PadPerGroup - 1) >> 5)){
      return EFI_INVALID_PARAMETER;
    }
    //
    // For future use. If there are more then 32 pads per group then certain
    // group information would be split into more then one DWord register.
    // For SKL PCH DwNum must be 0.
    //
    RegOffset += DwNum *0x4;

    if (Write) {
      MmioWrite32 (
        (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset),
        (UINT32)(*ReadWriteVal)
      );
    } else {
      *ReadWriteVal = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset));
    }
  }

  return EFI_SUCCESS;
}

/**
  This procedure will write GPIO Lock/LockTx register using SBI.

  @param[in] RegType              GPIO register (Lock or LockTx)
  @param[in] Unlock               Lock pads(0) or unlock(1)
  @param[in] Group                GPIO group number
  @param[in] DwNum                Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] PadsToModify         Bit mask for pads that are going to be modified
  @param[in] GpioPad              GPIO pad
  @param[in] OnePad               Access whole register(0) or one pad(1)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group, pad or DwNum parameter number
**/
static
EFI_STATUS
GpioLockPadsUsingSbi (
  IN GPIO_REG                  RegType,
  IN BOOLEAN                   Unlock,
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToModify,
  IN GPIO_PAD                  GpioPad,
  IN BOOLEAN                   OnePad
  )
{
  UINT8                Response;
  EFI_STATUS           Status;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               RegOffset;
  UINT32               OldPadCfgLockRegVal;
  UINT32               NewPadCfgLockRegVal;
  UINT32               GroupIndex;
  UINT32               PadNumber;

  RegOffset = 0;
  OldPadCfgLockRegVal = 0;
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  if (OnePad) {
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
    PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
    Group = GpioGetGroupFromGpioPad (GpioPad);
DEBUG_CODE_BEGIN();
    if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      return EFI_UNSUPPORTED;
    }
DEBUG_CODE_END();
  } else {
    GroupIndex = GpioGetGroupIndexFromGroup (Group);
    PadNumber = 0;
  }

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return EFI_INVALID_PARAMETER;
  }

  switch (RegType) {
    case GpioPadConfigLockRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockOffset;
      break;
    case GpioPadLockOutputRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockTxOffset;
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  //
  // Check if selected register exists
  //
  if (RegOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Access one GPIO Pad
  //
  if (OnePad) {
    //
    // Check if legal pin number
    //
    if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
      return EFI_INVALID_PARAMETER;
    }

    //
    // For future use. If there are more then 32 pads per group then certain
    // group information would be split into more then one DWord register.
    //
    DwNum = (PadNumber >> 5);
    RegOffset += DwNum * 0x4;
    //
    // Calculate pad bit position within DWord register
    //
    PadNumber %= 32;

    switch (RegType) {
      case GpioPadConfigLockRegister:
        GpioGetPadCfgLockForGroupDw (Group, DwNum, &OldPadCfgLockRegVal);
        break;
      case GpioPadLockOutputRegister:
        GpioGetPadCfgLockTxForGroupDw (Group, DwNum, &OldPadCfgLockRegVal);
        break;
    }
    if (Unlock) {
      NewPadCfgLockRegVal = OldPadCfgLockRegVal & (~(0x1 << PadNumber));
    } else {
      NewPadCfgLockRegVal = OldPadCfgLockRegVal | (0x1 << PadNumber);
    }

  } else {
    //
    // Access whole register
    //

    //
    // Check if DwNum argument does not exceed number of DWord registers
    // resulting from available pads for certain group
    //
    if (DwNum > ((GpioGroupInfo[GroupIndex].PadPerGroup - 1) >> 5)){
      return EFI_INVALID_PARAMETER;
    }
    //
    // For future use. If there are more then 32 pads per group then certain
    // group information would be split into more then one DWord register.
    // For SKL PCH DwNum must be 0.
    //
    RegOffset += DwNum *0x4;

    switch (RegType) {
      case GpioPadConfigLockRegister:
        GpioGetPadCfgLockForGroupDw (Group, DwNum, &OldPadCfgLockRegVal);
        break;
      case GpioPadLockOutputRegister:
        GpioGetPadCfgLockTxForGroupDw (Group, DwNum, &OldPadCfgLockRegVal);
        break;
    }
    if (Unlock) {
      NewPadCfgLockRegVal = OldPadCfgLockRegVal & (~PadsToModify);
    } else {
      NewPadCfgLockRegVal = OldPadCfgLockRegVal | PadsToModify;
    }
  }

  Status = PchSbiExecution (
             GpioGroupInfo[GroupIndex].Community,
             RegOffset,
             GpioLockUnlock,
             FALSE,
             &NewPadCfgLockRegVal,
             &Response
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will read multiple GPIO settings

  @param[in]  GpioPad                   GPIO Pad
  @param[out] GpioData                  GPIO data structure

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
EFI_STATUS
GpioGetPadConfig (
  IN  GPIO_PAD               GpioPad,
  OUT GPIO_CONFIG            *GpioData
  )
{
  UINT32               Dw0Reg;
  UINT32               Dw1Reg;
  UINT32               PadCfgReg;
  UINT32               RegVal;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  GPIO_GROUP           Group;
  UINT32               GroupIndex;
  UINT32               PadNumber;

  GPIO_PAD_OWN         PadOwnVal;


  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  Group = GpioGetGroupFromGpioPad (GpioPad);
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  GpioGetPadOwnership (GpioPad, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  //
  // Check if group argument exceeds GPIO group range
  //
  if ((Group < GpioGetLowestGroup ()) || (Group > GpioGetHighestGroup ())) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pin number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Create PADCFG register offset using group and pad number
  //
  PadCfgReg = 0x8 * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;

  //
  // Read PADCFG DW0 register
  //
  Dw0Reg = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg));

  //
  // Read PADCFG DW1 register
  //
  Dw1Reg = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg + 0x4));


  //
  // Get Reset Type (PadRstCfg)
  //
  GpioData->PowerConfig = ((Dw0Reg & B_PCH_GPIO_RST_CONF) >> (N_PCH_GPIO_RST_CONF - (GPIO_CONF_RESET_BIT_POS + 1))) | (0x1 << GPIO_CONF_RESET_BIT_POS);

  //
  // Get how interrupt is triggered (RxEvCfg)
  //
  GpioData->InterruptConfig = ((Dw0Reg & B_PCH_GPIO_RX_LVL_EDG) >> (N_PCH_GPIO_RX_LVL_EDG - (GPIO_CONF_INT_TRIG_BIT_POS + 1))) | (0x1 << GPIO_CONF_INT_TRIG_BIT_POS);

  //
  // Get interrupt generation (GPIRoutIOxAPIC/SCI/SMI/NMI)
  //
  GpioData->InterruptConfig |= ((Dw0Reg & (B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE)) >> (N_PCH_GPIO_RX_NMI_ROUTE - (GPIO_CONF_INT_ROUTE_BIT_POS + 1))) | (0x1 << GPIO_CONF_INT_ROUTE_BIT_POS);

  //
  // Get GPIO direction (GPIORxDis and GPIOTxDis)
  //
  GpioData->Direction = ((Dw0Reg & (B_PCH_GPIO_RXDIS | B_PCH_GPIO_TXDIS)) >> (N_PCH_GPIO_TXDIS - (GPIO_CONF_DIR_BIT_POS + 1))) | (0x1 << GPIO_CONF_DIR_BIT_POS);

  //
  // Get GPIO input inversion (RXINV)
  //
  GpioData->Direction |= ((Dw0Reg & B_PCH_GPIO_RXINV) >> (N_PCH_GPIO_RXINV - (GPIO_CONF_INV_BIT_POS + 1))) | (0x1 << GPIO_CONF_INV_BIT_POS);

  //
  // Get GPIO output state (GPIOTxState)
  //
  GpioData->OutputState = ((Dw0Reg & B_PCH_GPIO_TX_STATE) << (N_PCH_GPIO_TX_STATE + (GPIO_CONF_OUTPUT_BIT_POS + 1))) | (0x1 << GPIO_CONF_OUTPUT_BIT_POS) ;

  //
  // Configure GPIO RX raw override to '1' (RXRAW1)
  //
  GpioData->OtherSettings = ((Dw0Reg & B_PCH_GPIO_RX_RAW1) >> (N_PCH_GPIO_RX_RAW1 - (GPIO_CONF_RXRAW_BIT_POS + 1))) | (0x1 << GPIO_CONF_RXRAW_BIT_POS) ;

  //
  // Get GPIO Pad Mode (PMode)
  //
  GpioData->PadMode = ((Dw0Reg & B_PCH_GPIO_PAD_MODE) >> (N_PCH_GPIO_PAD_MODE - (GPIO_CONF_PAD_MODE_BIT_POS + 1))) | (0x1 << GPIO_CONF_PAD_MODE_BIT_POS);

  //
  // Get GPIO termination (Term)
  //
  GpioData->ElectricalConfig = ((Dw1Reg & B_PCH_GPIO_TERM) >> (N_PCH_GPIO_TERM - (GPIO_CONF_TERM_BIT_POS + 1))) | (0x1 << GPIO_CONF_TERM_BIT_POS) ;

  //
  // Get GPIO pad tolerance (padtol)
  //
  GpioData->ElectricalConfig |= ((Dw1Reg & B_PCH_GPIO_PADTOL) >> (N_PCH_GPIO_PADTOL - (GPIO_CONF_PADTOL_BIT_POS + 1))) | (0x1 << GPIO_CONF_PADTOL_BIT_POS) ;

  //
  // Read HOSTSW_OWN registers
  //
  RegVal = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].HostOwnOffset));

  //
  // Get Host Software Ownership
  //
  GpioData->HostSoftPadOwn = (((RegVal >> PadNumber) & 0x1) << (GPIO_CONF_HOST_OWN_BIT_POS + 1)) | (0x1 << GPIO_CONF_HOST_OWN_BIT_POS);

  //
  // Read PADCFGLOCK register
  //
  RegVal = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].PadCfgLockOffset));

  //
  // Get Pad Configuration Lock state
  //
  GpioData->LockConfig = (((RegVal >> PadNumber) & 0x1) << 1) | 0x1;

  //
  // Read PADCFGLOCKTX register
  //
  RegVal = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].PadCfgLockTxOffset));

  //
  // Get Pad Configuration Lock Tx state
  //
  GpioData->LockConfig |= (((RegVal >> PadNumber) & 0x1) << 2) | 0x1;

  return EFI_SUCCESS;
}

/**
  This procedure will configure multiple GPIO settings

  @param[in] GpioPad                    GPIO Pad
  @param[in] GpioData                   GPIO data structure

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
EFI_STATUS
GpioSetPadConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_CONFIG               *GpioData
  )
{
  UINT32               Dw0Reg;
  UINT32               Dw0RegMask;
  UINT32               Dw1Reg;
  UINT32               Dw1RegMask;
  UINT32               PadCfgReg;
  UINT32               HostSoftOwnReg;
  UINT32               HostSoftOwnRegMask;
  UINT32               GpiGpeEnReg;
  UINT32               GpiGpeEnRegMask;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  GPIO_GROUP           Group;
  UINT32               GroupIndex;
  UINT32               PadNumber;

  GPIO_PAD_OWN         PadOwnVal;


  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  Dw0RegMask = 0;
  Dw0Reg = 0;
  Dw1RegMask = 0;
  Dw1Reg = 0;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  GpioGetPadOwnership (GpioPad, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();
  //
  // Check if group argument exceeds GPIO group range
  //
  if ((Group < GpioGetLowestGroup ()) || (Group > GpioGetHighestGroup ())) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pin number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    return EFI_INVALID_PARAMETER;
  }

  if (GpioIsPadLocked (GroupIndex, PadNumber)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pad is locked (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_WRITE_PROTECTED;
  }

  //
  // Configure Reset Type (PadRstCfg)
  //
  Dw0RegMask |= ((((GpioData->PowerConfig & GPIO_CONF_RESET_MASK) >> GPIO_CONF_RESET_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RST_CONF);
  Dw0Reg |= (((GpioData->PowerConfig & GPIO_CONF_RESET_MASK) >> (GPIO_CONF_RESET_BIT_POS + 1)) << N_PCH_GPIO_RST_CONF);

  //
  // Configure how interrupt is triggered (RxEvCfg)
  //
  Dw0RegMask |= ((((GpioData->InterruptConfig & GPIO_CONF_INT_TRIG_MASK) >> GPIO_CONF_INT_TRIG_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RX_LVL_EDG);
  Dw0Reg |= (((GpioData->InterruptConfig & GPIO_CONF_INT_TRIG_MASK) >> (GPIO_CONF_INT_TRIG_BIT_POS + 1)) << N_PCH_GPIO_RX_LVL_EDG);

  //
  // Configure interrupt generation (GPIRoutIOxAPIC/SCI/SMI/NMI)
  //
  Dw0RegMask |= ((((GpioData->InterruptConfig & GPIO_CONF_INT_ROUTE_MASK) >> GPIO_CONF_INT_ROUTE_BIT_POS) == GpioHardwareDefault)  ? 0x0 : (B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE));
  Dw0Reg |= (((GpioData->InterruptConfig & GPIO_CONF_INT_ROUTE_MASK) >> (GPIO_CONF_INT_ROUTE_BIT_POS + 1)) << N_PCH_GPIO_RX_NMI_ROUTE);

  //
  // Configure GPIO direction (GPIORxDis and GPIOTxDis)
  //
  Dw0RegMask |= ((((GpioData->Direction & GPIO_CONF_DIR_MASK) >> GPIO_CONF_DIR_BIT_POS) == GpioHardwareDefault) ? 0x0 : (B_PCH_GPIO_RXDIS | B_PCH_GPIO_TXDIS));
  Dw0Reg |= (((GpioData->Direction & GPIO_CONF_DIR_MASK) >> (GPIO_CONF_DIR_BIT_POS + 1)) << N_PCH_GPIO_TXDIS);

  //
  // Configure GPIO input inversion (RXINV)
  //
  Dw0RegMask |= ((((GpioData->Direction & GPIO_CONF_INV_MASK) >> GPIO_CONF_INV_BIT_POS) == GpioHardwareDefault) ?  0x0 : B_PCH_GPIO_RXINV);
  Dw0Reg |= (((GpioData->Direction & GPIO_CONF_INV_MASK) >> (GPIO_CONF_INV_BIT_POS + 1)) << N_PCH_GPIO_RXINV);

  //
  // Configure GPIO output state (GPIOTxState)
  //
  Dw0RegMask |= ((((GpioData->OutputState & GPIO_CONF_OUTPUT_MASK) >> GPIO_CONF_OUTPUT_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_TX_STATE);
  Dw0Reg |= (((GpioData->OutputState & GPIO_CONF_OUTPUT_MASK) >> (GPIO_CONF_OUTPUT_BIT_POS + 1)) << N_PCH_GPIO_TX_STATE);

  //
  // Configure GPIO RX raw override to '1' (RXRAW1)
  //
  Dw0RegMask |= ((((GpioData->OtherSettings & GPIO_CONF_RXRAW_MASK) >> GPIO_CONF_RXRAW_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RX_RAW1);
  Dw0Reg |= (((GpioData->OtherSettings & GPIO_CONF_RXRAW_MASK) >> (GPIO_CONF_RXRAW_BIT_POS + 1)) << N_PCH_GPIO_RX_RAW1);

  //
  // Configure GPIO Pad Mode (PMode)
  //
  Dw0RegMask |= ((((GpioData->PadMode & GPIO_CONF_PAD_MODE_MASK) >> GPIO_CONF_PAD_MODE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PAD_MODE);
  Dw0Reg |= (((GpioData->PadMode & GPIO_CONF_PAD_MODE_MASK) >> (GPIO_CONF_PAD_MODE_BIT_POS + 1)) << N_PCH_GPIO_PAD_MODE);

  //
  // Configure GPIO termination (Term)
  //
  Dw1RegMask |= ((((GpioData->ElectricalConfig & GPIO_CONF_TERM_MASK) >> GPIO_CONF_TERM_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_TERM);
  Dw1Reg |= (((GpioData->ElectricalConfig & GPIO_CONF_TERM_MASK) >> (GPIO_CONF_TERM_BIT_POS + 1)) << N_PCH_GPIO_TERM);

  //
  // Configure GPIO pad tolerance (padtol)
  //
  Dw1RegMask |= ((((GpioData->ElectricalConfig & GPIO_CONF_PADTOL_MASK) >> GPIO_CONF_PADTOL_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PADTOL);
  Dw1Reg |= (((GpioData->ElectricalConfig & GPIO_CONF_PADTOL_MASK) >> (GPIO_CONF_PADTOL_BIT_POS + 1)) << N_PCH_GPIO_PADTOL);

  //
  // Create PADCFG register offset using group and pad number
  //
  PadCfgReg = 0x8 * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;

  //
  // Write PADCFG DW0 register
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg),
    ~(UINT32)Dw0RegMask,
    (UINT32)Dw0Reg
  );

  //
  // Write PADCFG DW1 register
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg + 0x4),
    ~(UINT32)Dw1RegMask,
    (UINT32)Dw1Reg
  );

  //
  // Update value to be programmed in HOSTSW_OWN register
  //
  HostSoftOwnRegMask = (GpioData->HostSoftPadOwn & 0x1) << PadNumber;
  HostSoftOwnReg = (GpioData->HostSoftPadOwn >> 0x1) << PadNumber;

  //
  // Write HOSTSW_OWN registers
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].HostOwnOffset),
    ~(UINT32)HostSoftOwnRegMask,
    (UINT32)HostSoftOwnReg
    );

  //
  // Update value to be programmed in GPI_GPE_EN register
  //
  GpiGpeEnRegMask = (GpioData->InterruptConfig & 0x1) << PadNumber;
  GpiGpeEnReg = ((GpioData->InterruptConfig & GpioIntSci) >> 3) << PadNumber;

  //
  // Write GPI_GPE_EN registers
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeEnOffset),
    ~(UINT32)GpiGpeEnRegMask,
    (UINT32)GpiGpeEnReg
    );

  //
  // Program Pad Configuration Lock
  //
  if ((GpioData->LockConfig & GpioPadConfigLock) == GpioPadConfigLock) {
    GpioLockPadsUsingSbi (
                 GpioPadConfigLockRegister,
                 FALSE,
                 0,
                 0,
                 0,
                 GpioPad,
                 TRUE
                 );
  }

  //
  // Program Pad Configuration Lock Tx
  //
  if ((GpioData->LockConfig & GpioOutputStateLock) == GpioOutputStateLock) {
    GpioLockPadsUsingSbi (
                 GpioPadLockOutputRegister,
                 FALSE,
                 0,
                 0,
                 0,
                 GpioPad,
                 TRUE
                 );
  }
  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO output level

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Output value
                                  0: OutputLow, 1: OutputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetOutputValue (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  )
{
  EFI_STATUS Status;

  Status = GpioReadWritePadCfgReg (
            GpioPad,
            0,
            B_PCH_GPIO_TX_STATE,
            TRUE,
            &Value
            );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will get GPIO output level

  @param[in]  GpioPad             GPIO pad
  @param[out] OutputVal           GPIO Output value
                                  0: OutputLow, 1: OutputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetOutputValue (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *OutputVal
  )
{
  EFI_STATUS Status;

  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_TX_STATE,
             FALSE,
             OutputVal
             );
  ASSERT_EFI_ERROR (Status);
  *OutputVal >>= N_PCH_GPIO_TX_STATE;

  return Status;
}

/**
  This procedure will get GPIO input level

  @param[in] GpioPad              GPIO pad
  @param[out] InputVal            GPIO Input value
                                  0: InputLow, 1: InpuHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetInputValue (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *InputVal
  )
{
  EFI_STATUS Status;

  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_RX_STATE,
             FALSE,
             InputVal
             );
  ASSERT_EFI_ERROR (Status);
  *InputVal >>= N_PCH_GPIO_RX_STATE;

  return Status;
}

/**
  This procedure will get GPIO IOxAPIC interrupt number

  @param[in]  GpioPad             GPIO pad
  @param[out] IrqNum              IRQ number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadIoApicIrqNumber (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *IrqNum
  )
{
  EFI_STATUS Status;

  Status = GpioReadWritePadCfgReg (
             GpioPad,
             1,
             B_PCH_GPIO_INTSEL,
             FALSE,
             IrqNum
             );
  ASSERT_EFI_ERROR (Status);
  *IrqNum >>= N_PCH_GPIO_INTSEL;

  return Status;
}

/**
  This procedure will configure GPIO input inversion

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value for GPIO input inversion
                                  0: No input inversion, 1: Invert input

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetInputInversion (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  )
{
  EFI_STATUS Status;

  Value <<= N_PCH_GPIO_RXINV;
  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_RXINV,
             TRUE,
             &Value
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will get GPIO pad input inversion value

  @param[in]  GpioPad             GPIO pad
  @param[out] InvertState         GPIO inversion state
                                  0: No input inversion, 1: Inverted input

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetInputInversion (
  IN  GPIO_PAD                 GpioPad,
  OUT UINT32                   *InvertState
  )
{
  EFI_STATUS Status;

  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_RXINV,
             FALSE,
             InvertState
             );
  ASSERT_EFI_ERROR (Status);
  *InvertState >>= N_PCH_GPIO_RXINV;

  return Status;
}

/**
  This procedure will set GPIO interrupt settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of Level/Edge
                                  use GPIO_INT_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadInterruptConfig (
  IN GPIO_PAD                 GpioPad,
  IN GPIO_INT_CONFIG          Value
  )
{
  EFI_STATUS  Status;
  UINT32      RxLvlEdgeValue;
  UINT32      IntRouteValue;
  UINT32      GpeEnable;

  Status = EFI_SUCCESS;

  if (((Value & GPIO_CONF_INT_TRIG_MASK) >> GPIO_CONF_INT_TRIG_BIT_POS) != GpioHardwareDefault) {
    RxLvlEdgeValue = ((Value & GPIO_CONF_INT_TRIG_MASK) >> (GPIO_CONF_INT_TRIG_BIT_POS + 1)) << N_PCH_GPIO_RX_LVL_EDG;

    Status = GpioReadWritePadCfgReg (
               GpioPad,
               0,
               B_PCH_GPIO_RX_LVL_EDG,
               TRUE,
               &RxLvlEdgeValue
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (((Value & GPIO_CONF_INT_ROUTE_MASK) >> GPIO_CONF_INT_ROUTE_BIT_POS) != GpioHardwareDefault) {

    IntRouteValue = ((Value & GPIO_CONF_INT_ROUTE_MASK) >> (GPIO_CONF_INT_ROUTE_BIT_POS + 1)) << N_PCH_GPIO_RX_NMI_ROUTE;

    Status = GpioReadWritePadCfgReg (
               GpioPad,
               0,
               (B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE),
               TRUE,
               &IntRouteValue
               );
    ASSERT_EFI_ERROR (Status);

    if ((Value & GpioIntSci) == GpioIntSci) {
      GpeEnable = 0x1;
    } else {
      GpeEnable = 0x0;
    }

    Status = GpioReadWriteReg (
               GpioGpeEnableRegister,
               0,
               0,
               GpioPad,
               TRUE,
               TRUE,
               &GpeEnable
               );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  This procedure will set GPIO electrical settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of termination
                                  use GPIO_ELECTRICAL_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadElectricalConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_ELECTRICAL_CONFIG    Value
  )
{
  EFI_STATUS  Status;
  UINT32      TermValue;
  UINT32      PadTolValue;

  Status = EFI_SUCCESS;

  if (((Value & GPIO_CONF_TERM_MASK) >> GPIO_CONF_TERM_BIT_POS) != GpioHardwareDefault) {
    TermValue = ((Value & GPIO_CONF_TERM_MASK) >> (GPIO_CONF_TERM_BIT_POS + 1)) << N_PCH_GPIO_TERM;

    Status = GpioReadWritePadCfgReg (
               GpioPad,
               1,
               B_PCH_GPIO_TERM,
               TRUE,
               &TermValue
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (((Value & GPIO_CONF_PADTOL_MASK) >> GPIO_CONF_PADTOL_BIT_POS) != GpioHardwareDefault) {
    PadTolValue = ((Value & GPIO_CONF_PADTOL_MASK) >> (GPIO_CONF_PADTOL_BIT_POS + 1)) << N_PCH_GPIO_PADTOL;

    Status = GpioReadWritePadCfgReg (
               GpioPad,
               1,
               B_PCH_GPIO_PADTOL,
               TRUE,
               &PadTolValue
               );
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**
  This procedure will set GPIO Reset settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value for Pad Reset Configuration
                                  use GPIO_RESET_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadResetConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_RESET_CONFIG         Value
  )
{
  EFI_STATUS  Status;
  UINT32      ResetValue;

  Status = EFI_SUCCESS;

  if (((Value & GPIO_CONF_RESET_MASK) >> GPIO_CONF_RESET_BIT_POS) != GpioHardwareDefault) {
    ResetValue = ((Value & GPIO_CONF_RESET_MASK) >> (GPIO_CONF_RESET_BIT_POS + 1)) << N_PCH_GPIO_RST_CONF;

    Status = GpioReadWritePadCfgReg (
               GpioPad,
               0,
               B_PCH_GPIO_RST_CONF,
               TRUE,
               &ResetValue
               );
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**
  This procedure will get GPIO Reset settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of Pad Reset Configuration
                                  based on GPIO_RESET_CONFIG

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadResetConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_RESET_CONFIG         *Value
  )
{
  EFI_STATUS  Status;
  UINT32      ResetValue;

  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_RST_CONF,
             FALSE,
             &ResetValue
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Get Reset Type (PadRstCfg)
  //
  *Value = (ResetValue >> (N_PCH_GPIO_RST_CONF - (GPIO_CONF_RESET_BIT_POS + 1))) | (0x1 << GPIO_CONF_RESET_BIT_POS);

  return Status;
}

/**
  This procedure will get GPIO Host Software Pad Ownership for certain group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               Host Ownership register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] HostSwRegVal        Value of Host Software Pad Ownership register
                                  Bit position - PadNumber
                                  Bit value - 0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetHostSwOwnershipForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *HostSwRegVal
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioHostOwnershipRegister,
             Group,
             DwNum,
             0,
             FALSE,
             FALSE,
             HostSwRegVal
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will get GPIO Host Software Pad Ownership for certain group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               Host Ownership register number for current group
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  HostSwRegVal        Value of Host Software Pad Ownership register
                                  Bit position - PadNumber
                                  Bit value - 0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioSetHostSwOwnershipForGroupDw (
  IN GPIO_GROUP                   Group,
  IN UINT32                       DwNum,
  IN UINT32                       HostSwRegVal
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioHostOwnershipRegister,
             Group,
             DwNum,
             0,
             TRUE,
             FALSE,
             &HostSwRegVal
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will get Gpio Pad Host Software Ownership

  @param[in]  GpioPad             GPIO pad
  @param[out] PadHostSwOwn        Value of Host Software Pad Owner
                                  0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetHostSwOwnershipForPad (
  IN GPIO_PAD                 GpioPad,
  OUT UINT32                  *PadHostSwOwn
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioHostOwnershipRegister,
             0,
             0,
             GpioPad,
             FALSE,
             TRUE,
             PadHostSwOwn
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set Gpio Pad Host Software Ownership

  @param[in] GpioPad              GPIO pad
  @param[in] PadHostSwOwn         Pad Host Software Owner
                                  0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetHostSwOwnershipForPad (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadHostSwOwn
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioHostOwnershipRegister,
             0,
             0,
             GpioPad,
             TRUE,
             TRUE,
             &PadHostSwOwn
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will get Gpio Pad Ownership

  @param[in] GpioPad              GPIO pad
  @param[out] PadOwnVal           Value of Pad Ownership

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadOwnership (
  IN  GPIO_PAD                GpioPad,
  OUT GPIO_PAD_OWN            *PadOwnVal
  )
{
  UINT32               Mask;
  UINT32               RegOffset;
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               PadOwnRegValue;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    ASSERT(FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pin number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    ASSERT(FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  // Calculate RegOffset using Pad Ownership offset and GPIO Pad number.
  // One DWord register contains information for 8 pads.
  //
  RegOffset = GpioGroupInfo[GroupIndex].PadOwnOffset + (PadNumber >> 3) * 0x4;

  //
  // Calculate pad bit position within DWord register
  //
  PadNumber %= 8;
  Mask = (BIT1 | BIT0) << (PadNumber * 4);

  PadOwnRegValue = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset));

  *PadOwnVal = (GPIO_PAD_OWN)((PadOwnRegValue & Mask) >> (PadNumber * 4));

  return EFI_SUCCESS;
}

/**
  This procedure will check state of Pad Config Lock for pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] PadCfgLockRegVal    Value of PadCfgLock register
                                  Bit position - PadNumber
                                  Bit value - 0: NotLocked, 1: Locked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetPadCfgLockForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *PadCfgLockRegVal
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioPadConfigLockRegister,
             Group,
             DwNum,
             0,
             FALSE,
             FALSE,
             PadCfgLockRegVal
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will check state of Pad Config Lock for selected pad

  @param[in] GpioPad              GPIO pad
  @param[out] PadCfgLock          PadCfgLock for selected pad
                                  0: NotLocked, 1: Locked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadCfgLock (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *PadCfgLock
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioPadConfigLockRegister,
             0,
             0,
             GpioPad,
             FALSE,
             TRUE,
             PadCfgLock
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will check state of Pad Config Tx Lock for pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLockTx register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] PadCfgLockTxRegVal  Value of PadCfgLockTx register
                                  Bit position - PadNumber
                                  Bit value - 0: NotLockedTx, 1: LockedTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetPadCfgLockTxForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *PadCfgLockTxRegVal
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioPadLockOutputRegister,
             Group,
             DwNum,
             0,
             FALSE,
             FALSE,
             PadCfgLockTxRegVal
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will check state of Pad Config Tx Lock for selected pad

  @param[in] GpioPad              GPIO pad
  @param[out] PadCfgLock          PadCfgLockTx for selected pad
                                  0: NotLockedTx, 1: LockedTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadCfgLockTx (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *PadCfgLockTx
  )
{
  EFI_STATUS  Status;

  Status = GpioReadWriteReg (
             GpioPadLockOutputRegister,
             0,
             0,
             GpioPad,
             FALSE,
             TRUE,
             PadCfgLockTx
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will clear PadCfgLock for selected pads within one group.
  This function should be used only inside SMI.

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToUnlock        Bitmask for pads which are going to be unlocked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotUnlock, 1: Unlock

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgForGroupDw (
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToUnlock
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadConfigLockRegister,
             TRUE,
             Group,
             DwNum,
             PadsToUnlock,
             0,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will clear PadCfgLock for selected pad.
  This function should be used only inside SMI.

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfg (
    IN GPIO_PAD                   GpioPad
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadConfigLockRegister,
             TRUE,
             0,
             0,
             0,
             GpioPad,
             TRUE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set PadCfgLock for selected pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToLock          Bitmask for pads which are going to be locked
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotLock, 1: Lock

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioLockPadCfgForGroupDw (
  IN GPIO_GROUP                   Group,
  IN UINT32                       DwNum,
  IN UINT32                       PadsToLock
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadConfigLockRegister,
             FALSE,
             Group,
             DwNum,
             PadsToLock,
             0,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set PadCfgLock for selected pad

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioLockPadCfg (
  IN GPIO_PAD                   GpioPad
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadConfigLockRegister,
             FALSE,
             0,
             0,
             0,
             GpioPad,
             TRUE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will clear PadCfgLockTx for selected pads within one group.
  This function should be used only inside SMI.

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLockTx register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToUnlockTx      Bitmask for pads which are going to be unlocked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotUnLockTx, 1: LockTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgTxForGroupDw (
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToUnlockTx
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadLockOutputRegister,
             TRUE,
             Group,
             DwNum,
             PadsToUnlockTx,
             0,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will clear PadCfgLockTx for selected pad.
  This function should be used only inside SMI.

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgTx (
  IN GPIO_PAD                   GpioPad
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadLockOutputRegister,
             TRUE,
             0,
             0,
             0,
             GpioPad,
             TRUE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set PadCfgLockTx for selected pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToLockTx        Bitmask for pads which are going to be locked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotLockTx, 1: LockTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioLockPadCfgTxForGroupDw (
  IN GPIO_GROUP                   Group,
  IN UINT32                       DwNum,
  IN UINT32                       PadsToLockTx
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadLockOutputRegister,
             FALSE,
             Group,
             DwNum,
             PadsToLockTx,
             0,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set PadCfgLockTx for selected pad

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioLockPadCfgTx (
  IN GPIO_PAD                   GpioPad
  )
{
  EFI_STATUS           Status;

  Status = GpioLockPadsUsingSbi (
             GpioPadLockOutputRegister,
             FALSE,
             0,
             0,
             0,
             GpioPad,
             TRUE
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}


/**
  This procedure will get Group to GPE mapping.

  @param[out] GroupToGpeDw0       GPIO group to be mapped to GPE_DW0
  @param[out] GroupToGpeDw1       GPIO group to be mapped to GPE_DW1
  @param[out] GroupToGpeDw2       GPIO group to be mapped to GPE_DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetGroupToGpeDwX (
  IN GPIO_GROUP               *GroupToGpeDw0,
  IN GPIO_GROUP               *GroupToGpeDw1,
  IN GPIO_GROUP               *GroupToGpeDw2
  )
{
  UINT32               Data32;
  UINT32               PchPwrmBase;
  GPIO_GROUP           GpioGroupOffset;

  GpioGroupOffset = GpioGetLowestGroup ();


  PchPwrmBaseGet (&PchPwrmBase);

  Data32 = MmioRead32 ((UINTN) (PchPwrmBase + R_PCH_PWRM_GPIO_CFG));

  *GroupToGpeDw0 = ((Data32 & B_PCH_PWRM_GPIO_CFG_GPE0_DW0) >> N_PCH_PWRM_GPIO_CFG_GPE0_DW0) + GpioGroupOffset;
  *GroupToGpeDw1 = ((Data32 & B_PCH_PWRM_GPIO_CFG_GPE0_DW1) >> N_PCH_PWRM_GPIO_CFG_GPE0_DW1) + GpioGroupOffset;
  *GroupToGpeDw2 = ((Data32 & B_PCH_PWRM_GPIO_CFG_GPE0_DW2) >> N_PCH_PWRM_GPIO_CFG_GPE0_DW2) + GpioGroupOffset;

  return EFI_SUCCESS;
}

/**
  This procedure will set Group to GPE mapping.

  @param[in]  GroupToGpeDw0       GPIO group to be mapped to GPE_DW0
  @param[in]  GroupToGpeDw1       GPIO group to be mapped to GPE_DW1
  @param[in]  GroupToGpeDw2       GPIO group to be mapped to GPE_DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetGroupToGpeDwX (
  IN GPIO_GROUP                GroupToGpeDw0,
  IN GPIO_GROUP                GroupToGpeDw1,
  IN GPIO_GROUP                GroupToGpeDw2
  )
{
  UINT32               Data32Or;
  UINT32               Data32And;
  UINT32               PchPwrmBase;
  GPIO_GROUP           GpioGroupLowest;
  GPIO_GROUP           GpioGroupHighest;

  GpioGroupLowest = GpioGetLowestGroup ();
  GpioGroupHighest = GpioGetHighestGroup ();

  //
  // Check if group argument exceeds GPIO group range
  //
  if (((UINT32)GroupToGpeDw0 < GpioGroupLowest) || ((UINT32)GroupToGpeDw0 > GpioGroupHighest) ||
    ((UINT32)GroupToGpeDw1 < GpioGroupLowest) || ((UINT32)GroupToGpeDw1 > GpioGroupHighest) ||
    ((UINT32)GroupToGpeDw2 < GpioGroupLowest) || ((UINT32)GroupToGpeDw2 > GpioGroupHighest)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument exceeds GPIO group range\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if each group number is unique
  //
  if ((GroupToGpeDw0 == GroupToGpeDw1) ||
    (GroupToGpeDw0 == GroupToGpeDw2) ||
    (GroupToGpeDw1 == GroupToGpeDw2)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Values in GPE0_DWx registers are 0 based (GPP_A = 0h)
  //
  GroupToGpeDw0 = GpioGetGroupIndexFromGroup(GroupToGpeDw0);
  GroupToGpeDw1 = GpioGetGroupIndexFromGroup(GroupToGpeDw1);
  GroupToGpeDw2 = GpioGetGroupIndexFromGroup(GroupToGpeDw2);

  PchPwrmBaseGet (&PchPwrmBase);

  //
  // Program GPIO_CFG (PMRMBASE + 120h) register
  //
  Data32And = (UINT32) ~(B_PCH_PWRM_GPIO_CFG_GPE0_DW2 | B_PCH_PWRM_GPIO_CFG_GPE0_DW1 | B_PCH_PWRM_GPIO_CFG_GPE0_DW0);
  Data32Or  = (UINT32)((GroupToGpeDw2 << N_PCH_PWRM_GPIO_CFG_GPE0_DW2) |
              (GroupToGpeDw1 << N_PCH_PWRM_GPIO_CFG_GPE0_DW1) |
              (GroupToGpeDw0 << N_PCH_PWRM_GPIO_CFG_GPE0_DW0));

  MmioAndThenOr32 (
    (UINTN) (PchPwrmBase + R_PCH_PWRM_GPIO_CFG),
    Data32And,
    Data32Or
    );

  Data32And = (UINT32) ~(B_PCH_PCR_GPIO_MISCCFG_GPE0_DW2 | B_PCH_PCR_GPIO_MISCCFG_GPE0_DW1 | B_PCH_PCR_GPIO_MISCCFG_GPE0_DW0);
  Data32Or = (UINT32)((GroupToGpeDw2 << N_PCH_PCR_GPIO_MISCCFG_GPE0_DW2) |
             (GroupToGpeDw1 << N_PCH_PCR_GPIO_MISCCFG_GPE0_DW1) |
             (GroupToGpeDw0 << N_PCH_PCR_GPIO_MISCCFG_GPE0_DW0));
  //
  // Program MISCCFG register for Community 0
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (PID_GPIOCOM0, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 1
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (PID_GPIOCOM1, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 2
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (PID_GPIOCOM2, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 3
  //
  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (PID_GPIOCOM3, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  return EFI_SUCCESS;
}

/**
  This procedure will get GPE number for provided GpioPad.
  PCH allows to configure mapping between GPIO groups and related GPE (GpioSetGroupToGpeDwX())
  what results in the fact that certain Pad can cause different General Purpose Event. Only three
  GPIO groups can be mapped to cause unique GPE (1-tier), all others groups will be under one common
  event (GPE_111 for 2-tier).

  1-tier:
  Returned GpeNumber is in range <0,95>. GpioGetGpeNumber() can be used
  to determine what _LXX ACPI method would be called on event on selected GPIO pad

  2-tier:
  Returned GpeNumber is 0x6F (111). All GPIO pads which are not mapped to 1-tier GPE
  will be under one master GPE_111 which is linked to _L6F ACPI method. If it is needed to determine
  what Pad from 2-tier has caused the event, _L6F method should check GPI_GPE_STS and GPI_GPE_EN
  registers for all GPIO groups not mapped to 1-tier GPE.

  @param[in]  GpioPad             GPIO pad
  @param[out] GpeNumber           GPE number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetGpeNumber (
  IN  GPIO_PAD                  GpioPad,
  OUT UINT32                    *GpeNumber
  )
{
  GPIO_GROUP           GroupToGpeDw0;
  GPIO_GROUP           GroupToGpeDw1;
  GPIO_GROUP           GroupToGpeDw2;
  GPIO_GROUP           GpioGroupLowest;
  GPIO_GROUP           GpioGroupHighest;
  UINT32               GroupIndex;
  GPIO_GROUP           Group;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  Group = GpioGetGroupFromGpioPad (GpioPad);
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  GpioGroupLowest = GpioGetLowestGroup ();
  GpioGroupHighest = GpioGetHighestGroup ();

  //
  // Check if group argument exceeds GPIO group range
  //
  if ((GroupIndex < GpioGetGroupIndexFromGroup (GpioGroupLowest)) || (GroupIndex > GpioGetGroupIndexFromGroup (GpioGroupHighest))) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get GPIO groups mapping to 1-tier GPE
  //
  GpioGetGroupToGpeDwX (&GroupToGpeDw0,&GroupToGpeDw1,&GroupToGpeDw2);

  if (Group == GroupToGpeDw0) {
    *GpeNumber = PadNumber;
  } else if (Group== GroupToGpeDw1) {
    *GpeNumber = PadNumber + 32;
  } else if (Group == GroupToGpeDw2) {
    *GpeNumber = PadNumber + 64;
  } else {
    //
    // If Group number doesn't match any of above then
    // it means than certain pad is routed to 2-tier GPE
    // which all are under GPE_111 (0x6F)
    //
    *GpeNumber = PCH_GPIO_2_TIER_MASTER_GPE_NUMBER;
  }

  return EFI_SUCCESS;
}

/**
  This procedure is used to clear SMI STS for a specified Pad

  @param[in]  GpioPad             GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioClearGpiSmiSts (
  IN GPIO_PAD                   GpioPad
  )
{
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if group has GPI SMI register
  //
  if (GpioGroupInfo[GroupIndex].SmiStsOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Clear all GPI SMI Status bits by writing '1'
  //
  MmioWrite32 (
    PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].SmiStsOffset),
    (UINT32)(BIT0 << PadNumber)
    );

  return EFI_SUCCESS;
}

/**
  This procedure is used by PchSmiDispatcher and will clear
  all GPI SMI Status bits

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioClearAllGpiSmiSts (
  VOID
  )
{
  UINT32               GroupIndex;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  for (GroupIndex = 0; GroupIndex < GpioGroupInfoLength; GroupIndex++) {
    //
    // Check if group has GPI SMI register
    //
    if (GpioGroupInfo[GroupIndex].SmiStsOffset == NO_REGISTER_FOR_PROPERTY) {
      continue;
    }
    //
    // Clear all GPI SMI Status bits by writing '1'
    //
    MmioWrite32 (
      PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].SmiStsOffset),
      (UINT32)0xFFFFFFFF
      );
  }
  return EFI_SUCCESS;
}

/**
  This procedure is used to disable all GPI SMI

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioDisableAllGpiSmi (
  VOID
  )
{
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINT32               GroupIndex;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  for (GroupIndex = 0; GroupIndex < GpioGroupInfoLength; GroupIndex++) {
    //
    // Check if group has GPI SMI register
    //
    if (GpioGroupInfo[GroupIndex].SmiEnOffset == NO_REGISTER_FOR_PROPERTY) {
      continue;
    }

    //
    // Disable all GPI SMI
    //
    MmioWrite32 (
      PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].SmiEnOffset),
      (UINT32)0x0
      );
  }
  return EFI_SUCCESS;
}

/**
  This procedure is used to register GPI SMI dispatch function.

  @param[in]  GpioPad             GPIO pad
  @param[out] GpiNum              GPI number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetGpiSmiNum (
  IN GPIO_PAD          GpioPad,
  OUT UINTN            *GpiNum
  )
{
  UINT32               GroupIndex;
  UINT32               Index;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    return EFI_INVALID_PARAMETER;
  }

  *GpiNum = 0;

  for (Index = 0; Index < (UINT32)GroupIndex; Index++) {
    *GpiNum += (UINTN)(GpioGroupInfo[Index].PadPerGroup);
  }
  *GpiNum += (UINTN)PadNumber;

  return EFI_SUCCESS;
}

/**
  This procedure is used to check GPIO inputs belongs to 2 tier or 1 tier architecture

  @param[in]  GpioPad             GPIO pad

  @retval     Data                0 means 1-tier, 1 means 2-tier
**/
BOOLEAN
GpioCheckFor2Tier (
  IN GPIO_PAD                  GpioPad
  )
{
  UINT32               Data32;

  GpioGetGpeNumber (GpioPad, &Data32);
  if(Data32 == PCH_GPIO_2_TIER_MASTER_GPE_NUMBER) {
    return TRUE;
  }

  return FALSE;
}

/**
  This procedure is used to clear GPE STS for a specified GpioPad

  @param[in]  GpioPad             GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioClearGpiGpeSts (
  IN GPIO_PAD                  GpioPad
  )
{
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if group has GPI GPE register
  //
  if (GpioGroupInfo[GroupIndex].GpiGpeStsOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }

  // Check for 2-tier
  if(!(GpioCheckFor2Tier (GpioPad))) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Clear all GPI SMI Status bits by writing '1'
  //
  MmioWrite32 (
    PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeStsOffset),
    (UINT32)(BIT0 << PadNumber)
    );

  return EFI_SUCCESS;
}

/**
  This procedure is used to read GPE STS for a specified Pad

  @param[in]  GpioPad             GPIO pad
  @param[out] Data                GPE STS data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetGpiGpeSts (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32*                  Data
  )
{
  UINT32               Data32;
  UINT32               Mask;
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  *Data = 0xFFFFFFFF;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if group has GPI GPE register
  //
  if (GpioGroupInfo[GroupIndex].GpiGpeStsOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }

  // Check for 2-tier
  if(!(GpioCheckFor2Tier (GpioPad))) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read GPI GPE Status bits
  //
  Data32 = MmioRead32(
    PCH_PCR_ADDRESS(GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeStsOffset)
    );

  Mask = (UINT32)(BIT0 << PadNumber);
  Data32 = (Data32 & Mask) >> PadNumber;
  *Data = Data32;

  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO Input Rout SCI

  @param[in] GpioPad             GPIO pad
  @param[in] Value                Value for GPIRoutSCI

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetGpiRoutSci (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  )
{
  EFI_STATUS           Status;

  Value <<= N_PCH_GPIO_RX_SCI_ROUTE;
  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_RX_SCI_ROUTE,
             TRUE,
             &Value
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set GPIO Input Rout SMI

  @param[in] GpioPad             GPIO pad
  @param[in] Value                Value for GPIRoutSMI

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetGpiRoutSmi (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  )
{
  EFI_STATUS  Status;

  Value <<= N_PCH_GPIO_RX_SMI_ROUTE;
  Status = GpioReadWritePadCfgReg (
             GpioPad,
             0,
             B_PCH_GPIO_RX_SMI_ROUTE,
             TRUE,
             &Value
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set GPI SMI Enable setting for selected pad

  @param[in]  GpioPad               GPIO pad
  @param[in]  PadGpiSmiEn         GPI SMI Enable setting for selected pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetGpiSmiPadEn (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadGpiSmiEn
  )
{
  GPIO_GROUP           Group;
  GPIO_GROUP           GpioGroupOffset;
  UINT32               NumberOfGroups;
  EFI_STATUS  Status;

  GpioGroupOffset = GpioGetLowestGroup ();
  NumberOfGroups = GpioGetNumberOfGroups ();

  Group = GpioGetGroupFromGpioPad (GpioPad);

  //
  // Check if group argument exceeds GPIO group range
  //
  if ((Group < GpioGroupOffset) || (Group >= NumberOfGroups + GpioGroupOffset)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioReadWriteReg (
             GpioSmiEnableRegister,
             Group,
             0,
             GpioPad,
             TRUE,
             TRUE,
             &PadGpiSmiEn
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This procedure will set GPI General Purpose Event Enable setting for selected pad

  @param[in]  GpioPad               GPIO pad
  @param[in]  PadGpiGpeEn         GPI General Purpose Event Enable setting for selected pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetGpiGpePadEn (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadGpiGpeEn
  )
{
  GPIO_GROUP           Group;
  GPIO_GROUP           GpioGroupOffset;
  UINT32               NumberOfGroups;
  EFI_STATUS           Status;

  GpioGroupOffset = GpioGetLowestGroup ();
  NumberOfGroups = GpioGetNumberOfGroups ();

  Group = GpioGetGroupFromGpioPad (GpioPad);

  //
  // Check if group argument exceeds GPIO group range
  //
  if ((Group < GpioGroupOffset) || (Group >= NumberOfGroups + GpioGroupOffset)) {
    return EFI_INVALID_PARAMETER;
  }


  Status = GpioReadWriteReg (
             GpioGpeEnableRegister,
             Group,
             0,
             GpioPad,
             TRUE,
             TRUE,
             &PadGpiGpeEn
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Check if given GPIO Pad is locked

  @param[in]  GroupIndex                GPIO group index
  @param[in]  PadNumber                 GPIO pad number

  @retval TRUE                          Pad is locked
  @retval FALSE                         Pad is not locked
**/
BOOLEAN
GpioIsPadLocked (
  IN  UINT32                 GroupIndex,
  IN  GPIO_PAD               PadNumber
  )
{
  UINT32               RegVal;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  // Read PADCFGLOCK register
  //
  RegVal = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].PadCfgLockOffset));

  return (((RegVal >> PadNumber) & 0x1) == 1);
}

/**
  Locks multiple GPIO pads using GPIO_INIT_CONFIG array.
  Only locking is applied and no other GPIO pad configuration is changed.

  @param[in] NumberOfItems              Number of GPIO pads to be locked
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
  @retval EFI_UNSUPPORTED               Incorrect GPIO pad definition
**/
static
EFI_STATUS
GpioLockPads (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  )
{
  UINT32               Index;
  UINT32               PadsToLock[V_PCH_GPIO_GROUP_MAX];
  UINT32               PadsToLockTx[V_PCH_GPIO_GROUP_MAX];
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  GPIO_GROUP           GpioGroupOffset;
  UINT32               NumberOfGroups;
  GPIO_PAD_OWN         PadOwnVal;
  GPIO_INIT_CONFIG     *GpioData;
  GPIO_GROUP           Group;
  UINT32               GroupIndex;
  UINT32               PadNumber;
  PCH_SERIES           PchSeries;

  PchSeries = GetPchSeries ();
  PadOwnVal = GpioPadOwnHost;

  ZeroMem (PadsToLock, sizeof (PadsToLock));
  ZeroMem (PadsToLockTx, sizeof (PadsToLockTx));

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupOffset = GpioGetLowestGroup ();
  NumberOfGroups = GpioGetNumberOfGroups ();

  for (Index = 0; Index < NumberOfItems; Index ++) {

    GpioData   = &GpioInitTableAddress[Index];

    Group      = GpioGetGroupFromGpioPad (GpioData->GpioPad);
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioData->GpioPad);
    PadNumber  = GpioGetPadNumberFromGpioPad (GpioData->GpioPad);

    //
    // Checking GroupIndex to avoid Buffer Overflows or Array Out of Index
    //
    if (GroupIndex >= V_PCH_GPIO_GROUP_MAX) {
      ASSERT (FALSE);
      continue;
    }

    //
    // Check if group argument exceeds GPIO group range
    //
    if ((Group < GpioGroupOffset) || (Group >= NumberOfGroups + GpioGroupOffset)) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Check if legal pin number
    //
    if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
      return EFI_INVALID_PARAMETER;
    }

    //
    // Check if selected GPIO Pad is not owned by CSME/ISH
    //
    GpioGetPadOwnership (GpioData->GpioPad, &PadOwnVal);

    if (PadOwnVal != GpioPadOwnHost) {
      continue;
    }

    //
    // Update information on Pad Configuration Lock
    //
    PadsToLock[GroupIndex] |= ((GpioData->GpioConfig.LockConfig >> 0x1) & 0x1) << PadNumber;

    //
    // Update information on Pad Configuration Lock Tx
    //
    PadsToLockTx[GroupIndex] |= ((GpioData->GpioConfig.LockConfig >> 0x2) & 0x1) << PadNumber;
  }

  for (Index = 0; Index < NumberOfGroups; Index++) {
    //
    // Write Pad Configuration Lock
    //
    if (PadsToLock[Index] != 0) {
      GpioLockPadCfgForGroupDw (Index + GpioGroupOffset, 0, PadsToLock[Index]);
    }

    //
    // Write Pad Configuration Lock Tx
    //
    if (PadsToLockTx[Index] != 0) {
      GpioLockPadCfgTxForGroupDw (Index + GpioGroupOffset, 0, PadsToLockTx[Index]);
    }
  }

  return EFI_SUCCESS;
}

/**
  Locks GPIO pads according to GPIO_INIT_CONFIG array from
  gPlatformGpioConfigGuid HOB. Only locking is applied and no other GPIO pad
  configuration is changed.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_NOT_FOUND                 gPlatformGpioConfigGuid not found
**/
EFI_STATUS
GpioLockGpios (
  VOID
  )
{
  EFI_HOB_GUID_TYPE *GpioConfigHob;
  GPIO_INIT_CONFIG  *GpioConfig;
  UINT16            GpioConfigSize;

  GpioConfigHob = GetFirstGuidHob (&gPlatformGpioConfigGuid);
  if (GpioConfigHob == NULL) {
    return EFI_NOT_FOUND;
  }
  ASSERT (GET_GUID_HOB_DATA_SIZE (GpioConfigHob) % sizeof (GpioConfig[0]) == 0);
  GpioConfigSize = GET_GUID_HOB_DATA_SIZE (GpioConfigHob) / sizeof (GpioConfig[0]);
  GpioConfig = GET_GUID_HOB_DATA (GpioConfigHob);
  GpioLockPads (GpioConfigSize, GpioConfig);

  return EFI_SUCCESS;
}

/**
  Unlocks all PCH GPIO pads

  @retval None
**/
VOID
GpioUnlockAllGpios (
  VOID
  )
{
  GPIO_GROUP           GpioGroupOffset;
  UINT32               NumberOfGroups;
  UINT32               Index;

  GpioGroupOffset = GpioGetLowestGroup ();
  NumberOfGroups = GpioGetNumberOfGroups ();

  for (Index = 0; Index < NumberOfGroups; Index++) {
    //
    // Reset Pad Configuration Lock
    //
    GpioUnlockPadCfgForGroupDw (Index + GpioGroupOffset, 0, 0xFFFFFFFF);

    //
    // Reset Pad Configuration Lock Tx
    //
    GpioUnlockPadCfgTxForGroupDw (Index + GpioGroupOffset, 0, 0xFFFFFFFF);
  }
}

