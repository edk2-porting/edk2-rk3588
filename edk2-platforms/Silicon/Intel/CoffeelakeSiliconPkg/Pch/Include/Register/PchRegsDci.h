/** @file
  Register names for PCH DCI device

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

#ifndef _PCH_REGS_DCI_H_
#define _PCH_REGS_DCI_H_

//
// DCI PCR Registers
//

#define R_DCI_PCR_ECTRL                       0x04            ///< DCI Control Register

#define B_DCI_PCR_ECTRL_HDCIEN_LOCK           BIT0            ///< Host DCI Enable Lock
#define B_DCI_PCR_ECTRL_HDCIEN                BIT4            ///< Host DCI Enable

#define R_DCI_PCR_ECKPWRCTL                   0x08            ///< DCI Power Control
// CNP-A0 (DCI Gen2) and backwards
#define R_DCI_PCR_PCE                         0x30            ///< DCI Power Control Enable Register
#define B_DCI_PCR_PCE_HAE                     BIT5            ///< Hardware Autonomous Enable
#define B_DCI_PCR_PCE_D3HE                    BIT2            ///< D3-Hot Enable
#define B_DCI_PCR_PCE_I3E                     BIT1            ///< I3 Enable
#define B_DCI_PCR_PCE_PMCRE                   BIT0            ///< PMC Request Enable
#endif
