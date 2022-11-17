/** @file
  Register names for PCH SATA controllers

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

#ifndef _PCH_REGS_SATA_H_
#define _PCH_REGS_SATA_H_

//
//  SATA Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SATA          23
#define PCI_FUNCTION_NUMBER_PCH_SATA        0

#define PCI_DEVICE_NUMBER_CDF_PCH_SATA_1    7
#define PCI_FUNCTION_NUMBER_CDF_PCH_SATA_1  0

#define PCI_DEVICE_NUMBER_CDF_PCH_SATA_2    8
#define PCI_FUNCTION_NUMBER_CDF_PCH_SATA_2  0

#define PCI_DEVICE_NUMBER_CDF_PCH_SATA_3    14
#define PCI_FUNCTION_NUMBER_CDF_PCH_SATA_3  0

//
//  PCH-LP SATA Device ID's
//
#define V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_AHCI         0x9DD3  ///< SATA Controller (AHCI) - Mobile
#define V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_RAID         0x9DD5  ///< SATA Controller (RAID 0/1/5/10) - NOT premium - Mobile
#define V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_RAID_PREM    0x9DD7  ///< SATA Controller (RAID 0/1/5/10) - premium - Mobile
#define V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_RAID_IBC     0x282A  ///< SATA Controller (RAID 0/1/5/10) - In-box compatible - Mobile

//
//  PCH-H SATA Device ID's
//
#define V_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_ALTDIS   0x2822  ///< SATA Controller (RAID 0/1/5/10) - premium - Alternate ID
#define V_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_RSTE     0x2826  ///< SATA Controller (RAID 0/1/5/10) - RSTe of Server SKU

//
//  PCH-H SATA Device ID's
//
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_AHCI         0xA352  ///< SATA Controller (AHCI) Desktop/Server
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_MH_AHCI        0xA353  ///< SATA Controller (AHCI) Mobile Halo
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID         0xA354  ///< SATA Controller (RAID 0/1/5/10) - NOT premium Desktop/Server
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_MH_RAID        0xA355  ///< SATA Controller (RAID 0/1/5/10) - NOT premium Mobile Halo
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_PREM    0xA356  ///< SATA Controller (RAID 0/1/5/10) - premium Desktop/Server
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_MH_RAID_PREM   0xA357  ///< SATA Controller (RAID 0/1/5/10) - premium Mobile Halo
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_OP_AHCI      0xA35E  ///< SATA Controller (AHCI) Optane Caching - Desktop
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_IBC     0x2822  ///< SATA Controller (RAID 0/1/5/10) - In-box compatible
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_IBC_RST 0x2826  ///< SATA Controller (RAID 0/1/5/10) - In-box compatible (RSTe)
#define V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_IBC_2   0x282A  ///< SATA Controller (RAID 0/1/5/10) - In-box compatible (Alternate)


//
//  SATA Controller common Registers
//
#define V_SATA_CFG_SUB_CLASS_CODE_AHCI      0x06
#define V_SATA_CFG_SUB_CLASS_CODE_RAID      0x04
#define R_SATA_CFG_AHCI_BAR                 0x24
#define B_SATA_CFG_AHCI_BAR_BA              0xFFFFF800
#define V_SATA_CFG_AHCI_BAR_LENGTH          0x800
#define N_SATA_CFG_AHCI_BAR_ALIGNMENT       11
#define V_SATA_CFG_AHCI_BAR_LENGTH_512K     0x80000
#define N_SATA_CFG_AHCI_BAR_ALIGNMENT_512K  19
#define B_SATA_CFG_AHCI_BAR_PF              BIT3
#define B_SATA_CFG_AHCI_BAR_TP              (BIT2 | BIT1)
#define B_SATA_CFG_AHCI_BAR_RTE             BIT0
#define R_SATA_CFG_PID                      0x70
#define B_SATA_CFG_PID_NEXT                 0xFF00
#define V_SATA_CFG_PID_NEXT_0               0xB000
#define V_SATA_CFG_PID_NEXT_1               0xA800
#define B_SATA_CFG_PID_CID                  0x00FF
#define R_SATA_CFG_PC                       0x72
#define S_SATA_CFG_PC                       2
#define B_SATA_CFG_PC_PME                   (BIT15 | BIT14 | BIT13 | BIT12 | BIT11)
#define V_SATA_CFG_PC_PME_0                 0x0000
#define V_SATA_CFG_PC_PME_1                 0x4000
#define B_SATA_CFG_PC_D2_SUP                BIT10
#define B_SATA_CFG_PC_D1_SUP                BIT9
#define B_SATA_CFG_PC_AUX_CUR               (BIT8 | BIT7 | BIT6)
#define B_SATA_CFG_PC_DSI                   BIT5
#define B_SATA_CFG_PC_PME_CLK               BIT3
#define B_SATA_CFG_PC_VER                   (BIT2 | BIT1 | BIT0)
#define R_SATA_CFG_PMCS                     0x74
#define B_SATA_CFG_PMCS_PMES                BIT15
#define B_SATA_CFG_PMCS_PMEE                BIT8
#define B_SATA_CFG_PMCS_NSFRST              BIT3
#define V_SATA_CFG_PMCS_NSFRST_1            0x01
#define V_SATA_CFG_PMCS_NSFRST_0            0x00
#define B_SATA_CFG_PMCS_PS                  (BIT1 | BIT0)
#define V_SATA_CFG_PMCS_PS_3                0x03
#define V_SATA_CFG_PMCS_PS_0                0x00
#define R_SATA_CFG_MID                      0x80
#define B_SATA_CFG_MID_NEXT                 0xFF00
#define B_SATA_CFG_MID_CID                  0x00FF
#define R_SATA_CFG_MC                       0x82
#define B_SATA_CFG_MC_C64                   BIT7
#define B_SATA_CFG_MC_MME                   (BIT6 | BIT5 | BIT4)
#define V_SATA_CFG_MC_MME_4                 0x04
#define V_SATA_CFG_MC_MME_2                 0x02
#define V_SATA_CFG_MC_MME_1                 0x01
#define V_SATA_CFG_MC_MME_0                 0x00
#define B_SATA_CFG_MC_MMC                   (BIT3 | BIT2 | BIT1)
#define V_SATA_CFG_MC_MMC_4                 0x04
#define V_SATA_CFG_MC_MMC_0                 0x00
#define B_SATA_CFG_MC_MSIE                  BIT0
#define V_SATA_CFG_MC_MSIE_1                0x01
#define V_SATA_CFG_MC_MSIE_0                0x00
#define R_SATA_CFG_MA                       0x84
#define B_SATA_CFG_MA                       0xFFFFFFFC
#define R_SATA_CFG_MD                       0x88
#define B_SATA_CFG_MD_MSIMD                 0xFFFF

#define R_SATA_CFG_MAP                      0x90
#define B_SATA_CFG_MAP_PCD                  0xFF
#define N_SATA_CFG_MAP_SPD                  16
#define B_SATA_CFG_MAP_SPD7                 BIT23
#define B_SATA_CFG_MAP_SPD6                 BIT22
#define B_SATA_CFG_MAP_SPD5                 BIT21
#define B_SATA_CFG_MAP_SPD4                 BIT20
#define B_SATA_CFG_MAP_SPD3                 BIT19
#define B_SATA_CFG_MAP_SPD2                 BIT18
#define B_SATA_CFG_MAP_SPD1                 BIT17
#define B_SATA_CFG_MAP_SPD0                 BIT16
#define B_SATA_CFG_MAP_PORT7_PCD            BIT7
#define B_SATA_CFG_MAP_PORT6_PCD            BIT6
#define B_SATA_CFG_MAP_PORT5_PCD            BIT5
#define B_SATA_CFG_MAP_PORT4_PCD            BIT4
#define B_SATA_CFG_MAP_PORT3_PCD            BIT3
#define B_SATA_CFG_MAP_PORT2_PCD            BIT2
#define B_SATA_CFG_MAP_PORT1_PCD            BIT1
#define B_SATA_CFG_MAP_PORT0_PCD            BIT0
#define R_SATA_CFG_PCS                      0x94
#define B_SATA_CFG_PCS_P7P                  BIT23
#define B_SATA_CFG_PCS_P6P                  BIT22
#define B_SATA_CFG_PCS_P5P                  BIT21
#define B_SATA_CFG_PCS_P4P                  BIT20
#define B_SATA_CFG_PCS_P3P                  BIT19
#define B_SATA_CFG_PCS_P2P                  BIT18
#define B_SATA_CFG_PCS_P1P                  BIT17
#define B_SATA_CFG_PCS_P0P                  BIT16
#define B_SATA_CFG_PCS_P7E                  BIT7
#define B_SATA_CFG_PCS_P6E                  BIT6
#define B_SATA_CFG_PCS_P5E                  BIT5
#define B_SATA_CFG_PCS_P4E                  BIT4
#define B_SATA_CFG_PCS_P3E                  BIT3
#define B_SATA_CFG_PCS_P2E                  BIT2
#define B_SATA_CFG_PCS_P1E                  BIT1
#define B_SATA_CFG_PCS_P0E                  BIT0
#define R_SATA_CFG_SATAGC                   0x9C
#define B_SATA_CFG_SATAGC_SMS_MASK          BIT16
#define N_SATA_CFG_SATAGC_SMS_MASK          16
#define V_SATA_CFG_SATAGC_SMS_AHCI          0x0
#define V_SATA_CFG_SATAGC_SMS_RAID          0x1
#define B_SATA_CFG_SATAGC_AIE               BIT7
#define B_SATA_CFG_SATAGC_AIES              BIT6
#define B_SATA_CFG_SATAGC_MSS               (BIT4 | BIT3)
#define V_SATA_CFG_SATAGC_MSS_8K            0x2
#define N_SATA_CFG_SATAGC_MSS               3
#define B_SATA_CFG_SATAGC_ASSEL             (BIT2 | BIT1 | BIT0)

#define V_SATA_CFG_SATAGC_ASSEL_2K          0x0
#define V_SATA_CFG_SATAGC_ASSEL_16K         0x1
#define V_SATA_CFG_SATAGC_ASSEL_32K         0x2
#define V_SATA_CFG_SATAGC_ASSEL_64K         0x3
#define V_SATA_CFG_SATAGC_ASSEL_128K        0x4
#define V_SATA_CFG_SATAGC_ASSEL_256K        0x5
#define V_SATA_CFG_SATAGC_ASSEL_512K        0x6

#define R_SATA_CFG_SIRI                     0xA0
#define R_SATA_CFG_STRD                     0xA4
#define R_SATA_CFG_SIR_0C                   0x0C
#define R_SATA_CFG_SIR_50                   0x50
#define R_SATA_CFG_SIR_54                   0x54
#define R_SATA_CFG_SIR_58                   0x58
#define R_SATA_CFG_SIR_5C                   0x5C
#define R_SATA_CFG_SIR_60                   0x60
#define R_SATA_CFG_SIR_64                   0x64
#define R_SATA_CFG_SIR_68                   0x68
#define R_SATA_CFG_SIR_6C                   0x6C
#define R_SATA_CFG_SIR_70                   0x70
#define R_SATA_CFG_SIR_80                   0x80
#define R_SATA_CFG_SIR_84                   0x84
#define R_SATA_CFG_SIR_8C                   0x8C
#define R_SATA_CFG_SIR_90                   0x90
#define R_SATA_CFG_SIR_98                   0x98
#define R_SATA_CFG_SIR_9C                   0x9C
#define R_SATA_CFG_SIR_A0                   0xA0
#define R_SATA_CFG_SIR_A4                   0xA4
#define R_SATA_CFG_SIR_A8                   0xA8
#define R_SATA_CFG_SIR_C8                   0xC8
#define R_SATA_CFG_SIR_CC                   0xCC
#define R_SATA_CFG_SIR_D0                   0xD0
#define R_SATA_CFG_SIR_D4                   0xD4
#define B_SATA_CFG_STRD_DTA                 0xFFFFFFFF
#define R_SATA_CFG_CR0                      0xA8
#define B_SATA_CFG_CR0_MAJREV               0x00F00000
#define B_SATA_CFG_CR0_MINREV               0x000F0000
#define B_SATA_CFG_CR0_NEXT                 0x0000FF00
#define B_SATA_CFG_CR0_CAP                  0x000000FF
#define R_SATA_CFG_CR1                      0xAC
#define B_SATA_CFG_CR1_BAROFST              0xFFF0
#define B_SATA_CFG_CR1_BARLOC               0x000F
#define R_SATA_CFG_FLR_CID                  0xB0
#define B_SATA_CFG_FLR_CID_NEXT             0xFF00
#define B_SATA_CFG_FLR_CID                  0x00FF
#define V_SATA_CFG_FLR_CID_1                0x0009
#define V_SATA_CFG_FLR_CID_0                0x0013
#define R_SATA_CFG_FLR_CLV                  0xB2
#define B_SATA_CFG_FLR_CLV_FLRC_FLRCSSEL_0  BIT9
#define B_SATA_CFG_FLR_CLV_TXPC_FLRCSSEL_0  BIT8
#define B_SATA_CFG_FLR_CLV_VSCID_FLRCSSEL_0 0x00FF
#define B_SATA_CFG_FLR_CLV_VSCID_FLRCSSEL_1 0x00FF
#define V_SATA_CFG_FLR_CLV_VSCID_FLRCSSEL   0x0006
#define R_SATA_CFG_FLRC                     0xB4
#define B_SATA_CFG_FLRC_TXP                 BIT8
#define B_SATA_CFG_FLRC_INITFLR             BIT0
#define R_SATA_CFG_SP                       0xC0
#define B_SATA_CFG_SP                       0xFFFFFFFF
#define R_SATA_CFG_MXID                     0xD0
#define N_SATA_CFG_MXID_NEXT                8

#define R_SATA_CFG_BFCS                     0xE0
#define B_SATA_CFG_BFCS_P7BFI               BIT17
#define B_SATA_CFG_BFCS_P6BFI               BIT16
#define B_SATA_CFG_BFCS_P5BFI               BIT15
#define B_SATA_CFG_BFCS_P4BFI               BIT14
#define B_SATA_CFG_BFCS_P3BFI               BIT13
#define B_SATA_CFG_BFCS_P2BFI               BIT12
#define B_SATA_CFG_BFCS_P2BFS               BIT11
#define B_SATA_CFG_BFCS_P2BFF               BIT10
#define B_SATA_CFG_BFCS_P1BFI               BIT9
#define B_SATA_CFG_BFCS_P0BFI               BIT8
#define B_SATA_CFG_BFCS_BIST_FIS_T          BIT7
#define B_SATA_CFG_BFCS_BIST_FIS_A          BIT6
#define B_SATA_CFG_BFCS_BIST_FIS_S          BIT5
#define B_SATA_CFG_BFCS_BIST_FIS_L          BIT4
#define B_SATA_CFG_BFCS_BIST_FIS_F          BIT3
#define B_SATA_CFG_BFCS_BIST_FIS_P          BIT2
#define R_SATA_CFG_BFTD1                    0xE4
#define B_SATA_CFG_BFTD1                    0xFFFFFFFF
#define R_SATA_CFG_BFTD2                    0xE8
#define B_SATA_CFG_BFTD2                    0xFFFFFFFF

#define R_SATA_CFG_VS_CAP                   0xA4
#define B_SATA_CFG_VS_CAP_NRMBE             BIT0                            ///< NVM Remap Memory BAR Enable
#define B_SATA_CFG_VS_CAP_MSL               0x1FFE                          ///< Memory Space Limit
#define N_SATA_CFG_VS_CAP_MSL               1
#define V_SATA_CFG_VS_CAP_MSL               0x1EF                           ///< Memory Space Limit Field Value
#define B_SATA_CFG_VS_CAP_NRMO              0xFFF0000                       ///< NVM Remapped Memory Offset
#define N_SATA_CFG_VS_CAP_NRMO              16
#define V_SATA_CFG_VS_CAP_NRMO              0x10                            ///< NVM Remapped Memory Offset Field Value

//
// RST PCIe Storage Remapping Registers
//
#define R_SATA_CFG_RST_PCIE_STORAGE_RCR                 0x800                           ///< Remap Capability Register
#define B_SATA_CFG_RST_PCIE_STORAGE_RCR_NRS             (BIT2|BIT1|BIT0)                ///< Number of Remapping Supported
#define B_SATA_CFG_RST_PCIE_STORAGE_RCR_NRS_CR1         BIT0                            ///< Number of Remapping Supported (RST PCIe Storage Cycle Router #1)
#define R_SATA_CFG_RST_PCIE_STORAGE_SPR                 0x80C                           ///< Scratch Pad Register
#define R_SATA_CFG_RST_PCIE_STORAGE_CR1_DCC             0x880                           ///< CR#1 Device Class Code
#define N_SATA_CFG_RST_PCIE_STORAGE_CR1_DCC_SCC         8
#define N_SATA_CFG_RST_PCIE_STORAGE_CR1_DCC_BCC         16
#define B_SATA_CFG_RST_PCIE_STORAGE_CR1_DCC_DT          BIT31                           ///< Device Type
#define V_SATA_CFG_RST_PCIE_STORAGE_REMAP_CONFIG_CR     0x80                            ///< Remapped Configuration for RST PCIe Storage Cycle Router #n
#define V_SATA_CFG_RST_PCIE_STORAGE_REMAP_RP_OFFSET     0x100                           ///< Remapped Root Port Offset Value
#define R_SATA_CFG_RST_PCIE_STORAGE_CCFG                0x1D0                           ///< Port Configuration Register
//
// AHCI BAR Area related Registers
//
#define R_SATA_MEM_AHCI_CAP                 0x0
#define B_SATA_MEM_AHCI_CAP_S64A            BIT31
#define B_SATA_MEM_AHCI_CAP_SCQA            BIT30
#define B_SATA_MEM_AHCI_CAP_SSNTF           BIT29
#define B_SATA_MEM_AHCI_CAP_SMPS            BIT28 ///< Supports Interlock Switch
#define B_SATA_MEM_AHCI_CAP_SSS             BIT27 ///< Supports Stagger Spin-up
#define B_SATA_MEM_AHCI_CAP_SALP            BIT26
#define B_SATA_MEM_AHCI_CAP_SAL             BIT25
#define B_SATA_MEM_AHCI_CAP_SCLO            BIT24 ///< Supports Command List Override
#define B_SATA_MEM_AHCI_CAP_ISS_MASK        (BIT23 | BIT22 | BIT21 | BIT20)
#define N_SATA_MEM_AHCI_CAP_ISS             20    ///< Interface Speed Support
#define V_SATA_MEM_AHCI_CAP_ISS_1_5_G       0x01
#define V_SATA_MEM_AHCI_CAP_ISS_3_0_G       0x02
#define V_SATA_MEM_AHCI_CAP_ISS_6_0_G       0x03
#define B_SATA_MEM_AHCI_CAP_SNZO            BIT19
#define B_SATA_MEM_AHCI_CAP_SAM             BIT18
#define B_SATA_MEM_AHCI_CAP_SPM             BIT17 ///< Supports Port Multiplier
#define B_SATA_MEM_AHCI_CAP_PMD             BIT15 ///< PIO Multiple DRQ Block
#define B_SATA_MEM_AHCI_CAP_SSC             BIT14
#define B_SATA_MEM_AHCI_CAP_PSC             BIT13
#define B_SATA_MEM_AHCI_CAP_NCS             0x1F00
#define B_SATA_MEM_AHCI_CAP_CCCS            BIT7
#define B_SATA_MEM_AHCI_CAP_EMS             BIT6
#define B_SATA_MEM_AHCI_CAP_SXS             BIT5  ///< External SATA is supported
#define B_SATA_MEM_AHCI_CAP_NPS             0x001F

#define R_SATA_MEM_AHCI_GHC                 0x04
#define B_SATA_MEM_AHCI_GHC_AE              BIT31
#define B_SATA_MEM_AHCI_GHC_MRSM            BIT2
#define B_SATA_MEM_AHCI_GHC_IE              BIT1
#define B_SATA_MEM_AHCI_GHC_HR              BIT0

#define R_SATA_MEM_AHCI_IS                  0x08
#define B_SATA_MEM_AHCI_IS_PORT7            BIT7
#define B_SATA_MEM_AHCI_IS_PORT6            BIT6
#define B_SATA_MEM_AHCI_IS_PORT5            BIT5
#define B_SATA_MEM_AHCI_IS_PORT4            BIT4
#define B_SATA_MEM_AHCI_IS_PORT3            BIT3
#define B_SATA_MEM_AHCI_IS_PORT2            BIT2
#define B_SATA_MEM_AHCI_IS_PORT1            BIT1
#define B_SATA_MEM_AHCI_IS_PORT0            BIT0
#define R_SATA_MEM_AHCI_PI                  0x0C
#define B_SATA_MEM_AHCI_PI_PORT_MASK        0xFF
#define B_SATA_MEM_PORT7_IMPLEMENTED        BIT7
#define B_SATA_MEM_PORT6_IMPLEMENTED        BIT6
#define B_SATA_MEM_PORT5_IMPLEMENTED        BIT5
#define B_SATA_MEM_PORT4_IMPLEMENTED        BIT4
#define B_SATA_MEM_PORT3_IMPLEMENTED        BIT3
#define B_SATA_MEM_PORT2_IMPLEMENTED        BIT2
#define B_SATA_MEM_PORT1_IMPLEMENTED        BIT1
#define B_SATA_MEM_PORT0_IMPLEMENTED        BIT0
#define R_SATA_MEM_AHCI_VS                  0x10
#define B_SATA_MEM_AHCI_VS_MJR              0xFFFF0000
#define B_SATA_MEM_AHCI_VS_MNR              0x0000FFFF
#define R_SATA_MEM_AHCI_EM_LOC              0x1C
#define B_SATA_MEM_AHCI_EM_LOC_OFST         0xFFFF0000
#define B_SATA_MEM_AHCI_EM_LOC_SZ           0x0000FFFF
#define R_SATA_MEM_AHCI_EM_CTRL             0x20
#define B_SATA_MEM_AHCI_EM_CTRL_ATTR_ALHD   BIT26
#define B_SATA_MEM_AHCI_EM_CTRL_ATTR_XMT    BIT25
#define B_SATA_MEM_AHCI_EM_CTRL_ATTR_SMB    BIT24
#define B_SATA_MEM_AHCI_EM_CTRL_SUPP_SGPIO  BIT19
#define B_SATA_MEM_AHCI_EM_CTRL_SUPP_SES2   BIT18
#define B_SATA_MEM_AHCI_EM_CTRL_SUPP_SAFTE  BIT17
#define B_SATA_MEM_AHCI_EM_CTRL_SUPP_LED    BIT16
#define B_SATA_MEM_AHCI_EM_CTRL_RST         BIT9
#define B_SATA_MEM_AHCI_EM_CTRL_CTL_TM      BIT8
#define B_SATA_MEM_AHCI_EM_CTRL_STS_MR      BIT0
#define R_SATA_MEM_AHCI_CAP2                0x24
#define B_SATA_MEM_AHCI_CAP2_DESO           BIT5
#define B_SATA_MEM_AHCI_CAP2_SADM           BIT4
#define B_SATA_MEM_AHCI_CAP2_SDS            BIT3
#define B_SATA_MEM_AHCI_CAP2_APST           BIT2  ///< Automatic Partial to Slumber Transitions
#define R_SATA_MEM_AHCI_VSP                 0xA0
#define B_SATA_MEM_AHCI_VSP_SLPD            BIT0
#define R_SATA_MEM_AHCI_SFM                 0xC8  ///< RST Feature Capabilities
#define B_SATA_MEM_AHCI_SFM_LEGACY          BIT12
#define B_SATA_MEM_AHCI_SFM_OUD             (BIT11 | BIT10)
#define N_SATA_MEM_AHCI_SFM_OUD             10
#define B_SATA_MEM_AHCI_SFM_SEREQ           BIT9
#define B_SATA_MEM_AHCI_SFM_IROES           BIT8
#define B_SATA_MEM_AHCI_SFM_LEDL            BIT7
#define B_SATA_MEM_AHCI_SFM_HDDLK           BIT6
#define B_SATA_MEM_AHCI_SFM_IRSTOROM        BIT5
#define B_SATA_MEM_AHCI_SFM_RSTE            BIT4
#define B_SATA_MEM_AHCI_SFM_R5E             BIT3
#define B_SATA_MEM_AHCI_SFM_R10E            BIT2
#define B_SATA_MEM_AHCI_SFM_R1E             BIT1
#define B_SATA_MEM_AHCI_SFM_R0E             BIT0
#define B_SATA_MEM_AHCI_SFM_LOWBYTES        0x1FF
#define R_SATA_MEM_AHCI_P0CLB               0x100
#define R_SATA_MEM_AHCI_P1CLB               0x180
#define R_SATA_MEM_AHCI_P2CLB               0x200
#define R_SATA_MEM_AHCI_P3CLB               0x280
#define R_SATA_MEM_AHCI_P4CLB               0x300
#define R_SATA_MEM_AHCI_P5CLB               0x380
#define R_SATA_MEM_AHCI_P6CLB               0x400
#define R_SATA_MEM_AHCI_P7CLB               0x480
#define B_SATA_MEM_AHCI_PXCLB               0xFFFFFC00
#define R_SATA_MEM_AHCI_P0CLBU              0x104
#define R_SATA_MEM_AHCI_P1CLBU              0x184
#define R_SATA_MEM_AHCI_P2CLBU              0x204
#define R_SATA_MEM_AHCI_P3CLBU              0x284
#define R_SATA_MEM_AHCI_P4CLBU              0x304
#define R_SATA_MEM_AHCI_P5CLBU              0x384
#define R_SATA_MEM_AHCI_P6CLBU              0x404
#define R_SATA_MEM_AHCI_P7CLBU              0x484
#define B_SATA_MEM_AHCI_PXCLBU              0xFFFFFFFF
#define R_SATA_MEM_AHCI_P0FB                0x108
#define R_SATA_MEM_AHCI_P1FB                0x188
#define R_SATA_MEM_AHCI_P2FB                0x208
#define R_SATA_MEM_AHCI_P3FB                0x288
#define R_SATA_MEM_AHCI_P4FB                0x308
#define R_SATA_MEM_AHCI_P5FB                0x388
#define R_SATA_MEM_AHCI_P6FB                0x408
#define R_SATA_MEM_AHCI_P7FB                0x488
#define B_SATA_MEM_AHCI_PXFB                0xFFFFFF00
#define R_SATA_MEM_AHCI_P0FBU               0x10C
#define R_SATA_MEM_AHCI_P1FBU               0x18C
#define R_SATA_MEM_AHCI_P2FBU               0x20C
#define R_SATA_MEM_AHCI_P3FBU               0x28C
#define R_SATA_MEM_AHCI_P4FBU               0x30C
#define R_SATA_MEM_AHCI_P5FBU               0x38C
#define R_SATA_MEM_AHCI_P6FBU               0x40C
#define R_SATA_MEM_AHCI_P7FBU               0x48C
#define B_SATA_MEM_AHCI_PXFBU               0xFFFFFFFF
#define R_SATA_MEM_AHCI_P0IS                0x110
#define R_SATA_MEM_AHCI_P1IS                0x190
#define R_SATA_MEM_AHCI_P2IS                0x210
#define R_SATA_MEM_AHCI_P3IS                0x290
#define R_SATA_MEM_AHCI_P4IS                0x310
#define R_SATA_MEM_AHCI_P5IS                0x390
#define R_SATA_MEM_AHCI_P6IS                0x410
#define R_SATA_MEM_AHCI_P7IS                0x490
#define B_SATA_MEM_AHCI_PXIS_CPDS           BIT31
#define B_SATA_MEM_AHCI_PXIS_TFES           BIT30
#define B_SATA_MEM_AHCI_PXIS_HBFS           BIT29
#define B_SATA_MEM_AHCI_PXIS_HBDS           BIT28
#define B_SATA_MEM_AHCI_PXIS_IFS            BIT27
#define B_SATA_MEM_AHCI_PXIS_INFS           BIT26
#define B_SATA_MEM_AHCI_PXIS_OFS            BIT24
#define B_SATA_MEM_AHCI_PXIS_IPMS           BIT23
#define B_SATA_MEM_AHCI_PXIS_PRCS           BIT22
#define B_SATA_MEM_AHCI_PXIS_DIS            BIT7
#define B_SATA_MEM_AHCI_PXIS_PCS            BIT6
#define B_SATA_MEM_AHCI_PXIS_DPS            BIT5
#define B_SATA_MEM_AHCI_PXIS_UFS            BIT4
#define B_SATA_MEM_AHCI_PXIS_SDBS           BIT3
#define B_SATA_MEM_AHCI_PXIS_DSS            BIT2
#define B_SATA_MEM_AHCI_PXIS_PSS            BIT1
#define B_SATA_MEM_AHCI_PXIS_DHRS           BIT0
#define R_SATA_MEM_AHCI_P0IE                0x114
#define R_SATA_MEM_AHCI_P1IE                0x194
#define R_SATA_MEM_AHCI_P2IE                0x214
#define R_SATA_MEM_AHCI_P3IE                0x294
#define R_SATA_MEM_AHCI_P4IE                0x314
#define R_SATA_MEM_AHCI_P5IE                0x394
#define R_SATA_MEM_AHCI_P6IE                0x414
#define R_SATA_MEM_AHCI_P7IE                0x494
#define B_SATA_MEM_AHCI_PXIE_CPDE           BIT31
#define B_SATA_MEM_AHCI_PXIE_TFEE           BIT30
#define B_SATA_MEM_AHCI_PXIE_HBFE           BIT29
#define B_SATA_MEM_AHCI_PXIE_HBDE           BIT28
#define B_SATA_MEM_AHCI_PXIE_IFE            BIT27
#define B_SATA_MEM_AHCI_PXIE_INFE           BIT26
#define B_SATA_MEM_AHCI_PXIE_OFE            BIT24
#define B_SATA_MEM_AHCI_PXIE_IPME           BIT23
#define B_SATA_MEM_AHCI_PXIE_PRCE           BIT22
#define B_SATA_MEM_AHCI_PXIE_DIE            BIT7
#define B_SATA_MEM_AHCI_PXIE_PCE            BIT6
#define B_SATA_MEM_AHCI_PXIE_DPE            BIT5
#define B_SATA_MEM_AHCI_PXIE_UFIE           BIT4
#define B_SATA_MEM_AHCI_PXIE_SDBE           BIT3
#define B_SATA_MEM_AHCI_PXIE_DSE            BIT2
#define B_SATA_MEM_AHCI_PXIE_PSE            BIT1
#define B_SATA_MEM_AHCI_PXIE_DHRE           BIT0
#define R_SATA_MEM_AHCI_P0CMD               0x118
#define R_SATA_MEM_AHCI_P1CMD               0x198
#define R_SATA_MEM_AHCI_P2CMD               0x218
#define R_SATA_MEM_AHCI_P3CMD               0x298
#define R_SATA_MEM_AHCI_P4CMD               0x318
#define R_SATA_MEM_AHCI_P5CMD               0x398
#define R_SATA_MEM_AHCI_P6CMD               0x418
#define R_SATA_MEM_AHCI_P7CMD               0x498
#define B_SATA_MEM_AHCI_PxCMD_ESP           BIT21 ///< Used with an external SATA device
#define B_SATA_MEM_AHCI_PxCMD_MPSP          BIT19 ///< Mechanical Switch Attached to Port
#define B_SATA_MEM_AHCI_PxCMD_HPCP          BIT18 ///< Hotplug capable
#define B_SATA_MEM_AHCI_PxCMD_CR            BIT15
#define B_SATA_MEM_AHCI_PxCMD_FR            BIT14
#define B_SATA_MEM_AHCI_PxCMD_ISS           BIT13
#define B_SATA_MEM_AHCI_PxCMD_CCS           0x00001F00
#define B_SATA_MEM_AHCI_PxCMD_FRE           BIT4
#define B_SATA_MEM_AHCI_PxCMD_CLO           BIT3
#define B_SATA_MEM_AHCI_PxCMD_POD           BIT2
#define B_SATA_MEM_AHCI_PxCMD_SUD           BIT1
#define B_SATA_MEM_AHCI_PxCMD_ST            BIT0
#define R_SATA_MEM_AHCI_P0TFD               0x120
#define R_SATA_MEM_AHCI_P1TFD               0x1A0
#define R_SATA_MEM_AHCI_P2TFD               0x220
#define R_SATA_MEM_AHCI_P3TFD               0x2A0
#define R_SATA_MEM_AHCI_P4TFD               0x320
#define R_SATA_MEM_AHCI_P5TFD               0x3A0
#define R_SATA_MEM_AHCI_P6TFD               0x420
#define B_SATA_MEM_AHCI_PXTFD_ERR           0x0000FF00
#define B_SATA_MEM_AHCI_PXTFD_STS           0x000000FF
#define R_SATA_MEM_AHCI_P0SIG               0x124
#define R_SATA_MEM_AHCI_P1SIG               0x1A4
#define R_SATA_MEM_AHCI_P2SIG               0x224
#define R_SATA_MEM_AHCI_P3SIG               0x2A4
#define R_SATA_MEM_AHCI_P4SIG               0x324
#define R_SATA_MEM_AHCI_P5SIG               0x3A4
#define R_SATA_MEM_AHCI_P6SIG               0x424
#define B_SATA_MEM_AHCI_PXSIG_LBA_HR        0xFF000000
#define B_SATA_MEM_AHCI_PXSIG_LBA_MR        0x00FF0000
#define B_SATA_MEM_AHCI_PXSIG_LBA_LR        0x0000FF00
#define B_SATA_MEM_AHCI_PXSIG_SCR           0x000000FF
#define R_SATA_MEM_AHCI_P0SSTS              0x128
#define R_SATA_MEM_AHCI_P1SSTS              0x1A8
#define R_SATA_MEM_AHCI_P2SSTS              0x228
#define R_SATA_MEM_AHCI_P3SSTS              0x2A8
#define R_SATA_MEM_AHCI_P4SSTS              0x328
#define R_SATA_MEM_AHCI_P5SSTS              0x3A8
#define R_SATA_MEM_AHCI_P6SSTS              0x428
#define B_SATA_MEM_AHCI_PXSSTS_IPM_0        0x00000000
#define B_SATA_MEM_AHCI_PXSSTS_IPM_1        0x00000100
#define B_SATA_MEM_AHCI_PXSSTS_IPM_2        0x00000200
#define B_SATA_MEM_AHCI_PXSSTS_IPM_6        0x00000600
#define B_SATA_MEM_AHCI_PXSSTS_SPD_0        0x00000000
#define B_SATA_MEM_AHCI_PXSSTS_SPD_1        0x00000010
#define B_SATA_MEM_AHCI_PXSSTS_SPD_2        0x00000020
#define B_SATA_MEM_AHCI_PXSSTS_SPD_3        0x00000030
#define B_SATA_MEM_AHCI_PXSSTS_DET_0        0x00000000
#define B_SATA_MEM_AHCI_PXSSTS_DET_1        0x00000001
#define B_SATA_MEM_AHCI_PXSSTS_DET_3        0x00000003
#define B_SATA_MEM_AHCI_PXSSTS_DET_4        0x00000004
#define R_SATA_MEM_AHCI_P0SCTL              0x12C
#define R_SATA_MEM_AHCI_P1SCTL              0x1AC
#define R_SATA_MEM_AHCI_P2SCTL              0x22C
#define R_SATA_MEM_AHCI_P3SCTL              0x2AC
#define R_SATA_MEM_AHCI_P4SCTL              0x32C
#define R_SATA_MEM_AHCI_P5SCTL              0x3AC
#define R_SATA_MEM_AHCI_P6SCTL              0x42C
#define B_SATA_MEM_AHCI_PXSCTL_IPM          0x00000F00
#define V_SATA_MEM_AHCI_PXSCTL_IPM_0        0x00000000
#define V_SATA_MEM_AHCI_PXSCTL_IPM_1        0x00000100
#define V_SATA_MEM_AHCI_PXSCTL_IPM_2        0x00000200
#define V_SATA_MEM_AHCI_PXSCTL_IPM_3        0x00000300
#define B_SATA_MEM_AHCI_PXSCTL_SPD          0x000000F0
#define V_SATA_MEM_AHCI_PXSCTL_SPD_0        0x00000000
#define V_SATA_MEM_AHCI_PXSCTL_SPD_1        0x00000010
#define V_SATA_MEM_AHCI_PXSCTL_SPD_2        0x00000020
#define V_SATA_MEM_AHCI_PXSCTL_SPD_3        0x00000030
#define B_SATA_MEM_AHCI_PXSCTL_DET          0x0000000F
#define V_SATA_MEM_AHCI_PXSCTL_DET_0        0x00000000
#define V_SATA_MEM_AHCI_PXSCTL_DET_1        0x00000001
#define V_SATA_MEM_AHCI_PXSCTL_DET_4        0x00000004
#define R_SATA_MEM_AHCI_P0SERR              0x130
#define R_SATA_MEM_AHCI_P1SERR              0x1B0
#define R_SATA_MEM_AHCI_P2SERR              0x230
#define R_SATA_MEM_AHCI_P3SERR              0x2B0
#define R_SATA_MEM_AHCI_P4SERR              0x330
#define R_SATA_MEM_AHCI_P5SERR              0x3B0
#define R_SATA_MEM_AHCI_P6SERR              0x430
#define B_SATA_MEM_AHCI_PXSERR_EXCHG        BIT26
#define B_SATA_MEM_AHCI_PXSERR_UN_FIS_TYPE  BIT25
#define B_SATA_MEM_AHCI_PXSERR_TRSTE_24     BIT24
#define B_SATA_MEM_AHCI_PXSERR_TRSTE_23     BIT23
#define B_SATA_MEM_AHCI_PXSERR_HANDSHAKE    BIT22
#define B_SATA_MEM_AHCI_PXSERR_CRC_ERROR    BIT21
#define B_SATA_MEM_AHCI_PXSERR_10B8B_DECERR BIT19
#define B_SATA_MEM_AHCI_PXSERR_COMM_WAKE    BIT18
#define B_SATA_MEM_AHCI_PXSERR_PHY_ERROR    BIT17
#define B_SATA_MEM_AHCI_PXSERR_PHY_RDY_CHG  BIT16
#define B_SATA_MEM_AHCI_PXSERR_INTRNAL_ERR  BIT11
#define B_SATA_MEM_AHCI_PXSERR_PROTOCOL_ERR BIT10
#define B_SATA_MEM_AHCI_PXSERR_PCDIE        BIT9
#define B_SATA_MEM_AHCI_PXSERR_TDIE         BIT8
#define B_SATA_MEM_AHCI_PXSERR_RCE          BIT1
#define B_SATA_MEM_AHCI_PXSERR_RDIE         BIT0
#define R_SATA_MEM_AHCI_P0SACT              0x134
#define R_SATA_MEM_AHCI_P1SACT              0x1B4
#define R_SATA_MEM_AHCI_P2SACT              0x234
#define R_SATA_MEM_AHCI_P3SACT              0x2B4
#define R_SATA_MEM_AHCI_P4SACT              0x334
#define R_SATA_MEM_AHCI_P5SACT              0x3B4
#define R_SATA_MEM_AHCI_P6SACT              0x434
#define B_SATA_MEM_AHCI_PXSACT_DS           0xFFFFFFFF
#define R_SATA_MEM_AHCI_P0CI                0x138
#define R_SATA_MEM_AHCI_P1CI                0x1B8
#define R_SATA_MEM_AHCI_P2CI                0x238
#define R_SATA_MEM_AHCI_P3CI                0x2B8
#define R_SATA_MEM_AHCI_P4CI                0x338
#define R_SATA_MEM_AHCI_P5CI                0x3B8
#define R_SATA_MEM_AHCI_P6CI                0x438
#define B_SATA_MEM_AHCI_PXCI                0xFFFFFFFF

//
//  SATA AHCI Device ID macros
//
#define IS_PCH_H_SATA_AHCI_DEVICE_ID(DeviceId) \
    ( \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_AHCI) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_MH_AHCI) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_OP_AHCI) \
    )

#define IS_PCH_LP_SATA_AHCI_DEVICE_ID(DeviceId) \
    ( \
      (DeviceId == V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_AHCI) \
    )

#define IS_PCH_SATA_AHCI_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_SATA_AHCI_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_SATA_AHCI_DEVICE_ID(DeviceId) \
    )

//
//  SATA RAID Device ID macros
//
#define IS_PCH_H_SATA_RAID_DEVICE_ID(DeviceId) \
    ( \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_MH_RAID) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_PREM) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_MH_RAID_PREM) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_IBC) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_IBC_RST) || \
      (DeviceId == V_CNL_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_IBC_2) || \
      (DeviceId == V_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_ALTDIS) || \
      (DeviceId == V_PCH_H_SATA_CFG_DEVICE_ID_D_RAID_RSTE) \
    )

#define IS_PCH_LP_SATA_RAID_DEVICE_ID(DeviceId) \
    ( \
      (DeviceId == V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_RAID) || \
      (DeviceId == V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_RAID_PREM) || \
      (DeviceId == V_CNL_PCH_LP_SATA_CFG_DEVICE_ID_M_RAID_IBC) \
    )

#define IS_PCH_SATA_RAID_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_SATA_RAID_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_SATA_RAID_DEVICE_ID(DeviceId) \
    )

//
//  Combined SATA IDE/AHCI/RAID Device ID macros
//
#define IS_PCH_H_SATA_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_SATA_AHCI_DEVICE_ID(DeviceId) || \
      IS_PCH_H_SATA_RAID_DEVICE_ID(DeviceId) \
    )

#define IS_PCH_LP_SATA_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_LP_SATA_AHCI_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_SATA_RAID_DEVICE_ID(DeviceId) \
    )
#define IS_PCH_SATA_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_SATA_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_SATA_DEVICE_ID(DeviceId) \
    )

#endif
