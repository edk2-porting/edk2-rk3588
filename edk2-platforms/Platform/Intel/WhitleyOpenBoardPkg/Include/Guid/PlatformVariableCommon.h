/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_VARIABLE_COMMON_H_
#define _PLATFORM_VARIABLE_COMMON_H_


#ifndef PLATFORM_SETUP_VARIABLE_NAME
#define PLATFORM_SETUP_VARIABLE_NAME             L"Setup"
#endif

#define CMOS_CLEAR_EVENT        BIT0
#define CMOS_BAD_CHECKSUM_EVENT BIT1
#define MFG_MODE_EVENT          BIT2
#define RECOVERY_MODE_EVENT     BIT3
#define LOAD_FAILSAFE_EVENT     BIT4
#define LOAD_CUSTOMIZED_EVENT   BIT5
#define NULL_VARIABLE_EVENT     BIT6

#define PLATFORM_VARIABLE_HOB_GUID { 0x71e6d4bc, 0x4837, 0x45f1, { 0xa2, 0xd7, 0x3f, 0x93, 0x8, 0xb1, 0x7e, 0xd7 } }

extern EFI_GUID gPlatformVariableHobGuid;

#define LOAD_FAILSAFE_VARIABLE_NAME        L"LoadEPSDConfigurationDefaults"
#define LOAD_CUSTOMIZED_VARIABLE_NAME   L"LoadSystemConfigurationDefaults"

#endif

