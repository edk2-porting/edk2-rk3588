/** @file
  Common driver source for several Serial Flash devices
  which are compliant with the Intel(R) Serial Flash Interface Compatibility Specification.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpiFvbServiceCommon.h"

//
// Global variable for this FVB driver  which contains
// the private data of all firmware volume block instances
//
FVB_GLOBAL   mFvbModuleGlobal;

//
// This platform driver knows there are multiple FVs on FD.
// Now we only provide FVs on Variable region and MicorCode region for performance issue.
//
FV_INFO mPlatformFvBaseAddress[] = {
  {0, 0}, // {FixedPcdGet32(PcdFlashNvStorageVariableBase), FixedPcdGet32(PcdFlashNvStorageVariableSize)},
  {0, 0}, // {FixedPcdGet32(PcdFlashFvMicrocodeBase), FixedPcdGet32(PcdFlashFvMicrocodeSize)},
  {0, 0}
};

FV_INFO mPlatformDefaultBaseAddress[] = {
  {0, 0}, // {FixedPcdGet32(PcdFlashNvStorageVariableBase), FixedPcdGet32(PcdFlashNvStorageVariableSize)},
  {0, 0}, // {FixedPcdGet32(PcdFlashFvMicrocodeBase), FixedPcdGet32(PcdFlashFvMicrocodeSize)},
  {0, 0}
};

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
    (EFI_PHYSICAL_ADDRESS) 0,
    (EFI_PHYSICAL_ADDRESS) 0,
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

//
// Template structure used when installing FVB protocol
//
EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL mFvbProtocolTemplate = {
  FvbProtocolGetAttributes,
  FvbProtocolSetAttributes,
  FvbProtocolGetPhysicalAddress,
  FvbProtocolGetBlockSize,
  FvbProtocolRead,
  FvbProtocolWrite,
  FvbProtocolEraseBlocks,
  NULL
};

/**
  Get the EFI_FVB_ATTRIBUTES_2 of a FV.

  @param[in]  FvbInstance The pointer to the EFI_FVB_INSTANCE.

  @return     Attributes of the FV identified by FvbInstance.

**/
EFI_FVB_ATTRIBUTES_2
FvbGetVolumeAttributes (
  IN EFI_FVB_INSTANCE         *FvbInstance
  )
{
  return FvbInstance->FvHeader.Attributes;
}

/**
  Retrieves the starting address of an LBA in an FV. It also
  return a few other attribut of the FV.

  @param[in]  FvbInstance     The pointer to the EFI_FVB_INSTANCE.
  @param[in]  Lba             The logical block address
  @param[out] LbaAddress      On output, contains the physical starting address
                              of the Lba
  @param[out] LbaLength       On output, contains the length of the block
  @param[out] NumOfBlocks     A pointer to a caller allocated UINTN in which the
                              number of consecutive blocks starting with Lba is
                              returned. All blocks in this range have a size of
                              BlockSize

  @retval   EFI_SUCCESS Successfully returns
  @retval   EFI_INVALID_PARAMETER Instance not found

**/
EFI_STATUS
FvbGetLbaAddress (
  IN  EFI_FVB_INSTANCE                    *FvbInstance,
  IN  EFI_LBA                             Lba,
  OUT UINTN                               *LbaAddress,
  OUT UINTN                               *LbaLength,
  OUT UINTN                               *NumOfBlocks
  )
{
  UINT32                                  NumBlocks;
  UINT32                                  BlockLength;
  UINTN                                   Offset;
  EFI_LBA                                 StartLba;
  EFI_LBA                                 NextLba;
  EFI_FV_BLOCK_MAP_ENTRY                  *BlockMap;

  StartLba  = 0;
  Offset    = 0;
  BlockMap  = &(FvbInstance->FvHeader.BlockMap[0]);

  //
  // Parse the blockmap of the FV to find which map entry the Lba belongs to
  //
  while (TRUE) {
    NumBlocks   = BlockMap->NumBlocks;
    BlockLength = BlockMap->Length;

    if ( NumBlocks == 0 || BlockLength == 0) {
      return EFI_INVALID_PARAMETER;
    }

    NextLba = StartLba + NumBlocks;

    //
    // The map entry found
    //
    if (Lba >= StartLba && Lba < NextLba) {
      Offset = Offset + (UINTN)MultU64x32((Lba - StartLba), BlockLength);
      if (LbaAddress ) {
        *LbaAddress = FvbInstance->FvBase + Offset;
      }

      if (LbaLength ) {
        *LbaLength = BlockLength;
      }

      if (NumOfBlocks ) {
        *NumOfBlocks = (UINTN)(NextLba - Lba);
      }
      return EFI_SUCCESS;
    }

    StartLba  = NextLba;
    Offset    = Offset + NumBlocks * BlockLength;
    BlockMap++;
  }
}

/**
  Reads specified number of bytes into a buffer from the specified block.

  @param[in]      FvbInstance           The pointer to the EFI_FVB_INSTANCE
  @param[in]      Lba                   The logical block address to be read from
  @param[in]      BlockOffset           Offset into the block at which to begin reading
  @param[in]      NumBytes              Pointer that on input contains the total size of
                                        the buffer. On output, it contains the total number
                                        of bytes read
  @param[in]      Buffer                Pointer to a caller allocated buffer that will be
                                        used to hold the data read


  @retval         EFI_SUCCESS           The firmware volume was read successfully and
                                        contents are in Buffer
  @retval         EFI_BAD_BUFFER_SIZE   Read attempted across a LBA boundary. On output,
                                        NumBytes contains the total number of bytes returned
                                        in Buffer
  @retval         EFI_ACCESS_DENIED     The firmware volume is in the ReadDisabled state
  @retval         EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                        could not be read
  @retval         EFI_INVALID_PARAMETER Instance not found, or NumBytes, Buffer are NULL

**/
EFI_STATUS
FvbReadBlock (
  IN EFI_FVB_INSTANCE                     *FvbInstance,
  IN EFI_LBA                              Lba,
  IN UINTN                                BlockOffset,
  IN OUT UINTN                            *NumBytes,
  IN UINT8                                *Buffer
  )
{
  EFI_FVB_ATTRIBUTES_2                    Attributes;
  UINTN                                   LbaAddress;
  UINTN                                   LbaLength;
  EFI_STATUS                              Status;
  BOOLEAN                                 BadBufferSize = FALSE;

  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (*NumBytes == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FvbGetLbaAddress (FvbInstance, Lba, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Attributes = FvbGetVolumeAttributes (FvbInstance);

  if ((Attributes & EFI_FVB2_READ_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }

  if (BlockOffset > LbaLength) {
   return EFI_INVALID_PARAMETER;
  }

  if (LbaLength < (*NumBytes + BlockOffset)) {
    DEBUG ((DEBUG_INFO,
      "FvReadBlock: Reducing Numbytes from 0x%x to 0x%x\n",
      *NumBytes,
      (UINT32)(LbaLength - BlockOffset))
      );
    *NumBytes     = (UINT32) (LbaLength - BlockOffset);
    BadBufferSize = TRUE;
  }

  Status = SpiFlashRead (LbaAddress + BlockOffset, (UINT32 *)NumBytes, Buffer);

  if (!EFI_ERROR (Status) && BadBufferSize) {
    return EFI_BAD_BUFFER_SIZE;
  } else {
    return Status;
  }
}

/**
  Writes specified number of bytes from the input buffer to the block.

  @param[in]  FvbInstance           The pointer to the EFI_FVB_INSTANCE
  @param[in]  Lba                   The starting logical block index to write to
  @param[in]  BlockOffset           Offset into the block at which to begin writing
  @param[in]  NumBytes              Pointer that on input contains the total size of
                                    the buffer. On output, it contains the total number
                                    of bytes actually written
  @param[in]  Buffer                Pointer to a caller allocated buffer that contains
                                    the source for the write
  @retval     EFI_SUCCESS           The firmware volume was written successfully
  @retval     EFI_BAD_BUFFER_SIZE   Write attempted across a LBA boundary. On output,
                                    NumBytes contains the total number of bytes
                                    actually written
  @retval     EFI_ACCESS_DENIED     The firmware volume is in the WriteDisabled state
  @retval     EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                    could not be written
  @retval     EFI_INVALID_PARAMETER Instance not found, or NumBytes, Buffer are NULL

**/
EFI_STATUS
FvbWriteBlock (
  IN EFI_FVB_INSTANCE                     *FvbInstance,
  IN EFI_LBA                              Lba,
  IN UINTN                                BlockOffset,
  IN OUT UINTN                            *NumBytes,
  IN UINT8                                *Buffer
  )
{
  EFI_FVB_ATTRIBUTES_2                    Attributes;
  UINTN                                   LbaAddress;
  UINTN                                   LbaLength;
  EFI_STATUS                              Status;
  BOOLEAN                                 BadBufferSize = FALSE;

  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (*NumBytes == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FvbGetLbaAddress (FvbInstance, Lba, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Check if the FV is write enabled
  //
  Attributes = FvbGetVolumeAttributes (FvbInstance);
  if ((Attributes & EFI_FVB2_WRITE_STATUS) == 0)  {
    return EFI_ACCESS_DENIED;
  }

  //
  // Perform boundary checks and adjust NumBytes
  //
  if (BlockOffset > LbaLength) {
    return EFI_INVALID_PARAMETER;
  }

  if (LbaLength < (*NumBytes + BlockOffset)) {
    DEBUG ((DEBUG_INFO,
      "FvWriteBlock: Reducing Numbytes from 0x%x to 0x%x\n",
      *NumBytes,
      (UINT32)(LbaLength - BlockOffset))
      );
    *NumBytes     = (UINT32) (LbaLength - BlockOffset);
    BadBufferSize = TRUE;
  }

  Status = SpiFlashWrite (LbaAddress + BlockOffset, (UINT32 *)NumBytes, Buffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SpiFlashLock ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  WriteBackInvalidateDataCacheRange ((VOID *) (LbaAddress + BlockOffset), *NumBytes);

  if (!EFI_ERROR (Status) && BadBufferSize) {
    return EFI_BAD_BUFFER_SIZE;
  } else {
    return Status;
  }
}



/**
  Erases and initializes a firmware volume block.

  @param[in]    FvbInstance       The pointer to the EFI_FVB_INSTANCE
  @param[in]    Lba               The logical block index to be erased

  @retval   EFI_SUCCESS           The erase request was successfully completed
  @retval   EFI_ACCESS_DENIED     The firmware volume is in the WriteDisabled state
  @retval   EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                  could not be written. Firmware device may have been
                                  partially erased
  @retval   EFI_INVALID_PARAMETER Instance not found

**/
EFI_STATUS
FvbEraseBlock (
  IN EFI_FVB_INSTANCE           *FvbInstance,
  IN EFI_LBA                    Lba
  )
{

  EFI_FVB_ATTRIBUTES_2                    Attributes;
  UINTN                                   LbaAddress;
  UINTN                                   LbaLength;
  EFI_STATUS                              Status;

  //
  // Check if the FV is write enabled
  //
  Attributes = FvbGetVolumeAttributes (FvbInstance);

  if( (Attributes & EFI_FVB2_WRITE_STATUS) == 0)  {
    return EFI_ACCESS_DENIED;
  }

  //
  // Get the starting address of the block for erase.
  //
  Status = FvbGetLbaAddress (FvbInstance, Lba, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = SpiFlashBlockErase (LbaAddress, &LbaLength);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SpiFlashLock ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  WriteBackInvalidateDataCacheRange ((VOID *) LbaAddress, LbaLength);

  return Status;
}

/**
  Modifies the current settings of the firmware volume according to the
  input parameter, and returns the new setting of the volume

  @param[in]  FvbInstance           The pointer to the EFI_FVB_INSTANCE.
  @param[in]  Attributes            On input, it is a pointer to EFI_FVB_ATTRIBUTES_2
                                    containing the desired firmware volume settings.
                                    On successful return, it contains the new settings
                                    of the firmware volume

  @retval     EFI_SUCCESS           Successfully returns
  @retval     EFI_ACCESS_DENIED     The volume setting is locked and cannot be modified
  @retval     EFI_INVALID_PARAMETER Instance not found, or The attributes requested are
                                    in conflict with the capabilities as declared in the
                                    firmware volume header

**/
EFI_STATUS
FvbSetVolumeAttributes (
  IN EFI_FVB_INSTANCE                     *FvbInstance,
  IN OUT EFI_FVB_ATTRIBUTES_2             *Attributes
  )
{
  EFI_FVB_ATTRIBUTES_2                      OldAttributes;
  EFI_FVB_ATTRIBUTES_2                      *AttribPtr;
  EFI_FVB_ATTRIBUTES_2                      UnchangedAttributes;
  UINT32                                    Capabilities;
  UINT32                                    OldStatus, NewStatus;

  AttribPtr     = (EFI_FVB_ATTRIBUTES_2 *) &(FvbInstance->FvHeader.Attributes);
  OldAttributes = *AttribPtr;
  Capabilities  = OldAttributes & EFI_FVB2_CAPABILITIES;
  OldStatus     = OldAttributes & EFI_FVB2_STATUS;
  NewStatus     = *Attributes & EFI_FVB2_STATUS;

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
  // Some attributes of FV is read only can *not* be set
  //
  if ((OldAttributes & UnchangedAttributes) ^ (*Attributes & UnchangedAttributes)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // If firmware volume is locked, no status bit can be updated
  //
  if ( OldAttributes & EFI_FVB2_LOCK_STATUS ) {
    if ( OldStatus ^ NewStatus ) {
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

  *AttribPtr  = (*AttribPtr) & (0xFFFFFFFF & (~EFI_FVB2_STATUS));
  *AttribPtr  = (*AttribPtr) | NewStatus;
  *Attributes = *AttribPtr;

  return EFI_SUCCESS;
}

/**
  Check the integrity of firmware volume header

  @param[in]  FvHeader   A pointer to a firmware volume header

  @retval     TRUE          The firmware volume is consistent
  @retval     FALSE         The firmware volume has corrupted.

**/
BOOLEAN
IsFvHeaderValid (
  IN       EFI_PHYSICAL_ADDRESS          FvBase,
  IN CONST EFI_FIRMWARE_VOLUME_HEADER    *FvHeader
  )
{
  if (FvBase == PcdGet32(PcdFlashNvStorageVariableBase)) {
    if (CompareMem (&FvHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid, sizeof(EFI_GUID)) != 0 ) {
      return FALSE;
    }
  } else {
    if (CompareMem (&FvHeader->FileSystemGuid, &gEfiFirmwareFileSystem2Guid, sizeof(EFI_GUID)) != 0 ) {
      return FALSE;
    }
  }
  if ( (FvHeader->Revision != EFI_FVH_REVISION)   ||
       (FvHeader->Signature != EFI_FVH_SIGNATURE) ||
       (FvHeader->FvLength == ((UINTN) -1))       ||
       ((FvHeader->HeaderLength & 0x01 ) !=0) )  {
    return FALSE;
  }

  if (CalculateCheckSum16 ((UINT16 *) FvHeader, FvHeader->HeaderLength) != 0) {
    return FALSE;
  }

  return TRUE;
}

//
// FVB protocol APIs
//

/**
  Retrieves the physical address of the device.

  @param[in]  This    A pointer to EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL.
  @param[out] Address Output buffer containing the address.

  retval      EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolGetPhysicalAddress (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *This,
  OUT EFI_PHYSICAL_ADDRESS                     *Address
  )
{
  EFI_FVB_INSTANCE      *FvbInstance;

  FvbInstance = FVB_INSTANCE_FROM_THIS (This);

  *Address = FvbInstance->FvBase;

  return EFI_SUCCESS;
}

/**
  Retrieve the size of a logical block

  @param[in]  This        Calling context
  @param[in]  Lba         Indicates which block to return the size for.
  @param[out] BlockSize   A pointer to a caller allocated UINTN in which
                          the size of the block is returned
  @param[out] NumOfBlocks A pointer to a caller allocated UINTN in which the
                          number of consecutive blocks starting with Lba is
                          returned. All blocks in this range have a size of
                          BlockSize

  @retval     EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolGetBlockSize (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *This,
  IN  EFI_LBA                                  Lba,
  OUT UINTN                                    *BlockSize,
  OUT UINTN                                    *NumOfBlocks
  )
{
  EFI_FVB_INSTANCE                 *FvbInstance;

  FvbInstance = FVB_INSTANCE_FROM_THIS (This);

  DEBUG((DEBUG_INFO,
    "FvbProtocolGetBlockSize: Lba: 0x%lx BlockSize: 0x%x NumOfBlocks: 0x%x\n",
    Lba,
    BlockSize,
    NumOfBlocks)
    );

  return FvbGetLbaAddress (
           FvbInstance,
           Lba,
           NULL,
           BlockSize,
           NumOfBlocks
           );
}

/**
  Retrieves Volume attributes.  No polarity translations are done.

  @param[in]    This        Calling context
  @param[out]   Attributes  Output buffer which contains attributes

  @retval       EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolGetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  OUT EFI_FVB_ATTRIBUTES_2                *Attributes
  )
{
  EFI_FVB_INSTANCE                 *FvbInstance;

  FvbInstance = FVB_INSTANCE_FROM_THIS (This);

  *Attributes = FvbGetVolumeAttributes (FvbInstance);

  DEBUG ((DEBUG_INFO,
    "FvbProtocolGetAttributes: This: 0x%x Attributes: 0x%x\n",
    This,
    *Attributes)
    );

  return EFI_SUCCESS;
}

/**
  Sets Volume attributes. No polarity translations are done.

  @param[in]  This        Calling context
  @param[out] Attributes  Output buffer which contains attributes

  @retval     EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolSetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN OUT EFI_FVB_ATTRIBUTES_2                   *Attributes
  )
{
  EFI_STATUS                       Status;
  EFI_FVB_INSTANCE                 *FvbInstance;

  DEBUG((DEBUG_INFO,
    "FvbProtocolSetAttributes: Before SET -  This: 0x%x Attributes: 0x%x\n",
    This,
    *Attributes)
    );

  FvbInstance  = FVB_INSTANCE_FROM_THIS (This);

  Status = FvbSetVolumeAttributes (FvbInstance, Attributes);

  DEBUG((DEBUG_INFO,
    "FvbProtocolSetAttributes: After SET -  This: 0x%x Attributes: 0x%x\n",
    This,
    *Attributes)
    );

  return Status;
}

/**
  The EraseBlock() function erases one or more blocks as denoted by the
  variable argument list. The entire parameter list of blocks must be verified
  prior to erasing any blocks.  If a block is requested that does not exist
  within the associated firmware volume (it has a larger index than the last
  block of the firmware volume), the EraseBlock() function must return
  EFI_INVALID_PARAMETER without modifying the contents of the firmware volume.

  @param[in] This         Calling context
  @param[in] ...          Starting LBA followed by Number of Lba to erase.
                          a -1 to terminate the list.

  @retval EFI_SUCCESS       The erase request was successfully completed
  @retval EFI_ACCESS_DENIED The firmware volume is in the WriteDisabled state
  @retval EFI_DEVICE_ERROR  The block device is not functioning correctly and
                            could not be written. Firmware device may have been
                            partially erased

**/
EFI_STATUS
EFIAPI
FvbProtocolEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *This,
  ...
  )
{
  EFI_FVB_INSTANCE                      *FvbInstance;
  UINTN                                 NumOfBlocks;
  VA_LIST                               Args;
  EFI_LBA                               StartingLba;
  UINTN                                 NumOfLba;
  EFI_STATUS                            Status;

  DEBUG((DEBUG_INFO, "FvbProtocolEraseBlocks: \n"));

  FvbInstance  = FVB_INSTANCE_FROM_THIS (This);

  NumOfBlocks = FvbInstance->NumOfBlocks;

  VA_START (Args, This);

  do {
    StartingLba = VA_ARG (Args, EFI_LBA);
    if ( StartingLba == EFI_LBA_LIST_TERMINATOR ) {
      break;
    }

    NumOfLba = VA_ARG (Args, UINT32);

    //
    // Check input parameters
    //
    if (NumOfLba == 0) {
      VA_END (Args);
      return EFI_INVALID_PARAMETER;
    }

    if ( ( StartingLba + NumOfLba ) > NumOfBlocks ) {
      return EFI_INVALID_PARAMETER;
    }
  } while ( 1 );

  VA_END (Args);

  VA_START (Args, This);
  do {
    StartingLba = VA_ARG (Args, EFI_LBA);
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      break;
    }

    NumOfLba = VA_ARG (Args, UINT32);

    while ( NumOfLba > 0 ) {
      Status = FvbEraseBlock (FvbInstance, StartingLba);
      if ( EFI_ERROR(Status)) {
        VA_END (Args);
        return Status;
      }
      StartingLba ++;
      NumOfLba --;
    }

  } while ( 1 );

  VA_END (Args);

  return EFI_SUCCESS;
}

/**
  Writes data beginning at Lba:Offset from FV. The write terminates either
  when *NumBytes of data have been written, or when a block boundary is
  reached.  *NumBytes is updated to reflect the actual number of bytes
  written. The write opertion does not include erase. This routine will
  attempt to write only the specified bytes. If the writes do not stick,
  it will return an error.

  @param[in]      This      Calling context
  @param[in]      Lba       Block in which to begin write
  @param[in]      Offset    Offset in the block at which to begin write
  @param[in,out]  NumBytes  On input, indicates the requested write size. On
                            output, indicates the actual number of bytes written
  @param[in]      Buffer    Buffer containing source data for the write.

  @retval EFI_SUCCESS           The firmware volume was written successfully
  @retval EFI_BAD_BUFFER_SIZE   Write attempted across a LBA boundary. On output,
                                NumBytes contains the total number of bytes
                                actually written
  @retval EFI_ACCESS_DENIED     The firmware volume is in the WriteDisabled state
  @retval EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                could not be written
  @retval EFI_INVALID_PARAMETER NumBytes or Buffer are NULL

**/
EFI_STATUS
EFIAPI
FvbProtocolWrite (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN EFI_LBA                                    Lba,
  IN UINTN                                      Offset,
  IN OUT UINTN                                  *NumBytes,
  IN UINT8                                      *Buffer
  )
{
  EFI_FVB_INSTANCE        *FvbInstance;

  FvbInstance = FVB_INSTANCE_FROM_THIS (This);

  DEBUG((DEBUG_INFO,
    "FvbProtocolWrite: Lba: 0x%lx Offset: 0x%x NumBytes: 0x%x, Buffer: 0x%x\n",
    Lba,
    Offset,
    *NumBytes,
    Buffer)
    );

  return FvbWriteBlock (FvbInstance, Lba, Offset, NumBytes, Buffer);
}

/**
  Reads data beginning at Lba:Offset from FV. The Read terminates either
  when *NumBytes of data have been read, or when a block boundary is
  reached.  *NumBytes is updated to reflect the actual number of bytes
  written. The write opertion does not include erase. This routine will
  attempt to write only the specified bytes. If the writes do not stick,
  it will return an error.

  @param[in]      This      Calling context
  @param[in]      Lba       Block in which to begin write
  @param[in]      Offset    Offset in the block at which to begin write
  @param[in,out]  NumBytes  On input, indicates the requested write size. On
                            output, indicates the actual number of bytes written
  @param[in]      Buffer    Buffer containing source data for the write.

  @retval EFI_SUCCESS           The firmware volume was read successfully and
                                contents are in Buffer
  @retval EFI_BAD_BUFFER_SIZE   Read attempted across a LBA boundary. On output,
                                NumBytes contains the total number of bytes returned
                                in Buffer
  @retval EFI_ACCESS_DENIED     The firmware volume is in the ReadDisabled state
  @retval EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                could not be read
  @retval EFI_INVALID_PARAMETER NumBytes or Buffer are NULL

**/
EFI_STATUS
EFIAPI
FvbProtocolRead (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN EFI_LBA                                    Lba,
  IN UINTN                                      Offset,
  IN OUT UINTN                                  *NumBytes,
  OUT UINT8                                     *Buffer
  )
{
  EFI_FVB_INSTANCE     *FvbInstance;
  EFI_STATUS           Status;

  FvbInstance = FVB_INSTANCE_FROM_THIS (This);
  Status = FvbReadBlock (FvbInstance, Lba, Offset, NumBytes, Buffer);
  DEBUG((DEBUG_INFO,
    "FvbProtocolRead: Lba: 0x%lx Offset: 0x%x NumBytes: 0x%x, Buffer: 0x%x\n",
    Lba,
    Offset,
    *NumBytes,
    Buffer)
    );

  return Status;
}
