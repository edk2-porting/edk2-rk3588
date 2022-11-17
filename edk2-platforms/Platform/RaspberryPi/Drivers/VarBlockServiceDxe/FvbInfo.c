/** @file
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2006-2014, Intel Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Pi/PiFirmwareVolume.h>
#include <Guid/SystemNvDataGuid.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>

typedef struct {
  UINT64                      FvLength;
  EFI_FIRMWARE_VOLUME_HEADER  FvbInfo;
  EFI_FV_BLOCK_MAP_ENTRY      End[1];
} EFI_FVB_MEDIA_INFO;

EFI_FVB_MEDIA_INFO  mPlatformFvbMediaInfo[] = {
  //
  // System NvStorage FVB
  //
  {
    FixedPcdGet32 (PcdFlashNvStorageVariableSize) +
    FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
    FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize) +
    FixedPcdGet32 (PcdNvStorageEventLogSize),
    {
      {
        0,
      },  // ZeroVector[16]
      EFI_SYSTEM_NV_DATA_FV_GUID,
      FixedPcdGet32 (PcdFlashNvStorageVariableSize) +
      FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
      FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize) +
      FixedPcdGet32 (PcdNvStorageEventLogSize),
      EFI_FVH_SIGNATURE,
      EFI_FVB2_MEMORY_MAPPED |
        EFI_FVB2_READ_ENABLED_CAP |
        EFI_FVB2_READ_STATUS |
        EFI_FVB2_WRITE_ENABLED_CAP |
        EFI_FVB2_WRITE_STATUS |
        EFI_FVB2_ERASE_POLARITY |
        EFI_FVB2_ALIGNMENT_16,
      sizeof (EFI_FIRMWARE_VOLUME_HEADER) + sizeof (EFI_FV_BLOCK_MAP_ENTRY),
      0,  // CheckSum
      0,  // ExtHeaderOffset
      {
        0,
      },  // Reserved[1]
      2,  // Revision
      {
        {
          (FixedPcdGet32 (PcdFlashNvStorageVariableSize) +
           FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
           FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize) +
           FixedPcdGet32 (PcdNvStorageEventLogSize)) /
          FixedPcdGet32 (PcdFirmwareBlockSize),
          FixedPcdGet32 (PcdFirmwareBlockSize),
        }
      } // BlockMap[1]
    },
    {
      {
        0,
        0
      }
    }  // End[1]
  }
};


EFI_STATUS
GetFvbInfo (
  IN  UINT64 FvLength,
  OUT EFI_FIRMWARE_VOLUME_HEADER **FvbInfo
  )
{
  STATIC BOOLEAN Checksummed = FALSE;
  UINTN Index;

  if (!Checksummed) {
    for (Index = 0;
         Index < sizeof (mPlatformFvbMediaInfo) / sizeof (EFI_FVB_MEDIA_INFO);
         Index += 1) {
      UINT16 Checksum;
      mPlatformFvbMediaInfo[Index].FvbInfo.Checksum = 0;
      Checksum = CalculateCheckSum16 (
                   (UINT16*)&mPlatformFvbMediaInfo[Index].FvbInfo,
                   mPlatformFvbMediaInfo[Index].FvbInfo.HeaderLength
                 );
      mPlatformFvbMediaInfo[Index].FvbInfo.Checksum = Checksum;
    }
    Checksummed = TRUE;
  }

  for (Index = 0;
       Index < sizeof (mPlatformFvbMediaInfo) / sizeof (EFI_FVB_MEDIA_INFO);
       Index += 1) {
    if (mPlatformFvbMediaInfo[Index].FvLength == FvLength) {
      *FvbInfo = &mPlatformFvbMediaInfo[Index].FvbInfo;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}
