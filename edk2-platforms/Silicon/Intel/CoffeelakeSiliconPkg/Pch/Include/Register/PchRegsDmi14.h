/** @file
  Register names for DMI SIP14

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

#ifndef _PCH_REGS_DMI14_H_
#define _PCH_REGS_DMI14_H_

//
// DMI Chipset Configuration Registers (PID:DMI)
//

//
// DMI Control
//
#define R_PCH_DMI14_PCR_DMIC                   0x2234                              ///< DMI Control
#define B_PCH_DMI14_PCR_DMIC_SRL               BIT31                               ///< Secured register lock
#define B_PCH_DMI14_PCR_DMIC_DMICGEN           (BIT4 | BIT3 | BIT2 | BIT1 | BIT0)  ///< DMI Clock Gate Enable

#define R_PCH_DMI14_PCR_2314                   0x2314

#endif
