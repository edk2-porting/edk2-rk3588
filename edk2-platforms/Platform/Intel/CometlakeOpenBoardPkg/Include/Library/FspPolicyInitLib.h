/** @file
  Function prototype of FspPolicyInitLib.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _FSP_POLICY_INIT_LIB_H_
#define _FSP_POLICY_INIT_LIB_H_

#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>

VOID
EFIAPI
FspPolicyInitPreMem (
  IN FSPM_UPD           *FspmUpdDataPtr
  );

VOID
EFIAPI
FspPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  );

#endif // _FSP_POLICY_INIT_LIB_H_

