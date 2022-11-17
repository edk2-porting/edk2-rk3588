/** @file
  Physical memory map for SynQuacer

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SYNQUACER_PLATFORM_MEMORYMAP_H_
#define _SYNQUACER_PLATFORM_MEMORYMAP_H_

// Memory mapped SPI NOR
#define SYNQUACER_SPI_NOR_BASE          0x08000000
#define SYNQUACER_SPI_NOR_SIZE          SIZE_128MB

// On-Chip non-secure ROM
#define SYNQUACER_NON_SECURE_ROM_BASE   0x1F000000
#define SYNQUACER_NON_SECURE_ROM_SZ     SIZE_512KB

// On-Chip Peripherals
#define SYNQUACER_PERIPHERALS_BASE      0x20000000
#define SYNQUACER_PERIPHERALS_SZ        0x0E000000

// On-Chip non-secure SRAM
#define SYNQUACER_NON_SECURE_SRAM_BASE  0x2E000000
#define SYNQUACER_NON_SECURE_SRAM_SZ    SIZE_64KB

// GIC-500
#define SYNQUACER_GIC500_DIST_BASE      FixedPcdGet64 (PcdGicDistributorBase)
#define SYNQUACER_GIC500_DIST_SIZE      SIZE_256KB
#define SYNQUACER_GIC500_RDIST_BASE     FixedPcdGet64 (PcdGicRedistributorsBase)
#define SYNQUACER_GIC500_RDIST_SIZE     SIZE_8MB

// GPIO block
#define SYNQUACER_GPIO_BASE             0x51000000
#define SYNQUACER_GPIO_SIZE             SIZE_4KB

// EXIU interrupt controller
#define SYNQUACER_EXIU_BASE             0x510c0000
#define SYNQUACER_EXIU_SIZE             0x20

// I2C0 block
#define SYNQUACER_I2C0_BASE             0x51200000
#define SYNQUACER_I2C0_SIZE             SIZE_4KB

// I2C1 block
#define SYNQUACER_I2C1_BASE             0x51210000
#define SYNQUACER_I2C1_SIZE             SIZE_4KB

// eMMC(SDH30)
#define SYNQUACER_EMMC_BASE             0x52300000
#define SYNQUACER_EMMC_BASE_SZ          SIZE_4KB

#define SYNQUACER_EEPROM_BASE           0x10000000
#define SYNQUACER_EEPROM_BASE_SZ        SIZE_64KB

// NETSEC
#define SYNQUACER_NETSEC1_BASE          0x522D0000
#define SYNQUACER_NETSEC1_BASE_SZ       SIZE_64KB

// PCI
#define SYNQUACER_PCIE_BASE             0x58200000
#define SYNQUACER_PCIE_SIZE             0x00200000

// SCB SMMU
#define SYNQUACER_SCB_SMMU_BASE         0x52E00000
#define SYNQUACER_SCB_SMMU_SIZE         SIZE_64KB

#define SYNQUACER_MMIO_TIMER_CTL_BASE   0x2A810000
#define SYNQUACER_MMIO_TIMER_CNT_BASE0  0x2A830000

// PL011 UART
#define SYNQUACER_UART0_BASE            0x2A400000
#define SYNQUACER_UART0_SIZE            SIZE_4KB

// DesignWare FUART
#define SYNQUACER_UART1_BASE            0x51040000
#define SYNQUACER_UART1_SIZE            SIZE_4KB

// SPI controller #1
#define SYNQUACER_SPI1_BASE             0x54810000
#define SYNQUACER_SPI1_SIZE             SIZE_4KB

// SPI controller #1 MMIO Region
#define SYNQUACER_SPI1_MMIO_BASE        0x10000000
#define SYNQUACER_SPI1_MMIO_SIZE        SIZE_1MB

#endif
