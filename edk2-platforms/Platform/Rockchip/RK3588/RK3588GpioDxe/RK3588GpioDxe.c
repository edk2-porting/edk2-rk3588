/** @file
*
*  Copyright (c) 2018, Linaro. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EmbeddedGpio.h>

GPIO_CONTROLLER gGpioDevice[] = {
  //
  // { base address, gpio index, gpio count }
  //
  { 0xFDD60000, 0, 32 },    // GPIO0
  { 0xFE740000, 32, 32 },    // GPIO1
  { 0xFE750000, 64, 32 },   // GPIO2
  { 0xFE760000, 96, 32 },   // GPIO3
  { 0xFE770000, 128, 32 },   // GPIO3
};

PLATFORM_GPIO_CONTROLLER gPlatformGpioDevice = {
  //
  // { global gpio count, gpio controller counter, GPIO_CONTROLLER }
  //
  160, 20, gGpioDevice
};

EFI_STATUS
EFIAPI
RK3588GpioEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  // Install the Embedded Platform GPIO Protocol onto a new handle
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces(
                  &Handle,
                  &gPlatformGpioProtocolGuid, &gPlatformGpioDevice,
                  NULL
                 );
  if (EFI_ERROR(Status)) {
    Status = EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
