/*++ @file  MvFvbDxe.c

 Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
 Copyright (c) 2017 Marvell International Ltd.<BR>

 SPDX-License-Identifier: BSD-2-Clause-Patent

 --*/

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Guid/NvVarStoreFormatted.h>
#include <Guid/SystemNvDataGuid.h>
#include <Guid/VariableFormat.h>

#include "MvFvbDxe.h"

STATIC EFI_EVENT     mFvbVirtualAddrChangeEvent;
STATIC FVB_DEVICE    *mFvbDevice;

STATIC CONST FVB_DEVICE mMvFvbFlashInstanceTemplate = {
  {
    0,    // SpiFlash Chip Select ... NEED TO BE FILLED
    0,    // SpiFlash Maximum Frequency ... NEED TO BE FILLED
    0,    // SpiFlash Transfer Mode ... NEED TO BE FILLED
    0,    // SpiFlash Address Size ... NEED TO BE FILLED
    NULL, // SpiFlash detailed information ... NEED TO BE FILLED
    0,    // HostRegisterBaseAddress ... NEED TO BE FILLED
    0,    // CoreClock ... NEED TO BE FILLED
  }, // SpiDevice

  NULL, // SpiFlashProtocol ... NEED TO BE FILLED
  NULL, // SpiMasterProtocol ... NEED TO BE FILLED
  NULL, // Handle ... NEED TO BE FILLED

  FVB_FLASH_SIGNATURE, // Signature

  FALSE, // IsMemoryMapped ... NEED TO BE FILLED
  0, // DeviceBaseAddress ... NEED TO BE FILLED
  0, // RegionBaseAddress ... NEED TO BE FILLED
  SIZE_256KB, // Size
  0, // FvbOffset ... NEED TO BE FILLED
  0, // FvbSize ... NEED TO BE FILLED
  0, // StartLba

  {
    0,     // MediaId ... NEED TO BE FILLED
    FALSE, // RemovableMedia
    TRUE,  // MediaPresent
    FALSE, // LogicalPartition
    FALSE, // ReadOnly
    FALSE, // WriteCaching;
    0,     // BlockSize ... NEED TO BE FILLED
    4,     // IoAlign
    0,     // LastBlock ... NEED TO BE FILLED
    0,     // LowestAlignedLba
    1,     // LogicalBlocksPerPhysicalBlock
  }, //Media;

  {
    MvFvbGetAttributes,       // GetAttributes
    MvFvbSetAttributes,       // SetAttributes
    MvFvbGetPhysicalAddress,  // GetPhysicalAddress
    MvFvbGetBlockSize,        // GetBlockSize
    MvFvbRead,                // Read
    MvFvbWrite,               // Write
    MvFvbEraseBlocks,         // EraseBlocks
    NULL,                     // ParentHandle
  }, //  FvbProtocol;

  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8)sizeof (VENDOR_DEVICE_PATH),
          (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
        }
      },
      { 0xfc0cb972, 0x21df, 0x44d2, { 0x92, 0xa5, 0x78, 0x98, 0x99, 0xcb, 0xf6, 0x61 } }
    },
    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      { sizeof (EFI_DEVICE_PATH_PROTOCOL), 0 }
    }
  } // DevicePath
};

//
// The Firmware Volume Block Protocol is the low-level interface
// to a firmware volume. File-level access to a firmware volume
// should not be done using the Firmware Volume Block Protocol.
// Normal access to a firmware volume must use the Firmware
// Volume Protocol. Typically, only the file system driver that
// produces the Firmware Volume Protocol will bind to the
// Firmware Volume Block Protocol.
//

/**
  Initialises the FV Header and Variable Store Header
  to support variable operations.

  @param[in]  Ptr - Location to initialise the headers

**/
STATIC
EFI_STATUS
MvFvbInitFvAndVariableStoreHeaders (
  IN FVB_DEVICE *FlashInstance
  )
{
  EFI_FIRMWARE_VOLUME_HEADER  *FirmwareVolumeHeader;
  VARIABLE_STORE_HEADER       *VariableStoreHeader;
  EFI_STATUS                  Status;
  VOID*                       Headers;
  UINTN                       HeadersLength;
  UINTN                       BlockSize;

  HeadersLength = sizeof (EFI_FIRMWARE_VOLUME_HEADER) +
                  sizeof (EFI_FV_BLOCK_MAP_ENTRY) +
                  sizeof (VARIABLE_STORE_HEADER);
  Headers = AllocateZeroPool (HeadersLength);

  BlockSize = FlashInstance->Media.BlockSize;

  //
  // FirmwareVolumeHeader->FvLength is declared to have the Variable area
  // AND the FTW working area AND the FTW Spare contiguous.
  //
  ASSERT (PcdGet64 (PcdFlashNvStorageVariableBase64) +
    PcdGet32 (PcdFlashNvStorageVariableSize) ==
    PcdGet64 (PcdFlashNvStorageFtwWorkingBase64));
  ASSERT (PcdGet64 (PcdFlashNvStorageFtwWorkingBase64) +
    PcdGet32 (PcdFlashNvStorageFtwWorkingSize) ==
    PcdGet64 (PcdFlashNvStorageFtwSpareBase64));

  // Check if the size of the area is at least one block size
  ASSERT ((PcdGet32 (PcdFlashNvStorageVariableSize) > 0) &&
    (PcdGet32 (PcdFlashNvStorageVariableSize) / BlockSize > 0));
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwWorkingSize) > 0) &&
    (PcdGet32 (PcdFlashNvStorageFtwWorkingSize) / BlockSize > 0));
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwSpareSize) > 0) &&
    (PcdGet32 (PcdFlashNvStorageFtwSpareSize) / BlockSize > 0));

  // Ensure the Variable areas are aligned on block size boundaries
  ASSERT ((PcdGet64 (PcdFlashNvStorageVariableBase64) % BlockSize) == 0);
  ASSERT ((PcdGet64 (PcdFlashNvStorageFtwWorkingBase64) % BlockSize) == 0);
  ASSERT ((PcdGet64 (PcdFlashNvStorageFtwSpareBase64) % BlockSize) == 0);

  //
  // EFI_FIRMWARE_VOLUME_HEADER
  //
  FirmwareVolumeHeader = (EFI_FIRMWARE_VOLUME_HEADER*)Headers;
  CopyGuid (&FirmwareVolumeHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid);
  FirmwareVolumeHeader->FvLength = FlashInstance->FvbSize;
  FirmwareVolumeHeader->Signature = EFI_FVH_SIGNATURE;
  FirmwareVolumeHeader->Attributes = EFI_FVB2_READ_ENABLED_CAP |
                                     EFI_FVB2_READ_STATUS |
                                     EFI_FVB2_STICKY_WRITE |
                                     EFI_FVB2_ERASE_POLARITY |
                                     EFI_FVB2_WRITE_STATUS |
                                     EFI_FVB2_WRITE_ENABLED_CAP;

  if (FlashInstance->IsMemoryMapped) {
    FirmwareVolumeHeader->Attributes |= EFI_FVB2_MEMORY_MAPPED;
  }

  FirmwareVolumeHeader->HeaderLength = sizeof (EFI_FIRMWARE_VOLUME_HEADER) +
                                       sizeof (EFI_FV_BLOCK_MAP_ENTRY);
  FirmwareVolumeHeader->Revision = EFI_FVH_REVISION;
  FirmwareVolumeHeader->BlockMap[0].NumBlocks = FlashInstance->Media.LastBlock + 1;
  FirmwareVolumeHeader->BlockMap[0].Length = FlashInstance->Media.BlockSize;
  FirmwareVolumeHeader->BlockMap[1].NumBlocks = 0;
  FirmwareVolumeHeader->BlockMap[1].Length = 0;
  FirmwareVolumeHeader->Checksum = CalculateCheckSum16 (
                                     (UINT16 *)FirmwareVolumeHeader,
                                     FirmwareVolumeHeader->HeaderLength);

  //
  // VARIABLE_STORE_HEADER
  //
  VariableStoreHeader = (VOID *)((UINTN)Headers +
                                 FirmwareVolumeHeader->HeaderLength);
  CopyGuid (&VariableStoreHeader->Signature, &gEfiAuthenticatedVariableGuid);
  VariableStoreHeader->Size = PcdGet32(PcdFlashNvStorageVariableSize) -
                              FirmwareVolumeHeader->HeaderLength;
  VariableStoreHeader->Format = VARIABLE_STORE_FORMATTED;
  VariableStoreHeader->State = VARIABLE_STORE_HEALTHY;

  // Install the combined super-header in the flash device
  Status = MvFvbWrite (&FlashInstance->FvbProtocol, 0, 0, &HeadersLength, Headers);

  FreePool (Headers);

  return Status;
}

/**
  Check the integrity of firmware volume header.

  @param[in] FwVolHeader - A pointer to a firmware volume header

  @retval  EFI_SUCCESS   - The firmware volume is consistent
  @retval  EFI_NOT_FOUND - The firmware volume has been corrupted.

**/
STATIC
EFI_STATUS
MvFvbValidateFvHeader (
  IN  FVB_DEVICE *FlashInstance
  )
{
  UINT16                      Checksum;
  EFI_FIRMWARE_VOLUME_HEADER  *FwVolHeader;
  VARIABLE_STORE_HEADER       *VariableStoreHeader;
  UINTN                       VariableStoreLength;

  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FlashInstance->RegionBaseAddress;

  // Verify the header revision, header signature, length
  if ((FwVolHeader->Revision  != EFI_FVH_REVISION) ||
      (FwVolHeader->Signature != EFI_FVH_SIGNATURE) ||
      (FwVolHeader->FvLength  != FlashInstance->FvbSize)) {
    DEBUG ((DEBUG_ERROR,
      "%a: No Firmware Volume header present\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Check the Firmware Volume Guid
  if (!CompareGuid (&FwVolHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Firmware Volume Guid non-compatible\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Verify the header checksum
  Checksum = CalculateSum16 ((UINT16 *)FwVolHeader, FwVolHeader->HeaderLength);
  if (Checksum != 0) {
    DEBUG ((DEBUG_ERROR,
      "%a: FV checksum is invalid (Checksum:0x%x)\n",
      __FUNCTION__,
      Checksum));
    return EFI_NOT_FOUND;
  }

  VariableStoreHeader = (VOID *)((UINTN)FwVolHeader + FwVolHeader->HeaderLength);

  // Check the Variable Store Guid
  if (!CompareGuid (&VariableStoreHeader->Signature, &gEfiVariableGuid) &&
      !CompareGuid (&VariableStoreHeader->Signature,
                    &gEfiAuthenticatedVariableGuid)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Variable Store Guid non-compatible\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  VariableStoreLength = PcdGet32 (PcdFlashNvStorageVariableSize) -
                        FwVolHeader->HeaderLength;
  if (VariableStoreHeader->Size != VariableStoreLength) {
    DEBUG ((DEBUG_ERROR,
      "%a: Variable Store Length does not match\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
 The GetAttributes() function retrieves the attributes and
 current settings of the block.

 @param This         Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Attributes   Pointer to EFI_FVB_ATTRIBUTES_2 in which the attributes and
                     current settings are returned.
                     Type EFI_FVB_ATTRIBUTES_2 is defined in
                     EFI_FIRMWARE_VOLUME_HEADER.

 @retval EFI_SUCCESS The firmware volume attributes were returned.

 **/
EFI_STATUS
EFIAPI
MvFvbGetAttributes (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL    *This,
  OUT       EFI_FVB_ATTRIBUTES_2                   *Attributes
  )
{
  EFI_FIRMWARE_VOLUME_HEADER  *FwVolHeader;
  EFI_FVB_ATTRIBUTES_2        *FlashFvbAttributes;
  FVB_DEVICE                  *FlashInstance;

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);

  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FlashInstance->RegionBaseAddress;
  FlashFvbAttributes = (EFI_FVB_ATTRIBUTES_2 *)&(FwVolHeader->Attributes);

  *Attributes = *FlashFvbAttributes;

  return EFI_SUCCESS;
}

/**
 The SetAttributes() function sets configurable firmware volume attributes
 and returns the new settings of the firmware volume.


 @param This                     EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Attributes               On input, Attributes is a pointer to
                                 EFI_FVB_ATTRIBUTES_2 that contains the desired
                                 firmware volume settings.
                                 On successful return, it contains the new
                                 settings of the firmware volume.

 @retval EFI_SUCCESS             The firmware volume attributes were returned.

 @retval EFI_INVALID_PARAMETER   The attributes requested are in conflict with
                                 the capabilities as declared in the firmware
                                 volume header.

 **/
EFI_STATUS
EFIAPI
MvFvbSetAttributes (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL  *This,
  IN OUT    EFI_FVB_ATTRIBUTES_2                 *Attributes
  )
{
  EFI_FVB_ATTRIBUTES_2  OldAttributes;
  EFI_FVB_ATTRIBUTES_2  FlashFvbAttributes;
  EFI_FVB_ATTRIBUTES_2  UnchangedAttributes;
  FVB_DEVICE           *FlashInstance;
  UINT32                Capabilities;
  UINT32                OldStatus;
  UINT32                NewStatus;

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);

  //
  // Obtain attributes from FVB header
  //
  MvFvbGetAttributes (This, &FlashFvbAttributes);

  OldAttributes = FlashFvbAttributes;
  Capabilities  = OldAttributes & EFI_FVB2_CAPABILITIES;
  OldStatus     = OldAttributes & EFI_FVB2_STATUS;
  NewStatus     = *Attributes & EFI_FVB2_STATUS;

  UnchangedAttributes = EFI_FVB2_READ_DISABLED_CAP  | \
                        EFI_FVB2_READ_ENABLED_CAP   | \
                        EFI_FVB2_WRITE_DISABLED_CAP | \
                        EFI_FVB2_WRITE_ENABLED_CAP  | \
                        EFI_FVB2_LOCK_CAP           | \
                        EFI_FVB2_STICKY_WRITE       | \
                        EFI_FVB2_ERASE_POLARITY     | \
                        EFI_FVB2_READ_LOCK_CAP      | \
                        EFI_FVB2_WRITE_LOCK_CAP     | \
                        EFI_FVB2_ALIGNMENT;

  if (FlashInstance->IsMemoryMapped) {
    UnchangedAttributes |= EFI_FVB2_MEMORY_MAPPED;
  }

  //
  // Some attributes of FV is read only can *not* be set
  //
  if ((OldAttributes & UnchangedAttributes) ^
      (*Attributes & UnchangedAttributes)) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // If firmware volume is locked, no status bit can be updated
  //
  if (OldAttributes & EFI_FVB2_LOCK_STATUS) {
    if (OldStatus ^ NewStatus) {
      return EFI_ACCESS_DENIED;
    }
  }
  //
  // Test read disable
  //
  if ((Capabilities & EFI_FVB2_READ_DISABLED_CAP) == 0) {
    if ((NewStatus & EFI_FVB2_READ_STATUS) == 0) {
      return EFI_INVALID_PARAMETER;
    }
  }
  //
  // Test read enable
  //
  if ((Capabilities & EFI_FVB2_READ_ENABLED_CAP) == 0) {
    if (NewStatus & EFI_FVB2_READ_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }
  //
  // Test write disable
  //
  if ((Capabilities & EFI_FVB2_WRITE_DISABLED_CAP) == 0) {
    if ((NewStatus & EFI_FVB2_WRITE_STATUS) == 0) {
      return EFI_INVALID_PARAMETER;
    }
  }
  //
  // Test write enable
  //
  if ((Capabilities & EFI_FVB2_WRITE_ENABLED_CAP) == 0) {
    if (NewStatus & EFI_FVB2_WRITE_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }
  //
  // Test lock
  //
  if ((Capabilities & EFI_FVB2_LOCK_CAP) == 0) {
    if (NewStatus & EFI_FVB2_LOCK_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  FlashFvbAttributes = FlashFvbAttributes & (0xFFFFFFFF & (~EFI_FVB2_STATUS));
  FlashFvbAttributes = FlashFvbAttributes | NewStatus;
  *Attributes = FlashFvbAttributes;

  return EFI_SUCCESS;
}

/**
 The GetPhysicalAddress() function retrieves the base address of
 a memory-mapped firmware volume. This function should be called
 only for memory-mapped firmware volumes.

 @param This               EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Address            Pointer to a caller-allocated
                           EFI_PHYSICAL_ADDRESS that, on successful
                           return from GetPhysicalAddress(), contains the
                           base address of the firmware volume.

 @retval EFI_SUCCESS       The firmware volume base address was returned.

 @retval EFI_NOT_SUPPORTED The firmware volume is not memory mapped.

 **/
EFI_STATUS
EFIAPI
MvFvbGetPhysicalAddress (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL  *This,
  OUT       EFI_PHYSICAL_ADDRESS                 *Address
  )
{
  FVB_DEVICE *FlashInstance;

  ASSERT (Address != NULL);

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);

  *Address = FlashInstance->RegionBaseAddress;

  return EFI_SUCCESS;
}

/**
 The GetBlockSize() function retrieves the size of the requested
 block. It also returns the number of additional blocks with
 the identical size. The GetBlockSize() function is used to
 retrieve the block map (see EFI_FIRMWARE_VOLUME_HEADER).


 @param This                     EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Lba                      Indicates the block whose size to return

 @param BlockSize                Pointer to a caller-allocated UINTN in which
                                 the size of the block is returned.

 @param NumberOfBlocks           Pointer to a caller-allocated UINTN in
                                 which the number of consecutive blocks,
                                 starting with Lba, is returned. All
                                 blocks in this range have a size of
                                 BlockSize.


 @retval EFI_SUCCESS             The firmware volume base address was returned.

 @retval EFI_INVALID_PARAMETER   The requested LBA is out of range.

 **/
EFI_STATUS
EFIAPI
MvFvbGetBlockSize (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL  *This,
  IN        EFI_LBA                              Lba,
  OUT       UINTN                                *BlockSize,
  OUT       UINTN                                *NumberOfBlocks
  )
{
  FVB_DEVICE *FlashInstance;

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);

  if (Lba > FlashInstance->Media.LastBlock) {
    DEBUG ((DEBUG_ERROR,
      "%a: Error: Requested LBA %ld is beyond the last available LBA (%ld).\n",
      __FUNCTION__,
      Lba,
      FlashInstance->Media.LastBlock));
    return EFI_INVALID_PARAMETER;
  } else {
    // Assume equal sized blocks in all flash devices
    *BlockSize = (UINTN)FlashInstance->Media.BlockSize;
    *NumberOfBlocks = (UINTN)(FlashInstance->Media.LastBlock - Lba + 1);

    return EFI_SUCCESS;
  }
}

/**
 Reads the specified number of bytes into a buffer from the specified block.

 The Read() function reads the requested number of bytes from the
 requested block and stores them in the provided buffer.
 Implementations should be mindful that the firmware volume
 might be in the ReadDisabled state. If it is in this state,
 the Read() function must return the status code
 EFI_ACCESS_DENIED without modifying the contents of the
 buffer. The Read() function must also prevent spanning block
 boundaries. If a read is requested that would span a block
 boundary, the read must read up to the boundary but not
 beyond. The output parameter NumBytes must be set to correctly
 indicate the number of bytes actually read. The caller must be
 aware that a read may be partially completed.

 @param This                 EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Lba                  The starting logical block index from which to read

 @param Offset               Offset into the block at which to begin reading.

 @param NumBytes             Pointer to a UINTN.
                             At entry, *NumBytes contains the total size of the
                             buffer.
                             At exit, *NumBytes contains the total number of
                             bytes read.

 @param Buffer               Pointer to a caller-allocated buffer that will be
                             used to hold the data that is read.

 @retval EFI_SUCCESS         The firmware volume was read successfully, and
                             contents are in Buffer.

 @retval EFI_BAD_BUFFER_SIZE Read attempted across an LBA boundary.
                             On output, NumBytes contains the total number of
                             bytes returned in Buffer.

 @retval EFI_ACCESS_DENIED   The firmware volume is in the ReadDisabled state.

 @retval EFI_DEVICE_ERROR    The block device is not functioning correctly and
                             could not be read.

 **/
EFI_STATUS
EFIAPI
MvFvbRead (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL   *This,
  IN        EFI_LBA                               Lba,
  IN        UINTN                                 Offset,
  IN OUT    UINTN                                 *NumBytes,
  IN OUT    UINT8                                 *Buffer
  )
{
  FVB_DEVICE   *FlashInstance;
  UINTN         BlockSize;
  UINTN         DataOffset;

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);


  // Cache the block size to avoid de-referencing pointers all the time
  BlockSize = FlashInstance->Media.BlockSize;

  //
  // The read must not span block boundaries.
  // We need to check each variable individually because adding two large
  // values together overflows.
  //
  if (Offset               >= BlockSize ||
      *NumBytes            >  BlockSize ||
      (Offset + *NumBytes) >  BlockSize) {
    DEBUG ((DEBUG_ERROR,
      "%a: Wrong buffer size: (Offset=0x%x + NumBytes=0x%x) > BlockSize=0x%x\n",
      __FUNCTION__,
      Offset,
      *NumBytes,
      BlockSize));
    return EFI_BAD_BUFFER_SIZE;
  }

  // No bytes to read
  if (*NumBytes == 0) {
    return EFI_SUCCESS;
  }

  DataOffset = GET_DATA_OFFSET (FlashInstance->RegionBaseAddress + Offset,
                 FlashInstance->StartLba + Lba,
                 FlashInstance->Media.BlockSize);

  // Read the memory-mapped data
  CopyMem (Buffer, (UINTN *)DataOffset, *NumBytes);

  return EFI_SUCCESS;
}

/**
 Writes the specified number of bytes from the input buffer to the block.

 The Write() function writes the specified number of bytes from
 the provided buffer to the specified block and offset. If the
 firmware volume is sticky write, the caller must ensure that
 all the bits of the specified range to write are in the
 EFI_FVB_ERASE_POLARITY state before calling the Write()
 function, or else the result will be unpredictable. This
 unpredictability arises because, for a sticky-write firmware
 volume, a write may negate a bit in the EFI_FVB_ERASE_POLARITY
 state but cannot flip it back again.  Before calling the
 Write() function,  it is recommended for the caller to first call
 the EraseBlocks() function to erase the specified block to
 write. A block erase cycle will transition bits from the
 (NOT)EFI_FVB_ERASE_POLARITY state back to the
 EFI_FVB_ERASE_POLARITY state. Implementations should be
 mindful that the firmware volume might be in the WriteDisabled
 state. If it is in this state, the Write() function must
 return the status code EFI_ACCESS_DENIED without modifying the
 contents of the firmware volume. The Write() function must
 also prevent spanning block boundaries. If a write is
 requested that spans a block boundary, the write must store up
 to the boundary but not beyond. The output parameter NumBytes
 must be set to correctly indicate the number of bytes actually
 written. The caller must be aware that a write may be
 partially completed. All writes, partial or otherwise, must be
 fully flushed to the hardware before the Write() service
 returns.

 @param This                 EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Lba                  The starting logical block index to write to.

 @param Offset               Offset into the block at which to begin writing.

 @param NumBytes             The pointer to a UINTN.
                             At entry, *NumBytes contains the total size of the
                             buffer.
                             At exit, *NumBytes contains the total number of
                             bytes actually written.

 @param Buffer               The pointer to a caller-allocated buffer that
                             contains the source for the write.

 @retval EFI_SUCCESS         The firmware volume was written successfully.

 @retval EFI_BAD_BUFFER_SIZE The write was attempted across an LBA boundary.
                             On output, NumBytes contains the total number of
                             bytes actually written.

 @retval EFI_ACCESS_DENIED   The firmware volume is in the WriteDisabled state.

 @retval EFI_DEVICE_ERROR    The block device is malfunctioning and could not be
                             written.


 **/
EFI_STATUS
EFIAPI
MvFvbWrite (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL   *This,
  IN        EFI_LBA                               Lba,
  IN        UINTN                                 Offset,
  IN OUT    UINTN                                 *NumBytes,
  IN        UINT8                                 *Buffer
  )
{
  EFI_STATUS    Status;
  FVB_DEVICE   *FlashInstance;
  UINTN         DataOffset;

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);

  DataOffset = GET_DATA_OFFSET (FlashInstance->FvbOffset + Offset,
                 FlashInstance->StartLba + Lba,
                 FlashInstance->Media.BlockSize);

  Status = FlashInstance->SpiFlashProtocol->Write (&FlashInstance->SpiDevice,
                                              DataOffset,
                                              *NumBytes,
                                              Buffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Failed to write to Spi device\n",
      __FUNCTION__));
    return Status;
  }

  // Update shadow buffer
  if (!FlashInstance->IsMemoryMapped) {
    DataOffset = GET_DATA_OFFSET (FlashInstance->RegionBaseAddress + Offset,
                   FlashInstance->StartLba + Lba,
                   FlashInstance->Media.BlockSize);

    CopyMem ((UINTN *)DataOffset, Buffer, *NumBytes);
  }

  return EFI_SUCCESS;
}

/**
 Erases and initialises a firmware volume block.

 The EraseBlocks() function erases one or more blocks as denoted
 by the variable argument list. The entire parameter list of
 blocks must be verified before erasing any blocks. If a block is
 requested that does not exist within the associated firmware
 volume (it has a larger index than the last block of the
 firmware volume), the EraseBlocks() function must return the
 status code EFI_INVALID_PARAMETER without modifying the contents
 of the firmware volume. Implementations should be mindful that
 the firmware volume might be in the WriteDisabled state. If it
 is in this state, the EraseBlocks() function must return the
 status code EFI_ACCESS_DENIED without modifying the contents of
 the firmware volume. All calls to EraseBlocks() must be fully
 flushed to the hardware before the EraseBlocks() service
 returns.

 @param This                     EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL
 instance.

 @param ...                      The variable argument list is a list of tuples.
                                 Each tuple describes a range of LBAs to erase
                                 and consists of the following:
                                 - An EFI_LBA that indicates the starting LBA
                                 - A UINTN that indicates the number of blocks
                                   to erase.

                                 The list is terminated with an
                                 EFI_LBA_LIST_TERMINATOR.

 @retval EFI_SUCCESS             The erase request successfully completed.

 @retval EFI_ACCESS_DENIED       The firmware volume is in the WriteDisabled
                                 state.

 @retval EFI_DEVICE_ERROR        The block device is not functioning correctly
                                 and could not be written.
                                 The firmware device may have been partially
                                 erased.

 @retval EFI_INVALID_PARAMETER   One or more of the LBAs listed in the variable
                                 argument list do not exist in the firmware
                                 volume.

 **/
EFI_STATUS
EFIAPI
MvFvbEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL *This,
  ...
  )
{
  EFI_FVB_ATTRIBUTES_2   FlashFvbAttributes;
  FVB_DEVICE            *FlashInstance;
  EFI_STATUS             Status;
  VA_LIST                Args;
  UINTN                  BlockAddress; // Physical address of Lba to erase
  EFI_LBA                StartingLba;  // Lba from which we start erasing
  UINTN                  NumOfLba;     // Number of Lba blocks to erase

  FlashInstance = INSTANCE_FROM_FVB_THIS (This);

  Status = EFI_SUCCESS;

  // Detect WriteDisabled state
  MvFvbGetAttributes (This, &FlashFvbAttributes);
  if ((FlashFvbAttributes & EFI_FVB2_WRITE_STATUS) == 0) {
    DEBUG ((DEBUG_ERROR,
      "%a: Device is in WriteDisabled state.\n",
      __FUNCTION__));
    return EFI_ACCESS_DENIED;
  }

  //
  // Before erasing, check the entire list of parameters to ensure
  // all specified blocks are valid.
  //
  VA_START (Args, This);
  do {
    // Get the Lba from which we start erasing
    StartingLba = VA_ARG (Args, EFI_LBA);

    // Have we reached the end of the list?
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      //Exit the while loop
      break;
    }

    // How many Lba blocks are we requested to erase?
    NumOfLba = VA_ARG (Args, UINT32);

    // All blocks must be within range
    if (NumOfLba == 0 ||
        (FlashInstance->StartLba + StartingLba + NumOfLba - 1) >
        FlashInstance->Media.LastBlock) {

      DEBUG ((DEBUG_ERROR,
        "%a: Error: Requested LBA are beyond the last available LBA (%ld).\n",
        __FUNCTION__,
        FlashInstance->Media.LastBlock));

      VA_END (Args);

      return EFI_INVALID_PARAMETER;
    }
  } while (TRUE);
  VA_END (Args);

  //
  // Start erasing
  //
  VA_START (Args, This);
  do {
    // Get the Lba from which we start erasing
    StartingLba = VA_ARG (Args, EFI_LBA);

    // Have we reached the end of the list?
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      // Exit the while loop
      break;
    }

    // How many Lba blocks are we requested to erase?
    NumOfLba = VA_ARG (Args, UINT32);

    // Go through each one and erase it
    while (NumOfLba > 0) {

      // Get the physical address of Lba to erase
      BlockAddress = GET_DATA_OFFSET (FlashInstance->FvbOffset,
                       FlashInstance->StartLba + StartingLba,
                       FlashInstance->Media.BlockSize);

      // Erase single block
      Status = FlashInstance->SpiFlashProtocol->Erase (&FlashInstance->SpiDevice,
                                                  BlockAddress,
                                                  FlashInstance->Media.BlockSize);
      if (EFI_ERROR (Status)) {
        VA_END (Args);
        return EFI_DEVICE_ERROR;
      }

      // Move to the next Lba
      StartingLba++;
      NumOfLba--;
    }
  } while (TRUE);
  VA_END (Args);

  return EFI_SUCCESS;
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
STATIC
VOID
EFIAPI
MvFvbVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  // Convert SPI memory mapped region
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->RegionBaseAddress);

  // Convert SPI device description
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->SpiDevice.Info);
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->SpiDevice.HostRegisterBaseAddress);
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->SpiDevice);

  // Convert SpiFlashProtocol
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->SpiFlashProtocol->Erase);
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->SpiFlashProtocol->Write);
  EfiConvertPointer (0x0, (VOID**)&mFvbDevice->SpiFlashProtocol);

  return;
}

STATIC
EFI_STATUS
MvFvbFlashProbe (
  IN FVB_DEVICE *FlashInstance
  )
{
  MARVELL_SPI_FLASH_PROTOCOL *SpiFlashProtocol;
  EFI_STATUS Status;

  SpiFlashProtocol = FlashInstance->SpiFlashProtocol;

  // Read SPI flash ID
  Status = SpiFlashProtocol->ReadId (&FlashInstance->SpiDevice, TRUE);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = SpiFlashProtocol->Init (SpiFlashProtocol, &FlashInstance->SpiDevice);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot initialize flash device\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  //
  // SPI flash may require 20ms interval between enabling it and
  // accessing in Direct Mode to its memory mapped content.
  //
  gBS->Stall (20000);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvFvbPrepareFvHeader (
  IN FVB_DEVICE *FlashInstance
  )
{
  EFI_BOOT_MODE BootMode;
  EFI_STATUS Status;

  // Check if it is required to use default environment
  BootMode = GetBootModeHob ();
  if (BootMode == BOOT_WITH_DEFAULT_SETTINGS) {
    Status = EFI_INVALID_PARAMETER;
  } else {
    // Validate header at the beginning of FV region
    Status = MvFvbValidateFvHeader (FlashInstance);
  }

  // Install the default FVB header if required
  if (EFI_ERROR (Status)) {
    // There is no valid header, so time to install one.
    DEBUG ((DEBUG_ERROR, "%a: The FVB Header is not valid.\n", __FUNCTION__));
    DEBUG ((DEBUG_ERROR,
      "%a: Installing a correct one for this volume.\n",
      __FUNCTION__));

    // Erase entire region that is reserved for variable storage
    Status = FlashInstance->SpiFlashProtocol->Erase (&FlashInstance->SpiDevice,
                                                FlashInstance->FvbOffset,
                                                FlashInstance->FvbSize);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Install all appropriate headers
    Status = MvFvbInitFvAndVariableStoreHeaders (FlashInstance);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvFvbConfigureFlashInstance (
  IN OUT FVB_DEVICE *FlashInstance
  )
{
  EFI_STATUS Status;
  UINTN     DataOffset;
  UINTN     VariableSize, FtwWorkingSize, FtwSpareSize, MemorySize;


  // Locate SPI protocols
  Status = gBS->LocateProtocol (&gMarvellSpiFlashProtocolGuid,
                  NULL,
                  (VOID **)&FlashInstance->SpiFlashProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot locate SpiFlash protocol\n", __FUNCTION__));
    return Status;
  }

  Status = gBS->LocateProtocol (&gMarvellSpiMasterProtocolGuid,
                  NULL,
                  (VOID **)&FlashInstance->SpiMasterProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot locate SpiMaster protocol\n", __FUNCTION__));
    return Status;
  }

  // Setup and probe SPI flash
  FlashInstance->SpiMasterProtocol->SetupDevice (FlashInstance->SpiMasterProtocol,
                                      &FlashInstance->SpiDevice,
                                      0,
                                      0);

  Status = MvFvbFlashProbe (FlashInstance);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Error while performing SPI flash probe\n",
      __FUNCTION__));
    return Status;
  }

  // Fill remaining flash description
  VariableSize = PcdGet32 (PcdFlashNvStorageVariableSize);
  FtwWorkingSize = PcdGet32 (PcdFlashNvStorageFtwWorkingSize);
  FtwSpareSize = PcdGet32 (PcdFlashNvStorageFtwSpareSize);

  FlashInstance->IsMemoryMapped = PcdGetBool (PcdSpiMemoryMapped);
  FlashInstance->FvbSize = VariableSize + FtwWorkingSize + FtwSpareSize;
  FlashInstance->FvbOffset = PcdGet32 (PcdSpiVariableOffset);

  FlashInstance->Media.MediaId = 0;
  FlashInstance->Media.BlockSize = FlashInstance->SpiDevice.Info->SectorSize;
  FlashInstance->Media.LastBlock = FlashInstance->Size /
                                   FlashInstance->Media.BlockSize - 1;

  if (FlashInstance->IsMemoryMapped) {
    FlashInstance->DeviceBaseAddress = PcdGet64 (PcdSpiMemoryBase);
    FlashInstance->RegionBaseAddress = PcdGet64 (PcdFlashNvStorageVariableBase64);
  } else {
    MemorySize = EFI_SIZE_TO_PAGES (FlashInstance->FvbSize);

    // FaultTolerantWriteDxe requires memory to be aligned to FtwWorkingSize
    FlashInstance->RegionBaseAddress = (UINTN) AllocateAlignedRuntimePages (MemorySize,
                                                 SIZE_64KB);
    if (FlashInstance->RegionBaseAddress == (UINTN) NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    Status = PcdSet64S (PcdFlashNvStorageVariableBase64,
               (UINT64) FlashInstance->RegionBaseAddress);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet64S (PcdFlashNvStorageFtwWorkingBase64,
               (UINT64) FlashInstance->RegionBaseAddress
               + VariableSize);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet64S (PcdFlashNvStorageFtwSpareBase64,
               (UINT64) FlashInstance->RegionBaseAddress
               + VariableSize
               + FtwWorkingSize);
    ASSERT_EFI_ERROR (Status);

    // Fill the buffer with data from flash
    DataOffset = GET_DATA_OFFSET (FlashInstance->FvbOffset,
                   FlashInstance->StartLba,
                   FlashInstance->Media.BlockSize);
    Status = FlashInstance->SpiFlashProtocol->Read (&FlashInstance->SpiDevice,
                                                DataOffset,
                                                FlashInstance->FvbSize,
                                                (VOID *)FlashInstance->RegionBaseAddress);
    if (EFI_ERROR (Status)) {
      goto ErrorFreeAllocatedPages;
    }
  }

  Status = gBS->InstallMultipleProtocolInterfaces (&FlashInstance->Handle,
                  &gEfiDevicePathProtocolGuid, &FlashInstance->DevicePath,
                  &gEfiFirmwareVolumeBlockProtocolGuid, &FlashInstance->FvbProtocol,
                  NULL);
  if (EFI_ERROR (Status)) {
    goto ErrorFreeAllocatedPages;
  }

  Status = MvFvbPrepareFvHeader (FlashInstance);
  if (EFI_ERROR (Status)) {
    goto ErrorPrepareFvbHeader;
  }

  return EFI_SUCCESS;

ErrorPrepareFvbHeader:
  gBS->UninstallMultipleProtocolInterfaces (&FlashInstance->Handle,
         &gEfiDevicePathProtocolGuid,
         &gEfiFirmwareVolumeBlockProtocolGuid,
         NULL);

ErrorFreeAllocatedPages:
  if (!FlashInstance->IsMemoryMapped) {
    FreeAlignedPages ((VOID *)FlashInstance->RegionBaseAddress,
      MemorySize);
  }

  return Status;
}

EFI_STATUS
EFIAPI
MvFvbEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  UINTN       RuntimeMmioRegionSize;
  UINTN       RegionBaseAddress;

  //
  // Create FVB flash device
  //
  mFvbDevice = AllocateRuntimeCopyPool (sizeof (mMvFvbFlashInstanceTemplate),
                 &mMvFvbFlashInstanceTemplate);
  if (mFvbDevice == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Detect and configure flash device
  //
  Status = MvFvbConfigureFlashInstance (mFvbDevice);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to configure Fvb SPI device\n", __FUNCTION__));
    goto ErrorConfigureFlash;
  }

  //
  // The driver implementing the variable read service can now be dispatched;
  // the varstore headers are in place.
  //
  Status = gBS->InstallProtocolInterface (&gImageHandle,
                  &gEdkiiNvVarStoreFormattedGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Failed to install gEdkiiNvVarStoreFormattedGuid\n",
      __FUNCTION__));
    goto ErrorInstallNvVarStoreFormatted;
  }

  //
  // Declare the Non-Volatile storage as EFI_MEMORY_RUNTIME
  //
  RegionBaseAddress = mFvbDevice->RegionBaseAddress;

  if (mFvbDevice->IsMemoryMapped) {
    RuntimeMmioRegionSize = mFvbDevice->FvbSize;
    Status = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                    RegionBaseAddress,
                    RuntimeMmioRegionSize,
                    EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to add memory space\n", __FUNCTION__));
      goto ErrorAddSpace;
    }


    Status = gDS->SetMemorySpaceAttributes (RegionBaseAddress,
                    RuntimeMmioRegionSize,
                    EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
    if (EFI_ERROR (Status)) {
     DEBUG ((DEBUG_ERROR, "%a: Failed to set memory attributes\n", __FUNCTION__));
      goto ErrorSetMemAttr;
    }
  }

  //
  // Register for the virtual address change event
  //
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  MvFvbVirtualNotifyEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mFvbVirtualAddrChangeEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to register VA change event\n", __FUNCTION__));
    goto ErrorSetMemAttr;
  }

  //
  // Configure runtime access to host controller registers
  //
  Status = mFvbDevice->SpiMasterProtocol->ConfigRuntime (&mFvbDevice->SpiDevice);
  if (EFI_ERROR (Status)) {
    goto ErrorSetMemAttr;
  }

  return Status;

ErrorSetMemAttr:
  gDS->RemoveMemorySpace (RegionBaseAddress, RuntimeMmioRegionSize);

ErrorAddSpace:
  gBS->UninstallProtocolInterface (gImageHandle,
         &gEdkiiNvVarStoreFormattedGuid,
         NULL);

ErrorInstallNvVarStoreFormatted:
  gBS->UninstallMultipleProtocolInterfaces (&mFvbDevice->Handle,
         &gEfiDevicePathProtocolGuid,
         &gEfiFirmwareVolumeBlockProtocolGuid,
         NULL);

ErrorConfigureFlash:
  FreePool (mFvbDevice);

  return Status;
}
