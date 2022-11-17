/**
*
*  Copyright (c) 2018, Marvell International Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __MV_PCA953X_H__
#define __MV_PCA953X_H__

#include <Library/ArmadaBoardDescLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MvGpioLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/BoardDesc.h>
#include <Protocol/MvI2c.h>

#include <Uefi/UefiBaseType.h>

#define PCA95XX_GPIO_SIGNATURE   SIGNATURE_32 ('I', 'O', 'E', 'X')

#define PCA95XX_INPUT_REG        0x0
#define PCA95XX_OUTPUT_REG       0x2
#define PCA95XX_DIRECTION_REG    0x6

#define PCA95XX_BANK_SIZE        8
#define PCA95XX_OPERATION_COUNT  2
#define PCA95XX_OPERATION_LENGTH 1

typedef enum {
  PCA9505_PIN_COUNT = 40,
  PCA9534_PIN_COUNT = 8,
  PCA9535_PIN_COUNT = 16,
  PCA9536_PIN_COUNT = 4,
  PCA9537_PIN_COUNT = 4,
  PCA9538_PIN_COUNT = 8,
  PCA9539_PIN_COUNT = 16,
  PCA9554_PIN_COUNT = 8,
  PCA9555_PIN_COUNT = 16,
  PCA9556_PIN_COUNT = 16,
  PCA9557_PIN_COUNT = 16,
} PCA95XX_PIN_COUNT;

typedef struct {
  EMBEDDED_GPIO      GpioProtocol;
  MV_GPIO_EXPANDER  *GpioExpanders;
  UINTN              GpioExpanderCount;
  UINTN              Signature;
  EFI_HANDLE         ControllerHandle;
} PCA95XX;

#endif
