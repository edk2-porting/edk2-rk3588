/** @file
 *
 *  GPIO manipulation.
 *
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>
#include <Library/TimerLib.h>
#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/Bcm2836Gpio.h>

STATIC
VOID
GpioFSELModify (
  IN  UINTN RegIndex,
  IN  UINT32 ModifyMask,
  IN  UINT32 FunctionMask
  )
{
  UINT32 Val;
  EFI_PHYSICAL_ADDRESS Reg;

  Reg = RegIndex * sizeof (UINT32) + GPIO_GPFSEL0;

  ASSERT (Reg <= GPIO_GPFSEL5);
  ASSERT ((~ModifyMask & FunctionMask) == 0);

  Val = MmioRead32 (Reg);
  Val &= ~ModifyMask;
  Val |= FunctionMask;
  MmioWrite32 (Reg, Val);
}

VOID
GpioPinFuncSet (
  IN  UINTN Pin,
  IN  UINTN Function
  )
{
  UINTN RegIndex;
  UINTN SelIndex;
  UINT32 ModifyMask;
  UINT32 FunctionMask;

  ASSERT (Pin < GPIO_PINS);
  ASSERT (Function <= GPIO_FSEL_MASK);

  RegIndex = Pin / 10;
  SelIndex = Pin % 10;

  ModifyMask = GPIO_FSEL_MASK << (SelIndex * GPIO_FSEL_BITS_PER_PIN);
  FunctionMask = Function << (SelIndex * GPIO_FSEL_BITS_PER_PIN);
  GpioFSELModify (RegIndex, ModifyMask, FunctionMask);
}

UINTN
GpioPinFuncGet (
  IN  UINTN Pin
  )
{
  UINT32 Val;
  UINTN RegIndex;
  UINTN SelIndex;
  EFI_PHYSICAL_ADDRESS Reg;

  ASSERT (Pin < GPIO_PINS);

  RegIndex = Pin / 10;
  SelIndex = Pin % 10;
  Reg = RegIndex * sizeof (UINT32) + GPIO_GPFSEL0;

  Val = MmioRead32 (Reg);
  Val >>= SelIndex * GPIO_FSEL_BITS_PER_PIN;
  Val &= GPIO_FSEL_MASK;
  return Val;
}

VOID
GpioPinConfigure (
  IN  UINTN   Pin,
  IN  BOOLEAN Config
  )
{
  EFI_PHYSICAL_ADDRESS Reg;
  UINTN RegIndex;
  UINTN SelIndex;

  ASSERT (Pin < GPIO_PINS);

  RegIndex = Pin / 32;
  SelIndex = Pin % 32;

  //
  // Different base addresses are used for clear and set
  //
  Reg = (Config == CLEAR_GPIO) ? GPIO_GPCLR0 : GPIO_GPSET0;
  Reg += RegIndex * sizeof (UINT32);
  MmioWrite32 (Reg, 1 << SelIndex);
}

UINTN
GpioPinGet (
  IN  UINTN Pin
  )
{
  EFI_PHYSICAL_ADDRESS Reg;
  UINTN RegIndex;
  UINTN SelIndex;
  UINT32 Val;

  ASSERT (Pin < GPIO_PINS);

  RegIndex = Pin / 32;
  SelIndex = Pin % 32;

  Reg = RegIndex * sizeof (UINT32) + GPIO_GPLEV0;
  Val = MmioRead32 (Reg) >> SelIndex;
  return Val & 1;
}

VOID
GpioSetPull (
  IN  UINTN Pin,
  IN  UINTN Pud
)
{
  EFI_PHYSICAL_ADDRESS Reg;
  UINTN RegIndex;
  UINTN SelIndex;
  UINT32 Val;

  ASSERT (Pin < GPIO_PINS);
  ASSERT (Pud <= GPIO_PULL_UP);

  //
  // Check if we should use the legacy or newer method of
  // enabling pullup/pulldown by querying GPPUPPDN3, which
  // is unused on Bcm2835/2836/2837 and returns 'gpio'.
  //
  if (MmioRead32 (GPIO_GPPUPPDN3) == 0x6770696f) {
    RegIndex = Pin / 32;
    SelIndex = Pin % 32;

    MmioWrite32 (GPIO_GPPUD, Pud);
    MicroSecondDelay (5);

    Reg = RegIndex * sizeof (UINT32) + GPIO_GPPUDCLK0;
    MmioWrite32 (Reg, 1 << SelIndex);
    MicroSecondDelay (5);

    MmioWrite32 (GPIO_GPPUD, 0);
    MmioWrite32 (Reg, 0);
  } else {
    //
    // The newer method uses inverted values for up/down.
    //
    if (Pud != 0)
      Pud ^= 0x3;
    RegIndex = Pin / 16;
    SelIndex = (Pin % 16) * 2;
    Reg = RegIndex * sizeof (UINT32) + GPIO_GPPUPPDN0;
    Val = MmioRead32 (Reg);
    Val &= ~(0x3 << SelIndex);
    Val |= Pud << SelIndex;
    MmioWrite32 (Reg, Val);
  }
}
