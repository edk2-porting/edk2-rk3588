/** @file
  Read FFS Library.

  @copyright
  Copyright 2009 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __READ_FFS__
#define __READ_FFS__
#include <Base.h>
#include <Pi/PiFirmwareVolume.h>

#define MAX_COMPRESS_ITEM 196
#define MAX_FFS_BUFFER_SIZE  8*1024
#define COMPRESS_DUPLICATE 1
#define COMPRESS_SINGLE    2

#pragma pack(1)
typedef struct {
  UINT16                   Value;
  UINT16                   Length;
  UINT8                    Type;
  UINT16                   Offset;
} COMPRESS_ITEM;

typedef struct {
  UINT32                    Signature;
  UINT32                    Count;
  //COMPRESS_ITEM             item[Count];
} COMPRESS_HOBO_DATA;
#pragma pack()


BOOLEAN NormalHobToCompressHob(IN OUT VOID* hobAddr,IN OUT UINTN* size);
BOOLEAN CompressHobToNormalHob(IN OUT VOID* hobAddr,OUT UINTN* size);

//read a FFS from FV.
UINT8*
PreMemReadFFSFile (
  IN EFI_FIRMWARE_VOLUME_HEADER* FwVolHeader,
  IN EFI_GUID FFSGuid,
  IN UINT32   FFSDataSize,
  IN BOOLEAN skipheader
  );

EFI_STATUS
ReadFFSFile (
  IN EFI_FIRMWARE_VOLUME_HEADER* FwVolHeader,
  IN EFI_GUID FFSGuid,
  IN UINT32   FFSDataSize,
  IN OUT VOID *FFSData,
  OUT UINT32  *FFSSize,
  IN BOOLEAN skipheader
  );
EFI_STATUS ValidateCommonFvHeader (  EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader  );
#endif
