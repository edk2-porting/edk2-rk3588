/** @file
 *
 *  Pcf8563RealTimeClockLib initializer for Rockchip platforms.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/RockchipI2cMasterProtocol.h>

STATIC
VOID
EFIAPI
RockchipI2cMasterRegistrationEvent (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *Handles;
  UINTN                         NumHandles;
  ROCKCHIP_I2C_MASTER_PROTOCOL  *RockchipI2cMaster;
  UINTN                         Index;

  Handles = NULL;
  Status  = gBS->LocateHandleBuffer (
                   ByProtocol,
                   &gRockchipI2cMasterProtocolGuid,
                   NULL,
                   &NumHandles,
                   &Handles
                   );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((
        DEBUG_WARN,
        "%a: Failed to locate gRockchipI2cMasterProtocolGuid. Status=%r\n",
        __FUNCTION__,
        Status
        ));
    }

    return;
  }

  for (Index = 0; Index < NumHandles; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gRockchipI2cMasterProtocolGuid,
                    (VOID **)&RockchipI2cMaster
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to get RockchipI2cMaster: %r\n", __FUNCTION__, Status));
      continue;
    }

    if (RockchipI2cMaster->Bus != FixedPcdGet8 (PcdRtc8563Bus)) {
      continue;
    }

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handles[Index],
                    &gPcf8563RealTimeClockLibI2cMasterProtocolGuid,
                    NULL,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Failed to install gPcf8563RealTimeClockLibI2cMasterProtocolGuid: %r\n",
        __FUNCTION__,
        Status
        ));
      continue;
    }

    gBS->CloseEvent (Event);
  }

  FreePool (Handles);
}

EFI_STATUS
EFIAPI
Rtc8563PlatformDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  VOID  *Registration = NULL;

  EfiCreateProtocolNotifyEvent (
    &gRockchipI2cMasterProtocolGuid,
    TPL_CALLBACK,
    RockchipI2cMasterRegistrationEvent,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}
