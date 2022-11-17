/** @file
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2006-2014, Intel Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Protocol/DevicePath.h>
#include <Protocol/FirmwareVolumeBlock.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "VarBlockService.h"

#define EFI_FVB2_STATUS \
          (EFI_FVB2_READ_STATUS | EFI_FVB2_WRITE_STATUS | EFI_FVB2_LOCK_STATUS)

EFI_FW_VOL_INSTANCE *mFvInstance;

FV_MEMMAP_DEVICE_PATH mFvMemmapDevicePathTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_MEMMAP_DP,
      {
        (UINT8)(sizeof (MEMMAP_DEVICE_PATH)),
        (UINT8)(sizeof (MEMMAP_DEVICE_PATH) >> 8)
      }
    },
    EfiMemoryMappedIO,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

FV_PIWG_DEVICE_PATH mFvPIWGDevicePathTemplate = {
  {
    {
      MEDIA_DEVICE_PATH,
      MEDIA_PIWG_FW_VOL_DP,
      {
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH)),
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH) >> 8)
      }
    },
    { 0 }
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

EFI_FW_VOL_BLOCK_DEVICE mFvbDeviceTemplate = {
  NULL,
  {
    FvbProtocolGetAttributes,
    FvbProtocolSetAttributes,
    FvbProtocolGetPhysicalAddress,
    FvbProtocolGetBlockSize,
    FvbProtocolRead,
    FvbProtocolWrite,
    FvbProtocolEraseBlocks,
    NULL
  }
};


EFI_STATUS
VarStoreWrite (
  IN     UINTN Address,
  IN OUT UINTN *NumBytes,
  IN     UINT8 *Buffer
  )
{
  CopyMem ((VOID*)Address, Buffer, *NumBytes);
  mFvInstance->Dirty = TRUE;

  return EFI_SUCCESS;
}


EFI_STATUS
VarStoreErase (
  IN UINTN Address,
  IN UINTN LbaLength
  )
{
  SetMem ((VOID*)Address, LbaLength, 0xff);
  mFvInstance->Dirty = TRUE;

  return EFI_SUCCESS;
}


EFI_STATUS
FvbGetVolumeAttributes (
  OUT EFI_FVB_ATTRIBUTES_2 *Attributes
  )
{
  *Attributes = mFvInstance->VolumeHeader->Attributes;
  return EFI_SUCCESS;
}


EFI_STATUS
FvbGetLbaAddress (
  IN  EFI_LBA Lba,
  OUT UINTN *LbaAddress,
  OUT UINTN *LbaLength,
  OUT UINTN *NumOfBlocks
  )
/*++

  Routine Description:
    Retrieves the starting address of an LBA in an FV

  Arguments:
    Lba                   - The logical block address
    LbaAddress            - On output, contains the physical starting address
                            of the Lba
    LbaLength             - On output, contains the length of the block
    NumOfBlocks           - A pointer to a caller allocated UINTN in which the
                            number of consecutive blocks starting with Lba is
                            returned. All blocks in this range have a size of
                            BlockSize

  Returns:
    EFI_SUCCESS
    EFI_INVALID_PARAMETER

--*/
{
  UINT32 NumBlocks;
  UINT32 BlockLength;
  UINTN Offset;
  EFI_LBA StartLba;
  EFI_LBA NextLba;
  EFI_FV_BLOCK_MAP_ENTRY *BlockMap;

  StartLba = 0;
  Offset = 0;
  BlockMap = &(mFvInstance->VolumeHeader->BlockMap[0]);

  //
  // Parse the blockmap of the FV to find which map entry the Lba belongs to.
  //
  while (TRUE) {
    NumBlocks = BlockMap->NumBlocks;
    BlockLength = BlockMap->Length;

    if (NumBlocks == 0 || BlockLength == 0) {
      return EFI_INVALID_PARAMETER;
    }

    NextLba = StartLba + NumBlocks;

    //
    // The map entry found.
    //
    if (Lba >= StartLba && Lba < NextLba) {
      Offset = Offset + (UINTN)MultU64x32 ((Lba - StartLba), BlockLength);
      if (LbaAddress != NULL) {
        *LbaAddress = mFvInstance->FvBase + Offset;
      }

      if (LbaLength != NULL) {
        *LbaLength = BlockLength;
      }

      if (NumOfBlocks != NULL) {
        *NumOfBlocks = (UINTN)(NextLba - Lba);
      }

      return EFI_SUCCESS;
    }

    StartLba = NextLba;
    Offset = Offset + NumBlocks * BlockLength;
    BlockMap++;
  }
}


EFI_STATUS
FvbEraseBlock (
  IN EFI_LBA Lba
  )
/*++

Routine Description:
  Erases and initializes a firmware volume block

Arguments:
  Lba                   - The logical block index to be erased

Returns:
  EFI_SUCCESS           - The erase request was successfully completed
  EFI_ACCESS_DENIED     - The firmware volume is in the WriteDisabled state
  EFI_DEVICE_ERROR      - The block device is not functioning correctly and
                          could not be written. Firmware device may have been
                          partially erased
  EFI_INVALID_PARAMETER

--*/
{
  EFI_FVB_ATTRIBUTES_2 Attributes;
  UINTN                LbaAddress;
  UINTN                LbaLength;
  EFI_STATUS           Status;

  //
  // Check if the FV is write enabled
  //
  FvbGetVolumeAttributes (&Attributes);

  if ((Attributes & EFI_FVB2_WRITE_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }
  //
  // Get the starting address of the block for erase. For debug reasons,
  // LbaWriteAddress may not be the same as LbaAddress.
  //
  Status = FvbGetLbaAddress (Lba, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return VarStoreErase (
           LbaAddress,
           LbaLength
         );
}


EFI_STATUS
FvbSetVolumeAttributes (
  IN OUT EFI_FVB_ATTRIBUTES_2 *Attributes
  )
/*++

  Routine Description:
    Modifies the current settings of the firmware volume according to the
    input parameter, and returns the new setting of the volume

  Arguments:
    Attributes            - On input, it is a pointer to EFI_FVB_ATTRIBUTES_2
                            containing the desired firmware volume settings.
                            On successful return, it contains the new setting.

  Returns:
    EFI_SUCCESS           - Successfully returns
    EFI_ACCESS_DENIED     - The volume setting is locked and cannot be modified
    EFI_INVALID_PARAMETER

--*/
{
  EFI_FVB_ATTRIBUTES_2 OldAttributes;
  EFI_FVB_ATTRIBUTES_2 *AttribPtr;
  UINT32 Capabilities;
  UINT32 OldStatus;
  UINT32 NewStatus;
  EFI_FVB_ATTRIBUTES_2 UnchangedAttributes;

  AttribPtr =
    (EFI_FVB_ATTRIBUTES_2*) &(mFvInstance->VolumeHeader->Attributes);
  OldAttributes = *AttribPtr;
  Capabilities = OldAttributes & (EFI_FVB2_READ_DISABLED_CAP | \
                                  EFI_FVB2_READ_ENABLED_CAP |    \
                                  EFI_FVB2_WRITE_DISABLED_CAP |  \
                                  EFI_FVB2_WRITE_ENABLED_CAP |   \
                                  EFI_FVB2_LOCK_CAP              \
                                  );
  OldStatus = OldAttributes & EFI_FVB2_STATUS;
  NewStatus = *Attributes & EFI_FVB2_STATUS;

  UnchangedAttributes = EFI_FVB2_READ_DISABLED_CAP  | \
                        EFI_FVB2_READ_ENABLED_CAP   | \
                        EFI_FVB2_WRITE_DISABLED_CAP | \
                        EFI_FVB2_WRITE_ENABLED_CAP  | \
                        EFI_FVB2_LOCK_CAP           | \
                        EFI_FVB2_STICKY_WRITE       | \
                        EFI_FVB2_MEMORY_MAPPED      | \
                        EFI_FVB2_ERASE_POLARITY     | \
                        EFI_FVB2_READ_LOCK_CAP      | \
                        EFI_FVB2_WRITE_LOCK_CAP     | \
                        EFI_FVB2_ALIGNMENT;

  //
  // Some attributes of FV is read only can *not* be set.
  //
  if ((OldAttributes & UnchangedAttributes) ^ (*Attributes & UnchangedAttributes)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // If firmware volume is locked, no status bit can be updated.
  //
  if (OldAttributes & EFI_FVB2_LOCK_STATUS) {
    if (OldStatus ^ NewStatus) {
      return EFI_ACCESS_DENIED;
    }
  }

  //
  // Test read disable.
  //
  if ((Capabilities & EFI_FVB2_READ_DISABLED_CAP) == 0) {
    if ((NewStatus & EFI_FVB2_READ_STATUS) == 0) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test read enable.
  //
  if ((Capabilities & EFI_FVB2_READ_ENABLED_CAP) == 0) {
    if (NewStatus & EFI_FVB2_READ_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test write disable.
  //
  if ((Capabilities & EFI_FVB2_WRITE_DISABLED_CAP) == 0) {
    if ((NewStatus & EFI_FVB2_WRITE_STATUS) == 0) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test write enable.
  //
  if ((Capabilities & EFI_FVB2_WRITE_ENABLED_CAP) == 0) {
    if (NewStatus & EFI_FVB2_WRITE_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test lock.
  //
  if ((Capabilities & EFI_FVB2_LOCK_CAP) == 0) {
    if (NewStatus & EFI_FVB2_LOCK_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  *AttribPtr = (*AttribPtr) & (0xFFFFFFFF & (~EFI_FVB2_STATUS));
  *AttribPtr = (*AttribPtr) | NewStatus;
  *Attributes = *AttribPtr;

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
FvbProtocolGetPhysicalAddress (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  OUT EFI_PHYSICAL_ADDRESS *Address
  )
{
  *Address = mFvInstance->FvBase;
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
FvbProtocolGetBlockSize (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN CONST EFI_LBA Lba,
  OUT UINTN *BlockSize,
  OUT UINTN *NumOfBlocks
  )
/*++

  Routine Description:
    Retrieve the size of a logical block

  Arguments:
    This                  - Calling context
    Lba                   - Indicates which block to return the size for.
    BlockSize             - A pointer to a caller allocated UINTN in which
                            the size of the block is returned
    NumOfBlocks           - a pointer to a caller allocated UINTN in which the
                            number of consecutive blocks starting with Lba is
                            returned. All blocks in this range have a size of
                            BlockSize

  Returns:
    EFI_SUCCESS           - The firmware volume was read successfully and
                            contents are in Buffer

--*/
{
  return FvbGetLbaAddress (
           Lba,
           NULL,
           BlockSize,
           NumOfBlocks
         );
}


EFI_STATUS
EFIAPI
FvbProtocolGetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  OUT EFI_FVB_ATTRIBUTES_2 *Attributes
  )
/*++

  Routine Description:
      Retrieves Volume attributes.  No polarity translations are done.

  Arguments:
      This                - Calling context
      Attributes          - output buffer which contains attributes

  Returns:
    EFI_SUCCESS           - Successfully returns

--*/
{
  return FvbGetVolumeAttributes (Attributes);
}


EFI_STATUS
EFIAPI
FvbProtocolSetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN OUT EFI_FVB_ATTRIBUTES_2 *Attributes
  )
/*++

  Routine Description:
    Sets Volume attributes. No polarity translations are done.

  Arguments:
    This                  - Calling context
    Attributes            - output buffer which contains attributes

  Returns:
    EFI_SUCCESS           - Successfully returns

--*/
{
  return FvbSetVolumeAttributes (Attributes);
}


EFI_STATUS
EFIAPI
FvbProtocolEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL*This,
  ...
  )
/*++

  Routine Description:

    The EraseBlock() function erases one or more blocks as denoted by the
    variable argument list. The entire parameter list of blocks must be
    verified prior to erasing any blocks.  If a block is requested that does
    not exist within the associated firmware volume (it has a larger index than
    the last block of the firmware volume), the EraseBlock() function must
    return EFI_INVALID_PARAMETER without modifying the contents of the firmware
    volume.

  Arguments:
    This                  - Calling context
    ...                   - Starting LBA followed by Number of Lba to erase.
                            a -1 to terminate the list.

  Returns:
    EFI_SUCCESS           - The erase request was successfully completed
    EFI_ACCESS_DENIED     - The firmware volume is in the WriteDisabled state
    EFI_DEVICE_ERROR      - The block device is not functioning correctly and
                            could not be written. Firmware device may have been
                            partially erased

--*/
{
  UINTN NumOfBlocks;
  VA_LIST args;
  EFI_LBA StartingLba;
  UINTN NumOfLba;
  EFI_STATUS Status;

  NumOfBlocks = mFvInstance->NumOfBlocks;
  VA_START (args, This);

  do {
    StartingLba = VA_ARG (args, EFI_LBA);
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      break;
    }

    NumOfLba = VA_ARG (args, UINTN);

    if ((NumOfLba == 0) || ((StartingLba + NumOfLba) > NumOfBlocks)) {
      VA_END (args);
      return EFI_INVALID_PARAMETER;
    }
  } while (1);

  VA_END (args);

  VA_START (args, This);
  do {
    StartingLba = VA_ARG (args, EFI_LBA);
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      break;
    }

    NumOfLba = VA_ARG (args, UINTN);

    while (NumOfLba > 0) {
      Status = FvbEraseBlock (StartingLba);
      if (EFI_ERROR (Status)) {
        VA_END (args);
        return Status;
      }

      StartingLba++;
      NumOfLba--;
    }

  } while (1);

  VA_END (args);

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
FvbProtocolWrite (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN       EFI_LBA Lba,
  IN       UINTN Offset,
  IN OUT   UINTN *NumBytes,
  IN       UINT8 *Buffer
  )
/*++

  Routine Description:

    Writes data beginning at Lba:Offset from FV. The write terminates either
    when *NumBytes of data have been written, or when a block boundary is
    reached.  *NumBytes is updated to reflect the actual number of bytes
    written. The write opertion does not include erase. This routine will
    attempt to write only the specified bytes. If the writes do not stick,
    it will return an error.

  Arguments:
    This                  - Calling context
    Lba                   - Block in which to begin write
    Offset                - Offset in the block at which to begin write
    NumBytes              - On input, indicates the requested write size. On
                            output, indicates the actual number of bytes
                            written
    Buffer                - Buffer containing source data for the write.

  Returns:
    EFI_SUCCESS           - The firmware volume was written successfully
    EFI_BAD_BUFFER_SIZE   - Write attempted across a LBA boundary. On output,
                            NumBytes contains the total number of bytes
                            actually written
    EFI_ACCESS_DENIED     - The firmware volume is in the WriteDisabled state
    EFI_DEVICE_ERROR      - The block device is not functioning correctly and
                            could not be written
    EFI_INVALID_PARAMETER - NumBytes or Buffer are NULL

--*/
{
  EFI_FVB_ATTRIBUTES_2 Attributes;
  UINTN LbaAddress;
  UINTN LbaLength;
  EFI_STATUS Status;
  EFI_STATUS ReturnStatus;

  //
  // Check for invalid conditions.
  //
  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (*NumBytes == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FvbGetLbaAddress (Lba, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if the FV is write enabled.
  //
  FvbGetVolumeAttributes (&Attributes);

  if ((Attributes & EFI_FVB2_WRITE_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Perform boundary checks and adjust NumBytes.
  //
  if (Offset > LbaLength) {
    return EFI_INVALID_PARAMETER;
  }

  if (LbaLength < (*NumBytes + Offset)) {
    *NumBytes = (UINT32)(LbaLength - Offset);
    Status = EFI_BAD_BUFFER_SIZE;
  }

  ReturnStatus = VarStoreWrite (
                   LbaAddress + Offset,
                   NumBytes,
                   Buffer
                 );
  if (EFI_ERROR (ReturnStatus)) {
    return ReturnStatus;
  }

  return Status;
}


EFI_STATUS
EFIAPI
FvbProtocolRead (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN CONST EFI_LBA Lba,
  IN CONST UINTN Offset,
  IN OUT UINTN *NumBytes,
  IN UINT8 *Buffer
  )
/*++

  Routine Description:

    Reads data beginning at Lba:Offset from FV. The Read terminates either
    when *NumBytes of data have been read, or when a block boundary is
    reached.  *NumBytes is updated to reflect the actual number of bytes
    written. The write opertion does not include erase. This routine will
    attempt to write only the specified bytes. If the writes do not stick,
    it will return an error.

  Arguments:
    This                  - Calling context
    Lba                   - Block in which to begin Read
    Offset                - Offset in the block at which to begin Read
    NumBytes              - On input, indicates the requested write size. On
                            output, indicates the actual number of bytes Read
    Buffer                - Buffer containing source data for the Read.

  Returns:
    EFI_SUCCESS           - The firmware volume was read successfully and
                            contents are in Buffer
    EFI_BAD_BUFFER_SIZE   - Read attempted across a LBA boundary. On output,
                            NumBytes contains the total number of bytes
                            returned in Buffer
    EFI_ACCESS_DENIED     - The firmware volume is in the ReadDisabled state
    EFI_DEVICE_ERROR      - The block device is not functioning correctly and
                            could not be read
    EFI_INVALID_PARAMETER - NumBytes or Buffer are NULL

--*/
{
  EFI_FVB_ATTRIBUTES_2 Attributes;
  UINTN LbaAddress;
  UINTN LbaLength;
  EFI_STATUS Status;

  //
  // Check for invalid conditions.
  //
  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (*NumBytes == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FvbGetLbaAddress (Lba, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if the FV is read enabled.
  //
  FvbGetVolumeAttributes (&Attributes);

  if ((Attributes & EFI_FVB2_READ_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Perform boundary checks and adjust NumBytes.
  //
  if (Offset > LbaLength) {
    return EFI_INVALID_PARAMETER;
  }

  if (LbaLength < (*NumBytes + Offset)) {
    *NumBytes = (UINT32)(LbaLength - Offset);
    Status = EFI_BAD_BUFFER_SIZE;
  }

  CopyMem (Buffer, (VOID*)(LbaAddress + Offset), (UINTN)*NumBytes);

  return Status;
}


EFI_STATUS
ValidateFvHeader (
  IN EFI_FIRMWARE_VOLUME_HEADER *FwVolHeader
  )
/*++

  Routine Description:
    Check the integrity of firmware volume header

  Arguments:
    FwVolHeader           - A pointer to a firmware volume header

  Returns:
    EFI_SUCCESS           - The firmware volume is consistent
    EFI_NOT_FOUND         - The firmware volume has corrupted. So it is not an
                            FV

--*/
{
  UINT16 Checksum;

  //
  // Verify the header revision, header signature, length
  // Length of FvBlock cannot be 2**64-1
  // HeaderLength cannot be an odd number.
  //
  if ((FwVolHeader->Revision != EFI_FVH_REVISION) ||
      (FwVolHeader->Signature != EFI_FVH_SIGNATURE) ||
      (FwVolHeader->FvLength == ((UINTN)-1)) ||
      ((FwVolHeader->HeaderLength & 0x01) != 0)
      ) {
    return EFI_NOT_FOUND;
  }

  //
  // Verify the header checksum.
  //

  Checksum = CalculateSum16 ((UINT16*)FwVolHeader, FwVolHeader->HeaderLength);
  if (Checksum != 0) {
    UINT16 Expected;

    Expected =
      (UINT16)(((UINTN)FwVolHeader->Checksum + 0x10000 - Checksum) & 0xffff);

    DEBUG ((DEBUG_INFO, "FV@%p Checksum is 0x%x, expected 0x%x\n",
      FwVolHeader, FwVolHeader->Checksum, Expected));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
FvbInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++

  Routine Description:
    This function does common initialization for FVB services

  Arguments:

  Returns:

--*/
{
  EFI_STATUS Status;
  UINT32 BufferSize;
  EFI_FV_BLOCK_MAP_ENTRY *PtrBlockMapEntry;
  EFI_FW_VOL_BLOCK_DEVICE *FvbDevice;
  UINT32 MaxLbaSize;
  EFI_PHYSICAL_ADDRESS BaseAddress;
  UINTN Length;
  UINTN NumOfBlocks;
  RETURN_STATUS PcdStatus;
  UINTN StartOffset;

  BaseAddress = PcdGet32 (PcdNvStorageVariableBase);
  Length = (FixedPcdGet32 (PcdFlashNvStorageVariableSize) +
    FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
    FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize) +
    FixedPcdGet32 (PcdNvStorageEventLogSize));
  StartOffset = BaseAddress - FixedPcdGet64 (PcdFdBaseAddress);

  BufferSize = sizeof (EFI_FW_VOL_INSTANCE);

  mFvInstance = AllocateRuntimeZeroPool (BufferSize);
  if (mFvInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mFvInstance->FvBase = (UINTN)BaseAddress;
  mFvInstance->FvLength = (UINTN)Length;
  mFvInstance->Offset = StartOffset;
  /*
   * Should I parse config.txt instead and find the real name?
   */
  mFvInstance->MappedFile = L"RPI_EFI.FD";

  Status = ValidateFvHeader (mFvInstance->VolumeHeader);
  if (!EFI_ERROR (Status)) {
    if (mFvInstance->VolumeHeader->FvLength != Length ||
        mFvInstance->VolumeHeader->BlockMap[0].Length !=
        PcdGet32 (PcdFirmwareBlockSize)) {
      Status = EFI_VOLUME_CORRUPTED;
    }
  }
  if (EFI_ERROR (Status)) {
    EFI_FIRMWARE_VOLUME_HEADER *GoodFwVolHeader;
    UINTN WriteLength;

    DEBUG ((DEBUG_INFO,
      "Variable FV header is not valid. It will be reinitialized.\n"));

    //
    // Get FvbInfo
    //
    Status = GetFvbInfo (Length, &GoodFwVolHeader);
    ASSERT_EFI_ERROR (Status);

    //
    // Erase all the blocks
    //
    Status = VarStoreErase ((UINTN)mFvInstance->FvBase, mFvInstance->FvLength);
    ASSERT_EFI_ERROR (Status);
    //
    // Write good FV header
    //
    WriteLength = GoodFwVolHeader->HeaderLength;
    Status = VarStoreWrite ((UINTN)mFvInstance->FvBase, &WriteLength,
               (UINT8*)GoodFwVolHeader);
    ASSERT_EFI_ERROR (Status);
    ASSERT (WriteLength == GoodFwVolHeader->HeaderLength);

    Status = ValidateFvHeader (mFvInstance->VolumeHeader);
    ASSERT_EFI_ERROR (Status);
  }

  MaxLbaSize = 0;
  NumOfBlocks = 0;
  for (PtrBlockMapEntry = mFvInstance->VolumeHeader->BlockMap;
       PtrBlockMapEntry->NumBlocks != 0;
       PtrBlockMapEntry++) {
    //
    // Get the maximum size of a block.
    //
    if (MaxLbaSize < PtrBlockMapEntry->Length) {
      MaxLbaSize = PtrBlockMapEntry->Length;
    }

    NumOfBlocks = NumOfBlocks + PtrBlockMapEntry->NumBlocks;
  }

  //
  // The total number of blocks in the FV.
  //
  mFvInstance->NumOfBlocks = NumOfBlocks;

  //
  // Add a FVB Protocol Instance
  //
  FvbDevice = AllocateRuntimePool (sizeof (EFI_FW_VOL_BLOCK_DEVICE));
  ASSERT (FvbDevice != NULL);
  CopyMem (FvbDevice, &mFvbDeviceTemplate, sizeof (EFI_FW_VOL_BLOCK_DEVICE));

  //
  // Set up the devicepath
  //
  if (mFvInstance->VolumeHeader->ExtHeaderOffset == 0) {
    FV_MEMMAP_DEVICE_PATH *FvMemmapDevicePath;

    //
    // FV does not contains extension header, then produce MEMMAP_DEVICE_PATH
    //
    FvMemmapDevicePath = AllocateCopyPool (sizeof (FV_MEMMAP_DEVICE_PATH),
                           &mFvMemmapDevicePathTemplate);
    FvMemmapDevicePath->MemMapDevPath.StartingAddress = mFvInstance->FvBase;
    FvMemmapDevicePath->MemMapDevPath.EndingAddress = mFvInstance->FvBase +
      mFvInstance->FvLength - 1;
    FvbDevice->DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)FvMemmapDevicePath;
  } else {
    FV_PIWG_DEVICE_PATH *FvPiwgDevicePath;

    FvPiwgDevicePath = AllocateCopyPool (sizeof (FV_PIWG_DEVICE_PATH),
                         &mFvPIWGDevicePathTemplate);
    CopyGuid (&FvPiwgDevicePath->FvDevPath.FvName,
      (GUID*)(UINTN)(mFvInstance->FvBase + mFvInstance->VolumeHeader->ExtHeaderOffset));
    FvbDevice->DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)FvPiwgDevicePath;
  }

  //
  // Module type specific hook.
  //
  InstallProtocolInterfaces (FvbDevice);

  //
  // Set several PCD values to point to flash.
  //
  PcdStatus = PcdSet64S (PcdFlashNvStorageVariableBase64,
                (UINTN)PcdGet32 (PcdNvStorageVariableBase));
  ASSERT_RETURN_ERROR (PcdStatus);
  PcdStatus = PcdSet32S (PcdFlashNvStorageFtwWorkingBase,
                PcdGet32 (PcdNvStorageFtwWorkingBase));
  ASSERT_RETURN_ERROR (PcdStatus);
  PcdStatus = PcdSet32S (PcdFlashNvStorageFtwSpareBase,
                PcdGet32 (PcdNvStorageFtwSpareBase));
  ASSERT_RETURN_ERROR (PcdStatus);

  InstallFSNotifyHandler ();
  InstallDumpVarEventHandlers ();
  InstallVirtualAddressChangeHandler ();

  return EFI_SUCCESS;
}
