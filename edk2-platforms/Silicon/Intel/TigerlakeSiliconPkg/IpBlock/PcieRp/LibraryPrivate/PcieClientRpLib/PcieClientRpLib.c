/** @file
  This file contains routines that support PCI Express initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PsfLib.h>
#include <Library/HobLib.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PcieSipRegs.h>
#include <PcieRegs.h>
#include <PchPcieRpInfo.h>
#include <CpuPcieInfo.h>
#include <CpuPcieHob.h>

/**
  Get PCIe port number for enabled port.
  @param[in] RpBase    Root Port pci segment base address

  @retval Root Port number (1 based)
**/
UINT32
PciePortNum (
  IN     UINT64  RpBase
  )
{
  return PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LCAP) >> N_PCH_PCIE_CFG_LCAP_PN;
}

/**
  Get PCIe root port index

  @param[in] RpBase    Root Port pci segment base address

  @retval Root Port index (0 based)
**/
UINT32
PciePortIndex (
  IN     UINT64  RpBase
  )
{
  return PciePortNum (RpBase) - 1;
}

/**
  This function checks whether PHY lane power gating is enabled on the port.

  @param[in] RpBase                 Root Port base address

  @retval TRUE                      PHY power gating is enabled
  @retval FALSE                     PHY power gating disabled
**/
BOOLEAN
PcieIsPhyLanePgEnabled (
  IN     UINT64  RpBase
  )
{
  UINT32 Data32;

  Data32 = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_PCIEPMECTL);
  return (Data32 & B_PCH_PCIE_CFG_PCIEPMECTL_DLSULPPGE) != 0;
}

/**
  Configures Root Port packet split.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Mps                            maximum packet size
**/
VOID
ConfigureRpPacketSplit (
  UINT64 RpBase,
  UINT8  Mps
  )
{
  PciSegmentAndThenOr32 (RpBase + R_PCIE_CFG_CCFG, (UINT32) ~(B_PCIE_CFG_CCFG_UNRS), Mps << N_PCIE_CFG_CCFG_UNRS);
}

/**
  Configures LTR override in Root Port's proprietary registers.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] DevNum                         currently visited device number
  @param[in] RpConfig                       Root Port LTR configuration
  @param[in] AspmOverride                   combination of LTR override values from all devices under this Root Port
**/
VOID
ConfigureRpLtrOverride (
  UINT64           RpBase,
  UINT32           DevNum,
  LTR_OVERRIDE     *TreeLtr,
  PCIE_LTR_CONFIG  *LtrConfig
  )
{
  UINT32       OvrEn;
  UINT32       OvrVal;
  BOOLEAN      IsCpuPcie;

  IsCpuPcie = FALSE;
  OvrEn = 0;
  OvrVal = 0;

  if (DevNum == SA_PEG0_DEV_NUM || DevNum == SA_PEG3_DEV_NUM) {
    IsCpuPcie = TRUE;
  }

  //
  // LTR settings from LTROVR register only get acknowledged on rising edge of LTROVR2[1:0]
  // If those bits were already set (that can happen on a plug-hotUnplug-hotPlug scenario),
  // they need to be toggled
  //
  if (PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LTROVR2) != 0) {
    PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR2, 0);
  }
  //
  // (*)LatencyOverrideMode = 0 -> no override
  //                          1 -> override with RP policy values
  //                          2 -> override with endpoint's override values
  //

  if (LtrConfig->ForceLtrOverride || TreeLtr->ForceOverride) {
    OvrEn |= B_PCH_PCIE_CFG_LTROVR2_FORCE_OVERRIDE;
  }
  if (LtrConfig->LtrConfigLock == TRUE) {
    OvrEn |= B_PCH_PCIE_CFG_LTROVR2_LOCK;
  }

  if (LtrConfig->SnoopLatencyOverrideMode == 1) {
    OvrEn |= B_PCH_PCIE_CFG_LTROVR2_LTRSOVREN;
    OvrVal |= LtrConfig->SnoopLatencyOverrideValue;
    OvrVal |= LtrConfig->SnoopLatencyOverrideMultiplier << 10;
    OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRSROVR;
  } else if (LtrConfig->SnoopLatencyOverrideMode == 2) {
    if (TreeLtr->MaxSnoopLatencyRequirement) {
      OvrEn |= B_PCH_PCIE_CFG_LTROVR2_LTRSOVREN;
      OvrVal |= TreeLtr->MaxSnoopLatencyValue;
      OvrVal |= TreeLtr->MaxSnoopLatencyScale << 10;
      OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRSROVR;
    }
  }
  if (LtrConfig->NonSnoopLatencyOverrideMode == 1) {
    OvrEn |= B_PCH_PCIE_CFG_LTROVR2_LTRNSOVREN;
    OvrVal |= LtrConfig->NonSnoopLatencyOverrideValue << 16;
    OvrVal |= LtrConfig->NonSnoopLatencyOverrideMultiplier << 26;
    OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRNSROVR;
  } else if (LtrConfig->NonSnoopLatencyOverrideMode == 2) {
    if (TreeLtr->MaxNoSnoopLatencyRequirement) {
      OvrEn |= B_PCH_PCIE_CFG_LTROVR2_LTRNSOVREN;
      OvrVal |= TreeLtr->MaxNoSnoopLatencyValue << 16;
      OvrVal |= TreeLtr->MaxNoSnoopLatencyScale << 26;
      OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRNSROVR;
    }
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR, OvrVal);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR2, OvrEn);

  DEBUG ((DEBUG_INFO, "ConfigureRpLtrOverride IsCpuPcie=%d\n", IsCpuPcie));
  DEBUG ((DEBUG_INFO, "ConfigureRpLtrOverride %x Val %x En %x\n", RpBase, OvrVal, OvrEn));
}

/**
  This function configures EOI message forwarding for PCIe port.
  If there's an IoAPIC behind this port, forwarding will be enabled
  Otherwise it will be disabled to minimize bus traffic

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] IoApicPresent  TRUE if there's IoAPIC behind this Root Port
**/
VOID
ConfigureEoiForwarding (
  UINT64   RpBase,
  BOOLEAN  IoApicPresent
  )
{
  UINT32 RpIndex;

  RpIndex = PciePortIndex (RpBase);

  if (IoApicPresent == FALSE) {
   PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_MPC2, B_PCH_PCIE_CFG_MPC2_EOIFD);
  } else {
    ///
    /// If there is an IOAPIC discovered behind Root Port, program PSF Multicast registers
    /// in accordance with PCH PCIe BWG  PSF EOI Multicast Configuration
    ///
    PciSegmentAnd32 (RpBase + R_PCH_PCIE_CFG_MPC2, (UINT32)~B_PCH_PCIE_CFG_MPC2_EOIFD);
    PsfConfigurEoiForPciePort (RpIndex);
  }
}

/**
  Configures proprietary parts of L1 substates configuration in Root Port

  @param[in] RpSbdf       segment:bus:device:function coordinates of Root Port
  @param[in] LtrCapable   TRUE if Root Port is LTR capable
**/
VOID
L1ssProprietaryConfiguration (
  UINT64  RpBase,
  BOOLEAN LtrCapable
  )
{
  BOOLEAN ClkreqSupported;
  BOOLEAN L1ssEnabled;
  UINT16  PcieCapOffset;
  UINT32  Data32;
  BOOLEAN L1LowSupported;

  ClkreqSupported = PcieIsPhyLanePgEnabled (RpBase);

  PcieCapOffset = PcieBaseFindExtendedCapId (RpBase, V_PCIE_EX_L1S_CID);
  if (PcieCapOffset == 0) {
    L1ssEnabled = FALSE;
  } else {
    Data32 = PciSegmentRead32 (RpBase + PcieCapOffset + R_PCIE_EX_L1SCTL1_OFFSET);
    L1ssEnabled = Data32 & (B_PCIE_EX_L1SCAP_AL1SS | B_PCIE_EX_L1SCAP_AL12S | B_PCIE_EX_L1SCAP_PPL11S |B_PCIE_EX_L1SCAP_PPL12S);
  }
  L1LowSupported = ClkreqSupported && LtrCapable && !L1ssEnabled;

  ///
  /// If L1.SNOOZ and L1.OFF (L1 Sub-States) are not supported and per-port CLKREQ# is supported, and LTR is supported:
  /// Enable L1.LOW by setting Dxx:Fn:420[17] = 1b
  ///
  if (L1LowSupported) {
    PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_PCIEPMECTL, (UINT32) B_PCH_PCIE_CFG_PCIEPMECTL_L1LE);
  } else {
    PciSegmentAnd32 (RpBase + R_PCH_PCIE_CFG_PCIEPMECTL, (UINT32) ~B_PCH_PCIE_CFG_PCIEPMECTL_L1LE);
  }

  if (L1LowSupported || L1ssEnabled) {
    ///
    /// f.  Set Dxx:Fn:420h[0] to 1b prior to L1 enabling if any L1substate is enabled (including L1.LOW)
    ///
    PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_PCIEPMECTL, B_PCH_PCIE_CFG_PCIEPMECTL_L1FSOE);
  }
}
