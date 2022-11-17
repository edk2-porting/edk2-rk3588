/** @file
  Lan policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _LAN_CONFIG_H_
#define _LAN_CONFIG_H_

#define LAN_CONFIG_REVISION 1
extern EFI_GUID gLanConfigGuid;

#pragma pack (push,1)

/**
  PCH intergrated LAN controller configuration settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal LAN, 0: Disable; <b>1: Enable</b>.
    When Enable is changed (from disabled to enabled or from enabled to disabled),
    it needs to set LAN Disable register, which might be locked by FDSWL register.
    So it's recommendated to issue a global reset when changing the status for PCH Internal LAN.
  **/
  UINT32  Enable          :  1;
  UINT32  LtrEnable       :  1;  ///< <b>0: Disable</b>; 1: Enable LTR capabilty of PCH internal LAN.
  UINT32  RsvdBits0       : 30;  ///< Reserved bits
} PCH_LAN_CONFIG;

#pragma pack (pop)

#endif // _LAN_CONFIG_H_
