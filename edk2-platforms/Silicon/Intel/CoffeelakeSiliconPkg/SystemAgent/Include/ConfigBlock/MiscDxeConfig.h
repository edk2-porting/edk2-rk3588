/** @file
  MISC DXE policy definitions

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MISC_DXE_CONFIG_H_
#define _MISC_DXE_CONFIG_H_

#pragma pack(push, 1)

#define MISC_DXE_CONFIG_REVISION 2

/**
  This data structure includes miscellaneous configuration variables such SA thermal device
  control. The data elements should be initialized by a Platform Module.\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2:</b>
  - Added RmrrCsmeBaseAddress fields.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                    ///< Offset 0-27 Config Block Header
  EFI_PHYSICAL_ADDRESS  *RmrrUsbBaseAddress;       ///< Offset 28 The field is used to describe the platform USB Reserved memory for Intel VT-d support. Platform code should provide this information for Intel VT-d DXE driver use
  UINT32                EnableAbove4GBMmio : 1;    ///< Offset 29:0 Enable/disable above 4GB MMIO resource support: <b>0=Disable</b>, 1=Enable
  UINT32                RsvdBits0          : 31;   ///< Offset 29:1 Reserved bits.
  EFI_PHYSICAL_ADDRESS  *RmrrCsmeBaseAddress;      ///< The field is used to describe the CSME Reserved memory.
} MISC_DXE_CONFIG;
#pragma pack(pop)

#endif // _MISC_DXE_CONFIG_H_
