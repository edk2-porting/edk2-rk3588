/*
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
*/
#include <RdkBootManagerLib.h>

/* See sparse_format.h in AOSP  */
#define SPARSE_HEADER_MAGIC       0xed26ff3a
#define CHUNK_TYPE_RAW            0xCAC1
#define CHUNK_TYPE_FILL           0xCAC2
#define CHUNK_TYPE_DONT_CARE      0xCAC3
#define CHUNK_TYPE_CRC32          0xCAC4

#define PARTITION_NAME_MAX_LENGTH     72/2

#define FLASH_DEVICE_PATH_SIZE(DevPath) ( GetDevicePathSize (DevPath) - \
    sizeof (EFI_DEVICE_PATH_PROTOCOL))

#define IS_ALPHA(Char) (((Char) <= L'z' && (Char) >= L'a') || \
    ((Char) <= L'Z' && (Char) >= L'Z'))

typedef struct _DISKIO_PARTITION_LIST {
  LIST_ENTRY  Link;
  CHAR16      PartitionName[PARTITION_NAME_MAX_LENGTH];
  EFI_HANDLE  PartitionHandle;
} DISKIO_PARTITION_LIST;

typedef struct _SPARSE_HEADER {
  UINT32    Magic;
  UINT16    MajorVersion;
  UINT16    MinorVersion;
  UINT16    FileHeaderSize;
  UINT16    ChunkHeaderSize;
  UINT32    BlockSize;
  UINT32    TotalBlocks;
  UINT32    TotalChunks;
  UINT32    ImageChecksum;
} SPARSE_HEADER;

typedef struct _CHUNK_HEADER {
  UINT16    ChunkType;
  UINT16    Reserved1;
  UINT32    ChunkSize;
  UINT32    TotalSize;
} CHUNK_HEADER;

STATIC LIST_ENTRY       mPartitionListHead;
STATIC EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *mTextOut;

/*
 * Helper to free the partition list
 */
STATIC
VOID
FreePartitionList (
  VOID
)
{
  DISKIO_PARTITION_LIST *Entry;
  DISKIO_PARTITION_LIST *NextEntry;

  Entry = (DISKIO_PARTITION_LIST *)GetFirstNode (&mPartitionListHead);
  while (!IsNull (&mPartitionListHead, &Entry->Link)) {
    NextEntry = (DISKIO_PARTITION_LIST *) GetNextNode (\
      &mPartitionListHead, &Entry->Link);

    RemoveEntryList (&Entry->Link);
    FreePool (Entry);

    Entry = NextEntry;
  }
}

/*
 * lists the available Block Io and adds handle of given dev path
 */
STATIC
EFI_STATUS
ListBlockIos (
  IN CHAR16       *PartitionName
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        *AllHandles;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL  *DevPathToText;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  UINTN                             LoopIndex;
  UINTN                             NumHandles;
  UINT16                            *DeviceFullPath;
  DISKIO_PARTITION_LIST             *Entry;
  RETURN_STATUS                     RetStatus;

  InitializeListHead (&mPartitionListHead);

  Status = gBS->LocateProtocol (
    &gEfiDevicePathToTextProtocolGuid,
    NULL,
    (VOID **) &DevPathToText
    );
  ASSERT_EFI_ERROR (Status);

  // Get every Block IO protocol instance installed in the system
  Status = gBS->LocateHandleBuffer (
    ByProtocol,
    &gEfiBlockIoProtocolGuid,
    NULL,
    &NumHandles,
    &AllHandles
    );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "Block IO: %d handles \n", NumHandles));

  // Get HTTP driver handle from AllHandles
  for (LoopIndex = 0; LoopIndex < NumHandles; LoopIndex++) {
    // Get the device path for the handle
    Status = gBS->OpenProtocol (
      AllHandles[LoopIndex],
      &gEfiDevicePathProtocolGuid,
      (VOID **) &DevicePath,
      gImageHandle,
      NULL,
      EFI_OPEN_PROTOCOL_GET_PROTOCOL
      );

    DeviceFullPath = DevPathToText->ConvertDevicePathToText (
      DevicePath,
      FALSE,
      TRUE
      );

    DEBUG((DEBUG_INFO,"Handle[%d] is %p, fullpath %s\n", \
      LoopIndex, AllHandles[LoopIndex], DeviceFullPath));

    if (StrCmp (PartitionName, DeviceFullPath) == 0) {
      DEBUG((DEBUG_INFO, "rootfs partition path matched\n"));
      //
      // Add the partition handle to the list
      //
      // Create entry
      Entry = AllocatePool (sizeof (DISKIO_PARTITION_LIST));
      if (Entry == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }

      // Copy handle and partition name
      Entry->PartitionHandle = AllHandles[LoopIndex];
      RetStatus = StrnCpyS (
        Entry->PartitionName,
        PARTITION_NAME_MAX_LENGTH,
        PartitionName,
        PARTITION_NAME_MAX_LENGTH - 1
      );
      ASSERT_RETURN_ERROR (RetStatus);
      InsertTailList (&mPartitionListHead, &Entry->Link);
      break;
    }
  }
  FreePool (AllHandles);
  ASSERT (LoopIndex < NumHandles);
Exit:
  return Status;
}

STATIC
EFI_STATUS
OpenPartition (
  IN  CHAR8       *PartitionName,
  IN  VOID        *Image,
  IN  UINTN       Size,
  OUT EFI_BLOCK_IO_PROTOCOL     **BlockIo,
  OUT EFI_DISK_IO_PROTOCOL      **DiskIo
  )
{
  EFI_STATUS               Status;
  UINTN                    PartitionSize;
  DISKIO_PARTITION_LIST    *Entry;
  SPARSE_HEADER            *SparseHeader;
  UINT16                   UnicodePartitionName[100];
  RETURN_STATUS            RetStatus;

  RetStatus = AsciiStrToUnicodeStrS (PartitionName, UnicodePartitionName,
                sizeof (UnicodePartitionName));
  if (RETURN_ERROR (RetStatus)) {
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG((DEBUG_INFO, "Unicode partition name %s\n", UnicodePartitionName));

  Status = ListBlockIos (UnicodePartitionName);
  ASSERT_EFI_ERROR (Status);

  Entry = (DISKIO_PARTITION_LIST *)GetFirstNode (&(mPartitionListHead));
  ASSERT (Entry != NULL);

  Status = gBS->OpenProtocol (
    Entry->PartitionHandle,
    &gEfiBlockIoProtocolGuid,
    (VOID **) BlockIo,
    gImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to open Block IO protocol: %r\n", Status));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  SparseHeader = (SPARSE_HEADER *)Image;

  if (SparseHeader->Magic == SPARSE_HEADER_MAGIC) {
    DEBUG ((DEBUG_INFO, \
      "Sparse Magic: 0x%x Major: %d Minor: %d fhs: %d chs: %d bs: %d tbs: %d tcs: %d checksum: %d \n", \
      SparseHeader->Magic, SparseHeader->MajorVersion, \
      SparseHeader->MinorVersion,  SparseHeader->FileHeaderSize, \
      SparseHeader->ChunkHeaderSize, SparseHeader->BlockSize, \
      SparseHeader->TotalBlocks, \
      SparseHeader->TotalChunks, SparseHeader->ImageChecksum));

    if (SparseHeader->MajorVersion != 1) {
      DEBUG ((DEBUG_ERROR, "Sparse image version %d.%d not supported.\n",
            SparseHeader->MajorVersion, SparseHeader->MinorVersion));
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }

    Size = SparseHeader->BlockSize * SparseHeader->TotalBlocks;
  }

  // Check image will fit on device
  PartitionSize = (BlockIo[0]->Media->LastBlock + 1) * BlockIo[0]->Media->BlockSize;
  if (PartitionSize < Size) {
    DEBUG ((DEBUG_ERROR, "Partition not big enough.\n"));
    DEBUG ((DEBUG_ERROR, \
      "Partition Size:\t%ld\nImage Size:\t%ld\n", PartitionSize, Size));

    Status = EFI_VOLUME_FULL;
    goto Exit;
  }

  Status = gBS->OpenProtocol (
    Entry->PartitionHandle,
    &gEfiDiskIoProtocolGuid,
    (VOID **) DiskIo,
    gImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );

Exit:
  FreePartitionList ();
  return Status;
}

EFI_STATUS
PartitionRead (
  IN CHAR8  *PartitionName,
  IN VOID   *Image,
  IN UINTN  Size
  )
{
  EFI_STATUS               Status;
  EFI_BLOCK_IO_PROTOCOL    *BlockIo;
  EFI_DISK_IO_PROTOCOL     *DiskIo;
  UINT32                   MediaId;

  Status = OpenPartition (PartitionName, Image, Size, &BlockIo, &DiskIo);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  MediaId = BlockIo->Media->MediaId;

  Status = DiskIo->ReadDisk (DiskIo, MediaId, 0, Size, Image);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  BlockIo->FlushBlocks(BlockIo);

Exit:
  return Status;
}

EFI_STATUS
PartitionWrite (
  IN CHAR8  *PartitionName,
  IN VOID   *Image,
  IN UINTN  Size
  )
{
  EFI_STATUS               Status;
  EFI_BLOCK_IO_PROTOCOL    *BlockIo;
  EFI_DISK_IO_PROTOCOL     *DiskIo;
  UINT32                   MediaId;
  SPARSE_HEADER            *SparseHeader;
  CHUNK_HEADER             *ChunkHeader;
  UINT32                   Chunk;
  UINTN                    Offset;

  Status = OpenPartition (PartitionName, Image, Size, &BlockIo, &DiskIo);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Offset = 0;
  MediaId = BlockIo->Media->MediaId;
  SparseHeader = (SPARSE_HEADER *)Image;

  if (SparseHeader->Magic == SPARSE_HEADER_MAGIC) {
    CHAR16 OutputString[64];
    UINTN ChunkPrintDensity =
      SparseHeader->TotalChunks > 1600 ? SparseHeader->TotalChunks / 200 : 32;

    Image += SparseHeader->FileHeaderSize;
    for (Chunk = 0; Chunk < SparseHeader->TotalChunks; Chunk++) {
      UINTN WriteSize;
      ChunkHeader = (CHUNK_HEADER *)Image;

      // Show progress. Don't do it for every packet as outputting text
      // might be time consuming. ChunkPrintDensity is calculated to
      // provide an update every half percent change for large
      // downloads.
      if (Chunk % ChunkPrintDensity == 0) {
        UnicodeSPrint(OutputString, sizeof(OutputString),
          L"\r%5d / %5d chunks written (%d%%)", Chunk,
          SparseHeader->TotalChunks,
          (Chunk * 100) / SparseHeader->TotalChunks);
        mTextOut->OutputString(mTextOut, OutputString);
      }

      DEBUG ((DEBUG_INFO, "Chunk #%d - Type: 0x%x Size: %d TotalSize: %d Offset %d\n",
        (Chunk+1), ChunkHeader->ChunkType, ChunkHeader->ChunkSize,
        ChunkHeader->TotalSize, Offset));
      Image += sizeof(CHUNK_HEADER);
      WriteSize = (SparseHeader->BlockSize) * ChunkHeader->ChunkSize;
      switch (ChunkHeader->ChunkType) {
        case CHUNK_TYPE_RAW:
          DEBUG ((DEBUG_INFO, "Writing %d at Offset %d\n", WriteSize, Offset));
          Status = DiskIo->WriteDisk (DiskIo, MediaId, Offset, WriteSize, Image);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          Image += WriteSize;
          break;
        case CHUNK_TYPE_DONT_CARE:
          break;
        case CHUNK_TYPE_CRC32:
          break;
        default:
          DEBUG ((DEBUG_ERROR, "Unknown Chunk Type: 0x%x", ChunkHeader->ChunkType));
          Status = EFI_PROTOCOL_ERROR;
          goto Exit;
      }
      Offset += WriteSize;
    }

    UnicodeSPrint (OutputString, sizeof (OutputString),
        L"\r%5d / %5d chunks written (100%%)\r\n",
        SparseHeader->TotalChunks, SparseHeader->TotalChunks);
    mTextOut->OutputString(mTextOut, OutputString);

  } else {

    Status = DiskIo->WriteDisk (DiskIo, MediaId, 0, Size, Image);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
  }

  BlockIo->FlushBlocks (BlockIo);

Exit:
  return Status;
}
