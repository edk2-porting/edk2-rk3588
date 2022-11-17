/** @file
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2007-2009, Intel Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "VarBlockService.h"


EFI_STATUS
FileWrite (
  IN EFI_FILE_PROTOCOL *File,
  IN UINTN Offset,
  IN UINTN Buffer,
  IN UINTN Size
  )
{
  EFI_STATUS Status;

  Status = File->SetPosition (File, Offset);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = File->Write (File, &Size, (VOID*)Buffer);
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}


VOID
FileClose (
  IN  EFI_FILE_PROTOCOL *File
  )
{
  File->Flush (File);
  File->Close (File);
}


EFI_STATUS
FileOpen (
  IN  EFI_DEVICE_PATH_PROTOCOL *Device,
  IN  CHAR16 *MappedFile,
  OUT EFI_FILE_PROTOCOL **File,
  IN  UINT64 OpenMode
  )
{
  EFI_HANDLE                        Handle;
  EFI_FILE_HANDLE                   Root;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Volume;
  EFI_STATUS                        Status;

  *File = NULL;

  Status = gBS->LocateDevicePath (
                  &gEfiSimpleFileSystemProtocolGuid,
                  &Device,
                  &Handle
                );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID**)&Volume
                );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Open the root directory of the volume
  //
  Root = NULL;
  Status = Volume->OpenVolume (
                     Volume,
                     &Root
                   );
  ASSERT_EFI_ERROR (Status);
  ASSERT (Root != NULL);

  //
  // Open file
  //
  Status = Root->Open (
                   Root,
                   File,
                   MappedFile,
                   OpenMode,
                   0
                 );
  if (EFI_ERROR (Status)) {
    *File = NULL;
  }

  //
  // Close the Root directory
  //
  Root->Close (Root);
  return Status;
}


EFI_STATUS
CheckStore (
  IN  EFI_HANDLE SimpleFileSystemHandle,
  OUT EFI_DEVICE_PATH_PROTOCOL **Device
  )
{
  EFI_STATUS Status;
  EFI_BLOCK_IO_PROTOCOL *BlkIo;
  EFI_FILE_PROTOCOL *File;

  *Device = NULL;
  Status = gBS->HandleProtocol (
                  SimpleFileSystemHandle,
                  &gEfiBlockIoProtocolGuid,
                  (VOID*)&BlkIo
                );

  if (EFI_ERROR (Status)) {
    goto ErrHandle;
  }
  if (!BlkIo->Media->MediaPresent) {
    DEBUG ((DEBUG_ERROR, "FwhMappedFile: Media not present!\n"));
    Status = EFI_NO_MEDIA;
    goto ErrHandle;
  }
  if (BlkIo->Media->ReadOnly) {
    DEBUG ((DEBUG_ERROR, "FwhMappedFile: Media is read-only!\n"));
    Status = EFI_ACCESS_DENIED;
    goto ErrHandle;
  }

  Status = FileOpen (DevicePathFromHandle (SimpleFileSystemHandle),
             mFvInstance->MappedFile, &File,
             EFI_FILE_MODE_READ);
  if (EFI_ERROR (Status)) {
    goto ErrHandle;
  }

  /* We found it! Maybe do more checks...? */

  FileClose (File);
  *Device = DuplicateDevicePath (DevicePathFromHandle (SimpleFileSystemHandle));

  ASSERT (*Device != NULL);

ErrHandle:
  return Status;
}


EFI_STATUS
CheckStoreExists (
  IN  EFI_DEVICE_PATH_PROTOCOL *Device
  )
{
  EFI_HANDLE Handle;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Volume;
  EFI_STATUS Status;

  Status = gBS->LocateDevicePath (
                  &gEfiSimpleFileSystemProtocolGuid,
                  &Device,
                  &Handle
                );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID**)&Volume
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
