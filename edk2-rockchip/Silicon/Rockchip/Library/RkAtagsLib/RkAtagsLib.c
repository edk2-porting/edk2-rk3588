/** @file
 *
 *  Rockchip ATAGS library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/RkAtagsLib.h>

#define ATAGS_BASE  0x1FE000
#define ATAGS_SIZE  0x2000

#define ATAG_SIZE_BYTES(Size)  (Size * 4)

#define ATAG_ID_HASH      0x47C6A7E6
#define ATAG_HASH_LENGTH  sizeof (UINT32)

typedef enum {
  RkAtagTypeNone         = 0x00000000,
  RkAtagTypeCore         = 0x54410001,
  RkAtagTypeSerial       = 0x54410050,
  RkAtagTypeBootDev      = 0x54410051,
  RkAtagTypeDdrMem       = 0x54410052,
  RkAtagTypeTosMem       = 0x54410053,
  RkAtagTypeRamPartition = 0x54410054,
  RkAtagTypeAtfMem       = 0x54410055,
  RkAtagTypePubKey       = 0x54410056,
  RkAtagTypeSocInfo      = 0x54410057,
  RkAtagTypeBoot1Param   = 0x54410058,
  RkAtagTypeMax          = 0x544100ff
} RKATAG_TYPE;

#pragma pack(1)

typedef struct {
  UINT32    Flags;
  UINT32    Pagesize;
  UINT32    RootDev;
} RKATAG_CORE;

typedef struct {
  UINT32         Size;
  RKATAG_TYPE    Type;
} RKATAG_HEADER;

#pragma pack()

STATIC
UINT32
ComputeHash (
  IN  VOID    *Buffer,
  IN  UINT32  Length
  )
{
  UINT32  Index;
  UINT32  Hash;
  UINT8   *Data;

  Hash = ATAG_ID_HASH;
  Data = Buffer;

  if (Data == NULL) {
    return Hash;
  }

  for (Index = 0; Index < Length; Index++) {
    Hash ^= ((Hash << 5) + Data[Index] + (Hash >> 2));
  }

  return Hash;
}

STATIC
VOID *
RkAtagsGetTagBuffer (
  IN RKATAG_TYPE  Type
  )
{
  VOID           *TagBuffer;
  RKATAG_HEADER  *TagHeader;
  UINT32         TagHashOffset;
  UINT32         *TagHash;
  UINT32         ComputedTagHash;

  TagBuffer = (VOID *)ATAGS_BASE;
  TagHeader = (RKATAG_HEADER *)TagBuffer;

  if (TagHeader->Type != RkAtagTypeCore) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: ATAGS not initialized!\n",
      __FUNCTION__
      ));
    return NULL;
  }

  if ((Type <= RkAtagTypeCore) || (Type > RkAtagTypeMax)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Type=%x unsupported!\n",
      __FUNCTION__,
      Type
      ));
    return NULL;
  }

  while (TagHeader->Size > 0) {
    if ((UINTN)TagBuffer + ATAG_SIZE_BYTES (TagHeader->Size) - ATAGS_BASE > ATAGS_SIZE) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Type=%x overflown!\n",
        __FUNCTION__,
        TagHeader->Type
        ));
      return NULL;
    }

    if (TagHeader->Type == Type) {
      TagHashOffset = ATAG_SIZE_BYTES (TagHeader->Size) - ATAG_HASH_LENGTH;
      TagHash       = (UINT32 *)(TagBuffer + TagHashOffset);

      if (*TagHash == 0) {
        DEBUG ((
          DEBUG_WARN,
          "%a: Tag hash missing! Type=%x. Ignoring.\n",
          __FUNCTION__,
          TagHeader->Type
          ));
        return TagBuffer + sizeof (RKATAG_HEADER);
      }

      ComputedTagHash = ComputeHash (TagBuffer, TagHashOffset);

      if (ComputedTagHash != *TagHash) {
        DEBUG ((
          DEBUG_ERROR,
          "%a: Corrupted tag hash! Type=%x, TagHash=%x, ComputedTagHash=%x\n",
          __FUNCTION__,
          TagHeader->Type,
          *TagHash,
          ComputedTagHash
          ));
        return NULL;
      }

      return TagBuffer + sizeof (RKATAG_HEADER);
    }

    TagBuffer = (VOID *)((UINT32 *)TagBuffer + TagHeader->Size);
    TagHeader = (RKATAG_HEADER *)TagBuffer;
  }

  DEBUG ((
    DEBUG_ERROR,
    "%a: Tag not found! Type=%x.\n",
    __FUNCTION__,
    Type
    ));
  return NULL;
}

RKATAG_SERIAL *
RkAtagsGetSerial (
  VOID
  )
{
  return (RKATAG_SERIAL *)RkAtagsGetTagBuffer (RkAtagTypeSerial);
}

RKATAG_BOOTDEV *
RkAtagsGetBootDev (
  VOID
  )
{
  return (RKATAG_BOOTDEV *)RkAtagsGetTagBuffer (RkAtagTypeBootDev);
}

RKATAG_DDR_MEM *
RkAtagsGetDdrMem (
  VOID
  )
{
  return (RKATAG_DDR_MEM *)RkAtagsGetTagBuffer (RkAtagTypeDdrMem);
}

RKATAG_TOS_MEM *
RkAtagsGetTosMem (
  VOID
  )
{
  return (RKATAG_TOS_MEM *)RkAtagsGetTagBuffer (RkAtagTypeTosMem);
}

RKATAG_RAM_PARTITION *
RkAtagsGetRamPartition (
  VOID
  )
{
  return (RKATAG_RAM_PARTITION *)RkAtagsGetTagBuffer (RkAtagTypeRamPartition);
}

RKATAG_ATF_MEM *
RkAtagsGetAtfMem (
  VOID
  )
{
  return (RKATAG_ATF_MEM *)RkAtagsGetTagBuffer (RkAtagTypeAtfMem);
}

RKATAG_PUB_KEY *
RkAtagsGetPubKey (
  VOID
  )
{
  return (RKATAG_PUB_KEY *)RkAtagsGetTagBuffer (RkAtagTypePubKey);
}

RKATAG_SOC_INFO *
RkAtagsGetSocInfo (
  VOID
  )
{
  return (RKATAG_SOC_INFO *)RkAtagsGetTagBuffer (RkAtagTypeSocInfo);
}

RKATAG_BOOT1_PARAM *
RkAtagsGetBoot1Param (
  VOID
  )
{
  return (RKATAG_BOOT1_PARAM *)RkAtagsGetTagBuffer (RkAtagTypeBoot1Param);
}
