/** @file
  Register definition for PSF component
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
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_PSF_H_
#define _PCH_REGS_PSF_H_

//
// Private chipset regsiter (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PSFx segment registers
//
#define R_PCH_PCR_PSF_GLOBAL_CONFIG                     0x4000                  ///< PSF Segment Global Configuration Register
#define B_PCH_PCR_PSF_GLOBAL_CONFIG_ENTCG               BIT4
#define B_PCH_PCR_PSF_GLOBAL_CONFIG_ENLCG               BIT3
#define R_PCH_PCR_PSF_ROOTSPACE_CONFIG_RS0              0x4014                  ///< PSF Segment Rootspace Configuration Register
#define B_PCH_PCR_PSF_ROOTSPACE_CONFIG_RS0_ENADDRP2P    BIT1
#define B_PCH_PCR_PSF_ROOTSPACE_CONFIG_RS0_VTDEN        BIT0
#define R_PCH_PCR_PSF_PORT_CONFIG_PG0_PORT0             0x4020                  ///< PSF Segment Port Configuration Register

#define S_PCH_PSF_DEV_GNTCNT_RELOAD_DGCR                4
#define S_PCH_PSF_TARGET_GNTCNT_RELOAD                  4
#define B_PCH_PSF_DEV_GNTCNT_RELOAD_DGCR_GNT_CNT_RELOAD 0x1F
#define B_PCH_PSF_TARGET_GNTCNT_RELOAD_GNT_CNT_RELOAD   0x1F

//
// PSFx PCRs definitions
//
#define R_PCH_PCR_PSFX_T0_SHDW_BAR0                     0                       ///< PCI BAR0
#define R_PCH_PCR_PSFX_T0_SHDW_BAR1                     0x04                    ///< PCI BAR1
#define R_PCH_PCR_PSFX_T0_SHDW_BAR2                     0x08                    ///< PCI BAR2
#define R_PCH_PCR_PSFX_T0_SHDW_BAR3                     0x0C                    ///< PCI BAR3
#define R_PCH_PCR_PSFX_T0_SHDW_BAR4                     0x10                    ///< PCI BAR4
#define R_PCH_PSFX_PCR_T0_SHDW_PCIEN                    0x1C                    ///< PCI configuration space enable bits
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR0DIS            BIT16                   ///< Disable BAR0
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR1DIS            BIT17                   ///< Disable BAR1
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR2DIS            BIT18                   ///< Disable BAR2
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR3DIS            BIT19                   ///< Disable BAR3
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR4DIS            BIT20                   ///< Disable BAR4
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_BAR5DIS            BIT21                   ///< Disable BAR5
#define B_PCH_PSFX_PCR_T0_SHDW_PCIEN_FUNDIS             BIT8                    ///< Function disable
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_MEMEN              BIT1                    ///< Memory decoding enable
#define B_PCH_PCR_PSFX_T0_SHDW_PCIEN_IOEN               BIT0                    ///< IO decoding enable
#define R_PCH_PCR_PSFX_T0_SHDW_PMCSR                    0x20                    ///< PCI power management configuration
#define B_PCH_PCR_PSFX_T0_SHDW_PMCSR_PWRST              (BIT1 | BIT0)           ///< Power status
#define R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS                  0x38                    ///< PCI configuration disable
#define B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS           BIT0                    ///< config disable

#define R_PCH_PCR_PSFX_T1_SHDW_PCIEN                    0x3C                    ///< PCI configuration space enable bits
#define B_PCH_PSFX_PCR_T1_SHDW_PCIEN_FUNDIS             BIT8                    ///< Function disable
#define B_PCH_PCR_PSFX_T1_SHDW_PCIEN_MEMEN              BIT1                    ///< Memory decoding enable
#define B_PCH_PCR_PSFX_T1_SHDW_PCIEN_IOEN               BIT0                    ///< IO decoding enable

#define B_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_DEVICE          0x01F0                 ///< device number
#define N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_DEVICE          4
#define B_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION        (BIT3 | BIT2 | BIT1)   ///< function number
#define N_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_FUNCTION        1

#define V_PCH_LP_PCR_PSFX_PSF_MC_AGENT_MCAST_TGT_P2SB   0x38A00
#define V_PCH_H_PCR_PSFX_PSF_MC_AGENT_MCAST_TGT_P2SB    0x38B00

//
// PSF1 PCRs
//
// PSF1 PCH-LP Specific Base Address
#define R_PCH_LP_PCR_PSF1_T0_SHDW_GBE_REG_BASE          0x0200                  ///< D31F6 PSF base address (GBE)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_CAM_REG_BASE          0x0300                  ///< D20F3 PSF base address (CAM)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_CSE_WLAN_REG_BASE     0x0500                  ///< D22F7 PSF base address (CSME: WLAN)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_HECI3_REG_BASE        0x0700                  ///< D22F4 PSF base address (CSME: HECI3)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_HECI2_REG_BASE        0x0800                  ///< D22F1 PSF base address (CSME: HECI2)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_CSE_UMA_REG_BASE      0x0900                  ///< D18F3 PSF base address (CSME: CSE UMA)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_HECI1_REG_BASE        0x0A00                  ///< D22F0 PSF base address (CSME: HECI1)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_KT_REG_BASE           0x0B00                  ///< D22F3 PSF base address (CSME: KT)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_IDER_REG_BASE         0x0C00                  ///< D22F2 PSF base address (CSME: IDER)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_CLINK_REG_BASE        0x0D00                  ///< D18F1 PSF base address (CSME: CLINK)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_PMT_REG_BASE          0x0E00                  ///< D18F2 PSF base address (CSME: PMT)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_KVM_REG_BASE          0x0F00                  ///< D18F0 PSF base address (CSME: KVM)
#define R_PCH_LP_PCR_PSF1_T0_SHDW_SATA_REG_BASE         0x1000                  ///< PCH-LP D23F0 PSF base address (SATA)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE12_REG_BASE       0x2000                  ///< PCH-LP D29F3 PSF base address (PCIE PORT 12)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE11_REG_BASE       0x2100                  ///< PCH-LP D29F2 PSF base address (PCIE PORT 11)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE10_REG_BASE       0x2200                  ///< PCH-LP D29F1 PSF base address (PCIE PORT 10)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE09_REG_BASE       0x2300                  ///< PCH-LP D29F0 PSF base address (PCIE PORT 09)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE08_REG_BASE       0x2400                  ///< PCH-LP D28F7 PSF base address (PCIE PORT 08)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE07_REG_BASE       0x2500                  ///< PCH-LP D28F6 PSF base address (PCIE PORT 07)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE06_REG_BASE       0x2600                  ///< PCH-LP D28F5 PSF base address (PCIE PORT 06)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE05_REG_BASE       0x2700                  ///< PCH-LP D28F4 PSF base address (PCIE PORT 05)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE04_REG_BASE       0x2800                  ///< PCH-LP D28F3 PSF base address (PCIE PORT 04)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE03_REG_BASE       0x2900                  ///< PCH-LP D28F2 PSF base address (PCIE PORT 03)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE02_REG_BASE       0x2A00                  ///< PCH-LP D28F1 PSF base address (PCIE PORT 02)
#define R_PCH_LP_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE       0x2B00                  ///< PCH-LP D28F0 PSF base address (PCIE PORT 01)

// PSF1 PCH-H Specific Base Address
#define R_PCH_H_PCR_PSF1_T0_SHDW_CSE_WLAN_REG_BASE      0x0200                  ///< D22F7 PSF base address (CSME: WLAN)
#define R_PCH_H_PCR_PSF1_T0_SHDW_HECI3_REG_BASE         0x0300                  ///< SPT-H D22F4 PSF base address (CSME: HECI3)
#define R_PCH_H_PCR_PSF1_T0_SHDW_HECI2_REG_BASE         0x0400                  ///< SPT-H D22F1 PSF base address (CSME: HECI2)
#define R_PCH_H_PCR_PSF1_T0_SHDW_CSE_UMA_REG_BASE       0x0500                  ///< D18F3 PSF base address (CSME: CSE UMA)
#define R_PCH_H_PCR_PSF1_T0_SHDW_HECI1_REG_BASE         0x0600                  ///< SPT-H D22F0 PSF base address (CSME: HECI1)
#define R_PCH_H_PCR_PSF1_T0_SHDW_KT_REG_BASE            0x0700                  ///< SPT-H D22F3 PSF base address (CSME: KT)
#define R_PCH_H_PCR_PSF1_T0_SHDW_IDER_REG_BASE          0x0800                  ///< SPT-H D22F2 PSF base address (CSME: IDER)
#define R_PCH_H_PCR_PSF1_T0_SHDW_PMT_REG_BASE           0x0900                  ///< D18F2 PSF base address (CSME: PMT)
#define R_PCH_H_PCR_PSF1_T0_SHDW_IEPMT_REG_BASE         0x0A00                  ///< D16F5 PSF base address (CSME: IEPMT)
#define R_PCH_H_PCR_PSF1_T0_SHDW_SATA_REG_BASE          0x0B00                  ///< D23F0 PSF base address (SATA)
#define R_PCH_H_PCR_PSF1_T0_SHDW_sSATA_REG_BASE         0x0C00                  ///< D17F5 PSF base address (sSATA)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE20_REG_BASE        0x2000                  ///< PCH-H D27F3 PSF base address (PCIE PORT 20)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE19_REG_BASE        0x2100                  ///< PCH-H D27F2 PSF base address (PCIE PORT 19)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE18_REG_BASE        0x2200                  ///< PCH-H D27F1 PSF base address (PCIE PORT 18)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE17_REG_BASE        0x2300                  ///< PCH-H D27F0 PSF base address (PCIE PORT 17)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE16_REG_BASE        0x2400                  ///< PCH-H D29F7 PSF base address (PCIE PORT 16)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE15_REG_BASE        0x2500                  ///< PCH-H D29F6 PSF base address (PCIE PORT 15)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE14_REG_BASE        0x2600                  ///< PCH-H D29F5 PSF base address (PCIE PORT 14)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE13_REG_BASE        0x2700                  ///< PCH-H D29F4 PSF base address (PCIE PORT 13)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE12_REG_BASE        0x2800                  ///< PCH-H D29F3 PSF base address (PCIE PORT 10)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE11_REG_BASE        0x2900                  ///< PCH-H D29F2 PSF base address (PCIE PORT 11)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE10_REG_BASE        0x2A00                  ///< PCH-H D29F1 PSF base address (PCIE PORT 10)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE09_REG_BASE        0x2B00                  ///< PCH-H D29F0 PSF base address (PCIE PORT 09)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE08_REG_BASE        0x2C00                  ///< PCH-H D28F7 PSF base address (PCIE PORT 08)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE07_REG_BASE        0x2D00                  ///< PCH-H D28F6 PSF base address (PCIE PORT 07)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE06_REG_BASE        0x2E00                  ///< PCH-H D28F5 PSF base address (PCIE PORT 06)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE05_REG_BASE        0x2F00                  ///< PCH-H D28F4 PSF base address (PCIE PORT 05)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE04_REG_BASE        0x3000                  ///< PCH-H D28F3 PSF base address (PCIE PORT 04)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE03_REG_BASE        0x3100                  ///< PCH-H D28F2 PSF base address (PCIE PORT 03)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE02_REG_BASE        0x3200                  ///< PCH-H D28F1 PSF base address (PCIE PORT 02)
#define R_PCH_H_PCR_PSF1_T1_SHDW_PCIE01_REG_BASE        0x3300                  ///< PCH-H D28F0 PSF base address (PCIE PORT 01)

// Other PSF1 PCRs definition
#define R_PCH_H_PCR_PSF1_T0_SHDW_SATA_VS_CAP_VR_RS0_D23_F0_OFFSET16 0x1024
#define R_PCH_H_PCR_PSF1_T0_SHDW_SATA_MMIOPI_VR_RS0_D23_F0_OFFSET16 0x1030
#define R_PCH_H_PCR_PSF1_PSF_PORT_CONFIG_PG1_PORT7                  0x4040      ///< PSF Port Configuration Register
#define R_PCH_LP_PCR_PSF1_PSF_PORT_CONFIG_PG1_PORT7                 0x403C      ///< PSF Port Configuration Register
#define R_PCH_LP_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI                 0x4050      ///< Multicast Control Register
#define R_PCH_LP_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI              0x4060      ///< Destination ID
#define R_PCH_H_PCR_PSF1_PSF_MC_CONTROL_MCAST0_EOI                  0x4054      ///< Multicast Control Register
#define R_PCH_H_PCR_PSF1_PSF_MC_AGENT_MCAST0_TGT0_EOI               0x406C      ///< Destination ID


//PSF 1 Multicast Message Configuration

#define R_PCH_PCR_PSF1_RC_OWNER_RS0                                                     0x4008      ///< Destination ID

#define B_PCH_PCR_PSF1_TARGET_CHANNELID                                                 0xFF
#define B_PCH_PCR_PSF1_TARGET_PORTID                                                    0x7F00
#define N_PCH_PCR_PSF1_TARGET_PORTID                                                    8
#define B_PCH_PCR_PSF1_TARGET_PORTGROUPID                                               BIT15
#define N_PCH_PCR_PSF1_TARGET_PORTGROUPID                                               15
#define B_PCH_PCR_PSF1_TARGET_PSFID                                                     0xFF0000
#define N_PCH_PCR_PSF1_TARGET_PSFID                                                     16
#define B_PCH_PCR_PSF1_TARGET_CHANMAP                                                   BIT31

#define V_PCH_PCR_PSF1_RC_OWNER_RS0_CHANNELID                                           0
#define V_PCH_PCR_PSF1_RC_OWNER_RS0_PORTID                                              10
#define V_PCH_PCR_PSF1_RC_OWNER_RS0_PORTGROUPID_DOWNSTREAM                              1
#define V_PCH_PCR_PSF1_RC_OWNER_RS0_PSFID_PMT                                           0
#define V_PCH_PCR_PSF1_RC_OWNER_RS0_PSFID_PSF1                                          1

#define R_PCH_LP_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1                            0x409C      ///< Destination ID
#define R_PCH_H_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1                             0x40D0      ///< Destination ID
#define R_PCH_H_PCR_PSF5_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0                             0x404C      ///< Destination ID
#define R_PCH_H_PCR_PSF6_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0                             0x4050      ///< Destination ID

#define V_PCH_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGTX_MCTP1_PORTGROUPID_UPSTREAM          0
#define V_PCH_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGTX_MCTP1_PORTGROUPID_DOWNSTREAM        1
#define V_PCH_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGTX_MCTP1_PSFID_PSF1                    1

#define R_PCH_H_PCR_PSF1_PSF_MC_CONTROL_MCAST1_RS0_MCTP1                                0x4060      ///< Multicast Control Register
#define R_PCH_LP_PCR_PSF1_PSF_MC_CONTROL_MCAST1_RS0_MCTP1                               0x4058      ///< Multicast Control Register
#define R_PCH_H_PCR_PSF5_PSF_MC_CONTROL_MCAST0_RS0_MCTP0                                0x4040      ///< Multicast Control Register
#define R_PCH_H_PCR_PSF6_PSF_MC_CONTROL_MCAST0_RS0_MCTP0                                0x4044      ///< Multicast Control Register

#define B_PCH_PCR_PSF1_PSF_MC_CONTROL_MCAST1_RS0_MCTP1_MULTCEN                          BIT0
#define B_PCH_PCR_PSF1_PSF_MC_CONTROL_MCAST1_RS0_MCTP1_NUMMC                            0xFE
#define N_PCH_PCR_PSF1_PSF_MC_CONTROL_MCAST1_RS0_MCTP1_NUMMC                            1

#define V_PCH_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGTX_MCTP1_CHANNELID_DMI                 0
#define V_PCH_PCR_PSF1_PSF_MC_AGENT_MCAST1_RS0_TGTX_MCTP1_PORTID_DMI                    0

#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_VR_RS0_D23_F0    0x4240   ///< VR
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_PMT_RS0_D18_F2   0x4248   ///< PMT
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_PTIO_RS0_D22_F2  0x424C   ///< PTIO
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_PTIO_RS0_D22_F3  0x4250   ///< PTIO
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_CSE_RS0_D22_F0   0x4254   ///< CSE
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_CSE_RS0_D18_F3   0x4258   ///< CSE
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_CSE_RS0_D22_F1   0x425C   ///< CSE
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_CSE_RS0_D22_F4   0x4260   ///< CSE
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_CSE_RS0_D22_F7   0x4264   ///< CSE
#define R_PCH_PCR_PSF1_T0_AGENT_FUNCTION_CONFIG_CSE_RS0_D18_F4   0x4268   ///< CSE

#define B_PCH_PCR_PSFX_TX_AGENT_FUNCTION_CONFIG_IFR              BIT0     ///< IFR
#define R_PCH_PCR_PSF1_RS_IFR                                    0x42C0   ///< This register can be used to reset all functions in a particular Root Space simultaneously

//
// controls the PCI configuration header of a PCI function
//
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0   0x4198   ///< SPA
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F1   0x419C   ///< SPA
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F2   0x41A0   ///< SPA
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F3   0x41A4   ///< SPA
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F4   0x41A8   ///< SPB
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F5   0x41AC   ///< SPB
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F6   0x41B0   ///< SPB
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F7   0x41B4   ///< SPB
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F0   0x41B8   ///< SPC
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F1   0x41BC   ///< SPC
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F2   0x41C0   ///< SPC
#define R_PCH_LP_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F3   0x41C4   ///< SPC

#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F0    0x426C   ///< SPA
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F1    0x4270   ///< SPA
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F2    0x4274   ///< SPA
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPA_D28_F3    0x4278   ///< SPA
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F4    0x427C   ///< SPB
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F5    0x4280   ///< SPB
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F6    0x4284   ///< SPB
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPB_D28_F7    0x4288   ///< SPB
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F0    0x428C   ///< SPC
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F1    0x4290   ///< SPC
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F2    0x4294   ///< SPC
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPC_D29_F3    0x4298   ///< SPC
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPD_D29_F4    0x429C   ///< SPD
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPD_D29_F5    0x42A0   ///< SPD
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPD_D29_F6    0x42A4   ///< SPD
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPD_D29_F7    0x42A8   ///< SPD
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPE_D27_F0    0x42AC   ///< SPE
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPE_D27_F1    0x42B0   ///< SPE
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPE_D27_F2    0x42B4   ///< SPE
#define R_PCH_H_PCR_PSF1_T1_AGENT_FUNCTION_CONFIG_SPE_D27_F3    0x42B8   ///< SPE

//
// PSF1 grant count registers
//
#define R_PCH_LP_PSF1_DEV_GNTCNT_RELOAD_DGCR0           0x41CC
#define R_PCH_LP_PSF1_TARGET_GNTCNT_RELOAD_PG1_TGT0     0x45D0

#define R_PCH_H_PSF1_DEV_GNTCNT_RELOAD_DGCR0            0x42C0
#define R_PCH_H_PSF1_TARGET_GNTCNT_RELOAD_PG1_TGT0      0x47AC

//
// PSF2 PCRs (PID:PSF2)
//
#define R_PCH_PCR_PSF2_T0_SHDW_TRH_REG_BASE             0x0100                  ///< D20F2 PSF base address (Thermal). // LP&H
// PSF2 PCH-LP Specific Base Address
#define R_PCH_LP_PCR_PSF2_T0_SHDW_UFS_REG_BASE          0x0200                  ///< D30F7 PSF base address (SCC: UFS)
#define R_PCH_LP_PCR_PSF2_T0_SHDW_SDCARD_REG_BASE       0x0300                  ///< D30F6 PSF base address (SCC: SDCard)
#define R_PCH_LP_PCR_PSF2_T0_SHDW_SDIO_REG_BASE         0x0400                  ///< D30F5 PSF base address (SCC: SDIO)
#define R_PCH_LP_PCR_PSF2_T0_SHDW_EMMC_REG_BASE         0x0500                  ///< D30F4 PSF base address (SCC: eMMC)
#define R_PCH_LP_PCR_PSF2_T0_SHDW_OTG_REG_BASE          0x0600                  ///< D20F1 PSF base address (USB device controller: OTG)
#define R_PCH_LP_PCR_PSF2_T0_SHDW_XHCI_REG_BASE         0x0700                  ///< D20F0 PSF base address (XHCI)
// PSF2 PCH-H Specific Base Address
#define R_PCH_H_PCR_PSF2_T0_SHDW_OTG_REG_BASE           0x0200                  ///< D20F1 PSF base address (USB device controller: OTG)
#define R_PCH_H_PCR_PSF2_T0_SHDW_XHCI_REG_BASE          0x0300                  ///< D20F0 PSF base address (XHCI)

//
// PSF3 PCRs (PID:PSF3)
//
#define R_PCH_PCR_PSF3_T0_SHDW_HECI3_REG_BASE           0x0100                  ///< D16F4 PSF base address (IE: HECI3)
#define R_PCH_PCR_PSF3_T0_SHDW_HECI2_REG_BASE           0x0200                  ///< D16F1 PSF base address (IE: HECI2)
#define R_PCH_PCR_PSF3_T0_SHDW_HECI1_REG_BASE           0x0400                  ///< D16F0 PSF base address (IE: HECI1)
#define R_PCH_PCR_PSF3_T0_SHDW_KT_REG_BASE              0x0500                  ///< D16F3 PSF base address (IE: KT)
#define R_PCH_PCR_PSF3_T0_SHDW_IDER_REG_BASE            0x0600                  ///< D16F2 PSF base address (IE: IDER)
#define R_PCH_PCR_PSF3_T0_SHDW_P2SB_REG_BASE            0x0700                  ///< D31F1 PSF base address (P2SB)
#define R_PCH_PCR_PSF3_T0_SHDW_TRACE_HUB_ACPI_REG_BASE  0x0800                  ///< D20F4 PSF base address (TraceHub ACPI)
#define R_PCH_PCR_PSF3_T0_SHDW_TRACE_HUB_REG_BASE       0x0900                  ///< D31F7 PSF base address (TraceHub PCI)
#define R_PCH_PCR_PSF3_T0_SHDW_LPC_REG_BASE             0x0A00                  ///< D31F0 PSF base address (LPC)
#define R_PCH_PCR_PSF3_T0_SHDW_SMBUS_REG_BASE           0x0B00                  ///< D31F4 PSF base address (SMBUS)
#define R_PCH_PCR_PSF3_T0_SHDW_PMC_REG_BASE             0x0E00                  ///< D31F2 PSF base address (PMC)
#define R_PCH_PCR_PSF3_T0_SHDW_SPI_SPI_REG_BASE         0x1300                  ///< D31F5 PSF base address (SPI SPI)
#define R_PCH_H_PCR_PSF3_T0_SHDW_GBE_REG_BASE           0x1600                  ///< D31F6 PSF base address (GBE)
#define R_PCH_PCR_PSF3_T0_SHDW_AUD_REG_BASE             0x1800                  ///< D31F3 PSF base address (HDA, ADSP)
#define R_PCH_PCR_PSF3_T0_SHDW_AUD_PCIEN                0x181C                  ///< D31F3 PCI Configuration space enable bits (HDA, ADSP)
#define R_PCH_PCR_PSF3_T0_SHDW_MROM1_REG_BASE           0x1A00                  ///< D17F1 PSF base address (MROM1)
#define B_PCH_PCR_PSF3_T0_SHDW_AUD_PCIEN_FUNDIS         BIT8                    ///< D31F3 Function Disable
#define R_PCH_PSF3_T0_SHDW_AUD_RS1_D24_F0_BASE          0x1700                  ///< RS1D24F0 PSF base address (HDA)

#define R_PCH_PCR_PSF3_T0_AGENT_FUNCTION_CONFIG_GBE_RS0_D31_F6   0x40F8         ///< GBE

#define R_PCH_PCR_PSF3_PSF_MC_CONTROL_MCAST0_EOI        0x4058                  ///< Multicast Control Register
#define R_PCH_PCR_PSF3_PSF_MC_AGENT_MCAST0_TGT0_EOI     0x4064                  ///< Destination ID

#endif
