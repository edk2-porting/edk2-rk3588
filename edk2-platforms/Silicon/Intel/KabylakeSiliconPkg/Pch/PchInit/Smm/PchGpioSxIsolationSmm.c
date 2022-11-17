/** @file
  PCH GPIO Sx Isolation Driver Entry

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchInitSmm.h"
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>

/**
  This function performs GPIO Sx Isolation for DevSlp pins.
**/
VOID
PchGpioSxIsolationCallback (
  VOID
  )
{
  UINT32         SataPortsMax;
  UINT32         SataPort;
  GPIO_PAD       DevSlpGpioPad;

  SataPortsMax = GetPchMaxSataPortNum ();

  //
  // If DevSlp pad is used in its native mode then
  // set PadRstCfg to 10'b (GPIO Reset)
  //
  for (SataPort = 0; SataPort < SataPortsMax; SataPort++) {
    //
    // Check if DevSlp pad is in native mode
    //
    if (GpioIsSataDevSlpPinEnabled (SataPort, &DevSlpGpioPad)) {
      //
      // Program PADCFG_DW0.PadRstCfg
      //
      GpioSetPadResetConfig (DevSlpGpioPad, GpioResetNormal);
    }
  }
}

