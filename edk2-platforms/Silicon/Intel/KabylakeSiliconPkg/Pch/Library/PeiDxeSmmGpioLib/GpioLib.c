/** @file
  This file contains routines for GPIO

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "GpioLibrary.h"

/**
  This procedure will check if GpioPad is owned by host.

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is owned by host
  @retval FALSE            GPIO pad is not owned by host and should not be used with GPIO lib API
**/
BOOLEAN
GpioIsPadHostOwned (
  IN GPIO_PAD             GpioPad
  )
{
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_PAD_OWN         PadOwnVal;

  //
  // Check if selected GPIO Pad is not owned by CSME/ISH
  // If GPIO is not owned by Host all access to PadCfg will be dropped
  //
  GpioGetPadOwnership (GpioPad, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
    PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Gpio pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return FALSE;
  }

  return TRUE;
}

/**
  This procedure will check if GpioPad argument is valid.
  Function will check below conditions:
   - GpioPad represents a pad for current PCH
   - GpioPad belongs to valid GpioGroup
   - GPIO PadNumber is not greater than number of pads for this group

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is valid and can be used with GPIO lib API
  @retval FALSE            GPIO pad is invalid and cannot be used with GPIO lib API
**/
BOOLEAN
GpioIsPadValid (
  IN GPIO_PAD             GpioPad
  )
{
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               GroupIndex;
  UINT32               PadNumber;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return FALSE;
  }

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN) GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return FALSE;
  }

  if ((GroupIndex < GpioGetGroupIndexFromGroup (GpioGetLowestGroup ())) ||
      (GroupIndex > GpioGetGroupIndexFromGroup (GpioGetHighestGroup ()))) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) is not within range of possible groups for this PCH\n", GroupIndex));
    return FALSE;
  }

  //
  // Check if legal pin number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for this group\n", PadNumber));
    return FALSE;
  }

  return TRUE;
}

/**
  This procedure will check if GpioGroup argument is correct and
  supplied DW reg number can be used for this group to access DW registers.
  Function will check below conditions:
   - Valid GpioGroup
   - DwNum is has valid value for this group

  @param[in] Group        GPIO group
  @param[in] DwNum        Register number for current group (parameter applicable in accessing whole register).
                          For group which has less then 32 pads per group DwNum must be 0.

  @retval TRUE             DW Reg number and GpioGroup is valid
  @retval FALSE            DW Reg number and GpioGroup is invalid
**/
STATIC
BOOLEAN
GpioIsGroupAndDwNumValid (
  IN GPIO_GROUP             Group,
  IN UINT32                 DwNum
  )
{
  UINT32               GroupIndex;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN) GroupIndex >= GpioGroupInfoLength) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) exceeds GPIO group range\n", GroupIndex));
    return FALSE;
  }

  if ((GroupIndex < GpioGetGroupIndexFromGroup (GpioGetLowestGroup ())) ||
      (GroupIndex > GpioGetGroupIndexFromGroup (GpioGetHighestGroup ()))) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) is not within range of possible groups for this PCH\n", GroupIndex));
    return FALSE;
  }

  //
  // Check if DwNum argument does not exceed number of DWord registers
  // resulting from available pads for certain group
  //
  if (DwNum > GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup - 1)){
    return FALSE;
  }

  return TRUE;
}

/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[out] PadCfgRegValue      Read data

  @retval none
**/
VOID
GpioReadPadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  OUT UINT32              *PadCfgRegValue
  )
{
  UINT32               PadCfgReg;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               GroupIndex;
  UINT32               PadNumber;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = GpioGroupInfo[GroupIndex].PadCfgOffset + PAD_CFG_SIZE * PadNumber + 0x4 * DwReg;

  *PadCfgRegValue = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg));
}

/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[in] PadCfgAndMask        Mask to be AND'ed with PADCFG reg value
  @param[in] PadCfgOrMask         Mask to be OR'ed with PADCFG reg value

  @retval none
**/
VOID
GpioWritePadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  IN UINT32               PadCfgAndMask,
  IN UINT32               PadCfgOrMask
  )
{
  UINT32               PadCfgReg;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               GroupIndex;
  UINT32               PadNumber;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = GpioGroupInfo[GroupIndex].PadCfgOffset + PAD_CFG_SIZE * PadNumber + 0x4 * DwReg;

  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg),
    PadCfgAndMask,
    PadCfgOrMask
    );
}

//
// Possible registers to be accessed using GpioReadReg()/GpioWriteReg() functions
//
typedef enum {
  GpioHostOwnershipRegister = 0,
  GpioGpeEnableRegister,
  GpioGpeStatusRegister,
  GpioSmiEnableRegister,
  GpioSmiStatusRegister,
  GpioNmiEnableRegister,
  GpioPadConfigLockRegister,
  GpioPadLockOutputRegister
} GPIO_REG;

/**
  This procedure will read GPIO register

  @param[in] RegType              GPIO register type
  @param[in] Group                GPIO group
  @param[in] DwNum                Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] ReadVal             Read data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
STATIC
EFI_STATUS
GpioReadReg (
  IN GPIO_REG               RegType,
  IN GPIO_GROUP             Group,
  IN UINT32                 DwNum,
  OUT UINT32                *ReadVal
  )
{
  UINT32               RegOffset;
  UINT32               GroupIndex;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  RegOffset = NO_REGISTER_FOR_PROPERTY;
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  switch (RegType) {
    case GpioHostOwnershipRegister:
      RegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset;
      break;
    case GpioGpeEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeEnOffset;
      break;
    case GpioGpeStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeStsOffset;
      break;
    case GpioSmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiEnOffset;
      break;
    case GpioSmiStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset;
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
    return EFI_UNSUPPORTED;
  }

  //
  // If there are more then 32 pads per group then certain
  // group information would be split into more then one DWord register.
  //
  if ((RegType == GpioPadConfigLockRegister) || (RegType == GpioPadLockOutputRegister)) {
    //
    // PadConfigLock and OutputLock registers when used for group containing more than 32 pads
    // are not placed in a continuous way, e.g:
    // 0x0 - PadConfigLock_DW0
    // 0x4 - OutputLock_DW0
    // 0x8 - PadConfigLock_DW1
    // 0xC - OutputLock_DW1
    //
    RegOffset += DwNum * 0x8;
  } else {
    RegOffset += DwNum * 0x4;
  }

  *ReadVal = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset));

  return EFI_SUCCESS;
}

/**
  This procedure will write GPIO register

  @param[in] RegType              GPIO register type
  @param[in] Group                GPIO group
  @param[in] DwNum                Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] RegAndMask           Mask which will be AND'ed with register value
  @param[in] RegOrMask            Mask which will be OR'ed with register value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
STATIC
EFI_STATUS
GpioWriteReg (
  IN GPIO_REG               RegType,
  IN GPIO_GROUP             Group,
  IN UINT32                 DwNum,
  IN UINT32                 RegAndMask,
  IN UINT32                 RegOrMask
  )
{
  UINT32               RegOffset;
  UINT32               GroupIndex;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;

  RegOffset = NO_REGISTER_FOR_PROPERTY;
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  switch (RegType) {
    case GpioHostOwnershipRegister:
      RegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset;
      break;
    case GpioGpeEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeEnOffset;
      break;
    case GpioGpeStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeStsOffset;
      break;
    case GpioSmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiEnOffset;
      break;
    case GpioSmiStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset;
      break;
    case GpioNmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].NmiEnOffset;
      break;
    case GpioPadConfigLockRegister:
    case GpioPadLockOutputRegister:
      ASSERT (FALSE);
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  //
  // Check if selected register exists
  //
  if (RegOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_UNSUPPORTED;
  }

  //
  // If there are more then 32 pads per group then certain
  // group information would be split into more then one DWord register.
  //
  RegOffset += DwNum * 0x4;

  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset),
    RegAndMask,
    RegOrMask
    );

  return EFI_SUCCESS;
}

/**
  This procedure will write GPIO Lock/LockTx register using SBI.

  @param[in] RegType              GPIO register (Lock or LockTx)
  @param[in] Group                GPIO group number
  @param[in] DwNum                Register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] LockRegAndMask       Mask which will be AND'ed with Lock register value
  @param[in] LockRegOrMask        Mask which will be Or'ed with Lock register value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
STATIC
EFI_STATUS
GpioWriteLockReg (
  IN GPIO_REG                  RegType,
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    LockRegAndMask,
  IN UINT32                    LockRegOrMask
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

  RegOffset = NO_REGISTER_FOR_PROPERTY;
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

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
    return EFI_UNSUPPORTED;
  }

  //
  // If there are more then 32 pads per group then certain
  // group information would be split into more then one DWord register.
  // PadConfigLock and OutputLock registers when used for group containing more than 32 pads
  // are not placed in a continuous way, e.g:
  // 0x0 - PadConfigLock_DW0
  // 0x4 - OutputLock_DW0
  // 0x8 - PadConfigLock_DW1
  // 0xC - OutputLock_DW1
  //
  RegOffset += DwNum *0x8;

  GpioGetPadCfgLockForGroupDw (Group, DwNum, &OldPadCfgLockRegVal);

  NewPadCfgLockRegVal = (OldPadCfgLockRegVal & LockRegAndMask) | LockRegOrMask;

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
  This internal procedure will calculate GPIO_RESET_CONFIG value  (new type)
  based on provided PadRstCfg for a specific GPIO Pad.

  @param[in]  GpioPad               GPIO Pad
  @param[in]  PadRstCfg             GPIO PadRstCfg value

  @retval GpioResetConfig           GPIO Reset configuration (new type)
**/
GPIO_RESET_CONFIG
GpioResetConfigFromPadRstCfg (
  IN  GPIO_PAD           GpioPad,
  IN  UINT32             PadRstCfg
  )
{
  GPIO_GROUP           Group;

  static GPIO_RESET_CONFIG  GppPadRstCfgToGpioResetConfigMap[] = {
                              GpioResumeReset,
                              GpioHostDeepReset,
                              GpioPlatformReset};
  static GPIO_RESET_CONFIG  GpdPadRstCfgToGpioResetConfigMap[] = {
                              GpioDswReset,
                              GpioHostDeepReset,
                              GpioPlatformReset,
                              GpioResumeReset};

  Group = GpioGetGroupFromGpioPad (GpioPad);

  if ((Group == GPIO_SKL_LP_GROUP_GPD) || (Group == GPIO_SKL_H_GROUP_GPD)) {
    ASSERT (PadRstCfg < 4);
    if (PadRstCfg >= 4) return GpioResumeReset;
    return GpdPadRstCfgToGpioResetConfigMap[PadRstCfg];
  } else {
    ASSERT (PadRstCfg < 3);
    if (PadRstCfg >= 3) return GpioResumeReset;
    return GppPadRstCfgToGpioResetConfigMap[PadRstCfg];
  }
}

/**
  This internal procedure will calculate PadRstCfg register value based
  on provided GPIO Reset configuration for a certain pad.

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  GpioResetConfig           GPIO Reset configuration
  @param[out] PadRstCfg                 GPIO PadRstCfg value

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid configuration
**/
EFI_STATUS
GpioPadRstCfgFromResetConfig (
  IN  GPIO_PAD           GpioPad,
  IN  GPIO_RESET_CONFIG  GpioResetConfig,
  OUT UINT32             *PadRstCfg
  )
{
  GPIO_GROUP           Group;

  Group = GpioGetGroupFromGpioPad (GpioPad);

  switch (GpioResetConfig) {
    case GpioResetDefault:
      *PadRstCfg = 0x0;
      break;
    case GpioResetPwrGood:  //old reset type
      *PadRstCfg = V_PCH_GPIO_RST_CONF_POW_GOOD;
      break;
    case GpioResetDeep:  //old reset type
    case GpioHostDeepReset:  //new reset type
      *PadRstCfg = V_PCH_GPIO_RST_CONF_DEEP_RST;
      break;
    case GpioResetNormal:  //old reset type
    case GpioPlatformReset:  //new reset type
      *PadRstCfg = V_PCH_GPIO_RST_CONF_GPIO_RST;
      break;
    case GpioResetResume:  //old reset type
      if ((Group == GPIO_SKL_LP_GROUP_GPD) || (Group == GPIO_SKL_H_GROUP_GPD)) {
        *PadRstCfg = V_PCH_GPIO_RST_CONF_RESUME_RST;
      } else {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Only GPD group pads can use GpioResetResume (Group=%d, Pad=%d)!\n", GpioGetGroupIndexFromGpioPad (GpioPad), GpioGetPadNumberFromGpioPad (GpioPad)));
        return EFI_INVALID_PARAMETER;
      }
      break;
    case GpioResumeReset:  //new reset type
      if ((Group == GPIO_SKL_LP_GROUP_GPD) || (Group == GPIO_SKL_H_GROUP_GPD)) {
        *PadRstCfg = V_PCH_GPIO_RST_CONF_RESUME_RST;
      } else {
        *PadRstCfg = V_PCH_GPIO_RST_CONF_POW_GOOD;
      }
      break;
    case GpioDswReset:  //new reset type
      if ((Group == GPIO_SKL_LP_GROUP_GPD) || (Group == GPIO_SKL_H_GROUP_GPD)) {
        *PadRstCfg = V_PCH_GPIO_RST_CONF_POW_GOOD;
      } else {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Only GPD group pads can use GpioDswReset (Group=%d, Pad=%d)!\n", GpioGetGroupIndexFromGpioPad (GpioPad), GpioGetPadNumberFromGpioPad (GpioPad)));
        return EFI_INVALID_PARAMETER;
      }
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  return EFI_SUCCESS;
}

/**
  This internal procedure will get GPIO_CONFIG data from PADCFG registers value

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  PadCfgDwReg               PADCFG DWx register values
  @param[out] GpioData                  GPIO Configuration data

  @retval Status
**/
STATIC
VOID
GpioConfigFromPadCfgRegValue (
  IN GPIO_PAD      GpioPad,
  IN CONST UINT32  *PadCfgDwReg,
  OUT GPIO_CONFIG  *GpioConfig
  )
{
  UINT32               PadRstCfg;

  //
  // Get Reset Type (PadRstCfg)
  //
  PadRstCfg = (PadCfgDwReg[0] & B_PCH_GPIO_RST_CONF) >> N_PCH_GPIO_RST_CONF;

  GpioConfig->PowerConfig = GpioResetConfigFromPadRstCfg (
                              GpioPad,
                              PadRstCfg
                              );

  //
  // Get how interrupt is triggered (RxEvCfg)
  //
  GpioConfig->InterruptConfig = ((PadCfgDwReg[0] & B_PCH_GPIO_RX_LVL_EDG) >> (N_PCH_GPIO_RX_LVL_EDG - (N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS + 1))) | (0x1 << N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS);

  //
  // Get interrupt generation (GPIRoutIOxAPIC/SCI/SMI/NMI)
  //
  GpioConfig->InterruptConfig |= ((PadCfgDwReg[0] & (B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE)) >> (N_PCH_GPIO_RX_NMI_ROUTE - (N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS + 1))) | (0x1 << N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS);

  //
  // Get GPIO direction (GPIORxDis and GPIOTxDis)
  //
  GpioConfig->Direction = ((PadCfgDwReg[0] & (B_PCH_GPIO_RXDIS | B_PCH_GPIO_TXDIS)) >> (N_PCH_GPIO_TXDIS - (N_GPIO_DIRECTION_DIR_BIT_POS + 1))) | (0x1 << N_GPIO_DIRECTION_DIR_BIT_POS);

  //
  // Get GPIO input inversion (RXINV)
  // (Only meaningful if input enabled)
  //
  if((PadCfgDwReg[0] & B_PCH_GPIO_RXDIS) == 0) {
    GpioConfig->Direction |= ((PadCfgDwReg[0] & B_PCH_GPIO_RXINV) >> (N_PCH_GPIO_RXINV - (N_GPIO_DIRECTION_INV_BIT_POS + 1))) | (0x1 << N_GPIO_DIRECTION_INV_BIT_POS);
  }

  //
  // Get GPIO output state (GPIOTxState)
  //
  GpioConfig->OutputState = ((PadCfgDwReg[0] & B_PCH_GPIO_TX_STATE) << (N_PCH_GPIO_TX_STATE + (N_GPIO_OUTPUT_BIT_POS + 1))) | (0x1 << N_GPIO_OUTPUT_BIT_POS);

  //
  // Configure GPIO RX raw override to '1' (RXRAW1)
  //
  GpioConfig->OtherSettings = ((PadCfgDwReg[0] & B_PCH_GPIO_RX_RAW1) >> (N_PCH_GPIO_RX_RAW1 - (N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS + 1))) | (0x1 << N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS);

  //
  // Get GPIO Pad Mode (PMode)
  //
  GpioConfig->PadMode = ((PadCfgDwReg[0] & B_PCH_GPIO_PAD_MODE) >> (N_PCH_GPIO_PAD_MODE - (N_GPIO_PAD_MODE_BIT_POS + 1))) | (0x1 << N_GPIO_PAD_MODE_BIT_POS);

  //
  // Get GPIO termination (Term)
  //
  GpioConfig->ElectricalConfig = ((PadCfgDwReg[1] & B_PCH_GPIO_TERM) >> (N_PCH_GPIO_TERM - (N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS + 1))) | (0x1 << N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS);

  //
  // Get GPIO pad tolerance (padtol)
  //
  GpioConfig->ElectricalConfig |= ((PadCfgDwReg[1] & B_PCH_GPIO_PADTOL) >> (N_PCH_GPIO_PADTOL - (N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS + 1))) | (0x1 << N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS);
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
  EFI_STATUS           Status;
  UINT32               PadCfgDwReg[2];
  UINT32               RegVal;
  GPIO_GROUP           Group;
  UINT32               PadNumber;
  UINT32               PadBitPosition;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Read PADCFG DW0 register
  //
  GpioReadPadCfgReg (
    GpioPad,
    0,
    &PadCfgDwReg[0]
    );

  //
  // Read PADCFG DW1 register
  //
  GpioReadPadCfgReg (
    GpioPad,
    1,
    &PadCfgDwReg[1]
    );

  GpioConfigFromPadCfgRegValue (
    GpioPad,
    PadCfgDwReg,
    GpioData
    );

  //
  // Read HOSTSW_OWN registers
  //
  Status = GpioReadReg (
             GpioHostOwnershipRegister,
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             &RegVal
             );

  ASSERT_EFI_ERROR (Status);

  //
  // Get Host Software Ownership
  //
  GpioData->HostSoftPadOwn = (((RegVal >> PadBitPosition) & 0x1) << (N_GPIO_HOSTSW_OWN_BIT_POS + 1)) | (0x1 << N_GPIO_HOSTSW_OWN_BIT_POS);

  //
  // Read PADCFGLOCK register
  //
  Status = GpioReadReg (
             GpioPadConfigLockRegister,
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             &RegVal
             );

  ASSERT_EFI_ERROR (Status);

  //
  // Get Pad Configuration Lock state
  //
  GpioData->LockConfig = (((RegVal >> PadBitPosition) & 0x1) << 1) | 0x1;

  //
  // Read PADCFGLOCKTX register
  //
  Status = GpioReadReg (
             GpioPadLockOutputRegister,
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             &RegVal
             );

  ASSERT_EFI_ERROR (Status);

  //
  // Get Pad Configuration Lock Tx state
  //
  GpioData->LockConfig |= (((RegVal >> PadBitPosition) & 0x1) << 2) | 0x1;

  return Status;
}

/**
  This procedure will calculate PADCFG register value based on GpioConfig data

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  GpioConfig                GPIO Configuration data
  @param[out] PadCfgDwReg               PADCFG DWx register value
  @param[out] PadCfgDwRegMask           Mask with PADCFG DWx register bits to be modified

  @retval Status
**/
EFI_STATUS
GpioPadCfgRegValueFromGpioConfig (
  IN  GPIO_PAD           GpioPad,
  IN  CONST GPIO_CONFIG  *GpioConfig,
  OUT UINT32             *PadCfgDwReg,
  OUT UINT32             *PadCfgDwRegMask
  )
{
  EFI_STATUS           Status;
  UINT32               PadRstCfg;

  //
  // Configure Reset Type (PadRstCfg)
  // Reset configuration depends on group type.
  // This field requires support for new and deprecated settings.
  //
  Status = GpioPadRstCfgFromResetConfig (
             GpioPad,
             GpioConfig->PowerConfig,
             &PadRstCfg
             );
  ASSERT_EFI_ERROR (Status);

  PadCfgDwRegMask[0] |= ((((GpioConfig->PowerConfig & B_GPIO_RESET_CONFIG_RESET_MASK) >> N_GPIO_RESET_CONFIG_RESET_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RST_CONF);
  PadCfgDwReg[0] |= PadRstCfg << N_PCH_GPIO_RST_CONF;

  //
  // Configure how interrupt is triggered (RxEvCfg)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_TYPE_MASK) >> N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RX_LVL_EDG);
  PadCfgDwReg[0] |= (((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_TYPE_MASK) >> (N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS + 1)) << N_PCH_GPIO_RX_LVL_EDG);

  //
  // Configure interrupt generation (GPIRoutIOxAPIC/SCI/SMI/NMI)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) >> N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS) == GpioHardwareDefault)  ? 0x0 : (B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE));
  PadCfgDwReg[0] |= (((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) >> (N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS + 1)) << N_PCH_GPIO_RX_NMI_ROUTE);

  //
  // Configure GPIO direction (GPIORxDis and GPIOTxDis)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->Direction & B_GPIO_DIRECTION_DIR_MASK) >> N_GPIO_DIRECTION_DIR_BIT_POS) == GpioHardwareDefault) ? 0x0 : (B_PCH_GPIO_RXDIS | B_PCH_GPIO_TXDIS));
  PadCfgDwReg[0] |= (((GpioConfig->Direction & B_GPIO_DIRECTION_DIR_MASK) >> (N_GPIO_DIRECTION_DIR_BIT_POS + 1)) << N_PCH_GPIO_TXDIS);

  //
  // Configure GPIO input inversion (RXINV)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->Direction & B_GPIO_DIRECTION_INV_MASK) >> N_GPIO_DIRECTION_INV_BIT_POS) == GpioHardwareDefault) ?  0x0 : B_PCH_GPIO_RXINV);
  PadCfgDwReg[0] |= (((GpioConfig->Direction & B_GPIO_DIRECTION_INV_MASK) >> (N_GPIO_DIRECTION_INV_BIT_POS + 1)) << N_PCH_GPIO_RXINV);

  //
  // Configure GPIO output state (GPIOTxState)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->OutputState & B_GPIO_OUTPUT_MASK) >> N_GPIO_OUTPUT_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_TX_STATE);
  PadCfgDwReg[0] |= (((GpioConfig->OutputState & B_GPIO_OUTPUT_MASK) >> (N_GPIO_OUTPUT_BIT_POS + 1)) << N_PCH_GPIO_TX_STATE);

  //
  // Configure GPIO RX raw override to '1' (RXRAW1)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->OtherSettings & B_GPIO_OTHER_CONFIG_RXRAW_MASK) >> N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RX_RAW1);
  PadCfgDwReg[0] |= (((GpioConfig->OtherSettings & B_GPIO_OTHER_CONFIG_RXRAW_MASK) >> (N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS + 1)) << N_PCH_GPIO_RX_RAW1);

  //
  // Configure GPIO Pad Mode (PMode)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->PadMode & B_GPIO_PAD_MODE_MASK) >> N_GPIO_PAD_MODE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PAD_MODE);
  PadCfgDwReg[0] |= (((GpioConfig->PadMode & B_GPIO_PAD_MODE_MASK) >> (N_GPIO_PAD_MODE_BIT_POS + 1)) << N_PCH_GPIO_PAD_MODE);

  //
  // Configure GPIO termination (Term)
  //
  PadCfgDwRegMask[1] |= ((((GpioConfig->ElectricalConfig & B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK) >> N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_TERM);
  PadCfgDwReg[1] |= (((GpioConfig->ElectricalConfig & B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK) >> (N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS + 1)) << N_PCH_GPIO_TERM);

  //
  // Configure GPIO pad tolerance (padtol)
  //
  PadCfgDwRegMask[1] |= ((((GpioConfig->ElectricalConfig & B_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_MASK) >> N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PADTOL);
  PadCfgDwReg[1] |= (((GpioConfig->ElectricalConfig & B_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_MASK) >> (N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS + 1)) << N_PCH_GPIO_PADTOL);

  return Status;
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
  EFI_STATUS           Status;
  UINT32               PadCfgDwReg[2];
  UINT32               PadCfgDwRegMask[2];
  UINT32               HostSoftOwnReg;
  UINT32               HostSoftOwnRegMask;
  UINT32               GpiGpeEnReg;
  UINT32               GpiGpeEnRegMask;
  UINT32               GpiNmiEnReg;
  UINT32               GpiNmiEnRegMask;
  GPIO_GROUP           Group;
  UINT32               GroupIndex;
  UINT32               PadNumber;
  UINT32               PadBitPosition;
  UINT32               DwNum;

  ZeroMem (PadCfgDwReg, sizeof (PadCfgDwReg));
  ZeroMem (PadCfgDwRegMask, sizeof (PadCfgDwRegMask));

  Group = GpioGetGroupFromGpioPad (GpioPad);
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);
  DwNum = GPIO_GET_DW_NUM (PadNumber);

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get GPIO PADCFG register value from GPIO config data
  //
  GpioPadCfgRegValueFromGpioConfig (
    GpioPad,
    GpioData,
    PadCfgDwReg,
    PadCfgDwRegMask
    );

  //
  // Write PADCFG DW0 register
  //
  GpioWritePadCfgReg (
    GpioPad,
    0,
    ~PadCfgDwRegMask[0],
    PadCfgDwReg[0]
    );

  //
  // Write PADCFG DW1 register
  //
  GpioWritePadCfgReg (
    GpioPad,
    1,
    ~PadCfgDwRegMask[1],
    PadCfgDwReg[1]
    );

  //
  // Update value to be programmed in HOSTSW_OWN register
  //
  HostSoftOwnRegMask = (GpioData->HostSoftPadOwn & 0x1) << PadBitPosition;
  HostSoftOwnReg = (GpioData->HostSoftPadOwn >> 0x1) << PadBitPosition;

  //
  // Write HOSTSW_OWN registers
  //
  Status = GpioWriteReg (
             GpioHostOwnershipRegister,
             Group,
             DwNum,
             ~HostSoftOwnRegMask,
             HostSoftOwnReg
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Update value to be programmed in GPI_GPE_EN register
  //
  GpiGpeEnRegMask = (GpioData->InterruptConfig & 0x1) << PadBitPosition;
  GpiGpeEnReg = ((GpioData->InterruptConfig & GpioIntSci) >> 3) << PadBitPosition;

  //
  // Write GPI_GPE_EN registers
  //
  Status = GpioWriteReg (
             GpioGpeEnableRegister,
             Group,
             DwNum,
             ~GpiGpeEnRegMask,
             GpiGpeEnReg
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Update value to be programmed in GPI_NMI_EN register
  //
  GpiNmiEnRegMask = (GpioData->InterruptConfig & 0x1) << PadBitPosition;
  GpiNmiEnReg = ((GpioData->InterruptConfig & GpioIntNmi) >> 1) << PadBitPosition;

  Status = GpioWriteReg (
             GpioNmiEnableRegister,
             Group,
             DwNum,
             ~GpiNmiEnRegMask,
             GpiNmiEnReg
             );
  if (Status == EFI_UNSUPPORTED) {
    if (GpiNmiEnReg == 0) {
      //
      // Not all GPIO have NMI capabilities. Since we always try to program this register,
      // even when not enabling NMI for a pad so do not report such access as an error
      //
      Status = EFI_SUCCESS;
    } else {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group %d has no pads supporting NMI\n", GroupIndex));
      ASSERT (FALSE);
      return Status;
    }
  }

  //
  // Program Pad Configuration Lock
  //
  if ((GpioData->LockConfig & GpioPadConfigLock) == GpioPadConfigLock) {
    GpioWriteLockReg (
      GpioPadConfigLockRegister,
      Group,
      DwNum,
      ~0u,
      1 << PadBitPosition
      );
  }

  //
  // Program Pad Configuration Lock Tx
  //
  if ((GpioData->LockConfig & GpioOutputStateLock) == GpioOutputStateLock) {
    GpioWriteLockReg (
      GpioPadLockOutputRegister,
      Group,
      DwNum,
      ~0u,
      1 << PadBitPosition
      );
  }
  return Status;
}

/**
  This procedure will set GPIO output level

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Output value
                                  0: OutputLow, 1: OutputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetOutputValue (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  )
{
  if (Value > 1) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioWritePadCfgReg (
    GpioPad,
    0,
    (UINT32)~B_PCH_GPIO_TX_STATE,
    Value << N_PCH_GPIO_TX_STATE
    );

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO output level

  @param[in]  GpioPad             GPIO pad
  @param[out] OutputVal           GPIO Output value
                                  0: OutputLow, 1: OutputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetOutputValue (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *OutputVal
  )
{
  UINT32      PadCfgReg;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioReadPadCfgReg (
    GpioPad,
    0,
    &PadCfgReg
    );

  *OutputVal = (PadCfgReg & B_PCH_GPIO_TX_STATE) >> N_PCH_GPIO_TX_STATE;

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO input level

  @param[in] GpioPad              GPIO pad
  @param[out] InputVal            GPIO Input value
                                  0: InputLow, 1: InpuHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetInputValue (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *InputVal
  )
{
  UINT32      PadCfgReg;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioReadPadCfgReg (
    GpioPad,
    0,
    &PadCfgReg
    );

  *InputVal = (PadCfgReg & B_PCH_GPIO_RX_STATE) >> N_PCH_GPIO_RX_STATE;

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO IOxAPIC interrupt number

  @param[in]  GpioPad             GPIO pad
  @param[out] IrqNum              IRQ number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadIoApicIrqNumber (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *IrqNum
  )
{
  UINT32      PadCfgReg;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioReadPadCfgReg (
    GpioPad,
    1,
    &PadCfgReg
    );

  *IrqNum = (PadCfgReg & B_PCH_GPIO_INTSEL) >> N_PCH_GPIO_INTSEL;

  return EFI_SUCCESS;
}

/**
  This procedure will configure GPIO input inversion

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value for GPIO input inversion
                                  0: No input inversion, 1: Invert input

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetInputInversion (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  )
{
  if (Value > 1) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioWritePadCfgReg (
    GpioPad,
    0,
    (UINT32)~B_PCH_GPIO_RXINV,
    Value << N_PCH_GPIO_RXINV
    );

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO pad input inversion value

  @param[in]  GpioPad             GPIO pad
  @param[out] InvertState         GPIO inversion state
                                  0: No input inversion, 1: Inverted input

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetInputInversion (
  IN  GPIO_PAD                 GpioPad,
  OUT UINT32                   *InvertState
  )
{
  UINT32      PadCfgReg;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioReadPadCfgReg (
    GpioPad,
    0,
    &PadCfgReg
    );

  *InvertState = (PadCfgReg & B_PCH_GPIO_RXINV) >> N_PCH_GPIO_RXINV;
  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO interrupt settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of Level/Edge
                                  use GPIO_INT_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
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
  UINT32      PadNumber;
  UINT32      GpeEnable;
  UINT32      NmiEnable;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;

  if (((Value & B_GPIO_INT_CONFIG_INT_TYPE_MASK) >> N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS) != GpioHardwareDefault) {
    RxLvlEdgeValue = ((Value & B_GPIO_INT_CONFIG_INT_TYPE_MASK) >> (N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS + 1)) << N_PCH_GPIO_RX_LVL_EDG;

    GpioWritePadCfgReg (
      GpioPad,
      0,
      (UINT32)~B_PCH_GPIO_RX_LVL_EDG,
      RxLvlEdgeValue
      );
  }

  if (((Value & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) >> N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS) != GpioHardwareDefault) {

    IntRouteValue = ((Value & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) >> (N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS + 1)) << N_PCH_GPIO_RX_NMI_ROUTE;

    GpioWritePadCfgReg (
      GpioPad,
      0,
      (UINT32)~(B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE),
      IntRouteValue
      );

    if ((Value & GpioIntSci) == GpioIntSci) {
      GpeEnable = 0x1;
    } else {
      GpeEnable = 0x0;
    }

    PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

    Status = GpioWriteReg (
               GpioGpeEnableRegister,
               GpioGetGroupFromGpioPad (GpioPad),
               GPIO_GET_DW_NUM (PadNumber),
               ~(1 << GPIO_GET_PAD_POSITION (PadNumber)),
               GpeEnable << GPIO_GET_PAD_POSITION (PadNumber)
               );
    ASSERT_EFI_ERROR (Status);

    if ((Value & GpioIntNmi) == GpioIntNmi) {
      NmiEnable = 0x1;
    } else {
      NmiEnable = 0x0;
    }

    Status = GpioWriteReg (
               GpioNmiEnableRegister,
               GpioGetGroupFromGpioPad (GpioPad),
               GPIO_GET_DW_NUM (PadNumber),
               ~(1 << GPIO_GET_PAD_POSITION (PadNumber)),
               NmiEnable << GPIO_GET_PAD_POSITION (PadNumber)
               );
    if (Status == EFI_UNSUPPORTED) {
      if (NmiEnable == 0) {
        //
        // Not all GPIO have NMI capabilities. Since we always try to program this register,
        // even when not enabling NMI for a pad so do not report such access as an error
        //
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group %d has no pads supporting NMI\n", GpioGetGroupIndexFromGpioPad (GpioPad)));
      }
    }
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
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetPadElectricalConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_ELECTRICAL_CONFIG    Value
  )
{
  UINT32      TermValue;
  UINT32      PadTolValue;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  if (((Value & B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK) >> N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS) != GpioHardwareDefault) {
    TermValue = ((Value & B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK) >> (N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS + 1)) << N_PCH_GPIO_TERM;

    GpioWritePadCfgReg (
      GpioPad,
      1,
      (UINT32)~B_PCH_GPIO_TERM,
      TermValue
      );
  }

  if (((Value & B_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_MASK) >> N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS) != GpioHardwareDefault) {
    PadTolValue = ((Value & B_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_MASK) >> (N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS + 1)) << N_PCH_GPIO_PADTOL;

    GpioWritePadCfgReg (
      GpioPad,
      1,
      (UINT32)~B_PCH_GPIO_PADTOL,
      PadTolValue
      );
  }
  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO Reset settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value for Pad Reset Configuration
                                  use GPIO_RESET_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetPadResetConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_RESET_CONFIG         Value
  )
{
  EFI_STATUS  Status;
  UINT32      PadRstCfg;

  Status = EFI_SUCCESS;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  if (((Value & B_GPIO_RESET_CONFIG_RESET_MASK) >> N_GPIO_RESET_CONFIG_RESET_BIT_POS) != GpioHardwareDefault) {

    //
    // Reset configuration depends on group type.
    // This field requires support for new and deprecated settings.
    //
    Status = GpioPadRstCfgFromResetConfig (
               GpioPad,
               Value,
               &PadRstCfg
               );
    ASSERT_EFI_ERROR (Status);

    GpioWritePadCfgReg (
      GpioPad,
      0,
      (UINT32)~B_PCH_GPIO_RST_CONF,
      PadRstCfg << N_PCH_GPIO_RST_CONF
      );
  }
  return Status;
}

/**
  This procedure will get GPIO Reset settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of Pad Reset Configuration
                                  based on GPIO_RESET_CONFIG

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadResetConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_RESET_CONFIG         *Value
  )
{
  UINT32      PadRstCfg;
  UINT32      PadCfgDw0Reg;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioReadPadCfgReg (
    GpioPad,
    0,
    &PadCfgDw0Reg
    );

  //
  // Get Reset Type (PadRstCfg)
  //
  PadRstCfg = (PadCfgDw0Reg & B_PCH_GPIO_RST_CONF) >> N_PCH_GPIO_RST_CONF;

  *Value = GpioResetConfigFromPadRstCfg (
             GpioPad,
             PadRstCfg
             );

  return EFI_SUCCESS;
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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioReadReg (
             GpioHostOwnershipRegister,
             Group,
             DwNum,
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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioWriteReg (
             GpioHostOwnershipRegister,
             Group,
             DwNum,
             0,
             HostSwRegVal
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
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetHostSwOwnershipForPad (
  IN GPIO_PAD                 GpioPad,
  OUT UINT32                  *PadHostSwOwn
  )
{
  EFI_STATUS  Status;
  UINT32      PadNumber;
  UINT32      HostSwRegVal;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioReadReg (
             GpioHostOwnershipRegister,
             GpioGetGroupFromGpioPad (GpioPad),
             GPIO_GET_DW_NUM (PadNumber),
             &HostSwRegVal
             );
  ASSERT_EFI_ERROR (Status);

  *PadHostSwOwn = (HostSwRegVal >> GPIO_GET_PAD_POSITION (PadNumber)) & 0x1;

  return Status;
}

/**
  This procedure will set Gpio Pad Host Software Ownership

  @param[in] GpioPad              GPIO pad
  @param[in] PadHostSwOwn         Pad Host Software Owner
                                  0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetHostSwOwnershipForPad (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadHostSwOwn
  )
{
  EFI_STATUS  Status;
  UINT32      PadNumber;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioWriteReg (
             GpioHostOwnershipRegister,
             GpioGetGroupFromGpioPad (GpioPad),
             GPIO_GET_DW_NUM (PadNumber),
             ~0u,
             1 << GPIO_GET_PAD_POSITION (PadNumber)
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

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

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

  *PadOwnVal = (GPIO_PAD_OWN) ((PadOwnRegValue & Mask) >> (PadNumber * 4));

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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioReadReg (
             GpioPadConfigLockRegister,
             Group,
             DwNum,
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
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadCfgLock (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *PadCfgLock
  )
{
  EFI_STATUS  Status;
  UINT32      PadNumber;
  UINT32      PadCfgLockRegVal;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioReadReg (
             GpioPadConfigLockRegister,
             GpioGetGroupFromGpioPad (GpioPad),
             GPIO_GET_DW_NUM (PadNumber),
             &PadCfgLockRegVal
             );
  ASSERT_EFI_ERROR (Status);

  *PadCfgLock = (PadCfgLockRegVal >> GPIO_GET_PAD_POSITION (PadNumber)) & 0x1;

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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioReadReg (
             GpioPadLockOutputRegister,
             Group,
             DwNum,
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
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadCfgLockTx (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *PadCfgLockTx
  )
{
  EFI_STATUS  Status;
  UINT32      PadNumber;
  UINT32      PadCfgLockTxRegVal;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioReadReg (
             GpioPadLockOutputRegister,
             GpioGetGroupFromGpioPad (GpioPad),
             GPIO_GET_DW_NUM (PadNumber),
             &PadCfgLockTxRegVal
             );
  ASSERT_EFI_ERROR (Status);

  *PadCfgLockTx = (PadCfgLockTxRegVal >> GPIO_GET_PAD_POSITION (PadNumber)) & 0x1;

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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioWriteLockReg (
             GpioPadConfigLockRegister,
             Group,
             DwNum,
             ~PadsToUnlock,
             0
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
  EFI_STATUS   Status;
  GPIO_GROUP   Group;
  UINT32       PadNumber;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioUnlockPadCfgForGroupDw (
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             1 << GPIO_GET_PAD_POSITION (PadNumber)
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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioWriteLockReg (
             GpioPadConfigLockRegister,
             Group,
             DwNum,
             ~0u,
             PadsToLock
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
  EFI_STATUS   Status;
  GPIO_GROUP   Group;
  UINT32       PadNumber;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioLockPadCfgForGroupDw (
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             1 << GPIO_GET_PAD_POSITION (PadNumber)
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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioWriteLockReg (
             GpioPadLockOutputRegister,
             Group,
             DwNum,
             ~PadsToUnlockTx,
             0
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
  EFI_STATUS   Status;
  GPIO_GROUP   Group;
  UINT32       PadNumber;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioUnlockPadCfgTxForGroupDw (
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             1 << GPIO_GET_PAD_POSITION (PadNumber)
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

  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioWriteLockReg (
             GpioPadLockOutputRegister,
             Group,
             DwNum,
             ~0u,
             PadsToLockTx
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
  EFI_STATUS   Status;
  GPIO_GROUP   Group;
  UINT32       PadNumber;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  Status = GpioLockPadCfgTxForGroupDw (
             Group,
             GPIO_GET_DW_NUM (PadNumber),
             1 << GPIO_GET_PAD_POSITION (PadNumber)
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
  if (((UINT32) GroupToGpeDw0 < GpioGroupLowest) || ((UINT32) GroupToGpeDw0 > GpioGroupHighest) ||
      ((UINT32) GroupToGpeDw1 < GpioGroupLowest) || ((UINT32) GroupToGpeDw1 > GpioGroupHighest) ||
      ((UINT32) GroupToGpeDw2 < GpioGroupLowest) || ((UINT32) GroupToGpeDw2 > GpioGroupHighest)) {
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
  GroupToGpeDw0 = GpioGetGroupIndexFromGroup (GroupToGpeDw0);
  GroupToGpeDw1 = GpioGetGroupIndexFromGroup (GroupToGpeDw1);
  GroupToGpeDw2 = GpioGetGroupIndexFromGroup (GroupToGpeDw2);

  PchPwrmBaseGet (&PchPwrmBase);

  //
  // Program GPIO_CFG (PMRMBASE + 120h) register
  //
  Data32And = (UINT32) ~(B_PCH_PWRM_GPIO_CFG_GPE0_DW2 | B_PCH_PWRM_GPIO_CFG_GPE0_DW1 | B_PCH_PWRM_GPIO_CFG_GPE0_DW0);
  Data32Or  = (UINT32) ((GroupToGpeDw2 << N_PCH_PWRM_GPIO_CFG_GPE0_DW2) |
                        (GroupToGpeDw1 << N_PCH_PWRM_GPIO_CFG_GPE0_DW1) |
                        (GroupToGpeDw0 << N_PCH_PWRM_GPIO_CFG_GPE0_DW0));

  MmioAndThenOr32 (
    (PchPwrmBase + R_PCH_PWRM_GPIO_CFG),
    Data32And,
    Data32Or
    );

  Data32And = (UINT32) ~(B_PCH_PCR_GPIO_MISCCFG_GPE0_DW2 | B_PCH_PCR_GPIO_MISCCFG_GPE0_DW1 | B_PCH_PCR_GPIO_MISCCFG_GPE0_DW0);
  Data32Or = (UINT32) ((GroupToGpeDw2 << N_PCH_PCR_GPIO_MISCCFG_GPE0_DW2) |
                       (GroupToGpeDw1 << N_PCH_PCR_GPIO_MISCCFG_GPE0_DW1) |
                       (GroupToGpeDw0 << N_PCH_PCR_GPIO_MISCCFG_GPE0_DW0));
  //
  // Program MISCCFG register for Community 0
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM0, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 1
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM1, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 2
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM2, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 3
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM3, R_PCH_PCR_GPIO_MISCCFG),
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
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
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
  GPIO_GROUP           Group;
  UINT32               PadNumber;

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get GPIO groups mapping to 1-tier GPE
  // This mapping is dependent on GPIO IP implementation
  // and may change between chipset generations
  //
  GpioGetGroupToGpeDwX (&GroupToGpeDw0,&GroupToGpeDw1,&GroupToGpeDw2);

  if (Group == GroupToGpeDw0) {
    *GpeNumber = PadNumber;
  } else if (Group == GroupToGpeDw1) {
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
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioClearGpiSmiSts (
  IN GPIO_PAD                   GpioPad
  )
{
  EFI_STATUS           Status;
  GPIO_GROUP           Group;
  UINT32               PadNumber;
  UINT32               DwNum;
  UINT32               PadBitPosition;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  DwNum = GPIO_GET_DW_NUM (PadNumber);
  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);

  //
  // Clear GPI SMI Status bit by writing '1'
  //
  Status = GpioWriteReg (
             GpioSmiStatusRegister,
             Group,
             DwNum,
             0u,
             (UINT32) (BIT0 << PadBitPosition)
             );

  return Status;
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
  UINT32         DwNum;
  GPIO_GROUP     Group;
  GPIO_GROUP     GroupMin;
  GPIO_GROUP     GroupMax;

  GroupMin = GpioGetLowestGroup ();
  GroupMax = GpioGetHighestGroup ();

  for (Group = GroupMin; Group <= GroupMax; Group++) {
    //
    // Disable all GPI SMI
    //
    for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGetPadPerGroup (Group)); DwNum++) {
      GpioWriteReg (
        GpioSmiStatusRegister,
        Group,
        DwNum,
        0u,
        0xFFFFFFFF
        );
    }
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
  UINT32         DwNum;
  GPIO_GROUP     Group;
  GPIO_GROUP     GroupMin;
  GPIO_GROUP     GroupMax;

  GroupMin = GpioGetLowestGroup ();
  GroupMax = GpioGetHighestGroup ();

  for (Group = GroupMin; Group <= GroupMax; Group++) {
    //
    // Disable all GPI SMI
    //
    for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGetPadPerGroup (Group)); DwNum++) {
      GpioWriteReg (
        GpioSmiEnableRegister,
        Group,
        DwNum,
        0u,
        0u
        );
    }
  }
  return EFI_SUCCESS;
}

/**
  This procedure is used to register GPI SMI dispatch function.

  @param[in]  GpioPad             GPIO pad
  @param[out] GpiNum              GPI number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
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

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  *GpiNum = 0;

  for (Index = 0; Index < GroupIndex; Index++) {
    *GpiNum += (UINTN) (GpioGroupInfo[Index].PadPerGroup);
  }
  *GpiNum += (UINTN) PadNumber;

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
  if (Data32 == PCH_GPIO_2_TIER_MASTER_GPE_NUMBER) {
    return TRUE;
  }

  return FALSE;
}

/**
  This procedure is used to clear GPE STS for a specified GpioPad

  @param[in]  GpioPad             GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioClearGpiGpeSts (
  IN GPIO_PAD                  GpioPad
  )
{
  EFI_STATUS           Status;
  GPIO_GROUP           Group;
  UINT32               PadNumber;
  UINT32               DwNum;
  UINT32               PadBitPosition;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Check for 2-tier
  //
  if (!(GpioCheckFor2Tier (GpioPad))) {
    return EFI_INVALID_PARAMETER;
  }

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  DwNum = GPIO_GET_DW_NUM (PadNumber);
  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);

  //
  // Clear GPI GPE Status bit by writing '1'
  //
  Status = GpioWriteReg (
             GpioGpeStatusRegister,
             Group,
             DwNum,
             0u,
             (UINT32) (BIT0 << PadBitPosition)
             );

  return Status;
}

/**
  This procedure is used to read GPE STS for a specified Pad

  @param[in]  GpioPad             GPIO pad
  @param[out] Data                GPE STS data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetGpiGpeSts (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *Data
  )
{
  EFI_STATUS           Status;
  UINT32               GpeStsRegVal;
  GPIO_GROUP           Group;
  UINT32               PadNumber;
  UINT32               DwNum;
  UINT32               PadBitPosition;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Check for 2-tier
  //
  if (!(GpioCheckFor2Tier (GpioPad))) {
    return EFI_INVALID_PARAMETER;
  }

  Group = GpioGetGroupFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  DwNum = GPIO_GET_DW_NUM (PadNumber);
  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);

  //
  // Read GPI GPE Status bits
  //
  Status = GpioReadReg (
             GpioGpeStatusRegister,
             Group,
             DwNum,
             &GpeStsRegVal
             );
  if (EFI_ERROR (Status)) {
    return EFI_INVALID_PARAMETER;
  }

  *Data = (GpeStsRegVal >> PadBitPosition) & 0x1;

  return Status;
}
