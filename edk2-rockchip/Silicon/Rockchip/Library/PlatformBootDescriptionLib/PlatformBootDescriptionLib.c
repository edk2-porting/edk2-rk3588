/** @file
 *
 *  Boot Manager device description handler.
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Emmc.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>

CHAR16  mBootDescFirmwareSuffix[] = L" [Fw]";

STATIC
BOOLEAN
CheckIsBootDevice (
  IN EFI_HANDLE  Handle
  )
{
  EFI_STATUS  Status;
  VOID        *Instance;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gRockchipFirmwareBootDeviceProtocolGuid,
                  &Instance
                  );

  return !EFI_ERROR (Status);
}

STATIC
CHAR16 *
EFIAPI
PlatformBootDescriptionHandler (
  IN EFI_HANDLE    Handle,
  IN CONST CHAR16  *DefaultDescription
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  CHAR16                    *Desc;
  UINTN                     DescSize;

  if (!CheckIsBootDevice (Handle)) {
    return NULL;
  }

  DevicePath = DevicePathFromHandle (Handle);
  DevicePath = NextDevicePathNode (DevicePath);

  if (DevicePath->SubType == MSG_EMMC_DP) {
    DevicePath = NextDevicePathNode (DevicePath);
    if (DevicePath->SubType == HW_CONTROLLER_DP) {
      if (((CONTROLLER_DEVICE_PATH *)DevicePath)->ControllerNumber != EmmcPartitionUserData) {
        //
        // There can only be one boot device.
        // eMMC has multiple partitions, but I don't know if it's possible
        // to load UEFI from anything other than UserData.
        // If it turns out to be a possibility, we'd need to create a protocol
        // for RkFvbDxe that would allow getting the Device Path of the block
        // device that holds the NVRAM.
        //
        return NULL;
      }
    }
  } else if (DevicePath->SubType == MSG_SD_DP) {
  } else {
    return NULL;
  }

  DescSize = StrSize (DefaultDescription) - sizeof (CHAR16)           // "UEFI ..."
             + StrSize (mBootDescFirmwareSuffix) - sizeof (CHAR16)    // " [Fw]"
             + sizeof (CHAR16);                                       // '\0'

  Desc = AllocateCopyPool (DescSize, DefaultDescription);
  if (Desc == NULL) {
    return NULL;
  }

  StrCatS (Desc, DescSize / sizeof (CHAR16), mBootDescFirmwareSuffix);

  return Desc;
}

EFI_STATUS
EFIAPI
PlatformBootDescriptionLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EfiBootManagerRegisterBootDescriptionHandler (PlatformBootDescriptionHandler);
}
