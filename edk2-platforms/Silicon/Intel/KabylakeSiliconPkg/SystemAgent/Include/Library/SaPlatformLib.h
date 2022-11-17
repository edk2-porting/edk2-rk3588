/** @file
  Header file for SaPlatformLib.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SA_PLATFORM_LIB_H_
#define _SA_PLATFORM_LIB_H_

#include <SaAccess.h>
#include <CpuAccess.h>

/**
  Determine if PCH Link is DMI/OPI

  @param[in] CpuModel             CPU model

  @retval TRUE                    DMI
  @retval FALSE                   OPI
**/
BOOLEAN
IsPchLinkDmi (
  IN CPU_FAMILY  CpuModel
  );

#endif
