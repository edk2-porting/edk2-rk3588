/** @file
 *
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RK3588DXE_DISPLAY_H__
#define __RK3588DXE_DISPLAY_H__

#include <RockchipDisplay.h>

//
// Don't declare these in the VFR file.
//
#ifndef VFR_FILE_INCLUDE
VOID
EFIAPI
ApplyDisplayVariables (
  VOID
  );

VOID
EFIAPI
SetupDisplayVariables (
  VOID
  );

#endif // VFR_FILE_INCLUDE

#endif // __RK3588DXE_DISPLAY_H__
