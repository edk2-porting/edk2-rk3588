/** @file
  PCH policy PPI produced by a platform driver specifying various
  expected PCH settings. This PPI is consumed by the PCH PEI modules
  and carried over to PCH DXE modules.

  @copyright
  Copyright 2009 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_POLICY_PPI_H_
#define _PCH_POLICY_PPI_H_

#include <PchAccess.h>
#include <PchPolicyCommon.h>

extern EFI_GUID gPchPlatformPolicyPpiGuid;


typedef struct _PCH_POLICY  PCH_POLICY_PPI;

#endif // PCH_POLICY_PPI_H_
