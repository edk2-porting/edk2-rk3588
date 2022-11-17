/** @file
*
*  Copyright (c) 2014, ARM Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __BOOTMON_FS_FILE_INFO_H__
#define __BOOTMON_FS_FILE_INFO_H__

#define BOOTMON_FS_FILE_INFO_ID \
  { \
    0x41e26b9c, 0xada6, 0x45b3, {0x80, 0x8e, 0x23, 0x57, 0xa3, 0x5b, 0x60, 0xd6 } \
  }

// From BootMonFS header file
#define BOOTMONFS_IMAGE_DESCRIPTION_REGION_MAX 4

typedef struct {
  // The size of the structure.
  UINT64    Size;

  // Subset of properties stored in the file meta-data.
  UINT32  EntryPoint;
  UINT32  RegionCount;
  struct {
    UINT32 LoadAddress;
    UINT32 Size;
    UINT32 Offset;
    UINT32 Checksum;
  } Region[BOOTMONFS_IMAGE_DESCRIPTION_REGION_MAX];

} BOOTMON_FS_FILE_INFO;

#define SIZE_OF_BOOTMON_FS_FILE_INFO \
  OFFSET_OF (BOOTMON_FS_FILE_INFO, Region[BOOTMONFS_IMAGE_DESCRIPTION_REGION_MAX - 1].Checksum)

extern EFI_GUID gArmBootMonFsFileInfoGuid;

#endif // __BOOTMON_FS_FILE_INFO_H__
