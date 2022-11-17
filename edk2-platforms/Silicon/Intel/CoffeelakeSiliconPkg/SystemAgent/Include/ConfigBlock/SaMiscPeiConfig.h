/** @file
  Policy details for miscellaneous configuration in System Agent

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_MISC_PEI_CONFIG_H_
#define _SA_MISC_PEI_CONFIG_H_

#pragma pack(push, 1)

#ifndef SA_MC_MAX_SOCKETS
#define SA_MC_MAX_SOCKETS 4
#endif

#define SA_MISC_PEI_CONFIG_REVISION 1

///
/// Subsystem Vendor ID / Subsystem ID
///
typedef struct _SA_DEFAULT_SVID_SID{
  UINT16         SubSystemVendorId;
  UINT16         SubSystemId;
} SA_DEFAULT_SVID_SID;

/**
  This configuration block is to configure SA Miscellaneous variables during PEI Post-Mem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  /**
  Offset 28:0
  This policy is used to control enable or disable System Agent Thermal device (0,4,0).
  The default value is <b>1: TRUE</b> for WHL, and <b>0: FALSE</b> for all other CPU's
  **/
  UINT32  Device4Enable:1;
  /**
  Offset 28:1
  <b>(Test)</b>This policy is used to control enable or disable System Agent Chap device (0,7,0).
  <b>0=FALSE</b>,
  1=TRUE.
  **/
  UINT32  ChapDeviceEnable:1;
  /**
  Offset 28:2
  For Platforms supporting Intel(R) SIPP, this policy is use control enable/disable Compatibility Revision ID (CRID) feature.
  <b>0=FALSE</b>,
  1=TRUE
  **/
  UINT32  CridEnable:1;
  UINT32  SkipPamLock:1;                     ///< Offset 28:3 :To skip PAM register locking. @note It is still recommended to set PCI Config space B0: D0: F0: Offset 80h[0]=1 in platform code even Silicon code skipped this.\n <b>0=All PAM registers will be locked in Silicon code</b>, 1=Skip lock PAM registers in Silicon code.
  UINT32  EdramTestMode:2;                   ///< Offset 28:4 :EDRAM Test Mode. For EDRAM stepping - 0- EDRAM SW Disable, 1- EDRAM SW Enable, <b> 2- EDRAM HW Mode</b>
  UINT32  RsvdBits0          :26;            ///< Offset 28:7 :Reserved for future use
} SA_MISC_PEI_CONFIG;
#pragma pack(pop)

#endif // _SA_MISC_PEI_CONFIG_H_
