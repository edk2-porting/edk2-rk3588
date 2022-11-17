/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "GpioLibrary.h"


/**
  This procedure will handle requirement on SATA DEVSLPx pins.

  @param[in]     GpioPad        GPIO pad
  @param[in]     PadMode        GPIO PadMode value
  @param[in out] Dw0Reg         Value for PADCFG_DW0 register
  @param[in out] Dw0RegMask     Mask of bits which will change in PADCFG_DWO register

  @retval None

**/
static
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
    *Dw0Reg |= ((GpioResetPwrGood >> 1) << N_PCH_GPIO_RST_CONF);
  }
}

/**
  This SKL PCH specific procedure will initialize multiple SKL PCH GPIO pins

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
static
EFI_STATUS
GpioConfigureSklPch (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  )
{
  UINT32               Index;
  UINT32               Dw0Reg;
  UINT32               Dw0RegMask;
  UINT32               Dw1Reg;
  UINT32               Dw1RegMask;
  UINT32               PadCfgReg;
  UINT32               HostSoftOwnReg[V_PCH_GPIO_GROUP_MAX];
  UINT32               HostSoftOwnRegMask[V_PCH_GPIO_GROUP_MAX];
  UINT32               GpiGpeEnReg[V_PCH_GPIO_GROUP_MAX];
  UINT32               GpiGpeEnRegMask[V_PCH_GPIO_GROUP_MAX];
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

  ZeroMem (HostSoftOwnReg, sizeof (HostSoftOwnReg));
  ZeroMem (HostSoftOwnRegMask, sizeof (HostSoftOwnRegMask));
  ZeroMem (GpiGpeEnReg, sizeof (GpiGpeEnReg));
  ZeroMem (GpiGpeEnRegMask, sizeof (GpiGpeEnRegMask));

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupOffset = GpioGetLowestGroup ();
  NumberOfGroups = GpioGetNumberOfGroups ();

  for (Index = 0; Index < NumberOfItems; Index ++) {

    Dw0RegMask = 0;
    Dw0Reg = 0;
    Dw1RegMask = 0;
    Dw1Reg = 0;

    GpioData   = &GpioInitTableAddress[Index];

    Group      = GpioGetGroupFromGpioPad (GpioData->GpioPad);
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioData->GpioPad);
    PadNumber  = GpioGetPadNumberFromGpioPad (GpioData->GpioPad);

    if (GroupIndex >= V_PCH_GPIO_GROUP_MAX) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Invalid Group Index (GroupIndex=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      ASSERT (FALSE);
      continue;
    }

DEBUG_CODE_BEGIN();
    if (!(((PchSeries == PchH) && (GPIO_GET_CHIPSET_ID(GpioData->GpioPad) == GPIO_SKL_H_CHIPSET_ID)) ||
        ((PchSeries == PchLp) && (GPIO_GET_CHIPSET_ID(GpioData->GpioPad) == GPIO_SKL_LP_CHIPSET_ID)))) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
DEBUG_CODE_END();

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

    if (GpioIsPadLocked (GroupIndex, PadNumber)) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pad is locked (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      continue;
    }

    if (DebugCodeEnabled ()) {

    //
    // Check if selected GPIO Pad is not owned by CSME/ISH
    //
    GpioGetPadOwnership (GpioData->GpioPad, &PadOwnVal);

    if (PadOwnVal != GpioPadOwnHost) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
      DEBUG ((DEBUG_ERROR, "** Please make sure the GPIO usage in sync between CSME and BIOS configuration. \n"));
      DEBUG ((DEBUG_ERROR, "** All the GPIO occupied by CSME should not do any configuration by BIOS.\n"));
      continue;
    }

    }

    //
    // Configure Reset Type (PadRstCfg)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.PowerConfig & GPIO_CONF_RESET_MASK) >> GPIO_CONF_RESET_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RST_CONF);
    Dw0Reg |= (((GpioData->GpioConfig.PowerConfig & GPIO_CONF_RESET_MASK) >> (GPIO_CONF_RESET_BIT_POS + 1)) << N_PCH_GPIO_RST_CONF);

    //
    // Configure how interrupt is triggered (RxEvCfg)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.InterruptConfig & GPIO_CONF_INT_TRIG_MASK) >> GPIO_CONF_INT_TRIG_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RX_LVL_EDG);
    Dw0Reg |= (((GpioData->GpioConfig.InterruptConfig & GPIO_CONF_INT_TRIG_MASK) >> (GPIO_CONF_INT_TRIG_BIT_POS + 1)) << N_PCH_GPIO_RX_LVL_EDG);


    //
    // Configure interrupt generation (GPIRoutIOxAPIC/SCI/SMI/NMI)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.InterruptConfig & GPIO_CONF_INT_ROUTE_MASK) >> GPIO_CONF_INT_ROUTE_BIT_POS) == GpioHardwareDefault)  ? 0x0 : (B_PCH_GPIO_RX_NMI_ROUTE | B_PCH_GPIO_RX_SCI_ROUTE | B_PCH_GPIO_RX_SMI_ROUTE | B_PCH_GPIO_RX_APIC_ROUTE));
    Dw0Reg |= (((GpioData->GpioConfig.InterruptConfig & GPIO_CONF_INT_ROUTE_MASK) >> (GPIO_CONF_INT_ROUTE_BIT_POS + 1)) << N_PCH_GPIO_RX_NMI_ROUTE);

    //
    // Configure GPIO direction (GPIORxDis and GPIOTxDis)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.Direction & GPIO_CONF_DIR_MASK) >> GPIO_CONF_DIR_BIT_POS) == GpioHardwareDefault) ? 0x0 : (B_PCH_GPIO_RXDIS | B_PCH_GPIO_TXDIS));
    Dw0Reg |= (((GpioData->GpioConfig.Direction & GPIO_CONF_DIR_MASK) >> (GPIO_CONF_DIR_BIT_POS + 1)) << N_PCH_GPIO_TXDIS);

    //
    // Configure GPIO input inversion (RXINV)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.Direction & GPIO_CONF_INV_MASK) >> GPIO_CONF_INV_BIT_POS) == GpioHardwareDefault) ?  0x0 : B_PCH_GPIO_RXINV);
    Dw0Reg |= (((GpioData->GpioConfig.Direction & GPIO_CONF_INV_MASK) >> (GPIO_CONF_INV_BIT_POS + 1)) << N_PCH_GPIO_RXINV);

    //
    // Configure GPIO output state (GPIOTxState)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.OutputState & GPIO_CONF_OUTPUT_MASK) >> GPIO_CONF_OUTPUT_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_TX_STATE);
    Dw0Reg |= (((GpioData->GpioConfig.OutputState & GPIO_CONF_OUTPUT_MASK) >> (GPIO_CONF_OUTPUT_BIT_POS + 1)) << N_PCH_GPIO_TX_STATE);

    //
    // Configure GPIO RX raw override to '1' (RXRAW1)
    //
    Dw0RegMask |= ((((GpioData->GpioConfig.OtherSettings & GPIO_CONF_RXRAW_MASK) >> GPIO_CONF_RXRAW_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_RX_RAW1);
    Dw0Reg |= (((GpioData->GpioConfig.OtherSettings & GPIO_CONF_RXRAW_MASK) >> (GPIO_CONF_RXRAW_BIT_POS + 1)) << N_PCH_GPIO_RX_RAW1);

    //
    // Configure GPIO Pad Mode (PMode)
    //
    if (((GpioData->GpioPad == GPIO_SKL_H_GPP_B2) ||
        (GpioData->GpioPad == GPIO_SKL_H_GPD7) ||
        (GpioData->GpioPad == GPIO_SKL_H_GPD9)) &&
        (GpioData->GpioConfig.PadMode != GpioPadModeGpio)) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group/Index: %d/%d, Pad: %d cannot be set as NATIVE. Force it to GPIO mode!\n", Group, GroupIndex, PadNumber));
      Dw0RegMask |= B_PCH_GPIO_PAD_MODE;
      Dw0Reg |= ((GpioPadModeGpio >> (GPIO_CONF_PAD_MODE_BIT_POS + 1)) << N_PCH_GPIO_PAD_MODE);
    } else {
      Dw0RegMask |= ((((GpioData->GpioConfig.PadMode & GPIO_CONF_PAD_MODE_MASK) >> GPIO_CONF_PAD_MODE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PAD_MODE);
      Dw0Reg |= (((GpioData->GpioConfig.PadMode & GPIO_CONF_PAD_MODE_MASK) >> (GPIO_CONF_PAD_MODE_BIT_POS + 1)) << N_PCH_GPIO_PAD_MODE);
    }

    //
    // Configure GPIO termination (Term)
    //
    Dw1RegMask |= ((((GpioData->GpioConfig.ElectricalConfig & GPIO_CONF_TERM_MASK) >> GPIO_CONF_TERM_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_TERM);
    Dw1Reg |= (((GpioData->GpioConfig.ElectricalConfig & GPIO_CONF_TERM_MASK) >> (GPIO_CONF_TERM_BIT_POS + 1)) << N_PCH_GPIO_TERM);

    //
    // Configure GPIO pad tolerance (padtol)
    //
    Dw1RegMask |= ((((GpioData->GpioConfig.ElectricalConfig & GPIO_CONF_PADTOL_MASK) >> GPIO_CONF_PADTOL_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PADTOL);
    Dw1Reg |= (((GpioData->GpioConfig.ElectricalConfig & GPIO_CONF_PADTOL_MASK) >> (GPIO_CONF_PADTOL_BIT_POS + 1)) << N_PCH_GPIO_PADTOL);

    //
    // Check for additional requirements on setting PADCFG register
    //
    GpioHandleSataDevSlpPad (GpioData->GpioPad, GpioData->GpioConfig.PadMode, &Dw0Reg, &Dw0RegMask);

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
    HostSoftOwnRegMask[GroupIndex] |= (GpioData->GpioConfig.HostSoftPadOwn & 0x1) << PadNumber;
    HostSoftOwnReg[GroupIndex] |= (GpioData->GpioConfig.HostSoftPadOwn >> 0x1) << PadNumber;

    //
    // Update value to be programmed in GPI_GPE_EN register
    //
    GpiGpeEnRegMask[GroupIndex] |= (GpioData->GpioConfig.InterruptConfig & 0x1) << PadNumber;
    GpiGpeEnReg[GroupIndex] |= ((GpioData->GpioConfig.InterruptConfig & GpioIntSci) >> 3) << PadNumber;
  }

  for (Index = 0; Index < NumberOfGroups; Index++) {
    //
    // Write HOSTSW_OWN registers
    //
    if (GpioGroupInfo[Index].HostOwnOffset != NO_REGISTER_FOR_PROPERTY) {
      MmioAndThenOr32 (
        (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[Index].Community, GpioGroupInfo[Index].HostOwnOffset),
        ~(UINT32)HostSoftOwnRegMask[Index],
        (UINT32)HostSoftOwnReg[Index]
        );
    }

    //
    // Write GPI_GPE_EN registers
    //
    if (GpioGroupInfo[Index].GpiGpeEnOffset != NO_REGISTER_FOR_PROPERTY) {
      MmioAndThenOr32 (
        (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[Index].Community, GpioGroupInfo[Index].GpiGpeEnOffset),
        ~(UINT32)GpiGpeEnRegMask[Index],
        (UINT32)GpiGpeEnReg[Index]
        );
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
static
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

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupLowest = GpioGetLowestGroup ();
  GpioGroupHighest = GpioGetHighestGroup ();

  for (Group = GpioGroupLowest; Group <= GpioGroupHighest; Group++) {
    GroupIndex = GpioGetGroupIndexFromGroup (Group);
    //
    // Check if group has GPI IS register
    //
    if (GpioGroupInfo[Group].GpiIsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all GPI_IS Status bits by writing '1'
      //
      MmioWrite32 (
        PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiIsOffset),
        (UINT32)0xFFFFFFFF
        );
    }

    //
    // Check if group has GPI_GPE_STS register
    //
    if (GpioGroupInfo[GroupIndex].GpiGpeStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all GPI_GPE_STS Status bits by writing '1'
      //
      MmioWrite32 (
        PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeStsOffset),
        (UINT32)0xFFFFFFFF
        );
    }

    //
    // Check if group has SMI_STS register
    //
    if (GpioGroupInfo[GroupIndex].SmiStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all SMI_STS Status bits by writing '1'
      //
      MmioWrite32 (
        PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].SmiStsOffset),
        (UINT32)0xFFFFFFFF
        );
    }

    //
    // Check if group has NMI_STS register
    //
    if (GpioGroupInfo[GroupIndex].NmiStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all NMI_STS Status bits by writing '1'
      //
      MmioWrite32 (
        PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].NmiStsOffset),
        (UINT32)0xFFFFFFFF
        );
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
