/** @file
  Silicon Config HOB is used for gathering platform
  related Intel silicon information and config setting.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SI_CONFIG_HOB_H_
#define _SI_CONFIG_HOB_H_

#include <SiPolicyStruct.h>

extern EFI_GUID gSiConfigHobGuid;

// Rename SI_CONFIG_HOB into SI_CONFIG_HOB_DATA for it does not follow HOB structure.
typedef CONST SI_CONFIG SI_CONFIG_HOB_DATA;
#endif
