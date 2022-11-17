/** @file
  This file contains PSF routines for RC usage

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPsfPrivateLib.h>
#include <Library/PchPciExpressHelpersLib.h>
#include <Library/S3BootScriptLib.h>

/**
  This procedure will disable CIO2 device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableCio2Device (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF1,
    R_PCH_LP_PCR_PSF1_T0_SHDW_CAM_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    0xFFFFFFFF,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Hide CIO2 devices PciCfgSpace at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfHideCio2Device (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF1,
    R_PCH_LP_PCR_PSF1_T0_SHDW_CAM_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS
    );
}

/**
  Disable HDAudio device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableHdaDevice (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_AUD_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable xDCI device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableXdciDevice (
  VOID
  )
{
  UINT16 RegOffset;

  if (GetPchSeries () == PchLp) {
    RegOffset = R_PCH_LP_PCR_PSF2_T0_SHDW_OTG_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  } else {
    RegOffset = R_PCH_H_PCR_PSF2_T0_SHDW_OTG_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  }

  PchPcrAndThenOr32 (
    PID_PSF2,
    RegOffset,
    ~0u,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable xHCI device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableXhciDevice (
  VOID
  )
{
  UINT16 RegOffset;

  if (GetPchSeries () == PchLp) {
    RegOffset = R_PCH_LP_PCR_PSF2_T0_SHDW_XHCI_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  } else {
    RegOffset = R_PCH_H_PCR_PSF2_T0_SHDW_XHCI_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  }

  PchPcrAndThenOr32 (
    PID_PSF2,
    RegOffset,
    ~0u,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable SATA device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableSataDevice (
  VOID
  )
{
  UINT16 RegOffset;

  if (GetPchSeries () == PchLp) {
    RegOffset = R_PCH_LP_PCR_PSF1_T0_SHDW_SATA_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  } else {
    RegOffset = R_PCH_H_PCR_PSF1_T0_SHDW_SATA_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  }

  PchPcrAndThenOr32 (
    PID_PSF1,
    RegOffset,
    ~0u,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable ISH device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableIshDevice (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_ISH_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    ~0u,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable ISH BAR1 at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableIshBar1 (
  VOID
  )
{
  //
  // Set AGNT_T0_SHDW_PCIEN[19:18] = 11b
  // ISH device BAR is 64bit wide so to disable BAR1
  // at PSF both BAR2 (32bit wide) and BAR3 (32bit wide) need to be disabled
  //
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_ISH_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    (UINT32) ~0,
    (B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR2DIS | B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR3DIS)
    );
}

/**
  Disable GbE device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableGbeDevice (
  VOID
  )
{
  UINT16       RegOffset;
  PCH_SBI_PID  PortId;

  if (GetPchSeries () == PchLp) {
    PortId = PID_PSF1;
    RegOffset = R_PCH_LP_PCR_PSF1_T0_SHDW_GBE_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  } else {
    PortId = PID_PSF3;
    RegOffset = R_PCH_H_PCR_PSF3_T0_SHDW_GBE_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN;
  }

  PchPcrAndThenOr32 (
    PortId,
    RegOffset,
    ~0u,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable SMBUS device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableSmbusDevice (
  VOID
  )
{
  ///
  /// Hide SMBUS device
  /// Set PCR[PSF_3] + "AGNT_T0_SHDW_CFG_DIS"[SMBUS][0]
  ///
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_SMBUS_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS
    );

  ///
  /// Function disable SMBUS device
  /// Set PCR[PSF_3] + "AGNT_T0_SHDW_PCIEN"[SMBUS][8]
  ///
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_SMBUS_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable Thermal device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableThermalDevice (
  VOID
  )
{
  ///
  /// Set PCR[PSF2] TRH PCIEN[8] to 1
  ///
  PchPcrAndThenOr32 (
    PID_PSF2, R_PCH_PCR_PSF2_T0_SHDW_TRH_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Hide Thermal device PciCfgSpace at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfHideThermalDevice (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF2, R_PCH_PCR_PSF2_T0_SHDW_TRH_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS
    );
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpScsDevicePsfRegBase[] =
{
  R_PCH_LP_PCR_PSF2_T0_SHDW_EMMC_REG_BASE,
  R_PCH_LP_PCR_PSF2_T0_SHDW_SDIO_REG_BASE,
  R_PCH_LP_PCR_PSF2_T0_SHDW_SDCARD_REG_BASE,
  R_PCH_LP_PCR_PSF2_T0_SHDW_UFS_REG_BASE
};

/**
  Disable SCS device at PSF level

  @param[in] ScsDevNum   SCS Device

  @retval None
**/
VOID
PsfDisableScsDevice (
  IN PCH_PSF_SCS_DEV_NUM  ScsDevNum
  )
{
  if (ScsDevNum >= PchPsfMaxScsDevNum) {
    ASSERT (FALSE);
  }

  PchPcrAndThenOr32 (
    PID_PSF2,
    mPchLpScsDevicePsfRegBase[ScsDevNum] + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable SCS devices BAR1 PSF level

  @param[in] ScsDevNum   SCS Device

  @retval None
**/
VOID
PsfDisableScsBar1 (
  IN PCH_PSF_SCS_DEV_NUM  ScsDevNum
  )
{
  if (ScsDevNum >= PchPsfMaxScsDevNum) {
    ASSERT (FALSE);
  }

  //
  // Set AGNT_T0_SHDW_PCIEN[19:18] = 11b
  // SCS device BAR is 64bit wide so to disable BAR1
  // at PSF both BAR2 (32bit wide) and BAR3 (32bit wide) need to be disabled
  //
  PchPcrAndThenOr32 (
    PID_PSF2,
    mPchLpScsDevicePsfRegBase[ScsDevNum] + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    (UINT32) ~0,
    (B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR2DIS | B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR3DIS)
    );
}

/**
  Hide TraceHub ACPI devices PciCfgSpace at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfHideTraceHubAcpiDevice (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_TRACE_HUB_ACPI_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS,
    (UINT32) ~0,
    B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS
    );
}

/**
  This procedure will hide TraceHub PciCfgSpace at PSF level

  @retval None
**/
VOID
PsfHideTraceHubDevice (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_TRACE_HUB_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS,
    ~0u,
    B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS
    );
}

/**
  This procedure will reveal TraceHub PciCfgSpace at PSF level

  @retval None
**/
VOID
PsfRevealTraceHubDevice (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_TRACE_HUB_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS,
    (UINT32) ~(B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS),
    0
    );
}

/**
  This procedure will disable and hide TraceHub device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfDisableTraceHubDevice (
  VOID
  )
{
  PsfHideTraceHubDevice ();

  PchPcrAndThenOr32 (
    PID_PSF3,
    R_PCH_PCR_PSF3_T0_SHDW_TRACE_HUB_REG_BASE + R_PCH_PCR_PSFX_T0_SHDW_PCIEN,
    0xFFFFFFFF,
    B_PCH_PCR_PSFX_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Calculates offset of PCR register where given rootport can be enabled/disabled

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Offset
**/
static UINT16
GetPsfPcieRootPortOffset (
  IN UINT32 RpIndex
  )
{
  UINT16          RootPortReg;

  if (GetPchSeries () == PchLp) {
    RootPortReg = R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE;
  } else {
    if (GetPchGeneration () == KblPch) {
      RootPortReg = R_KBL_PCH_H_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE;
    } else {
      RootPortReg = R_SKL_PCH_H_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE;
    }
  }
  return (RootPortReg - ((UINT16)RpIndex * 0x100) + R_PCH_PCR_PSFX_T1_SHDW_PCIEN);
}

/**
  Re-enable PCIe Root Port at PSF level after it was disabled

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval None
**/
VOID
PsfEnablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  ASSERT (RpIndex < GetPchMaxPciePortNum ());

  PchPcrAndThenOr32 (PID_PSF1, GetPsfPcieRootPortOffset (RpIndex), (UINT32)(~B_PCH_PCR_PSFX_T1_SHDW_PCIEN_FUNDIS), 0);
}

/**
  Disable PCIe Root Port at PSF level

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval None
**/
VOID
PsfDisablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  ASSERT (RpIndex < GetPchMaxPciePortNum ());

  PchPcrAndThenOr32 (PID_PSF1, GetPsfPcieRootPortOffset (RpIndex), ~0u, B_PCH_PCR_PSFX_T1_SHDW_PCIEN_FUNDIS);
}

/**
  Disable PCIe Root Port at PSF level.
  This function will also perform S3 boot script programming

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval None
**/
VOID
PsfDisablePcieRootPortWithS3BootScript (
  IN UINT32  RpIndex
  )
{
  UINT16          RootPortReg;
  UINT32          Data32And;
  UINT32          Data32Or;

  if (GetPchSeries () == PchLp) {
    if (RpIndex >= PCH_LP_PCIE_MAX_ROOT_PORTS) {
      ASSERT (FALSE);
      return;
    }
    RootPortReg = R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE;
  } else {
    if (GetPchGeneration () == KblPch) {
      if (RpIndex >= KBL_PCH_H_PCIE_MAX_ROOT_PORTS) {
        ASSERT (FALSE);
        return;
      }
      RootPortReg = R_KBL_PCH_H_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE;
    } else {
      if (RpIndex >= SKL_PCH_H_PCIE_MAX_ROOT_PORTS) {
        ASSERT (FALSE);
        return;
      }
      RootPortReg = R_SKL_PCH_H_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE;
    }
  }

  Data32And = ~0u;
  Data32Or  = B_PCH_PCR_PSFX_T1_SHDW_PCIEN_FUNDIS;

  PchPcrAndThenOr32 (
    PID_PSF1,
    RootPortReg - ((UINT16)RpIndex * 0x100) + R_PCH_PCR_PSFX_T1_SHDW_PCIEN,
    Data32And,
    Data32Or
    );

  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_PSF1, (RootPortReg - ((UINT16)RpIndex * 0x100) + R_PCH_PCR_PSFX_T1_SHDW_PCIEN),
    &Data32Or,
    &Data32And
    );
}

/**
  Program PSF grant counts for PCI express depending on controllers configuration

  @param[in] Controller        PCIe controller index
  @param[in] ControllerConfig  Port configuration of controller

  @retval Status
**/
EFI_STATUS
PsfConfigurePcieGrantCounts (
  UINT8       Controller,
  UINT8       ControllerConfig
  )
{
  static CONST UINT8 DgcrPairsLp [PCH_LP_PCIE_MAX_CONTROLLERS][PCH_PCIE_CONTROLLER_PORTS][2] = {
    { {  5,  6 }, {  8,  9 }, { 11, 12 }, { 14, 15 } }, //SPA
    { { 19, 20 }, { 22, 23 }, { 25, 26 }, { 28, 29 } }, //SPB
    { { 33, 34 }, { 36, 37 }, { 39, 40 }, { 42, 43 } }, //SPC
  };

  static CONST UINT8 PgTgtLp [PCH_LP_PCIE_MAX_CONTROLLERS][PCH_PCIE_CONTROLLER_PORTS] = {
    {  4,  5,  6,  7 }, //SPA
    { 12, 13, 14, 15 }, //SPB
    { 20, 21, 22, 23 }  //SPC
  };

  static CONST UINT8 DgcrPairsH [KBL_PCH_H_PCIE_MAX_CONTROLLERS][PCH_PCIE_CONTROLLER_PORTS][2] = {
    { { 11, 12 }, { 14, 15 }, { 17, 18 }, { 20, 21 } }, //SPA
    { { 24, 25 }, { 27, 28 }, { 30, 31 }, { 33, 34 } }, //SPB
    { { 37, 38 }, { 40, 41 }, { 43, 44 }, { 46, 47 } }, //SPC
    { { 50, 51 }, { 53, 54 }, { 56, 57 }, { 59, 60 } }, //SPD
    { { 63, 64 }, { 66, 67 }, { 69, 70 }, { 72, 73 } }, //SPE
    { {182,183 }, {185,186 }, {188,189 }, {191,192 } }  //SPF, only for KBL-H
  };

  static CONST UINT8 PgTgtH [KBL_PCH_H_PCIE_MAX_CONTROLLERS][PCH_PCIE_CONTROLLER_PORTS] = {
    {  0,  1,  2,  3 }, //SPA
    {  4,  5,  6,  7 }, //SPB
    { 12, 13, 14, 15 }, //SPC
    { 20, 21, 22, 23 }, //SPD
    { 28, 29, 30, 31 }, //SPE
    { 32, 33, 34, 35 }  //SPF, only for KBL-H
  };

  static CONST UINT8 DefaultGrantCount = 4;

  PCH_SERIES  PchSeries;
  UINT32      Dgcr0Addr;
  UINT32      PgTgt0Addr;
  UINT32      DgcrNo0;
  UINT32      DgcrNo1;
  UINT32      PgTgtNo;
  UINT32      Channel;
  UINT32      ChannelGrant[PCH_PCIE_CONTROLLER_PORTS];

  DEBUG ((DEBUG_INFO, "PsfConfigurePcieGrantCounts(%d) Start\n", Controller));

  PchSeries = GetPchSeries ();
  if (PchSeries == PchLp) {
    Dgcr0Addr     = R_PCH_LP_PSF1_DEV_GNTCNT_RELOAD_DGCR0;
    PgTgt0Addr    = R_PCH_LP_PSF1_TARGET_GNTCNT_RELOAD_PG1_TGT0;
  } else {
    if (GetPchGeneration () == KblPch) {
      Dgcr0Addr     = R_KBL_PCH_H_PSF1_DEV_GNTCNT_RELOAD_DGCR0;
      PgTgt0Addr    = R_KBL_PCH_H_PSF1_TARGET_GNTCNT_RELOAD_PG1_TGT0;
    } else {
      Dgcr0Addr     = R_SKL_PCH_H_PSF1_DEV_GNTCNT_RELOAD_DGCR0;
      PgTgt0Addr    = R_SKL_PCH_H_PSF1_TARGET_GNTCNT_RELOAD_PG1_TGT0;
    }
  }


  switch (ControllerConfig) {
    case V_PCH_PCIE_STRPFUSECFG_RPC_1_1_1_1: //Pcie4x1
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case V_PCH_PCIE_STRPFUSECFG_RPC_2_1_1: //Pcie1x2_2x1
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DefaultGrantCount;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case V_PCH_PCIE_STRPFUSECFG_RPC_2_2: //Pcie2x2
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DefaultGrantCount;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DefaultGrantCount;
      break;
    case V_PCH_PCIE_STRPFUSECFG_RPC_4: //Pcie1x4
      ChannelGrant[0] = 4;
      ChannelGrant[1] = DefaultGrantCount;
      ChannelGrant[2] = DefaultGrantCount;
      ChannelGrant[3] = DefaultGrantCount;
      break;
    default:
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }

  for (Channel = 0; Channel < PCH_PCIE_CONTROLLER_PORTS; ++Channel) {
    if (PchSeries == PchLp) {
      DgcrNo0 = DgcrPairsLp[Controller][Channel][0];
      DgcrNo1 = DgcrPairsLp[Controller][Channel][1];
      PgTgtNo = PgTgtLp[Controller][Channel];
    } else {
      DgcrNo0 = DgcrPairsH[Controller][Channel][0];
      DgcrNo1 = DgcrPairsH[Controller][Channel][1];
      PgTgtNo = PgTgtH[Controller][Channel];
    }

    DEBUG ((DEBUG_INFO, "DGCR%d = %d\n", DgcrNo0, ChannelGrant[Channel]));
    PchPcrAndThenOr32 (
      PID_PSF1,
      (UINT16) (Dgcr0Addr  + (DgcrNo0 * S_PCH_PSF_DEV_GNTCNT_RELOAD_DGCR)),
      (UINT32) ~B_PCH_PSF_DEV_GNTCNT_RELOAD_DGCR_GNT_CNT_RELOAD,
      ChannelGrant[Channel]
      );
    DEBUG ((DEBUG_INFO, "DGCR%d = %d\n", DgcrNo1, ChannelGrant[Channel]));
    PchPcrAndThenOr32 (
      PID_PSF1,
      (UINT16) (Dgcr0Addr  + (DgcrNo1 * S_PCH_PSF_DEV_GNTCNT_RELOAD_DGCR)),
      (UINT32) ~B_PCH_PSF_DEV_GNTCNT_RELOAD_DGCR_GNT_CNT_RELOAD,
      ChannelGrant[Channel]
      );
    DEBUG ((DEBUG_INFO, "PG1_TGT%d = %d\n", PgTgtNo, ChannelGrant[Channel]));
    PchPcrAndThenOr32 (
      PID_PSF1,
      (UINT16) (PgTgt0Addr + (PgTgtNo * S_PCH_PSF_TARGET_GNTCNT_RELOAD)),
      (UINT32) ~B_PCH_PSF_TARGET_GNTCNT_RELOAD_GNT_CNT_RELOAD,
      ChannelGrant[Channel]
      );
  }

  DEBUG ((DEBUG_INFO, "PsfConfigurePcieGrantCounts() End\n"));

  return EFI_SUCCESS;
}

/**
  Disable ISM NP Completion Tracking for GbE PSF port

  @param[in] None

  @retval None
**/
VOID
PsfDisableIsmNpCompletionTrackForGbe (
  VOID
  )
{
  UINT16       RegOffset;
  PCH_SBI_PID  PortId;

  if (GetPchSeries () == PchLp) {
    PortId = PID_PSF1;
    RegOffset = R_PCH_LP_PCR_PSF1_PSF_PORT_CONFIG_PG1_PORT7;
  } else {
    PortId = PID_PSF3;
    RegOffset = R_PCH_H_PCR_PSF3_PSF_PORT_CONFIG_PG1_PORT1;
  }
  ///
  /// Clear PCR PSF_x_PSF_PORT_CONFIG_PGx_PORTx[5] = 0b
  ///
  PchPcrAndThenOr8 (
    PortId,
    RegOffset,
    (UINT8) ~(BIT5),
    0
    );
}

/**
  Program PSF EOI Multicast configuration

  @param[in] None

  @retval None
**/
VOID
PsfSetEoiMulticastConfiguration (
  VOID
  )
{
  UINT32                McastTarget;

  ///
  /// Program PSF Multicast registers accordingly to SKL PCH BWG 5.14.4 PSF EOI Multicast Configuration
  ///
  if (GetPchSeries () == PchLp) {
    McastTarget = V_PCH_LP_PCR_PSFX_PSF_MC_AGENT_MCAST_TGT_P2SB;
    ///
    /// Set PCR[PSF1] + 4060h = 38A00h
    ///
    PchPcrWrite32 (
      PID_PSF1, R_PCH_LP_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI,
      McastTarget
      );
    ///
    /// Set PCR[PSF1] + 4050h[7:0] = 11b
    ///
    PchPcrWrite32 (
      PID_PSF1, R_PCH_LP_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI,
      0x3
      );
  } else {
    McastTarget = V_PCH_H_PCR_PSFX_PSF_MC_AGENT_MCAST_TGT_P2SB;
    if (GetPchGeneration () == KblPch) {
      ///
      /// Set PCR[PSF1] + 405Ch = 38B00h
      ///
      PchPcrWrite32 (
        PID_PSF1, R_KBL_PCH_H_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI,
        McastTarget
        );
      ///
      /// Set PCR[PSF1] + 404Ch[7:0] = 11b
      ///
      PchPcrWrite32 (
        PID_PSF1, R_KBL_PCH_H_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI,
        0x3
        );
    } else {
      ///
      /// Set PCR[PSF1] + 4058h = 38B00h
      ///
      PchPcrWrite32 (
        PID_PSF1, R_SKL_PCH_H_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI,
        McastTarget
        );
      ///
      /// Set PCR[PSF1] + 4048h[7:0] = 11b
      ///
      PchPcrWrite32 (
        PID_PSF1, R_SKL_PCH_H_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI,
        0x3
        );
    }
  }
  ///
  /// Set PCR[PSF1] + 4054h to P2SB target
  ///
  PchPcrWrite32 (
    PID_PSF3, R_PCH_PCR_PSF3_PSF_MC_AGENT_MCAST0_TGT0_EOI,
    McastTarget
    );
  ///
  /// Set PCR[PSF3] + 404Ch[7:0] = 11b
  ///
  PchPcrWrite32 (
    PID_PSF3, R_PCH_PCR_PSF3_PSF_MC_CONTROL_MCAST0_EOI,
    0x3
    );
}

/**
  This function enables EOI message forwarding in PSF for PCIe ports
  for cases where IOAPIC is present behind this root port.

  @param[in] RpIndex        Root port index (0 based)

  @retval Status
**/
EFI_STATUS
PsfConfigurEoiForPciePort (
  IN  UINT32  RpIndex
  )
{

  UINT32          Data32;
  UINT8           NumMcVal;
  UINT32          RpDestId;
  ///
  /// PCH-LP:
  /// RP Destination ID  RP  Destination ID
  ///  1     0x18100      7     0x18302
  ///  2     0x18101      8     0x18303
  ///  3     0x18102      9     0x18500
  ///  4     0x18103      10    0x18501
  ///  5     0x18300      11    0x18502
  ///  6     0x18301      12    0x18503
  ///
  static UINT32   PchSklLpRpDestId[PCH_LP_PCIE_MAX_ROOT_PORTS] =
  {
    0x18100, 0x18101, 0x18102, 0x18103, 0x18300,
    0x18301, 0x18302, 0x18303, 0x18500, 0x18501,
    0x18502, 0x18503
  };
  ///
  /// PCH-H:
  /// RP Destination ID  RP  Destination ID
  ///  1     0x18000      11    0x18302
  ///  2     0x18001      12    0x18303
  ///  3     0x18002      13    0x18500
  ///  4     0x18003      14    0x18501
  ///  5     0x18100      15    0x18502
  ///  6     0x18101      16    0x18503
  ///  7     0x18102      17    0x18700
  ///  8     0x18103      18    0x18701
  ///  9     0x18300      19    0x18702
  ///  10    0x18301      20    0x18703
  ///
  /// KBL-H: same as SKL-H plus the following 4 ports
  ///  21    0x18800      23    0x18802
  ///  22    0x18801      24    0x18803
  ///
  static UINT32   PchHRpDestId[KBL_PCH_H_PCIE_MAX_ROOT_PORTS] =
  {
    0x18000, 0x18001, 0x18002, 0x18003,
    0x18100, 0x18101, 0x18102, 0x18103,
    0x18300, 0x18301, 0x18302, 0x18303,
    0x18500, 0x18501, 0x18502, 0x18503,
    0x18700, 0x18701, 0x18702, 0x18703,
    0x18800, 0x18801, 0x18802, 0x18803
  };
  PCH_SERIES      PchSeries;
  PCH_GENERATION  PchGen;
  UINT16          PsfReg;

  ASSERT (RpIndex < GetPchMaxPciePortNum ());

  PchSeries = GetPchSeries ();
  PchGen = GetPchGeneration ();

  ///
  /// If there is an IOAPIC discovered behind root port program PSF Multicast registers
  /// accordingly to SKL PCH BWG 5.14.4 PSF EOI Multicast Configuration
  ///
  ///
  ///
  ///
  /// Read PSF_1_PSF_MC_CONTROL_MCAST0_RS0_EOI bit [7:1], increase it by 1, write back
  ///
  if (PchSeries == PchLp) {
    PsfReg = R_PCH_LP_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI;
  } else {
    if (PchGen == KblPch) {
      PsfReg = R_KBL_PCH_H_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI;
    } else {
      PsfReg = R_SKL_PCH_H_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI;
    }
  }
  PchPcrRead32 (PID_PSF1, PsfReg, &Data32);
  NumMcVal = (UINT8) (Data32 >> 1);
  Data32 += 0x2;
  PchPcrWrite32 (PID_PSF1, PsfReg, Data32);

  ///
  /// Program PSF_1_PSF_MC_AGENT_MCAST0_RS0_TGT<x>_EOI, the next empty register,
  /// where x is the same as the value read in step 1 before the increment.
  /// Program this register according to the RP# and the table below
  ///
  if (PchSeries == PchLp) {
    PsfReg = R_PCH_LP_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI;
    RpDestId = PchSklLpRpDestId[RpIndex];
  } else {
    if (PchGen == KblPch) {
      PsfReg = R_KBL_PCH_H_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI;
    } else {
      PsfReg = R_SKL_PCH_H_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI;
    }
    RpDestId = PchHRpDestId[RpIndex];
  }

  PchPcrAndThenOr32 (
    PID_PSF1, PsfReg + (NumMcVal * 4),
    (UINT32) ~0,
    RpDestId
    );

  return EFI_SUCCESS;
}

/**
  Reload default RP PSF device number and function number.
  The PSF register doesn't got reset after system reset, which will result in mismatch between
  PSF register setting and PCIE PCR PCD register setting. Reset PSF register in early phase
  to avoid cycle decoding confusing.

  @param[in] None

  @retval Status
**/
VOID
PsfReloadDefaultPcieRpDevFunc (
  VOID
  )
{
  UINT16   Psf1RpFuncCfgBase;
  UINTN    PortIndex;
  UINTN    MaxPciePorts;

  PortIndex = 0;
  MaxPciePorts = GetPchMaxPciePortNum ();

  if (GetPchSeries () == PchLp) {
    Psf1RpFuncCfgBase = R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0;
  } else {
    if (GetPchGeneration () == KblPch) {
      Psf1RpFuncCfgBase = R_KBL_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0;
    } else {
      Psf1RpFuncCfgBase = R_SKL_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0;
    }
  }
  //
  // Reload D28 registers
  //
  for (; PortIndex < 8; PortIndex++) {
    PchPcrWrite32 (
      PID_PSF1,
      Psf1RpFuncCfgBase + 4 * (UINT16) PortIndex,
      (PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1 << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_DEVICE) +
      ((PortIndex % 8) << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION)
      );
  }
  //
  // Reload D29 registers
  //
  for (; PortIndex < 16 && PortIndex < MaxPciePorts; PortIndex++) {
    PchPcrWrite32 (
      PID_PSF1,
      Psf1RpFuncCfgBase + 4 * (UINT16) PortIndex,
      (PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2 << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_DEVICE) +
      ((PortIndex % 8) << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION)
      );
  }
  //
  // Reload D27 registers
  //
  for (; PortIndex < MaxPciePorts; PortIndex++) {
    PchPcrWrite32 (
      PID_PSF1,
      Psf1RpFuncCfgBase + 4 * (UINT16) PortIndex,
      (PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3 << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_DEVICE) +
      ((PortIndex % 8) << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION)
      );
  }
}


/**
  Assign new function number for PCIe Port Number.
  This function will also perform S3 boot script programming

  @param[in] RpIndex        PCIe Root Port Index (0 based)
  @param[in] NewFunction    New Function number

  @retval None
**/
VOID
PsfSetPcieFunctionWithS3BootScript (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  )
{
  UINT16  PsfRpFuncCfgBase;
  UINT16  PsfD28F0FuncCfgBase;
  UINT32  Data32;

  if (GetPchSeries () == PchLp) {
    PsfD28F0FuncCfgBase = R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0;
  } else {
    if (GetPchGeneration () == KblPch) {
      PsfD28F0FuncCfgBase = R_KBL_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0;
    } else {
      PsfD28F0FuncCfgBase = R_SKL_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0;
    }
  }

  PsfRpFuncCfgBase = PsfD28F0FuncCfgBase + 4 * (UINT16)RpIndex;

  //
  // Program PSF1 RP function config register.
  //
  PchPcrAndThenOr32 (
    PID_PSF1,
    PsfRpFuncCfgBase,
    (UINT32) ~B_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION,
    (UINT32) (NewFunction << N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION)
    );

  PchPcrRead32 (PID_PSF1, PsfRpFuncCfgBase, &Data32);
  PCH_PCR_BOOT_SCRIPT_WRITE (
    S3BootScriptWidthUint32,
    PID_PSF1,
    PsfRpFuncCfgBase,
    1,
    &Data32
    );
}

/**
  This function enables PCIe Relaxed Order in PSF

  @param[in] None

  @retval None
**/
VOID
PsfEnablePcieRelaxedOrder (
  VOID
  )
{
  ///
  /// PCH BIOS Spec Section 8.2.9
  ///
  PchPcrAndThenOr32 (
    PID_PSF1,
    R_PCH_PCR_PSF_PORT_CONFIG_PG0_PORT0,
    ~0u,
    BIT1
    );
}

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.

  @param[in] None

  @retval None
**/
VOID
PsfConfigurePowerManagement (
  VOID
  )
{
  PchPcrAndThenOr32 (PID_PSF1,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENTCG);
  PchPcrAndThenOr32 (PID_PSF2,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENTCG);
  PchPcrAndThenOr32 (PID_PSF3,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENTCG);
  PchPcrAndThenOr32 (PID_PSF4,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENTCG);
  PchPcrAndThenOr32 (PID_CSME_PSF, R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENTCG);
  PchPcrAndThenOr32 (PID_PSF2,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENLCG);
  PchPcrAndThenOr32 (PID_PSF3,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENLCG);
  PchPcrAndThenOr32 (PID_PSF4,     R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENLCG);
  PchPcrAndThenOr32 (PID_CSME_PSF, R_PCH_PCR_PSF_GLOBAL_CONFIG, ~0u, B_PCH_PCR_PSF_GLOBAL_CONFIG_ENLCG);
}

/**
  Enable VTd support in PSF.

  @param[in] None

  @retval None
**/
VOID
PchPsfEnableVtd (
  VOID
  )
{
  STATIC PCH_SBI_PID PsfPidTable[] = {PID_PSF1, PID_PSF2, PID_PSF3, PID_PSF4};
  UINTN              PsfPidIndex;

  for (PsfPidIndex = 0; PsfPidIndex < (sizeof (PsfPidTable) / sizeof (PsfPidTable[0])); PsfPidIndex++) {
    PchPcrAndThenOr32 (
      PsfPidTable[PsfPidIndex],
      R_PCH_PCR_PSF_ROOTSPACE_CONFIG_RS0,
      ~0u,
      B_PCH_PCR_PSF_ROOTSPACE_CONFIG_RSX_VTDEN
      );
  }
}

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PchPsfDisableP2pDecoding (
  VOID
  )
{
  STATIC PCH_SBI_PID PsfPidTable[] = {PID_PSF1, PID_PSF2, PID_PSF3, PID_PSF4};
  UINTN              PsfPidIndex;

  for (PsfPidIndex = 0; PsfPidIndex < (sizeof (PsfPidTable) / sizeof (PsfPidTable[0])); PsfPidIndex++) {
    PchPcrAndThenOr32 (
      PsfPidTable[PsfPidIndex],
      R_PCH_PCR_PSF_ROOTSPACE_CONFIG_RS0,
      (UINT32)~B_PCH_PCR_PSF_ROOTSPACE_CONFIG_RSX_ENADDRP2P,
      0
      );

    PchPcrAndThenOr32 (
      PsfPidTable[PsfPidIndex],
      R_PCH_PCR_PSF_ROOTSPACE_CONFIG_RS1,
      (UINT32)~B_PCH_PCR_PSF_ROOTSPACE_CONFIG_RSX_ENADDRP2P,
      0
      );
  }
}
