/** @file
  Mem common Hearder File

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __MEM_COMMON_H__
#define __MEM_COMMON_H__

#define MAX_CH              ((MAX_IMC)*(MAX_MC_CH))         // Max channels per socket

typedef enum {
  ReservedMemSs,
  Ddr4MemSs = 1,
  Ddr5MemSs = 2,
  LpDdr4MemSs = 3,
  LpDdr5MemSs = 4,
  Hbm2MemSs = 5,
  MrcMstMax,
  MrcMstDelim = MAX_INT32
} MRC_MST;

typedef enum {
  TYPE_SCF_BAR = 0,
  TYPE_PCU_BAR,
  TYPE_MEM_BAR0,
  TYPE_MEM_BAR1,
  TYPE_MEM_BAR2,
  TYPE_MEM_BAR3,
  TYPE_MEM_BAR4,
  TYPE_MEM_BAR5,
  TYPE_MEM_BAR6,
  TYPE_MEM_BAR7,
  TYPE_SBREG_BAR,
  TYPE_MAX_MMIO_BAR
} MMIO_BARS;

#endif //#ifndef __MEM_COMMON_H__
