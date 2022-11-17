/** @file
  Register names for PCH Storage and Communication Subsystem

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
    e.g., "_PCH_H_", "_PCH_LP_"
    Registers / bits names without _H_ or _LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without [generation_name] inserted.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_REGS_SCS_H_
#define _PCH_REGS_SCS_H_

//
//  SCS eMMC Controller Registers (D30:F4)
//
#define PCI_DEVICE_NUMBER_PCH_SCS_EMMC        30
#define PCI_FUNCTION_NUMBER_PCH_SCS_EMMC      4
#define V_PCH_SCS_EMMC_VENDOR_ID              V_PCH_INTEL_VENDOR_ID
#define V_PCH_SCS_EMMC_DEVICE_ID              0x9D2B

//
//  SCS SDIO Controller Registers (D30:F5)
//
#define PCI_DEVICE_NUMBER_PCH_SCS_SDIO        30
#define PCI_FUNCTION_NUMBER_PCH_SCS_SDIO      5
#define V_PCH_SCS_SDIO_VENDOR_ID              V_PCH_INTEL_VENDOR_ID
#define V_PCH_SCS_SDIO_DEVICE_ID              0x9D2C

//
//  SCS SDCARD Controller Registers (D30:F6)
//
#define PCI_DEVICE_NUMBER_PCH_SCS_SDCARD      30
#define PCI_FUNCTION_NUMBER_PCH_SCS_SDCARD    6
#define V_PCH_SCS_SDCARD_VENDOR_ID            V_PCH_INTEL_VENDOR_ID
#define V_PCH_SCS_SDCARD_DEVICE_ID            0x9D2D

//
// SCS Devices PCI Config Space Registers
//
#define R_PCH_SCS_DEV_PCS                     0x84                          ///< PME Control Status
#define B_PCH_SCS_DEV_PCS_PMESTS              BIT15                         ///< PME Status
#define B_PCH_SCS_DEV_PCS_PMEEN               BIT8                          ///< PME Enable
#define B_PCH_SCS_DEV_PCS_NSS                 BIT3                          ///< No Soft Reset
#define B_PCH_SCS_DEV_PCS_PS                  (BIT1 | BIT0)                 ///< Power State
#define B_PCH_SCS_DEV_PCS_PS_D3HOT            (BIT1 | BIT0)                 ///< Power State: D3Hot State
#define R_PCH_SCS_DEV_PG_CONFIG               0xA2                          ///< PG Config
#define B_PCH_SCS_DEV_PG_CONFIG_SE            BIT3                          ///< Sleep Enable
#define B_PCH_SCS_DEV_PG_CONFIG_PGE           BIT2                          ///< PG Enable
#define B_PCH_SCS_DEV_PG_CONFIG_I3E           BIT1                          ///< I3 Enable
#define B_PCH_SCS_DEV_PG_CONFIG_PMCRE         BIT0                          ///< PMC Request Enable
#define V_PCH_SCS_DEV_BAR0_SIZE               0x1000                        ///< BAR0 size
//
// SCS Devices MMIO Space Register
//
#define R_PCH_SCS_DEV_MEM_DMAADR                                    0x00
#define R_PCH_SCS_DEV_MEM_BLKSZ                                     0x04
#define R_PCH_SCS_DEV_MEM_BLKCNT                                    0x06
#define R_PCH_SCS_DEV_MEM_CMDARG                                    0x08
#define R_PCH_SCS_DEV_MEM_XFRMODE                                   0x0C
#define B_PCH_SCS_DEV_MEM_XFRMODE_DMA_EN                            BIT0
#define B_PCH_SCS_DEV_MEM_XFRMODE_BLKCNT_EN                         BIT1
#define B_PCH_SCS_DEV_MEM_XFRMODE_AUTOCMD_EN_MASK                   (BIT2 | BIT3)
#define V_PCH_SCS_DEV_MEM_XFRMODE_AUTOCMD12_EN                      1
#define B_PCH_SCS_DEV_MEM_XFRMODE_DATA_TRANS_DIR                    BIT4               ///< 1: Read (Card to Host), 0: Write (Host to Card)
#define B_PCH_SCS_DEV_MEM_XFRMODE_MULTI_SINGLE_BLK                  BIT5               ///< 1: Multiple Block, 0: Single Block
#define R_PCH_SCS_DEV_MEM_SDCMD                                     0x0E
#define B_PCH_SCS_DEV_MEM_SDCMD_RESP_TYPE_SEL_MASK                  (BIT0 | BIT1)
#define V_PCH_SCS_DEV_MEM_SDCMD_RESP_TYPE_SEL_NO_RESP               0
#define V_PCH_SCS_DEV_MEM_SDCMD_RESP_TYPE_SEL_RESP136               1
#define V_PCH_SCS_DEV_MEM_SDCMD_RESP_TYPE_SEL_RESP48                2
#define V_PCH_SCS_DEV_MEM_SDCMD_RESP_TYPE_SEL_RESP48_CHK            3
#define B_PCH_SCS_DEV_MEM_SDCMD_CMD_CRC_CHECK_EN                    BIT3
#define B_PCH_SCS_DEV_MEM_SDCMD_CMD_INDEX_CHECK_EN                  BIT4
#define B_PCH_SCS_DEV_MEM_SDCMD_DATA_PRESENT_SEL                    BIT5
#define R_PCH_SCS_DEV_MEM_RESP                                      0x10
#define R_PCH_SCS_DEV_MEM_BUFDATAPORT                               0x20
#define R_PCH_SCS_DEV_MEM_PSTATE                                    0x24
#define B_PCH_SCS_DEV_MEM_PSTATE_DAT0                               BIT20
#define R_PCH_SCS_DEV_MEM_PWRCTL                                    0x29
#define R_PCH_SCS_DEV_MEM_CLKCTL                                    0x2C
#define R_PCH_SCS_DEV_MEM_TIMEOUT_CTL                               0x2E               ///< Timeout Control
#define B_PCH_SCS_DEV_MEM_TIMEOUT_CTL_DTCV                          0x0F               ///< Data Timeout Counter Value
#define R_PCH_SCS_DEV_MEM_SWRST                                     0x2F
#define B_PCH_SCS_DEV_MEM_SWRST_CMDLINE                             BIT1
#define B_PCH_SCS_DEV_MEM_SWRST_DATALINE                            BIT2
#define R_PCH_SCS_DEV_MEM_NINTSTS                                   0x30
#define B_PCH_SCS_DEV_MEM_NINTSTS_MASK                              0xFFFF
#define B_PCH_SCS_DEV_MEM_NINTSTS_CLEAR_MASK                        0x60FF
#define B_PCH_SCS_DEV_MEM_NINTSTS_CMD_COMPLETE                      BIT0
#define B_PCH_SCS_DEV_MEM_NINTSTS_TRANSFER_COMPLETE                 BIT1
#define B_PCH_SCS_DEV_MEM_NINTSTS_DMA_INTERRUPT                     BIT3
#define B_PCH_SCS_DEV_MEM_NINTSTS_BUF_READ_READY_INTR               BIT5
#define R_PCH_SCS_DEV_MEM_ERINTSTS                                  0x32
#define B_PCH_SCS_DEV_MEM_ERINTSTS_MASK                             0x13FF
#define B_PCH_SCS_DEV_MEM_ERINTSTS_CLEAR_MASK                       0x13FF
#define R_PCH_SCS_DEV_MEM_NINTEN                                    0x34
#define B_PCH_SCS_DEV_MEM_NINTEN_MASK                               0x7FFF
#define R_PCH_SCS_DEV_MEM_ERINTEN                                   0x36
#define B_PCH_SCS_DEV_MEM_ERINTEN_MASK                              0x13FF
#define R_PCH_SCS_DEV_MEM_NINTSIGNEN                                0x38
#define B_PCH_SCS_DEV_MEM_NINTSIGNEN_MASK                           0x7FFF
#define R_PCH_SCS_DEV_MEM_ERINTSIGNEN                               0x3A
#define B_PCH_SCS_DEV_MEM_ERINTSIGNEN_MASK                          0x13FF
#define R_PCH_SCS_DEV_MEM_HOST_CTL2                                 0x3E
#define B_PCH_SCS_DEV_MEM_HOST_CTL2_MODE_MASK                       (BIT0 | BIT1 | BIT2)
#define V_PCH_SCS_DEV_MEM_HOST_CTL2_MODE_HS400                      5
#define V_PCH_SCS_DEV_MEM_HOST_CTL2_MODE_DDR50                      4
#define V_PCH_SCS_DEV_MEM_HOST_CTL2_MODE_SDR104                     3
#define V_PCH_SCS_DEV_MEM_HOST_CTL2_MODE_SDR25                      1
#define R_PCH_SCS_DEV_MEM_CAP1                                      0x40
#define R_PCH_SCS_DEV_MEM_CAP2                                      0x44
#define B_PCH_SCS_DEV_MEM_CAP2_HS400_SUPPORT                        BIT31
#define B_PCH_SCS_DEV_MEM_CAP2_SDR104_SUPPORT                       BIT1
#define R_PCH_SCS_DEV_MEM_CESHC2                                    0x3C              ///< Auto CMD12 Error Status Register & Host Control 2
#define B_PCH_SCS_DEV_MEM_CESHC2_ASYNC_INT                          BIT30             ///< Asynchronous Interrupt Enable
#define R_PCH_SCS_DEV_MEM_CAP_BYPASS_CONTROL                        0x810
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_CONTROL_EN                     0x5A
#define R_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1                           0x814
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_EMMC_DEFAULTS             0x3C80EB1E
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_SDIO_DEFAULTS             0x1C80EF1E
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_SDCARD_DEFAULTS           0x1C80E75C
#define B_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_HS400                     BIT29
#define B_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_TIMEOUT_CLK_FREQ          (BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22)
#define N_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_TIMEOUT_CLK_FREQ          22
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_TIMEOUT_CLK_FREQ          0x1
#define B_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_TIMER_COUNT               (BIT20 | BIT19 | BIT18 | BIT17)
#define N_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_TIMER_COUNT               17
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_TIMER_COUNT               0x8
#define B_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_SLOT_TYPE                 (BIT12 | BIT11)
#define N_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_SLOT_TYPE                 11
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG1_SLOT_TYPE_EMBEDDED        0x1
#define R_PCH_SCS_DEV_MEM_CAP_BYPASS_REG2                           0x818
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG2_EMMC_DEFAULTS             0x040040C8
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG2_SDIO_DEFAULTS             0x040000C8
#define V_PCH_SCS_DEV_MEM_CAP_BYPASS_REG2_SDCARD_DEFAULTS           0x040000C8
#define R_PCH_SCS_DEV_MEM_TX_CMD_DLL_CNTL1                          0x820
#define R_PCH_SCS_DEV_MEM_TX_CMD_DLL_CNTL2                          0x80C
#define R_PCH_SCS_DEV_MEM_TX_DATA_DLL_CNTL1                         0x824
#define R_PCH_SCS_DEV_MEM_TX_DATA_DLL_CNTL2                         0x828
#define R_PCH_SCS_DEV_MEM_RX_CMD_DATA_DLL_CNTL1                     0x82C
#define R_PCH_SCS_DEV_MEM_RX_STROBE_DLL_CNTL                        0x830
#define R_PCH_SCS_DEV_MEM_RX_CMD_DATA_DLL_CNTL2                     0x834
#define N_PCH_SCS_DEV_MEM_RX_CMD_DATA_DLL_CNTL2_CLKSRC_RX            16
#define V_PCH_SCS_DEV_MEM_RX_CMD_DATA_DLL_CNTL2_CLKSRC_RX_CLK_AUTO   0x2
#define V_PCH_SCS_DEV_MEM_RX_CMD_DATA_DLL_CNTL2_CLKSRC_RX_CLK_BEFORE 0x1
#define V_PCH_SCS_DEV_MEM_RX_CMD_DATA_DLL_CNTL2_CLKSRC_RX_CLK_AFTER  0x0

//
// SCS Private Configuration Space Registers
//
#define R_PCH_PCR_SCS_IOSFCTL                     0x00                          ///< IOSF Control
#define B_PCH_PCR_SCS_IOSFCTL_NSNPDIS             BIT7                          ///< Non-Snoop Disable
#define B_PCH_PCR_SCS_IOSFCTL_MAX_RD_PEND         (BIT3 | BIT2 | BIT1 | BIT0)   ///< Max upstream pending reads
#define R_PCH_PCR_SCS_OCPCTL                      0x10                          ///< OCP Control
#define B_PCH_PCR_SCS_OCPCTL_NPEN                 BIT0                          ///< Downstream non-posted memory write capability
#define R_PCH_PCR_SCS_PMCTL                       0x1D0                         ///< Power Management Control
#define R_PCH_PCR_SCS_PCICFGCTR_EMMC              0x200                         ///< PCI Configuration Control 1 - eMMC
#define R_PCH_PCR_SCS_PCICFGCTR_SDIO              0x204                         ///< PCI Configuration Control 2 - SDIO
#define R_PCH_PCR_SCS_PCICFGCTR_SDCARD            0x208                         ///< PCI Configuration Control 3 - SD Card
#define B_PCH_PCR_SCS_PCICFGCTR_PCI_IRQ           0x0FF00000                    ///< PCI IRQ number
#define N_PCH_PCR_SCS_PCICFGCTR_PCI_IRQ           20
#define B_PCH_PCR_SCS_PCICFGCTR_ACPI_IRQ          0x000FF000                    ///< ACPI IRQ number
#define N_PCH_PCR_SCS_PCICFGCTR_ACPI_IRQ          12
#define B_PCH_PCR_SCS_PCICFGCTR_IPIN1             (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_PCH_PCR_SCS_PCICFGCTR_IPIN1             8
#define B_PCH_PCR_SCS_PCICFGCTR_BAR1DIS           BIT7                          ///< BAR 1 Disable
#define B_PCH_PCR_SCS_PCICFGCTR_PS                0x7C                          ///< PME Support
#define B_PCH_PCR_SCS_PCICFGCTR_ACPI_INT_EN       BIT1                          ///< ACPI Interrupt Enable
#define B_PCH_PCR_SCS_PCICFGCTR_PCI_CFG_DIS       BIT0                          ///< PCI Configuration Space Disable

#define R_PCH_PCR_SCS_GPPRVRW1                    0x600                         ///< Clock Gating Control
#define R_PCH_PCR_SCS_GPPRVRW2                    0x604                         ///< Host Controller Disable
#define B_PCH_PCR_SCS_GPPRVRW2_EMMC_DIS           BIT1                          ///< eMMC Host Controller Disable
#define B_PCH_PCR_SCS_GPPRVRW2_SDIO_SDCARD_DIS    BIT2                          ///< 1: SDIO Host Controller Disable, 0: SDCARD Host Controller Disable
#define R_PCH_PCR_SCS_GPPRVRW6                    0x614                         ///< 1.8V Signal Select Delay Control
#define V_PCH_PCR_SCS_GPPRVRW6_1P8_SEL_DELAY      0x7F                          ///< Rcomp SDCARD 10ms delay during switch

#endif
