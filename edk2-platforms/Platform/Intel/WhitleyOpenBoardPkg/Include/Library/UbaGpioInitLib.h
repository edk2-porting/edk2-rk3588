/** @file
  UBA GPIO Initializtion Library Header File.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_GPIO_INIT_LIB_H
#define _UBA_GPIO_INIT_LIB_H

#include <Base.h>
#include <Uefi.h>

// {9282563E-AE17-4E12-B1DC-070F29F37120}
#define   PLATFORM_GPIO_INIT_DATA_GUID  { 0x9282563e, 0xae17, 0x4e12, { 0xb1, 0xdc, 0x7, 0xf, 0x29, 0xf3, 0x71, 0x20 } }

EFI_STATUS
PlatformInitGpios (
  VOID
);

extern EFI_GUID gPlatformGpioInitDataGuid;

#endif //_UBA_GPIO_INIT_LIB_H
