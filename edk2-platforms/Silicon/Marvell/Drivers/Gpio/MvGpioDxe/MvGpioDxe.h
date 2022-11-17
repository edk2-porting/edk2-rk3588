/**
*
*  Copyright (c) 2018, Marvell International Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __MV_GPIO_H__
#define __MV_GPIO_H__


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

#include <Uefi/UefiBaseType.h>

#define MV_GPIO_SIGNATURE        SIGNATURE_64 ('M', 'V','_','G', 'P', 'I','O',' ')

// Marvell MV_GPIO Controller Registers
#define MV_GPIO_DATA_OUT_REG     (0x0)
#define MV_GPIO_OUT_EN_REG       (0x4)
#define MV_GPIO_BLINK_EN_REG     (0x8)
#define MV_GPIO_DATA_IN_POL_REG  (0xc)
#define MV_GPIO_DATA_IN_REG      (0x10)

typedef struct {
  EMBEDDED_GPIO     GpioProtocol;
  GPIO_CONTROLLER  *SoCGpio;
  UINTN             GpioDeviceCount;
  UINT64            Signature;
  EFI_HANDLE        Handle;
} MV_GPIO;

#endif // __MV_GPIO_H__
