/** @file

  Copyright (c) 2021, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef MORELLO_PLATFORM_H_
#define MORELLO_PLATFORM_H_

#define MORELLO_DRAM_BLOCK1_SIZE               SIZE_2GB

// ****************************************************************************
// Platform Memory Map
// ****************************************************************************

// SubSystem Peripherals - UART0
#define MORELLO_UART0_BASE                     0x2A400000
#define MORELLO_UART0_SZ                       SIZE_64KB

// SubSystem Peripherals - UART1
#define MORELLO_UART1_BASE                     0x2A410000
#define MORELLO_UART1_SZ                       SIZE_64KB

// SubSystem Peripherals - Generic Watchdog
#define MORELLO_GENERIC_WDOG_BASE              0x2A440000
#define MORELLO_GENERIC_WDOG_SZ                SIZE_128KB

// SubSystem Peripherals - GIC(600)
#define MORELLO_GIC_BASE                       0x30000000
#define MORELLO_GICR_BASE                      0x300C0000
#define MORELLO_GIC_SZ                         SIZE_256KB
#define MORELLO_GICR_SZ                        SIZE_1MB

// SubSystem non-secure SRAM
#define MORELLO_NON_SECURE_SRAM_BASE           0x06000000
#define MORELLO_NON_SECURE_SRAM_SZ             SIZE_64KB

// AXI Expansion peripherals
#define MORELLO_EXP_PERIPH_BASE                0x1C000000
#define MORELLO_EXP_PERIPH_BASE_SZ             0x1300000

// Platform information structure base address
#define MORELLO_PLAT_INFO_STRUCT_BASE          MORELLO_NON_SECURE_SRAM_BASE

/*
 * Platform information structure stored in Non-secure SRAM. Platform
 * information are passed from the trusted firmware with the below structure
 * format. The elements of MORELLO_PLAT_INFO should be always in sync with
 * the lower level firmware.
 */
#pragma pack(1)

typedef struct {
  UINT64  LocalDdrSize;  ///< Local DDR memory size in Bytes
  UINT64  RemoteDdrSize; ///< Remote DDR memory size in Bytes
  UINT8   SlaveCount;    ///< Slave count in C2C mode
  UINT8   Mode;          ///< 0 - Single Chip, 1 - Chip to Chip (C2C)
} MORELLO_PLAT_INFO;

#pragma pack()

#endif //MORELLO_PLATFORM_H_
