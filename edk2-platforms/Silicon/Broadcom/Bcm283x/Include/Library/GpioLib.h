/** @file
 *
 *  GPIO manipulation.
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __GPIO_LIB__
#define __GPIO_LIB__

#include <IndustryStandard/Bcm2836Gpio.h>

#define CLEAR_GPIO    0
#define SET_GPIO      1

VOID
GpioPinFuncSet (
  IN  UINTN Pin,
  IN  UINTN Function
  );

UINTN
GpioPinFuncGet (
  IN  UINTN Pin
  );

VOID
GpioPinConfigure (
  IN  UINTN   Pin,
  IN  BOOLEAN Config
  );

UINTN
GpioPinGet (
  IN  UINTN Pin
  );

VOID
GpioSetPull (
  IN  UINTN   Pin,
  IN  UINTN   Pud
);

#endif /* __GPIO_LIB__ */
