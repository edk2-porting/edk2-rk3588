/**
*
*  Copyright (c) 2018, Marvell International Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Protocol/I2cIo.h>

#include <Pi/PiI2c.h>

#include "MvPca95xxDxe.h"

STATIC PCA95XX *mPca95xxInstance;

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
  MV_GPIO_DRIVER_TYPE_PCA95XX,
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof(EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC PCA95XX_PIN_COUNT mPca95xxPinCount[PCA95XX_MAX_ID] = {
  PCA9505_PIN_COUNT,
  PCA9534_PIN_COUNT,
  PCA9535_PIN_COUNT,
  PCA9536_PIN_COUNT,
  PCA9537_PIN_COUNT,
  PCA9538_PIN_COUNT,
  PCA9539_PIN_COUNT,
  PCA9554_PIN_COUNT,
  PCA9555_PIN_COUNT,
  PCA9556_PIN_COUNT,
  PCA9557_PIN_COUNT,
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
MvPca95xxValidate (
  IN UINTN ControllerIndex,
  IN UINTN GpioPin
  )
{
  UINTN ControllerId;

  if (ControllerIndex >= mPca95xxInstance->GpioExpanderCount) {
    DEBUG ((DEBUG_ERROR,
      "%a: Invalid GPIO ControllerIndex: %d\n",
      __FUNCTION__,
      ControllerIndex));
    return EFI_INVALID_PARAMETER;
  }

  ControllerId = mPca95xxInstance->GpioExpanders[ControllerIndex].ChipId;

  if (GpioPin >= mPca95xxPinCount[ControllerId]) {
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

EFI_STATUS
EFIAPI
MvPca95xxGetI2c (
  IN     UINTN                 ControllerIndex,
  IN OUT EFI_I2C_IO_PROTOCOL **I2cIo
  )
{
  UINTN        I2cBus, I2cAddress;
  UINTN        HandleCount, Index;
  EFI_HANDLE  *HandleBuffer;
  EFI_STATUS   Status;

  I2cBus = mPca95xxInstance->GpioExpanders[ControllerIndex].I2cBus;
  I2cAddress = mPca95xxInstance->GpioExpanders[ControllerIndex].I2cAddress;

  /* Locate Handles of all EfiI2cIoProtocol producers */
  Status = gBS->LocateHandleBuffer (ByProtocol,
                  &gEfiI2cIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to locate handles\n", __FUNCTION__));
    return Status;
  }

  /* Iterate over all protocol producers and pick one upon DeviceIndex match */
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->OpenProtocol (HandleBuffer[Index],
                    &gEfiI2cIoProtocolGuid,
                    (VOID **)I2cIo,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to open protocol\n", __FUNCTION__));
      gBS->FreePool (HandleBuffer);
      return Status;
    }
    if ((*I2cIo)->DeviceIndex == I2C_DEVICE_INDEX (I2cBus, I2cAddress)) {
      gBS->FreePool (HandleBuffer);
      return EFI_SUCCESS;
    }
  }

  gBS->FreePool (HandleBuffer);

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
MvPca95xxI2cTransfer (
  IN EFI_I2C_IO_PROTOCOL *I2cIo,
  IN UINT8                Address,
  IN UINT8               *Buffer,
  IN UINT32               Flag
  )
{
  EFI_I2C_REQUEST_PACKET *RequestPacket;
  UINTN RequestPacketSize;
  UINT8 AddressBuffer;
  EFI_STATUS Status;

  RequestPacketSize = sizeof (UINTN) +
                      sizeof (EFI_I2C_OPERATION) * PCA95XX_OPERATION_COUNT;
  RequestPacket = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  /* Operations contain address and payload, consecutively. */
  RequestPacket->OperationCount = PCA95XX_OPERATION_COUNT;
  RequestPacket->Operation[0].LengthInBytes = PCA95XX_OPERATION_LENGTH;
  RequestPacket->Operation[0].Buffer = &AddressBuffer;
  RequestPacket->Operation[0].Buffer[0] = Address & MAX_UINT8;
  RequestPacket->Operation[1].LengthInBytes = PCA95XX_OPERATION_LENGTH;
  RequestPacket->Operation[1].Buffer = Buffer;
  RequestPacket->Operation[1].Flags = Flag;

  Status = I2cIo->QueueRequest (I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: transmission error: 0x%d\n",
      __FUNCTION__,
      Status));
  }

  gBS->FreePool(RequestPacket);

  return Status;
}

STATIC
EFI_STATUS
MvPca95xxReadRegs (
  IN  EFI_I2C_IO_PROTOCOL *I2cIo,
  IN  UINT8                Reg,
  OUT UINT8               *RegVal
  )
{
  return MvPca95xxI2cTransfer (I2cIo, Reg, RegVal, I2C_FLAG_READ);
}

STATIC
EFI_STATUS
MvPca95xxWriteRegs (
  IN  EFI_I2C_IO_PROTOCOL *I2cIo,
  IN  UINTN                Reg,
  IN  UINT8                RegVal
  )
{
  return MvPca95xxI2cTransfer (I2cIo, Reg, &RegVal, I2C_FLAG_NORESTART);
}

STATIC
EFI_STATUS
MvPca95xxSetOutputValue (
  IN UINTN               ControllerIndex,
  IN UINTN               GpioPin,
  IN EMBEDDED_GPIO_MODE  Mode
  )
{
  EFI_I2C_IO_PROTOCOL *I2cIo;
  EFI_STATUS Status;
  UINT8 RegVal;
  UINTN Bank;

  Status = MvPca95xxGetI2c (ControllerIndex, &I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Bank = GpioPin / PCA95XX_BANK_SIZE;

  Status = MvPca95xxReadRegs (I2cIo, PCA95XX_OUTPUT_REG + Bank, &RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to read device register\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  if (Mode == GPIO_MODE_OUTPUT_1) {
    RegVal |= (1 << (GpioPin % PCA95XX_BANK_SIZE));
  } else {
    RegVal &= ~(1 << (GpioPin % PCA95XX_BANK_SIZE));
  }

  Status = MvPca95xxWriteRegs (I2cIo, PCA95XX_OUTPUT_REG + Bank, RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to write device register\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvPca95xxSetDirection (
  IN UINTN              ControllerIndex,
  IN UINTN              GpioPin,
  IN EMBEDDED_GPIO_MODE Mode
  )
{
  EFI_I2C_IO_PROTOCOL *I2cIo;
  EFI_STATUS Status;
  UINT8 RegVal;
  UINTN Bank;

  Status = MvPca95xxGetI2c (ControllerIndex, &I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return Status;
  }

  Bank = GpioPin / PCA95XX_BANK_SIZE;

  Status = MvPca95xxReadRegs (I2cIo, PCA95XX_DIRECTION_REG + Bank, &RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to read device register\n", __FUNCTION__));
    return Status;
  }

  if (Mode == GPIO_MODE_INPUT) {
    RegVal |= (1 << (GpioPin % PCA95XX_BANK_SIZE));
  } else {
    RegVal &= ~(1 << (GpioPin % PCA95XX_BANK_SIZE));
  }

  Status = MvPca95xxWriteRegs (I2cIo, PCA95XX_DIRECTION_REG + Bank, RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to write device register\n", __FUNCTION__));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvPca95xxReadMode (
  IN  UINTN               ControllerIndex,
  IN  UINTN               GpioPin,
  OUT EMBEDDED_GPIO_MODE *Mode
  )
{
  EFI_I2C_IO_PROTOCOL *I2cIo;
  EFI_STATUS Status;
  UINT8 RegVal;
  UINTN Bank;

  ASSERT_EFI_ERROR (MvPca95xxValidate (ControllerIndex, GpioPin));

  Status = MvPca95xxGetI2c (ControllerIndex, &I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return Status;
  }

  Bank = GpioPin / PCA95XX_BANK_SIZE;

  Status = MvPca95xxReadRegs (I2cIo, PCA95XX_DIRECTION_REG + Bank, &RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to read device register\n", __FUNCTION__));
    return Status;
  }

  if (RegVal & (1 << (GpioPin % PCA95XX_BANK_SIZE))) {
    *Mode = GPIO_MODE_INPUT;
  } else {
    Status = MvPca95xxReadRegs (I2cIo, PCA95XX_INPUT_REG + Bank, &RegVal);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: fail to read device register\n", __FUNCTION__));
      return Status;
    }

    if (RegVal & (1 << (GpioPin % PCA95XX_BANK_SIZE))) {
      *Mode = GPIO_MODE_OUTPUT_1;
    } else {
      *Mode = GPIO_MODE_OUTPUT_0;
    }
  }

  return EFI_SUCCESS;
}

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
MvPca95xxGetMode (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN    Gpio,
  OUT EMBEDDED_GPIO_MODE  *Mode
  )
{
  EFI_STATUS Status;
  UINTN ControllerIndex;
  UINTN GpioPin;

  GpioPin = GPIO_PIN (Gpio);
  ControllerIndex = GPIO_PORT (Gpio);

  ASSERT_EFI_ERROR (MvPca95xxValidate (ControllerIndex, GpioPin));

  Status = MvPca95xxReadMode (ControllerIndex, GpioPin, Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: fail to get pin %d of controller#%d mode\n",
      __FUNCTION__,
      GpioPin,
      ControllerIndex));
  }

  return Status;
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
MvPca95xxGet (
  IN  EMBEDDED_GPIO      *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  OUT UINTN              *Value
  )
{
  EFI_I2C_IO_PROTOCOL *I2cIo;
  EFI_STATUS Status;
  UINTN ControllerIndex;
  UINTN GpioPin;
  UINT8 RegVal;
  UINTN Bank;

  GpioPin = GPIO_PIN (Gpio);
  ControllerIndex = GPIO_PORT (Gpio);

  ASSERT_EFI_ERROR (MvPca95xxValidate (ControllerIndex, GpioPin));

  Status = MvPca95xxGetI2c (ControllerIndex, &I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return Status;
  }

  Bank = GpioPin / PCA95XX_BANK_SIZE;

  Status = MvPca95xxReadRegs (I2cIo, PCA95XX_INPUT_REG + Bank, &RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to read device register\n", __FUNCTION__));
    return Status;
  }

  if (RegVal & (1 << (GpioPin % PCA95XX_BANK_SIZE))) {
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
MvPca95xxSet (
  IN EMBEDDED_GPIO       *This,
  IN EMBEDDED_GPIO_PIN   Gpio,
  IN EMBEDDED_GPIO_MODE  Mode
  )
{
  EFI_STATUS Status;
  UINTN ControllerIndex;
  UINTN GpioPin;

  GpioPin = GPIO_PIN (Gpio);
  ControllerIndex = GPIO_PORT (Gpio);

  ASSERT_EFI_ERROR (MvPca95xxValidate (ControllerIndex, GpioPin));

  switch (Mode) {
  case GPIO_MODE_OUTPUT_0:
  case GPIO_MODE_OUTPUT_1:
    Status = MvPca95xxSetOutputValue (ControllerIndex, GpioPin, Mode);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: fail to set ouput value\n", __FUNCTION__));
      return Status;
    }

  /* Fall-through */
  case GPIO_MODE_INPUT:
    Status = MvPca95xxSetDirection (ControllerIndex, GpioPin, Mode);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: fail to set direction\n", __FUNCTION__));
      return Status;
    }
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
MvPca95xxSetPull (
  IN EMBEDDED_GPIO       *This,
  IN EMBEDDED_GPIO_PIN   Gpio,
  IN EMBEDDED_GPIO_PULL  Direction
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
VOID
MvPca95xxInitProtocol (
  IN EMBEDDED_GPIO *GpioProtocol
  )
{
  GpioProtocol->Get     = MvPca95xxGet;
  GpioProtocol->Set     = MvPca95xxSet;
  GpioProtocol->GetMode = MvPca95xxGetMode;
  GpioProtocol->SetPull = MvPca95xxSetPull;
}

EFI_STATUS
EFIAPI
MvPca95xxEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  MARVELL_BOARD_DESC_PROTOCOL *MvBoardProtocol;
  MV_BOARD_GPIO_DESCRIPTION *GpioDescription;
  MV_GPIO_DEVICE_PATH *Pca95xxDevicePath;
  EFI_STATUS Status;

  /* Obtain list of available controllers */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&MvBoardProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot locate BoardDesc protocol\n",
      __FUNCTION__));
    return Status;
  }

  Status = MvBoardProtocol->GpioDescriptionGet (MvBoardProtocol,
             &GpioDescription);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot get GPIO board desc from BoardDesc protocol\n",
      __FUNCTION__));
    return Status;
  } else if (GpioDescription->GpioExpanders == NULL) {
    /* Silently exit, if the board does not support the controllers */
    return EFI_SUCCESS;
  }

  Pca95xxDevicePath = AllocateCopyPool (sizeof (MV_GPIO_DEVICE_PATH),
                        &mDevicePathTemplate);
  if (Pca95xxDevicePath == NULL) {
    DEBUG ((DEBUG_ERROR,
      "%a: Fail to allocate Pca95xxDevicePath\n",
      __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  mPca95xxInstance = AllocateZeroPool (sizeof (PCA95XX));
  if (mPca95xxInstance == NULL) {
    DEBUG ((DEBUG_ERROR,
      "%a: Fail to allocate mPca95xxInstance\n",
      __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrPca95xxInstanceAlloc;
  }

  MvPca95xxInitProtocol (&mPca95xxInstance->GpioProtocol);

  mPca95xxInstance->Signature = PCA95XX_GPIO_SIGNATURE;
  mPca95xxInstance->GpioExpanders = GpioDescription->GpioExpanders;
  mPca95xxInstance->GpioExpanderCount = GpioDescription->GpioExpanderCount;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(mPca95xxInstance->ControllerHandle),
                  &gEmbeddedGpioProtocolGuid,
                  &(mPca95xxInstance->GpioProtocol),
                  &gEfiDevicePathProtocolGuid,
                  (EFI_DEVICE_PATH_PROTOCOL *)Pca95xxDevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    goto ErrInstallProtocols;
  }

  return EFI_SUCCESS;

ErrInstallProtocols:
  gBS->FreePool (mPca95xxInstance);

ErrPca95xxInstanceAlloc:
  gBS->FreePool (Pca95xxDevicePath);

  return Status;
}
