/** @file
  This file provides function to handle client-server differences.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PchSmmHelpers.h"
#include <Register/PmcRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/CpuPcieRegs.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <Library/CpuPcieRpLib.h>
#include <CpuPcieInfo.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>

/**
  Get Root Port physical Number by CPU or PCH Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)
**/
VOID
GetPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  )
{
  GetPchPcieRpNumber (RpDev, RpFun, RpNumber);
}

/**
  Get CPU or PCH Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.
**/
VOID
GetPcieRpDevFun (
  IN  UINTN   RpIndex,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  if (RpIndex >= CpuRpIndex0 && RpIndex <= CpuRpIndex3) {
    GetCpuPcieRpDevFun ((RpIndex - CpuRpIndex0), RpDev, RpFun);
  } else {
    *RpDev = PchPcieRpDevNumber (RpIndex);
    *RpFun = PchPcieRpFuncNumber (RpIndex);
  }
}
