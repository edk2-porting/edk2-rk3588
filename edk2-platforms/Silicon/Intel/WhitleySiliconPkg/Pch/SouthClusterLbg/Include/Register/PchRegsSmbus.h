/** @file
  Register names for PCH Smbus Device.
  Conventions:
  Prefixes:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values within the bits
  Definitions beginning with "S_" are register sizes
  Definitions beginning with "N_" are the bit position
  In general, PCH registers are denoted by "_PCH_" in register names
  Registers / bits that are different between PCH generations are denoted by
  _PCH_[generation_name]_" in register/bit names.
  Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
  Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
  e.g., "_PCH_H_", "_PCH_LP_"
  Registers / bits names without _H_ or _LP_ apply for both H and LP.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_SMBUS_H_
#define _PCH_REGS_SMBUS_H_

//
// SMBus Controller Registers (D31:F4)
//
#define PCI_DEVICE_NUMBER_PCH_SMBUS   31
#define PCI_FUNCTION_NUMBER_PCH_SMBUS 4
#define V_PCH_SMBUS_VENDOR_ID         V_PCH_INTEL_VENDOR_ID
#define V_PCH_H_SMBUS_DEVICE_ID               0xA123
//
// LBG Production SMBus Controller Device ID
//
#define V_PCH_LBG_PROD_SMBUS_DEVICE_ID     0xA1A3
//
// LBG SSX (Super SKU) SMBus Controller Device ID
//
#define V_PCH_LBG_SMBUS_DEVICE_ID  0xA223
#define V_PCH_LP_SMBUS_DEVICE_ID              0x9D23
#define R_PCH_SMBUS_BASE              0x20
#define V_PCH_SMBUS_BASE_SIZE         (1 << 5)
#define B_PCH_SMBUS_BASE_BAR          0x0000FFE0
#define R_PCH_SMBUS_HOSTC             0x40
#define B_PCH_SMBUS_HOSTC_SPDWD       BIT4
#define B_PCH_SMBUS_HOSTC_SSRESET     BIT3
#define B_PCH_SMBUS_HOSTC_I2C_EN      BIT2
#define B_PCH_SMBUS_HOSTC_SMI_EN      BIT1
#define B_PCH_SMBUS_HOSTC_HST_EN      BIT0
#define R_PCH_SMBUS_TCOBASE           0x50
#define B_PCH_SMBUS_TCOBASE_BAR       0x0000FFE0
#define R_PCH_SMBUS_TCOCTL                   0x54
#define B_PCH_SMBUS_TCOCTL_TCO_BASE_EN       BIT8
#define B_PCH_SMBUS_TCOCTL_TCO_BASE_LOCK     BIT0
#define R_PCH_SMBUS_64                        0x64
#define R_PCH_SMBUS_80                0x80

//
// SMBus I/O Registers
//
#define R_PCH_SMBUS_HSTS                  0x00  ///< Host Status Register R/W
#define B_PCH_SMBUS_HBSY                  0x01
#define B_PCH_SMBUS_INTR                  0x02
#define B_PCH_SMBUS_DERR                  0x04
#define B_PCH_SMBUS_BERR                  0x08
#define B_PCH_SMBUS_FAIL                  0x10
#define B_PCH_SMBUS_SMBALERT_STS          0x20
#define B_PCH_SMBUS_IUS                   0x40
#define B_PCH_SMBUS_BYTE_DONE_STS         0x80
#define B_PCH_SMBUS_ERROR                 (B_PCH_SMBUS_DERR | B_PCH_SMBUS_BERR | B_PCH_SMBUS_FAIL)
#define B_PCH_SMBUS_HSTS_ALL              0xFF
#define R_PCH_SMBUS_HCTL                  0x02  ///< Host Control Register R/W
#define B_PCH_SMBUS_INTREN                0x01
#define B_PCH_SMBUS_KILL                  0x02
#define B_PCH_SMBUS_SMB_CMD               0x1C
#define V_PCH_SMBUS_SMB_CMD_QUICK         0x00
#define V_PCH_SMBUS_SMB_CMD_BYTE          0x04
#define V_PCH_SMBUS_SMB_CMD_BYTE_DATA     0x08
#define V_PCH_SMBUS_SMB_CMD_WORD_DATA     0x0C
#define V_PCH_SMBUS_SMB_CMD_PROCESS_CALL  0x10
#define V_PCH_SMBUS_SMB_CMD_BLOCK         0x14
#define V_PCH_SMBUS_SMB_CMD_IIC_READ      0x18
#define V_PCH_SMBUS_SMB_CMD_BLOCK_PROCESS 0x1C
#define B_PCH_SMBUS_LAST_BYTE             0x20
#define B_PCH_SMBUS_START                 0x40
#define B_PCH_SMBUS_PEC_EN                0x80
#define R_PCH_SMBUS_HCMD                  0x03  ///< Host Command Register R/W
#define R_PCH_SMBUS_TSA                   0x04  ///< Transmit Slave Address Register R/W
#define B_PCH_SMBUS_RW_SEL                0x01
#define B_PCH_SMBUS_READ                  0x01  // RW
#define B_PCH_SMBUS_WRITE                 0x00  // RW
#define B_PCH_SMBUS_ADDRESS               0xFE
#define R_PCH_SMBUS_HD0                   0x05  ///< Data 0 Register R/W
#define R_PCH_SMBUS_HD1                   0x06  ///< Data 1 Register R/W
#define R_PCH_SMBUS_HBD                   0x07  ///< Host Block Data Register R/W
#define R_PCH_SMBUS_PEC                   0x08  ///< Packet Error Check Data Register R/W
#define R_PCH_SMBUS_RSA                   0x09  ///< Receive Slave Address Register R/W
#define B_PCH_SMBUS_SLAVE_ADDR            0x7F
#define R_PCH_SMBUS_SD                    0x0A  ///< Receive Slave Data Register R/W
#define R_PCH_SMBUS_AUXS                  0x0C  ///< Auxiliary Status Register R/WC
#define B_PCH_SMBUS_CRCE                  0x01
#define B_PCH_SMBUS_STCO                  0x02  ///< SMBus TCO Mode
#define R_PCH_SMBUS_AUXC                  0x0D  ///< Auxiliary Control Register R/W
#define B_PCH_SMBUS_AAC                   0x01
#define B_PCH_SMBUS_E32B                  0x02
#define R_PCH_SMBUS_SMLC                  0x0E  ///< SMLINK Pin Control Register R/W
#define B_PCH_SMBUS_SMLINK0_CUR_STS       0x01
#define B_PCH_SMBUS_SMLINK1_CUR_STS       0x02
#define B_PCH_SMBUS_SMLINK_CLK_CTL        0x04
#define R_PCH_SMBUS_SMBC                  0x0F  ///< SMBus Pin Control Register R/W
#define B_PCH_SMBUS_SMBCLK_CUR_STS        0x01
#define B_PCH_SMBUS_SMBDATA_CUR_STS       0x02
#define B_PCH_SMBUS_SMBCLK_CTL            0x04
#define R_PCH_SMBUS_SSTS                  0x10  ///< Slave Status Register R/WC
#define B_PCH_SMBUS_HOST_NOTIFY_STS       0x01
#define R_PCH_SMBUS_SCMD                  0x11  ///< Slave Command Register R/W
#define B_PCH_SMBUS_HOST_NOTIFY_INTREN    0x01
#define B_PCH_SMBUS_HOST_NOTIFY_WKEN      0x02
#define B_PCH_SMBUS_SMBALERT_DIS          0x04
#define R_PCH_SMBUS_NDA                   0x14  ///< Notify Device Address Register RO
#define B_PCH_SMBUS_DEVICE_ADDRESS        0xFE
#define R_PCH_SMBUS_NDLB                  0x16  ///< Notify Data Low Byte Register RO
#define R_PCH_SMBUS_NDHB                  0x17  ///< Notify Data High Byte Register RO

//
// SMBus Private Config Registers
// (PID:SMB)
//
#define R_PCH_PCR_SMBUS_TCOCFG                0x00                        ///< TCO Configuration register
#define B_PCH_PCR_SMBUS_TCOCFG_IE             BIT7                        ///< TCO IRQ Enable
#define B_PCH_PCR_SMBUS_TCOCFG_IS             (BIT2 | BIT1 | BIT0)        ///< TCO IRQ Select
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_9          0x00
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_10         0x01
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_11         0x02
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_20         0x04                        ///< only if APIC enabled
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_21         0x05                        ///< only if APIC enabled
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_22         0x06                        ///< only if APIC enabled
#define V_PCH_PCR_SMBUS_TCOCFG_IRQ_23         0x07                        ///< only if APIC enabled
#define R_PCH_PCR_SMBUS_SMBTM                 0x04                        ///< SMBus Test Mode
#define B_PCH_PCR_SMBUS_SMBTM_SMBCT           BIT1                        ///< SMBus Counter
#define B_PCH_PCR_SMBUS_SMBTM_SMBDG           BIT0                        ///< SMBus Deglitch
#define R_PCH_PCR_SMBUS_SCTM                  0x08                        ///< Short Counter Test Mode
#define B_PCH_PCR_SMBUS_SCTM_SSU              BIT31                       ///< Simulation Speed-Up
#define R_PCH_PCR_SMBUS_GC                    0x0C                        ///< General Control
#define B_PCH_PCR_SMBUS_GC_FD                 BIT0                        ///< Function Disable
#define B_PCH_PCR_SMBUS_GC_NR                 BIT1                        ///< No Reboot
#define B_PCH_PCR_SMBUS_GC_SMBSCGE            BIT2                        ///< SMB Static Clock Gating Enable
#define R_PCH_PCR_SMBUS_PCE                   0x10                        ///< Power Control Enable
#define B_PCH_PCR_SMBUS_PCE_HAE               BIT5                        ///< Hardware Autonomous Enable

#endif
