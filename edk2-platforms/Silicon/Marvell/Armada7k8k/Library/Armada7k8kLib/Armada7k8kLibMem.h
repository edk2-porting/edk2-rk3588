/*******************************************************************************
Copyright (C) 2017 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#define CCU_MC_RCR_REG                  0xf0001700
#define REMAP_EN_MASK                   0x1
#define REMAP_SIZE_OFFS                 20
#define REMAP_SIZE_MASK                 (0xfff << REMAP_SIZE_OFFS)
#define CCU_MC_RTBR_REG                 0xf0001708
#define TARGET_BASE_OFFS                10
#define TARGET_BASE_MASK                (0xfffff << TARGET_BASE_OFFS)

#define DRAM_REMAP_ENABLED \
          (MmioRead32 (CCU_MC_RCR_REG) & REMAP_EN_MASK)
#define DRAM_REMAP_SIZE \
          (MmioRead32 (CCU_MC_RCR_REG) & REMAP_SIZE_MASK) + SIZE_1MB
#define DRAM_REMAP_TARGET \
          (MmioRead32 (CCU_MC_RTBR_REG) << TARGET_BASE_OFFS)
