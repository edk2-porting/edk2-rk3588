/** @file
  SysHostChipCommon.h

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SYSHOST_CHIP_COMMON_H_
#define _SYSHOST_CHIP_COMMON_H_

#include <DataTypes.h>
#include <RcVersion.h>

//
// CpuPciAccess
//
#define READ_ACCESS  0
#define WRITE_ACCESS 1

//
// -----------------------------------------------------------------------------
// Nvram structures
//
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

//
// -----------------------------------------------------------------------------
// Nvram structures
//

struct commonNvram {
  UINT64_STRUCT         cpuFreq;
  RC_VERSION            RcVersion;      // Version of the reference code last executed
  UINT8                 platformType;
  DUMMY_REG             TsegBase;
  DUMMY_REG             TsegLimit;
  DUMMY_REG             MeMemLowBaseAddr;
  DUMMY_REG             MeMemHighBaseAddr;
  DUMMY_REG             MeMemLowLimit;
  DUMMY_REG             MeMemHighLimit;
  DUMMY_REG             MeNcMemLowBaseAddr;
  DUMMY_REG             MeNcMemHighBaseAddr;
  DUMMY_REG             MeNcMemLowLimit;
  DUMMY_REG             MeNcMemHighLimit;
  DUMMY_REG             MeNcMemLowRac;
  DUMMY_REG             MeNcMemLowWac;
  UINT32                IeRequestedSize;
  DUMMY_REG             IeMemLowBaseAddr;
  DUMMY_REG             IeMemHighBaseAddr;
  DUMMY_REG             IeMemLowLimit;
  DUMMY_REG             IeMemHighLimit;
  DUMMY_REG             IeNcMemLowBaseAddr;
  DUMMY_REG             IeNcMemHighBaseAddr;
  DUMMY_REG             IeNcMemLowLimit;
  DUMMY_REG             IeNcMemHighLimit;
  DUMMY_REG             IeNcMemHighRac;
  DUMMY_REG             IeNcMemHighWac;
};
#pragma pack()

#endif // _SYSHOST_CHIP_COMMON_H_
