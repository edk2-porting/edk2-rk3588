/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>

#include <Library/PlatformSysCtrlLib.h>
#include "I2CLibInternal.h"

UINTN GetI2cBase (UINT32 Socket, UINT8 Port)
{
  return PlatformGetI2cBase(Socket, Port);
}

EFI_STATUS
I2cLibRuntimeSetup (UINT32 Socket, UINT8 Port)
{
  return EFI_SUCCESS;
}


