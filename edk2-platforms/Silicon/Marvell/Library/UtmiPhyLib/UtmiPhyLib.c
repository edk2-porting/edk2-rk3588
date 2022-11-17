/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include "UtmiPhyLib.h"

typedef struct {
  EFI_PHYSICAL_ADDRESS UtmiPllAddr;
  EFI_PHYSICAL_ADDRESS UtmiBaseAddr;
  EFI_PHYSICAL_ADDRESS UsbCfgAddr;
  EFI_PHYSICAL_ADDRESS UtmiCfgAddr;
  UINT32 UtmiPhyPort;
  UINT32 PhyId;
} UTMI_PHY_DATA;

STATIC
VOID
RegSetSilent (
  IN EFI_PHYSICAL_ADDRESS Addr,
  IN UINT32 Data,
  IN UINT32 Mask
  )
{
  UINT32 RegData;

  RegData = MmioRead32 (Addr);
  RegData &= ~Mask;
  RegData |= Data;
  MmioWrite32 (Addr, RegData);
}

STATIC
VOID
RegSet (
  IN EFI_PHYSICAL_ADDRESS Addr,
  IN UINT32 Data,
  IN UINT32 Mask
  )
{
  DEBUG((DEBUG_INFO, "Write to address = %10x, data = %10x (mask = %10x)-\n",
    Addr, Data, Mask));
  DEBUG((DEBUG_INFO, "old value = %10x ==>\n", MmioRead32 (Addr)));
  RegSetSilent (Addr, Data, Mask);
  DEBUG((DEBUG_INFO, "new value %10x\n", MmioRead32 (Addr)));
}

STATIC
VOID
UtmiPhyPowerDown (
  IN UINT32 UtmiIndex,
  IN EFI_PHYSICAL_ADDRESS UtmiBaseAddr,
  IN EFI_PHYSICAL_ADDRESS UsbCfgAddr,
  IN EFI_PHYSICAL_ADDRESS UtmiCfgAddr,
  IN UINT32 UtmiPhyPort
  )
{
  UINT32 Mask, Data;

  DEBUG((DEBUG_INFO, "UtmiPhy: stage: UTMI %d - Power down transceiver(power down Phy)\n",
    UtmiIndex));
  DEBUG((DEBUG_INFO, "UtmiPhy: stage: Power down PLL, and SuspendDM\n"));
  /* Power down UTMI PHY */
  RegSet (UtmiCfgAddr, 0x0 << UTMI_PHY_CFG_PU_OFFSET, UTMI_PHY_CFG_PU_MASK);
  /* Config USB3 Device UTMI enable */
  Mask = UTMI_USB_CFG_DEVICE_EN_MASK;

  /*
   * Prior to PHY init, configure mux for Device
   * (Device can be connected to UTMI0 or to UTMI1)
   */
  if (UtmiPhyPort == UTMI_PHY_TO_USB_DEVICE0) {
    Data = 0x1 << UTMI_USB_CFG_DEVICE_EN_OFFSET;
    /* Config USB3 Device UTMI MUX */
    Mask |= UTMI_USB_CFG_DEVICE_MUX_MASK;
    Data |= UtmiIndex << UTMI_USB_CFG_DEVICE_MUX_OFFSET;
  } else {
    Data = 0x0 << UTMI_USB_CFG_DEVICE_EN_OFFSET;
  }
  RegSet (UsbCfgAddr, Data, Mask);

  /* Set Test suspendm mode */
  Mask = UTMI_CTRL_STATUS0_SUSPENDM_MASK;
  Data = 0x1 << UTMI_CTRL_STATUS0_SUSPENDM_OFFSET;
  /* Enable Test UTMI select */
  Mask |= UTMI_CTRL_STATUS0_TEST_SEL_MASK;
  Data |= 0x1 << UTMI_CTRL_STATUS0_TEST_SEL_OFFSET;
  RegSet (UtmiBaseAddr + UTMI_CTRL_STATUS0_REG, Data, Mask);

  /* Wait for UTMI power down */
  MicroSecondDelay (1000);
}

STATIC
VOID
UtmiPhyConfig (
  IN UINT32 UtmiIndex,
  IN EFI_PHYSICAL_ADDRESS UtmiPllAddr,
  IN EFI_PHYSICAL_ADDRESS UtmiBaseAddr,
  IN EFI_PHYSICAL_ADDRESS UsbCfgAddr,
  IN EFI_PHYSICAL_ADDRESS UtmiCfgAddr,
  IN UINT32 UtmiPhyPort
  )
{
  UINT32 Mask, Data;

  DEBUG((DEBUG_INFO, "UtmiPhy: stage: Configure UTMI PHY %d registers\n",
    UtmiIndex));
  /* Reference Clock Divider Select */
  Mask = UTMI_PLL_CTRL_REFDIV_MASK;
  Data = 0x5 << UTMI_PLL_CTRL_REFDIV_OFFSET;
  /* Feedback Clock Divider Select - 90 for 25Mhz */
  Mask |= UTMI_PLL_CTRL_FBDIV_MASK;
  Data |= 0x60 << UTMI_PLL_CTRL_FBDIV_OFFSET;
  /* Select LPFR - 0x0 for 25Mhz/5=5Mhz */
  Mask |= UTMI_PLL_CTRL_SEL_LPFR_MASK;
  Data |= 0x0 << UTMI_PLL_CTRL_SEL_LPFR_OFFSET;
  RegSet (UtmiPllAddr + UTMI_PLL_CTRL_REG, Data, Mask);

  /* Impedance Calibration Threshold Setting */
  RegSet (UtmiPllAddr + UTMI_CALIB_CTRL_REG,
    0x7 << UTMI_CALIB_CTRL_IMPCAL_VTH_OFFSET,
    UTMI_CALIB_CTRL_IMPCAL_VTH_MASK);

  /* Start Impedance and PLL Calibration */
  Mask = UTMI_CALIB_CTRL_PLLCAL_START_MASK;
  Data = (0x1 << UTMI_CALIB_CTRL_PLLCAL_START_OFFSET);
  Mask |= UTMI_CALIB_CTRL_IMPCAL_START_MASK;
  Data |= (0x1 << UTMI_CALIB_CTRL_IMPCAL_START_OFFSET);
  RegSet (UtmiPllAddr + UTMI_CALIB_CTRL_REG, Data, Mask);

  /* Set LS TX driver strength coarse control */
  Mask = UTMI_TX_CH_CTRL_DRV_EN_LS_MASK;
  Data = 0x3 << UTMI_TX_CH_CTRL_DRV_EN_LS_OFFSET;
  Mask |= UTMI_TX_CH_CTRL_AMP_MASK;
  Data |= 0x4 << UTMI_TX_CH_CTRL_AMP_OFFSET;
  Mask |= UTMI_TX_CH_CTRL_IMP_SEL_LS_MASK;
  Data |= 0x3 << UTMI_TX_CH_CTRL_IMP_SEL_LS_OFFSET;
  RegSet (UtmiBaseAddr + UTMI_TX_CH_CTRL_REG, Data, Mask);

  /* Enable SQ */
  Mask = UTMI_RX_CH_CTRL0_SQ_DET_MASK;
  Data = 0x1 << UTMI_RX_CH_CTRL0_SQ_DET_OFFSET;
  /* Enable analog squelch detect */
  Mask |= UTMI_RX_CH_CTRL0_SQ_ANA_DTC_MASK;
  Data |= 0x0 << UTMI_RX_CH_CTRL0_SQ_ANA_DTC_OFFSET;
  Mask |= UTMI_RX_CH_CTRL0_DISCON_THRESH_MASK;
  Data |= 0x0 << UTMI_RX_CH_CTRL0_DISCON_THRESH_OFFSET;
  RegSet (UtmiBaseAddr + UTMI_RX_CH_CTRL0_REG, Data, Mask);

  /* Set External squelch calibration number */
  Mask = UTMI_RX_CH_CTRL1_SQ_AMP_CAL_MASK;
  Data = 0x1 << UTMI_RX_CH_CTRL1_SQ_AMP_CAL_OFFSET;
  /* Enable the External squelch calibration */
  Mask |= UTMI_RX_CH_CTRL1_SQ_AMP_CAL_EN_MASK;
  Data |= 0x1 << UTMI_RX_CH_CTRL1_SQ_AMP_CAL_EN_OFFSET;
  RegSet (UtmiBaseAddr + UTMI_RX_CH_CTRL1_REG, Data, Mask);

  /* Set Control VDAT Reference Voltage - 0.325V */
  Mask = UTMI_CHGDTC_CTRL_VDAT_MASK;
  Data = 0x1 << UTMI_CHGDTC_CTRL_VDAT_OFFSET;
  /* Set Control VSRC Reference Voltage - 0.6V */
  Mask |= UTMI_CHGDTC_CTRL_VSRC_MASK;
  Data |= 0x1 << UTMI_CHGDTC_CTRL_VSRC_OFFSET;
  RegSet (UtmiBaseAddr + UTMI_CHGDTC_CTRL_REG, Data, Mask);
}

STATIC
UINTN
UtmiPhyPowerUp (
  IN UINT32 UtmiIndex,
  IN EFI_PHYSICAL_ADDRESS UtmiPllAddr,
  IN EFI_PHYSICAL_ADDRESS UtmiBaseAddr,
  IN EFI_PHYSICAL_ADDRESS UsbCfgAddr,
  IN EFI_PHYSICAL_ADDRESS UtmiCfgAddr,
  IN UINT32 UtmiPhyPort
  )
{
  EFI_STATUS Status;
  UINT32 Data;

  DEBUG((DEBUG_INFO, "UtmiPhy: stage: UTMI %d - Power up transceiver(Power up Phy)\n",
    UtmiIndex));
  DEBUG((DEBUG_INFO, "UtmiPhy: stage: exit SuspendDM\n"));
  /* Power up UTMI PHY */
  RegSet (UtmiCfgAddr, 0x1 << UTMI_PHY_CFG_PU_OFFSET, UTMI_PHY_CFG_PU_MASK);
  /* Disable Test UTMI select */
  RegSet (UtmiBaseAddr + UTMI_CTRL_STATUS0_REG,
    0x0 << UTMI_CTRL_STATUS0_TEST_SEL_OFFSET,
    UTMI_CTRL_STATUS0_TEST_SEL_MASK);

  DEBUG((DEBUG_INFO, "UtmiPhy: stage: Wait for PLL and impedance calibration done, and PLL ready\n"));

  /* Delay 10ms */
  MicroSecondDelay (10000);

  Data = MmioRead32 (UtmiPllAddr + UTMI_CALIB_CTRL_REG);
  if ((Data & UTMI_CALIB_CTRL_IMPCAL_DONE_MASK) == 0) {
    DEBUG((DEBUG_ERROR, "UtmiPhy: Impedance calibration is not done\n"));
    Status = EFI_D_ERROR;
  }
  if ((Data & UTMI_CALIB_CTRL_PLLCAL_DONE_MASK) == 0) {
    DEBUG((DEBUG_ERROR, "UtmiPhy: PLL calibration is not done\n"));
    Status = EFI_D_ERROR;
  }
  Data = MmioRead32 (UtmiPllAddr + UTMI_PLL_CTRL_REG);
  if ((Data & UTMI_PLL_CTRL_PLL_RDY_MASK) == 0) {
    DEBUG((DEBUG_ERROR, "UtmiPhy: PLL is not ready\n"));
    Status = EFI_D_ERROR;
  }

  return Status;
}

/*
 * Cp110UtmiPhyInit initializes the UTMI PHY
 * the init split in 3 parts:
 * 1. Power down transceiver and PLL
 * 2. UTMI PHY configure
 * 3. Power up transceiver and PLL
 */
STATIC
VOID
Cp110UtmiPhyInit (
  IN UTMI_PHY_DATA *UtmiData
  )
{
  EFI_STATUS Status;

  UtmiPhyPowerDown (UtmiData->PhyId,
    UtmiData->UtmiBaseAddr,
    UtmiData->UsbCfgAddr,
    UtmiData->UtmiCfgAddr,
    UtmiData->UtmiPhyPort);

  /* Power down PLL */
  DEBUG((DEBUG_INFO, "UtmiPhy: stage: PHY power down PLL\n"));
  MmioAnd32 (UtmiData->UsbCfgAddr, ~UTMI_USB_CFG_PLL_MASK);

  UtmiPhyConfig (UtmiData->PhyId,
    UtmiData->UtmiPllAddr,
    UtmiData->UtmiBaseAddr,
    UtmiData->UsbCfgAddr,
    UtmiData->UtmiCfgAddr,
    UtmiData->UtmiPhyPort);

  Status = UtmiPhyPowerUp (UtmiData->PhyId,
             UtmiData->UtmiPllAddr,
             UtmiData->UtmiBaseAddr,
             UtmiData->UsbCfgAddr,
             UtmiData->UtmiCfgAddr,
             UtmiData->UtmiPhyPort);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UtmiPhy: Failed to initialize UTMI PHY %d\n", UtmiData->PhyId));
    return;
  }

  DEBUG ((DEBUG_ERROR, "UTMI PHY %d initialized to ", UtmiData->PhyId));
  if (UtmiData->UtmiPhyPort == UTMI_PHY_TO_USB_DEVICE0) {
    DEBUG ((DEBUG_ERROR, "USB Device\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "USB Host%d\n", UtmiData->UtmiPhyPort));
  }

  /* Power up PLL */
  DEBUG((DEBUG_INFO, "UtmiPhy: stage: PHY power up PLL\n"));
  MmioOr32 (UtmiData->UsbCfgAddr, UTMI_USB_CFG_PLL_MASK);
}

EFI_STATUS
UtmiPhyInit (
  VOID
  )
{
  MARVELL_BOARD_DESC_PROTOCOL *BoardDescProtocol;
  MV_BOARD_UTMI_DESC *BoardDesc;
  UTMI_PHY_DATA UtmiData;
  EFI_STATUS Status;
  UINTN Index;

  /* Obtain board description */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&BoardDescProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = BoardDescProtocol->BoardDescUtmiGet (BoardDescProtocol, &BoardDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Initialize enabled chips */
  for (Index = 0; Index < BoardDesc->UtmiDevCount; Index++) {
    /* Get base address of UTMI phy */
    UtmiData.UtmiBaseAddr = BoardDesc[Index].SoC->UtmiBaseAddress;

    /* Get base address of PLL registers */
    UtmiData.UtmiPllAddr = BoardDesc[Index].SoC->UtmiPllAddress;

    /* Get usb config address */
    UtmiData.UsbCfgAddr = BoardDesc[Index].SoC->UsbConfigAddress;

    /* Get UTMI config address */
    UtmiData.UtmiCfgAddr = BoardDesc[Index].SoC->UtmiConfigAddress;

    /* Get UTMI PHY ID */
    UtmiData.PhyId = BoardDesc[Index].SoC->UtmiPhyId;

    /* Get the usb port type */
    UtmiData.UtmiPhyPort = BoardDesc[Index].UtmiPortType;

    /* Currently only Cp110 is supported */
    Cp110UtmiPhyInit (&UtmiData);
  }

  BoardDescProtocol->BoardDescFree (BoardDesc);

  return EFI_SUCCESS;
}
