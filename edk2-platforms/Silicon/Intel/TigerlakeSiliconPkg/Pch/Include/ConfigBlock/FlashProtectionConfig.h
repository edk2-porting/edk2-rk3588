/** @file
  FlashProtection policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _FLASH_PROTECTION_CONFIG_H_
#define _FLASH_PROTECTION_CONFIG_H_

#define FLASH_PROTECTION_CONFIG_REVISION 1
extern EFI_GUID gFlashProtectionConfigGuid;

#pragma pack (push,1)

//
// Flash Protection Range Register
//
#define PCH_FLASH_PROTECTED_RANGES         5

/**
  Protected Flash Range
**/
typedef struct {
  UINT32                WriteProtectionEnable     :  1;     ///< Write or erase is blocked by hardware. <b>0: Disable</b>; 1: Enable.
  UINT32                ReadProtectionEnable      :  1;     ///< Read is blocked by hardware. <b>0: Disable</b>; 1: Enable.
  UINT32                RsvdBits                  :  30;    ///< Reserved
  /**
    The address of the upper limit of protection
    This is a left shifted address by 12 bits with address bits 11:0 are assumed to be FFFh for limit comparison
  **/
  UINT16                ProtectedRangeLimit;
  /**
    The address of the upper limit of protection
    This is a left shifted address by 12 bits with address bits 11:0 are assumed to be 0
  **/
  UINT16                ProtectedRangeBase;
} PROTECTED_RANGE;

/**
  The PCH provides a method for blocking writes and reads to specific ranges
  in the SPI flash when the Protected Ranges are enabled.
  PROTECTED_RANGE is used to specify if flash protection are enabled,
  the write protection enable bit and the read protection enable bit,
  and to specify the upper limit and lower base for each register
  Platform code is responsible to get the range base by PchGetSpiRegionAddresses routine,
  and set the limit and base accordingly.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                     ///< Config Block Header
  PROTECTED_RANGE       ProtectRange[PCH_FLASH_PROTECTED_RANGES];   ///< Protected Flash Ranges
} PCH_FLASH_PROTECTION_CONFIG;

#pragma pack (pop)

#endif // _FLASH_PROTECTION_CONFIG_H_
