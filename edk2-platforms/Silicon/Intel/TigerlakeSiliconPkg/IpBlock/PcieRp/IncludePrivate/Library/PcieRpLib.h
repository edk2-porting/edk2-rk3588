/** @file
  Header file for PCH PCI Express helpers library

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCIE_RP_LIB_
#define _PCIE_RP_LIB_

#include <PchPolicyCommon.h>

typedef struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxSnoopLatencyRequirement   : 1;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 MaxNoSnoopLatencyRequirement : 1;
  UINT32 ForceOverride                : 1;
} LTR_OVERRIDE;

/**
  Get PCIe port number for enabled Root Port.

  @param[in] RpBase    Root Port pci segment base address

  @retval Root Port number (1 based)
**/
UINT32
PciePortNum (
  IN     UINT64  RpBase
  );

/**
  Get PCIe root port index
  @param[in] RpBase    Root Port pci segment base address
  @return Root Port index (0 based)
**/
UINT32
PciePortIndex (
  IN     UINT64  RpBase
  );

/**
  This function checks whether PHY lane power gating is enabled on the port.

  @param[in] RpBase                 Root Port base address

  @retval TRUE                      PHY power gating is enabled
  @retval FALSE                     PHY power gating disabled
**/
BOOLEAN
PcieIsPhyLanePgEnabled (
  IN     UINT64  RpBase
  );

/**
  Configures Root Port packet split.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Mps                            maximum packet size
**/
VOID
ConfigureRpPacketSplit (
  UINT64 RpBase,
  UINT8  Mps
  );

/**
  Configures LTR override in Root Port's proprietary registers.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] LtrConfig                      Root Port LTR configuration
  @param[in] AspmOverride                   combination of LTR override values from all devices under this Root Port
**/
VOID
ConfigureRpLtrOverride (
  UINT64           RpBase,
  UINT32           DevNum,
  LTR_OVERRIDE     *TreeLtr,
  PCIE_LTR_CONFIG  *LtrConfig
  );

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
  );

/**
  Configures proprietary parts of L1 substates configuration in Root Port

  @param[in] RpSbdf       segment:bus:device:function coordinates of Root Port
  @param[in] LtrCapable   TRUE if Root Port is LTR capable
**/
VOID
L1ssProprietaryConfiguration (
  UINT64  RpBase,
  BOOLEAN LtrCapable
  );
#endif
