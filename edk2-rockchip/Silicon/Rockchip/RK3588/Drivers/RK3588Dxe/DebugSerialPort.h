/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RK3588DXE_DEBUG_SERIAL_PORT_H__
#define __RK3588DXE_DEBUG_SERIAL_PORT_H__

#define DEBUG_SERIAL_PORT_BAUD_RATE_MIN      0
#define DEBUG_SERIAL_PORT_BAUD_RATE_MAX      1500000
#define DEBUG_SERIAL_PORT_BAUD_RATE_DEFAULT  1500000

//
// Don't declare these in the VFR file.
//
#ifndef VFR_FILE_INCLUDE
VOID
EFIAPI
ApplyDebugSerialPortVariables (
  VOID
  );

VOID
EFIAPI
SetupDebugSerialPortVariables (
  VOID
  );

#endif // VFR_FILE_INCLUDE

#endif // __RK3588DXE_DEBUG_SERIAL_PORT_H__
