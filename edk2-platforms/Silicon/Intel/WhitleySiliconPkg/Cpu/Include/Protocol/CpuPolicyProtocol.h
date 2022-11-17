/** @file
  Intel CPU policy protocol should be installed after CPU related setting
  are set by platform driver. CPU driver only could get CPU policy data after this
  protocol installed.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_POLICY_PROTOCOL_H_
#define _CPU_POLICY_PROTOCOL_H_

#include <CpuPolicyPeiDxeCommon.h>

#define CPU_POLICY_PROTOCOL_GUID \
  { \
    0xec7c60b4, 0xa82c, 0x42a5, { 0xbe, 0x76, 0x87, 0xfc, 0xb5, 0x81, 0xa9, 0x1b } \
  }

typedef struct {
  UINT64                PlatformCpuSocketNames;
  UINT64                PlatformCpuAssetTags;
  UINT8                 SbspSelection;
  CPU_POLICY_COMMON     Policy;
} CPU_POLICY_CONFIGURATION;

extern EFI_GUID gEfiCpuPolicyProtocolGuid;

#endif
