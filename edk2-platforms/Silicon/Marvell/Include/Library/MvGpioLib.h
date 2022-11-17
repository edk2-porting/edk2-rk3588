/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __MV_GPIO_LIB_H__
#define __MV_GPIO_LIB_H__

#include <Protocol/EmbeddedGpio.h>

typedef enum {
  MV_GPIO_DRIVER_TYPE_SOC_CONTROLLER,
  MV_GPIO_DRIVER_TYPE_PCA95XX,
} MV_GPIO_DRIVER_TYPE;

typedef enum {
  MV_GPIO_AP806_CONTROLLER0,
  MV_GPIO_CP0_CONTROLLER0,
  MV_GPIO_CP0_CONTROLLER1,
  MV_GPIO_CP1_CONTROLLER0,
  MV_GPIO_CP1_CONTROLLER1,
  MV_GPIO_CP2_CONTROLLER0,
  MV_GPIO_CP2_CONTROLLER1,
} MV_GPIO_SOC_CONTROLLER_TYPE;

typedef enum {
  PCA9505_ID,
  PCA9534_ID,
  PCA9535_ID,
  PCA9536_ID,
  PCA9537_ID,
  PCA9538_ID,
  PCA9539_ID,
  PCA9554_ID,
  PCA9555_ID,
  PCA9556_ID,
  PCA9557_ID,
  PCA95XX_MAX_ID,
} MV_GPIO_EXPANDER_TYPE_PCA95XX;

typedef struct {
  VENDOR_DEVICE_PATH        Header;
  MV_GPIO_DRIVER_TYPE       GpioDriverType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} MV_GPIO_DEVICE_PATH;

typedef struct {
  MV_GPIO_DRIVER_TYPE  ControllerType;
  UINTN    ControllerId;
  UINTN    PinNumber;
  BOOLEAN  ActiveHigh;
} MV_GPIO_PIN;

EFI_STATUS
EFIAPI
MvGpioGetProtocol (
  IN     MV_GPIO_DRIVER_TYPE    GpioDriverType,
  IN OUT EMBEDDED_GPIO        **GpioProtocol
  );

#endif // __MV_GPIO_LIB_H__
