/** @file
 *
 *  Rockchip ATAGS library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef RKATAGSLIB_H__
#define RKATAGSLIB_H__

/*
  These Atags get populated by earlier boot stages (U-Boot, ATF).
  Some may be unavailable depending on the platform.

  It's also possible to write new/existing tags, but we won't support that
  since there are more appropriate ways to pass configuration data around
  from the UEFI.
*/

typedef enum {
  RkAtagSerialMuxM0 = 0,
  RkAtagSerialMuxM1 = 1,
  RkAtagSerialMuxM2 = 2
} RKATAG_SERIAL_MUX_MODE;

typedef enum {
  RkAtagBootDevTypeUnknown       = 0,
  RkAtagBootDevTypeNand          = BIT0,
  RkAtagBootDevTypeEmmc          = BIT1,
  RkAtagBootDevTypeSd0           = BIT2,
  RkAtagBootDevTypeSd1           = BIT3,
  RkAtagBootDevTypeSpiNor        = BIT4,
  RkAtagBootDevTypeSpiNand       = BIT5,
  RkAtagBootDevTypeRam           = BIT6,
  RkAtagBootDevTypeMtdBlkNand    = BIT7,
  RkAtagBootDevTypeMtdBlkSpiNand = BIT8,
  RkAtagBootDevTypeMtdBlkSpiNor  = BIT9,
  RkAtagBootDevTypeSata          = BIT10,
  RkAtagBootDevTypePcie          = BIT11
} RKATAG_BOOTDEV_TYPE;

typedef enum {
  RkAtagBootDevSdCardUnknown = 0,
  RkAtagBootDevSdCardUpdate  = 1
} RKATAG_BOOTDEV_SDCARD_UPDATE;

typedef enum {
  RkAtagDdrMemExtTop = 1
} RKATAG_DDR_MEM_FLAG;

typedef enum {
  RkAtagPubKeyFuseProgrammed = 0x4B415352
} RKATAG_PUB_KEY_FLAG;

typedef enum {
  RkAtagSocInfoFlagTdbt = BIT0
} RKATAG_SOC_INFO_FLAG;

#define BOOT1_PARAM2_BOOT_CPU_HWID_MASK  0x0000001F

#pragma pack(1)

typedef struct {
  UINT32                    Version;
  UINT32                    Enable;
  UINT64                    Address;
  UINT32                    BaudRate;
  RKATAG_SERIAL_MUX_MODE    MuxMode;
  UINT32                    Id;
  UINT32                    Reserved[2];
} RKATAG_SERIAL;

typedef struct {
  UINT32                          Version;
  RKATAG_BOOTDEV_TYPE             DevType;
  UINT32                          DevNum;
  UINT32                          Mode;
  RKATAG_BOOTDEV_SDCARD_UPDATE    SdUpdate;
  UINT32                          Reserved[6];
} RKATAG_BOOTDEV;

typedef struct {
  UINT32                 Count;
  UINT32                 Version;
  UINT64                 Bank[20];
  RKATAG_DDR_MEM_FLAG    Flags;
  UINT32                 Data[2];
} RKATAG_DDR_MEM;

typedef struct {
  UINT32    Version;
  struct {
    CHAR8     Name[8];
    UINT64    PhyAddr;
    UINT32    Size;
    UINT32    Flags;
  } TeeMem;

  struct {
    CHAR8     Name[8];
    UINT64    PhyAddr;
    UINT32    Size;
    UINT32    Flags;
  } DrmMem;

  UINT32    Reserved[15];
} RKATAG_TOS_MEM;

typedef struct {
  UINT32    Version;
  UINT32    Count;
  UINT32    Reserved[4];

  struct {
    CHAR8     Name[16];
    UINT64    Start;
    UINT64    Size;
  } Partition[6];

  UINT32    Reserved1[3];
} RKATAG_RAM_PARTITION;

typedef struct {
  UINT32    Version;
  UINT64    PhyAddr;
  UINT32    Size;
  UINT32    Flags;
  UINT32    Reserved[2];
} RKATAG_ATF_MEM;

typedef struct {
  UINT32    Version;
  UINT32    Len;

  struct {
    UINT32    RsaN[64];
    UINT32    RsaE[64];
    UINT32    RsaC[64];
  } Data;

  UINT32    Flags;
  UINT32    Reserved[5];
} RKATAG_PUB_KEY;

typedef struct {
  UINT32                  Version;
  UINT32                  HexName;
  RKATAG_SOC_INFO_FLAG    Flags;
  UINT32                  Reserved[6];
} RKATAG_SOC_INFO;

typedef struct {
  UINT32    Version;
  UINT32    Param[8];
  UINT32    Reserved[4];
} RKATAG_BOOT1_PARAM;

#pragma pack()

RKATAG_SERIAL *
RkAtagsGetSerial (
  VOID
  );

RKATAG_BOOTDEV *
RkAtagsGetBootDev (
  VOID
  );

RKATAG_DDR_MEM *
RkAtagsGetDdrMem (
  VOID
  );

RKATAG_TOS_MEM *
RkAtagsGetTosMem (
  VOID
  );

RKATAG_RAM_PARTITION *
RkAtagsGetRamPartition (
  VOID
  );

RKATAG_ATF_MEM *
RkAtagsGetAtfMem (
  VOID
  );

RKATAG_PUB_KEY *
RkAtagsGetPubKey (
  VOID
  );

RKATAG_SOC_INFO *
RkAtagsGetSocInfo (
  VOID
  );

RKATAG_BOOT1_PARAM *
RkAtagsGetBoot1Param (
  VOID
  );

#endif /* RKATAGSLIB_H__ */
