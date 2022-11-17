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

#ifndef _PCH_REGS_PMC_CNL_H_
#define _PCH_REGS_PMC_CNL_H_

//
// PWRM Registers
//
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_A                        0x0
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_B                        0x1
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_C                        0xD
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_D                        0x4
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_E                        0xE
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_F                        0x5
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_G                        0x2
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_H                        0x6
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPD                          0xA
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_VGPIO                        0x7
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_SPI                          0x3
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_AZA                          0xB
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_JTAG                         0xF

#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_A                         0x0
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_B                         0x1
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_C                         0x2
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_D                         0x3
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_E                         0xA
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_F                         0xB
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_G                         0x4
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_H                         0x9
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_I                         0xC
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_J                         0xD
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_K                         0x8
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPD                           0x7
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_VGPIO                         0x5

#endif // _PCH_REGS_PMC_CNL_H_
