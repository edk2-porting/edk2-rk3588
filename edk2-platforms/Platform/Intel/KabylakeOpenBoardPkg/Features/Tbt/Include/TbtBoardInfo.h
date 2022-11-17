/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _TBT_INFO_GUID_H_
#define _TBT_INFO_GUID_H_
#include <TbtPolicyCommonDefinition.h>

#pragma pack(1)
//
// TBT Info HOB
//
typedef struct _TBT_INFO_HOB {
  EFI_HOB_GUID_TYPE      EfiHobGuidType;
  DTBT_COMMON_CONFIG     DTbtCommonConfig;                                  ///< dTbt Common Configuration
  DTBT_CONTROLLER_CONFIG DTbtControllerConfig;                             ///< dTbt Controller Configuration
} TBT_INFO_HOB;
#pragma pack()

#endif
