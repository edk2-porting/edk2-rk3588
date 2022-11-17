/** @file
  Register names for VER2 GPIO

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
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
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

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_REGS_VER2_H_
#define _GPIO_REGS_VER2_H_

//
// PCH-LP GPIO
//
#define GPIO_VER2_PCH_LP_GPIO_GPP_B_PAD_MAX    26
#define GPIO_VER2_PCH_LP_GPIO_GPP_A_PAD_MAX    25
#define GPIO_VER2_PCH_LP_GPIO_GPP_R_PAD_MAX    8
#define GPIO_VER2_PCH_LP_GPIO_GPD_PAD_MAX      17
#define GPIO_VER2_PCH_LP_GPIO_GPP_S_PAD_MAX    8
#define GPIO_VER2_PCH_LP_GPIO_GPP_H_PAD_MAX    24
#define GPIO_VER2_PCH_LP_GPIO_GPP_D_PAD_MAX    21
#define GPIO_VER2_PCH_LP_GPIO_GPP_F_PAD_MAX    25
#define GPIO_VER2_PCH_LP_GPIO_GPP_C_PAD_MAX    24
#define GPIO_VER2_PCH_LP_GPIO_GPP_E_PAD_MAX    25
#define GPIO_VER2_PCH_LP_GPIO_CPU_PAD_MAX      15

//
// PCH-LP GPIO registers
//

//
// GPIO Community 0 Private Configuration Registers
//
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_PAD_OWN        0x20
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_PAD_OWN        0x38

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_PADCFGLOCK     0x80
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_PADCFGLOCKTX   0x84
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_PADCFGLOCK     0x90
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_PADCFGLOCKTX   0x94

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_HOSTSW_OWN     0xB0
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_HOSTSW_OWN     0xB8

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_GPI_IS         0x0100
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_GPI_IS         0x0108

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_GPI_IE         0x0120
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_GPI_IE         0x0128

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_GPI_GPE_STS    0x0140
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_GPI_GPE_STS    0x0148

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_GPI_GPE_EN     0x0160
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_GPI_GPE_EN     0x0168

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_SMI_STS        0x0180

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_SMI_EN         0x01A0

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_NMI_STS        0x01C0

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_NMI_EN         0x01E0

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_B_PADCFG_OFFSET  0x700
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_A_PADCFG_OFFSET  0x9A0

//
// GPIO Community 1 Private Configuration Registers
//
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_PAD_OWN        0x20
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_PAD_OWN        0x24
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_PAD_OWN        0x30

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_PADCFGLOCK     0x80
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_PADCFGLOCKTX   0x84
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_PADCFGLOCK     0x88
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_PADCFGLOCKTX   0x8C
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_PADCFGLOCK     0x90
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_PADCFGLOCKTX   0x94

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_HOSTSW_OWN     0xB0
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_HOSTSW_OWN     0xB4
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_HOSTSW_OWN     0xB8

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_GPI_IS         0x0100
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_GPI_IS         0x0104
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_GPI_IS         0x0108

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_GPI_IE         0x0120
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_GPI_IE         0x0124
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_GPI_IE         0x0128

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_GPI_GPE_STS    0x0140
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_GPI_GPE_STS    0x0144
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_GPI_GPE_STS    0x0148

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_GPI_GPE_EN     0x0160
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_GPI_GPE_EN     0x0164
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_GPI_GPE_EN     0x0168

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_SMI_STS        0x0188
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_SMI_EN         0x01A8
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_NMI_STS        0x01C8

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_NMI_EN         0x01E8

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_S_PADCFG_OFFSET  0x700
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_H_PADCFG_OFFSET  0x780
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_D_PADCFG_OFFSET  0x900

//
// GPIO Community 2 Private Configuration Registers
//
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_PAD_OWN          0x20

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_PADCFGLOCK       0x80
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_PADCFGLOCKTX     0x84

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_HOSTSW_OWN       0xB0

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_GPI_IS           0x0100
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_GPI_IE           0x0120

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_GPI_GPE_STS      0x0140
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_GPI_GPE_EN       0x0160

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPD_PADCFG_OFFSET    0x700

//
// GPIO Community 4 Private Configuration Registers
//
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_PAD_OWN        0x20
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_PAD_OWN        0x2C
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_PAD_OWN        0x40

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_PADCFGLOCK     0x80
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_PADCFGLOCKTX   0x84
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_PADCFGLOCK     0x88
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_PADCFGLOCKTX   0x8C
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_PADCFGLOCK     0x98
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_PADCFGLOCKTX   0x9C

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_HOSTSW_OWN     0xB0
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_HOSTSW_OWN     0xB4
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_HOSTSW_OWN     0xBC

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_GPI_IS         0x0100
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_GPI_IS         0x0104
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_GPI_IS         0x010C

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_GPI_IE         0x0120
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_GPI_IE         0x0124
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_GPI_IE         0x012C

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_GPI_GPE_STS    0x0140
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_GPI_GPE_STS    0x0144
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_GPI_GPE_STS    0x014C

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_GPI_GPE_EN     0x0160
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_GPI_GPE_EN     0x0164
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_GPI_GPE_EN     0x016C

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_SMI_STS        0x0180
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_SMI_STS        0x018C

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_SMI_EN         0x01A0
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_SMI_EN         0x01AC

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_NMI_STS        0x01C0
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_NMI_STS        0x01CC

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_NMI_EN         0x01E0
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_NMI_EN         0x01EC

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_C_PADCFG_OFFSET  0x700
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_F_PADCFG_OFFSET  0x880
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_E_PADCFG_OFFSET  0xA70

//
// GPIO Community 5 Private Configuration Registers
//
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_PAD_OWN        0x20

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_PADCFGLOCK     0x80
#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_PADCFGLOCKTX   0x84

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_HOSTSW_OWN     0xB0

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_GPI_IS         0x0100

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_GPI_IE         0x0120

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_GPI_GPE_STS    0x0140

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_GPI_GPE_EN     0x0160

#define R_GPIO_VER2_PCH_LP_GPIO_PCR_GPP_R_PADCFG_OFFSET  0x700

#endif // _GPIO_REGS_VER2_H_
