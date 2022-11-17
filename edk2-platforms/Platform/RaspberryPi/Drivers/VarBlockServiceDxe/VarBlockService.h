/** @file
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2007-2009, Intel Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef _FW_BLOCK_SERVICE_H
#define _FW_BLOCK_SERVICE_H

#include <Guid/EventGroup.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Protocol/LoadedImage.h>

typedef struct {
  union {
    UINTN                      FvBase;
    EFI_FIRMWARE_VOLUME_HEADER *VolumeHeader;
  };
  UINTN                      FvLength;
  UINTN                      Offset;
  UINTN                      NumOfBlocks;
  EFI_DEVICE_PATH_PROTOCOL   *Device;
  CHAR16                     *MappedFile;
  BOOLEAN                    Dirty;
} EFI_FW_VOL_INSTANCE;

extern EFI_FW_VOL_INSTANCE *mFvInstance;

typedef struct {
  MEDIA_FW_VOL_DEVICE_PATH  FvDevPath;
  EFI_DEVICE_PATH_PROTOCOL  EndDevPath;
} FV_PIWG_DEVICE_PATH;

typedef struct {
  MEMMAP_DEVICE_PATH          MemMapDevPath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevPath;
} FV_MEMMAP_DEVICE_PATH;

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  FwVolBlockInstance;
} EFI_FW_VOL_BLOCK_DEVICE;

EFI_STATUS
GetFvbInfo (
  IN  UINT64                            FvLength,
  OUT EFI_FIRMWARE_VOLUME_HEADER        **FvbInfo
  );

EFI_STATUS
FvbSetVolumeAttributes (
  IN OUT EFI_FVB_ATTRIBUTES_2 *Attributes
  );

EFI_STATUS
FvbGetVolumeAttributes (
  OUT EFI_FVB_ATTRIBUTES_2 *Attributes
  );

EFI_STATUS
FvbGetPhysicalAddress (
  OUT EFI_PHYSICAL_ADDRESS *Address
  );

EFI_STATUS
EFIAPI
FvbInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );


VOID
EFIAPI
FvbClassAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

EFI_STATUS
FvbGetLbaAddress (
  IN  EFI_LBA Lba,
  OUT UINTN   *LbaAddress,
  OUT UINTN   *LbaLength,
  OUT UINTN   *NumOfBlocks
  );

//
// Protocol APIs
//
EFI_STATUS
EFIAPI
FvbProtocolGetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL           *This,
  OUT EFI_FVB_ATTRIBUTES_2                              *Attributes
  );

EFI_STATUS
EFIAPI
FvbProtocolSetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL           *This,
  IN OUT EFI_FVB_ATTRIBUTES_2                           *Attributes
  );

EFI_STATUS
EFIAPI
FvbProtocolGetPhysicalAddress (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL           *This,
  OUT EFI_PHYSICAL_ADDRESS                        *Address
  );

EFI_STATUS
EFIAPI
FvbProtocolGetBlockSize (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL           *This,
  IN CONST EFI_LBA                                     Lba,
  OUT UINTN                                       *BlockSize,
  OUT UINTN                                       *NumOfBlocks
  );

EFI_STATUS
EFIAPI
FvbProtocolRead (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL           *This,
  IN CONST EFI_LBA                                      Lba,
  IN CONST UINTN                                        Offset,
  IN OUT UINTN                                    *NumBytes,
  IN UINT8                                        *Buffer
  );

EFI_STATUS
EFIAPI
FvbProtocolWrite (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL           *This,
  IN       EFI_LBA                                      Lba,
  IN       UINTN                                        Offset,
  IN OUT   UINTN                                        *NumBytes,
  IN       UINT8                                        *Buffer
  );

EFI_STATUS
EFIAPI
FvbProtocolEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *This,
  ...
  );

VOID
InstallProtocolInterfaces (
  IN EFI_FW_VOL_BLOCK_DEVICE *FvbDevice
  );

VOID
InstallVirtualAddressChangeHandler (
  VOID
  );

VOID
InstallFSNotifyHandler (
  VOID
  );

VOID
InstallDumpVarEventHandlers (
  VOID
);

EFI_STATUS
FileWrite (
  IN EFI_FILE_PROTOCOL *File,
  IN UINTN             Offset,
  IN UINTN             Buffer,
  IN UINTN             Size
  );

EFI_STATUS
CheckStore (
  IN  EFI_HANDLE SimpleFileSystemHandle,
  OUT EFI_DEVICE_PATH_PROTOCOL **Device
  );

EFI_STATUS
CheckStoreExists (
  IN  EFI_DEVICE_PATH_PROTOCOL *Device
  );

EFI_STATUS
FileOpen (
  IN  EFI_DEVICE_PATH_PROTOCOL *Device,
  IN  CHAR16 *MappedFile,
  OUT EFI_FILE_PROTOCOL **File,
  IN  UINT64 OpenMode
  );

VOID
FileClose (
  IN  EFI_FILE_PROTOCOL *File
  );

#endif
