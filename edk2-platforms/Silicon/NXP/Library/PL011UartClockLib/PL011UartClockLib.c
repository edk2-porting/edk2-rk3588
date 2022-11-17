/** @file
*
*  Copyright 2018, 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Base.h>
#include <Ppi/NxpPlatformGetClock.h>

/**
  Return clock in for PL011 Uart IP
**/
UINT32
EFIAPI
PL011UartClockGetFreq (
  VOID
  )
{
  return gPlatformGetClockPpi.PlatformGetClock (NXP_UART_CLOCK, 0);
}
