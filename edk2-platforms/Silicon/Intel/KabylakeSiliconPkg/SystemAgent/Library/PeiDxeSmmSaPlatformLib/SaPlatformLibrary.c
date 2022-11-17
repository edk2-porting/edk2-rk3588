/** @file
  SA Platform Lib implementation.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "SaPlatformLibrary.h"
#include <Library/MmPciLib.h>
#include <SaRegs.h>

/**
  Determine if PCH Link is DMI/OPI

  @param[in] CpuModel             CPU model

  @retval TRUE                    DMI
  @retval FALSE                   OPI
**/
BOOLEAN
IsPchLinkDmi (
  IN CPU_FAMILY  CpuModel
  )
{
  if (((CpuModel == EnumCpuSklDtHalo) || (CpuModel == EnumCpuKblDtHalo)) && ((MmioRead8 (MmPciBase (0, 0, 0) + R_SA_MC_DEVICE_ID) & (BIT2 | BIT1)) != BIT2)) {
    return TRUE; // DMI
  }
  return FALSE;  // OPI
}
