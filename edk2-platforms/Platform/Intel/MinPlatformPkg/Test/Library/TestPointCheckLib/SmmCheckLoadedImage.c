/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiSmm.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>

VOID
DumpLoadedImage (
  IN UINTN                                  Index,
  IN EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage,
  IN EFI_DEVICE_PATH_PROTOCOL               *DevicePath,
  IN EFI_DEVICE_PATH_PROTOCOL               *LoadedImageDevicePath
  );

VOID
TestPointDumpSmmLoadedImage (
  VOID
  )
{
  EFI_STATUS                             Status;
  EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage;
  UINTN                                  Index;
  UINTN                                  HandleBufSize;
  EFI_HANDLE                             *HandleBuf;
  UINTN                                  HandleCount;
  EFI_DEVICE_PATH_PROTOCOL               *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL               *LoadedImageDevicePath;
  
  DEBUG ((DEBUG_INFO, "==== TestPointDumpSmmLoadedImage - Enter\n"));
  HandleBuf = NULL;
  HandleBufSize = 0;
  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &HandleBufSize,
                    HandleBuf
                    );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    goto Done ;
  }
  HandleBuf = AllocateZeroPool (HandleBufSize);
  if (HandleBuf == NULL) {
    goto Done ;
  }
  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &HandleBufSize,
                    HandleBuf
                    );
  if (EFI_ERROR (Status)) {
    goto Done ;
  }
  HandleCount = HandleBufSize / sizeof(EFI_HANDLE);
  
  DEBUG ((DEBUG_INFO, "SmmLoadedImage (%d):\n", HandleCount));
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gSmst->SmmHandleProtocol (
                      HandleBuf[Index],
                      &gEfiLoadedImageProtocolGuid,
                      (VOID **)&LoadedImage
                      );
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = gSmst->SmmHandleProtocol (LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath);
    if (EFI_ERROR(Status)) {
      DevicePath = NULL;
    }

    Status = gSmst->SmmHandleProtocol (HandleBuf[Index], &gEfiLoadedImageDevicePathProtocolGuid, (VOID **)&LoadedImageDevicePath);
    if (EFI_ERROR(Status)) {
      LoadedImageDevicePath = NULL;
    }

    DumpLoadedImage (Index, LoadedImage, DevicePath, LoadedImageDevicePath);
  }

Done:

  if (HandleBuf != NULL) {
    FreePool (HandleBuf);
  }

  DEBUG ((DEBUG_INFO, "==== TestPointDumpSmmLoadedImage - Exit\n"));

  return ;
}
