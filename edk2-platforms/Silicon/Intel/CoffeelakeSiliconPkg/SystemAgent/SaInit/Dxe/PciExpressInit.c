/** @file
  This driver does SA PCI Express ACPI table initialization.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PciExpressInit.h"
#include <Library/ConfigBlockLib.h>

extern SYSTEM_AGENT_NVS_AREA_PROTOCOL   mSaNvsAreaProtocol;
extern SA_CONFIG_HOB                    *mSaConfigHob;

/**
  PCI Express Dxe Initialization.
  Run before PCI Bus Init, where assignment of Bus, Memory,
    and I/O Resources are assigned.

  @param[in] SaPolicy     -     SA DXE Policy protocol

  @retval EFI_SUCCESS     - Pci Express successfully started and ready to be used
**/
EFI_STATUS
PciExpressInit (
  IN SA_POLICY_PROTOCOL *SaPolicy
  )
{
  EFI_STATUS                                    Status;
  PCIE_DXE_CONFIG                               *PcieDxeConfig;
  MSR_BROADWELL_PKG_CST_CONFIG_CONTROL_REGISTER Msr;

  Status = GetConfigBlock ((VOID *) SaPolicy, &gPcieDxeConfigGuid, (VOID *)&PcieDxeConfig);
  ASSERT_EFI_ERROR (Status);


  Msr.Uint64 = AsmReadMsr64 (MSR_BROADWELL_PKG_CST_CONFIG_CONTROL);
  mSaNvsAreaProtocol.Area->PackageCstateLimit  = (UINT8) Msr.Bits.Limit;

  mSaNvsAreaProtocol.Area->PwrDnBundlesGlobalEnable  = 0;

  if (mSaConfigHob != NULL) {
    mSaNvsAreaProtocol.Area->Peg0PowerDownUnusedBundles  = mSaConfigHob->PowerDownUnusedBundles[0];
    mSaNvsAreaProtocol.Area->Peg1PowerDownUnusedBundles  = mSaConfigHob->PowerDownUnusedBundles[1];
    mSaNvsAreaProtocol.Area->Peg2PowerDownUnusedBundles  = mSaConfigHob->PowerDownUnusedBundles[2];
    if (SA_PEG_MAX_FUN > 3) {
      mSaNvsAreaProtocol.Area->Peg3PowerDownUnusedBundles  = mSaConfigHob->PowerDownUnusedBundles[3];
    }
  }
  ///
  /// LTR/OBFF
  ///
  mSaNvsAreaProtocol.Area->Peg0LtrEnable                = PcieDxeConfig->PegPwrOpt[0].LtrEnable;
  mSaNvsAreaProtocol.Area->Peg0ObffEnable               = PcieDxeConfig->PegPwrOpt[0].ObffEnable;
  mSaNvsAreaProtocol.Area->Peg1LtrEnable                = PcieDxeConfig->PegPwrOpt[1].LtrEnable;
  mSaNvsAreaProtocol.Area->Peg1ObffEnable               = PcieDxeConfig->PegPwrOpt[1].ObffEnable;
  mSaNvsAreaProtocol.Area->Peg2LtrEnable                = PcieDxeConfig->PegPwrOpt[2].LtrEnable;
  mSaNvsAreaProtocol.Area->Peg2ObffEnable               = PcieDxeConfig->PegPwrOpt[2].ObffEnable;
  mSaNvsAreaProtocol.Area->PegLtrMaxSnoopLatency        = LTR_MAX_SNOOP_LATENCY_VALUE;
  mSaNvsAreaProtocol.Area->PegLtrMaxNoSnoopLatency      = LTR_MAX_NON_SNOOP_LATENCY_VALUE;

  return EFI_SUCCESS;
}

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] Segment   -   Pci Segment Number
  @param[in] Bus       -   Pci Bus Number
  @param[in] Device    -   Pci Device Number
  @param[in] Function  -   Pci Function Number
  @param[in] CapId     -   CAPID to search for

  @retval 0       - CAPID not found
  @retval Other   - CAPID found, Offset of desired CAPID
**/
UINT32
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  )
{
  UINT64 DeviceBaseAddress;
  UINT8  CapHeader;

  ///
  /// Always start at Offset 0x34
  ///
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
  CapHeader         = PciSegmentRead8 (DeviceBaseAddress + PCI_CAPBILITY_POINTER_OFFSET);
  if (CapHeader == 0xFF) {
    return 0;
  }

  while (CapHeader != 0) {
    ///
    /// Bottom 2 bits of the pointers are reserved per PCI Local Bus Spec 2.2
    ///
    CapHeader &= ~(BIT1 + BIT0);
    ///
    /// Search for desired CapID
    ///
    if (PciSegmentRead8 (DeviceBaseAddress + CapHeader) == CapId) {
      return CapHeader;
    }

    CapHeader = PciSegmentRead8 (DeviceBaseAddress + CapHeader + 1);
  }

  return 0;
}

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Rreporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Segment   -   Pci Segment Number
  @param[in] Bus       -   Pci Bus Number
  @param[in] Device    -   Pci Device Number
  @param[in] Function  -   Pci Function Number
  @param[in] CapId     -   Extended CAPID to search for

  @retval 0       - CAPID not found
  @retval Other   - CAPID found, Offset of desired CAPID
**/
UINT32
PcieFindExtendedCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  )
{
  UINT64  DeviceBaseAddress;
  UINT16  CapHeaderOffset;
  UINT16  CapHeaderId;

  ///
  /// Start to search at Offset 0x100
  /// Get Capability Header
  ///
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
  CapHeaderId     = 0;
  CapHeaderOffset = 0x100;

  while (CapHeaderOffset != 0 && CapHeaderId != 0xFFFF) {
    ///
    /// Search for desired CapID
    ///
    CapHeaderId = PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }

    CapHeaderOffset = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 2) >> 4);
  }

  return 0;
}
