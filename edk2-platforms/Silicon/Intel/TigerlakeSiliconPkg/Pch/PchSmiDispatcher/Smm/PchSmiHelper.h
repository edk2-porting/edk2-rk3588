/** @file
  PCH SMI Helper Header

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SMI_HELPER_H_
#define _PCH_SMI_HELPER_H_
#include <Uefi/UefiBaseType.h>

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
  );

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
  );

#endif
