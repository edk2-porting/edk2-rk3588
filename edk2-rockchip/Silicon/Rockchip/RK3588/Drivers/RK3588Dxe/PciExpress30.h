/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RK3588DXE_PCIE30_H__
#define __RK3588DXE_PCIE30_H__

//
// Don't declare these in the VFR file.
//
#ifndef VFR_FILE_INCLUDE
VOID
EFIAPI
ApplyPcie30Variables (
  VOID
  );

VOID
EFIAPI
SetupPcie30Variables (
  VOID
  );

#endif // VFR_FILE_INCLUDE

#endif // __RK3588DXE_PCIE30_H__
