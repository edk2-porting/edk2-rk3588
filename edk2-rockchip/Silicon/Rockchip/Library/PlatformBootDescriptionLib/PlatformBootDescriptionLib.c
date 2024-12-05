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
#include <Library/RkAtagsLib.h>
#include <Protocol/NonDiscoverableDevice.h>

RKATAG_BOOTDEV_TYPE  mBootDeviceType;

CHAR16               mBootDescFirmwareSuffix[]       = L" [Fw]";

STATIC
NON_DISCOVERABLE_DEVICE *
GetNonDiscoverableDevice (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 DeviceHandle;
  NON_DISCOVERABLE_DEVICE    *Device;
  EFI_DEVICE_PATH_PROTOCOL   *CurrentDevicePath;

  CurrentDevicePath = DevicePath;

  if (DevicePath->Type != HARDWARE_DEVICE_PATH
      || DevicePath->SubType != HW_VENDOR_DP) {
    return NULL;
  }

  Status = gBS->LocateDevicePath (&gEdkiiNonDiscoverableDeviceProtocolGuid,
                   &CurrentDevicePath, &DeviceHandle);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = gBS->HandleProtocol (DeviceHandle,
                   &gEdkiiNonDiscoverableDeviceProtocolGuid, (VOID **) &Device);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return Device;
}

STATIC
BOOLEAN
CheckIsBootDevice (
  IN NON_DISCOVERABLE_DEVICE *Device
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptor;
  Descriptor = &Device->Resources[0];

  if (Descriptor->Desc != ACPI_ADDRESS_SPACE_DESCRIPTOR ||
      Descriptor->ResType != ACPI_ADDRESS_SPACE_TYPE_MEM) {
    return FALSE;
  }

  if (mBootDeviceType == RkAtagBootDevTypeEmmc) {
    return Descriptor->AddrRangeMin == PcdGet32 (PcdDwcSdhciBaseAddress);
  }
  if (mBootDeviceType == RkAtagBootDevTypeSd0) {
    return Descriptor->AddrRangeMin == PcdGet32 (PcdRkSdmmcBaseAddress);
  }

  return FALSE;
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
  NON_DISCOVERABLE_DEVICE   *NonDiscoverableDevice;
  CHAR16                    *Desc;
  UINTN                     DescSize;

  DevicePath = DevicePathFromHandle (Handle);

  NonDiscoverableDevice = GetNonDiscoverableDevice (DevicePath);
  if (NonDiscoverableDevice == NULL) {
    return NULL;
  }

  if (!CheckIsBootDevice (NonDiscoverableDevice)) {
    return NULL;
  }

  DevicePath = NextDevicePathNode (DevicePath);

  if (DevicePath->SubType == MSG_EMMC_DP) {
    DevicePath = NextDevicePathNode (DevicePath);
    if (DevicePath->SubType == HW_CONTROLLER_DP) {
      if (((CONTROLLER_DEVICE_PATH *) DevicePath)->ControllerNumber != EmmcPartitionUserData) {
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
              + StrSize (mBootDescFirmwareSuffix) - sizeof (CHAR16)   // " [Fw]"
              + sizeof (CHAR16);                                      // '\0'

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
  RKATAG_BOOTDEV *BootDevice;

  BootDevice = RkAtagsGetBootDev ();
  mBootDeviceType = BootDevice != NULL ? BootDevice->DevType : RkAtagBootDevTypeUnknown;

  return EfiBootManagerRegisterBootDescriptionHandler (PlatformBootDescriptionHandler);
}
