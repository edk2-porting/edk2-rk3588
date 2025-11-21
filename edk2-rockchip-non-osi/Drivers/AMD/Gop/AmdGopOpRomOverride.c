/** @file
 *
 *  AMD GOP OpROM override
 *
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Protocol/DriverFamilyOverride.h>
#include <Protocol/LoadedImage.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

STATIC CONST EFI_GUID  mAmdGopDriverGuids[] = {
  { 0x62c34cea, 0xa08d, 0x4676, { 0x81, 0xe5, 0xf1, 0x6b, 0x0e, 0xe2, 0x2f, 0x22 }
  }, // AmdGopPreSoc15Dxe.inf
  { 0x92f10585, 0xfb63, 0x430f, { 0x81, 0x88, 0x71, 0xfe, 0xeb, 0x2d, 0xda, 0x5b }
  }, // AmdGopPostSoc15Dxe.inf
};

STATIC
UINT32
AmdGopDriverFamilyOverrideGetVersion (
  IN EFI_DRIVER_FAMILY_OVERRIDE_PROTOCOL  *This
  )
{
  return MAX_UINT32;
}

STATIC EFI_DRIVER_FAMILY_OVERRIDE_PROTOCOL  mAmdGopDriverFamilyOverride = {
  AmdGopDriverFamilyOverrideGetVersion
};

STATIC
EFI_STATUS
EFIAPI
AmdGopUnloadImage (
  IN  EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS                 Status;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;
  EFI_IMAGE_UNLOAD           OriginalUnload;

  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **)&LoadedImage
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiCallerIdGuid,
                  (VOID **)&OriginalUnload
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  gBS->UninstallMultipleProtocolInterfaces (
         ImageHandle,
         &gEfiDriverFamilyOverrideProtocolGuid,
         &mAmdGopDriverFamilyOverride,
         &gEfiCallerIdGuid,
         OriginalUnload,
         NULL
         );

  LoadedImage->Unload = OriginalUnload;

  return LoadedImage->Unload (ImageHandle);
}

STATIC
EFI_GUID *
GetLoadedImageGuid (
  IN EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevPathNode;
  EFI_GUID                  *NameGuid;

  if ((LoadedImage == NULL) || (LoadedImage->FilePath == NULL)) {
    return NULL;
  }

  DevPathNode = LoadedImage->FilePath;

  while (!IsDevicePathEnd (DevPathNode)) {
    NameGuid = EfiGetNameGuidFromFwVolDevicePathNode (
                 (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *)DevPathNode
                 );
    if (NameGuid != NULL) {
      return NameGuid;
    }

    DevPathNode = NextDevicePathNode (DevPathNode);
  }

  return NULL;
}

STATIC VOID  *mDriverBindingEventRegistration;

STATIC
VOID
EFIAPI
NotifyDriverBinding (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                 Status;
  UINTN                      BufferSize;
  EFI_HANDLE                 Handle;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;
  EFI_GUID                   *LoadedImageGuid;
  UINTN                      Index;

  while (TRUE) {
    BufferSize = sizeof (EFI_HANDLE);
    Status     = gBS->LocateHandle (
                        ByRegisterNotify,
                        NULL,
                        mDriverBindingEventRegistration,
                        &BufferSize,
                        &Handle
                        );
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        ASSERT_EFI_ERROR (Status);
      }

      break;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiLoadedImageProtocolGuid,
                    (VOID **)&LoadedImage
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    LoadedImageGuid = GetLoadedImageGuid (LoadedImage);
    if (LoadedImageGuid == NULL) {
      continue;
    }

    for (Index = 0; Index < ARRAY_SIZE (mAmdGopDriverGuids); Index++) {
      if (!CompareGuid (LoadedImageGuid, &mAmdGopDriverGuids[Index])) {
        continue;
      }

      DEBUG ((DEBUG_INFO, "%a: %g\n", gEfiCallerBaseName, LoadedImageGuid));

      Status = gBS->InstallMultipleProtocolInterfaces (
                      &Handle,
                      &gEfiDriverFamilyOverrideProtocolGuid,
                      &mAmdGopDriverFamilyOverride,
                      NULL
                      );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        break;
      }

      if (LoadedImage->Unload != NULL) {
        Status = gBS->InstallMultipleProtocolInterfaces (
                        &Handle,
                        &gEfiCallerIdGuid,
                        LoadedImage->Unload,
                        NULL
                        );
        if (EFI_ERROR (Status)) {
          ASSERT_EFI_ERROR (Status);
          break;
        }

        LoadedImage->Unload = AmdGopUnloadImage;
      }
    }
  }
}

EFI_STATUS
EFIAPI
AmdGopOpRomOverrideInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EfiCreateProtocolNotifyEvent (
    &gEfiDriverBindingProtocolGuid,
    TPL_CALLBACK,
    NotifyDriverBinding,
    NULL,
    &mDriverBindingEventRegistration
    );

  return EFI_SUCCESS;
}
