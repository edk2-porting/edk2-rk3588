/** @file
  Header file for a library about SPCR device.
  SpcrDeviceLib is usd for Serial Port Console Redirection Table (SPCR) device.

  Copyright (c) 2011 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SPCR_DEVICE_LIB_H_
#define _SPCR_DEVICE_LIB_H_

/**
  Get a Serial Port device used for SPCR.
  The caller should call FreePool() to free the memory if return value is not NULL.

  @retval NULL          Can not get device.
  @retval Others        A serial port device path.

**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
GetSpcrDevice (
  VOID
  );

#endif // _SPCR_DEVICE_LIB_H_
