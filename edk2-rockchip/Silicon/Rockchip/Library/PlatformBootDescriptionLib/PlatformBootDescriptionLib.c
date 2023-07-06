/** @file
 *
 *  Boot Manager device description handler.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/RkAtagsLib.h>
#include <Protocol/BlockIo.h>

RKATAG_BOOTDEV_TYPE  mBootDeviceType;

CHAR16               mBootDescNotConnectedSuffix[]  = L" (Not Connected)";
CHAR16               mBootDescFirmwareSuffix[]      = L" [Firmware]";

STATIC
CHAR16*
EFIAPI
PlatformBootDescriptionHandler (
  IN EFI_HANDLE    Handle,
  IN CONST CHAR16  *DefaultDescription
  )
{
  EFI_STATUS              Status;
  EFI_DEV_PATH            *Device;
  CHAR16                  *Description;
  UINTN                   MaxDescriptionSize;
  BOOLEAN                 IsBootDevice;
  EFI_BLOCK_IO_PROTOCOL   *BlkIo;
  BOOLEAN                 IsMediaPresent;

  Device = (EFI_DEV_PATH *) DevicePathFromHandle (Handle);

  if (Device->DevPath.Type != HARDWARE_DEVICE_PATH || Device->DevPath.SubType != HW_MEMMAP_DP) {
    return NULL;
  }

  Description = NULL;
  IsBootDevice = FALSE;

  if (Device->MemMap.StartingAddress == PcdGet32 (PcdDwcSdhciBaseAddress)) {
    Description = L"UEFI eMMC Device";
    IsBootDevice = mBootDeviceType == RkAtagBootDevTypeEmmc;
  } else if (Device->MemMap.StartingAddress == PcdGet32 (PcdRkSdmmcBaseAddress)) {
    Description = L"UEFI SD Device";
    IsBootDevice = mBootDeviceType == RkAtagBootDevTypeSd0;
  } else {
    return NULL;
  }

  MaxDescriptionSize = StrSize (Description) - sizeof (CHAR16)
                       + StrSize (mBootDescNotConnectedSuffix) - sizeof (CHAR16)
                       + StrSize (mBootDescFirmwareSuffix);

  IsMediaPresent = FALSE;
  Status = gBS->HandleProtocol (Handle, &gEfiBlockIoProtocolGuid, (VOID *) &BlkIo);
  if (!EFI_ERROR (Status)) {
    IsMediaPresent = BlkIo->Media->MediaPresent;
  }

  Description = AllocateCopyPool (MaxDescriptionSize, Description);
  if (Description == NULL) {
    return NULL;
  }

  if (!IsMediaPresent) {
    StrCatS (Description, MaxDescriptionSize / sizeof (CHAR16), mBootDescNotConnectedSuffix);
  }
  if (IsBootDevice) {
    StrCatS (Description, MaxDescriptionSize / sizeof (CHAR16), mBootDescFirmwareSuffix);
  }

  return Description;
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
