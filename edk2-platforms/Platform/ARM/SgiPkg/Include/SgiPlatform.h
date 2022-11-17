/** @file
*
*  Copyright (c) 2018-2021, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __SGI_PLATFORM_H__
#define __SGI_PLATFORM_H__

/***********************************************************************************
// Platform Memory Map
************************************************************************************/

// Sub System Peripherals - UART0
#define SGI_SUBSYS_UART0_BASE                     0x2A400000
#define SGI_SUBSYS_UART0_SZ                       0x00010000

// Sub System Peripherals - UART1
#define SGI_SUBSYS_UART1_BASE                     0x2A410000
#define SGI_SUBSYS_UART1_SZ                       0x00010000

// Register offsets into the System Registers Block
#define SGI_SYSPH_SYS_REG_FLASH                   0x4C
#define SGI_SYSPH_SYS_REG_FLASH_RWEN              0x1

// SGI575_VERSION values
#define SGI575_CONF_NUM                           0x3
#define SGI575_PART_NUM                           0x783

//RDN1E1EDGE Platform Identification values
#define RD_N1E1_EDGE_PART_NUM                     0x786
#define RD_N1_EDGE_CONF_ID                        0x1
#define RD_E1_EDGE_CONF_ID                        0x2

// RD-V1 Platform Identification values
#define RD_V1_PART_NUM                            0x78A
#define RD_V1_CONF_ID                             0x1
#define RD_V1_MC_CONF_ID                          0x2

// RD-N2-Cfg1 Platform Identification values
#define RD_N2_CFG1_PART_NUM                       0x7B6
#define RD_N2_CFG1_CONF_ID                        0x1

// RD-N2 Platform Identification values
#define RD_N2_PART_NUM                            0x7B7
#define RD_N2_CONF_ID                             0x1

#define SGI_CONFIG_MASK                           0x0F
#define SGI_CONFIG_SHIFT                          0x1C
#define SGI_PART_NUM_MASK                         0xFFF

#define MULTI_CHIP_MODE_DISABLED                  0x0
#define MULTI_CHIP_MODE_ENABLED                   0x1

// Remote chip address offset (4TB per chip)
#define SGI_REMOTE_CHIP_MEM_OFFSET(n)             ((1ULL << 42) * (n))

// Base address of the DRAM1 block in a remote chip
#define SYSTEM_MEMORY_BASE_REMOTE(ChipId) \
          (SGI_REMOTE_CHIP_MEM_OFFSET (ChipId) + FixedPcdGet64 (PcdSystemMemoryBase))

// Base address of the DRAM2 block in a remote chip
#define DRAM_BLOCK2_BASE_REMOTE(ChipId) \
          (SGI_REMOTE_CHIP_MEM_OFFSET (ChipId) + FixedPcdGet64 (PcdDramBlock2Base))

// ARM platform description data.
typedef struct {
  UINTN  PlatformId;
  UINTN  ConfigId;
  UINTN  MultiChipMode;
} SGI_PLATFORM_DESCRIPTOR;

// Arm SGI/RD Product IDs
typedef enum {
  UnknownId = 0,
  Sgi575,
  RdN1Edge,
  RdN1EdgeX2,
  RdE1Edge,
  RdV1,
  RdV1Mc,
  RdN2,
  RdN2Cfg1
} ARM_RD_PRODUCT_ID;

// Arm ProductId look-up table
typedef struct {
  UINTN  ProductId;
  UINTN  PlatformId;
  UINTN  ConfigId;
  UINTN  MultiChipMode;
} SGI_PRODUCT_ID_LOOKUP;

/**
  Derermine the product ID.

  Determine the product ID by using the data in the Platform ID Descriptor HOB
  to lookup for a matching product ID.

  @retval Zero           Failed identify platform.
  @retval Others         ARM_RD_PRODUCT_ID of the identified platform.
**/
UINT8 SgiGetProductId (VOID);
#endif // __SGI_PLATFORM_H__
