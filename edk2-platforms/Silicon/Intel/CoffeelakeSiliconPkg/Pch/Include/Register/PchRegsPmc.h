/** @file
  Register names for PCH PMC device

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

#ifndef _PCH_REGS_PMC_H_
#define _PCH_REGS_PMC_H_

//
// PMC Registers (D31:F2)
//
#define PCI_DEVICE_NUMBER_PCH_PMC                31
#define PCI_FUNCTION_NUMBER_PCH_PMC              2

#define R_PMC_CFG_BASE                           0x10
#define B_PMC_CFG_PWRM_BASE_MASK                 0xFFFF0000                    ///< PWRM must be 64KB alignment to align the source decode.

//
// ACPI and legacy I/O register offsets from ACPIBASE
//
#define R_ACPI_IO_PM1_STS                        0x00
#define S_ACPI_IO_PM1_STS                        2
#define B_ACPI_IO_PM1_STS_WAK                    BIT15
#define B_ACPI_IO_PM1_STS_PCIEXP_WAKE_STS        BIT14
#define B_ACPI_IO_PM1_STS_PRBTNOR                BIT11
#define B_ACPI_IO_PM1_STS_RTC                    BIT10
#define B_ACPI_IO_PM1_STS_PWRBTN                 BIT8
#define B_ACPI_IO_PM1_STS_GBL                    BIT5
#define B_ACPI_IO_PM1_STS_BM                     BIT4
#define B_ACPI_IO_PM1_STS_TMROF                  BIT0
#define N_ACPI_IO_PM1_STS_WAK                    15
#define N_ACPI_IO_PM1_STS_PCIEXP_WAKE_STS        14
#define N_ACPI_IO_PM1_STS_PRBTNOR                11
#define N_ACPI_IO_PM1_STS_RTC                    10
#define N_ACPI_IO_PM1_STS_PWRBTN                 8
#define N_ACPI_IO_PM1_STS_GBL                    5
#define N_ACPI_IO_PM1_STS_BM                     4
#define N_ACPI_IO_PM1_STS_TMROF                  0

#define R_ACPI_IO_PM1_EN                         0x02
#define S_ACPI_IO_PM1_EN                         2
#define B_ACPI_IO_PM1_EN_RTC                     BIT10
#define B_ACPI_IO_PM1_EN_PWRBTN                  BIT8
#define B_ACPI_IO_PM1_EN_GBL                     BIT5
#define B_ACPI_IO_PM1_EN_TMROF                   BIT0
#define N_ACPI_IO_PM1_EN_RTC                     10
#define N_ACPI_IO_PM1_EN_PWRBTN                  8
#define N_ACPI_IO_PM1_EN_GBL                     5
#define N_ACPI_IO_PM1_EN_TMROF                   0

#define R_ACPI_IO_PM1_CNT                        0x04
#define S_ACPI_IO_PM1_CNT                        4
#define B_ACPI_IO_PM1_CNT_SLP_EN                 BIT13
#define B_ACPI_IO_PM1_CNT_SLP_TYP                (BIT12 | BIT11 | BIT10)
#define V_ACPI_IO_PM1_CNT_S0                     0
#define V_ACPI_IO_PM1_CNT_S1                     BIT10
#define V_ACPI_IO_PM1_CNT_S3                     (BIT12 | BIT10)
#define V_ACPI_IO_PM1_CNT_S4                     (BIT12 | BIT11)
#define V_ACPI_IO_PM1_CNT_S5                     (BIT12 | BIT11 | BIT10)
#define B_ACPI_IO_PM1_CNT_GBL_RLS                BIT2
#define B_ACPI_IO_PM1_CNT_BM_RLD                 BIT1
#define B_ACPI_IO_PM1_CNT_SCI_EN                 BIT0

#define R_ACPI_IO_PM1_TMR                        0x08
#define V_ACPI_IO_PM1_TMR_FREQUENCY              3579545
#define B_ACPI_IO_PM1_TMR_VAL                    0xFFFFFF
#define V_ACPI_IO_PM1_TMR_MAX_VAL                0x1000000       ///< The timer is 24 bit overflow

#define R_ACPI_IO_SMI_EN                              0x30
#define S_ACPI_IO_SMI_EN                              4
#define B_ACPI_IO_SMI_EN_LEGACY_USB3                  BIT31
#define B_ACPI_IO_SMI_EN_GPIO_UNLOCK_SMI              BIT27
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

#define R_ACPI_IO_GPE_CNTL                            0x40
#define B_ACPI_IO_GPE_CNTL_SWGPE_CTRL                 BIT17

#define R_ACPI_IO_DEVACT_STS                          0x44
#define S_ACPI_IO_DEVACT_STS                          2
#define B_ACPI_IO_DEVACT_STS_MASK                     0x13E1
#define B_ACPI_IO_DEVACT_STS_KBC                      BIT12
#define B_ACPI_IO_DEVACT_STS_PIRQDH                   BIT9
#define B_ACPI_IO_DEVACT_STS_PIRQCG                   BIT8
#define B_ACPI_IO_DEVACT_STS_PIRQBF                   BIT7
#define B_ACPI_IO_DEVACT_STS_PIRQAE                   BIT6
#define B_ACPI_IO_DEVACT_STS_D0_TRP                   BIT0
#define N_ACPI_IO_DEVACT_STS_KBC                      12
#define N_ACPI_IO_DEVACT_STS_PIRQDH                   9
#define N_ACPI_IO_DEVACT_STS_PIRQCG                   8
#define N_ACPI_IO_DEVACT_STS_PIRQBF                   7
#define N_ACPI_IO_DEVACT_STS_PIRQAE                   6

#define R_ACPI_IO_PM2_CNT                             0x50
#define B_ACPI_IO_PM2_CNT_ARB_DIS                     BIT0

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

#define R_ACPI_IO_GPE0_STS_31_0                    0x60
#define R_ACPI_IO_GPE0_STS_63_32                   0x64
#define R_ACPI_IO_GPE0_STS_95_64                   0x68
#define R_ACPI_IO_GPE0_STS_127_96                  0x6C
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
#define N_ACPI_IO_GPE0_STS_127_96_PME_B0           13
#define N_ACPI_IO_GPE0_STS_127_96_PME              11
#define N_ACPI_IO_GPE0_STS_127_96_BATLOW           10
#define N_ACPI_IO_GPE0_STS_127_96_PCI_EXP          9
#define N_ACPI_IO_GPE0_STS_127_96_RI               8
#define N_ACPI_IO_GPE0_STS_127_96_SMB_WAK          7
#define N_ACPI_IO_GPE0_STS_127_96_TC0SCI           6
#define N_ACPI_IO_GPE0_STS_127_96_SWGPE            2
#define N_ACPI_IO_GPE0_STS_127_96_HOT_PLUG         1

#define R_ACPI_IO_GPE0_EN_31_0                     0x70
#define R_ACPI_IO_GPE0_EN_63_32                    0x74
#define R_ACPI_IO_GPE0_EN_95_64                    0x78
#define R_ACPI_IO_GPE0_EN_127_96                   0x7C
#define S_ACPI_IO_GPE0_EN_127_96                   4
#define B_ACPI_IO_GPE0_EN_127_96_WADT              BIT18
#define B_ACPI_IO_GPE0_EN_127_96_USB_CON_DSX_EN    BIT17
#define B_ACPI_IO_GPE0_EN_127_96_LAN_WAKE          BIT16
#define B_ACPI_IO_GPE0_EN_127_96_PME_B0            BIT13
#define B_ACPI_IO_GPE0_EN_127_96_ME_SCI            BIT12
#define B_ACPI_IO_GPE0_EN_127_96_PME               BIT11
#define B_ACPI_IO_GPE0_EN_127_96_BATLOW            BIT10
#define B_ACPI_IO_GPE0_EN_127_96_PCI_EXP           BIT9
#define B_ACPI_IO_GPE0_EN_127_96_RI                BIT8
#define B_ACPI_IO_GPE0_EN_127_96_TC0SCI            BIT6
#define B_ACPI_IO_GPE0_EN_127_96_SWGPE             BIT2
#define B_ACPI_IO_GPE0_EN_127_96_HOT_PLUG          BIT1
#define N_ACPI_IO_GPE0_EN_127_96_PME_B0            13
#define N_ACPI_IO_GPE0_EN_127_96_USB3              12
#define N_ACPI_IO_GPE0_EN_127_96_PME               11
#define N_ACPI_IO_GPE0_EN_127_96_BATLOW            10
#define N_ACPI_IO_GPE0_EN_127_96_PCI_EXP           9
#define N_ACPI_IO_GPE0_EN_127_96_RI                8
#define N_ACPI_IO_GPE0_EN_127_96_TC0SCI            6
#define N_ACPI_IO_GPE0_EN_127_96_SWGPE             2
#define N_ACPI_IO_GPE0_EN_127_96_HOT_PLUG          1


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
#define N_TCO_IO_TCO1_STS_DMISMI                     10
#define N_TCO_IO_TCO1_STS_BIOSWR                     8
#define N_TCO_IO_TCO1_STS_NEWCENTURY                 7
#define N_TCO_IO_TCO1_STS_TIMEOUT                    3
#define N_TCO_IO_TCO1_STS_SW_TCO_SMI                 1

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
#define B_TCO_IO_TCO1_CNT_NR_MSUS                    BIT0  //NO_REBOOT

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
// PWRM Registers for IPC interface
//
#define R_PMC_PWRM_IPC_CMD                                  0x00                        ///< IPC command
#define N_PMC_PWRM_IPC_CMD_CMD_ID                           12                          ///< IPC command.cmd.ID
#define N_PMC_PWRM_IPC_CMD_SIZE                             16                          ///< IPC command.size
#define B_PMC_PWRM_IPC_CMD_SIZE_MASK                        0x00FF0000                  ///< IPC command.size mask Bits[23:16]
#define N_PMC_PWRM_IPC_CMD_COMMAND                          0                           ///< IPC command.cmd.Command
#define B_PMC_PWRM_IPC_CMD_COMMAND_MASK                     0x000000FF                  ///< IPC command.size mask Bits[07:00]
#define V_PMC_PWRM_IPC_CMD_COMMAND_SLP_CTRL                 0xA1                        ///< IPC commmand to control S0ix policies RCOMP
#define V_PMC_PWRM_IPC_CMD_COMMAND_NPK_STATE                0xA4                        ///< IPC commmand to control NPK Power State
#define V_PMC_PWRM_IPC_CMD_COMMAND_PROXY                    0xAA                        ///< Proxy access to SOC registers
#define V_PMC_PWRM_IPC_CMD_CMD_ID_PROXY_READ                   0                        ///< Read command
#define V_PMC_PWRM_IPC_CMD_CMD_ID_PROXY_WRITE                  1                        ///< Write command
#define V_PMC_PWRM_IPC_CMD_WBUF0_PROXY_NMI                     2                        ///< parameter to access NMI control register
#define R_PMC_PWRM_IPC_STS                                  0x04                        ///< IPC Status
#define B_PMC_PWRM_IPC_STS_BUSY                             BIT0                        ///< IPC Status Busy Bit
#define B_PMC_PWRM_IPC_STS_ERROR                            BIT1                        ///< IPC Status Error Bit
#define N_PMC_PWRM_IPC_STS_ERR_CODE                         BIT16                       ///< IPC Status Error status
#define B_PMC_PWRM_IPC_STS_ERR_CODE_MASK                    0x00FF0000                  ///< IPC Status Error status mask[23:16]
#define R_PMC_PWRM_IPC_SPTR                                 0x08                        ///< IPC Source Pointer
#define R_PMC_PWRM_IPC_DPTR                                 0x0C                        ///< IPC Destination Pointer
#define R_PMC_PWRM_IPC_WBUF0                                0x80                        ///< IPC Write Buffer
#define R_PMC_PWRM_IPC_WBUF1                                0x84                        ///< IPC Write Buffer
#define R_PMC_PWRM_IPC_WBUF2                                0x88                        ///< IPC Write Buffer
#define R_PMC_PWRM_IPC_WBUF3                                0x8C                        ///< IPC Write Buffer
#define R_PMC_PWRM_IPC_RBUF0                                0x90                        ///< IPC Read Buffer
#define R_PMC_PWRM_IPC_RBUF1                                0x94                        ///< IPC Read Buffer
#define R_PMC_PWRM_IPC_RBUF2                                0x98                        ///< IPC Read Buffer
#define R_PMC_PWRM_IPC_RBUF3                                0x9C                        ///< IPC Read Buffer
//
// PWRM Registers
//
#define R_PMC_PWRM_GEN_PMCON_A                              0x1020
#define B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS                    BIT30
#define B_PMC_PWRM_GEN_PMCON_A_DSX_PP_DIS                   BIT29
#define B_PMC_PWRM_GEN_PMCON_A_AG3_PP_EN                    BIT28
#define B_PMC_PWRM_GEN_PMCON_A_SX_PP_EN                     BIT27
#define B_PMC_PWRM_GEN_PMCON_A_ALLOW_ICLK_PLL               BIT26
#define B_PMC_PWRM_GEN_PMCON_A_MPHY_CRICLK_GATE_OVR         BIT25
#define B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS                  BIT24
#define B_PMC_PWRM_GEN_PMCON_A_DISB                         BIT23
#define B_PMC_PWRM_GEN_PMCON_A_ALLOW_OPI_PLL_SD_INC0        BIT22
#define B_PMC_PWRM_GEN_PMCON_A_MEM_SR                       BIT21
#define B_PMC_PWRM_GEN_PMCON_A_ALLOW_SPXB_CG_INC0           BIT20
#define B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_C0               BIT19
#define B_PMC_PWRM_GEN_PMCON_A_MS4V                         BIT18
#define B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_OPI_ON           BIT17
#define B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR                  BIT16
#define B_PMC_PWRM_GEN_PMCON_A_PME_B0_S5_DIS                BIT15
#define B_PMC_PWRM_GEN_PMCON_A_PWR_FLR                      BIT14
#define B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_BCLKREQ_ON       BIT13
#define B_PMC_PWRM_GEN_PMCON_A_DISABLE_SX_STRETCH           BIT12
#define B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS                 BIT9
#define B_PMC_PWRM_GEN_PMCON_A_ESPI_SMI_LOCK                BIT8
#define B_PMC_PWRM_GEN_PMCON_A_SLP_S4_ASE                   BIT3
#define B_PMC_PWRM_GEN_PMCON_A_AFTERG3_EN                   BIT0
#define B_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL                   0xC0
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_64MS              0xC0
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_32MS              0x80
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_16MS              0x40
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_1_5MS             0x00
#define B_PMC_PWRM_GEN_PMCON_A_PER_SMI_SEL                  0x6
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_64S                  0x0000
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_32S                  0x0002
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_16S                  0x0004
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_8S                   0x0006

#define R_PMC_PWRM_GEN_PMCON_B                              0x1024
#define B_PMC_PWRM_GEN_PMCON_B_SLPSX_STR_POL_LOCK           BIT18            ///< Lock down SLP_S3/SLP_S4 Minimum Assertion width
#define B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD                  BIT13
#define B_PMC_PWRM_GEN_PMCON_B_BIOS_PCI_EXP_EN              BIT10
#define B_PMC_PWRM_GEN_PMCON_B_PWRBTN_LVL                   BIT9
#define B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK                     BIT4
#define B_PMC_PWRM_GEN_PMCON_B_RTC_PWR_STS                  BIT2

#define R_PMC_PWRM_CRID                                     0x1030           ///< Configured Revision ID
#define B_PMC_PWRM_CRID_RID_SEL                             (BIT0 | BIT1)    ///< RID Select
#define V_PMC_PWRM_CRID_RID_SEL_REVISIONID                  0
#define V_PMC_PWRM_CRID_RID_SEL_CRID0                       1
#define B_PMC_PWRM_CRID_CRID_LK                             BIT31            ///< CRID Lock

#define R_PMC_PWRM_ETR3                                     0x1048          ///< this is PWRM register
#define B_PMC_PWRM_ETR3_CF9LOCK                             BIT31           ///< CF9h Lockdown
#define B_PMC_PWRM_ETR3_LATCH_EVENTS_C10_EXIT               BIT30
#define B_PMC_PWRM_ETR3_USB_CACHE_DIS                       BIT21
#define B_PMC_PWRM_ETR3_CF9GR                               BIT20           ///< CF9h Global Reset
#define B_PMC_PWRM_ETR3_SKIP_HOST_RST_HS                    BIT19
#define B_PMC_PWRM_ETR3_CWORWRE                             BIT18
#define B_PMC_PWRM_THROT_1_VR_ALERT                         BIT0

#define R_PMC_PWRM_SSML                                     0x104C           ///< Set Strap Msg Lock
#define B_PMC_PWRM_SSML_SSL                                 BIT0             ///< Set_Strap Lock
#define R_PMC_PWRM_SSMC                                     0x1050           ///< Set Strap Msg Control
#define B_PMC_PWRM_SSMC_SSMS                                BIT0             ///< Set_Strap Mux Select
#define R_PMC_PWRM_SSMD                                     0x1054           ///< Set Strap Msg Data

#define R_PMC_PWRM_MODPHY_PM_CFG5                           0x10D0
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_UFS2            BIT26
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_DMI             BIT25
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_E3              BIT24
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_E2              BIT23
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_E1              BIT22
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_E0              BIT21
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_D3              BIT20
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_D2              BIT19
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_D1              BIT18
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_D0              BIT17
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_UFS             BIT16    ///< UFS ModPHY SPD RT Request
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_XDCI            BIT15    ///< xDCI ModPHY SPD RT Request
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_XHCI            BIT14    ///< xHCI ModPHY SPD RT Request
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_GBE             BIT13    ///< GbE ModPHY SPD RT Request
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_SATA            BIT12    ///< SATA ModPHY SPD RT Request
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_C3              BIT11
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_C2              BIT10
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_C1              BIT9
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_C0              BIT8
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_B3              BIT7
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_B2              BIT6
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_B1              BIT5
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_B0              BIT4
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_A3              BIT3
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_A2              BIT2
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_A1              BIT1
#define B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_A0              BIT0
#define R_PMC_PWRM_WADT_AC                                  0x1800
#define R_PMC_PWRM_PRSTS                                    0x1810                      ///< Power and Reset Status
#define B_PMC_PWRM_PRSTS_VE_WD_TMR_STS                      BIT7                        ///< VE Watchdog Timer Status
#define B_PMC_PWRM_PRSTS_WOL_OVR_WK_STS                     BIT5
#define B_PMC_PWRM_PRSTS_FIELD_1                            BIT4
#define B_PMC_PWRM_PRSTS_ME_WAKE_STS                        BIT0

#define R_PMC_PWRM_1814                                     0x1814
#define R_PMC_PWRM_CFG                                      0x1818                      ///< Power Management Configuration
#define B_PMC_PWRM_CFG_ALLOW_24_OSC_SD                      BIT29                       ///< Allow 24MHz Crystal Oscillator Shutdown
#define B_PMC_PWRM_CFG_DBG_MODE_LOCK                        BIT27                       ///< Debug Mode Lock
#define B_PMC_PWRM_CFG_ALLOW_USB2_CORE_PG                   BIT25                       ///< Allow USB2 Core Power Gating
#define B_PMC_PWRM_CFG_ER_LOCK                              BIT24                       ///< Energy Reporting Lock
#define B_PMC_PWRM_CFG_EN_PMC_UNC_ERR                       BIT23                       ///< Enable Global Reset on Uncorrectable Parity Error on PMC SRAM Interface
#define B_PMC_PWRM_CFG_PMCREAD_DISABLE                      BIT22                       ///< Disable Reads to PMC
#define B_PMC_PWRM_CFG_RTC_DS_WAKE_DIS                      BIT21                       ///< RTC Wake from Deep S4/S5 Disable
#define B_PMC_PWRM_CFG_SSMAW_MASK                           (BIT19 | BIT18)             ///< SLP_SUS# Min Assertion Width
#define V_PMC_PWRM_CFG_SSMAW_4S                             (BIT19 | BIT18)             ///< 4 seconds
#define V_PMC_PWRM_CFG_SSMAW_1S                             BIT19                       ///< 1 second
#define V_PMC_PWRM_CFG_SSMAW_0_5S                           BIT18                       ///< 0.5 second (500ms)
#define V_PMC_PWRM_CFG_SSMAW_0S                             0                           ///< 0 second
#define B_PMC_PWRM_CFG_SAMAW_MASK                           (BIT17 | BIT16)             ///< SLP_A# Min Assertion Width
#define V_PMC_PWRM_CFG_SAMAW_2S                             (BIT17 | BIT16)             ///< 2 seconds
#define V_PMC_PWRM_CFG_SAMAW_98ms                           BIT17                       ///< 98ms
#define V_PMC_PWRM_CFG_SAMAW_4S                             BIT16                       ///< 4 seconds
#define V_PMC_PWRM_CFG_SAMAW_0S                             0                           ///< 0 second
#define B_PMC_PWRM_CFG_RPCD_MASK                            (BIT9 | BIT8)               ///< Reset Power Cycle Duration
#define V_PMC_PWRM_CFG_RPCD_1S                              (BIT9 | BIT8)               ///< 1-2 seconds
#define V_PMC_PWRM_CFG_RPCD_2S                              BIT9                        ///< 2-3 seconds
#define V_PMC_PWRM_CFG_RPCD_3S                              BIT8                        ///< 3-4 seconds
#define V_PMC_PWRM_CFG_RPCD_4S                              0                           ///< 4-5 seconds (Default)
#define B_PMC_PWRM_CFG_COCS                                 BIT5                        ///< CPU OC Strap
#define B_PMC_PWRM_CFG_ER_EN                                BIT2                        ///< Energy Reporting Enable
#define B_PMC_PWRM_CFG_TIMING_TPCH25                        (BIT1 | BIT0)               ///< tPCH25 timing

#define R_PMC_PWRM_S3_PWRGATE_POL                           0x1828                      ///< S3 Power Gating Policies
#define B_PMC_PWRM_S3_PWRGATE_POL_S3DC_GATE_SUS             BIT1                        ///< Deep S3 Enable in DC Mode
#define B_PMC_PWRM_S3_PWRGATE_POL_S3AC_GATE_SUS             BIT0                        ///< Deep S3 Enable in AC Mode

#define R_PMC_PWRM_S4_PWRGATE_POL                           0x182C                      ///< Deep S4 Power Policies
#define B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS             BIT1                        ///< Deep S4 Enable in DC Mode
#define B_PMC_PWRM_S4_PWRGATE_POL_S4AC_GATE_SUS             BIT0                        ///< Deep S4 Enable in AC Mode

#define R_PMC_PWRM_S5_PWRGATE_POL                           0x1830                      ///< Deep S5 Power Policies
#define B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS             BIT15                       ///< Deep S5 Enable in DC Mode
#define B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS             BIT14                       ///< Deep S5 Enable in AC Mode

#define R_PMC_PWRM_DSX_CFG                                  0x1834                      ///< Deep SX Configuration
#define B_PMC_PWRM_DSX_CFG_WAKE_PIN_DSX_EN                  BIT2                        ///< WAKE# Pin DeepSx Enable
#define B_PMC_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS                BIT1                        ///< AC_PRESENT pin pulldown in DeepSx disable
#define B_PMC_PWRM_DSX_CFG_LAN_WAKE_EN                      BIT0                        ///< LAN_WAKE Pin DeepSx Enable

#define R_PMC_PWRM_CFG2                                     0x183C                      ///< Power Management Configuration Reg 2
#define B_PMC_PWRM_CFG2_PBOP                                (BIT31 | BIT30 | BIT29)     ///< Power Button Override Period (PBOP)
#define N_PMC_PWRM_CFG2_PBOP                                29                          ///< Power Button Override Period (PBOP)
#define B_PMC_PWRM_CFG2_PB_DIS                              BIT28                       ///< Power Button Native Mode Disable (PB_DIS)
#define B_PMC_PWRM_CFG2_EN_DBG_MSG                          BIT27                       ///< Enable PMC Debug Messages
#define B_PMC_PWRM_CFG2_DRAM_RESET_CTL                      BIT26                       ///< DRAM RESET# control
#define N_PMC_PWRM_CFG2_DRAM_RESET_CTL                      26

#define R_PMC_PWRM_EN_SN_SLOW_RING                          0x1848                      ///< Enable Snoop Request to SLOW_RING
#define R_PMC_PWRM_EN_SN_SLOW_RING2                         0x184C                      ///< Enable Snoop Request to SLOW_RING 2nd Reg
#define R_PMC_PWRM_EN_SN_SA                                 0x1850                      ///< Enable Snoop Request to SA
#define R_PMC_PWRM_EN_SN_SA2                                0x1854                      ///< Enable Snoop Request to SA 2nd Reg
#define R_PMC_PWRM_EN_SN_SLOW_RING_CF                       0x1858                      ///< Enable Snoop Request to SLOW_RING_CF
#define R_PMC_PWRM_EN_NS_SA                                 0x1868                      ///< Enable Non-Snoop Request to SA
#define R_PMC_PWRM_EN_CW_SLOW_RING                          0x1880                      ///< Enable Clock Wake to SLOW_RING
#define R_PMC_PWRM_EN_CW_SLOW_RING2                         0x1884                      ///< Enable Clock Wake to SLOW_RING 2nd Reg
#define R_PMC_PWRM_EN_CW_SA                                 0x1888                      ///< Enable Clock Wake to SA
#define R_PMC_PWRM_EN_CW_SA2                                0x188C                      ///< Enable Clock Wake to SA 2nd Reg
#define R_PMC_PWRM_EN_CW_SLOW_RING_CF                       0x1898                      ///< Enable Clock Wake to SLOW_RING_CF
#define R_PMC_PWRM_EN_PA_SLOW_RING                          0x18A8                      ///< Enable Pegged Active to SLOW_RING
#define R_PMC_PWRM_EN_PA_SLOW_RING2                         0x18AC                      ///< Enable Pegged Active to SLOW_RING 2nd Reg
#define R_PMC_PWRM_EN_PA_SA                                 0x18B0                      ///< Enable Pegged Active to SA
#define R_PMC_PWRM_EN_PA_SA2                                0x18B4                      ///< Enable Pegged Active to SA 2nd Reg
#define R_PMC_PWRM_EN_MISC_EVENT                            0x18C0                      ///< Enable Misc PM_SYNC Events
#define R_PMC_PWRM_PMSYNC_TPR_CONFIG                        0x18C4
#define B_PMC_PWRM_PMSYNC_TPR_CONFIG_LOCK                   BIT31
#define B_PMC_PWRM_PMSYNC_PCH2CPU_TT_EN                     BIT26
#define B_PMC_PWRM_PMSYNC_PCH2CPU_TT_STATE                  (BIT25 | BIT24)
#define N_PMC_PWRM_PMSYNC_PCH2CPU_TT_STATE                  24
#define V_PMC_PWRM_PMSYNC_PCH2CPU_TT_STATE_1                1
#define B_PMC_PWRM_PMSYNC_PM_SYNC_LOCK                      BIT15                       ///< PM_SYNC Configuration Lock
#define B_PMC_PWRM_PMSYNC_GPIO_D_SEL                        BIT11
#define B_PMC_PWRM_PMSYNC_GPIO_C_SEL                        BIT10

#define R_PMC_PWRM_PM_SYNC_STATE_HYS                        0x18D0                      ///< PM_SYNC State Hysteresis
#define R_PMC_PWRM_PM_SYNC_MODE                             0x18D4                      ///< PM_SYNC Pin Mode

#define R_PMC_PWRM_CFG3                                     0x18E0                      ///< Power Management Configuration Reg 3
#define B_PMC_PWRM_CFG3_HOST_WLAN_PP_EN                     BIT17                       ///< Host Wireless LAN Phy Power Enable
#define B_PMC_PWRM_CFG3_DSX_WLAN_PP_EN                      BIT16                       ///< Deep-Sx WLAN Phy Power Enable

#define R_PMC_PWRM_PM_DOWN_PPB_CFG                          0x18E4                      ///< PM_DOWN PCH_POWER_BUDGET CONFIGURATION

#define R_PMC_PWRM_CFG4                                     0x18E8                      ///< Power Management Configuration Reg 4
#define B_PMC_PWRM_CFG4_U2_PHY_PG_EN                        BIT30                       ///< USB2 PHY SUS Well Power Gating Enable
#define B_PMC_PWRM_CFG4_CPU_IOVR_RAMP_DUR                   (0x000001FF)                ///< CPU I/O VR Ramp Duration, [8:0]
#define N_PMC_PWRM_CFG4_CPU_IOVR_RAMP_DUR                   0
#define V_PMC_PWRM_CFG4_CPU_IOVR_RAMP_DUR_70US              0x007
#define V_PMC_PWRM_CFG4_CPU_IOVR_RAMP_DUR_240US             0x018

#define R_PMC_PWRM_CPU_EPOC                                 0x18EC

#define R_PMC_PWRM_GPIO_CFG                                 0x1920
#define B_PMC_PWRM_GPIO_CFG_GPE0_DW2                        (BIT11 | BIT10 | BIT9 | BIT8)
#define N_PMC_PWRM_GPIO_CFG_GPE0_DW2                        8
#define B_PMC_PWRM_GPIO_CFG_GPE0_DW1                        (BIT7 | BIT6 | BIT5 | BIT4)
#define N_PMC_PWRM_GPIO_CFG_GPE0_DW1                        4
#define B_PMC_PWRM_GPIO_CFG_GPE0_DW0                        (BIT3 | BIT2 | BIT1 | BIT0)
#define N_PMC_PWRM_GPIO_CFG_GPE0_DW0                        0


#define R_PMC_PWRM_CS_SD_CTL1                               0x1BE8            ///< Clock Source Shutdown Control Reg 1
#define B_PMC_PWRM_CS_SD_CTL1_CS5_CTL_CFG                   (BIT22 | BIT21 | BIT20)    ///< Clock Source 5 Control Configuration
#define N_PMC_PWRM_CS_SD_CTL1_CS5_CTL_CFG                   20
#define B_PMC_PWRM_CS_SD_CTL1_CS1_CTL_CFG                   (BIT2 | BIT1 | BIT0)       ///< Clock Source 1 Control Configuration
#define N_PMC_PWRM_CS_SD_CTL1_CS1_CTL_CFG                   0

#define R_PMC_PWRM_CS_SD_CTL2                               0x1BEC ///< Clock Source Shutdown Control Reg 2

#define R_PMC_PWRM_HSWPGCR1                                 0x1DD0
#define B_PMC_PWRM_SW_PG_CTRL_LOCK                          BIT31
#define B_PMC_PWRM_NPK_VNN_SW_PG_CTRL                       BIT0

#define R_PMC_PWRM_1E00                                     0x1E00
#define R_PMC_PWRM_1E04                                     0x1E04

#define R_PMC_PWRM_ST_PG_FDIS_PMC_1                         0x1E20 ///< Static PG Related Function Disable Register 1
#define B_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI_FDIS_PMC           BIT1   ///< CNVi Function Disable (PMC Version) (CNVI_FDIS_PMC)
#define B_PMC_PWRM_ST_PG_FDIS_PMC_1_ST_FDIS_LK              BIT31 ///< Static Function Disable Lock (ST_FDIS_LK)
#define B_PMC_PWRM_ST_PG_FDIS_PMC_1_CAM_FDIS_PMC            BIT6  ///< Camera Function Disable (PMC Version) (CAM_FDIS_PMC)
#define B_PMC_PWRM_ST_PG_FDIS_PMC_1_ISH_FDIS_PMC            BIT5  ///< SH Function Disable (PMC Version) (ISH_FDIS_PMC)
#define B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC            BIT0  ///< GBE Function Disable (PMC Version) (GBE_FDIS_PMC)

#define R_PMC_PWRM_ST_PG_FDIS_PMC_2                         0x1E24 ///< Static Function Disable Control Register 2
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI2_FDIS_PMC BIT11 ///< SerialIo Controller GSPI Device 2 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI1_FDIS_PMC BIT10 ///< SerialIo Controller GSPI Device 1 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI0_FDIS_PMC BIT9  ///< SerialIo Controller GSPI Device 0 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART2_FDIS_PMC BIT8  ///< SerialIo Controller UART Device 2 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART1_FDIS_PMC BIT7  ///< SerialIo Controller UART Device 1 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART0_FDIS_PMC BIT6  ///< SerialIo Controller UART Device 0 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C5_FDIS_PMC  BIT5  ///< SerialIo Controller I2C Device 5 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C4_FDIS_PMC  BIT4  ///< SerialIo Controller I2C Device 4 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C3_FDIS_PMC  BIT3  ///< SerialIo Controller I2C Device 3 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C2_FDIS_PMC  BIT2  ///< SerialIo Controller I2C Device 2 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C1_FDIS_PMC  BIT1  ///< SerialIo Controller I2C Device 1 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C0_FDIS_PMC  BIT0  ///< SerialIo Controller I2C Device 0 Function Disable
#define B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO                0xFFF  ///< SerialIo Devices Disable Mask

#define R_PMC_PWRM_NST_PG_FDIS_1                            0x1E28
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_F3_FDIS_PMC     BIT31  ///< PCIe Controller F Port 3 Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_F2_FDIS_PMC     BIT30  ///< PCIe Controller F Port 2 Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_F1_FDIS_PMC     BIT29  ///< PCIe Controller F Port 1 Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_F0_FDIS_PMC     BIT28  ///< PCIe Controller F Port 0 Function Disable
#define B_PCH_LP_PMC_PWRM_NST_PG_FDIS_1_SDCARD_FDIS_PMC     BIT29  ///< SD Card Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_SDCARD_FDIS_PMC      BIT27  ///< SD Card Function Disable
#define B_PCH_LP_PMC_PWRM_NST_PG_FDIS_1_EMMC_FDIS_PMC       BIT28  ///< eMMC Function Disable
#define B_PCH_LP_PMC_PWRM_NST_PG_FDIS_1_UFS_FDIS_PMC        BIT27  ///< UFS Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_XDCI_FDIS_PMC              BIT26  ///< XDCI Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_SMBUS_FDIS_PMC             BIT25  ///< Smbus Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_ADSP_FDIS_PMC              BIT23  ///< ADSP Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_SATA_FDIS_PMC              BIT22  ///< SATA Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_E3_FDIS_PMC     BIT21  ///< PCIe Controller E Port 3 Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_E2_FDIS_PMC     BIT20  ///< PCIe Controller E Port 2 Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_E1_FDIS_PMC     BIT19  ///< PCIe Controller E Port 1 Function Disable
#define B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_E0_FDIS_PMC     BIT18  ///< PCIe Controller E Port 0 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_D3_FDIS_PMC           BIT17  ///< PCIe Controller D Port 3 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_D2_FDIS_PMC           BIT16  ///< PCIe Controller D Port 2 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_D1_FDIS_PMC           BIT15  ///< PCIe Controller D Port 1 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_D0_FDIS_PMC           BIT14  ///< PCIe Controller D Port 0 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_C3_FDIS_PMC           BIT13  ///< PCIe Controller C Port 3 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_C2_FDIS_PMC           BIT12  ///< PCIe Controller C Port 2 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_C1_FDIS_PMC           BIT11  ///< PCIe Controller C Port 1 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_C0_FDIS_PMC           BIT10  ///< PCIe Controller C Port 0 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_B3_FDIS_PMC           BIT9   ///< PCIe Controller B Port 3 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_B2_FDIS_PMC           BIT8   ///< PCIe Controller B Port 2 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_B1_FDIS_PMC           BIT7   ///< PCIe Controller B Port 1 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_B0_FDIS_PMC           BIT6   ///< PCIe Controller B Port 0 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_A3_FDIS_PMC           BIT5   ///< PCIe Controller A Port 3 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_A2_FDIS_PMC           BIT4   ///< PCIe Controller A Port 2 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_A1_FDIS_PMC           BIT3   ///< PCIe Controller A Port 1 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_PCIE_A0_FDIS_PMC           BIT2   ///< PCIe Controller A Port 0 Function Disable
#define B_PMC_PWRM_NST_PG_FDIS_1_XHCI_FDIS_PMC              BIT0   ///< XHCI Function Disable

#define R_PMC_PWRM_FUSE_DIS_RD_2                            0x1E44 ///< Fuse Disable Read 2 Register
#define B_PMC_PWRM_FUSE_DIS_RD_2_SPC_SS_DIS                 BIT25 ///< SPC Fuse Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_SPB_SS_DIS                 BIT24 ///< SPB Fuse Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_SPA_SS_DIS                 BIT23 ///< SPA Fuse Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_PSTH_FUSE_SS_DIS           BIT21 ///< PSTH Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_DMI_FUSE_SS_DIS            BIT20 ///< DMI Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_OTG_FUSE_SS_DIS            BIT19 ///< OTG Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_XHCI_SS_DIS                BIT18 ///< XHCI Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_FIA_FUSE_SS_DIS            BIT17 ///< FIA Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_DSP_FUSE_SS_DIS            BIT16 ///< DSP Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_SATA_FUSE_SS_DIS           BIT15 ///< SATA Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_ICC_FUSE_SS_DIS            BIT14 ///< ICC Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_LPC_FUSE_SS_DIS            BIT13 ///< LPC Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_RTC_FUSE_SS_DIS            BIT12 ///< RTC Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_P2S_FUSE_SS_DIS            BIT11 ///< P2S Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_TRSB_FUSE_SS_DIS           BIT10 ///< TRSB Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_SMB_FUSE_SS_DIS            BIT9  ///< SMB Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_ITSS_FUSE_SS_DIS           BIT8  ///< ITSS Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_UFSX2_FUSE_SS_DIS          BIT7   ///< UFSX2 Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_SERIALIO_FUSE_SS_DIS       BIT6  ///< SerialIo Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_EMMC_FUSE_SS_DIS           BIT5   ///< EMMC Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_CNVI_FUSE_SS_DIS           BIT4   ///< CNVi Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_P2D_FUSE_SS_DIS            BIT3  ///< P2D Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_SDX_FUSE_SS_DIS            BIT2   ///< SD Conroller Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_ISH_FUSE_SS_DIS            BIT1  ///< ISH Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_2_GBE_FUSE_SS_DIS            BIT0  ///< GBE Fuse or Soft Strap Disable

#define R_PMC_PWRM_FUSE_DIS_RD_3                            0x1E48 ///< Static PG Fuse and Soft Strap Disable Read Register 3
#define B_PMC_PWRM_FUSE_DIS_RD_3_PNCRA3_FUSE_SS_DIS         BIT3  ///< PNCRA3 Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_3_PNCRA2_FUSE_SS_DIS         BIT2  ///< PNCRA2 Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_3_PNCRA1_FUSE_SS_DIS         BIT1  ///< PNCRA1 Fuse or Soft Strap Disable
#define B_PMC_PWRM_FUSE_DIS_RD_3_PNCRA_FUSE_SS_DIS          BIT0  ///< PNCRA Fuse or Soft Strap Disable

#endif
