/**
*
*  Copyright (c) 2018, Marvell International Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "MvGpioDxe.h"

STATIC MV_GPIO *mGpioInstance;

STATIC MV_GPIO_DEVICE_PATH mDevicePathTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH) +
                 sizeof (MV_GPIO_DRIVER_TYPE)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH) +
                 sizeof (MV_GPIO_DRIVER_TYPE)) >> 8),
      },
    },
    EFI_CALLER_ID_GUID
  },
  MV_GPIO_DRIVER_TYPE_SOC_CONTROLLER,
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof(EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

#if !defined(MDEPKG_NDEBUG)
/**

Routine Description:

  Verifies if controller index / GPIO pin values
  are within proper boundaries.

Arguments:

  ControllerIndex - index of controller
  GpioPin - which pin to read

Returns:

  EFI_SUCCESS           - GPIO pin / controller index are proper
  EFI_INVALID_PARAMETER - GPIO pin / controller index is out of range
**/
STATIC
EFI_STATUS
MvGpioValidate (
  IN UINTN ControllerIndex,
  IN UINTN GpioPin
  )
{
  if (ControllerIndex >= mGpioInstance->GpioDeviceCount) {
    DEBUG ((DEBUG_ERROR,
      "%a: Invalid GPIO ControllerIndex: %d\n",
      __FUNCTION__,
      ControllerIndex));
    return EFI_INVALID_PARAMETER;
  }

  if (GpioPin >= mGpioInstance->SoCGpio[ControllerIndex].InternalGpioCount) {
    DEBUG ((DEBUG_ERROR,
      "%a: GPIO pin #%d not available in Controller#%d\n",
      __FUNCTION__,
      GpioPin,
      ControllerIndex));
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}
#endif

/**

Routine Description:

  Gets the mode (function) of a GPIO pin

Arguments:

  This  - pointer to protocol
  Gpio  - which pin
  Mode  - pointer to output mode value

Returns:

  EFI_SUCCESS           - mode value retrieved
  EFI_INVALID_PARAMETER - Mode is a null pointer or Gpio pin is out of range

**/
STATIC
EFI_STATUS
MvGpioGetMode (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN    Gpio,
  OUT EMBEDDED_GPIO_MODE  *Mode
  )
{
  UINTN ControllerIndex;
  UINTN BaseAddress;
  UINTN GpioPin;

  GpioPin = GPIO_PIN (Gpio);
  ControllerIndex = GPIO_PORT (Gpio);

  ASSERT_EFI_ERROR (MvGpioValidate (ControllerIndex, GpioPin));

  BaseAddress = mGpioInstance->SoCGpio[ControllerIndex].RegisterBase;

  if (MmioRead32 (BaseAddress + MV_GPIO_OUT_EN_REG) & (1 << GpioPin)) {
    *Mode = GPIO_MODE_INPUT;
  } else {
    if (MmioRead32 (BaseAddress + MV_GPIO_DATA_IN_REG) & (1 << GpioPin)) {
      *Mode = GPIO_MODE_OUTPUT_1;
    } else {
      *Mode = GPIO_MODE_OUTPUT_0;
    }
  }

  return EFI_SUCCESS;
}

/**

Routine Description:

  Gets the state of a GPIO pin

Arguments:

  This  - pointer to protocol
  Gpio  - which pin to read
  Value - state of the pin

Returns:

  EFI_SUCCESS           - GPIO state returned in Value
  EFI_INVALID_PARAMETER - Value is NULL pointer or Gpio pin is out of range
**/
STATIC
EFI_STATUS
MvGpioGet (
  IN  EMBEDDED_GPIO      *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  OUT UINTN              *Value
  )
{
  UINTN ControllerIndex;
  UINTN BaseAddress;
  UINTN GpioPin;

  GpioPin = GPIO_PIN (Gpio);
  ControllerIndex = GPIO_PORT (Gpio);

  ASSERT_EFI_ERROR (MvGpioValidate (ControllerIndex, GpioPin));

  BaseAddress = mGpioInstance->SoCGpio[ControllerIndex].RegisterBase;

  if (MmioRead32 (BaseAddress + MV_GPIO_DATA_IN_REG) & (1 << GpioPin)) {
    *Value = 1;
  } else {
    *Value = 0;
  }

  return EFI_SUCCESS;
}

/**

Routine Description:

  Sets the state of a GPIO pin

Arguments:

  This  - pointer to protocol
  Gpio  - which pin to modify
  Mode  - mode to set

Returns:

  EFI_SUCCESS           - GPIO set as requested
  EFI_UNSUPPORTED       - Mode is not supported
  EFI_INVALID_PARAMETER - Gpio pin is out of range
**/
STATIC
EFI_STATUS
MvGpioSet (
  IN EMBEDDED_GPIO       *This,
  IN EMBEDDED_GPIO_PIN    Gpio,
  IN EMBEDDED_GPIO_MODE   Mode
  )
{
  UINTN ControllerIndex;
  UINTN BaseAddress;
  UINTN GpioPin;

  GpioPin = GPIO_PIN (Gpio);
  ControllerIndex = GPIO_PORT (Gpio);

  ASSERT_EFI_ERROR (MvGpioValidate (ControllerIndex, GpioPin));

  BaseAddress = mGpioInstance->SoCGpio[ControllerIndex].RegisterBase;

  switch (Mode) {
  case GPIO_MODE_OUTPUT_0:
    MmioAnd32 (BaseAddress + MV_GPIO_DATA_OUT_REG, ~(1 << GpioPin));
    MmioAnd32 (BaseAddress + MV_GPIO_OUT_EN_REG, ~(1 << GpioPin));
    break;

  case GPIO_MODE_OUTPUT_1:
    MmioOr32 (BaseAddress + MV_GPIO_DATA_OUT_REG, (1 << GpioPin));
    MmioAnd32 (BaseAddress + MV_GPIO_OUT_EN_REG, ~(1 << GpioPin));
    break;

  case GPIO_MODE_INPUT:
    MmioOr32 (BaseAddress + MV_GPIO_OUT_EN_REG, (1 << GpioPin));
    break;

  default:
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**

Routine Description:

  Sets the pull-up / pull-down resistor of a GPIO pin

Arguments:

  This  - pointer to protocol
  Gpio  - which pin
  Direction - pull-up, pull-down, or none

Returns:

  EFI_UNSUPPORTED - Can not perform the requested operation

**/
EFI_STATUS
EFIAPI
MvGpioSetPull (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  IN  EMBEDDED_GPIO_PULL  Direction
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
VOID
MvGpioInitProtocol (
  IN EMBEDDED_GPIO *GpioProtocol
  )
{
  GpioProtocol->Get     = MvGpioGet;
  GpioProtocol->Set     = MvGpioSet;
  GpioProtocol->GetMode = MvGpioGetMode;
  GpioProtocol->SetPull = MvGpioSetPull;
}

EFI_STATUS
EFIAPI
MvGpioEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  MARVELL_BOARD_DESC_PROTOCOL *MvBoardProtocol;
  MV_BOARD_GPIO_DESCRIPTION *GpioDescription;
  MV_GPIO_DEVICE_PATH *GpioDevicePath;
  EFI_STATUS Status;

  GpioDevicePath = AllocateCopyPool (sizeof (MV_GPIO_DEVICE_PATH),
                     &mDevicePathTemplate);
  if (GpioDevicePath == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mGpioInstance = AllocateZeroPool (sizeof (MV_GPIO));
  if (mGpioInstance == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrGpioInstanceAlloc;
  }

  /* Obtain list of available controllers */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&MvBoardProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot locate BoardDesc protocol\n",
      __FUNCTION__));
    goto ErrLocateBoardDesc;
  }

  Status = MvBoardProtocol->GpioDescriptionGet (MvBoardProtocol,
             &GpioDescription);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot get GPIO board desc from BoardDesc protocol\n",
      __FUNCTION__));
    goto ErrLocateBoardDesc;
  }

  mGpioInstance->Signature = MV_GPIO_SIGNATURE;
  mGpioInstance->GpioDeviceCount = GpioDescription->GpioDeviceCount;
  mGpioInstance->SoCGpio = GpioDescription->SoCGpio;

  MvGpioInitProtocol (&mGpioInstance->GpioProtocol);

  Status = gBS->InstallMultipleProtocolInterfaces (&(mGpioInstance->Handle),
                  &gEmbeddedGpioProtocolGuid,
                  &(mGpioInstance->GpioProtocol),
                  &gEfiDevicePathProtocolGuid,
                  (EFI_DEVICE_PATH_PROTOCOL *)GpioDevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    goto ErrLocateBoardDesc;
  }

  return EFI_SUCCESS;

ErrLocateBoardDesc:
  gBS->FreePool (mGpioInstance);

ErrGpioInstanceAlloc:
  gBS->FreePool (GpioDevicePath);

  return Status;
}
