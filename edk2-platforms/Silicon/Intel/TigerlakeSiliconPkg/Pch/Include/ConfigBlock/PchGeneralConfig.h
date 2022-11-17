/** @file
  PCH General policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_GENERAL_CONFIG_H_
#define _PCH_GENERAL_CONFIG_H_

#define PCH_GENERAL_CONFIG_REVISION 1
#define PCH_GENERAL_PREMEM_CONFIG_REVISION 2

extern EFI_GUID gPchGeneralConfigGuid;
extern EFI_GUID gPchGeneralPreMemConfigGuid;

#pragma pack (push,1)

enum PCH_RESERVED_PAGE_ROUTE {
  PchReservedPageToLpc,                   ///< Port 80h cycles are sent to LPC.
  PchReservedPageToPcie                   ///< Port 80h cycles are sent to PCIe.
};

/**
  PCH General Configuration
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32    Crid            :  1;
  /**
    Set to enable low latency of legacy IO.
    Some systems require lower IO latency irrespective of power.
    This is a tradeoff between power and IO latency.
    @note: Once this is enabled, DmiAspm, Pcie DmiAspm in SystemAgent
    and ITSS Clock Gating are forced to disabled.
    <b>0: Disable</b>, 1: Enable
  **/
  UINT32    LegacyIoLowLatency  :  1;
  UINT32    RsvdBits0           : 30;       ///< Reserved bits
} PCH_GENERAL_CONFIG;

/**
  PCH General Pre-Memory Configuration
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added GpioOverride.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Control where the Port 80h cycles are sent, <b>0: LPC</b>; 1: PCI.
  **/
  UINT32    Port80Route     :  1;
  UINT32    IotgPllSscEn    :  1;       ///< Need to disable CPU Side SSC for A0 PO
  /**
    Gpio override Level
    -- <b>0: Disable</b>;
    -  1: Override Level 1 - only skips GpioSetNativePadByFunction
    -  2: Override Level 2 - skips GpioSetNativePadByFunction and GpioSetPadMode
    Additional policy that allows GPIO configuration to be done by external means.
    If equal to 1  PCH will skip every Pad configuration.
  **/
  UINT32    GpioOverride    :  3;
  UINT32    RsvdBits0       : 27;       ///< Reserved bits
} PCH_GENERAL_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _PCH_GENERAL_CONFIG_H_
