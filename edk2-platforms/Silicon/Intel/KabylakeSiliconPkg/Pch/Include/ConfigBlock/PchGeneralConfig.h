/** @file
  PCH General policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_GENERAL_CONFIG_H_
#define _PCH_GENERAL_CONFIG_H_

#define PCH_GENERAL_CONFIG_REVISION 1
#define PCH_GENERAL_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gPchGeneralConfigGuid;
extern EFI_GUID gPchGeneralPreMemConfigGuid;

#pragma pack (push,1)

enum PCH_RESERVED_PAGE_ROUTE {
  PchReservedPageToLpc,                   ///< Port 80h cycles are sent to LPC.
  PchReservedPageToPcie                   ///< Port 80h cycles are sent to PCIe.
};

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Subsystem Vendor ID and Subsystem ID of the PCH devices.
    This fields will be ignored if the value of SubSystemVendorId and SubSystemId
    are both 0.
  **/
  UINT16    SubSystemVendorId;          ///< Default Subsystem Vendor ID of the PCH devices. Default is <b>0x8086</b>
  UINT16    SubSystemId;                ///< Default Subsystem ID of the PCH devices. Default is <b>0x7270</b>
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32    Crid            :  1;
  UINT32    RsvdBits0       : 31;       ///< Reserved bits
} PCH_GENERAL_CONFIG;

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT16    AcpiBase;                   ///< Power management I/O base address. Default is <b>0x1800</b>.
  UINT8     RsvdBytes[2];
  /**
    Control where the Port 80h cycles are sent, <b>0: LPC</b>; 1: PCI.
  **/
  UINT32    Port80Route     :  1;
  UINT32    RsvdBits0       : 31;       ///< Reserved bits
} PCH_GENERAL_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _PCH_GENERAL_CONFIG_H_
