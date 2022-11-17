/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SYSHOST_CHIP_COMMON_H_
#define _SYSHOST_CHIP_COMMON_H_

#ifndef SEGMENT_ACCESS
#define SEGMENT_ACCESS
#endif

//
// Steppings
//
#define A0_REV      0x00

//TODO:Need to remove the old ones.  Keep for noe to allow building
#define CPU_HSX              0
#define CPU_IVT              1
#define CPU_BDX              2
#define CPU_SKX              0

// Defines for socketType
//
#define SOCKET_2S   0
#define SOCKET_4S   1
#define SOCKET_HEDT 2

//
// CpuPciAccess
//
#define READ_ACCESS  0
#define WRITE_ACCESS 1

#pragma pack(1)

typedef union {
  struct {
    UINT32 Bit0:1;
    UINT32 Bit1:1;
    UINT32 Bit2:1;
    UINT32 Bit3:1;
    UINT32 Bit4:1;
    UINT32 Bit5:1;
    UINT32 Bit6:1;
    UINT32 Bit7:1;
    UINT32 Bit8:1;
    UINT32 Bit9:1;
    UINT32 Bit10:1;
    UINT32 Bit11:1;
    UINT32 Bit12:1;
    UINT32 Bit13:1;
    UINT32 Bit14:1;
    UINT32 Bit15:1;
    UINT32 Bit16:1;
    UINT32 Bit17:1;
    UINT32 Bit18:1;
    UINT32 Bit19:1;
    UINT32 Bit20:1;
    UINT32 Bit21:1;
    UINT32 Bit22:1;
    UINT32 Bit23:1;
    UINT32 Bit24:1;
    UINT32 Bit25:1;
    UINT32 Bit26:1;
    UINT32 Bit27:1;
    UINT32 Bit28:1;
    UINT32 Bit29:1;
    UINT32 Bit30:1;
    UINT32 Bit31:1;
  } Bits;
    UINT32 Data;
} DUMMY_REG;

#pragma pack()

//
// System previous boot error structure
//
#define MAX_PREV_BOOT_ERR_ENTRIES   15


#endif // _SYSHOST_CHIP_COMMON_H_
