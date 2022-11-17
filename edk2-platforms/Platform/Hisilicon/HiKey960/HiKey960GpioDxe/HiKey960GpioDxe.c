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
  { 0xe8a0b000, 0, 8 },    // GPIO0
  { 0xe8a0c000, 8, 8 },    // GPIO1
  { 0xe8a0d000, 16, 8 },   // GPIO2
  { 0xe8a0e000, 24, 8 },   // GPIO3
  { 0xe8a0f000, 32, 8 },   // GPIO4
  { 0xe8a10000, 40, 8 },   // GPIO5
  { 0xe8a11000, 48, 8 },   // GPIO6
  { 0xe8a12000, 56, 8 },   // GPIO7
  { 0xe8a13000, 64, 8 },   // GPIO8
  { 0xe8a14000, 72, 8 },   // GPIO9
  { 0xe8a15000, 80, 8 },   // GPIO10
  { 0xe8a16000, 88, 8 },   // GPIO11
  { 0xe8a17000, 96, 8 },   // GPIO12
  { 0xe8a18000, 104, 8 },  // GPIO13
  { 0xe8a19000, 112, 8 },  // GPIO14
  { 0xe8a1a000, 120, 8 },  // GPIO15
  { 0xe8a1b000, 128, 8 },  // GPIO16
  { 0xe8a1c000, 136, 8 },  // GPIO17
  { 0xff3b4000, 144, 8 },  // GPIO18
  { 0xff3b5000, 152, 8 },  // GPIO19
  { 0xe8a1f000, 160, 8 },  // GPIO20
  { 0xe8a20000, 168, 8 },  // GPIO21
  { 0xfff0b000, 176, 8 },  // GPIO22
  { 0xfff0c000, 184, 8 },  // GPIO23
  { 0xfff0d000, 192, 8 },  // GPIO24
  { 0xfff0e000, 200, 8 },  // GPIO25
  { 0xfff0f000, 208, 8 },  // GPIO26
  { 0xfff10000, 216, 8 },  // GPIO27
  { 0xfff1d000, 224, 8 },  // GPIO28
};

PLATFORM_GPIO_CONTROLLER gPlatformGpioDevice = {
  //
  // { global gpio count, gpio controller count, GPIO_CONTROLLER }
  //
  232, 29, gGpioDevice
};

EFI_STATUS
EFIAPI
HiKey960GpioEntryPoint (
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
