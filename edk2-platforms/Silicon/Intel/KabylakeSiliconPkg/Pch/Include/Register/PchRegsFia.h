/** @file
  Register definition for FIA component

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
#ifndef _PCH_REGS_FIA_H_
#define _PCH_REGS_FIA_H_


//
// Private chipset regsiter (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PID:FIA
//
#define SKL_PCH_MAX_FIA_DRCRM                             3
#define KBL_PCH_MAX_FIA_DRCRM                             4
#define PCH_MAX_FIA_DRCRM                                 KBL_PCH_MAX_FIA_DRCRM
#define R_PCH_PCR_FIA_CC                                  0
#define B_PCH_PCR_FIA_CC_SRL                              BIT31
#define B_PCH_PCR_FIA_CC_PTOCGE                           BIT17
#define B_PCH_PCR_FIA_CC_OSCDCGE                          BIT16
#define B_PCH_PCR_FIA_CC_SCPTCGE                          BIT15

#define R_PCH_PCR_FIA_PLLCTL                              0x20
#define R_PCH_PCR_FIA_DRCRM1                              0x100
#define R_PCH_PCR_FIA_DRCRM2                              0x104
#define R_PCH_PCR_FIA_DRCRM3                              0x108
#define R_PCH_PCR_FIA_DRCRM4                              0x10C // KBL-H only
#define N_PCH_PCR_FIA_DRCRM_GBEPCKRQM                     23
#define S_PCH_PCR_FIA_DRCRM_BITS_PER_FIELD                4     // CLKREQ number is encoded in 4 bits
#define S_PCH_PCR_FIA_DRCRM_FIELDS_PER_REG                8     // each DRCRM register contains bitfields for 8 rootports
#define V_PCH_PCR_FIA_DRCRM_NO_CLKREQ                     0     // ClkReq not present
#define S_PCH_PCR_FIA_DRCRM                               4
#define R_PCH_PCR_FIA_STRPFUSECFG1_REG_BASE               0x200
#define B_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIE_PEN           BIT31
#define B_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIEPORTSEL        (BIT30 | BIT29 | BIT28)
#define N_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIEPORTSEL        28
#define R_PCH_PCR_FIA_PCIESATA_FUSECFG_REG_BASE           0x204
#define R_PCH_PCR_FIA_PCIESATA_STRPCFG_REG_BASE           0x208
#define R_PCH_PCR_FIA_PCIEUSB3_STRPFUSECFG_REG_BASE       0x20C
#define R_PCH_PCR_FIA_EXP_FUSECFG_REG_BASE                0x210
#define R_PCH_PCR_FIA_USB3SSIC_STRPFUSECFG_REG_BASE       0x214
#define R_PCH_PCR_FIA_CSI3_STRPFUSECFG_REG_BASE           0x218
#define R_PCH_PCR_FIA_USB3SATA_STRPFUSECFG_REG_BASE       0x21C
#define R_PCH_PCR_FIA_UFS_STRPFUSECFG_REG_BASE            0x220
#define R_PCH_PCR_FIA_LOS1_REG_BASE                       0x250
#define R_PCH_PCR_FIA_LOS2_REG_BASE                       0x254
#define R_PCH_PCR_FIA_LOS3_REG_BASE                       0x258
#define R_PCH_PCR_FIA_LOS4_REG_BASE                       0x25C
#define V_PCH_PCR_FIA_LANE_OWN_PCIEDMI                    0x0
#define V_PCH_PCR_FIA_LANE_OWN_USB3                       0x1
#define V_PCH_PCR_FIA_LANE_OWN_SATA                       0x2
#define V_PCH_PCR_FIA_LANE_OWN_GBE                        0x3
#define V_PCH_PCR_FIA_LANE_OWN_MOBEXP                     0x4
#define V_PCH_PCR_FIA_LANE_OWN_SSIC                       0x5
#define V_PCH_PCR_FIA_LANE_OWN_CSI3                       0x6
#define V_PCH_PCR_FIA_LANE_OWN_UFS                        0x7
#define B_PCH_PCR_FIA_L0O                                 (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_PCR_FIA_L1O                                 (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_PCR_FIA_L2O                                 (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_PCR_FIA_L3O                                 (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_PCR_FIA_L4O                                 (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_PCR_FIA_L5O                                 (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_PCR_FIA_L6O                                 (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_PCR_FIA_L7O                                 (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_PCR_FIA_L8O                                 (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_PCR_FIA_L9O                                 (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_PCR_FIA_L10O                                (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_PCR_FIA_L11O                                (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_PCR_FIA_L12O                                (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_PCR_FIA_L13O                                (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_PCR_FIA_L14O                                (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_PCR_FIA_L15O                                (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_PCR_FIA_L16O                                (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_PCR_FIA_L17O                                (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_PCR_FIA_L18O                                (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_PCR_FIA_L19O                                (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_PCR_FIA_L20O                                (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_PCR_FIA_L21O                                (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_PCR_FIA_L22O                                (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_PCR_FIA_L23O                                (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_PCR_FIA_L24O                                (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_PCR_FIA_L25O                                (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_PCR_FIA_L26O                                (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_PCR_FIA_L27O                                (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_PCR_FIA_L28O                                (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_PCR_FIA_L29O                                (BIT23 | BIT22 | BIT21 | BIT20)
#define PCH_H_FIA_DMILANE_START                           14
#define PCH_H_FIA_DMILANE_END                             17
#define PCH_FIA_MAX_LANES                                 34

#endif
