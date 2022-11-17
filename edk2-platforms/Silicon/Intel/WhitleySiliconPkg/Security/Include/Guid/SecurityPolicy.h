/** @file
  Provides data structure information used by ServerSecurity features in Mtkme etc.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SECURITY_POLICY_H_
#define _SECURITY_POLICY_H_

extern EFI_GUID gSecurityPolicyDataGuid;
#include <Library/SecurityPolicyDefinitions.h>

#pragma pack(1)

/**
  Security Policy
**/
typedef struct {
  /**
   * Please put common definitions inside the SecurityPolicy_Flat.h *
  **/
  #include "SecurityPolicy_Flat.h"
} SECURITY_POLICY;

#pragma pack()
#endif
