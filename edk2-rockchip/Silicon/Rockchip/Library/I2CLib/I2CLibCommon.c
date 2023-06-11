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

#include <Library/I2CLib.h>
#include <Rk3399/Rk3399.h>

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

	return (void *)&RkI2CDev[BusId];
}

EFI_STATUS
RkI2cLibRuntimeSetup(enum RkI2CBusID BusId)
{

  return EFI_SUCCESS;
}

