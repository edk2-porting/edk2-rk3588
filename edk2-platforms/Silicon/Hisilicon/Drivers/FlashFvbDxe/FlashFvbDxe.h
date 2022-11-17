/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef __FLASH_FVB_DXE_H__
#define __FLASH_FVB_DXE_H__


#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Library/DebugLib.h>

#include <Library/IoLib.h>
#include <Library/UefiLib.h>

#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HisiSpiFlashProtocol.h>

#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Guid/VariableFormat.h>
#include <Guid/SystemNvDataGuid.h>


#define FLASH_ERASE_RETRY                     10
#define FLASH_DEVICE_COUNT                     1

// Device access macros
// These are necessary because we use 2 x 16bit parts to make up 32bit data
typedef struct
{
    UINTN       DeviceBaseAddress;    // Start address of the Device Base Address (DBA)
    UINTN       RegionBaseAddress;    // Start address of one single region
    UINTN       Size;
    UINTN       BlockSize;
    EFI_GUID    Guid;
} FLASH_DESCRIPTION;

#define GET_BLOCK_ADDRESS(BaseAddr,Lba,LbaSize)( BaseAddr + (UINTN)((Lba) * LbaSize) )

#define FLASH_SIGNATURE                       SIGNATURE_32('s', 'p', 'i', '0')
#define INSTANCE_FROM_FVB_THIS(a)             CR(a, FLASH_INSTANCE, FvbProtocol, FLASH_SIGNATURE)
#define INSTANCE_FROM_BLKIO_THIS(a)           CR(a, FLASH_INSTANCE, BlockIoProtocol, FLASH_SIGNATURE)

typedef struct _FLASH_INSTANCE                FLASH_INSTANCE;

typedef EFI_STATUS (*FLASH_INITIALIZE)        (FLASH_INSTANCE* Instance);

typedef struct
{
    VENDOR_DEVICE_PATH                  Vendor;
    EFI_DEVICE_PATH_PROTOCOL            End;
} FLASH_DEVICE_PATH;

struct _FLASH_INSTANCE
{
    UINT32                              Signature;
    EFI_HANDLE                          Handle;

    BOOLEAN                             Initialized;
    FLASH_INITIALIZE                    Initialize;

    UINTN                               DeviceBaseAddress;
    UINTN                               RegionBaseAddress;
    UINTN                               Size;
    EFI_LBA                             StartLba;

    EFI_BLOCK_IO_PROTOCOL               BlockIoProtocol;
    EFI_BLOCK_IO_MEDIA                  Media;

    BOOLEAN                             SupportFvb;
    EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL FvbProtocol;

    FLASH_DEVICE_PATH                   DevicePath;
};


//
// BlockIO Protocol function EFI_BLOCK_IO_PROTOCOL.ReadBlocks
//
EFI_STATUS
EFIAPI
FlashBlockIoReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL*   This,
    IN  UINT32                   MediaId,
    IN  EFI_LBA                  Lba,
    IN  UINTN                    BufferSizeInBytes,
    OUT VOID*                    Buffer
);

//
// BlockIO Protocol function EFI_BLOCK_IO_PROTOCOL.WriteBlocks
//
EFI_STATUS
EFIAPI
FlashBlockIoWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL*   This,
    IN  UINT32                   MediaId,
    IN  EFI_LBA                  Lba,
    IN  UINTN                    BufferSizeInBytes,
    IN  VOID*                    Buffer
);

//
// BlockIO Protocol function EFI_BLOCK_IO_PROTOCOL.FlushBlocks
//
EFI_STATUS
EFIAPI
FlashBlockIoFlushBlocks (
    IN EFI_BLOCK_IO_PROTOCOL*    This
);


//
// FvbHw.c
//

EFI_STATUS
EFIAPI
FvbInitialize (
    IN FLASH_INSTANCE*                            Instance
);

EFI_STATUS
EFIAPI
FvbGetAttributes(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    OUT       EFI_FVB_ATTRIBUTES_2*                    Attributes
);

EFI_STATUS
EFIAPI
FvbSetAttributes(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    IN OUT    EFI_FVB_ATTRIBUTES_2*                    Attributes
);

EFI_STATUS
EFIAPI
FvbGetPhysicalAddress(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    OUT       EFI_PHYSICAL_ADDRESS*                    Address
);

EFI_STATUS
EFIAPI
FvbGetBlockSize(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    IN        EFI_LBA                                  Lba,
    OUT       UINTN*                                   BlockSize,
    OUT       UINTN*                                   NumberOfBlocks
);

EFI_STATUS
EFIAPI
FvbRead(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    IN        EFI_LBA                                  Lba,
    IN        UINTN                                   Offset,
    IN OUT    UINTN*                                   NumBytes,
    IN OUT    UINT8*                                   Buffer
);

EFI_STATUS
EFIAPI
FvbWrite(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    IN        EFI_LBA                                  Lba,
    IN        UINTN                                    Offset,
    IN OUT    UINTN*                                   NumBytes,
    IN        UINT8*                                   Buffer
);

EFI_STATUS
EFIAPI
FvbEraseBlocks(
    IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL*     This,
    ...
);

//
// FlashFvbDxe.c
//

EFI_STATUS
FlashUnlockAndEraseSingleBlock (
    IN FLASH_INSTANCE*         Instance,
    IN UINTN                   BlockAddress
);

EFI_STATUS
FlashWriteBlocks (
    IN  FLASH_INSTANCE*    Instance,
    IN  EFI_LBA            Lba,
    IN  UINTN              BufferSizeInBytes,
    IN  VOID*              Buffer
);

EFI_STATUS
FlashReadBlocks (
    IN FLASH_INSTANCE*       Instance,
    IN EFI_LBA               Lba,
    IN UINTN                 BufferSizeInBytes,
    OUT VOID*                Buffer
);

#endif
