/** @file
 *
 *  Copyright (c) 2019, Andrey Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Protocol/DevicePath.h>

#define ARASAN_MMC_DXE_FILE_GUID  \
  { 0x100c2cfa, 0xb586, 0x4198, { 0x9b, 0x4c, 0x16, 0x83, 0xd1, 0x95, 0xb1, 0xda } }

#define SDHOST_MMC_DXE_FILE_GUID  \
  { 0x58abd787, 0xf64d, 0x4ca2, { 0xa0, 0x34, 0xb9, 0xac, 0x2d, 0x5a, 0xd0, 0xcf } }

#define DP_NODE_LEN(Type) { (UINT8)sizeof (Type), (UINT8)(sizeof (Type) >> 8) }

typedef struct {
  VENDOR_DEVICE_PATH       Custom;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} PLATFORM_SD_DEV;

STATIC PLATFORM_SD_DEV mArasan = {
  //
  // VENDOR_DEVICE_PATH ArasanMMCHostDxe
  //
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, DP_NODE_LEN (VENDOR_DEVICE_PATH) },
    ARASAN_MMC_DXE_FILE_GUID
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};

STATIC PLATFORM_SD_DEV mSDHost = {
  //
  // VENDOR_DEVICE_PATH SdHostDxe
  //
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, DP_NODE_LEN (VENDOR_DEVICE_PATH) },
    SDHOST_MMC_DXE_FILE_GUID
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};

STATIC
CHAR16*
EFIAPI
BootDescriptionHandler (
  IN EFI_HANDLE Handle,
  IN CONST CHAR16 *DefaultDescription
  )
{
  CHAR16 *Name;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;

  DevicePath = DevicePathFromHandle (Handle);
  if (CompareMem (&mArasan, DevicePath, GetDevicePathSize (DevicePath)) == 0) {
    Name = L"SD/MMC on Arasan SDHCI";
  } else if (CompareMem (&mSDHost, DevicePath, GetDevicePathSize (DevicePath)) == 0) {
    Name = L"SD/MMC on Broadcom SDHOST";
  } else {
    return NULL;
  }

  return AllocateCopyPool(StrSize (Name), Name);
}

EFI_STATUS
EFIAPI
PlatformUiAppLibConstructor (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  Status = EfiBootManagerRegisterBootDescriptionHandler(BootDescriptionHandler);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PlatformUiAppLibDestructor (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  return EFI_SUCCESS;
}
