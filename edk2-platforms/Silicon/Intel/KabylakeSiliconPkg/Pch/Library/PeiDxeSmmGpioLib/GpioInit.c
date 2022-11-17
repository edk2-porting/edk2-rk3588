/** @file
  This file contains routines for GPIO initialization

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "GpioLibrary.h"

/**
  This procedure will handle requirement on gSPIx_CSB pins.

  @param[in]     GpioPad        GPIO pad
  @param[in]     PadMode        GPIO PadMode value
  @param[in out] Dw0Reg         Value for PADCFG_DW0 register
  @param[in out] Dw0RegMask     Mask of bits which will change in PADCFG_DWO register

  @retval None
**/
STATIC
VOID
GpioHandleGSpiCSBPad (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadMode,
  IN OUT UINT32                *Dw0Reg,
  IN OUT UINT32                *Dw0RegMask
  )
{
  //
  // For gSPIx_CSB pins if are going to be used in native mode then skip setting
  // pad mode here.
  //
  if (GpioIsGpioPadAGSpiCsbPin (GpioPad, PadMode)) {
    //
    // Skip setting PadMode to native, leave it in GPIO mode.
    // Setting it to native should be done after Serial IO initialization
    //
    *Dw0RegMask |= B_PCH_GPIO_PAD_MODE;
    *Dw0Reg &= ~B_PCH_GPIO_PAD_MODE;
    *Dw0Reg |= ((GpioPadModeGpio >> 1) << N_PCH_GPIO_PAD_MODE);
  }
}

/**
  This procedure will handle requirement on SATA DEVSLPx pins.

  @param[in]     GpioPad        GPIO pad
  @param[in]     PadMode        GPIO PadMode value
  @param[in out] Dw0Reg         Value for PADCFG_DW0 register
  @param[in out] Dw0RegMask     Mask of bits which will change in PADCFG_DWO register

  @retval None

**/
STATIC
VOID
GpioHandleSataDevSlpPad (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadMode,
  IN OUT UINT32                *Dw0Reg,
  IN OUT UINT32                *Dw0RegMask
  )
{
  //
  // For SATA DEVSLPx pins if used in native 1 mode then ensure that PadRstCfg
  // is set to "00" - Powergood
  //
  if (GpioIsPadASataDevSlpPin (GpioPad, PadMode)) {
    //
    // Set PadRstCfg to Powergood
    //
    *Dw0RegMask |= B_PCH_GPIO_RST_CONF;
    *Dw0Reg &= ~B_PCH_GPIO_RST_CONF;
    *Dw0Reg |= ((GpioResumeReset >> 1) << N_PCH_GPIO_RST_CONF);
  }
}

//
// GPIO_DW_REG_VALUE structure is used by GpioConfigureSklPch function
// to cache values which will be programmed into respective GPIO registers
// after all GpioPads are processed. This way MMIO accesses are decreased
// and instead of doing one programming for one GpioPad there is only
// one access for whole register.
//
typedef struct {
  UINT32             HostSoftOwnReg;
  UINT32             HostSoftOwnRegMask;
  UINT32             GpiGpeEnReg;
  UINT32             GpiGpeEnRegMask;
  UINT32             GpiNmiEnReg;
  UINT32             GpiNmiEnRegMask;
  UINT32             PadsToLock;
  UINT32             PadsToLockTx;
} GPIO_DW_REG_VALUE;

//
// GPIO_DW_REG_NUMBER contains number of DWords required to
// store Pad data for all groups. Each pad uses one bit.
//
// For Skylake and Kabylake each group has <32 pads what
// makes GPIO_DW_REG_NUMBER = 1
//
#define GPIO_DW_REG_NUMBER  1

/**
  Get GPIO DW Register values (HOSTSW_OWN, GPE_EN, NMI_EN, Lock).

  @param[in]     PadNumber      GPIO pad number
  @param[in]     GpioConfig     GPIO Config data
  @param[in out] DwRegsValues   Values for GPIO DW Registers

  @retval None
**/
STATIC
VOID
GpioDwRegValueFromGpioConfig (
  IN  UINT32                PadNumber,
  IN  GPIO_CONFIG           *GpioConfig,
  IN OUT GPIO_DW_REG_VALUE  *DwRegsValues
  )
{
  UINT32  PadBitPosition;
  UINT32  DwNum;

  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);
  DwNum = GPIO_GET_DW_NUM (PadNumber);

  if (DwNum >= GPIO_DW_REG_NUMBER) {
    ASSERT (FALSE);
    return;
  }
  //
  // Update value to be programmed in HOSTSW_OWN register
  //
  DwRegsValues[DwNum].HostSoftOwnRegMask |= (GpioConfig->HostSoftPadOwn & 0x1) << PadBitPosition;
  DwRegsValues[DwNum].HostSoftOwnReg |= (GpioConfig->HostSoftPadOwn >> 0x1) << PadBitPosition;

  //
  // Update value to be programmed in GPI_GPE_EN register
  //
  DwRegsValues[DwNum].GpiGpeEnRegMask |= (GpioConfig->InterruptConfig & 0x1) << PadBitPosition;
  DwRegsValues[DwNum].GpiGpeEnReg |= ((GpioConfig->InterruptConfig & GpioIntSci) >> 3) << PadBitPosition;

  //
  // Update value to be programmed in GPI_NMI_EN register
  //
  DwRegsValues[DwNum].GpiNmiEnRegMask |= (GpioConfig->InterruptConfig & 0x1) << PadBitPosition;
  DwRegsValues[DwNum].GpiNmiEnReg |= ((GpioConfig->InterruptConfig & GpioIntNmi) >> 1) << PadBitPosition;

  //
  // Update information on Pad Configuration Lock
  //
  DwRegsValues[DwNum].PadsToLock |= ((GpioConfig->LockConfig >> 0x1) & 0x1) << PadBitPosition;

  //
  // Update information on Pad Configuration Lock Tx
  //
  DwRegsValues[DwNum].PadsToLockTx |= ((GpioConfig->LockConfig >> 0x2) & 0x1) << PadBitPosition;
}

/**
  This SKL PCH specific procedure will initialize multiple SKL PCH GPIO pins

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
STATIC
EFI_STATUS
GpioConfigureSklPch (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  )
{
  UINT32               Index;
  UINT32               PadCfgDwReg[2];
  UINT32               PadCfgDwRegMask[2];
  UINT32               PadCfgReg;
  GPIO_DW_REG_VALUE    DwRegsValues[GPIO_DW_REG_NUMBER];
  UINT32               DwNum;
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

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupOffset = GpioGetLowestGroup ();
  NumberOfGroups = GpioGetNumberOfGroups ();

  Index = 0;
  while (Index < NumberOfItems) {

    GpioData   = &GpioInitTableAddress[Index];
    Group      = GpioGetGroupFromGpioPad (GpioData->GpioPad);
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioData->GpioPad);
    PadNumber  = GpioGetPadNumberFromGpioPad (GpioData->GpioPad);

    if (GroupIndex >= V_PCH_GPIO_GROUP_MAX) {
      // @todo SKL PCH: one platform code ready we should add this back      ASSERT (FALSE);
      continue;
    }

    DEBUG_CODE_BEGIN ();
    if (!(((PchSeries == PchH) && (GPIO_GET_CHIPSET_ID (GpioData->GpioPad) == GPIO_SKL_H_CHIPSET_ID)) ||
          ((PchSeries == PchLp) && (GPIO_GET_CHIPSET_ID (GpioData->GpioPad) == GPIO_SKL_LP_CHIPSET_ID)))) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    DEBUG_CODE_END ();

    //
    // Check if group argument exceeds GPIO group range
    //
    if ((Group < GpioGroupOffset) || (Group >= NumberOfGroups + GpioGroupOffset)) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Invalid group %d\n", GroupIndex));
      return EFI_INVALID_PARAMETER;
    }

    ZeroMem (DwRegsValues, sizeof (DwRegsValues));
    //
    // Loop through pads for one group. If pad belongs to a different group then
    // break and move to register programming.
    //
    while (Index < NumberOfItems) {

      GpioData   = &GpioInitTableAddress[Index];
      if (GroupIndex != GpioGetGroupIndexFromGpioPad (GpioData->GpioPad)) {
        //if next pad is from different group then break loop
        break;
      }

      PadNumber  = GpioGetPadNumberFromGpioPad (GpioData->GpioPad);
      //
      // Check if legal pin number
      //
      if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for group %d\n", PadNumber, GroupIndex));
        return EFI_INVALID_PARAMETER;
      }

      DEBUG_CODE_BEGIN ();
      //
      // Check if selected GPIO Pad is not owned by CSME/ISH
      //
      GpioGetPadOwnership (GpioData->GpioPad, &PadOwnVal);

      if (PadOwnVal != GpioPadOwnHost) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
        DEBUG ((DEBUG_ERROR, "** Please make sure the GPIO usage in sync between CSME and BIOS configuration. \n"));
        DEBUG ((DEBUG_ERROR, "** All the GPIO occupied by CSME should not do any configuration by BIOS.\n"));
        //Move to next item
        Index++;
        continue;
      }
      DEBUG_CODE_END ();

      ZeroMem (PadCfgDwReg, sizeof (PadCfgDwReg));
      ZeroMem (PadCfgDwRegMask, sizeof (PadCfgDwRegMask));
      //
      // Get GPIO PADCFG register value from GPIO config data
      //
      GpioPadCfgRegValueFromGpioConfig (
        GpioData->GpioPad,
        &GpioData->GpioConfig,
        PadCfgDwReg,
        PadCfgDwRegMask
        );

      //
      // Check for additional requirements on setting PADCFG register
      //
      GpioHandleGSpiCSBPad (GpioData->GpioPad, GpioData->GpioConfig.PadMode, &PadCfgDwReg[0], &PadCfgDwRegMask[0]);
      GpioHandleSataDevSlpPad (GpioData->GpioPad, GpioData->GpioConfig.PadMode, &PadCfgDwReg[0], &PadCfgDwRegMask[0]);

      //
      // Create PADCFG register offset using group and pad number
      //
      PadCfgReg = PAD_CFG_SIZE * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;

      //
      // Write PADCFG DW0 register
      //
      MmioAndThenOr32 (
        PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg),
        ~PadCfgDwRegMask[0],
        PadCfgDwReg[0]
        );

      //
      // Write PADCFG DW1 register
      //
      MmioAndThenOr32 (
        PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg + 0x4),
        ~PadCfgDwRegMask[1],
        PadCfgDwReg[1]
        );

      //
      // Get GPIO DW register values from GPIO config data
      //
      GpioDwRegValueFromGpioConfig (
        PadNumber,
        &GpioData->GpioConfig,
        DwRegsValues
        );

      //Move to next item
      Index++;
    }

    for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
      //
      // Write HOSTSW_OWN registers
      //
      if (GpioGroupInfo[GroupIndex].HostOwnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].HostOwnOffset + DwNum * 0x4),
          ~DwRegsValues[DwNum].HostSoftOwnRegMask,
          DwRegsValues[DwNum].HostSoftOwnReg
          );
      }

      //
      // Write GPI_GPE_EN registers
      //
      if (GpioGroupInfo[GroupIndex].GpiGpeEnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeEnOffset + DwNum * 0x4),
          ~DwRegsValues[DwNum].GpiGpeEnRegMask,
          DwRegsValues[DwNum].GpiGpeEnReg
          );
      }

      //
      // Write GPI_NMI_EN registers
      //
      if (GpioGroupInfo[GroupIndex].NmiEnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].NmiEnOffset + DwNum * 0x4),
          ~DwRegsValues[DwNum].GpiNmiEnRegMask,
          DwRegsValues[DwNum].GpiNmiEnReg
          );
      } else if (DwRegsValues[DwNum].GpiNmiEnReg != 0x0) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group %d has no pads supporting NMI\n", GroupIndex));
        ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
      }

      //
      // Write Pad Configuration Lock
      //
      if (DwRegsValues[DwNum].PadsToLock != 0) {
        GpioLockPadCfgForGroupDw (GroupIndex + GpioGroupOffset, DwNum, DwRegsValues[DwNum].PadsToLock);
      }

      //
      // Write Pad Configuration Lock Tx
      //
      if (DwRegsValues[DwNum].PadsToLockTx != 0) {
        GpioLockPadCfgTxForGroupDw (GroupIndex + GpioGroupOffset, DwNum, DwRegsValues[DwNum].PadsToLockTx);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  This procedure will clear all status bits of any GPIO interrupts.

  @param[in] none

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
STATIC
EFI_STATUS
GpioClearAllGpioInterrupts (
  VOID
  )
{
  GPIO_GROUP           Group;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  GPIO_GROUP           GpioGroupLowest;
  GPIO_GROUP           GpioGroupHighest;
  UINT32               GroupIndex;
  UINTN                GpioGroupInfoLength;
  UINT32               DwNum;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupLowest = GpioGetLowestGroup ();
  GpioGroupHighest = GpioGetHighestGroup ();

  for (Group = GpioGroupLowest; Group <= GpioGroupHighest; Group++) {
    GroupIndex = GpioGetGroupIndexFromGroup (Group);
    //
    // Check if group has GPI IS register
    //
    if (GpioGroupInfo[GroupIndex].GpiIsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all GPI_IS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiIsOffset + DwNum * 0x4),
          0xFFFFFFFF
          );
      }
    }

    //
    // Check if group has GPI_GPE_STS register
    //
    if (GpioGroupInfo[GroupIndex].GpiGpeStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all GPI_GPE_STS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeStsOffset + DwNum * 0x4),
          0xFFFFFFFF
          );
      }
    }

    //
    // Check if group has SMI_STS register
    //
    if (GpioGroupInfo[GroupIndex].SmiStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all SMI_STS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].SmiStsOffset + DwNum * 4),
          0xFFFFFFFF
          );
      }
    }

    //
    // Check if group has NMI_STS register
    //
    if (GpioGroupInfo[GroupIndex].NmiStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all NMI_STS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].NmiStsOffset + DwNum * 4),
          0xFFFFFFFF
          );
      }
    }

  }
  return EFI_SUCCESS;
}

/**
  This procedure will initialize multiple GPIO pins. Use GPIO_INIT_CONFIG structure.
  Structure contains fields that can be used to configure each pad.
  Pad not configured using GPIO_INIT_CONFIG will be left with hardware default values.
  Separate fields could be set to hardware default if it does not matter, except
  GpioPad and PadMode.
  Some GpioPads are configured and switched to native mode by RC, those include:
  SerialIo pins, ISH pins, ClkReq Pins

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
EFI_STATUS
GpioConfigurePads (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  )
{
  EFI_STATUS   Status;
  Status =  GpioConfigureSklPch (NumberOfItems, GpioInitTableAddress);
  GpioClearAllGpioInterrupts ();
  return Status;
}

