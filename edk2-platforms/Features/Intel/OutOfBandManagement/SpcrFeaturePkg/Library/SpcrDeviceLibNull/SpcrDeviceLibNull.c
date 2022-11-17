/** @file
  Implementation for a NULL library of SpcrDeviceLib.
  SpcrDeviceLib is usd for Serial Port Console Redirection Table (SPCR) device.

  Copyright (c) 2017 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Include/Uefi.h>
#include <Protocol/DevicePath.h>

/**
  Get a Serial Port device used for SPCR.

  @retval NULL          Can not get device.

**/
EFI_DEVICE_PATH_PROTOCOL*
EFIAPI
GetSpcrDevice (
  VOID
  )
{
  return NULL;
}
