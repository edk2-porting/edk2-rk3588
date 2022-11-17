/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/PlatformSysCtrlLib.h>

EFI_STATUS
EFIAPI
EhciVirtualPciIoInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  return RegisterNonDiscoverableMmioDevice (
           NonDiscoverableDeviceTypeEhci,
           NonDiscoverableDeviceDmaTypeCoherent,
           NULL,
           NULL,
           1,
           PlatformGetEhciBase (),
           SIZE_4KB);
}
