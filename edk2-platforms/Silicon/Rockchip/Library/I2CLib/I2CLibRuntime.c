/** @file
*
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*  Copyright (c) 2017, Rockchip Inc. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Guid/EventGroup.h>

#include <Library/I2CLib.h>
#include <Rk3399/Rk3399.h>

STATIC EFI_EVENT              mI2cLibVirtualAddrChangeEvent;

struct RkI2CInfo RkI2CDev[I2C_BUS_MAX] = {
	{ .Regs = (UINT32)RK3399_I2C0_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C1_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C2_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C3_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C4_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C5_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C6_BASE, 0},
	{ .Regs = (UINT32)RK3399_I2C7_BASE, 0}
};

STATIC UINTN gI2cRuntimeBase[I2C_BUS_MAX] = {0};

void *RkI2CGetBase(enum RkI2CBusID BusId)
{
	if (BusId >= I2C_BUS_MAX) {
		DEBUG ((EFI_D_ERROR, "I2C bus error, PLS set i2c bus first!\n"));
		return (void *)NULL;
	}

	if (RkI2CDev[BusId].Regs == 0) {
		DEBUG ((EFI_D_ERROR, "I2C base register error, PLS check i2c config!\n"));
		return (void *)NULL;
	}

	if (gI2cRuntimeBase[BusId] != 0) {
		RkI2CDev[BusId].Regs = gI2cRuntimeBase[BusId];
	}

	return (void *)&RkI2CDev[BusId];
}

VOID
EFIAPI
RkI2cLibVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINT32 BusId;

  for (BusId = 0; BusId < I2C_BUS_MAX; BusId++) {
    if (gI2cRuntimeBase[BusId] != 0) {
      EfiConvertPointer (0x0, (VOID **)&gI2cRuntimeBase[BusId]);
      DEBUG ((EFI_D_INFO, "I2cLibVirtualNotifyEvent: gI2cRuntimeBase[%d]=0x%x\n", BusId, gI2cRuntimeBase[BusId]));
    }
  }

  return;
}

EFI_STATUS
RkI2cLibRuntimeSetup(enum RkI2CBusID BusId)
{
  EFI_STATUS Status;
  struct RkI2CInfo *I2CInfo = (struct RkI2CInfo *)RkI2CGetBase(BusId);
  gI2cRuntimeBase[BusId] = I2CInfo->Regs;

  // Declare the controller as EFI_MEMORY_RUNTIME
  Status = gDS->AddMemorySpace (
      EfiGcdMemoryTypeMemoryMappedIo,
      gI2cRuntimeBase[BusId], SIZE_64KB,
      EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
      );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "[%a:%d] AddMemorySpace failed: %r\n", __FUNCTION__, __LINE__, Status));
  }

  Status = gDS->SetMemorySpaceAttributes (gI2cRuntimeBase[BusId], SIZE_64KB, EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[%a:%d] SetMemorySpaceAttributes failed: %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  //
  // Register for the virtual address change event
  //
  // Only create event once
  if (mI2cLibVirtualAddrChangeEvent == NULL) {
    Status = gBS->CreateEventEx (
        EVT_NOTIFY_SIGNAL,
        TPL_NOTIFY,
        RkI2cLibVirtualNotifyEvent,
        NULL,
        &gEfiEventVirtualAddressChangeGuid,
        &mI2cLibVirtualAddrChangeEvent
    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[%a:%d] Create event failed: %r\n", __FUNCTION__, __LINE__, Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

