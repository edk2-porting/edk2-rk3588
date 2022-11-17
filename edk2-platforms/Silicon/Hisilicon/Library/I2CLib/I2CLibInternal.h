/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _I2C_LIB_INTERNAL_H_
#define _I2C_LIB_INTERNAL_H_

#include <PlatformArch.h>
#include <Library/I2CLib.h>

UINTN GetI2cBase (UINT32 Socket, UINT8 Port);

EFI_STATUS
I2cLibRuntimeSetup (UINT32 Socket, UINT8 Port);


#endif

