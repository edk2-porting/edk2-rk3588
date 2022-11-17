/** @file
  Register names for PCH Smbus Device.

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used .
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_SMBUS_H_
#define _PCH_REGS_SMBUS_H_

//
// SMBus Controller Registers (D31:F4)
//
#define PCI_DEVICE_NUMBER_PCH_SMBUS           31
#define PCI_FUNCTION_NUMBER_PCH_SMBUS         4
#define R_SMBUS_CFG_BASE                      0x20
#define V_SMBUS_CFG_BASE_SIZE                 (1 << 5)
#define B_SMBUS_CFG_BASE_BAR                  0x0000FFE0
#define R_SMBUS_CFG_HOSTC                     0x40
#define B_SMBUS_CFG_HOSTC_SPDWD               BIT4
#define B_SMBUS_CFG_HOSTC_SSRESET             BIT3
#define B_SMBUS_CFG_HOSTC_I2C_EN              BIT2
#define B_SMBUS_CFG_HOSTC_SMI_EN              BIT1
#define B_SMBUS_CFG_HOSTC_HST_EN              BIT0
#define R_SMBUS_CFG_TCOBASE                   0x50
#define B_SMBUS_CFG_TCOBASE_BAR               0x0000FFE0
#define R_SMBUS_CFG_TCOCTL                    0x54
#define B_SMBUS_CFG_TCOCTL_TCO_BASE_EN        BIT8
#define B_SMBUS_CFG_TCOCTL_TCO_BASE_LOCK      BIT0
#define R_SMBUS_CFG_64                        0x64
#define R_SMBUS_CFG_80                        0x80

//
// SMBus I/O Registers
//
#define R_SMBUS_IO_HSTS                  0x00  ///< Host Status Register R/W
#define B_SMBUS_IO_HBSY                  0x01
#define B_SMBUS_IO_INTR                  0x02
#define B_SMBUS_IO_DERR                  0x04
#define B_SMBUS_IO_BERR                  0x08
#define B_SMBUS_IO_FAIL                  0x10
#define B_SMBUS_IO_SMBALERT_STS          0x20
#define B_SMBUS_IO_IUS                   0x40
#define B_SMBUS_IO_BYTE_DONE_STS         0x80
#define B_SMBUS_IO_ERROR                 (B_SMBUS_IO_DERR | B_SMBUS_IO_BERR | B_SMBUS_IO_FAIL)
#define B_SMBUS_IO_HSTS_ALL              0xFF
#define R_SMBUS_IO_HCTL                  0x02  ///< Host Control Register R/W
#define B_SMBUS_IO_INTREN                0x01
#define B_SMBUS_IO_KILL                  0x02
#define B_SMBUS_IO_SMB_CMD               0x1C
#define V_SMBUS_IO_SMB_CMD_QUICK         0x00
#define V_SMBUS_IO_SMB_CMD_BYTE          0x04
#define V_SMBUS_IO_SMB_CMD_BYTE_DATA     0x08
#define V_SMBUS_IO_SMB_CMD_WORD_DATA     0x0C
#define V_SMBUS_IO_SMB_CMD_PROCESS_CALL  0x10
#define V_SMBUS_IO_SMB_CMD_BLOCK         0x14
#define V_SMBUS_IO_SMB_CMD_IIC_READ      0x18
#define V_SMBUS_IO_SMB_CMD_BLOCK_PROCESS 0x1C
#define B_SMBUS_IO_LAST_BYTE             0x20
#define B_SMBUS_IO_START                 0x40
#define B_SMBUS_IO_PEC_EN                0x80
#define R_SMBUS_IO_HCMD                  0x03  ///< Host Command Register R/W
#define R_SMBUS_IO_TSA                   0x04  ///< Transmit Slave Address Register R/W
#define B_SMBUS_IO_RW_SEL                0x01
#define B_SMBUS_IO_READ                  0x01  // RW
#define B_SMBUS_IO_WRITE                 0x00  // RW
#define B_SMBUS_IO_ADDRESS               0xFE
#define R_SMBUS_IO_HD0                   0x05  ///< Data 0 Register R/W
#define R_SMBUS_IO_HD1                   0x06  ///< Data 1 Register R/W
#define R_SMBUS_IO_HBD                   0x07  ///< Host Block Data Register R/W
#define R_SMBUS_IO_PEC                   0x08  ///< Packet Error Check Data Register R/W
#define R_SMBUS_IO_RSA                   0x09  ///< Receive Slave Address Register R/W
#define B_SMBUS_IO_SLAVE_ADDR            0x7F
#define R_SMBUS_IO_SD                    0x0A  ///< Receive Slave Data Register R/W
#define R_SMBUS_IO_AUXS                  0x0C  ///< Auxiliary Status Register R/WC
#define B_SMBUS_IO_CRCE                  0x01
#define B_SMBUS_IO_STCO                  0x02  ///< SMBus TCO Mode
#define R_SMBUS_IO_AUXC                  0x0D  ///< Auxiliary Control Register R/W
#define B_SMBUS_IO_AAC                   0x01
#define B_SMBUS_IO_E32B                  0x02
#define R_SMBUS_IO_SMLC                  0x0E  ///< SMLINK Pin Control Register R/W
#define B_SMBUS_IO_SMLINK0_CUR_STS       0x01
#define B_SMBUS_IO_SMLINK1_CUR_STS       0x02
#define B_SMBUS_IO_SMLINK_CLK_CTL        0x04
#define R_SMBUS_IO_SMBC                  0x0F  ///< SMBus Pin Control Register R/W
#define B_SMBUS_IO_SMBCLK_CUR_STS        0x01
#define B_SMBUS_IO_SMBDATA_CUR_STS       0x02
#define B_SMBUS_IO_SMBCLK_CTL            0x04
#define R_SMBUS_IO_SSTS                  0x10  ///< Slave Status Register R/WC
#define B_SMBUS_IO_HOST_NOTIFY_STS       0x01
#define R_SMBUS_IO_SCMD                  0x11  ///< Slave Command Register R/W
#define B_SMBUS_IO_HOST_NOTIFY_INTREN    0x01
#define B_SMBUS_IO_HOST_NOTIFY_WKEN      0x02
#define B_SMBUS_IO_SMBALERT_DIS          0x04
#define R_SMBUS_IO_NDA                   0x14  ///< Notify Device Address Register RO
#define B_SMBUS_IO_DEVICE_ADDRESS        0xFE
#define R_SMBUS_IO_NDLB                  0x16  ///< Notify Data Low Byte Register RO
#define R_SMBUS_IO_NDHB                  0x17  ///< Notify Data High Byte Register RO

//
// SMBus Private Config Registers
// (PID:SMB)
//
#define R_SMBUS_PCR_TCOCFG                0x00                        ///< TCO Configuration register
#define B_SMBUS_PCR_TCOCFG_IE             BIT7                        ///< TCO IRQ Enable
#define B_SMBUS_PCR_TCOCFG_IS             (BIT2 | BIT1 | BIT0)        ///< TCO IRQ Select
#define V_SMBUS_PCR_TCOCFG_IRQ_9          0x00
#define V_SMBUS_PCR_TCOCFG_IRQ_10         0x01
#define V_SMBUS_PCR_TCOCFG_IRQ_11         0x02
#define V_SMBUS_PCR_TCOCFG_IRQ_20         0x04                        ///< only if APIC enabled
#define V_SMBUS_PCR_TCOCFG_IRQ_21         0x05                        ///< only if APIC enabled
#define V_SMBUS_PCR_TCOCFG_IRQ_22         0x06                        ///< only if APIC enabled
#define V_SMBUS_PCR_TCOCFG_IRQ_23         0x07                        ///< only if APIC enabled
#define R_SMBUS_PCR_SMBTM                 0x04                        ///< SMBus Test Mode
#define B_SMBUS_PCR_SMBTM_SMBCT           BIT1                        ///< SMBus Counter
#define B_SMBUS_PCR_SMBTM_SMBDG           BIT0                        ///< SMBus Deglitch
#define R_SMBUS_PCR_SCTM                  0x08                        ///< Short Counter Test Mode
#define B_SMBUS_PCR_SCTM_SSU              BIT31                       ///< Simulation Speed-Up
#define R_SMBUS_PCR_GC                    0x0C                        ///< General Control
#define B_SMBUS_PCR_GC_FD                 BIT0                        ///< Function Disable
#endif
