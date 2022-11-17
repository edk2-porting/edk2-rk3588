/** @file
  This file contains various definitions for IBV HSTI implementation
  including error string definitions

Copyright (c) 2017 -2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __HSTI_IBV_FEATURE_BIT_H__
#define __HSTI_IBV_FEATURE_BIT_H__

#define HSTI_IMPLEMENTATION_ID_PLATFORM  L"Minimum Platform"

#define HSTI_SECURITY_FEATURE_SIZE        3

#define HSTI_ERROR                                                                    L"Error "
#define HSTI_PLATFORM_SECURITY_SPECIFICATION                                          L" Platform Security Specification"

#define HSTI_FIRMWARE_TRUSTED_CONTINUATION_CRYPTO_STRENGTH                            L" - Firmware Trust Continuation Crypto Strength - "
#define HSTI_NO_TEST_KEY_VERIFICATION                                                 L" - No Test Key Verification - "
#define HSTI_FIRMWARE_VERSION_ROLLBACK_PROTECTION                                     L" - Firmware Version Rollback Protection - "
#define HSTI_SECUREBOOT_BYPASS_CHECKING                                               L" - SecureBoot Bypass Checking - "
#define HSTI_EXTERNAL_DEVICE_DMA_PROTECTION                                           L" - External Device DMA Protection - "
#define HSTI_MOR_SUPPORT                                                              L" - MOR Support - "

#define HSTI_BYTE2_FIRMWARE_TRUSTED_CONTINUATION_CRYPTO_STRENGTH                      BIT0
#define      HSTI_BYTE2_FIRMWARE_TRUSTED_CONTINUATION_CRYPTO_STRENGTH_ERROR_CODE_1    L"0x00100001"
#define      HSTI_BYTE2_FIRMWARE_TRUSTED_CONTINUATION_CRYPTO_STRENGTH_ERROR_STRING_1  L"SHA1\r\n"

#define HSTI_BYTE2_NO_TEST_KEY_VERIFICATION                                           BIT1
#define      HSTI_BYTE2_NO_TEST_KEY_VERIFICATION_ERROR_CODE_1                         L"0x00110001"
#define      HSTI_BYTE2_NO_TEST_KEY_VERIFICATION_ERROR_STRING_1                       L"fail\r\n"

#define HSTI_BYTE2_FIRMWARE_VERSION_ROLLBACK_PROTECTION                               BIT2
#define      HSTI_BYTE2_FIRMWARE_VERSION_ROLLBACK_PROTECTION_ERROR_CODE_1             L"0x00120001"
#define      HSTI_BYTE2_FIRMWARE_VERSION_ROLLBACK_PROTECTION_ERROR_STRING_1           L"fail\r\n"

#define HSTI_BYTE2_SECUREBOOT_BYPASS_CHECKING                                         BIT3
#define      HSTI_BYTE2_SECUREBOOT_BYPASS_CHECKING_ERROR_CODE_1                       L"0x00130001"
#define      HSTI_BYTE2_SECUREBOOT_BYPASS_CHECKING_ERROR_STRING_1                     L"fail\r\n"


#define HSTI_BYTE2_EXTERNAL_DEVICE_DMA_PROTECTION                                     BIT4
#define      HSTI_BYTE2_EXTERNAL_DEVICE_DMA_PROTECTION_ERROR_CODE_1                   L"0x00140001"
#define      HSTI_BYTE2_EXTERNAL_DEVICE_DMA_PROTECTION_ERROR_STRING_1                 L"unsupported\r\n"


#define HSTI_BYTE2_MOR_SUPPORT                                                        BIT5
#define      HSTI_BYTE2_MOR_SUPPORT_ERROR_CODE_1                                      L"0x00150001"
#define      HSTI_BYTE2_MOR_SUPPORT_ERROR_STRING_1                                    L"unsupported\r\n"


#endif
