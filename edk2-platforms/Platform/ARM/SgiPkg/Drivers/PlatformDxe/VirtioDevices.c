/** @file

  Copyright (c) 2018-2020, Arm Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Guid/SgiVirtioDevicesFormSet.h>
#include <Library/VirtioMmioDeviceLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <SgiPlatform.h>

#pragma pack (1)
typedef struct {
  VENDOR_DEVICE_PATH                  VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL            End;
} VIRTIO_DEVICE_PATH;
#pragma pack ()

STATIC VIRTIO_DEVICE_PATH mVirtioBlockDevicePath =
{
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SGI_VIRTIO_BLOCK_GUID,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC VIRTIO_DEVICE_PATH mVirtioNetDevicePath =
{
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SGI_VIRTIO_NET_GUID,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

/**
  Initialize platform Virtio devices.

  @return None.
**/
VOID
InitVirtioDevices (
  VOID
  )
{
  EFI_STATUS Status;
  STATIC EFI_HANDLE mVirtIoBlkController = NULL;
  STATIC EFI_HANDLE mVirtIoNetController = NULL;

  // Install protocol interface for storage device
  if ((FeaturePcdGet (PcdVirtioBlkSupported)) &&
      (FixedPcdGet32 (PcdVirtioBlkBaseAddress) != 0)) {
    Status = gBS->InstallProtocolInterface (&mVirtIoBlkController,
                    &gEfiDevicePathProtocolGuid, EFI_NATIVE_INTERFACE,
                    &mVirtioBlockDevicePath);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to install EFI_DEVICE_PATH protocol "
        "for Virtio Block device (Status = %r)\n",
        __FUNCTION__, Status));
    } else {
      // Declare the Virtio BlockIo device
      Status = VirtioMmioInstallDevice (FixedPcdGet32 (PcdVirtioBlkBaseAddress),
                 mVirtIoBlkController);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: Unable to find Virtio Block MMIO device "
          "(Status == %r)\n", __FUNCTION__, Status));
        gBS->UninstallProtocolInterface (
               mVirtIoBlkController,
               &gEfiDevicePathProtocolGuid,
               &mVirtioBlockDevicePath
             );
      } else {
        DEBUG ((DEBUG_INIT, "%a: Installed Virtio Block device\n",
          __FUNCTION__));
      }
    }
  }

  // Install protocol interface for network device
  if ((FeaturePcdGet (PcdVirtioNetSupported)) &&
      (FixedPcdGet32 (PcdVirtioNetBaseAddress) != 0)) {
    Status = gBS->InstallProtocolInterface (&mVirtIoNetController,
                    &gEfiDevicePathProtocolGuid, EFI_NATIVE_INTERFACE,
                    &mVirtioNetDevicePath);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to install EFI_DEVICE_PATH protocol "
        "for Virtio Network device (Status = %r)\n",
        __FUNCTION__, Status));
    } else {
      // Declare the Virtio Net device
      Status = VirtioMmioInstallDevice (FixedPcdGet32 (PcdVirtioNetBaseAddress),
                 mVirtIoNetController);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: Unable to find Virtio Network MMIO device "
          "(Status == %r)\n",
          __FUNCTION__, Status));
        gBS->UninstallProtocolInterface (
               mVirtIoNetController,
               &gEfiDevicePathProtocolGuid,
               &mVirtioNetDevicePath
            );
      } else {
        DEBUG ((DEBUG_INIT, "%a: Installed Virtio Network device\n",
          __FUNCTION__));
      }
    }
  }
}
