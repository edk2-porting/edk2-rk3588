/** @file
  DMI policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_DMI_CONFIG_H_
#define _PCH_DMI_CONFIG_H_

#define PCH_DMI_CONFIG_REVISION 2
extern EFI_GUID gPchDmiConfigGuid;


#pragma pack (push,1)


/**
 The PCH_DMI_CONFIG block describes the expected configuration of the PCH for DMI.
   <b>Revision 1</b>:  - Initial version.
   <b>Revision 2</b>:  - Add OpioRecenter
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header

  UINT32     PwrOptEnable      :  1;    ///< <b>0: Disable</b>; 1: Enable DMI Power Optimizer on PCH side.
  UINT32     DmiAspmCtrl       :  8;    ///< ASPM configuration on the PCH side of the DMI/OPI Link. Default is <b>PchPcieAspmAutoConfig</b>
  UINT32     CwbEnable         :  1;    ///< 0: Disable; <b>1: Enable</b> Central Write Buffer feature configurable and enabled by default
  UINT32     L1RpCtl           :  1;    ///< 0: Disable; <b>1: Enable</b> Allow DMI enter L1 when all root ports are in L1, L0s or link down. Disabled by default.
  /**
   When set to TRUE turns on:
     - L1 State Controller Power Gating
     - L1 State PHY Data Lane Power Gating
     - PHY Common Lane Power Gating
     - Hardware Autonomous Enable
     - PMC Request Enable and Sleep Enable
  **/
  UINT32     DmiPowerReduction :  1;
  UINT32     OpioRecenter      :  1;    ///< 0: Disable; <b>1: Enable</b> Opio Recentering Disable for Pcie latency
  UINT32     Rsvdbits          : 19;    ///< Reserved bits
} PCH_DMI_CONFIG;

#pragma pack (pop)

#endif // _PCH_DMI_CONFIG_H_
