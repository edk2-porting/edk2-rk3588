/** @file
  VBIOS DXE policy definitions

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _VBIOS_DXE_CONFIG_H_
#define _VBIOS_DXE_CONFIG_H_

#pragma pack(push, 1)

#define VBIOS_DXE_CONFIG_REVISION 1

/**
  This data structure includes Switchable Graphics VBIOS configuration.
  If Switchable Graphics/Hybrid Gfaphics feature is not supported, all the policies in this configuration block can be ignored.
  The data elements should be initialized by a Platform Module.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;              ///< Offset 0-27 Config Block Header
  UINT8                 LoadVbios    : 1;    ///< Offset 28:0 :This field is used to describe if the dGPU VBIOS needs to be loaded: <b>0=Not load</b>, 1=Load
  UINT8                 ExecuteVbios : 1;    ///< Offset 28:1 :This field is used to describe if the dGPU VBIOS need to be executed: <b>0=Not execute</b>, 1=Execute
/**
  Offset 28:2 :
  This field is used to identify the source location of dGPU VBIOS\n
  <b>1 = secondary display device VBIOS Source is PCI Card</b>\n
  0 = secondary display device VBIOS Source is FW Volume\n
**/
  UINT8                 VbiosSource  : 1;
  UINT8                 RsvdBits0    : 5;    ///< Offset 28:3 Reserved for future use
  UINT8                 Rsvd[3];             ///< Offset 29 : Reserved for DWORD alignment
} VBIOS_DXE_CONFIG;
#pragma pack(pop)

#endif // _VBIOS_DXE_CONFIG_H_
