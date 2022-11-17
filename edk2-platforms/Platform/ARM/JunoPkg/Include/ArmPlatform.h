/** @file
*
*  Copyright (c) 2013-2017, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __ARM_JUNO_H__
#define __ARM_JUNO_H__

//#include <VExpressMotherBoard.h>

/***********************************************************************************
// Platform Memory Map
************************************************************************************/

// Motherboard Peripheral and On-chip peripheral
#define ARM_VE_BOARD_PERIPH_BASE              0x1C010000
#define ARM_VE_BOARD_SYS_ID                   0x0000
#define ARM_VE_BOARD_SYS_PCIE_GBE_L           0x0074
#define ARM_VE_BOARD_SYS_PCIE_GBE_H           0x0078

#define ARM_VE_BOARD_SYS_ID_REV(word)         ((word >> 28) & 0xff)

// NOR Flash 0
#define ARM_VE_SMB_NOR0_BASE                  0x08000000
#define ARM_VE_SMB_NOR0_SZ                    SIZE_64MB

// Off-Chip peripherals (USB, Ethernet, VRAM)
#define ARM_VE_SMB_PERIPH_BASE                0x18000000
#define ARM_VE_SMB_PERIPH_SZ                  (SIZE_64MB + SIZE_2MB)

// On-Chip non-secure ROM
#define ARM_JUNO_NON_SECURE_ROM_BASE          0x1F000000
#define ARM_JUNO_NON_SECURE_ROM_SZ            SIZE_16MB

// On-Chip Peripherals
#define ARM_JUNO_PERIPHERALS_BASE             0x20000000
#define ARM_JUNO_PERIPHERALS_SZ               0x0E000000

// PCIe MSI address window
#define ARM_JUNO_GIV2M_MSI_BASE               0x2c1c0000
#define ARM_JUNO_GIV2M_MSI_SZ                 SIZE_256KB

// PCIe MSI to SPI mapping range
#define ARM_JUNO_GIV2M_MSI_SPI_BASE           224
#define ARM_JUNO_GIV2M_MSI_SPI_COUNT          127 //TRM says last SPI is 351, 351-224=127

// On-Chip non-secure SRAM
#define ARM_JUNO_NON_SECURE_SRAM_BASE         0x2E000000
#define ARM_JUNO_NON_SECURE_SRAM_SZ           SIZE_16MB

// SOC peripherals (HDLCD, UART, I2C, I2S, USB, SMC-PL354, etc)
#define ARM_JUNO_SOC_PERIPHERALS_BASE         0x7FF50000
#define ARM_JUNO_SOC_PERIPHERALS_SZ           (SIZE_64KB * 9)

// 6GB of DRAM from the 64bit address space
#define ARM_JUNO_EXTRA_SYSTEM_MEMORY_BASE     0x0880000000
#define ARM_JUNO_EXTRA_SYSTEM_MEMORY_SZ       (SIZE_2GB + SIZE_4GB)

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_ARM_OEM_ID           'A','R','M','L','T','D'   // OEMID 6 bytes long
#define EFI_ACPI_ARM_OEM_TABLE_ID     SIGNATURE_64('A','R','M','-','J','U','N','O') // OEM table id 8 bytes long
#define EFI_ACPI_ARM_OEM_REVISION     0x20140727
#define EFI_ACPI_ARM_CREATOR_ID       SIGNATURE_32('A','R','M',' ')
#define EFI_ACPI_ARM_CREATOR_REVISION 0x00000099

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define ARM_ACPI_HEADER(Signature, Type, Revision) {              \
    Signature,                      /* UINT32  Signature */       \
    sizeof (Type),                  /* UINT32  Length */          \
    Revision,                       /* UINT8   Revision */        \
    0,                              /* UINT8   Checksum */        \
    { EFI_ACPI_ARM_OEM_ID },        /* UINT8   OemId[6] */        \
    EFI_ACPI_ARM_OEM_TABLE_ID,      /* UINT64  OemTableId */      \
    EFI_ACPI_ARM_OEM_REVISION,      /* UINT32  OemRevision */     \
    EFI_ACPI_ARM_CREATOR_ID,        /* UINT32  CreatorId */       \
    EFI_ACPI_ARM_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }

//
// Hardware platform identifiers
//
#define JUNO_REVISION_PROTOTYPE 0
#define JUNO_REVISION_R0        1
#define JUNO_REVISION_R1        2
#define JUNO_REVISION_R2        3
#define JUNO_REVISION_UKNOWN    0xFF

//
// We detect whether we are running on a Juno r0, r1 or r2
// board at runtime by checking the value of board SYS_ID
//
#define GetJunoRevision(JunoRevision)                                  \
{                                                                      \
  UINT32    SysId;                                                     \
  SysId = MmioRead32 (ARM_VE_BOARD_PERIPH_BASE+ARM_VE_BOARD_SYS_ID);   \
  JunoRevision = ARM_VE_BOARD_SYS_ID_REV( SysId );                     \
}


// Define if the exported ACPI Tables are based on ACPI 5.0 spec or latest
//#define ARM_JUNO_ACPI_5_0

//
// Address of the system registers that contain the MAC address
// assigned to the PCI Gigabyte Ethernet device.
//

#define ARM_JUNO_SYS_PCIGBE_L  (ARM_VE_BOARD_PERIPH_BASE + ARM_VE_BOARD_SYS_PCIE_GBE_L)
#define ARM_JUNO_SYS_PCIGBE_H  (ARM_VE_BOARD_PERIPH_BASE + ARM_VE_BOARD_SYS_PCIE_GBE_H)

/***********************************************************************************
// Motherboard memory-mapped peripherals
************************************************************************************/

// Define MotherBoard SYS flags offsets (from ARM_VE_BOARD_PERIPH_BASE)
#define ARM_VE_SYS_ID_REG                   (ARM_VE_BOARD_PERIPH_BASE + 0x00000)
#define ARM_VE_SYS_SW_REG                   (ARM_VE_BOARD_PERIPH_BASE + 0x00004)
#define ARM_VE_SYS_LED_REG                  (ARM_VE_BOARD_PERIPH_BASE + 0x00008)
#define ARM_VE_SYS_FLAGS_REG                (ARM_VE_BOARD_PERIPH_BASE + 0x00030)
#define ARM_VE_SYS_FLAGS_SET_REG            (ARM_VE_BOARD_PERIPH_BASE + 0x00030)
#define ARM_VE_SYS_FLAGS_CLR_REG            (ARM_VE_BOARD_PERIPH_BASE + 0x00034)
#define ARM_VE_SYS_FLAGS_NV_REG             (ARM_VE_BOARD_PERIPH_BASE + 0x00038)
#define ARM_VE_SYS_FLAGS_NV_SET_REG         (ARM_VE_BOARD_PERIPH_BASE + 0x00038)
#define ARM_VE_SYS_FLAGS_NV_CLR_REG         (ARM_VE_BOARD_PERIPH_BASE + 0x0003C)
#define ARM_VE_SYS_FLASH                    (ARM_VE_BOARD_PERIPH_BASE + 0x0004C)
#define ARM_VE_SYS_CFGSWR_REG               (ARM_VE_BOARD_PERIPH_BASE + 0x00058)
#define ARM_VE_SYS_MISC                     (ARM_VE_BOARD_PERIPH_BASE + 0x00060)
#define ARM_VE_SYS_PROCID0_REG              (ARM_VE_BOARD_PERIPH_BASE + 0x00084)
#define ARM_VE_SYS_PROCID1_REG              (ARM_VE_BOARD_PERIPH_BASE + 0x00088)
#define ARM_VE_SYS_CFGDATA_REG              (ARM_VE_BOARD_PERIPH_BASE + 0x000A0)
#define ARM_VE_SYS_CFGCTRL_REG              (ARM_VE_BOARD_PERIPH_BASE + 0x000A4)
#define ARM_VE_SYS_CFGSTAT_REG              (ARM_VE_BOARD_PERIPH_BASE + 0x000A8)

//
// Sites where the peripheral is fitted
//
#define ARM_VE_UNSUPPORTED                        ~0
#define ARM_VE_MOTHERBOARD_SITE                   0
#define ARM_VE_DAUGHTERBOARD_1_SITE               1
#define ARM_VE_DAUGHTERBOARD_2_SITE               2

#define VIRTUAL_SYS_CFG(site,func)                (((site) << 24) | (func))

//
// System Configuration Control Functions
//
#define SYS_CFG_OSC           1
#define SYS_CFG_VOLT          2
#define SYS_CFG_AMP           3
#define SYS_CFG_TEMP          4
#define SYS_CFG_RESET         5
#define SYS_CFG_SCC           6
#define SYS_CFG_MUXFPGA       7
#define SYS_CFG_SHUTDOWN      8
#define SYS_CFG_REBOOT        9
#define SYS_CFG_DVIMODE       11
#define SYS_CFG_POWER         12
// Oscillator for Site 1
#define SYS_CFG_OSC_SITE1     VIRTUAL_SYS_CFG(ARM_VE_DAUGHTERBOARD_1_SITE, \
                                              SYS_CFG_OSC)
// Oscillator for Site 2
#define SYS_CFG_OSC_SITE2     VIRTUAL_SYS_CFG(ARM_VE_DAUGHTERBOARD_2_SITE, \
                                              SYS_CFG_OSC)
// Can not access the battery backed-up hardware clock on the
// Versatile Express motherboard
#define SYS_CFG_RTC           VIRTUAL_SYS_CFG(ARM_VE_UNSUPPORTED,1)

#endif
