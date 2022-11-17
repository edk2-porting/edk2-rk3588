/** @file
*
*  Copyright 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef NXP_PLATFORM_PPI_H__
#define NXP_PLATFORM_PPI_H__

#include <Uefi.h>

typedef enum _NXP_IP_CLOCK {
  NXP_CORE_CLOCK,
  NXP_I2C_CLOCK,
  NXP_SYSTEM_CLOCK,
  NXP_UART_CLOCK
} NXP_IP_CLOCK;

/**
  Get the clocks supplied by Platform(Board) to NXP Layerscape SOC IPs

  @param[in]  ClockType  Variable of Type NXP_IP_CLOCK. Indicates which IP clock
                         is to be retrieved.
  @param[in]  ...        Variable argument list which is parsed based on
                         ClockType. e.g. if the ClockType is NXP_I2C_CLOCK, then
                         the second argument will be interpreted as controller
                         number. e.g. if there are four i2c controllers in SOC,
                         then this value can be 0, 1, 2, 3
                         e.g. if ClockType is NXP_CORE_CLOCK, then second
                         argument is interpreted as cluster number and third
                         argument is interpreted as core number (within the
                         cluster)

  @return                Actual Clock Frequency. Return value 0 should be
                         interpreted as clock not being provided to IP.
**/
typedef
UINT64
(EFIAPI * NXP_PLATFORM_GET_CLOCK)(
  IN NXP_IP_CLOCK ClockType,
  ...
  );

typedef struct {
  NXP_PLATFORM_GET_CLOCK  PlatformGetClock;
} NXP_PLATFORM_GET_CLOCK_PPI;

extern NXP_PLATFORM_GET_CLOCK_PPI gPlatformGetClockPpi;

#endif // NXP_PLATFORM_PPI_H__
