/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PCIE_INIT_H__
#define __PCIE_INIT_H__

#include "PcieInitLib.h"
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

extern EFI_GUID gEfiPcieRootBridgeProtocolGuid;

#define PCIE_LOG_ID                             1

#define PCIE_CONFIG_SPACE_SIZE                  0x1000      //4k
#define PCIE_MEMORY_SPACE_SIZE                  0x800000        //8M
#define PCIE_IO_SPACE_SIZE          0x800000    //8M
#define PCIE_TYPE1_MEM_SIZE          (PCIE_MEMORY_SPACE_SIZE + PCIE_IO_SPACE_SIZE)

#define CONFIG_SPACE_BASE_ADDR_LOW 0xe2000000
#define CONFIG_SPACE_BASE_ADDR_HIGH 0x0
#define CONFIG_SPACE_ADDR_LIMIT      (CONFIG_SPACE_BASE_ADDR_LOW + PCIE_CONFIG_SPACE_SIZE - 1)

#define PCIE_MEM_BASE_ADDR_LOW   (CONFIG_SPACE_BASE_ADDR_LOW + PCIE_CONFIG_SPACE_SIZE)
#define PCIE_MEM_BASE_ADDR_HIGH 0x0
#define PCIE_MEM_ADDR_LIMIT      (PCIE_MEM_BASE_ADDR_LOW + PCIE_MEMORY_SPACE_SIZE - PCIE_CONFIG_SPACE_SIZE - 1)

#define PCIE_IO_BASE_ADDR_LOW   (PCIE_MEM_ADDR_LIMIT - 1)
#define PCIE_IO_BASE_ADDR_HIGH  0x0
#define PCIE_IO_ADDR_LIMIT      (PCIE_IO_BASE_ADDR_LOW + PCIE_IO_SPACE_SIZE - 1)

#define PCIE_INBOUND_BASE        0xD0000000


#define PCIE_ALL_DMA_BASE               (0x100000000)
#define PCIE0_ALL_DMA_BASE              (PCIE_ALL_DMA_BASE)
#define PCIE0_ALL_DMA_SIZE              (0x8000000)
#define PCIE0_ALL_BAR01_BASE             (0x10000000)
#define PCIE0_ALL_BAR23_BASE             (PCIE0_ALL_BAR01_BASE + PCIE_MAX_AXI_SIZE)
#define PCIE0_ALL_TRANSLATE01_BASE         0x2c0000000  //(HRD_ATTR_TRAN_ADDR_BASE_HOST_ADDR)
#define PCIE0_ALL_TRANSLATE01_SIZE         (PCIE_MAX_AXI_SIZE)
#define PCIE0_ALL_TRANSLATE23_BASE         (PCIE0_ALL_TRANSLATE01_BASE + PCIE0_ALL_TRANSLATE01_SIZE)
#define PCIE0_ALL_TRANSLATE23_SIZE         (PCIE0_ALL_DMA_SIZE)


#define PCIE0_REG_BASE        (0xb0070000)
#define PCIE1_REG_BASE        (0xb0080000)
#define PCIE2_REG_BASE        (0xb0090000)
#define PCIE3_REG_BASE        (0xb00a0000)

#define PCIE_BASE_BAR               (0xf0000000)
#define PCIE_BAR_SIZE                (0x1000000)


#define PCIE_AXI_SIZE         (0x1000000)
#define PCIE0_AXI_BASE        (0xb3000000)
#define PCIE1_AXI_BASE        (PCIE0_AXI_BASE + PCIE_AXI_SIZE)
#define PCIE2_AXI_BASE        (PCIE1_AXI_BASE + PCIE_AXI_SIZE)
#define PCIE3_AXI_BASE        (PCIE2_AXI_BASE + PCIE_AXI_SIZE)

#define PCIE0_CONFIG_BASE     (PCIE1_AXI_BASE - PCIE_CONFIG_SPACE_SIZE)
#define PCIE1_CONFIG_BASE     (PCIE2_AXI_BASE - PCIE_CONFIG_SPACE_SIZE)
#define PCIE2_CONFIG_BASE     (PCIE3_AXI_BASE - PCIE_CONFIG_SPACE_SIZE)
#define PCIE3_CONFIG_BASE     (PCIE3_AXI_BASE +  PCIE_AXI_SIZE - PCIE_CONFIG_SPACE_SIZE)


#define PCIE0_TRANSLATE_BASE            (0x30000000)
#define PCIE1_TRANSLATE_BASE            (PCIE0_TRANSLATE_BASE + PCIE_TRANSLATE_SIZE)
#define PCIE2_TRANSLATE_BASE            (PCIE1_TRANSLATE_BASE + PCIE_TRANSLATE_SIZE)
#define PCIE3_TRANSLATE_BASE            (PCIE2_TRANSLATE_BASE + PCIE_TRANSLATE_SIZE)

#define PCIE0_BAR_BASE              (PCIE0_AXI_BASE)
#define PCIE1_BAR_BASE              (PCIE1_AXI_BASE)
#define PCIE2_BAR_BASE              (PCIE2_AXI_BASE)
#define PCIE3_BAR_BASE              (PCIE3_AXI_BASE)


#endif

