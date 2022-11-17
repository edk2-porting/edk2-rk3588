/** @file
*
*  Copyright 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef MEMORY_INIT_PEI_LIB_H_
#define MEMORY_INIT_PEI_LIB_H_

#include <Uefi.h>

// Specifies the Maximum regions onto which DDR memory can be mapped in
// a Platform
#define MAX_DRAM_REGIONS    3

// Unique SMC call to retrieve the total DDR RAM size installed in system
// and the SOC  memory map regions to which DDR RAM is mapped
// This SMC call works in this way:
// x1 = -1 : return x0: SMC_OK, x1: total DDR Ram size
// x1 >= number of DRAM regions to which DDR RAM is mapped : return x0: SMC_UNK
// 0 <= x1 < number of DRAM regions to which DDR RAM is mapped : return
//           x0: SMC_OK, x1: Base address of DRAM region,
//           x2: Size of DRAM region
#define SMC_DRAM_BANK_INFO  (0xC200FF12)

#define SMC_DRAM_TOTAL_DRAM_ARG1  (-1)

// Return values from SMC calls. return values are always in x0
#define SMC_OK              (0)
#define SMC_UNK             (-1)

// Regions in SOC memory map to which DDR RAM is mapped.
typedef struct {
  UINTN  BaseAddress;
  UINTN  Size;
} DRAM_REGION_INFO;

#endif // MEMORY_INIT_PEI_LIB_H_
