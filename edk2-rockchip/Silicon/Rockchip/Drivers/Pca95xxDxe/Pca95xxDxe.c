/** @file
 *
 *  PCA95XX GPIO driver for Rockchip platforms.
 *
 *  Contribs:
 *
 *  Copyright (c) 2018, Marvell International Ltd. All rights reserved.
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2023, Shimrra Shai <shimmyshai00@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

#include <Protocol/RockchipI2cMasterProtocol.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2c.h>
#include <Protocol/EmbeddedGpio.h>

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2c.h>
#include <Protocol/EmbeddedGpio.h>

#include <Protocol/Pca95xx.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Pi/PiI2c.h>

#include "Pca95xxDxe.h"
#include "ProcessorBind.h"
#include "Uefi/UefiBaseType.h"
#include "Uefi/UefiSpec.h"

STATIC CONST EFI_GUID  I2cGuid = I2C_GUID;


typedef struct {
    UINT8 Input;
    UINT8 Output;
    UINT8 Polarity;
    UINT8 Control;
} PCA95XX_REGMAP;

typedef struct {
    PCA95XX_REGMAP Regs;
    UINT8 Ngpios;
    UINT8 NBanks;
} PCA95XX_CONFIG;

typedef struct {
    CHAR8       *Name;
    PCA95XX_CONFIG PcaConfig;
} PCA95XX_VARIANT;

STATIC CONST PCA95XX_VARIANT mPca95xxVariants[] = {
    { "PCA9554", {{0x00, 0x01, 0x02, 0x03}, 8,  1 }},
    { "PCA9555", {{0x00, 0x02, 0x04, 0x06}, 16, 2 }},
};

STATIC CONST PCA95XX_CONFIG *mPca95xxConfig = NULL;

/* Much of the driver below is ripped from the Marvell version, with
 * some adaptation.
 */

/* --- Low-level Methods --- */

EFI_STATUS
EFIAPI
Pca95xxI2cTransfer (
  IN EFI_I2C_IO_PROTOCOL  *I2cIo,
  IN UINT8                Address,
  IN UINT8                *Buffer,
  IN BOOLEAN              Read
  )
{
  EFI_I2C_REQUEST_PACKET  *RequestPacket;
  UINTN                   RequestPacketSize;
  UINT8                   AddressBuffer;
  UINT8                   WritePacketBuffer[2];
  EFI_STATUS              Status;
  UINTN                   NumOps;

  if (Read) {
    NumOps = 2;
  } else {
    NumOps = 1;
  }

  RequestPacketSize = sizeof (UINTN) +
                      sizeof (EFI_I2C_OPERATION) * NumOps;
  RequestPacket = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  /* Operations contain address and payload, consecutively. */

  /* Note: in the Rockchip I2C driver, a special control byte is sent with
   *       each operation *implicitly*. PCA95XX expects this only ONCE. So
   *       unlike the Marvell driver, we should encode the whole operation
   *       on a single buffer.
   */
  if (Read) {
    RequestPacket->OperationCount             = NumOps;
    RequestPacket->Operation[0].LengthInBytes = 1;
    RequestPacket->Operation[0].Buffer        = &AddressBuffer;
    RequestPacket->Operation[0].Buffer[0]     = Address & MAX_UINT8;
    RequestPacket->Operation[0].Flags         = 0;

    RequestPacket->Operation[1].LengthInBytes = 1;
    RequestPacket->Operation[1].Buffer        = Buffer;
    RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  } else {
    RequestPacket->OperationCount             = NumOps;
    RequestPacket->Operation[0].LengthInBytes = 2;
    RequestPacket->Operation[0].Buffer        = WritePacketBuffer;
    RequestPacket->Operation[0].Buffer[0]     = Address & MAX_UINT8;
    RequestPacket->Operation[0].Buffer[1]     = Buffer[0];
    RequestPacket->Operation[0].Flags         = 0;
  }

  Status = I2cIo->QueueRequest (I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: transmission error: 0x%d\n",
      __FUNCTION__,
      Status
      ));
  }

  gBS->FreePool (RequestPacket);

  return Status;
}

STATIC
EFI_STATUS
Pca95xxReadRegs (
  IN  EFI_I2C_IO_PROTOCOL  *I2cIo,
  IN  UINT8                Reg,
  OUT UINT8                *RegVal
  )
{
  return Pca95xxI2cTransfer (I2cIo, Reg, RegVal, TRUE);
}

STATIC
EFI_STATUS
Pca95xxWriteRegs (
  IN  EFI_I2C_IO_PROTOCOL  *I2cIo,
  IN  UINTN                Reg,
  IN  UINT8                RegVal
  )
{
  return Pca95xxI2cTransfer (I2cIo, Reg, &RegVal, FALSE);
}

#if 0
VOID
EFIAPI
DumpRegs (
  IN EFI_I2C_IO_PROTOCOL  *I2cIo
  )
{
  UINT8  Regs[8];
  UINTN Index;

  for (Index = 0; Index < 4 * mPca95xxConfig->NBanks; Index++) {
    Pca95xxReadRegs (I2cIo, Index, &Regs[Index]);
  }

  DEBUG ((DEBUG_INFO, "PCA95XX state:\n"));
  for (Index = 0; Index < 4 * mPca95xxConfig->NBanks; Index++) {
    DEBUG ((DEBUG_INFO, "reg %d = 0x%02X\n", Index, Regs[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

#endif

/* ------------------------- */

/* --- Driver Methods --- */
STATIC PCA95XX_CONTEXT  *mPca95xxContext = NULL;

/* Probe the device, bringing it up. */
EFI_STATUS
EFIAPI
Pca95xxProbe (
  IN EFI_I2C_IO_PROTOCOL  *I2c
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       Zero   = 0x00;
  UINTN       Index;

  if (mPca95xxContext != NULL) {
    /* The device has already been brought online. Nothing to do. */
    return EFI_SUCCESS;
  }

  mPca95xxContext = AllocateZeroPool (sizeof (PCA95XX_CONTEXT));
  if (mPca95xxContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: context allocation failed\n", __func__));
    Status = EFI_OUT_OF_RESOURCES;
    goto fail;
  }

  mPca95xxContext->Signature = PCA95XX_SIGNATURE;

  mPca95xxContext->I2cAddress = PcdGet8 (PcdPca95xxAddress);
  mPca95xxContext->I2cBus     = PcdGet8 (PcdPca95xxBus);
  mPca95xxContext->Type = (CONST CHAR8 *)PcdGetPtr (PcdPca95xxType);


  for (Index = 0; Index < ARRAY_SIZE(mPca95xxVariants); Index++) {
    if (AsciiStrCmp(mPca95xxContext->Type, mPca95xxVariants[Index].Name) == 0) {
        mPca95xxConfig = &mPca95xxVariants[Index].PcaConfig;
        break;
    }
  }

  if (mPca95xxConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: unsupported PCA95xx type '%a'\n",
            __func__, mPca95xxContext->Type));
    Status = EFI_UNSUPPORTED;
    goto fail;
  }

  /* Device setup: disable polarity inversion on all pins */
  for (Index = 0; Index < mPca95xxConfig->NBanks; ++Index) {
    Status = Pca95xxWriteRegs (I2c, mPca95xxConfig->Regs.Polarity + Index, Zero);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: setup failed!\n", __FUNCTION__));
      goto fail;
    }
  }

  /* and preset all outputs to 0 */
  for (Index = 0; Index < mPca95xxConfig->NBanks; ++Index) {
    Status = Pca95xxWriteRegs (I2c, mPca95xxConfig->Regs.Output + Index, Zero);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: setup failed!\n", __FUNCTION__));
      goto fail;
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: PCA95XX bound at I2C addr 0x%02X, bus 0x%02X\n",
    __FUNCTION__,
    mPca95xxContext->I2cAddress,
    mPca95xxContext->I2cBus
    ));

  return EFI_SUCCESS;
fail:
  if (mPca95xxContext != NULL) {
    FreePool (mPca95xxContext);
    mPca95xxContext = NULL;
  }

  return Status;
}

/* Shut down the device */
EFI_STATUS
EFIAPI
Pca95xxUnprobe (
  IN EFI_I2C_IO_PROTOCOL  *I2c
  )
{
  if (mPca95xxContext != NULL) {
    FreePool (mPca95xxContext);
    mPca95xxContext = NULL;

    DEBUG ((DEBUG_INFO, "%a: PCA95XX has been shut down.", __FUNCTION__));
  }

  return EFI_SUCCESS;
}

/* Read a pin register */
EFI_STATUS
EFIAPI
Pca95xxReadPinReg (
  IN EFI_I2C_IO_PROTOCOL  *I2c,
  IN UINT8                BaseReg,
  IN UINTN                GpioPin,
  OUT UINTN               *Value
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       Bank;
  UINT8       RegVal = 0x00;

  if (mPca95xxContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: device not probed", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Bank   = GpioPin / PCA95XX_BANK_SIZE;
  Status = Pca95xxReadRegs (I2c, BaseReg + Bank, &RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: read failure", __FUNCTION__));
    return Status;
  }

  if (RegVal & (1 << (GpioPin % PCA95XX_BANK_SIZE))) {
    *Value = 1;
  } else {
    *Value = 0;
  }

  return EFI_SUCCESS;
}

/* Write the output pin register */
EFI_STATUS
EFIAPI
Pca95xxWritePinOutReg (
  IN EFI_I2C_IO_PROTOCOL  *I2c,
  IN UINTN                GpioPin,
  IN UINTN                Value
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       Bank;
  UINT8       RegVal = 0x00;

  if (mPca95xxContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: device not probed", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Bank = GpioPin / PCA95XX_BANK_SIZE;

  Status = Pca95xxReadRegs (
             I2c,
             mPca95xxConfig->Regs.Output + Bank,
             &RegVal
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: read failure", __FUNCTION__));
    return Status;
  }

  if (Value) {
    RegVal |= (1 << (GpioPin % PCA95XX_BANK_SIZE));
  } else {
    RegVal &= ~(1 << (GpioPin % PCA95XX_BANK_SIZE));
  }

  Status = Pca95xxWriteRegs (
             I2c,
             mPca95xxConfig->Regs.Output + Bank,
             RegVal
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: write failure", __FUNCTION__));
    return Status;
  }

  return EFI_SUCCESS;
}

/* Write the pin control register */
EFI_STATUS
EFIAPI
Pca95xxWritePinCtrlReg (
  IN EFI_I2C_IO_PROTOCOL  *I2c,
  IN UINTN                GpioPin,
  IN UINTN                Value
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       Bank;
  UINT8       RegVal = 0x00;

  if (mPca95xxContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: device not probed", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Bank = GpioPin / PCA95XX_BANK_SIZE;

  Status = Pca95xxReadRegs (
             I2c,
             mPca95xxConfig->Regs.Control + Bank,
             &RegVal
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: read failure", __FUNCTION__));
    return Status;
  }

  if (Value) {
    RegVal |= (1 << (GpioPin % PCA95XX_BANK_SIZE));
  } else {
    RegVal &= ~(1 << (GpioPin % PCA95XX_BANK_SIZE));
  }

  Status = Pca95xxWriteRegs (
             I2c,
             mPca95xxConfig->Regs.Control + Bank,
             RegVal
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: write failure", __FUNCTION__));
    return Status;
  }

  return EFI_SUCCESS;
}

/* ---------------------- */

/* --- Embedded GPIO Protocol --- */

/* Get the I2C */
EFI_STATUS
EFIAPI
Pca95xxGetI2c (
  IN OUT EFI_I2C_IO_PROTOCOL  **I2cIo
  )
{
  UINTN       I2cBus, I2cAddress;
  UINTN       HandleCount, Index;
  EFI_HANDLE  *HandleBuffer;
  EFI_STATUS  Status;

  if (mPca95xxContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: device not probed\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  I2cBus     = mPca95xxContext->I2cBus;
  I2cAddress = mPca95xxContext->I2cAddress;

  /* Locate Handles of all EfiI2cIoProtocol producers */
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiI2cIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to locate handles\n", __FUNCTION__));
    return Status;
  }

  /* Iterate over all protocol producers and pick one upon DeviceIndex match */
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->OpenProtocol (
                    HandleBuffer[Index],
                    &gEfiI2cIoProtocolGuid,
                    (VOID **)I2cIo,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
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

#if !defined (MDEPKG_NDEBUG)
/* A validator; unlike Marvell version we only assume 1 chip */
STATIC
EFI_STATUS
Pca95xxValidatePin (
  IN UINTN  GpioPin
  )
{
  if (GpioPin >= mPca95xxConfig->Ngpios) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: GPIO pin #%d not available\n",
      __FUNCTION__,
      GpioPin
      ));
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

#endif

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
Pca95xxGet (
  IN  EMBEDDED_GPIO      *This,
  IN  EMBEDDED_GPIO_PIN  Gpio,
  OUT UINTN              *Value
  )
{
  EFI_I2C_IO_PROTOCOL  *I2cIo;
  EFI_STATUS           Status;
  UINTN                GpioPin;

  GpioPin = GPIO_PIN (Gpio);

  ASSERT_EFI_ERROR (Pca95xxValidatePin (GpioPin));

  Status = Pca95xxGetI2c (&I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return Status;
  }

  Status = Pca95xxReadPinReg (I2cIo, mPca95xxConfig->Regs.Input, Gpio, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: register read failure\n", __FUNCTION__));
    return Status;
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
Pca95xxSet (
  IN EMBEDDED_GPIO       *This,
  IN EMBEDDED_GPIO_PIN   Gpio,
  IN EMBEDDED_GPIO_MODE  Mode
  )
{
  EFI_I2C_IO_PROTOCOL  *I2cIo;
  EFI_STATUS           Status;
  UINTN                GpioPin;
  UINTN                Bit;

  GpioPin = GPIO_PIN (Gpio);

  ASSERT_EFI_ERROR (Pca95xxValidatePin (GpioPin));

  Status = Pca95xxGetI2c (&I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return Status;
  }

 #if 0
  DEBUG ((DEBUG_INFO, "Status before set of pin %d to mode 0x%02X:\n", Gpio, Mode));
  DumpRegs (I2cIo);
 #endif

  switch (Mode) {
    case GPIO_MODE_OUTPUT_0:
    case GPIO_MODE_OUTPUT_1:
      if (Mode == GPIO_MODE_OUTPUT_0) {
        Bit = 0;
      } else {
        Bit = 1;
      }

      Status = Pca95xxWritePinOutReg (I2cIo, GpioPin, Bit);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: fail to set ouput value\n", __FUNCTION__));
        return Status;
      }

      Status = Pca95xxWritePinCtrlReg (I2cIo, GpioPin, 0);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: fail to set direction\n", __FUNCTION__));
        return Status;
      }

      break;
    case GPIO_MODE_INPUT:
      Status = Pca95xxWritePinCtrlReg (I2cIo, GpioPin, 1);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: fail to set direction\n", __FUNCTION__));
        return Status;
      }

      break;

    default:
      return EFI_UNSUPPORTED;
  }

 #if 0
  DEBUG ((DEBUG_INFO, "Status after set:\n"));
  DumpRegs (I2cIo);
 #endif

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
Pca95xxGetMode (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  OUT EMBEDDED_GPIO_MODE  *Mode
  )
{
  EFI_I2C_IO_PROTOCOL  *I2cIo;
  EFI_STATUS           Status;
  UINTN                GpioPin;
  UINTN                RegVal;

  GpioPin = GPIO_PIN (Gpio);

  ASSERT_EFI_ERROR (Pca95xxValidatePin (GpioPin));

  Status = Pca95xxGetI2c (&I2cIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get I2C protocol\n", __FUNCTION__));
    return Status;
  }

  Status = Pca95xxReadPinReg (I2cIo, mPca95xxConfig->Regs.Control, Gpio, &RegVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: register read failure\n", __FUNCTION__));
    return Status;
  }

  if (RegVal == 0) {
    /* output modes */
    Status = Pca95xxReadPinReg (I2cIo, mPca95xxConfig->Regs.Output, Gpio, &RegVal);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: register read failure\n", __FUNCTION__));
      return Status;
    }

    if (RegVal) {
      *Mode = GPIO_MODE_OUTPUT_1;
    } else {
      *Mode = GPIO_MODE_OUTPUT_0;
    }
  } else {
    /* input mode */
    *Mode = GPIO_MODE_INPUT;
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
Pca95xxSetPull (
  IN EMBEDDED_GPIO       *This,
  IN EMBEDDED_GPIO_PIN   Gpio,
  IN EMBEDDED_GPIO_PULL  Direction
  )
{
  return EFI_UNSUPPORTED;
}

PCA95XX_PROTOCOL  mPca95xxProtocol = {
  .GpioProtocol = {
    .Get     = Pca95xxGet,
    .Set     = Pca95xxSet,
    .GetMode = Pca95xxGetMode,
    .SetPull = Pca95xxSetPull,
  },
};

/* ------------------------------ */

/* --- Binding Protocol --- */
EFI_DRIVER_BINDING_PROTOCOL  mDriverBindingProtocol = {
  Pca95xxSupported,
  Pca95xxStart,
  Pca95xxStop,
};

EFI_STATUS
EFIAPI
Pca95xxSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS           Status = EFI_UNSUPPORTED;
  EFI_I2C_IO_PROTOCOL  *TmpI2cIo;
  UINT8                Pca95xxAddress = 0x00;
  UINT8                Pca95xxBus     = 0x00;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&TmpI2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Pca95xxAddress = PcdGet8 (PcdPca95xxAddress);
  Pca95xxBus     = PcdGet8 (PcdPca95xxBus);

  DEBUG ((
    DEBUG_INFO,
    "%a: addr = 0x%02X, bus = 0x%02X\n",
    __FUNCTION__,
    Pca95xxAddress,
    Pca95xxBus
    ));

  if (CompareGuid (TmpI2cIo->DeviceGuid, &I2cGuid) &&
      (TmpI2cIo->DeviceIndex == I2C_DEVICE_INDEX (Pca95xxBus, Pca95xxAddress)))
  {
    DEBUG ((DEBUG_INFO, "%a: PCA95XX device is supported\n", __FUNCTION__));
    Status = EFI_SUCCESS;
  }

  /*out:*/
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );

  return Status;
}

EFI_STATUS
EFIAPI
Pca95xxStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  EFI_I2C_IO_PROTOCOL  *TmpI2cIo;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&TmpI2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = Pca95xxProbe (TmpI2cIo);
  if (EFI_ERROR (Status)) {
    goto fail;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gPca95xxProtocolGuid,
                  &mPca95xxProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto fail;
  }

  Status = EFI_SUCCESS;

fail:
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );

  return Status;
}

EFI_STATUS
EFIAPI
Pca95xxStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
  // EFI_STATUS  Status = EFI_SUCCESS;

  gBS->UninstallMultipleProtocolInterfaces (
         &ControllerHandle,
         &gPca95xxProtocolGuid,
         &mPca95xxProtocol,
         &gEfiDriverBindingProtocolGuid,
         &mDriverBindingProtocol,
         NULL
         );

  /* (SS) is this necessary? */

  /*
  Status = Pca95xxUnprobe();
  gBS->CloseProtocol (
        ControllerHandle,
        &gEfiI2cIoProtocolGuid,
        gImageHandle,
        ControllerHandle
        );
        */

  return EFI_SUCCESS;
}

/* ------------------------ */

EFI_STATUS
EFIAPI
Pca95xxDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  &mDriverBindingProtocol,
                  NULL
                  );

  return Status;
}
