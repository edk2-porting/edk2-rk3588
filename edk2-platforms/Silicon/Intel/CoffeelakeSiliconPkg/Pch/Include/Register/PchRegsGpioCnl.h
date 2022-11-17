/** @file
  Register names for GPIO

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

#ifndef _PCH_REGS_GPIO_CNL_H_
#define _PCH_REGS_GPIO_CNL_H_

//
// PCH-LP GPIO
//
#define CNL_PCH_LP_GPIO_GROUP_MAX             15

#define CNL_PCH_LP_GPIO_GPP_A_PAD_MAX         25
#define CNL_PCH_LP_GPIO_GPP_B_PAD_MAX         26
#define CNL_PCH_LP_GPIO_GPP_C_PAD_MAX         24
#define CNL_PCH_LP_GPIO_GPP_D_PAD_MAX         24
#define CNL_PCH_LP_GPIO_GPP_E_PAD_MAX         24
#define CNL_PCH_LP_GPIO_GPP_F_PAD_MAX         24
#define CNL_PCH_LP_GPIO_GPP_G_PAD_MAX         8
#define CNL_PCH_LP_GPIO_GPP_H_PAD_MAX         24
#define CNL_PCH_LP_GPIO_GPD_PAD_MAX           16
#define CNL_PCH_LP_GPIO_VGPIO_PAD_MAX         40
#define CNL_PCH_LP_GPIO_SPI_PAD_MAX           9
#define CNL_PCH_LP_GPIO_AZA_PAD_MAX           8
#define CNL_PCH_LP_GPIO_CPU_PAD_MAX           11
#define CNL_PCH_LP_GPIO_JTAG_PAD_MAX          9
#define CNL_PCH_LP_GPIO_HVMOS_PAD_MAX         6

//
// PCH-H GPIO
//
#define CNL_PCH_H_GPIO_GROUP_MAX              17

#define CNL_PCH_H_GPIO_GPP_A_PAD_MAX          25
#define CNL_PCH_H_GPIO_GPP_B_PAD_MAX          26
#define CNL_PCH_H_GPIO_GPP_C_PAD_MAX          24
#define CNL_PCH_H_GPIO_GPP_D_PAD_MAX          24
#define CNL_PCH_H_GPIO_GPP_E_PAD_MAX          13
#define CNL_PCH_H_GPIO_GPP_F_PAD_MAX          24
#define CNL_PCH_H_GPIO_GPP_G_PAD_MAX          8
#define CNL_PCH_H_GPIO_GPP_H_PAD_MAX          24
#define CNL_PCH_H_GPIO_GPP_I_PAD_MAX          18
#define CNL_PCH_H_GPIO_GPP_J_PAD_MAX          12
#define CNL_PCH_H_GPIO_GPP_K_PAD_MAX          24
#define CNL_PCH_H_GPIO_GPD_PAD_MAX            16
#define CNL_PCH_H_GPIO_VGPIO_PAD_MAX          40
#define CNL_PCH_H_GPIO_SPI_PAD_MAX            9
#define CNL_PCH_H_GPIO_AZA_PAD_MAX            8
#define CNL_PCH_H_GPIO_CPU_PAD_MAX            11
#define CNL_PCH_H_GPIO_JTAG_PAD_MAX           9

//
// PCH-LP GPIO registers
//
//
// GPIO Community Common Private Configuration Registers
//
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_A     0x0
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_B     0x1
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_C     0xC
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_D     0x4
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_E     0xD
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_F     0x5
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_G     0x2
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPP_H     0x6
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_GPD       0x9
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_VGPIO     0x7
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_SPI       0x3
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_AZA       0xA
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_CPU       0xB
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_JTAG      0xE
#define V_CNL_PCH_LP_GPIO_PCR_MISCCFG_GPE0_HVMOS     0xF

//
// GPIO Community 0 Private Configuration Registers
//
#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_PAD_OWN        0x20
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_PAD_OWN        0x30
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_PAD_OWN        0x40
#define R_CNL_PCH_LP_GPIO_PCR_SPI_PAD_OWN          0x44

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_PADCFGLOCK     0x80
#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_PADCFGLOCKTX   0x84
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_PADCFGLOCK     0x88
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_PADCFGLOCKTX   0x8C
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_PADCFGLOCK     0x90
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_PADCFGLOCKTX   0x94
#define R_CNL_PCH_LP_GPIO_PCR_SPI_PADCFGLOCK       0x98
#define R_CNL_PCH_LP_GPIO_PCR_SPI_PADCFGLOCKTX     0x9C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_HOSTSW_OWN     0xB0
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_HOSTSW_OWN     0xB4
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_HOSTSW_OWN     0xB8
#define R_CNL_PCH_LP_GPIO_PCR_SPI_HOSTSW_OWN       0xBC

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_GPI_IS         0x0100
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_GPI_IS         0x0104
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_GPI_IS         0x0108
#define R_CNL_PCH_LP_GPIO_PCR_SPI_GPI_IS           0x010C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_GPI_IE         0x0120
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_GPI_IE         0x0124
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_GPI_IE         0x0128
#define R_CNL_PCH_LP_GPIO_PCR_SPI_GPI_IE           0x012C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_GPI_GPE_STS    0x0140
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_GPI_GPE_STS    0x0144
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_GPI_GPE_STS    0x0148
#define R_CNL_PCH_LP_GPIO_PCR_SPI_GPI_GPE_STS      0x014C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_GPI_GPE_EN     0x0160
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_GPI_GPE_EN     0x0164
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_GPI_GPE_EN     0x0168
#define R_CNL_PCH_LP_GPIO_PCR_SPI_GPI_GPE_EN       0x016C

//#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_SMI_STS        0x0180  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_SMI_STS        0x0184
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_SMI_STS        0x0188  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_SPI_SMI_STS          0x018C  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_SMI_EN         0x01A0  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_SMI_EN         0x01A4
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_SMI_EN         0x01A8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_SPI_SMI_EN           0x01AC  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_NMI_STS        0x01C0  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_NMI_STS        0x01C4
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_NMI_STS        0x01C8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_SPI_NMI_STS          0x01CC  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_NMI_EN         0x01E0  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_NMI_EN         0x01E4
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_NMI_EN         0x01E8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_SPI_NMI_EN           0x01EC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_A_PADCFG_OFFSET  0x600
#define R_CNL_PCH_LP_GPIO_PCR_GPP_B_PADCFG_OFFSET  0x790
#define R_CNL_PCH_LP_GPIO_PCR_GPP_G_PADCFG_OFFSET  0x930
#define R_CNL_PCH_LP_GPIO_PCR_SPI_PADCFG_OFFSET    0x9B0

//
// GPIO Community 1 Private Configuration Registers
//
#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_PAD_OWN        0x20
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_PAD_OWN        0x30
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_PAD_OWN        0x3C
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_PAD_OWN        0x48

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_PADCFGLOCK     0x80
#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_PADCFGLOCKTX   0x84
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_PADCFGLOCK     0x88
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_PADCFGLOCKTX   0x8C
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_PADCFGLOCK     0x90
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_PADCFGLOCKTX   0x94
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_0_PADCFGLOCK   0x98
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_0_PADCFGLOCKTX 0x9C
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_1_PADCFGLOCK   0xA0
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_1_PADCFGLOCKTX 0xA4

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_HOSTSW_OWN     0xB0
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_HOSTSW_OWN     0xB4
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_HOSTSW_OWN     0xB8
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_HOSTSW_OWN     0xBC

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_GPI_IS         0x0100
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_GPI_IS         0x0104
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_GPI_IS         0x0108
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_GPI_IS         0x010C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_GPI_IE         0x0120
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_GPI_IE         0x0124
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_GPI_IE         0x0128
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_GPI_IE         0x012C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_GPI_GPE_STS    0x0140
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_GPI_GPE_STS    0x0144
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_GPI_GPE_STS    0x0148
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_GPI_GPE_STS    0x014C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_GPI_GPE_EN     0x0160
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_GPI_GPE_EN     0x0164
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_GPI_GPE_EN     0x0168
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_GPI_GPE_EN     0x016C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_SMI_STS        0x0180
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_SMI_STS        0x0184  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_SMI_STS        0x0188  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_SMI_STS        0x018C  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_SMI_EN         0x01A0
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_SMI_EN         0x01A4  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_SMI_EN         0x01A8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_SMI_EN         0x01AC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_NMI_STS        0x01C0
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_NMI_STS        0x01C4  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_NMI_STS        0x01C8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_NMI_STS        0x01CC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_NMI_EN         0x01E0
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_NMI_EN         0x01E4  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_NMI_EN         0x01E8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_NMI_EN         0x01EC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_D_PADCFG_OFFSET  0x600
#define R_CNL_PCH_LP_GPIO_PCR_GPP_F_PADCFG_OFFSET  0x790
#define R_CNL_PCH_LP_GPIO_PCR_GPP_H_PADCFG_OFFSET  0x910
#define R_CNL_PCH_LP_GPIO_PCR_VGPIO_PADCFG_OFFSET  0xA90

//
// GPIO Community 2 Private Configuration Registers
//
#define R_CNL_PCH_LP_GPIO_PCR_GPD_PAD_OWN          0x20

#define R_CNL_PCH_LP_GPIO_PCR_GPD_PADCFGLOCK       0x80
#define R_CNL_PCH_LP_GPIO_PCR_GPD_PADCFGLOCKTX     0x84

#define R_CNL_PCH_LP_GPIO_PCR_GPD_HOSTSW_OWN       0xB0

#define R_CNL_PCH_LP_GPIO_PCR_GPD_GPI_IS           0x0100
#define R_CNL_PCH_LP_GPIO_PCR_GPD_GPI_IE           0x0120

#define R_CNL_PCH_LP_GPIO_PCR_GPD_GPI_GPE_STS      0x0140
#define R_CNL_PCH_LP_GPIO_PCR_GPD_GPI_GPE_EN       0x0160

//#define R_CNL_PCH_LP_GPIO_PCR_GPD_SMI_STS        0x0180  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_GPD_SMI_EN         0x01A0  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_GPD_NMI_STS        0x01C0  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_GPD_NMI_EN         0x01E0  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPD_PADCFG_OFFSET    0x600

//
// GPIO Community 3 Private Configuration Registers
//
#define R_CNL_PCH_LP_GPIO_PCR_AZA_PAD_OWN          0x20
#define R_CNL_PCH_LP_GPIO_PCR_CPU_PAD_OWN          0x24

#define R_CNL_PCH_LP_GPIO_PCR_AZA_PADCFGLOCK       0x80
#define R_CNL_PCH_LP_GPIO_PCR_AZA_PADCFGLOCKTX     0x84
#define R_CNL_PCH_LP_GPIO_PCR_CPU_PADCFGLOCK       0x88
#define R_CNL_PCH_LP_GPIO_PCR_CPU_PADCFGLOCKTX     0x8C

#define R_CNL_PCH_LP_GPIO_PCR_AZA_HOSTSW_OWN       0xB0
#define R_CNL_PCH_LP_GPIO_PCR_CPU_HOSTSW_OWN       0xB4

#define R_CNL_PCH_LP_GPIO_PCR_AZA_GPI_IS           0x0100
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_GPI_IS           0x0104  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_AZA_GPI_IE           0x0120
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_GPI_IE           0x0124  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_AZA_GPI_GPE_STS      0x0140
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_GPI_GPE_STS      0x0144  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_AZA_GPI_GPE_EN       0x0160
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_GPI_GPE_EN       0x0164  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_AZA_SMI_STS          0x0180  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_SMI_STS          0x0184  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_AZA_SMI_EN           0x01A0  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_SMI_EN           0x01A4  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_AZA_NMI_STS          0x01C0  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_NMI_STS          0x01C4  // Not supported setting for this group

//#define R_CNL_PCH_LP_GPIO_PCR_AZA_NMI_EN           0x01E0  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_CPU_NMI_EN           0x01E4  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_AZA_PADCFG_OFFSET    0x600
#define R_CNL_PCH_LP_GPIO_PCR_CPU_PADCFG_OFFSET    0x680

//
// GPIO Community 4 Private Configuration Registers
//
#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_PAD_OWN        0x20
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_PAD_OWN        0x2C
#define R_CNL_PCH_LP_GPIO_PCR_JTAG_PAD_OWN         0x38
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_PAD_OWN        0x40

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_PADCFGLOCK     0x80
#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_PADCFGLOCKTX   0x84
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_PADCFGLOCK     0x88
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_PADCFGLOCKTX   0x8C
#define R_CNL_PCH_LP_GPIO_PCR_JTAG_PADCFGLOCK      0x90
#define R_CNL_PCH_LP_GPIO_PCR_JTAG_PADCFGLOCKTX    0x94
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_PADCFGLOCK     0x98
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_PADCFGLOCKTX   0x9C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_HOSTSW_OWN     0xB0
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_HOSTSW_OWN     0xB4
#define R_CNL_PCH_LP_GPIO_PCR_JTAG_HOSTSW_OWN      0xB8
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_HOSTSW_OWN     0xBC

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_GPI_IS         0x0100
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_GPI_IS         0x0104
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_GPI_IS          0x0108  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_GPI_IS         0x010C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_GPI_IE         0x0120
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_GPI_IE         0x0124
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_GPI_IE          0x0128  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_GPI_IE         0x012C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_GPI_GPE_STS    0x0140
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_GPI_GPE_STS    0x0144
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_GPI_GPE_STS     0x0148  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_GPI_GPE_STS    0x014C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_GPI_GPE_EN     0x0160
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_GPI_GPE_EN     0x0164
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_GPI_GPE_EN      0x0168  // Not supported setting for this group
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_GPI_GPE_EN     0x016C

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_SMI_STS        0x0180
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_SMI_STS        0x0184
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_SMI_STS         0x0188  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_SMI_STS        0x018C  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_SMI_EN         0x01A0
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_SMI_EN         0x01A4
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_SMI_EN          0x01A8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_SMI_EN         0x01AC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_NMI_STS        0x01C0
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_NMI_STS        0x01C4
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_NMI_STS         0x01C8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_NMI_STS        0x01CC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_NMI_EN         0x01E0
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_NMI_EN         0x01E4
//#define R_CNL_PCH_LP_GPIO_PCR_JTAG_NMI_EN          0x01E8  // Not supported setting for this group
//#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_NMI_EN         0x01EC  // Not supported setting for this group

#define R_CNL_PCH_LP_GPIO_PCR_GPP_C_PADCFG_OFFSET  0x600
#define R_CNL_PCH_LP_GPIO_PCR_GPP_E_PADCFG_OFFSET  0x780
#define R_CNL_PCH_LP_GPIO_PCR_JTAG_PADCFG_OFFSET   0x900
#define R_CNL_PCH_LP_GPIO_PCR_HVMOS_PADCFG_OFFSET  0x990

//
// PCH-H GPIO registers
//
//
// GPIO Community Common Private Configuration Registers
//
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_A      0x0
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_B      0x1
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_C      0x2
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_D      0x3
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_E      0x6
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_F      0x7
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_G      0x4
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_H      0x8
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_K      0x9
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_I      0xA
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_J      0xB
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPD        0x5
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_VGPIO      0xD

//
// GPIO Community 0 Private Configuration Registers
//
#define R_CNL_PCH_H_GPIO_PCR_GPP_A_PAD_OWN         0x20
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_PAD_OWN         0x30

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_PADCFGLOCK      0x80
#define R_CNL_PCH_H_GPIO_PCR_GPP_A_PADCFGLOCKTX    0x84
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_PADCFGLOCK      0x88
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_PADCFGLOCKTX    0x8C

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_HOSTSW_OWN      0xC0
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_HOSTSW_OWN      0xC4

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_GPI_IS          0x0100
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_GPI_IS          0x0104

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_GPI_IE          0x0120
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_GPI_IE          0x0124

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_GPI_GPE_STS     0x0140
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_GPI_GPE_STS     0x0144

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_GPI_GPE_EN      0x0160
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_GPI_GPE_EN      0x0164

//#define R_CNL_PCH_H_GPIO_PCR_GPP_A_SMI_STS         0x0180  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_SMI_STS         0x0184

//#define R_CNL_PCH_H_GPIO_PCR_GPP_A_SMI_EN          0x01A0  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_SMI_EN          0x01A4

//#define R_CNL_PCH_H_GPIO_PCR_GPP_A_NMI_STS         0x01C0  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_NMI_STS         0x01C4

//#define R_CNL_PCH_H_GPIO_PCR_GPP_A_NMI_EN          0x01E0  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_NMI_EN          0x01E4

#define R_CNL_PCH_H_GPIO_PCR_GPP_A_PADCFG_OFFSET   0x600
#define R_CNL_PCH_H_GPIO_PCR_GPP_B_PADCFG_OFFSET   0x790

//
// GPIO Community 1 Private Configuration Registers
//
#define R_CNL_PCH_H_GPIO_PCR_GPP_C_PAD_OWN         0x20
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_PAD_OWN         0x2C
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_PAD_OWN         0x38
#define R_CNL_PCH_H_GPIO_PCR_AZA_PAD_OWN           0x3C
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_PAD_OWN         0x40

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_PADCFGLOCK      0x80
#define R_CNL_PCH_H_GPIO_PCR_GPP_C_PADCFGLOCKTX    0x84
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_PADCFGLOCK      0x88
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_PADCFGLOCKTX    0x8C
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_PADCFGLOCK      0x90
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_PADCFGLOCKTX    0x94
#define R_CNL_PCH_H_GPIO_PCR_AZA_PADCFGLOCK        0x98
#define R_CNL_PCH_H_GPIO_PCR_AZA_PADCFGLOCKTX      0x9C
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_0_PADCFGLOCK    0xA0
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_0_PADCFGLOCKTX  0xA4
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_1_PADCFGLOCK    0xA8
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_1_PADCFGLOCKTX  0xAC

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_HOSTSW_OWN      0xC0
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_HOSTSW_OWN      0xC4
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_HOSTSW_OWN      0xC8
#define R_CNL_PCH_H_GPIO_PCR_AZA_HOSTSW_OWN        0xCC
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_HOSTSW_OWN      0xD0

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_GPI_IS          0x0100
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_GPI_IS          0x0104
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_GPI_IS          0x0108
//#define R_CNL_PCH_H_GPIO_PCR_AZA_GPI_IS            0x010C  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_GPI_IS          0x0110

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_GPI_IE          0x0120
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_GPI_IE          0x0124
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_GPI_IE          0x0128
//#define R_CNL_PCH_H_GPIO_PCR_AZA_GPI_IE            0x012C  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_GPI_IE          0x0130

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_GPI_GPE_STS     0x0140
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_GPI_GPE_STS     0x0144
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_GPI_GPE_STS     0x0148
//#define R_CNL_PCH_H_GPIO_PCR_AZA_GPI_GPE_STS       0x014C  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_GPI_GPE_STS     0x0150

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_GPI_GPE_EN      0x0160
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_GPI_GPE_EN      0x0164
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_GPI_GPE_EN      0x0168
//#define R_CNL_PCH_H_GPIO_PCR_AZA_GPI_GPE_EN        0x016C  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_GPI_GPE_EN      0x0170

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_SMI_STS         0x0180
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_SMI_STS         0x0184
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_SMI_STS         0x0188
//#define R_CNL_PCH_H_GPIO_PCR_AZA_SMI_STS           0x018C  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_VGPIO_SMI_STS         0x0190  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_SMI_EN          0x01A0
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_SMI_EN          0x01A4
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_SMI_EN          0x01A8
//#define R_CNL_PCH_H_GPIO_PCR_AZA_SMI_EN            0x01AC  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_VGPIO_SMI_EN          0x01B0  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_NMI_STS         0x01C0
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_NMI_STS         0x01C4
//#define R_CNL_PCH_H_GPIO_PCR_GPP_G_NMI_STS         0x01C8  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_AZA_NMI_STS           0x01CC  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_VGPIO_NMI_STS         0x01D0  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_NMI_EN          0x01E0
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_NMI_EN          0x01E4
//#define R_CNL_PCH_H_GPIO_PCR_GPP_G_NMI_EN          0x01E8  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_AZA_NMI_EN            0x01EC  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_VGPIO_NMI_EN          0x01F0  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_C_PADCFG_OFFSET   0x600
#define R_CNL_PCH_H_GPIO_PCR_GPP_D_PADCFG_OFFSET   0x780
#define R_CNL_PCH_H_GPIO_PCR_GPP_G_PADCFG_OFFSET   0x900
#define R_CNL_PCH_H_GPIO_PCR_AZA_PADCFG_OFFSET     0x980
#define R_CNL_PCH_H_GPIO_PCR_VGPIO_PADCFG_OFFSET   0xA00

//
// GPIO Community 2 Private Configuration Registers
//

#define R_CNL_PCH_H_GPIO_PCR_GPD_PAD_OWN           0x20

#define R_CNL_PCH_H_GPIO_PCR_GPD_PADCFGLOCK        0x80
#define R_CNL_PCH_H_GPIO_PCR_GPD_PADCFGLOCKTX      0x84

#define R_CNL_PCH_H_GPIO_PCR_GPD_HOSTSW_OWN        0xB0

#define R_CNL_PCH_H_GPIO_PCR_GPD_GPI_IS            0x0100

#define R_CNL_PCH_H_GPIO_PCR_GPD_GPI_IE            0x0120

#define R_CNL_PCH_H_GPIO_PCR_GPD_GPI_GPE_STS       0x0140

#define R_CNL_PCH_H_GPIO_PCR_GPD_GPI_GPE_EN        0x0160

//#define R_CNL_PCH_H_GPIO_PCR_GPD_SMI_STS         0x0180  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPD_SMI_EN          0x01A0  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPD_NMI_STS         0x01C0  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPD_NMI_EN          0x01E0  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPD_PADCFG_OFFSET     0x600

//
// GPIO Community 3 Private Configuration Registers
//
#define R_CNL_PCH_H_GPIO_PCR_GPP_K_PAD_OWN         0x20
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_PAD_OWN         0x2C
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_PAD_OWN         0x38
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_PAD_OWN         0x40
#define R_CNL_PCH_H_GPIO_PCR_SPI_PAD_OWN           0x4C

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_PADCFGLOCK      0x80
#define R_CNL_PCH_H_GPIO_PCR_GPP_K_PADCFGLOCKTX    0x84
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_PADCFGLOCK      0x88
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_PADCFGLOCKTX    0x8C
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_PADCFGLOCK      0x90
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_PADCFGLOCKTX    0x94
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_PADCFGLOCK      0x98
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_PADCFGLOCKTX    0x9C
#define R_CNL_PCH_H_GPIO_PCR_SPI_PADCFGLOCK        0xA0
#define R_CNL_PCH_H_GPIO_PCR_SPI_PADCFGLOCKTX      0xA4

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_HOSTSW_OWN      0xC0
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_HOSTSW_OWN      0xC4
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_HOSTSW_OWN      0xC8
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_HOSTSW_OWN      0xCC
#define R_CNL_PCH_H_GPIO_PCR_SPI_HOSTSW_OWN        0xD0

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_GPI_IS          0x0100
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_GPI_IS          0x0104
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_GPI_IS          0x0108
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_GPI_IS          0x010C
//#define R_CNL_PCH_H_GPIO_PCR_SPI_GPI_IS            0x0110  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_GPI_IE          0x0120
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_GPI_IE          0x0124
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_GPI_IE          0x0128
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_GPI_IE          0x012C
//#define R_CNL_PCH_H_GPIO_PCR_SPI_GPI_IE            0x0130  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_GPI_GPE_STS     0x0140
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_GPI_GPE_STS     0x0144
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_GPI_GPE_STS     0x0148
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_GPI_GPE_STS     0x014C
//#define R_CNL_PCH_H_GPIO_PCR_SPI_GPI_GPE_STS       0x0150  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_GPI_GPE_EN      0x0160
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_GPI_GPE_EN      0x0164
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_GPI_GPE_EN      0x0168
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_GPI_GPE_EN      0x016C
//#define R_CNL_PCH_H_GPIO_PCR_SPI_GPI_GPE_EN        0x0170  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPP_K_SMI_STS         0x0180  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_GPP_H_SMI_STS         0x0184  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_SMI_STS         0x0188
//#define R_CNL_PCH_H_GPIO_PCR_GPP_F_SMI_STS         0x018C  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_SPI_SMI_STS           0x0190  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPP_K_SMI_EN          0x01A0  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_GPP_H_SMI_EN          0x01A4  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_SMI_EN          0x01A8
//#define R_CNL_PCH_H_GPIO_PCR_GPP_F_SMI_EN          0x01AC  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_SPI_SMI_EN            0x01B0  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPP_K_NMI_STS         0x01C0  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_GPP_H_NMI_STS         0x01C4  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_NMI_STS         0x01C8
//#define R_CNL_PCH_H_GPIO_PCR_GPP_F_NMI_STS         0x01CC  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_SPI_NMI_STS           0x01D0  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_GPP_K_NMI_EN          0x01E0  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_GPP_H_NMI_EN          0x01E4  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_NMI_EN          0x01E8
//#define R_CNL_PCH_H_GPIO_PCR_GPP_F_NMI_EN          0x01EC  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_SPI_NMI_EN            0x01F0  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_GPP_K_PADCFG_OFFSET   0x600
#define R_CNL_PCH_H_GPIO_PCR_GPP_H_PADCFG_OFFSET   0x780
#define R_CNL_PCH_H_GPIO_PCR_GPP_E_PADCFG_OFFSET   0x900
#define R_CNL_PCH_H_GPIO_PCR_GPP_F_PADCFG_OFFSET   0x9D0
#define R_CNL_PCH_H_GPIO_PCR_SPI_PADCFG_OFFSET     0xB50

//
// GPIO Community 4 Private Configuration Registers
//
#define R_CNL_PCH_H_GPIO_PCR_CPU_PAD_OWN           0x20
#define R_CNL_PCH_H_GPIO_PCR_JTAG_PAD_OWN          0x28
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_PAD_OWN         0x30
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_PAD_OWN         0x3C

#define R_CNL_PCH_H_GPIO_PCR_CPU_PADCFGLOCK        0x80
#define R_CNL_PCH_H_GPIO_PCR_CPU_PADCFGLOCKTX      0x84
#define R_CNL_PCH_H_GPIO_PCR_JTAG_PADCFGLOCK       0x88
#define R_CNL_PCH_H_GPIO_PCR_JTAG_PADCFGLOCKTX     0x8C
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_PADCFGLOCK      0x90
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_PADCFGLOCKTX    0x94
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_PADCFGLOCK      0x98
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_PADCFGLOCKTX    0x9C

#define R_CNL_PCH_H_GPIO_PCR_CPU_HOSTSW_OWN        0xC0
#define R_CNL_PCH_H_GPIO_PCR_JTAG_HOSTSW_OWN       0xC4
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_HOSTSW_OWN      0xC8
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_HOSTSW_OWN      0xCC

//#define R_CNL_PCH_H_GPIO_PCR_CPU_GPI_IS            0x0100  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_GPI_IS           0x0104  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_GPI_IS          0x0108
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_GPI_IS          0x010C

//#define R_CNL_PCH_H_GPIO_PCR_CPU_GPI_IE            0x0120  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_GPI_IE           0x0124  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_GPI_IE          0x0128
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_GPI_IE          0x012C

//#define R_CNL_PCH_H_GPIO_PCR_CPU_GPI_GPE_STS       0x0140  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_GPI_GPE_STS      0x0144  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_GPI_GPE_STS     0x0148
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_GPI_GPE_STS     0x014C

//#define R_CNL_PCH_H_GPIO_PCR_CPU_GPI_GPE_EN        0x0160  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_GPI_GPE_EN       0x0164  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_GPI_GPE_EN      0x0168
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_GPI_GPE_EN      0x016C

//#define R_CNL_PCH_H_GPIO_PCR_CPU_SMI_STS           0x0180  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_SMI_STS          0x0184  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_SMI_STS         0x0188
//#define R_CNL_PCH_H_GPIO_PCR_GPP_J_SMI_STS         0x018C  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_CPU_SMI_EN            0x01A0  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_SMI_EN           0x01A4  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_SMI_EN          0x01A8
//#define R_CNL_PCH_H_GPIO_PCR_GPP_J_SMI_EN          0x01AC  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_CPU_NMI_STS           0x01C0  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_NMI_STS          0x01C4  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_NMI_STS         0x01C8
//#define R_CNL_PCH_H_GPIO_PCR_GPP_J_NMI_STS         0x01CC  // Not supported setting for this group

//#define R_CNL_PCH_H_GPIO_PCR_CPU_NMI_EN            0x01E0  // Not supported setting for this group
//#define R_CNL_PCH_H_GPIO_PCR_JTAG_NMI_EN           0x01E4  // Not supported setting for this group
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_NMI_EN          0x01E8
//#define R_CNL_PCH_H_GPIO_PCR_GPP_J_NMI_EN          0x01EC  // Not supported setting for this group

#define R_CNL_PCH_H_GPIO_PCR_CPU_PADCFG_OFFSET     0x600
#define R_CNL_PCH_H_GPIO_PCR_JTAG_PADCFG_OFFSET    0x6B0
#define R_CNL_PCH_H_GPIO_PCR_GPP_I_PADCFG_OFFSET   0x740
#define R_CNL_PCH_H_GPIO_PCR_GPP_J_PADCFG_OFFSET   0x860

#endif
