/** @file
 *
 *  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RK3588DXE_FANCONTROL_H__
#define __RK3588DXE_FANCONTROL_H__

#define FAN_PERCENTAGE_MIN      0
#define FAN_PERCENTAGE_MAX      100
#define FAN_PERCENTAGE_STEP     1
#define FAN_PERCENTAGE_DEFAULT  50

//
// Don't declare these in the VFR file.
//
#ifndef VFR_FILE_INCLUDE
VOID
EFIAPI
ApplyCoolingFanVariables (
  VOID
  );

VOID
EFIAPI
SetupCoolingFanVariables (
  VOID
  );

#endif // VFR_FILE_INCLUDE

#endif // __RK3588DXE_FANCONTROL_H__
