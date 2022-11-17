/** @file
  Register names for PCH LPC/eSPI device

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

#ifndef _PCH_REGS_LPC_H_
#define _PCH_REGS_LPC_H_

#define B_LPC_CFG_DID         0xFFE0

//
// PCI to LPC Bridge Registers (D31:F0)
//
#define PCI_DEVICE_NUMBER_PCH_LPC       31
#define PCI_FUNCTION_NUMBER_PCH_LPC     0

#define V_LPC_CFG_VENDOR_ID                       V_PCH_INTEL_VENDOR_ID


#define R_LPC_CFG_SERIRQ_CNT                      0x64
#define B_LPC_CFG_SERIRQ_CNT_SIRQEN               BIT7
#define B_LPC_CFG_SERIRQ_CNT_SIRQMD               BIT6
#define B_LPC_CFG_SERIRQ_CNT_SIRQSZ               (BIT5 | BIT4 | BIT3 | BIT2)
#define N_LPC_CFG_SERIRQ_CNT_SIRQSZ               2
#define B_LPC_CFG_SERIRQ_CNT_SFPW                 (BIT1 | BIT0)
#define N_LPC_CFG_SERIRQ_CNT_SFPW                 0
#define V_LPC_CFG_SERIRQ_CNT_SFPW_4CLK            0x00
#define V_LPC_CFG_SERIRQ_CNT_SFPW_6CLK            0x01
#define V_LPC_CFG_SERIRQ_CNT_SFPW_8CLK            0x02

#define R_LPC_CFG_IOD                             0x80
#define B_LPC_CFG_IOD_FDD                         BIT12
#define N_LPC_CFG_IOD_FDD                         12
#define V_LPC_CFG_IOD_FDD_3F0                     0
#define V_LPC_CFG_IOD_FDD_370                     1
#define B_LPC_CFG_IOD_LPT                         (BIT9 | BIT8)
#define N_LPC_CFG_IOD_LPT                         8
#define V_LPC_CFG_IOD_LPT_378                     0
#define V_LPC_CFG_IOD_LPT_278                     1
#define V_LPC_CFG_IOD_LPT_3BC                     2
#define B_LPC_CFG_IOD_COMB                        (BIT6 | BIT5 |BIT4)
#define N_LPC_CFG_IOD_COMB                        4
#define V_LPC_CFG_IOD_COMB_3F8                    0
#define V_LPC_CFG_IOD_COMB_2F8                    1
#define V_LPC_CFG_IOD_COMB_220                    2
#define V_LPC_CFG_IOD_COMB_228                    3
#define V_LPC_CFG_IOD_COMB_238                    4
#define V_LPC_CFG_IOD_COMB_2E8                    5
#define V_LPC_CFG_IOD_COMB_338                    6
#define V_LPC_CFG_IOD_COMB_3E8                    7
#define B_LPC_CFG_IOD_COMA                        (BIT2 | BIT1 | BIT0)
#define N_LPC_CFG_IOD_COMA                        0
#define V_LPC_CFG_IOD_COMA_3F8                    0
#define V_LPC_CFG_IOD_COMA_2F8                    1
#define V_LPC_CFG_IOD_COMA_220                    2
#define V_LPC_CFG_IOD_COMA_228                    3
#define V_LPC_CFG_IOD_COMA_238                    4
#define V_LPC_CFG_IOD_COMA_2E8                    5
#define V_LPC_CFG_IOD_COMA_338                    6
#define V_LPC_CFG_IOD_COMA_3E8                    7
#define R_LPC_CFG_IOE                             0x82
#define B_LPC_CFG_IOE_ME2                         BIT13           ///< Microcontroller Enable #2, Enables decoding of I/O locations 4Eh and 4Fh to LPC
#define B_LPC_CFG_IOE_SE                          BIT12           ///< Super I/O Enable, Enables decoding of I/O locations 2Eh and 2Fh to LPC.
#define B_LPC_CFG_IOE_ME1                         BIT11           ///< Microcontroller Enable #1, Enables decoding of I/O locations 62h and 66h to LPC.
#define B_LPC_CFG_IOE_KE                          BIT10           ///< Keyboard Enable, Enables decoding of the keyboard I/O locations 60h and 64h to LPC.
#define B_LPC_CFG_IOE_HGE                         BIT9            ///< High Gameport Enable, Enables decoding of the I/O locations 208h to 20Fh to LPC.
#define B_LPC_CFG_IOE_LGE                         BIT8            ///< Low Gameport Enable, Enables decoding of the I/O locations 200h to 207h to LPC.
#define B_LPC_CFG_IOE_FDE                         BIT3            ///< Floppy Drive Enable, Enables decoding of the FDD range to LPC. Range is selected by LIOD.FDE
#define B_LPC_CFG_IOE_PPE                         BIT2            ///< Parallel Port Enable, Enables decoding of the LPT range to LPC. Range is selected by LIOD.LPT.
#define B_LPC_CFG_IOE_CBE                         BIT1            ///< Com Port B Enable, Enables decoding of the COMB range to LPC. Range is selected LIOD.CB.
#define B_LPC_CFG_IOE_CAE                         BIT0            ///< Com Port A Enable, Enables decoding of the COMA range to LPC. Range is selected LIOD.CA.
#define R_LPC_CFG_GEN1_DEC                        0x84
#define R_LPC_CFG_GEN2_DEC                        0x88
#define R_LPC_CFG_GEN3_DEC                        0x8C
#define R_LPC_CFG_GEN4_DEC                        0x90
#define B_LPC_CFG_GENX_DEC_IODRA                  0x00FC0000
#define B_LPC_CFG_GENX_DEC_IOBAR                  0x0000FFFC
#define B_LPC_CFG_GENX_DEC_EN                     0x00000001
#define R_LPC_CFG_ULKMC                           0x94
#define B_LPC_CFG_ULKMC_SMIBYENDPS                BIT15
#define B_LPC_CFG_ULKMC_TRAPBY64W                 BIT11
#define B_LPC_CFG_ULKMC_TRAPBY64R                 BIT10
#define B_LPC_CFG_ULKMC_TRAPBY60W                 BIT9
#define B_LPC_CFG_ULKMC_TRAPBY60R                 BIT8
#define B_LPC_CFG_ULKMC_SMIATENDPS                BIT7
#define B_LPC_CFG_ULKMC_PSTATE                    BIT6
#define B_LPC_CFG_ULKMC_A20PASSEN                 BIT5
#define B_LPC_CFG_ULKMC_USBSMIEN                  BIT4
#define B_LPC_CFG_ULKMC_64WEN                     BIT3
#define B_LPC_CFG_ULKMC_64REN                     BIT2
#define B_LPC_CFG_ULKMC_60WEN                     BIT1
#define B_LPC_CFG_ULKMC_60REN                     BIT0
#define R_LPC_CFG_LGMR                            0x98
#define B_LPC_CFG_LGMR_MA                         0xFFFF0000
#define B_LPC_CFG_LGMR_LMRD_EN                    BIT0
#define R_ESPI_CFG_CS1IORE                        0xA0
#define R_ESPI_CFG_CS1IORE_DPCS1RE                BIT14
#define R_ESPI_CFG_CS1GIR1                        0xA4
#define R_ESPI_CFG_CS1GMR1                        0xA8

#define R_LPC_CFG_FWH_BIOS_SEL                    0xD0
#define B_LPC_CFG_FWH_BIOS_SEL_F8                 0xF0000000
#define B_LPC_CFG_FWH_BIOS_SEL_F0                 0x0F000000
#define B_LPC_CFG_FWH_BIOS_SEL_E8                 0x00F00000
#define B_LPC_CFG_FWH_BIOS_SEL_E0                 0x000F0000
#define B_LPC_CFG_FWH_BIOS_SEL_D8                 0x0000F000
#define B_LPC_CFG_FWH_BIOS_SEL_D0                 0x00000F00
#define B_LPC_CFG_FWH_BIOS_SEL_C8                 0x000000F0
#define B_LPC_CFG_FWH_BIOS_SEL_C0                 0x0000000F
#define R_LPC_CFG_FWH_BIOS_SEL2                   0xD4
#define B_LPC_CFG_FWH_BIOS_SEL2_70                0xF000
#define B_LPC_CFG_FWH_BIOS_SEL2_60                0x0F00
#define B_LPC_CFG_FWH_BIOS_SEL2_50                0x00F0
#define B_LPC_CFG_FWH_BIOS_SEL2_40                0x000F
#define R_LPC_CFG_BDE                             0xD8                          ///< BIOS decode enable
#define B_LPC_CFG_BDE_F8                          BIT15
#define B_LPC_CFG_BDE_F0                          BIT14
#define B_LPC_CFG_BDE_E8                          BIT13
#define B_LPC_CFG_BDE_E0                          BIT12
#define B_LPC_CFG_BDE_D8                          BIT11
#define B_LPC_CFG_BDE_D0                          BIT10
#define B_LPC_CFG_BDE_C8                          BIT9
#define B_LPC_CFG_BDE_C0                          BIT8
#define B_LPC_CFG_BDE_LEG_F                       BIT7
#define B_LPC_CFG_BDE_LEG_E                       BIT6
#define B_LPC_CFG_BDE_70                          BIT3
#define B_LPC_CFG_BDE_60                          BIT2
#define B_LPC_CFG_BDE_50                          BIT1
#define B_LPC_CFG_BDE_40                          BIT0
#define R_LPC_CFG_PCC                             0xE0
#define B_LPC_CFG_PCC_CLKRUN_EN                   BIT0

#define B_LPC_CFG_FVEC0_USB_PORT_CAP              (BIT11 | BIT10)
#define V_LPC_CFG_FVEC0_USB_14_PORT               0x00000000
#define V_LPC_CFG_FVEC0_USB_12_PORT               0x00000400
#define V_LPC_CFG_FVEC0_USB_10_PORT               0x00000800
#define B_LPC_CFG_FVEC0_SATA_RAID_CAP             BIT7
#define B_LPC_CFG_FVEC0_SATA_PORT23_CAP           BIT6
#define B_LPC_CFG_FVEC0_SATA_PORT1_6GB_CAP        BIT3
#define B_LPC_CFG_FVEC0_SATA_PORT0_6GB_CAP        BIT2
#define B_LPC_CFG_FVEC0_PCI_CAP                   BIT1
#define R_LPC_CFG_FVEC1                           0x01
#define B_LPC_CFG_FVEC1_USB_R_CAP                 BIT22
#define R_LPC_CFG_FVEC2                           0x02
#define V_LPC_CFG_FVEC2_PCIE_PORT78_CAP           0x00200000
#define V_LPC_CFG_FVEC2_PCH_IG_SUPPORT_CAP        0x00020000 ///< PCH Integrated Graphics Support Capability
#define R_LPC_CFG_FVEC3                           0x03
#define B_LPC_CFG_FVEC3_DCMI_CAP                  BIT13      ///< Data Center Manageability Interface (DCMI) Capability
#define B_LPC_CFG_FVEC3_NM_CAP                    BIT12      ///< Node Manager Capability

#define R_LPC_CFG_MDAP                            0xC0
#define B_LPC_CFG_MDAP_POLICY_EN                  BIT31
#define B_LPC_CFG_MDAP_PDMA_EN                    BIT30
#define B_LPC_CFG_MDAP_VALUE                      0x0001FFFF

//
// APM Registers
//
#define R_PCH_IO_APM_CNT                             0xB2
#define R_PCH_IO_APM_STS                             0xB3

#define R_LPC_CFG_BC                              0xDC            ///< Bios Control
#define S_LPC_CFG_BC                              1
#define B_LPC_CFG_BC_BILD                         BIT7            ///< BIOS Interface Lock-Down
#define B_LPC_CFG_BC_BBS                          BIT6            ///< Boot BIOS strap
#define N_LPC_CFG_BC_BBS                          6
#define V_LPC_CFG_BC_BBS_SPI                      0               ///< Boot BIOS strapped to SPI
#define V_LPC_CFG_BC_BBS_LPC                      1               ///< Boot BIOS strapped to LPC
#define B_LPC_CFG_BC_EISS                         BIT5            ///< Enable InSMM.STS
#define B_LPC_CFG_BC_TS                           BIT4            ///< Top Swap
#define B_LPC_CFG_BC_LE                           BIT1            ///< Lock Enable
#define N_LPC_CFG_BC_LE                           1
#define B_LPC_CFG_BC_WPD                          BIT0            ///< Write Protect Disable

#define R_ESPI_CFG_PCBC                           0xDC            ///< Peripheral Channel BIOS Control
#define S_ESPI_CFG_PCBC                           4               ///< Peripheral Channel BIOS Control register size
#define B_ESPI_CFG_PCBC_BWRE                      BIT11           ///< BIOS Write Report Enable
#define N_ESPI_CFG_PCBC_BWRE                      11              ///< BIOS Write Report Enable bit position
#define B_ESPI_CFG_PCBC_BWRS                      BIT10           ///< BIOS Write Report Status
#define N_ESPI_CFG_PCBC_BWRS                      10              ///< BIOS Write Report Status bit position
#define B_ESPI_CFG_PCBC_BWPDS                     BIT8            ///< BIOS Write Protect Disable Status
#define N_ESPI_CFG_PCBC_BWPDS                     8               ///< BIOS Write Protect Disable Status bit position
#define B_ESPI_CFG_PCBC_ESPI_EN                   BIT2            ///< eSPI Enable Pin Strap
#define B_ESPI_CFG_PCBC_LE                        BIT1            ///< Lock Enable
#define N_ESPI_CFG_PCBC_LE                        1

//
// eSPI slave registers
//
#define R_ESPI_SLAVE_CHA_0_CAP_AND_CONF           0x10            ///< Channel 0 Capabilities and Configurations
#define B_ESPI_SLAVE_BME                          BIT2            ///< Bus Master Enable

//
// Processor interface registers
//
#define R_PCH_IO_NMI_SC                              0x61
#define B_PCH_IO_NMI_SC_SERR_NMI_STS                 BIT7
#define B_PCH_IO_NMI_SC_IOCHK_NMI_STS                BIT6
#define B_PCH_IO_NMI_SC_TMR2_OUT_STS                 BIT5
#define B_PCH_IO_NMI_SC_REF_TOGGLE                   BIT4
#define B_PCH_IO_NMI_SC_IOCHK_NMI_EN                 BIT3
#define B_PCH_IO_NMI_SC_PCI_SERR_EN                  BIT2
#define B_PCH_IO_NMI_SC_SPKR_DAT_EN                  BIT1
#define B_PCH_IO_NMI_SC_TIM_CNT2_EN                  BIT0
#define R_PCH_IO_NMI_EN                              0x70
#define B_PCH_IO_NMI_EN_NMI_EN                       BIT7

//
// Reset Generator I/O Port
//
#define R_PCH_IO_RST_CNT                             0xCF9
#define B_PCH_IO_RST_CNT_FULL_RST                    BIT3
#define B_PCH_IO_RST_CNT_RST_CPU                     BIT2
#define B_PCH_IO_RST_CNT_SYS_RST                     BIT1
#define V_PCH_IO_RST_CNT_FULLRESET                   0x0E
#define V_PCH_IO_RST_CNT_HARDRESET                   0x06
#define V_PCH_IO_RST_CNT_SOFTRESET                   0x04
#define V_PCH_IO_RST_CNT_HARDSTARTSTATE              0x02
#define V_PCH_IO_RST_CNT_SOFTSTARTSTATE              0x00

//
// RTC register
//
#define R_RTC_IO_INDEX                           0x70
#define R_RTC_IO_TARGET                          0x71
#define R_RTC_IO_EXT_INDEX                       0x72
#define R_RTC_IO_EXT_TARGET                      0x73
#define R_RTC_IO_INDEX_ALT                       0x74
#define R_RTC_IO_TARGET_ALT                      0x75
#define R_RTC_IO_EXT_INDEX_ALT                   0x76
#define R_RTC_IO_EXT_TARGET_ALT                  0x77
#define R_RTC_IO_REGA                            0x0A
#define B_RTC_IO_REGA_UIP                        BIT7
#define R_RTC_IO_REGB                            0x0B
#define B_RTC_IO_REGB_SET                        0x80
#define B_RTC_IO_REGB_PIE                        0x40
#define B_RTC_IO_REGB_AIE                        0x20
#define B_RTC_IO_REGB_UIE                        0x10
#define B_RTC_IO_REGB_DM                         0x04
#define B_RTC_IO_REGB_HOURFORM                   0x02
#define R_RTC_IO_REGC                            0x0C
#define R_RTC_IO_REGD                            0x0D

//
// Private Configuration Register
// RTC PCRs (PID:RTC)
//
#define R_RTC_PCR_CONF                        0x3400               ///< RTC Configuration register
#define B_RTC_PCR_CONF_BILD                   BIT31                ///< BIOS Interface Lock-Down
#define B_RTC_PCR_CONF_HPM_HW_DIS             BIT6                 ///< RTC High Power Mode HW Disable
#define B_RTC_PCR_CONF_UCMOS_LOCK             BIT4                 ///< Upper 128 Byte Lock
#define B_RTC_PCR_CONF_LCMOS_LOCK             BIT3                 ///< Lower 128 Byte Lock
#define B_RTC_PCR_CONF_UCMOS_EN               BIT2                 ///< Upper CMOS bank enable
#define R_RTC_PCR_BUC                         0x3414               ///< Backed Up Control
#define B_RTC_PCR_BUC_DSO                     BIT4                 ///< Daylight Savings Override
#define B_RTC_PCR_BUC_TS                      BIT0                 ///< Top Swap
#define R_RTC_PCR_RTCDCG                      0x3418               ///< RTC Dynamic Clock Gating Control
#define R_RTC_PCR_RTCDCG_RTCPGCBDCGEN         BIT2                 ///< pgcb_clk (12Mhz) Dynamic Clock Gate Enable
#define R_RTC_PCR_RTCDCG_RTCPCICLKDCGEN       BIT1                 ///< ipciclk_clk (24 MHz) Dynamic Clock Gate Enable
#define R_RTC_PCR_RTCDCG_RTCROSIDEDCGEN       BIT0                 ///< rosc_side_clk (120 MHz) Dynamic Clock Gate Enable
#define R_RTC_PCR_PG1_CP_LO                   0x3428
#define R_RTC_PCR_PG1_AC_LO                   0x3438
#define R_RTC_PCR_3F00                        0x3F00
#define R_RTC_PCR_UIPSMI                      0x3F04               ///< RTC Update In Progress SMI Control

//
// LPC PCR Registers
//
#define R_LPC_PCR_HVMTCTL                     0x3410
#define R_LPC_PCR_GCFD                        0x3418
#define B_LPC_PCR_GCFD_SRVR_CLKRUN_EN         BIT2                 ///< Enables the CLKRUN# logic to stop the PCI clocks
#define R_LPC_PCR_PRC                         0x341C
#define R_LPC_PCR_PCT                         0x3420
#define R_LPC_PCR_SCT                         0x3424
#define R_LPC_PCR_LPCCT                       0x3428
#define R_LPC_PCR_ULTOR                       0x3500

//
// eSPI PCR Registers
//
#define R_ESPI_PCR_SLV_CFG_REG_CTL            0x4000                  ///< Slave Configuration Register and Link Control
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRE       BIT31                   ///< Slave Configuration Register Access Enable
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRS       (BIT30 | BIT29 | BIT28) ///< Slave Configuration Register Access Status
#define N_ESPI_PCR_SLV_CFG_REG_CTL_SCRS       28                      ///< Slave Configuration Register Access Status bit position
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SBLCL      BIT27                   ///< IOSF-SB eSPI Link Configuration Lock
#define V_ESPI_PCR_SLV_CFG_REG_CTL_SCRS_NOERR 7                       ///< No errors (transaction completed successfully)
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SID        (BIT20 | BIT19)         ///< Slave ID
#define N_ESPI_PCR_SLV_CFG_REG_CTL_SID        19                      ///< Slave ID bit position
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRT       (BIT17 | BIT16)         ///< Slave Configuration Register Access Type
#define N_ESPI_PCR_SLV_CFG_REG_CTL_SCRT       16                      ///< Slave Configuration Register Access Type bit position
#define V_ESPI_PCR_SLV_CFG_REG_CTL_SCRT_RD    0                       ///< Slave Configuration register read from address SCRA[11:0]
#define V_ESPI_PCR_SLV_CFG_REG_CTL_SCRT_WR    1                       ///< Slave Configuration register write to address SCRA[11:0]
#define V_ESPI_PCR_SLV_CFG_REG_CTL_SCRT_STS   2                       ///< Slave Status register read
#define V_ESPI_PCR_SLV_CFG_REG_CTL_SCRT_RS    3                       ///< In-Band reset
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRA       0x00000FFF              ///< Slave Configuration Register Address
#define R_ESPI_PCR_SLV_CFG_REG_DATA           0x4004                  ///< Slave Configuration Register Data

#define R_ESPI_PCR_PCERR_SLV0                 0x4020          ///< Peripheral Channel Error for Slave 0
#define B_ESPI_PCR_PCERR_PCURD                BIT24           ///< Peripheral Channel Unsupported Request Detected
#define R_ESPI_PCR_PCERR_SLV1                 0x4024          ///< Peripheral Channel Error for Slave 1
#define R_ESPI_PCR_VWERR_SLV0                 0x4030          ///< Virtual Wire Channel Error for Slave 0
#define R_ESPI_PCR_VWERR_SLV1                 0x4034          ///< Virtual Wire Channel Error for Slave 1
#define R_ESPI_PCR_FCERR_SLV0                 0x4040          ///< Flash Access Channel Error for Slave 0
#define B_ESPI_PCR_FCERR_SAFBLK               BIT17           ///< SAF Blocked (SAFBLK)
#define B_ESPI_PCR_XERR_XNFEE                 (BIT14 | BIT13) ///< Non-Fatal Error Reporting Enable bits
#define N_ESPI_PCR_XERR_XNFEE                 13              ///< Non-Fatal Error Reporting Enable bit position
#define V_ESPI_PCR_XERR_XNFEE_SMI             3               ///< Enable Non-Fatal Error Reporting as SMI
#define B_ESPI_PCR_XERR_XNFES                 BIT12           ///< Fatal Error Status
#define B_ESPI_PCR_XERR_XFEE                  (BIT6 | BIT5)   ///< Fatal Error Reporting Enable bits
#define N_ESPI_PCR_XERR_XFEE                  5               ///< Fatal Error Reporting Enable bit position
#define V_ESPI_PCR_XERR_XFEE_SMI              3               ///< Enable Fatal Error Reporting as SMI
#define B_ESPI_PCR_XERR_XFES                  BIT4            ///< Fatal Error Status
#define S_ESPI_PCR_XERR                       4               ///< Channel register sizes
#define B_ESPI_PCR_PCERR_SLV0_PCURD           BIT24           ///< Peripheral Channel Unsupported Request Detected
#define R_ESPI_PCR_LNKERR_SLV0                0x4050          ///< Link Error for Slave 0
#define S_ESPI_PCR_LNKERR_SLV0                4               ///< Link Error for Slave 0 register size
#define B_ESPI_PCR_LNKERR_SLV0_SLCRR          BIT31           ///< eSPI Link and Slave Channel Recovery Required
#define B_ESPI_PCR_LNKERR_SLV0_LFET1E         (BIT22 | BIT21) ///< Fatal Error Type 1 Reporting Enable
#define N_ESPI_PCR_LNKERR_SLV0_LFET1E         21              ///< Fatal Error Type 1 Reporting Enable bit position
#define V_ESPI_PCR_LNKERR_SLV0_LFET1E_SMI     3               ///< Enable Fatal Error Type 1 Reporting as SMI
#define B_ESPI_PCR_LNKERR_SLV0_LFET1S         BIT20           ///< Link Fatal Error Type 1 Status
#define R_ESPI_PCR_LNKERR_SLV1                0x4054          ///< Link Error for Slave 1
#define R_ESPI_PCR_CFG_VAL                    0xC00C          ///< ESPI Enabled Strap
#define B_ESPI_PCR_CFG_VAL_ESPI_EN            BIT0            ///< ESPI Enabled Strap bit position
#define R_ESPI_PCR_SOFTSTRAPS                 0xC210          ///< eSPI Sofstraps Register 0
#define R_ESPI_PCR_SOFTSTRAPS_CS1_EN          BIT12           ///< CS1# Enable

#endif
