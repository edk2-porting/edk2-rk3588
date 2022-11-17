/** @file
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (C) 2015, Red Hat, Inc.
 *  Copyright (c) 2006-2014, Intel Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "VarBlockService.h"

//
// Minimum delay to enact before reset, when variables are dirty (in μs).
// Needed to ensure that SSD-based USB 3.0 devices have time to flush their
// write cache after updating the NV vars. A much smaller delay is applied
// on Pi 3 compared to Pi 4, as we haven't had reports of issues there yet.
//
#if (RPI_MODEL == 3)
#define PLATFORM_RESET_DELAY     500000
#else
#define PLATFORM_RESET_DELAY    3500000
#endif

VOID *mSFSRegistration;


VOID
InstallProtocolInterfaces (
  IN EFI_FW_VOL_BLOCK_DEVICE *FvbDevice
  )
{
  EFI_STATUS Status;
  EFI_HANDLE FwbHandle;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *OldFwbInterface;

  //
  // Find a handle with a matching device path that has supports FW Block
  // protocol.
  //
  Status = gBS->LocateDevicePath (&gEfiFirmwareVolumeBlockProtocolGuid,
                  &FvbDevice->DevicePath, &FwbHandle);
  if (EFI_ERROR (Status)) {
    //
    // LocateDevicePath fails so install a new interface and device path.
    //
    FwbHandle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &FwbHandle,
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    &FvbDevice->FwVolBlockInstance,
                    &gEfiDevicePathProtocolGuid,
                    FvbDevice->DevicePath,
                    NULL
                  );
    ASSERT_EFI_ERROR (Status);
  } else if (IsDevicePathEnd (FvbDevice->DevicePath)) {
    //
    // Device already exists, so reinstall the FVB protocol
    //
    Status = gBS->HandleProtocol (
                    FwbHandle,
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    (VOID**)&OldFwbInterface
                  );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->ReinstallProtocolInterface (
                    FwbHandle,
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    OldFwbInterface,
                    &FvbDevice->FwVolBlockInstance
                  );
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // There was a FVB protocol on an End Device Path node
    //
    ASSERT (FALSE);
  }
}


STATIC
VOID
EFIAPI
FvbVirtualAddressChangeEvent (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
/*++

  Routine Description:

    Fixup internal data so that EFI can be called in virtual mode.

  Arguments:

    (Standard EFI notify event - EFI_EVENT_NOTIFY)

  Returns:

    None

--*/
{
  EfiConvertPointer (0x0, (VOID**)&mFvInstance->FvBase);
  EfiConvertPointer (0x0, (VOID**)&mFvInstance->VolumeHeader);
  EfiConvertPointer (0x0, (VOID**)&mFvInstance);
}


VOID
InstallVirtualAddressChangeHandler (
  VOID
  )
{
  EFI_STATUS Status;
  EFI_EVENT VirtualAddressChangeEvent;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  FvbVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &VirtualAddressChangeEvent
                );
  ASSERT_EFI_ERROR (Status);
}


STATIC
EFI_STATUS
DoDump (
  IN EFI_DEVICE_PATH_PROTOCOL *Device
  )
{
  EFI_STATUS Status;
  EFI_FILE_PROTOCOL *File;

  Status = FileOpen (Device,
             mFvInstance->MappedFile,
             &File,
             EFI_FILE_MODE_WRITE |
             EFI_FILE_MODE_READ);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = FileWrite (File,
             mFvInstance->Offset,
             mFvInstance->FvBase,
             mFvInstance->FvLength);
  FileClose (File);
  return Status;
}


STATIC
VOID
EFIAPI
DumpVars (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  EFI_STATUS Status;
  RETURN_STATUS PcdStatus;

  if (mFvInstance->Device == NULL) {
    DEBUG ((DEBUG_INFO, "Variable store not found?\n"));
    return;
  }

  if (!mFvInstance->Dirty) {
    DEBUG ((DEBUG_INFO, "Variables not dirty, not dumping!\n"));
    return;
  }

  Status = DoDump (mFvInstance->Device);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Couldn't dump '%s'\n", mFvInstance->MappedFile));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  DEBUG ((DEBUG_INFO, "Variables dumped!\n"));

  //
  // Add a reset delay to give time for slow/cached devices
  // to flush the NV variables write to permanent storage.
  // But only do so if this won't reduce an existing user-set delay.
  //
  if (PcdGet32 (PcdPlatformResetDelay) < PLATFORM_RESET_DELAY) {
    PcdStatus = PcdSet32S (PcdPlatformResetDelay, PLATFORM_RESET_DELAY);
    ASSERT_RETURN_ERROR (PcdStatus);
  }

  mFvInstance->Dirty = FALSE;
}


VOID
ReadyToBootHandler (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  EFI_STATUS Status;
  EFI_EVENT ImageInstallEvent;
  VOID *ImageRegistration;

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  DumpVars,
                  NULL,
                  &ImageInstallEvent
                );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (
                  &gEfiLoadedImageProtocolGuid,
                  ImageInstallEvent,
                  &ImageRegistration
                );
  ASSERT_EFI_ERROR (Status);

  DumpVars (NULL, NULL);
  Status = gBS->CloseEvent (Event);
  ASSERT_EFI_ERROR (Status);
}


VOID
InstallDumpVarEventHandlers (
  VOID
  )
{
  EFI_STATUS Status;
  EFI_EVENT ResetEvent;
  EFI_EVENT ReadyToBootEvent;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  DumpVars,
                  NULL,
                  &gRaspberryPiEventResetGuid,
                  &ResetEvent
                );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ReadyToBootHandler,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                );
  ASSERT_EFI_ERROR (Status);
}


VOID
EFIAPI
OnSimpleFileSystemInstall (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  EFI_STATUS Status;
  UINTN HandleSize;
  EFI_HANDLE Handle;
  EFI_DEVICE_PATH_PROTOCOL *Device;

  if ((mFvInstance->Device != NULL) &&
      !EFI_ERROR (CheckStoreExists (mFvInstance->Device))) {
    //
    // We've already found the variable store before,
    // and that device is not removed from the ssystem.
    //
    return;
  }

  while (TRUE) {
    HandleSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mSFSRegistration,
                    &HandleSize,
                    &Handle
                  );
    if (Status == EFI_NOT_FOUND) {
      break;
    }

    ASSERT_EFI_ERROR (Status);

    Status = CheckStore (Handle, &Device);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = DoDump (Device);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Couldn't update '%s'\n", mFvInstance->MappedFile));
      ASSERT_EFI_ERROR (Status);
      continue;
    }

    if (mFvInstance->Device != NULL) {
      gBS->FreePool (mFvInstance->Device);
    }

    DEBUG ((DEBUG_INFO, "Found variable store!\n"));
    mFvInstance->Device = Device;
    break;
  }
}


VOID
InstallFSNotifyHandler (
  VOID
  )
{
  EFI_STATUS Status;
  EFI_EVENT Event;

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnSimpleFileSystemInstall,
                  NULL,
                  &Event
                );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (
                  &gEfiSimpleFileSystemProtocolGuid,
                  Event,
                  &mSFSRegistration
                );
  ASSERT_EFI_ERROR (Status);
}
