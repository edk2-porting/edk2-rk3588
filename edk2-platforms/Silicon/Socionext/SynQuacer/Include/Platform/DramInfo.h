/** @file
  Data structure for passing DRAM information from lower level firmware

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SYNQUACER_PLATFORM_DRAM_INFO_H_
#define _SYNQUACER_PLATFORM_DRAM_INFO_H_

typedef struct {
  UINT64            Base;
  UINT64            Size;
} DRAM_INFO_ENTRY;

typedef struct {
  UINT32            NumRegions;
  UINT32            Reserved;
  DRAM_INFO_ENTRY   Entry[3];
} DRAM_INFO;

#endif
