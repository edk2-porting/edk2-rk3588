/** @file
  Register names for PCH LAN device

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
#ifndef _PCH_REGS_LAN_H_
#define _PCH_REGS_LAN_H_

//
// Gigabit LAN Controller configuration registers (D31:F6)
//
#define PCI_DEVICE_NUMBER_PCH_LAN     31
#define PCI_FUNCTION_NUMBER_PCH_LAN   6

#define R_PCH_LAN_MBARA               0x10
#define B_PCH_LAN_MBARA_BA            0xFFFE0000
#define N_PCH_LAN_MBARA_ALIGN         17
#define R_PCH_LAN_LTR_CAP             0xA8
#define R_PCH_LAN_CLIST1              0xC8
#define B_PCH_LAN_CLIST1_NEXT         0xFF00
#define B_PCH_LAN_CLIST1_CID          0x00FF
#define R_PCH_LAN_PMC                 0xCA
#define B_PCH_LAN_PMC_PMES            0xF800
#define B_PCH_LAN_PMC_D2S             BIT10
#define B_PCH_LAN_PMC_D1S             BIT9
#define B_PCH_LAN_PMC_AC              (BIT8 | BIT7 | BIT6)
#define B_PCH_LAN_PMC_DSI             BIT5
#define B_PCH_LAN_PMC_PMEC            BIT3
#define B_PCH_LAN_PMC_VS              (BIT2 | BIT1 | BIT0)
#define R_PCH_LAN_PMCS                0xCC
#define B_PCH_LAN_PMCS_PMES           BIT15
#define B_PCH_LAN_PMCS_DSC            (BIT14 | BIT13)
#define B_PCH_LAN_PMCS_DSL            0x1E00
#define V_PCH_LAN_PMCS_DSL0           0x0000
#define V_PCH_LAN_PMCS_DSL3           0x0600
#define V_PCH_LAN_PMCS_DSL4           0x0800
#define V_PCH_LAN_PMCS_DSL7           0x0E00
#define V_PCH_LAN_PMCS_DSL8           0x1000
#define B_PCH_LAN_PMCS_PMEE           BIT8
#define B_PCH_LAN_PMCS_PS             (BIT1 | BIT0)
#define V_PCH_LAN_PMCS_PS0            0x00
#define V_PCH_LAN_PMCS_PS3            0x03
#define R_PCH_LAN_DR                  0xCF
#define B_PCH_LAN_DR                  0xFF
#define R_PCH_LAN_CLIST2              0xD0
#define B_PCH_LAN_CLIST2_NEXT         0xFF00
#define B_PCH_LAN_CLIST2_CID          0x00FF
#define R_PCH_LAN_MCTL                0xD2
#define B_PCH_LAN_MCTL_CID            BIT7
#define B_PCH_LAN_MCTL_MME            (BIT6 | BIT5 | BIT4)
#define B_PCH_LAN_MCTL_MMC            (BIT3 | BIT2 | BIT1)
#define B_PCH_LAN_MCTL_MSIE           BIT0
#define R_PCH_LAN_MADDL               0xD4
#define B_PCH_LAN_MADDL               0xFFFFFFFF
#define R_PCH_LAN_MADDH               0xD8
#define B_PCH_LAN_MADDH               0xFFFFFFFF
#define R_PCH_LAN_MDAT                0xDC
#define B_PCH_LAN_MDAT                0xFFFFFFFF
#define R_PCH_LAN_FLRCAP              0xE0
#define B_PCH_LAN_FLRCAP_NEXT         0xFF00
#define B_PCH_LAN_FLRCAP_CID          0x00FF
#define V_PCH_LAN_FLRCAP_CID_SSEL0    0x13
#define V_PCH_LAN_FLRCAP_CID_SSEL1    0x09
#define R_PCH_LAN_FLRCLV              0xE2
#define B_PCH_LAN_FLRCLV_FLRC_SSEL0   BIT9
#define B_PCH_LAN_FLRCLV_TXP_SSEL0    BIT8
#define B_PCH_LAN_FLRCLV_VSCID_SSEL1  0xF000
#define B_PCH_LAN_FLRCLV_CAPVER_SSEL1 0x0F00
#define B_PCH_LAN_FLRCLV_CAPLNG       0x00FF
#define R_PCH_LAN_DEVCTRL             0xE4
#define B_PCH_LAN_DEVCTRL             BIT0
#define R_PCH_LAN_CPCE                0x80
#define B_PCH_LAN_CPCE_HAE            BIT5
#define B_PCH_LAN_CPCE_SE             BIT3
#define B_PCH_LAN_CPCE_D3HE           BIT2
#define B_PCH_LAN_CPCE_I3E            BIT1
#define B_PCH_LAN_CPCE_PCMCRE         BIT0
#define R_PCH_LAN_CD0I3               0x84
#define B_PCH_LAN_CD0I3_RR            BIT3
#define B_PCH_LAN_CD0I3_D0I3          BIT2
#define R_PCH_LAN_CLCTL               0x94
#define R_PCH_LAN_LANDISCTRL          0xA0
#define B_PCH_LAN_LANDISCTRL_DISABLE  BIT0
#define R_PCH_LAN_LOCKLANDIS          0xA4
#define B_PCH_LAN_LOCKLANDIS_LOCK     BIT0
//
// Gigabit LAN Capabilities and Status Registers (Memory space)
//
#define R_PCH_LAN_CSR_CTRL                 0
#define B_PCH_LAN_CSR_CTRL_MEHE            BIT19
#define R_PCH_LAN_CSR_STRAP                0x000C
#define B_PCH_LAN_CSR_STRAP_NVM_VALID      BIT11
#define R_PCH_LAN_CSR_FEXTNVM6             0x0010
#define R_PCH_LAN_CSR_CTRL_EXT             0x0018
#define B_PCH_LAN_CSR_CTRL_EXT_FORCE_SMB   BIT11
#define R_PCH_LAN_CSR_MDIC                 0x0020
#define B_PCH_LAN_CSR_MDIC_RB              BIT28
#define B_PCH_LAN_CSR_MDIC_DATA            0xFFFF
#define R_PCH_LAN_CSR_FEXT                 0x002C
#define B_PCH_LAN_CSR_FEXT_WOL             BIT30
#define B_PCH_LAN_CSR_FEXT_WOL_VALID       BIT31
#define R_PCH_LAN_CSR_EXTCNF_CTRL          0x0F00
#define B_PCH_LAN_CSR_EXTCNF_CTRL_SWFLAG   BIT5
#define B_PCH_LAN_CSR_EXTCNF_K1OFF_EN      BIT8
#define R_PCH_LAN_CSR_PHY_CTRL             0x0F10
#define B_PCH_LAN_CSR_PHY_CTRL_GGD         BIT6
#define B_PCH_LAN_CSR_PHY_CTRL_GBEDIS      BIT3
#define B_PCH_LAN_CSR_PHY_CTRL_LPLUND      BIT2
#define B_PCH_LAN_CSR_PHY_CTRL_LPLUD       BIT1
#define R_PCH_LAN_CSR_F18                  0x0F18
#define B_PCH_LAN_CSR_F18_K1OFF_EN         BIT31
#define R_PCH_LAN_CSR_PBECCSTS             0x100C
#define B_PCH_LAN_CSR_PBECCSTS_ECC_EN      BIT16
#define R_PCH_LAN_CSR_RAL                  0x5400
#define R_PCH_LAN_CSR_RAH                  0x5404
#define B_PCH_LAN_CSR_RAH_RAH              0x0000FFFF
#define R_PCH_LAN_CSR_WUC                  0x5800
#define B_PCH_LAN_CSR_WUC_APME             BIT0

#endif
