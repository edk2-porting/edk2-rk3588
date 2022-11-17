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
  { 0xf8011000, 0, 8 },    // GPIO0
  { 0xf8012000, 8, 8 },    // GPIO1
  { 0xf8013000, 16, 8 },   // GPIO2
  { 0xf8014000, 24, 8 },   // GPIO3
  { 0xf7020000, 32, 8 },   // GPIO4
  { 0xf7021000, 40, 8 },   // GPIO5
  { 0xf7022000, 48, 8 },   // GPIO6
  { 0xf7023000, 56, 8 },   // GPIO7
  { 0xf7024000, 64, 8 },   // GPIO8
  { 0xf7025000, 72, 8 },   // GPIO9
  { 0xf7026000, 80, 8 },   // GPIO10
  { 0xf7027000, 88, 8 },   // GPIO11
  { 0xf7028000, 96, 8 },   // GPIO12
  { 0xf7029000, 104, 8 },  // GPIO13
  { 0xf702a000, 112, 8 },  // GPIO14
  { 0xf702b000, 120, 8 },  // GPIO15
  { 0xf702c000, 128, 8 },  // GPIO16
  { 0xf702d000, 136, 8 },  // GPIO17
  { 0xf702e000, 144, 8 },  // GPIO18
  { 0xf702f000, 152, 8 }   // GPIO19
};

PLATFORM_GPIO_CONTROLLER gPlatformGpioDevice = {
  //
  // { global gpio count, gpio controller counter, GPIO_CONTROLLER }
  //
  160, 20, gGpioDevice
};

EFI_STATUS
EFIAPI
HiKeyGpioEntryPoint (
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
