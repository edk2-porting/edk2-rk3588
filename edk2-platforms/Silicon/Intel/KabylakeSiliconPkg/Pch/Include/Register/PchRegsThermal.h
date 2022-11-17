/** @file
  Register names for PCH Thermal Device

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
#ifndef _PCH_REGS_THERMAL_H_
#define _PCH_REGS_THERMAL_H_

//
//  Thermal Device Registers (D20:2)
//
#define PCI_DEVICE_NUMBER_PCH_THERMAL   20
#define PCI_FUNCTION_NUMBER_PCH_THERMAL 2

#define R_PCH_THERMAL_TBAR              0x10
#define V_PCH_THERMAL_TBAR_SIZE         (4 * 1024)
#define N_PCH_THREMAL_TBAR_ALIGNMENT    12
#define B_PCH_THERMAL_TBAR_MASK         0xFFFFF000
#define R_PCH_THERMAL_TBARH             0x14
#define R_PCH_THERMAL_TBARB             0x40
#define V_PCH_THERMAL_TBARB_SIZE        (4 * 1024)
#define N_PCH_THREMAL_TBARB_ALIGNMENT   12
#define B_PCH_THERMAL_SPTYPEN           BIT0
#define R_PCH_THERMAL_TBARBH            0x44
#define B_PCH_THERMAL_TBARB_MASK        0xFFFFF000

//
//  Thermal TBAR MMIO registers
//
#define R_PCH_TBAR_TSC                  0x04
#define B_PCH_TBAR_TSC_PLD              BIT7
#define B_PCH_TBAR_TSC_CPDE             BIT0
#define R_PCH_TBAR_TSS                  0x06
#define R_PCH_TBAR_TSEL                 0x08
#define B_PCH_TBAR_TSEL_PLD             BIT7
#define B_PCH_TBAR_TSEL_ETS             BIT0
#define R_PCH_TBAR_TSREL                0x0A
#define R_PCH_TBAR_TSMIC                0x0C
#define B_PCH_TBAR_TSMIC_PLD            BIT7
#define B_PCH_TBAR_TSMIC_SMIE           BIT0
#define R_PCH_TBAR_CTT                  0x10
#define R_PCH_TBAR_TAHV                 0x14
#define R_PCH_TBAR_TALV                 0x18
#define R_PCH_TBAR_TSPM                 0x1C
#define B_PCH_TBAR_TSPM_LTT             (BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define V_PCH_TBAR_TSPM_LTT             0x0C8
#define B_PCH_TBAR_TSPM_MAXTSST         (BIT11 | BIT10 | BIT9)
#define V_PCH_TBAR_TSPM_MAXTSST         (0x4 << 9)
#define B_PCH_TBAR_TSPM_MINTSST         BIT12
#define B_PCH_TBAR_TSPM_DTSSIC0         BIT13
#define B_PCH_TBAR_TSPM_DTSSS0EN        BIT14
#define B_PCH_TBAR_TSPM_TSPMLOCK        BIT15
#define R_PCH_TBAR_TL                   0x40
#define B_PCH_TBAR_TL_LOCK              BIT31
#define B_PCH_TBAR_TL_TTEN              BIT29
#define R_PCH_TBAR_TL2                  0x50
#define R_PCH_TBAR_TL2_LOCK             BIT15
#define R_PCH_TBAR_TL2_PMCTEN           BIT14
#define R_PCH_TBAR_PHL                  0x60
#define B_PCH_TBAR_PHLE                 BIT15
#define R_PCH_TBAR_PHLC                 0x62
#define B_PCH_TBAR_PHLC_LOCK            BIT0
#define R_PCH_TBAR_TAS                  0x80
#define R_PCH_TBAR_TSPIEN               0x82
#define R_PCH_TBAR_TSGPEN               0x84
#define B_PCH_TBAR_TL2_PMCTEN           BIT14
#define R_PCH_TBAR_A4                   0xA4
#define R_PCH_TBAR_C0                   0xC0
#define R_PCH_TBAR_C4                   0xC4
#define R_PCH_TBAR_C8                   0xC8
#define R_PCH_TBAR_CC                   0xCC
#define R_PCH_TBAR_D0                   0xD0
#define R_PCH_TBAR_E0                   0xE0
#define R_PCH_TBAR_E4                   0xE4
#define R_PCH_TBAR_E8                   0xE8
#define R_PCH_TBAR_TCFD                 0xF0                ///< Thermal controller function disable
#define B_PCH_TBAR_TCFD_TCD             BIT0                ///< Thermal controller disable

#endif
