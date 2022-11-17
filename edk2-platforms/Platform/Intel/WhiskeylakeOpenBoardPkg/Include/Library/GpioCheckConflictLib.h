/** @file
  Header file for check Gpio PadMode conflict.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_CHECK_CONFLICT_LIB_H_
#define _GPIO_CHECK_CONFLICT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <GpioConfig.h>
#include <Library/GpioLib.h>

extern EFI_GUID gGpioCheckConflictHobGuid;

typedef struct {
  GPIO_PAD  GpioPad;
  UINT32    GpioPadMode:5;
  UINT32    Reserved:27;
} GPIO_PAD_MODE_INFO;

/**
  Check Gpio PadMode conflict and report it.
**/
VOID
GpioCheckConflict (
  VOID
  );

/**
  This libaray will create one Hob for each Gpio config table
  without PadMode is GpioHardwareDefault

  @param[in]  GpioDefinition    Point to Platform Gpio table
  @param[in]  GpioTableCount    Number of Gpio table entries
**/
VOID
CreateGpioCheckConflictHob (
  IN GPIO_INIT_CONFIG          *GpioDefinition,
  IN UINT16                    GpioTableCount
  );

#endif // _GPIO_CHECK_CONFLICT_LIB_H_

