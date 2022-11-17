/** @file
  Gigabit Ethernet policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GBE_CONFIG_H_
#define _GBE_CONFIG_H_

#define GBE_CONFIG_REVISION 1
extern EFI_GUID gGbeConfigGuid;

#pragma pack (push,1)

/**
  PCH intergrated GBE controller configuration settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal GBE, 0: Disable; <b>1: Enable</b>.
    When Enable is changed (from disabled to enabled or from enabled to disabled),
    it needs to set LAN Disable regsiter, which might be locked by FDSWL register.
    So it's recommendated to issue a global reset when changing the status for PCH Internal LAN.
  **/
  UINT32  Enable          :  1;
  UINT32  LtrEnable       :  1;  ///< <b>0: Disable</b>; 1: Enable LTR capabilty of PCH internal LAN.
  UINT32  RsvdBits0       : 30;  ///< Reserved bits
} GBE_CONFIG;

#pragma pack (pop)

#endif // _GBE_CONFIG_H_
