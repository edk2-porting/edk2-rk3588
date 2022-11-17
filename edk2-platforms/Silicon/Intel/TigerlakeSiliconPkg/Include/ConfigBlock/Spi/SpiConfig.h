/** @file
  PCH SPI Flash Controller config block

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SPI_CONFIG_H_
#define _SPI_CONFIG_H_

#define SPI_CONFIG_REVISION 1
extern EFI_GUID gSpiConfigGuid;

#pragma pack (push,1)

/**
  Basic configuration for option features of PCH SPI Flash controller
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    Enable extended BIOS Direct Read Region feature
    Enabling this will make all memory accesses in a decode range to be translated
    to BIOS region reads from SPI flash
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32    ExtendedBiosDecodeRangeEnable   :  1;
  UINT32    RsvdBits0                       : 31;       ///< Reserved bits
  /**
    Base address that will be used for Extended Decode Range.
    This will be ignored when ExtendedBiosDecodeRangeEnable is set to 0.
  **/
  UINT32    ExtendedBiosDecodeRangeBase;
  /**
    Limit address that will be used for Extended Decode Range.
    This will be ignored when ExtendedBiosDecodeRangeEnable is set to 0.
  **/
  UINT32    ExtendedBiosDecodeRangeLimit;
} SPI_CONFIG;

#pragma pack (pop)

#endif // _SPI_CONFIG_H_
