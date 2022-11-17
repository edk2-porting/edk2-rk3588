/**
*
*  Copyright (c) 2017, Linaro Ltd. All rights reserved.
*  Copyright (c) 2019, Marvell International Ltd. All rights reserved.
*  Copyright (c) 2021, Semihalf. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MvGpioLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/NonDiscoverableDevice.h>

#include "NonDiscoverableInitLib.h"

STATIC
EFI_STATUS
EFIAPI
ConfigurePins (
  IN  CONST MV_GPIO_PIN        *VbusPin,
  IN  UINTN                     PinCount,
  IN  MV_GPIO_DRIVER_TYPE       DriverType
  )
{
  EMBEDDED_GPIO_MODE   Mode;
  EMBEDDED_GPIO_PIN    Gpio;
  EMBEDDED_GPIO       *GpioProtocol;
  EFI_STATUS           Status;
  UINTN                Index;

  Status = MvGpioGetProtocol (DriverType, &GpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to find GPIO protocol\n", __FUNCTION__));
    return Status;
  }

  for (Index = 0; Index < PinCount; Index++) {
    Mode = VbusPin->ActiveHigh ? GPIO_MODE_OUTPUT_1 : GPIO_MODE_OUTPUT_0;
    Gpio = GPIO (VbusPin->ControllerId, VbusPin->PinNumber);
    GpioProtocol->Set (GpioProtocol, Gpio, Mode);
    VbusPin++;
  }

  return EFI_SUCCESS;
}

STATIC CONST MV_GPIO_PIN mApSdMmcPins[] = {
  {
    MV_GPIO_DRIVER_TYPE_SOC_CONTROLLER,
    MV_GPIO_CP0_CONTROLLER0,
    CN913X_CEX7_AP_SDMMC_VCCQ_PIN,
    TRUE,
  },
};

STATIC
EFI_STATUS
EFIAPI
ApSdMmcInit (
  IN  NON_DISCOVERABLE_DEVICE  *This
  )
{
  return ConfigurePins (mApSdMmcPins,
           ARRAY_SIZE (mApSdMmcPins),
           MV_GPIO_DRIVER_TYPE_SOC_CONTROLLER);
}

NON_DISCOVERABLE_DEVICE_INIT
EFIAPI
NonDiscoverableDeviceInitializerGet (
  IN  NON_DISCOVERABLE_DEVICE_TYPE  Type,
  IN  UINTN                         Index
  )
{
  if (Type == NonDiscoverableDeviceTypeSdhci && Index == 0) {
    return ApSdMmcInit;
  }

  return NULL;
}
