/** @file
  ProcSmbChipCommon.h

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef  _PROC_SMB_CHIP_COMMON_H_
#define  _PROC_SMB_CHIP_COMMON_H_

#include <MemDefaults.h>

//
// Available SMBus clock periods to be programmed.
//
typedef enum {
  SmbClk100K = SMB_CLK_100K,     // 100 Khz in I2C mode; 4Mhz in I3C mode
  SmbClk400K = SMB_CLK_400K,     // 400 Khz in I2C mode; 6Mhz in I3C mode
  SmbClk700K = SMB_CLK_700K,     // 700 Khz in I2C mode; 8Mhz in I3C mode
  SmbClk1M   = SMB_CLK_1M,       // 1 Mhz in I2C mode; 10Mhz in I3C mode
  SmbClkPeriodMax
} SMB_CLOCK_FREQUENCY;

#define MAX_SMB_INSTANCE       2     // Maximum number of SMBUS Instances

#endif // _PROC_SMB_CHIP_COMMON_H_
