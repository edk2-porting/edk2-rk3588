/** @file
  Register names for PCH PMC device
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

#ifndef _PCH_REGS_PMC_H_
#define _PCH_REGS_PMC_H_

//
//PMC Registers (D31:F2)
//
#define PCI_DEVICE_NUMBER_PCH_PMC                 31
#define PCI_FUNCTION_NUMBER_PCH_PMC               2

#define V_PCH_PMC_VENDOR_ID                       V_PCH_INTEL_VENDOR_ID
#define V_PCH_H_PMC_DEVICE_ID                               0x9D21
//
// LBG Production (PRQ) PMC Device ID
//
#define V_PCH_LBG_PROD_PMC_DEVICE_ID              0xA1A1
//
// LBG Super SKU (SSX) PMC Device ID
//
#define V_PCH_LBG_PMC_DEVICE_ID                   0xA221

#define R_PCH_PMC_BM_CX_CNF                       0xA8
#define V_PCH_LP_PMC_DEVICE_ID                              0x9D21
#define R_PCH_PMC_PM_DATA_BAR                               0x10
#define B_PCH_PMC_PM_DATA_BAR                               0xFFFFC000
#define R_PCH_PMC_ACPI_BASE                       0x40
#define B_PCH_PMC_ACPI_BASE_BAR                   0xFFFC
#define R_PCH_PMC_ACPI_CNT                        0x44
#define B_PCH_PMC_ACPI_CNT_PWRM_EN                BIT8                          ///< PWRM enable
#define B_PCH_PMC_ACPI_CNT_ACPI_EN                BIT7                          ///< ACPI eanble
#define B_PCH_PMC_ACPI_CNT_SCIS                   (BIT2 | BIT1 | BIT0)          ///< SCI IRQ select
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ9                        0
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ10                       1
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ11                       2
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ20                       4
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ21                       5
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ22                       6
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ23                       7
#define R_PCH_PMC_PWRM_BASE                       0x48
#define B_PCH_PMC_PWRM_BASE_BAR                   0xFFFF0000                    ///< PWRM must be 64KB alignment to align the source decode.
#define R_PCH_PMC_GEN_PMCON_A                               0xA0
#define B_PCH_PMC_GEN_PMCON_A_DC_PP_DIS                     BIT30
#define B_PCH_PMC_GEN_PMCON_A_DSX_PP_DIS                    BIT29
#define B_PCH_PMC_GEN_PMCON_A_AG3_PP_EN                     BIT28
#define B_PCH_PMC_GEN_PMCON_A_SX_PP_EN                      BIT27
#define B_PCH_PMC_GEN_PMCON_A_DISB                          BIT23
#define B_PCH_PMC_GEN_PMCON_A_MEM_SR                        BIT21
#define B_PCH_PMC_GEN_PMCON_A_MS4V                          BIT18
#define B_PCH_PMC_GEN_PMCON_A_GBL_RST_STS                   BIT16
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_PLL_SD_INC0             BIT13
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_SPXB_CG_INC0            BIT12
#define B_PCH_PMC_GEN_PMCON_A_BIOS_PCI_EXP_EN               BIT10
#define B_PCH_PMC_GEN_PMCON_A_PWRBTN_LVL                    BIT9
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_L1LOW_C0                BIT7
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_L1LOW_OPI_ON            BIT6
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_L1LOW_BCLKREQ_ON        BIT5
#define B_PCH_PMC_GEN_PMCON_A_SMI_LOCK                      BIT4
#define B_PCH_PMC_GEN_PMCON_A_ESPI_SMI_LOCK                 BIT3             ///< ESPI SMI lock
#define B_PCH_PMC_GEN_PMCON_A_PER_SMI_SEL                   0x0003
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_64S                   0x0000
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_32S                   0x0001
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_16S                   0x0002
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_8S                    0x0003
#define R_PCH_PMC_GEN_PMCON_B                               0xA4
#define B_PCH_PMC_GEN_PMCON_B_SLPSX_STR_POL_LOCK            BIT18            ///< Lock down SLP_S3/SLP_S4 Minimum Assertion width
#define B_PCH_PMC_GEN_PMCON_B_ACPI_BASE_LOCK                BIT17            ///< Lock ACPI BASE at 0x40, only cleared by reset when set
#define B_PCH_PMC_GEN_PMCON_B_PM_DATA_BAR_DIS               BIT16
#define B_PCH_PMC_GEN_PMCON_B_PME_B0_S5_DIS                 BIT15
#define B_PCH_PMC_GEN_PMCON_B_SUS_PWR_FLR                   BIT14
#define B_PCH_PMC_GEN_PMCON_B_WOL_EN_OVRD                   BIT13
#define B_PCH_PMC_GEN_PMCON_B_DISABLE_SX_STRETCH            BIT12
#define B_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW                    0xC00
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_60US               0x000
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_1MS                0x400
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_50MS               0x800
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_2S                 0xC00
#define B_PCH_PMC_GEN_PMCON_B_HOST_RST_STS                  BIT9
#define B_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL                    0xC0
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_64MS               0xC0
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_32MS               0x80
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_16MS               0x40
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_1_5MS              0x00
#define B_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW                    0x30
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_1S                 0x30
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_2S                 0x20
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_3S                 0x10
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_4S                 0x00
#define B_PCH_PMC_GEN_PMCON_B_SLP_S4_ASE                    BIT3
#define B_PCH_PMC_GEN_PMCON_B_RTC_PWR_STS                   BIT2
#define B_PCH_PMC_GEN_PMCON_B_PWR_FLR                       BIT1
#define B_PCH_PMC_GEN_PMCON_B_AFTERG3_EN                    BIT0
#define R_PCH_PMC_BM_CX_CNF                       0xA8
#define B_PCH_PMC_BM_CX_CNF_STORAGE_BREAK_EN      BIT31
#define B_PCH_PMC_BM_CX_CNF_PCIE_BREAK_EN         BIT30
#define B_PCH_PMC_BM_CX_CNF_EHCI_BREAK_EN         BIT26
#define B_PCH_PMC_BM_CX_CNF_AZ_BREAK_EN           BIT24
#define B_PCH_PMC_BM_CX_CNF_DPSN_BREAK_EN         BIT19
#define B_PCH_PMC_BM_CX_CNF_XHCI_BREAK_EN         BIT17
#define B_PCH_PMC_BM_CX_CNF_SATA3_BREAK_EN        BIT16
#define B_PCH_PMC_BM_CX_CNF_SCRATCHPAD            BIT15
#define B_PCH_PMC_BM_CX_CNF_PHOLD_BM_STS_BLOCK    BIT14
#define B_PCH_PMC_BM_CX_CNF_MASK_CF               BIT11
#define B_PCH_PMC_BM_CX_CNF_BM_STS_ZERO_EN        BIT10
#define B_PCH_PMC_BM_CX_CNF_PM_SYNC_MSG_MODE      BIT9
#define R_PCH_PMC_ETR3                            0xAC
#define B_PCH_PMC_ETR3_CF9LOCK                    BIT31          ///< CF9h Lockdown
#define B_PCH_PMC_ETR3_USB_CACHE_DIS              BIT21
#define B_PCH_PMC_ETR3_CF9GR                      BIT20          ///< CF9h Global Reset
#define B_PCH_PMC_ETR3_SKIP_HOST_RST_HS           BIT19
#define B_PCH_PMC_ETR3_CWORWRE                    BIT18
#define R_PCH_PMC_PMC_THROT_1                     0xB0
#define B_PCH_PMC_PMC_PMC_THROT_LOCK              BIT15
#define B_PCH_PMC_PMC_THROT_1_VRALERT_EN                    BIT0
#define R_PCH_PMC_MDAP                            0xC0
#define B_PCH_PMC_MDAP_MDAP_POLICY_EN             BIT31
#define B_PCH_PMC_MDAP_PDMA_EN                    BIT30
#define B_PCH_PMC_MDAP_AUTO_POLICY_CTL            BIT29
#define V_PCH_PMC_MDAP_MDAP_VALUE                 0x1FFFF
#define R_PCH_PMC_MANID                           0xF8

//
// ACPI and legacy I/O register offsets from ACPIBASE
//
#define R_ACPI_IO_PM1_STS                        0x00
#define S_ACPI_IO_PM1_STS                        2
#define B_ACPI_IO_PM1_STS_WAK                    0x8000
#define B_ACPI_IO_PM1_STS_PRBTNOR                0x0800
#define B_ACPI_IO_PM1_STS_RTC                    0x0400
#define B_ACPI_IO_PM1_STS_PWRBTN                 0x0100
#define B_ACPI_IO_PM1_STS_GBL                    0x0020
#define B_ACPI_IO_PM1_STS_BM                     0x0010
#define B_ACPI_IO_PM1_STS_TMROF                  0x0001
#define N_ACPI_IO_PM1_STS_WAK                    15
#define N_ACPI_IO_PM1_STS_PRBTNOR                11
#define N_ACPI_IO_PM1_STS_RTC                    10
#define N_ACPI_IO_PM1_STS_PWRBTN                 8
#define N_ACPI_IO_PM1_STS_GBL                    5
#define N_ACPI_IO_PM1_STS_BM                     4
#define N_ACPI_IO_PM1_STS_TMROF                  0

#define R_ACPI_IO_PM1_EN                         0x02
#define S_ACPI_IO_PM1_EN                         2
#define B_ACPI_IO_PM1_EN_PCIEXP_WAKE_DIS         0x4000
#define B_ACPI_IO_PM1_EN_RTC                     0x0400
#define B_ACPI_IO_PM1_EN_PWRBTN                  0x0100
#define B_ACPI_IO_PM1_EN_GBL                     0x0020
#define B_ACPI_IO_PM1_EN_TMROF                   0x0001
#define N_ACPI_IO_PM1_EN_RTC                     10
#define N_ACPI_IO_PM1_EN_PWRBTN                  8
#define N_ACPI_IO_PM1_EN_GBL                     5
#define N_ACPI_IO_PM1_EN_TMROF                   0

#define R_ACPI_IO_PM1_CNT                        0x04
#define S_ACPI_IO_PM1_CNT                        4
#define B_ACPI_IO_PM1_CNT_SLP_EN                 0x00002000
#define B_ACPI_IO_PM1_CNT_SLP_TYP                0x00001C00
#define V_ACPI_IO_PM1_CNT_S0                     0x00000000
#define V_ACPI_IO_PM1_CNT_S1                     0x00000400
#define V_ACPI_IO_PM1_CNT_S3                     0x00001400
#define V_ACPI_IO_PM1_CNT_S4                     0x00001800
#define V_ACPI_IO_PM1_CNT_S5                     0x00001C00
#define B_ACPI_IO_PM1_CNT_GBL_RLS                0x00000004
#define B_ACPI_IO_PM1_CNT_BM_RLD                 0x00000002
#define B_ACPI_IO_PM1_CNT_SCI_EN                 0x00000001

#define R_ACPI_IO_PM1_TMR                        0x08
#define V_ACPI_IO_TMR_FREQUENCY                  3579545
#define B_ACPI_IO_PM1_TMR_VAL                    0xFFFFFF
#define V_ACPI_IO_PM1_TMR_MAX_VAL                0x1000000       ///< The timer is 24 bit overflow

#define R_ACPI_IO_SMI_EN                              0x30
#define S_ACPI_IO_SMI_EN                              4
#define B_ACPI_IO_SMI_EN_LEGACY_USB3                  BIT31
#define B_ACPI_IO_SMI_EN_GPIO_UNLOCK_SMI              BIT27
#define B_ACPI_IO_SMI_EN_IE_SMI                       BIT23
#define B_ACPI_IO_SMI_EN_INTEL_USB2                   BIT18
#define B_ACPI_IO_SMI_EN_LEGACY_USB2                  BIT17
#define B_ACPI_IO_SMI_EN_PERIODIC                     BIT14
#define B_ACPI_IO_SMI_EN_TCO                          BIT13
#define B_ACPI_IO_SMI_EN_MCSMI                        BIT11
#define B_ACPI_IO_SMI_EN_BIOS_RLS                     BIT7
#define B_ACPI_IO_SMI_EN_SWSMI_TMR                    BIT6
#define B_ACPI_IO_SMI_EN_APMC                         BIT5
#define B_ACPI_IO_SMI_EN_ON_SLP_EN                    BIT4
#define B_ACPI_IO_SMI_EN_LEGACY_USB                   BIT3
#define B_ACPI_IO_SMI_EN_BIOS                         BIT2
#define B_ACPI_IO_SMI_EN_EOS                          BIT1
#define B_ACPI_IO_SMI_EN_GBL_SMI                      BIT0
#define N_ACPI_IO_SMI_EN_LEGACY_USB3                  31
#define N_ACPI_IO_SMI_EN_ESPI                         28
#define N_ACPI_IO_SMI_EN_GPIO_UNLOCK                  27
#define N_ACPI_IO_SMI_EN_INTEL_USB2                   18
#define N_ACPI_IO_SMI_EN_LEGACY_USB2                  17
#define N_ACPI_IO_SMI_EN_PERIODIC                     14
#define N_ACPI_IO_SMI_EN_TCO                          13
#define N_ACPI_IO_SMI_EN_MCSMI                        11
#define N_ACPI_IO_SMI_EN_BIOS_RLS                     7
#define N_ACPI_IO_SMI_EN_SWSMI_TMR                    6
#define N_ACPI_IO_SMI_EN_APMC                         5
#define N_ACPI_IO_SMI_EN_ON_SLP_EN                    4
#define N_ACPI_IO_SMI_EN_LEGACY_USB                   3
#define N_ACPI_IO_SMI_EN_BIOS                         2
#define N_ACPI_IO_SMI_EN_EOS                          1
#define N_ACPI_IO_SMI_EN_GBL_SMI                      0

#define R_ACPI_IO_SMI_STS                             0x34
#define S_ACPI_IO_SMI_STS                             4
#define B_ACPI_IO_SMI_STS_LEGACY_USB3                 BIT31
#define B_ACPI_IO_SMI_STS_GPIO_UNLOCK                 BIT27
#define B_ACPI_IO_SMI_STS_SPI                         BIT26
#define B_ACPI_IO_SMI_STS_IE_SMI                      BIT23
#define B_ACPI_IO_SMI_STS_MONITOR                     BIT21
#define B_ACPI_IO_SMI_STS_PCI_EXP                     BIT20
#define B_ACPI_IO_SMI_STS_PATCH                       BIT19
#define B_ACPI_IO_SMI_STS_INTEL_USB2                  BIT18
#define B_ACPI_IO_SMI_STS_LEGACY_USB2                 BIT17
#define B_ACPI_IO_SMI_STS_SMBUS                       BIT16
#define B_ACPI_IO_SMI_STS_SERIRQ                      BIT15
#define B_ACPI_IO_SMI_STS_PERIODIC                    BIT14
#define B_ACPI_IO_SMI_STS_TCO                         BIT13
#define B_ACPI_IO_SMI_STS_DEVMON                      BIT12
#define B_ACPI_IO_SMI_STS_MCSMI                       BIT11
#define B_ACPI_IO_SMI_STS_GPIO_SMI                    BIT10
#define B_ACPI_IO_SMI_STS_GPE1                        BIT10
#define B_ACPI_IO_SMI_STS_GPE0                        BIT9
#define B_ACPI_IO_SMI_STS_PM1_STS_REG                 BIT8
#define B_ACPI_IO_SMI_STS_SWSMI_TMR                   BIT6
#define B_ACPI_IO_SMI_STS_APM                         BIT5
#define B_ACPI_IO_SMI_STS_ON_SLP_EN                   BIT4
#define B_ACPI_IO_SMI_STS_LEGACY_USB                  BIT3
#define B_ACPI_IO_SMI_STS_BIOS                        BIT2
#define N_ACPI_IO_SMI_STS_LEGACY_USB3                 31
#define N_ACPI_IO_SMI_STS_ESPI                        28
#define N_ACPI_IO_SMI_STS_GPIO_UNLOCK                 27
#define N_ACPI_IO_SMI_STS_SPI                         26
#define N_ACPI_IO_SMI_STS_MONITOR                     21
#define N_ACPI_IO_SMI_STS_PCI_EXP                     20
#define N_ACPI_IO_SMI_STS_PATCH                       19
#define N_ACPI_IO_SMI_STS_INTEL_USB2                  18
#define N_ACPI_IO_SMI_STS_LEGACY_USB2                 17
#define N_ACPI_IO_SMI_STS_SMBUS                       16
#define N_ACPI_IO_SMI_STS_SERIRQ                      15
#define N_ACPI_IO_SMI_STS_PERIODIC                    14
#define N_ACPI_IO_SMI_STS_TCO                         13
#define N_ACPI_IO_SMI_STS_DEVMON                      12
#define N_ACPI_IO_SMI_STS_MCSMI                       11
#define N_ACPI_IO_SMI_STS_GPIO_SMI                    10
#define N_ACPI_IO_SMI_STS_GPE0                        9
#define N_ACPI_IO_SMI_STS_PM1_STS_REG                 8
#define N_ACPI_IO_SMI_STS_SWSMI_TMR                   6
#define N_ACPI_IO_SMI_STS_APM                         5
#define N_ACPI_IO_SMI_STS_ON_SLP_EN                   4
#define N_ACPI_IO_SMI_STS_LEGACY_USB                  3
#define N_ACPI_IO_SMI_STS_BIOS                        2

#define R_ACPI_IO_GPE_CNTL                       0x40
#define B_ACPI_IO_GPE_CNTL_SWGPE_CTRL            BIT17

#define R_ACPI_IO_DEVACT_STS                          0x44
#define S_ACPI_IO_DEVACT_STS                          2
#define B_ACPI_IO_DEVACT_STS_MASK                     0x13E1
#define B_ACPI_IO_DEVACT_STS_KBC                      0x1000
#define B_ACPI_IO_DEVACT_STS_PIRQDH                   0x0200
#define B_ACPI_IO_DEVACT_STS_PIRQCG                   0x0100
#define B_ACPI_IO_DEVACT_STS_PIRQBF                   0x0080
#define B_ACPI_IO_DEVACT_STS_PIRQAE                   0x0040
#define B_ACPI_IO_DEVACT_STS_D0_TRP                   0x0001
#define N_ACPI_IO_DEVACT_STS_KBC                      12
#define N_ACPI_IO_DEVACT_STS_PIRQDH                   9
#define N_ACPI_IO_DEVACT_STS_PIRQCG                   8
#define N_ACPI_IO_DEVACT_STS_PIRQBF                   7
#define N_ACPI_IO_DEVACT_STS_PIRQAE                   6

#define R_ACPI_IO_PM2_CNT                        0x50
#define B_ACPI_IO_PM2_CNT_ARB_DIS                0x01

#define R_ACPI_IO_OC_WDT_CTL                          0x54
#define B_ACPI_IO_OC_WDT_CTL_RLD                      BIT31
#define B_ACPI_IO_OC_WDT_CTL_ICCSURV_STS              BIT25
#define B_ACPI_IO_OC_WDT_CTL_NO_ICCSURV_STS           BIT24
#define B_ACPI_IO_OC_WDT_CTL_FORCE_ALL                BIT15
#define B_ACPI_IO_OC_WDT_CTL_EN                       BIT14
#define B_ACPI_IO_OC_WDT_CTL_ICCSURV                  BIT13
#define B_ACPI_IO_OC_WDT_CTL_LCK                      BIT12
#define B_ACPI_IO_OC_WDT_CTL_TOV_MASK                 0x3FF
#define B_ACPI_IO_OC_WDT_CTL_FAILURE_STS              BIT23
#define B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS           BIT22
#define B_ACPI_IO_OC_WDT_CTL_AFTER_POST               0x3F0000
#define V_ACPI_IO_OC_WDT_CTL_STATUS_FAILURE           1
#define V_ACPI_IO_OC_WDT_CTL_STATUS_OK                0

#define R_ACPI_IO_GPE0_STS_127_96                  0x8C
#define S_ACPI_IO_GPE0_STS_127_96                  4
#define B_ACPI_IO_GPE0_STS_127_96_WADT             BIT18
#define B_ACPI_IO_GPE0_STS_127_96_USB_CON_DSX_STS  BIT17
#define B_ACPI_IO_GPE0_STS_127_96_LAN_WAKE         BIT16
#define B_ACPI_IO_GPE0_STS_127_96_GPIO_TIER_2      BIT15
#define B_ACPI_IO_GPE0_STS_127_96_PME_B0           BIT13
#define B_ACPI_IO_GPE0_STS_127_96_ME_SCI           BIT12
#define B_ACPI_IO_GPE0_STS_127_96_PME              BIT11
#define B_ACPI_IO_GPE0_STS_127_96_BATLOW           BIT10
#define B_ACPI_IO_GPE0_STS_127_96_PCI_EXP          BIT9
#define B_ACPI_IO_GPE0_STS_127_96_RI               BIT8
#define B_ACPI_IO_GPE0_STS_127_96_SMB_WAK          BIT7
#define B_ACPI_IO_GPE0_STS_127_96_TC0SCI           BIT6
#define B_ACPI_IO_GPE0_STS_127_96_SWGPE            BIT2
#define B_ACPI_IO_GPE0_STS_127_96_HOT_PLUG         BIT1
#define N_ACPI_IO_GPE0_STS_127_96_GPIO_TIER_2      15
#define N_ACPI_IO_GPE0_STS_127_96_PME_B0           13
#define N_ACPI_IO_GPE0_STS_127_96_PME              11
#define N_ACPI_IO_GPE0_STS_127_96_BATLOW           10
#define N_ACPI_IO_GPE0_STS_127_96_PCI_EXP          9
#define N_ACPI_IO_GPE0_STS_127_96_RI               8
#define N_ACPI_IO_GPE0_STS_127_96_SMB_WAK          7
#define N_ACPI_IO_GPE0_STS_127_96_TC0SCI           6
#define N_ACPI_IO_GPE0_STS_127_96_SWGPE            2
#define N_ACPI_IO_GPE0_STS_127_96_HOT_PLUG         1


#define R_ACPI_IO_GPE0_EN_31_0                   0x90
#define R_ACPI_IO_GPE0_EN_63_31                  0x94
#define R_ACPI_IO_GPE0_EN_94_64                  0x98
#define R_ACPI_IO_GPE0_EN_127_96                 0x9C
#define S_ACPI_IO_GPE0_EN_127_96                 4
#define B_ACPI_IO_GPE0_EN_127_96_WADT            BIT18
#define B_ACPI_IO_GPE0_EN_127_96_USB_CON_DSX_EN  BIT17
#define B_ACPI_IO_GPE0_EN_127_96_LAN_WAKE        BIT16
#define B_ACPI_IO_GPE0_EN_127_96_PME_B0          BIT13
#define B_ACPI_IO_GPE0_EN_127_96_ME_SCI          BIT12
#define B_ACPI_IO_GPE0_EN_127_96_PME             BIT11
#define B_ACPI_IO_GPE0_EN_127_96_BATLOW          BIT10
#define B_ACPI_IO_GPE0_EN_127_96_PCI_EXP         BIT9
#define B_ACPI_IO_GPE0_EN_127_96_RI              BIT8
#define B_ACPI_IO_GPE0_EN_127_96_TC0SCI          BIT6
#define B_ACPI_IO_GPE0_EN_127_96_SWGPE           BIT2
#define B_ACPI_IO_GPE0_EN_127_96_HOT_PLUG        BIT1
#define N_ACPI_IO_GPE0_EN_127_96_PME_B0          13
#define N_ACPI_IO_GPE0_EN_127_96_USB3            12
#define N_ACPI_IO_GPE0_EN_127_96_PME             11
#define N_ACPI_IO_GPE0_EN_127_96_BATLOW          10
#define N_ACPI_IO_GPE0_EN_127_96_PCI_EXP         9
#define N_ACPI_IO_GPE0_EN_127_96_RI              8
#define N_ACPI_IO_GPE0_EN_127_96_TC0SCI          6
#define N_ACPI_IO_GPE0_EN_127_96_SWGPE           2
#define N_ACPI_IO_GPE0_EN_127_96_HOT_PLUG        1


//
// TCO register I/O map
//
#define R_TCO_IO_RLD                                 0x0
#define R_TCO_IO_DAT_IN                              0x2
#define R_TCO_IO_DAT_OUT                             0x3
#define R_TCO_IO_TCO1_STS                            0x04
#define S_TCO_IO_TCO1_STS                            2
#define B_TCO_IO_TCO1_STS_DMISERR                    BIT12
#define B_TCO_IO_TCO1_STS_DMISMI                     BIT10
#define B_TCO_IO_TCO1_STS_DMISCI                     BIT9
#define B_TCO_IO_TCO1_STS_BIOSWR                     BIT8
#define B_TCO_IO_TCO1_STS_NEWCENTURY                 BIT7
#define B_TCO_IO_TCO1_STS_TIMEOUT                    BIT3
#define B_TCO_IO_TCO1_STS_TCO_INT                    BIT2
#define B_TCO_IO_TCO1_STS_SW_TCO_SMI                 BIT1
#define B_TCO_IO_TCO1_STS_NMI2SMI                    BIT0
#define N_TCO_IO_TCO1_STS_DMISMI                     10
#define N_TCO_IO_TCO1_STS_BIOSWR                     8
#define N_TCO_IO_TCO1_STS_NEWCENTURY                 7
#define N_TCO_IO_TCO1_STS_TIMEOUT                    3
#define N_TCO_IO_TCO1_STS_SW_TCO_SMI                 1
#define N_TCO_IO_TCO1_STS_NMI2SMI                    0

#define R_TCO_IO_TCO2_STS                            0x06
#define S_TCO_IO_TCO2_STS                            2
#define B_TCO_IO_TCO2_STS_SMLINK_SLV_SMI             BIT4
#define B_TCO_IO_TCO2_STS_BAD_BIOS                   BIT3
#define B_TCO_IO_TCO2_STS_BOOT                       BIT2
#define B_TCO_IO_TCO2_STS_SECOND_TO                  BIT1
#define B_TCO_IO_TCO2_STS_INTRD_DET                  BIT0
#define N_TCO_IO_TCO2_STS_INTRD_DET                  0

#define R_TCO_IO_TCO1_CNT                            0x08
#define S_TCO_IO_TCO1_CNT                            2
#define B_TCO_IO_TCO1_CNT_LOCK                       BIT12
#define B_TCO_IO_TCO1_CNT_TMR_HLT                    BIT11
#define B_TCO_IO_TCO1_CNT_NMI2SMI_EN                 BIT9
#define B_TCO_IO_TCO1_CNT_NMI_NOW                    BIT8
#define N_TCO_IO_TCO1_CNT_NMI2SMI_EN                 9

#define R_TCO_IO_TCO2_CNT                            0x0A
#define S_TCO_IO_TCO2_CNT                            2
#define B_TCO_IO_TCO2_CNT_OS_POLICY                  0x0030
#define B_TCO_IO_TCO2_CNT_GPI11_ALERT_DISABLE        0x0008
#define B_TCO_IO_TCO2_CNT_INTRD_SEL                  0x0006
#define N_TCO_IO_TCO2_CNT_INTRD_SEL                  2

#define R_TCO_IO_MESSAGE1                            0x0C
#define R_TCO_IO_MESSAGE2                            0x0D
#define R_TCO_IO_TWDS                                0x0E           ///< TCO_WDSTATUS register.
#define R_TCO_IO_LE                                  0x10           ///< LEGACY_ELIM register
#define B_TCO_IO_LE_IRQ12_CAUSE                      BIT1
#define B_TCO_IO_LE_IRQ1_CAUSE                       BIT0
#define R_TCO_IO_TMR                                 0x12

//
// PWRM Registers
//
#define R_PCH_WADT_AC                                       0x0                         ///< Wake Alarm Device Timer: AC
#define R_PCH_WADT_DC                                       0x4                         ///< Wake Alarm Device Timer: DC
#define R_PCH_WADT_EXP_AC                                   0x8                         ///< Wake Alarm Device Expired Timer: AC
#define R_PCH_WADT_EXP_DC                                   0xC                         ///< Wake Alarm Device Expired Timer: DC
#define R_PCH_PWRM_PRSTS                                    0x10                        ///< Power and Reset Status
#define B_PCH_PWRM_PRSTS_PM_WD_TMR                          BIT15                       ///< Power Management Watchdog Timer
#define B_PCH_PWRM_PRSTS_VE_WD_TMR_STS                      BIT7                        ///< VE Watchdog Timer Status
#define B_PCH_PWRM_PRSTS_ME_WD_TMR_STS                      BIT6                        ///< Management Engine Watchdog Timer Status
#define B_PCH_PWRM_PRSTS_WOL_OVR_WK_STS                     BIT5
#define B_PCH_PWRM_PRSTS_FIELD_1                            BIT4
#define B_PCH_PWRM_PRSTS_ME_HOST_PWRDN                      BIT3
#define B_PCH_PWRM_PRSTS_ME_HRST_WARM_STS                   BIT2
#define B_PCH_PWRM_PRSTS_ME_HRST_COLD_STS                   BIT1
#define B_PCH_PWRM_PRSTS_ME_WAKE_STS                        BIT0
#define R_PCH_PWRM_14                                       0x14
#define R_PCH_PWRM_CFG                                      0x18                        ///< Power Management Configuration
#define V_PCH_PWRM_CFG_TIMING_T581_10MS                     (BIT0 | BIT1)               ///< Timing t581 - 10ms
#define B_PCH_PWRM_CFG_ALLOW_24_OSC_SD                      BIT29                       ///< Allow 24MHz Crystal Oscillator Shutdown
#define B_PCH_PWRM_CFG_ALLOW_USB2_CORE_PG                   BIT25                       ///< Allow USB2 Core Power Gating
#define B_PCH_PWRM_CFG_RTC_DS_WAKE_DIS                      BIT21                       ///< RTC Wake from Deep S4/S5 Disable
#define B_PCH_PWRM_CFG_SSMAW_MASK                           (BIT19 | BIT18)             ///< SLP_SUS# Min Assertion Width
#define V_PCH_PWRM_CFG_SSMAW_4S                             (BIT19 | BIT18)             ///< 4 seconds
#define V_PCH_PWRM_CFG_SSMAW_1S                             BIT19                       ///< 1 second
#define V_PCH_PWRM_CFG_SSMAW_0_5S                           BIT18                       ///< 0.5 second (500ms)
#define V_PCH_PWRM_CFG_SSMAW_0S                             0                           ///< 0 second
#define B_PCH_PWRM_CFG_SAMAW_MASK                           (BIT17 | BIT16)             ///< SLP_A# Min Assertion Width
#define V_PCH_PWRM_CFG_SAMAW_2S                             (BIT17 | BIT16)             ///< 2 seconds
#define V_PCH_PWRM_CFG_SAMAW_98ms                           BIT17                       ///< 98ms
#define V_PCH_PWRM_CFG_SAMAW_4S                             BIT16                       ///< 4 seconds
#define V_PCH_PWRM_CFG_SAMAW_0S                             0                           ///< 0 second
#define B_PCH_PWRM_CFG_RPCD_MASK                            (BIT9 | BIT8)               ///< Reset Power Cycle Duration
#define V_PCH_PWRM_CFG_RPCD_1S                              (BIT9 | BIT8)               ///< 1-2 seconds
#define V_PCH_PWRM_CFG_RPCD_2S                              BIT9                        ///< 2-3 seconds
#define V_PCH_PWRM_CFG_RPCD_3S                              BIT8                        ///< 3-4 seconds
#define V_PCH_PWRM_CFG_RPCD_4S                              0                           ///< 4-5 seconds (Default)
#define R_PCH_PWRM_MTPMC                                    0x20                        ///< Message to PMC
#define V_PCH_PWRM_MTPMC_COMMAND_PG_LANE_0_15               0xE                         ///< Command to override lanes 0-15 power gating
#define V_PCH_PWRM_MTPMC_COMMAND_PG_LANE_16_31              0xF                         ///< Command to override lanes 16-31 power gating
#define B_PCH_PWRM_MTPMC_PG_CMD_DATA                        0xFFFF0000                  ///< Data part of PowerGate Message to PMC
#define N_PCH_PWRM_MTPMC_PG_CMD_DATA                        16
#define R_PCH_PWRM_PCH_PM_STS                               0x1C                        ///< PCH Power Management Status
#define B_PCH_PWRM_PCH_PM_STS_ADR_RST_STS                   BIT16                       ///< ADR Reset Status
#define R_PCH_PWRM_S0_S1_PWRGATE_POL                        0x24                        ///< S0/S1 Power Gating Policies
#define R_PCH_PWRM_S3_PWRGATE_POL                           0x28                        ///< S3 Power Gating Policies
#define B_PCH_PWRM_S3DC_GATE_SUS                            BIT1                        ///< Deep S3 Enable in DC Mode
#define B_PCH_PWRM_S3AC_GATE_SUS                            BIT0                        ///< Deep S3 Enable in AC Mode
#define R_PCH_PWRM_S4_PWRGATE_POL                           0x2C                        ///< Deep S4 Power Policies
#define B_PCH_PWRM_S4DC_GATE_SUS                            BIT1                        ///< Deep S4 Enable in DC Mode
#define B_PCH_PWRM_S4AC_GATE_SUS                            BIT0                        ///< Deep S4 Enable in AC Mode
#define R_PCH_PWRM_S5_PWRGATE_POL                           0x30                        ///< Deep S5 Power Policies
#define B_PCH_PWRM_S5DC_GATE_SUS                            BIT15                       ///< Deep S5 Enable in DC Mode
#define B_PCH_PWRM_S5AC_GATE_SUS                            BIT14                       ///< Deep S5 Enable in AC Mode
#define R_PCH_PWRM_DSX_CFG                                  0x34                        ///< Deep SX Configuration
#define B_PCH_PWRM_DSX_CFG_WAKE_PIN_DSX_EN                  BIT2                        ///< WAKE# Pin DeepSx Enable
#define B_PCH_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS                BIT1                        ///< AC_PRESENT pin pulldown in DeepSx disable
#define B_PCH_PWRM_DSX_CFG_LAN_WAKE_EN                      BIT0                        ///< LAN_WAKE Pin DeepSx Enable
#define R_PCH_PWRM_CFG2                                     0x3C                        ///< Power Management Configuration Reg 2
#define B_PCH_PWRM_CFG2_PBOP                                (BIT31 | BIT30 | BIT29)     ///< Power Button Override Period (PBOP)
#define N_PCH_PWRM_CFG2_PBOP                                29                          ///< Power Button Override Period (PBOP)
#define B_PCH_PWRM_CFG2_PB_DIS                              BIT28                       ///< Power Button Native Mode Disable (PB_DIS)
#define B_PCH_PWRM_CFG2_DRAM_RESET_CTL                      BIT26                       ///< DRAM RESET# control
#define R_PCH_PWRM_EN_SN_SLOW_RING                          0x48                        ///< Enable Snoop Request to SLOW_RING
#define R_PCH_PWRM_EN_SN_SLOW_RING2                         0x4C                        ///< Enable Snoop Request to SLOW_RING 2nd Reg
#define R_PCH_PWRM_EN_SN_SA                                 0x50                        ///< Enable Snoop Request to SA
#define R_PCH_PWRM_EN_SN_SA2                                0x54                        ///< Enable Snoop Request to SA 2nd Reg
#define R_PCH_PWRM_EN_SN_SLOW_RING_CF                       0x58                        ///< Enable Snoop Request to SLOW_RING_CF
#define R_PCH_PWRM_EN_NS_SA                                 0x68                        ///< Enable Non-Snoop Request to SA
#define R_PCH_PWRM_EN_CW_SLOW_RING                          0x80                        ///< Enable Clock Wake to SLOW_RING
#define R_PCH_PWRM_EN_CW_SLOW_RING2                         0x84                        ///< Enable Clock Wake to SLOW_RING 2nd Reg
#define R_PCH_PWRM_EN_CW_SA                                 0x88                        ///< Enable Clock Wake to SA
#define R_PCH_PWRM_EN_CW_SA2                                0x8C                        ///< Enable Clock Wake to SA 2nd Reg
#define R_PCH_PWRM_EN_CW_SLOW_RING_CF                       0x98                        ///< Enable Clock Wake to SLOW_RING_CF
#define R_PCH_PWRM_EN_PA_SLOW_RING                          0xA8                        ///< Enable Pegged Active to SLOW_RING
#define R_PCH_PWRM_EN_PA_SLOW_RING2                         0xAC                        ///< Enable Pegged Active to SLOW_RING 2nd Reg
#define R_PCH_PWRM_EN_PA_SA                                 0xB0                        ///< Enable Pegged Active to SA
#define R_PCH_PWRM_EN_PA_SA2                                0xB4                        ///< Enable Pegged Active to SA 2nd Reg
#define R_PCH_PWRM_EN_MISC_EVENT                            0xC0                        ///< Enable Misc PM_SYNC Events
#define R_PCH_PWRM_PMSYNC_TPR_CONFIG                        0xC4
#define B_PCH_PWRM_PMSYNC_TPR_CONFIG_LOCK                   BIT31
#define B_PCH_PWRM_PMSYNC_PCH2CPU_TT_EN                     BIT26
#define B_PCH_PWRM_PMSYNC_PCH2CPU_TT_STATE                  (BIT25 | BIT24)
#define N_PCH_PWRM_PMSYNC_PCH2CPU_TT_STATE                  24
#define V_PCH_PWRM_PMSYNC_PCH2CPU_TT_STATE_1                1
#define R_PCH_PWRM_PMSYNC_MISC_CFG                          0xC8
#define B_PCH_PWRM_PMSYNC_PM_SYNC_LOCK                      BIT15                       ///< PM_SYNC Configuration Lock
#define B_PCH_PWRM_PMSYNC_GPIO_D_SEL                        BIT11
#define B_PCH_PWRM_PMSYNC_GPIO_C_SEL                        BIT10
#define B_PCH_PWRM_PMSYNC_GPIO_B_SEL                        BIT9
#define B_PCH_PWRM_PMSYNC_GPIO_A_SEL                        BIT8
#define R_PCH_PWRM_PMSYNC_TPR_CONFIG2                       0xCC

#define R_PCH_PWRM_PM_SYNC_MODE                             0xD4
#define B_PCH_PWRM_PM_SYNC_MODE_GPIO_B                      BIT13
#define B_PCH_PWRM_PM_SYNC_MODE_GPIO_C                      BIT14
#define PM_SYNC_GPIO_B                                      0
#define PM_SYNC_GPIO_C                                      1
#define B_PCH_PWRM_PM_SYNC_MODE_32_64_WR                    BIT29
#define R_PCH_PWRM_PM_SYNC_MODE_C0                          0xF4
#define R_PCH_PWRM_PM_SYNC_STATE_HYS                        0xD0                        ///< PM_SYNC State Hysteresis
#define R_PCH_PWRM_PM_SYNC_MODE                             0xD4                        ///< PM_SYNC Pin Mode
#define R_PCH_PWRM_CFG3                                     0xE0             ///< Power Management Configuration Reg 3
#define B_PCH_PWRM_CFG3_DSX_WLAN_PP_EN                      BIT16            ///< Deep-Sx WLAN Phy Power Enable
#define B_PCH_PWRM_CFG3_HOST_WLAN_PP_EN                     BIT17            ///< Host Wireless LAN Phy Power Enable
#define B_PCH_PWRM_CFG3_PWRG_LOCK                           BIT2                        ///< Lock power gating override messages
#define B_PCH_PWRM_CFG3_GR_PFET_DUR_ON_DEF                  (BIT25 | BIT24)
#define N_PCH_PWRM_CFG3_GR_PFET_DUR_ON_DEF                  24
#define V_PCH_PWRM_CFG3_GR_PFET_DUR_ON_DEF_1US              0
#define V_PCH_PWRM_CFG3_GR_PFET_DUR_ON_DEF_2US              1
#define V_PCH_PWRM_CFG3_GR_PFET_DUR_ON_DEF_5US              2
#define V_PCH_PWRM_CFG3_GR_PFET_DUR_ON_DEF_20US             3
#define R_PCH_PWRM_PM_DOWN_PPB_CFG                          0xE4                        ///< PM_DOWN PCH_POWER_BUDGET CONFIGURATION
#define R_PCH_PWRM_CFG4                                     0xE8             ///< Power Management Configuration Reg 4
#define B_PCH_PWRM_CFG4_U2_PHY_PG_EN                        BIT30            ///< USB2 PHY SUS Well Power Gating Enable
#define B_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR                   (0x000001FF)     ///< CPU I/O VR Ramp Duration, [8:0]
#define N_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR                   0
#define V_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR_70US              0x007
#define V_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR_240US             0x018
#define R_PCH_PWRM_CPU_EPOC                                 0xEC
#define R_PCH_PWRM_ADR_EN                                   0xF0
#define B_PCH_PWRM_ADR_EN_ADR_FEAT_EN                       BIT0
#define B_PCH_PWRM_ADR_EN_CTWDT_ADR_EN                      BIT9
#define B_PCH_PWRM_ADR_EN_SYSPWR_ADR_EN                     BIT12
#define B_PCH_PWRM_ADR_EN_OCWDT_ADR_EN                      BIT13
#define B_PCH_PWRM_ADR_EN_PMC_PARERR_ADR_EN                 BIT14
#define B_PCH_PWRM_ADR_EN_HPR_ADR_EN                        BIT28
#define B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_MASK                 BIT31|BIT30
#define B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_GPIO_B               BIT30
#define B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_GPIO_C               BIT31
#define R_PCH_PWRM_GBL2HOST_EN                              0x10C    ///< Global to Host Reset Enable
#define V_PCH_PWRM_GBL2HOST_EN                              0x001F2DE4///< Global to Host Reset Enable default value
#define B_PCH_PWRM_GBL2HOST_EN_G2H_FEAT_EN                  BIT0     ///< G2H Feature Enable
#define B_PCH_PWRM_GBL2HOST_EN_LTRESET_G2H_EN               BIT1     ///< LT RESET G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_PMCGBL_G2H_EN                BIT2     ///< PMC FW-Initiated Global Reset G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_CPUTHRM_G2H_EN               BIT3     ///< CPU Thermal Trip G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_PCHTHRM_G2H_EN               BIT4     ///< PCH Internal Thermal Trip G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_PBO_G2H_EN                   BIT5     ///< Power Button Override G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_MEPBO_G2H_EN                 BIT6     ///< ME-Initiated Power Button Override G2H
#define B_PCH_PWRM_GBL2HOST_EN_MEWDT_G2H_EN                 BIT7     ///< ME FW Watchdog Timer G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_MEGBL_G2H_EN                 BIT8     ///< ME-Initiated Global Reset G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_CTWDT_G2H_EN                 BIT9     ///< CPU Thermal Watchdog Timer G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_PMCWDT_G2H_EN                BIT10    ///< PMC FW Watchdog Timer G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_ME_UERR_G2H_EN               BIT11    ///< ME Uncorrectable Error G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_SYSPWR_G2H_EN                BIT12    ///< SYS_PWROK Failure G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_OCWDT_G2H_EN                 BIT13    ///< Over-Clocking WDT G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_PMC_PARERR_G2H_EN            BIT14    ///< PMC Parity Error G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_IEPBO_G2H_EN                 BIT16    ///< IE-Initiated Power Button Override G2H
#define B_PCH_PWRM_GBL2HOST_EN_IEWDT_G2H_EN                 BIT17    ///< IE FW Watchdog Timer G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_IEGBL_G2H_EN                 BIT18    ///< IE-Initiated Global Reset G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_IE_UERR_G2H_EN               BIT19    ///< IE Uncorrectable Error G2H Enable
#define B_PCH_PWRM_GBL2HOST_EN_ACRU_ERR_G2H_EN              BIT20    ///< AC RU Error G2H Enable
#define R_PCH_PWRM_VR_MISC_CTL                              0x100
#define B_PCH_PWRM_VR_MISC_CTL_VIDSOVEN                     BIT3
#define R_PCH_PWRM_GPIO_CFG                                 0x120
#define B_PCH_PWRM_GPIO_CFG_GPE0_DW2                        (BIT11 | BIT10 | BIT9 | BIT8)
#define N_PCH_PWRM_GPIO_CFG_GPE0_DW2                        8
#define B_PCH_PWRM_GPIO_CFG_GPE0_DW1                        (BIT7 | BIT6 | BIT5 | BIT4)
#define N_PCH_PWRM_GPIO_CFG_GPE0_DW1                        4
#define B_PCH_PWRM_GPIO_CFG_GPE0_DW0                        (BIT3 | BIT2 | BIT1 | BIT0)
#define N_PCH_PWRM_GPIO_CFG_GPE0_DW0                        0
#define R_PCH_PWRM_PM_SYNC_MODE_C0                          0xF4                        ///< PM_SYNC Pin Mode in C0
#define R_PCH_PWRM_GBLRST_CAUSE0                            0x124    //Global reset cause 0
#define R_PCH_PWRM_GBLRST_CAUSE1                            0x128    //Global reset cause 1
#define R_PCH_PWRM_HPR_CAUSE0                               0x12C    ///< Host partition reset causes
#define B_PCH_PWRM_HPR_CAUSE0_GBL_TO_HOST                   BIT15    ///< Global reset converted to Host reset
#define R_PCH_PWRM_MODPHY_PM_CFG1                           0x200
#define B_PCH_PWRM_MODPHY_PM_CFG1_MLSXSWPGP                 0xFFFF
#define R_PCH_PWRM_MODPHY_PM_CFG2                           0x204    ///< ModPHY Power Management Configuration Reg 2
#define B_PCH_PWRM_MODPHY_PM_CFG2_MLSPDDGE                  BIT30    ///< ModPHY Lane SUS Power Domain Dynamic Gating Enable
#define B_PCH_PWRM_MODPHY_PM_CFG2_EMFC                      BIT29    ///< Enable ModPHY FET Control
#define B_PCH_PWRM_MODPHY_PM_CFG2_EFRT                      (BIT28 | BIT27 | BIT26 | BIT25 | BIT24)    ///< External FET Ramp Time
#define N_PCH_PWRM_MODPHY_PM_CFG2_EFRT                      24
#define V_PCH_PWRM_MODPHY_PM_CFG2_EFRT_200US                0x0A
#define B_PCH_PWRM_MODPHY_PM_CFG2_ASLOR_UFS                 BIT16    ///< UFS ModPHY SPD SPD Override
#define R_PCH_PWRM_MODPHY_PM_CFG3                           0x208    ///< ModPHY Power Management Configuration Reg 3
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_UFS             BIT16    ///< UFS ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_XDCI            BIT15    ///< xDCI ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_XHCI            BIT14    ///< xHCI ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_GBE             BIT13    ///< GbE ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_SATA            BIT12    ///< SATA ModPHY SPD RT Request
#define R_PCH_PWRM_30C                                      0x30C
#define R_PCH_PWRM_OBFF_CFG                                 0x314             ///< OBFF Configuration
#define R_PCH_PWRM_31C                                      0x31C
#define R_PCH_PWRM_CPPM_MISC_CFG                            0x320             ///< CPPM Miscellaneous Configuration
#define R_PCH_PWRM_CPPM_CG_POL1A                            0x324             ///< CPPM Clock Gating Policy Reg 1
#define R_PCH_PWRM_CPPM_CG_POL2A                            0x340             ///< CPPM Clock Gating Policy Reg 3
#define R_PCH_PWRM_34C                                      0x34C
#define R_PCH_PWRM_CPPM_CG_POL3A                            0x3A8             ///< CPPM Clock Gating Policy Reg 5
#define B_PCH_PWRM_CPPM_CG_POLXA_CPPM_GX_QUAL               BIT30             ///< CPPM Shutdown Qualifier Enable for Clock Source Group X
#define B_PCH_PWRM_CPPM_CG_POLXA_LTR_GX_THRESH              (0x000001FF)      ///< LTR Threshold for Clock Source Group X, [8:0]
#define R_PCH_PWRM_3D0                                      0x3D0
#define R_PCH_PWRM_CPPM_MPG_POL1A                           0x3E0             ///< CPPM ModPHY Gating Policy Reg 1A
#define B_PCH_PWRM_CPPM_MPG_POL1A_CPPM_MODPHY_QUAL          BIT30             ///< CPPM Shutdown Qualifier Enable for ModPHY
#define B_PCH_PWRM_CPPM_MPG_POL1A_LT_MODPHY_SEL             BIT29             ///< ASLT/PLT Selection for ModPHY
#define B_PCH_PWRM_CPPM_MPG_POL1A_LTR_MODPHY_THRESH         (0x000001FF)      ///< LTR Threshold for ModPHY, [8:0]
#define R_PCH_PWRM_CS_SD_CTL1                               0x3E8             ///< Clock Source Shutdown Control Reg 1
#define B_PCH_PWRM_CS_SD_CTL1_CS5_CTL_CFG                   (BIT22 | BIT21 | BIT20)    ///< Clock Source 5 Control Configuration
#define N_PCH_PWRM_CS_SD_CTL1_CS5_CTL_CFG                   20
#define B_PCH_PWRM_CS_SD_CTL1_CS1_CTL_CFG                   (BIT2 | BIT1 | BIT0)       ///< Clock Source 1 Control Configuration
#define N_PCH_PWRM_CS_SD_CTL1_CS1_CTL_CFG                   0
#define R_PCH_PWRM_CS_SD_CTL2                               0x3EC             ///< Clock Source Shutdown Control Reg 2
#define R_PCH_PWRM_HSWPGCR1                                 0x5D0
#define B_PCH_PWRM_SW_PG_CTRL_LOCK                          BIT31
#define B_PCH_PWRM_DFX_SW_PG_CTRL                           BIT0
#define R_PCH_PWRM_600                                      0x600
#define R_PCH_PWRM_604                                      0x604
#define R_PMC_PWRM_ST_PG_FDIS_PMC_1                         0x620 ///< Static PG Related Function Disable Register 1
#define B_PMC_PWRM_ST_PG_FDIS_PMC_1_ST_FDIS_LK              BIT31 ///< Static Function Disable Lock (ST_FDIS_LK)
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_CAM_FDIS_PMC            BIT6  ///< Camera Function Disable (PMC Version) (CAM_FDIS_PMC)
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_ISH_FDIS_PMC            BIT5  ///< SH Function Disable (PMC Version) (ISH_FDIS_PMC)
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC            BIT0  ///< GBE Function Disable (PMC Version) (GBE_FDIS_PMC)
#define R_PCH_PWRM_ST_PG_FDIS_PMC_2                         0x624 ///< Static Function Disable Control Register 2
#define V_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_FDIS_PMC       0x7FF ///< Static Function Disable Control Register 2
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI1_FDIS_PMC BIT10 ///< SerialIo Controller GSPI Device 1 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI0_FDIS_PMC BIT9  ///< SerialIo Controller GSPI Device 0 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART2_FDIS_PMC BIT8  ///< SerialIo Controller UART Device 2 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART1_FDIS_PMC BIT7  ///< SerialIo Controller UART Device 1 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART0_FDIS_PMC BIT6  ///< SerialIo Controller UART Device 0 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C5_FDIS_PMC  BIT5  ///< SerialIo Controller I2C Device 5 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C4_FDIS_PMC  BIT4  ///< SerialIo Controller I2C Device 4 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C3_FDIS_PMC  BIT3  ///< SerialIo Controller I2C Device 3 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C2_FDIS_PMC  BIT2  ///< SerialIo Controller I2C Device 2 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C1_FDIS_PMC  BIT1  ///< SerialIo Controller I2C Device 1 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C0_FDIS_PMC  BIT0  ///< SerialIo Controller I2C Device 0 Function Disable
#define R_PCH_PWRM_NST_PG_FDIS_1                            0x628
#define B_PCH_PWRM_NST_PG_FDIS_1_SCC_FDIS_PMC               BIT25 ///< SCC Function Disable. This is only avaiable in B0 onward.
#define B_PCH_PWRM_NST_PG_FDIS_1_XDCI_FDIS_PMC              BIT24 ///< XDCI Function Disable. This is only avaiable in B0 onward.
#define B_PCH_PWRM_NST_PG_FDIS_1_ADSP_FDIS_PMC              BIT23 ///< ADSP Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_SATA_FDIS_PMC              BIT22 ///< SATA Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_sSATA_FDIS_PMC             BIT27 ///< sSATA Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C3_FDIS_PMC           BIT13 ///< PCIe Controller C Port 3 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C2_FDIS_PMC           BIT12 ///< PCIe Controller C Port 2 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C1_FDIS_PMC           BIT11 ///< PCIe Controller C Port 1 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C0_FDIS_PMC           BIT10 ///< PCIe Controller C Port 0 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B3_FDIS_PMC           BIT9  ///< PCIe Controller B Port 3 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B2_FDIS_PMC           BIT8  ///< PCIe Controller B Port 2 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B1_FDIS_PMC           BIT7  ///< PCIe Controller B Port 1 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B0_FDIS_PMC           BIT6  ///< PCIe Controller B Port 0 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A3_FDIS_PMC           BIT5  ///< PCIe Controller A Port 3 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A2_FDIS_PMC           BIT4  ///< PCIe Controller A Port 2 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A1_FDIS_PMC           BIT3  ///< PCIe Controller A Port 1 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A0_FDIS_PMC           BIT2  ///< PCIe Controller A Port 0 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_XHCI_FDIS_PMC              BIT0  ///< XHCI Function Disable
#define R_PCH_PWRM_FUSE_DIS_RD_1                            0x640 ///< Fuse Disable Read 1 Register
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E3_FUSE_DIS           BIT21 ///< PCIe Controller E Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E2_FUSE_DIS           BIT20 ///< PCIe Controller E Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E1_FUSE_DIS           BIT19 ///< PCIe Controller E Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E0_FUSE_DIS           BIT18 ///< PCIe Controller E Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D3_FUSE_DIS           BIT17 ///< PCIe Controller D Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D2_FUSE_DIS           BIT16 ///< PCIe Controller D Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D1_FUSE_DIS           BIT15 ///< PCIe Controller D Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D0_FUSE_DIS           BIT14 ///< PCIe Controller D Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C3_FUSE_DIS           BIT13 ///< PCIe Controller C Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C2_FUSE_DIS           BIT12 ///< PCIe Controller C Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C1_FUSE_DIS           BIT11 ///< PCIe Controller C Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C0_FUSE_DIS           BIT10 ///< PCIe Controller C Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B3_FUSE_DIS           BIT9  ///< PCIe Controller B Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B2_FUSE_DIS           BIT8  ///< PCIe Controller B Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B1_FUSE_DIS           BIT7  ///< PCIe Controller B Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B0_FUSE_DIS           BIT6  ///< PCIe Controller B Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A3_FUSE_DIS           BIT5  ///< PCIe Controller A Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A2_FUSE_DIS           BIT4  ///< PCIe Controller A Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A1_FUSE_DIS           BIT3  ///< PCIe Controller A Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A0_FUSE_DIS           BIT2  ///< PCIe Controller A Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_XHCI_FUSE_DIS              BIT0  ///< XHCI Fuse Disable
#define R_PCH_PWRM_FUSE_DIS_RD_2                            0x644 ///< Fuse Disable Read 2 Register
#define B_PCH_PWRM_FUSE_DIS_RD_2_SPC_SS_DIS                 BIT25 ///< SPC Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SPB_SS_DIS                 BIT24 ///< SPB Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SPA_SS_DIS                 BIT23 ///< SPA Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ESPISPI_FUSE_SS_DIS        BIT22 ///< ESPISPI Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_PSTH_FUSE_SS_DIS           BIT21 ///< PSTH Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_DMI_FUSE_SS_DIS            BIT20 ///< DMI Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_OTG_FUSE_SS_DIS            BIT19 ///< OTG Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_XHCI_SS_DIS                BIT18 ///< XHCI Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_FIA_FUSE_SS_DIS            BIT17 ///< FIA Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_DSP_FUSE_SS_DIS            BIT16 ///< DSP Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SATA_FUSE_SS_DIS           BIT15 ///< SATA Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ICC_FUSE_SS_DIS            BIT14 ///< ICC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_LPC_FUSE_SS_DIS            BIT13 ///< LPC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_RTC_FUSE_SS_DIS            BIT12 ///< RTC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_P2S_FUSE_SS_DIS            BIT11 ///< P2S Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_TRSB_FUSE_SS_DIS           BIT10 ///< TRSB Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SMB_FUSE_SS_DIS            BIT9  ///< SMB Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ITSS_FUSE_SS_DIS           BIT8  ///< ITSS Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_TRACEHUB_FUSE_SS_DIS       BIT7  ///< TraceHub Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SERIALIO_FUSE_SS_DIS       BIT6  ///< SerialIo Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_FLIS_FUSE_SS_DIS           BIT5  ///< FLIS Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SCC_FUSE_SS_DIS            BIT4  ///< SCC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_P2D_FUSE_SS_DIS            BIT3  ///< P2D Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_CAM_FUSE_SS_DIS            BIT2  ///< Camera Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ISH_FUSE_SS_DIS            BIT1  ///< ISH Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_GBE_FUSE_SS_DIS            BIT0  ///< GBE Fuse or Soft Strap Disable
#define R_PCH_PWRM_FUSE_DIS_RD_3                            0x648 ///< Static PG Fuse and Soft Strap Disable Read Register 3
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA3_FUSE_SS_DIS         BIT3  ///< PNCRA3 Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA2_FUSE_SS_DIS         BIT2  ///< PNCRA2 Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA1_FUSE_SS_DIS         BIT1  ///< PNCRA1 Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA_FUSE_SS_DIS          BIT0  ///< PNCRA Fuse or Soft Strap Disable


#endif
