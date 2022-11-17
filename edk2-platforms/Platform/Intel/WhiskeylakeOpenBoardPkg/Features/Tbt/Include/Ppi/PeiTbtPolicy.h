/** @file
TBT PEI Policy


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_TBT_POLICY_H_
#define _PEI_TBT_POLICY_H_

#include <TbtPolicyCommonDefinition.h>

#pragma pack(push, 1)

#define PEI_TBT_POLICY_REVISION 1

/**
 TBT PEI configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _PEI_TBT_POLICY {
  DTBT_COMMON_CONFIG     DTbtCommonConfig;                                  ///< dTbt Common Configuration
  DTBT_CONTROLLER_CONFIG DTbtControllerConfig [MAX_DTBT_CONTROLLER_NUMBER]; ///< dTbt Controller Configuration
} PEI_TBT_POLICY;

#pragma pack(pop)

#endif

