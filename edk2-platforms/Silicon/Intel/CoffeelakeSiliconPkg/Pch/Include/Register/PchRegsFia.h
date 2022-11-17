/** @file
  Register definition for FIA component

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

#ifndef _PCH_REGS_FIA_H_
#define _PCH_REGS_FIA_H_


//
// Private chipset register (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PCH FIA lane owner encoding
//
#define V_PCH_FIA_PCR_LANE_OWN_PCIEDMI                    0x0
#define V_PCH_FIA_PCR_LANE_OWN_USB3                       0x1
#define V_PCH_FIA_PCR_LANE_OWN_SATA                       0x2
#define V_PCH_FIA_PCR_LANE_OWN_GBE                        0x3
#define V_PCH_FIA_PCR_LANE_OWN_MOBEXP                     0x4
#define V_PCH_FIA_PCR_LANE_OWN_SSIC                       0x5
#define V_PCH_FIA_PCR_LANE_OWN_CSI3                       0x6
#define V_PCH_FIA_PCR_LANE_OWN_UFS                        0x7

#define B_PCH_FIA_PCR_L0O                                 (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_L1O                                 (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_L2O                                 (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_L3O                                 (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_L4O                                 (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_L5O                                 (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_L6O                                 (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_L7O                                 (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_L8O                                 (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_L9O                                 (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_L10O                                (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_L11O                                (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_L12O                                (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_L13O                                (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_L14O                                (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_L15O                                (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_L16O                                (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_L17O                                (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_L18O                                (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_L19O                                (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_L20O                                (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_L21O                                (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_L22O                                (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_L23O                                (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_L24O                                (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_L25O                                (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_L26O                                (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_L27O                                (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_L28O                                (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_L29O                                (BIT23 | BIT22 | BIT21 | BIT20)

#endif
