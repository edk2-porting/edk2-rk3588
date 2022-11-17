/** @file
  DMI policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DMI_CONFIG_H_
#define _DMI_CONFIG_H_

#define DMI_CONFIG_REVISION 3
extern EFI_GUID gDmiConfigGuid;


#pragma pack (push,1)


/**
  The PCH_DMI_CONFIG block describes the expected configuration of the PCH for DMI.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Deprecate DmiAspm and add DmiAspmCtrl
  <b>Revision 3</b>
  - Added policy to enable/disable Central Write Buffer feature
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    @deprecated since revision 2
  **/

  UINT32     DmiAspm           :  1;
  UINT32     PwrOptEnable      :  1;    ///< <b>0: Disable</b>; 1: Enable DMI Power Optimizer on PCH side.
  UINT32     DmiAspmCtrl       :  8;    ///< ASPM configuration (PCH_PCIE_ASPM_CONTROL) on the PCH side of the DMI/OPI Link. Default is <b>PchPcieAspmAutoConfig</b>
  UINT32     CwbEnable         :  1;    ///< <b>0: Disable</b>; Central Write Buffer feature configurable and disabled by default
  UINT32     Rsvdbits          : 21;    ///< Reserved bits
} PCH_DMI_CONFIG;

#pragma pack (pop)

#endif // _DMI_CONFIG_H_
