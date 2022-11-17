/** @file
  Implementation of BaseGpioCheckConflicLibNull.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/GpioCheckConflictLib.h>

/**
  Check Gpio PadMode conflict and report it.
**/
VOID
GpioCheckConflict (
  VOID
  )
{
  return;
}

/**
  This libaray will create one Hob for each Gpio config table
  without PadMode is GpioHardwareDefault

  @param[in]  GpioDefinition    Point to Platform Gpio table
  @param[in]  GpioTableCount    Number of Gpio table entries
**/
VOID
CreateGpioCheckConflictHob (
  IN GPIO_INIT_CONFIG          *GpioDefinition,
  IN UINT32                    GpioTableCount
  )
{
  return;
}
