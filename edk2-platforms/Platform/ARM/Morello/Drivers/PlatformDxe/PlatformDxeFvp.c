/** @file

  Copyright (c) 2021, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/RamDisk.h>

VOID
InitVirtioDevices (
  VOID
  );

/**
  Entrypoint of Platform Dxe Driver

  @param  ImageHandle[in]       The firmware allocated handle for the EFI image.
  @param  SystemTable[in]       A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The entry point is executed successfully.
**/
EFI_STATUS
EFIAPI
ArmMorelloEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                 Status;
  EFI_RAM_DISK_PROTOCOL      *RamDisk;
  EFI_DEVICE_PATH_PROTOCOL   *DevicePath;

  Status = EFI_SUCCESS;

  InitVirtioDevices ();

  if (FeaturePcdGet (PcdRamDiskSupported)) {
    Status = gBS->LocateProtocol (
                    &gEfiRamDiskProtocolGuid,
                    NULL,
                    (VOID**)&RamDisk
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Couldn't find the RAM Disk protocol %r\n",
        __FUNCTION__, Status));
      return Status;
    }

    Status = RamDisk->Register (
                        (UINTN)PcdGet32 (PcdRamDiskBase),
                        (UINTN)PcdGet32 (PcdRamDiskSize),
                        &gEfiVirtualCdGuid,
                        NULL,
                        &DevicePath
                        );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to register RAM Disk - %r\n",
        __FUNCTION__, Status));
    }
  }

  return Status;
}
