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
#include <Library/UefiRuntimeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Guid/EventGroup.h>

#include <Library/PlatformSysCtrlLib.h>
#include "I2CLibInternal.h"

STATIC EFI_EVENT              mI2cLibVirtualAddrChangeEvent;

STATIC UINTN gI2cBase[MAX_SOCKET][I2C_PORT_MAX];

UINTN GetI2cBase (UINT32 Socket, UINT8 Port)
{
  if (gI2cBase[Socket][Port] == 0) {
    gI2cBase[Socket][Port] = PlatformGetI2cBase(Socket, Port);
  }

  return gI2cBase[Socket][Port];
}

VOID
EFIAPI
I2cLibVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINT32 Socket;
  UINT8  Port;

  // We assume that all I2C ports used in one runtime driver need to be
  // converted into virtual address.
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Port = 0; Port < I2C_PORT_MAX; Port++) {
      if (gI2cBase[Socket][Port] != 0) {
        EfiConvertPointer (0x0, (VOID **)&gI2cBase[Socket][Port]);
      }
    }
  }

  return;
}

EFI_STATUS
I2cLibRuntimeSetup (UINT32 Socket, UINT8 Port)
{
  EFI_STATUS Status;

  UINTN Base = GetI2cBase (Socket, Port);

  // Declare the controller as EFI_MEMORY_RUNTIME
  Status = gDS->AddMemorySpace (
      EfiGcdMemoryTypeMemoryMappedIo,
      Base, SIZE_64KB,
      EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
      );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "[%a:%d] AddMemorySpace failed: %r\n", __FUNCTION__, __LINE__, Status));
  }

  Status = gDS->SetMemorySpaceAttributes (Base, SIZE_64KB, EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
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
        I2cLibVirtualNotifyEvent,
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


