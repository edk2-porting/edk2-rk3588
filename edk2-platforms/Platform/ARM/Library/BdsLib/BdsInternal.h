/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __BDS_INTERNAL_H__
#define __BDS_INTERNAL_H__

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BdsLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Guid/GlobalVariable.h>
#include <Guid/FileInfo.h>

#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadFile.h>
#include <Protocol/PxeBaseCode.h>

#include <Uefi.h>

/**
 * Check if the file loader can support this device path.
 *
 * @param DevicePath    EFI Device Path of the image to load.
 *                      This device path generally comes from the boot entry (ie: Boot####).
 * @param Handle        Handle of the driver supporting the device path
 * @param RemainingDevicePath   Part of the EFI Device Path that has not been resolved during
 *                      the Device Path discovery
 */
typedef BOOLEAN (*BDS_FILE_LOADER_SUPPORT) (
  IN EFI_DEVICE_PATH            *DevicePath,
  IN EFI_HANDLE                 Handle,
  IN EFI_DEVICE_PATH            *RemainingDevicePath
  );

/**
 * Function to load an image from a given Device Path for a
 * specific support (FileSystem, TFTP, PXE, ...)
 *
 * @param DevicePath    EFI Device Path of the image to load.
 *                      This device path generally comes from the boot entry (ie: Boot####).
 *                      This path is also defined as 'OUT' as there are some device paths that
 *                      might not be completed such as EFI path for removable device. In these
 *                      cases, it is expected the loader to add \EFI\BOOT\BOOT(ARM|AA64).EFI
 * @param Handle        Handle of the driver supporting the device path
 * @param RemainingDevicePath   Part of the EFI Device Path that has not been resolved during
 *                      the Device Path discovery
 * @param Type          Define where the image should be loaded (see EFI_ALLOCATE_TYPE definition)
 * @param Image         Base Address of the image has been loaded
 * @param ImageSize     Size of the image that has been loaded
 */
typedef EFI_STATUS (*BDS_FILE_LOADER_LOAD_IMAGE) (
  IN OUT EFI_DEVICE_PATH        **DevicePath,
  IN     EFI_HANDLE             Handle,
  IN     EFI_DEVICE_PATH        *RemainingDevicePath,
  IN     EFI_ALLOCATE_TYPE      Type,
  IN OUT EFI_PHYSICAL_ADDRESS*  Image,
  OUT    UINTN                  *ImageSize
  );

typedef struct {
  BDS_FILE_LOADER_SUPPORT     Support;
  BDS_FILE_LOADER_LOAD_IMAGE  LoadImage;
} BDS_FILE_LOADER;

typedef struct _BDS_SYSTEM_MEMORY_RESOURCE {
  LIST_ENTRY                  Link; // This attribute must be the first entry of this structure (to avoid pointer computation)
  EFI_PHYSICAL_ADDRESS        PhysicalStart;
  UINT64                      ResourceLength;
} BDS_SYSTEM_MEMORY_RESOURCE;

typedef struct {
  UINT64  FileSize;
  UINT64  DownloadedNbOfBytes;
  UINT64  LastReportedNbOfBytes;
} BDS_TFTP_CONTEXT;

/**
  Connect all DXE drivers

  @retval EFI_SUCCESS           All drivers have been connected
  @retval EFI_NOT_FOUND         No handles match the search.
  @retval EFI_OUT_OF_RESOURCES  There is not resource pool memory to store the matching results.

**/
EFI_STATUS
BdsConnectAllDrivers (
  VOID
  );

#endif
