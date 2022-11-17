/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_REGS_GPIO_H_
#define _PCH_REGS_GPIO_H_

#define V_PCH_GPIO_GPP_A_PAD_MAX            24
#define V_PCH_GPIO_GPP_B_PAD_MAX            24
#define V_PCH_GPIO_GPP_C_PAD_MAX            24
#define V_PCH_GPIO_GPP_D_PAD_MAX            24
#define V_PCH_LP_GPIO_GPP_E_PAD_MAX         24
#define V_PCH_H_GPIO_GPP_E_PAD_MAX          13
#define V_PCH_GPIO_GPP_F_PAD_MAX            24
#define V_PCH_LP_GPIO_GPP_G_PAD_MAX         8
#define V_PCH_H_GPIO_GPP_G_PAD_MAX          24
#define V_PCH_H_GPIO_GPP_H_PAD_MAX          24
#define V_PCH_H_GPIO_GPP_J_PAD_MAX          24
#define V_PCH_H_GPIO_GPP_K_PAD_MAX          11
#define V_PCH_H_GPIO_GPP_L_PAD_MAX          20
#define V_PCH_H_GPIO_GPP_I_PAD_MAX          11

#define V_PCH_GPIO_GPD_PAD_MAX              12

#define V_PCH_GPIO_GROUP_MAX                13
#define V_PCH_H_GPIO_GROUP_MAX              V_PCH_GPIO_GROUP_MAX
#define V_PCH_LP_GPIO_GROUP_MAX             8
#define V_PCH_GPIO_NUM_SUPPORTED_GPIS       261
#define S_PCH_GPIO_GP_SMI_EN                4
#define S_PCH_GPIO_GP_SMI_STS               4

///
/// Groups mapped to 2-tier General Purpose Event will all be under
/// one master GPE_111 (0x6F)
///
#define PCH_GPIO_2_TIER_MASTER_GPE_NUMBER  0x6F


//
// GPIO Common Private Configuration Registers
//
#define R_PCH_PCR_GPIO_REV_ID               0x00
#define R_PCH_PCR_GPIO_CAP_LIST             0x04
#define R_PCH_PCR_GPIO_FAMBAR               0x08
#define R_PCH_PCR_GPIO_PADBAR               0x0C
#define B_PCH_PCR_GPIO_PADBAR               0x0000FFFF
#define R_PCH_PCR_GPIO_MISCCFG              0x10
#define B_PCH_PCR_GPIO_MISCCFG_GPE0_DW2     (BIT19 | BIT18 | BIT17 | BIT16)
#define N_PCH_PCR_GPIO_MISCCFG_GPE0_DW2     16
#define B_PCH_PCR_GPIO_MISCCFG_GPE0_DW1     (BIT15 | BIT14 | BIT13 | BIT12)
#define N_PCH_PCR_GPIO_MISCCFG_GPE0_DW1     12
#define B_PCH_PCR_GPIO_MISCCFG_GPE0_DW0     (BIT11 | BIT10 | BIT9 | BIT8)
#define N_PCH_PCR_GPIO_MISCCFG_GPE0_DW0     8
#define B_PCH_PCR_GPIO_MISCCFG_IRQ_ROUTE    BIT3
#define N_PCH_PCR_GPIO_MISCCFG_IRQ_ROUTE    3
#define B_PCH_PCR_GPIO_MISCCFG_GPDPCGEN     BIT1
#define B_PCH_PCR_GPIO_MISCCFG_GPDLCGEN     BIT0
// SKL PCH-H:
#define R_PCH_H_PCR_GPIO_MISCSECCFG         0x14

//
// GPIO Community 0 Private Configuration Registers
//
// SKL PCH-LP
#define R_PCH_LP_PCR_GPIO_GPP_A_PAD_OWN        0x20
#define R_PCH_LP_PCR_GPIO_GPP_B_PAD_OWN        0x30
#define R_PCH_LP_PCR_GPIO_GPP_A_GPI_VWM_EN     0x80
#define R_PCH_LP_PCR_GPIO_GPP_B_GPI_VWM_EN     0x84
#define R_PCH_LP_PCR_GPIO_GPP_A_PADCFGLOCK     0xA0
#define R_PCH_LP_PCR_GPIO_GPP_A_PADCFGLOCKTX   0xA4
#define R_PCH_LP_PCR_GPIO_GPP_B_PADCFGLOCK     0xA8
#define R_PCH_LP_PCR_GPIO_GPP_B_PADCFGLOCKTX   0xAC
// SKX Server PCH
#define R_PCH_H_PCR_GPIO_GPP_A_PAD_OWN        0x20
#define R_PCH_H_PCR_GPIO_GPP_B_PAD_OWN        0x2C
#define R_PCH_H_PCR_GPIO_GPP_F_PAD_OWN        0x38
#define R_PCH_H_PCR_GPIO_GPP_A_GPI_VWM_EN     0x50
#define R_PCH_H_PCR_GPIO_GPP_B_GPI_VWM_EN     0x54
#define R_PCH_H_PCR_GPIO_GPP_F_GPI_VWM_EN     0x58
#define R_PCH_H_PCR_GPIO_GPP_A_PADCFGLOCK     0x60
#define R_PCH_H_PCR_GPIO_GPP_A_PADCFGLOCKTX   0x64
#define R_PCH_H_PCR_GPIO_GPP_B_PADCFGLOCK     0x68
#define R_PCH_H_PCR_GPIO_GPP_B_PADCFGLOCKTX   0x6C
#define R_PCH_H_PCR_GPIO_GPP_F_PADCFGLOCK     0x70
#define R_PCH_H_PCR_GPIO_GPP_F_PADCFGLOCKTX   0x74
#define R_PCH_H_PCR_GPIO_GPP_F_HOSTSW_OWN     0x88
#define R_PCH_H_PCR_GPIO_GPP_F_GPI_IS         0x0108
#define R_PCH_H_PCR_GPIO_GPP_F_GPI_IE         0x0118
#define R_PCH_H_PCR_GPIO_GPP_F_GPI_GPE_STS    0x0128
#define R_PCH_H_PCR_GPIO_GPP_F_GPI_GPE_EN     0x0138
#define R_PCH_H_PCR_GPIO_GPP_F_PADCFG_OFFSET  0x580

// Common
#define R_PCH_PCR_GPIO_GPP_A_HOSTSW_OWN     0x80
#define R_PCH_PCR_GPIO_GPP_B_HOSTSW_OWN     0x84
#define R_PCH_PCR_GPIO_GPP_A_GPI_IS         0x0100
#define R_PCH_PCR_GPIO_GPP_B_GPI_IS         0x0104
#define R_PCH_PCR_GPIO_GPP_A_GPI_IE         0x0110
#define R_PCH_PCR_GPIO_GPP_B_GPI_IE         0x0114
#define R_PCH_PCR_GPIO_GPP_A_GPI_GPE_STS    0x0120
#define R_PCH_PCR_GPIO_GPP_B_GPI_GPE_STS    0x0124
#define R_PCH_PCR_GPIO_GPP_A_GPI_GPE_EN     0x0130
#define R_PCH_PCR_GPIO_GPP_B_GPI_GPE_EN     0x0134
#define R_PCH_PCR_GPIO_GPP_B_SMI_STS        0x0144
#define R_PCH_PCR_GPIO_GPP_B_SMI_EN         0x0154
#define R_PCH_PCR_GPIO_GPP_B_NMI_STS        0x0164
#define R_PCH_PCR_GPIO_GPP_B_NMI_EN         0x0174
#define R_PCH_PCR_GPIO_GPP_A_PADCFG_OFFSET  0x400
#define R_PCH_PCR_GPIO_GPP_B_PADCFG_OFFSET  0x4C0

//
// GPIO Community 1 Private Configuration Registers
//
//SKL PCH-LP:
#define R_PCH_LP_PCR_GPIO_GPP_C_PAD_OWN        0x20
#define R_PCH_LP_PCR_GPIO_GPP_D_PAD_OWN        0x30
#define R_PCH_LP_PCR_GPIO_GPP_E_PAD_OWN        0x40
#define R_PCH_LP_PCR_GPIO_GPP_C_GPI_VWM_EN     0x80
#define R_PCH_LP_PCR_GPIO_GPP_D_GPI_VWM_EN     0x84
#define R_PCH_LP_PCR_GPIO_GPP_E_GPI_VWM_EN     0x88
#define R_PCH_LP_PCR_GPIO_GPP_C_PADCFGLOCK     0xA0
#define R_PCH_LP_PCR_GPIO_GPP_C_PADCFGLOCKTX   0xA4
#define R_PCH_LP_PCR_GPIO_GPP_D_PADCFGLOCK     0xA8
#define R_PCH_LP_PCR_GPIO_GPP_D_PADCFGLOCKTX   0xAC
#define R_PCH_LP_PCR_GPIO_GPP_E_PADCFGLOCK     0xB0
#define R_PCH_LP_PCR_GPIO_GPP_E_PADCFGLOCKTX   0xB4
//SKL PCH-H:
#define R_PCH_H_PCR_GPIO_GPP_C_PAD_OWN        0x20
#define R_PCH_H_PCR_GPIO_GPP_D_PAD_OWN        0x2C
#define R_PCH_H_PCR_GPIO_GPP_E_PAD_OWN        0x38
// Server SKX PCH
#define R_PCH_H_PCR_GPIO_GPP_C_GPI_VWM_EN     0x50
#define R_PCH_H_PCR_GPIO_GPP_D_GPI_VWM_EN     0x54
#define R_PCH_H_PCR_GPIO_GPP_E_GPI_VWM_EN     0x58
#define R_PCH_H_PCR_GPIO_GPP_C_PADCFGLOCK     0x60
#define R_PCH_H_PCR_GPIO_GPP_C_PADCFGLOCKTX   0x64
#define R_PCH_H_PCR_GPIO_GPP_D_PADCFGLOCK     0x68
#define R_PCH_H_PCR_GPIO_GPP_D_PADCFGLOCKTX   0x6C
#define R_PCH_H_PCR_GPIO_GPP_E_PADCFGLOCK     0x70
#define R_PCH_H_PCR_GPIO_GPP_E_PADCFGLOCKTX   0x74
// Common
#define R_PCH_PCR_GPIO_GPP_C_HOSTSW_OWN       0x80
#define R_PCH_PCR_GPIO_GPP_D_HOSTSW_OWN       0x84
#define R_PCH_PCR_GPIO_GPP_E_HOSTSW_OWN       0x88
#define R_PCH_PCR_GPIO_GPP_C_GPI_IS           0x0100
#define R_PCH_PCR_GPIO_GPP_D_GPI_IS           0x0104
#define R_PCH_PCR_GPIO_GPP_E_GPI_IS           0x0108
#define R_PCH_PCR_GPIO_GPP_C_GPI_IE           0x0110
#define R_PCH_PCR_GPIO_GPP_D_GPI_IE           0x0114
#define R_PCH_PCR_GPIO_GPP_E_GPI_IE           0x0114
#define R_PCH_PCR_GPIO_GPP_C_GPI_GPE_STS      0x0120
#define R_PCH_PCR_GPIO_GPP_D_GPI_GPE_STS      0x0124
#define R_PCH_PCR_GPIO_GPP_E_GPI_GPE_STS      0x0128
#define R_PCH_PCR_GPIO_GPP_C_GPI_GPE_EN       0x0130
#define R_PCH_PCR_GPIO_GPP_D_GPI_GPE_EN       0x0134
#define R_PCH_PCR_GPIO_GPP_E_GPI_GPE_EN       0x0138
#define R_PCH_PCR_GPIO_GPP_C_SMI_STS          0x0140
#define R_PCH_PCR_GPIO_GPP_D_SMI_STS          0x0144
#define R_PCH_PCR_GPIO_GPP_E_SMI_STS          0x0148
#define R_PCH_PCR_GPIO_GPP_C_SMI_EN           0x0150
#define R_PCH_PCR_GPIO_GPP_D_SMI_EN           0x0154
#define R_PCH_PCR_GPIO_GPP_E_SMI_EN           0x0158
#define R_PCH_PCR_GPIO_GPP_C_NMI_STS          0x0160
#define R_PCH_PCR_GPIO_GPP_D_NMI_STS          0x0164
#define R_PCH_PCR_GPIO_GPP_E_NMI_STS          0x0168
#define R_PCH_PCR_GPIO_GPP_C_NMI_EN           0x0170
#define R_PCH_PCR_GPIO_GPP_D_NMI_EN           0x0174
#define R_PCH_PCR_GPIO_GPP_E_NMI_EN           0x0178


// Common:
#define R_PCH_PCR_GPIO_CAP_LIST_1_PWM         0x0200
#define R_PCH_PCR_GPIO_PWMC                   0x0204
#define R_PCH_PCR_GPIO_CAP_LIST_2_SER_BLINK   0x0208
#define R_PCH_PCR_GPIO_GP_SER_BLINK           0x020C
#define B_PCH_PCR_GPIO_GP_SER_BLINK           0x1F
#define R_PCH_PCR_GPIO_GP_SER_CMDSTS          0x0210
#define B_PCH_PCR_GPIO_GP_SER_CMDSTS_DLS      (BIT23 | BIT22)
#define N_PCH_PCR_GPIO_GP_SER_CMDSTS_DLS      22
#define B_PCH_PCR_GPIO_GP_SER_CMDSTS_DRS      0x003F0000
#define N_PCH_PCR_GPIO_GP_SER_CMDSTS_DRS      16
#define B_PCH_PCR_GPIO_GP_SER_CMDSTS_BUSY     BIT8
#define B_PCH_PCR_GPIO_GP_SER_CMDSTS_GO       BIT0
#define R_PCH_PCR_GPIO_GP_SER_DATA            0x0210
// Common:
#define R_PCH_PCR_GPIO_GPP_C_PADCFG_OFFSET    0x400
#define R_PCH_PCR_GPIO_GPP_D_PADCFG_OFFSET    0x4C0
#define R_PCH_PCR_GPIO_GPP_E_PADCFG_OFFSET    0x580

//
// GPIO Community 2 Private Configuration Registers
//
// SKL PCH-LP
#define R_PCH_LP_PCR_GPIO_GPD_PAD_OWN         0x20
#define R_PCH_LP_PCR_GPIO_GPD_GPI_VWM_EN      0x80
#define R_PCH_LP_PCR_GPIO_GPD_PADCFGLOCK      0xA0
#define R_PCH_LP_PCR_GPIO_GPD_PADCFGLOCKTX    0xA4
// SKX Server PCH
#define R_PCH_H_PCR_GPIO_GPD_PAD_OWN          0x20
#define R_PCH_H_PCR_GPIO_GPD_GPI_VWM_EN       0x50
#define R_PCH_H_PCR_GPIO_GPD_PADCFGLOCK       0x60
#define R_PCH_H_PCR_GPIO_GPD_PADCFGLOCKTX     0x64
// Common
#define R_PCH_PCR_GPIO_GPD_HOSTSW_OWN       0x80
#define R_PCH_PCR_GPIO_GPD_GPI_IS           0x0100
#define R_PCH_PCR_GPIO_GPD_GPI_IE           0x0110
#define R_PCH_PCR_GPIO_GPD_GPI_GPE_STS      0x0120
#define R_PCH_PCR_GPIO_GPD_GPI_GPE_EN       0x0130
#define R_PCH_PCR_GPIO_GPD_PADCFG_OFFSET    0x400

//
// GPIO Community 3 Private Configuration Registers
//
// SKL PCH-LP:
#define R_PCH_LP_PCR_GPIO_GPP_F_PAD_OWN        0x20
#define R_PCH_LP_PCR_GPIO_GPP_G_PAD_OWN        0x30
#define R_PCH_LP_PCR_GPIO_GPP_F_GPI_VWM_EN     0x80
#define R_PCH_LP_PCR_GPIO_GPP_G_GPI_VWM_EN     0x84
#define R_PCH_LP_PCR_GPIO_GPP_F_PADCFGLOCK     0xA0
#define R_PCH_LP_PCR_GPIO_GPP_F_PADCFGLOCKTX   0xA4
#define R_PCH_LP_PCR_GPIO_GPP_G_PADCFGLOCK     0xA8
#define R_PCH_LP_PCR_GPIO_GPP_G_PADCFGLOCKTX   0xAC
#define R_PCH_LP_PCR_GPIO_GPP_F_HOSTSW_OWN     0xD0
#define R_PCH_LP_PCR_GPIO_GPP_G_HOSTSW_OWN     0xD4
#define R_PCH_LP_PCR_GPIO_GPP_F_GPI_IS         0x0100
#define R_PCH_LP_PCR_GPIO_GPP_G_GPI_IS         0x0104
#define R_PCH_LP_PCR_GPIO_GPP_F_GPI_IE         0x0120
#define R_PCH_LP_PCR_GPIO_GPP_G_GPI_IE         0x0124
#define R_PCH_LP_PCR_GPIO_GPP_F_GPI_GPE_STS    0x0140
#define R_PCH_LP_PCR_GPIO_GPP_G_GPI_GPE_STS    0x0144
#define R_PCH_LP_PCR_GPIO_GPP_F_GPI_GPE_EN     0x0160
#define R_PCH_LP_PCR_GPIO_GPP_G_GPI_GPE_EN     0x0164
#define R_PCH_LP_PCR_GPIO_GPP_F_PADCFG_OFFSET  0x400
#define R_PCH_LP_PCR_GPIO_GPP_G_PADCFG_OFFSET  0x4C0

// SKX Server PCH
#define R_PCH_H_PCR_GPIO_GPP_I_PAD_OWN        0x20
#define R_PCH_H_PCR_GPIO_GPP_I_GPI_VWM_EN     0x50
#define R_PCH_H_PCR_GPIO_GPP_I_PADCFGLOCK     0x60
#define R_PCH_H_PCR_GPIO_GPP_I_PADCFGLOCKTX   0x64
#define R_PCH_H_PCR_GPIO_GPP_I_HOSTSW_OWN     0x80
#define R_PCH_H_PCR_GPIO_GPP_I_GPI_IS         0x0100
#define R_PCH_H_PCR_GPIO_GPP_I_GPI_IE         0x0110
#define R_PCH_H_PCR_GPIO_GPP_I_GPI_GPE_STS    0x0120
#define R_PCH_H_PCR_GPIO_GPP_I_GPI_GPE_EN     0x0130
#define R_PCH_H_PCR_GPIO_GPP_I_SMI_STS        0x0140
#define R_PCH_H_PCR_GPIO_GPP_I_SMI_EN         0x0150
#define R_PCH_H_PCR_GPIO_GPP_I_NMI_STS        0x0160
#define R_PCH_H_PCR_GPIO_GPP_I_NMI_EN         0x0170
#define R_PCH_H_PCR_GPIO_GPP_I_PADCFG_OFFSET  0x400

//
// GPIO Community 4 Private Configuration Registers
//

// SKX Server PCH
#define R_PCH_H_PCR_GPIO_GPP_J_PAD_OWN        0x20
#define R_PCH_H_PCR_GPIO_GPP_K_PAD_OWN        0x2C
#define R_PCH_H_PCR_GPIO_GPP_J_GPI_VWM_EN     0x50
#define R_PCH_H_PCR_GPIO_GPP_K_GPI_VWM_EN     0x54
#define R_PCH_H_PCR_GPIO_GPP_J_PADCFGLOCK     0x60
#define R_PCH_H_PCR_GPIO_GPP_J_PADCFGLOCKTX   0x64
#define R_PCH_H_PCR_GPIO_GPP_K_PADCFGLOCK     0x68
#define R_PCH_H_PCR_GPIO_GPP_K_PADCFGLOCKTX   0x6C
#define R_PCH_H_PCR_GPIO_GPP_J_HOSTSW_OWN     0x80
#define R_PCH_H_PCR_GPIO_GPP_K_HOSTSW_OWN     0x84
#define R_PCH_H_PCR_GPIO_GPP_J_GPI_IS         0x0100
#define R_PCH_H_PCR_GPIO_GPP_K_GPI_IS         0x0104
#define R_PCH_H_PCR_GPIO_GPP_J_GPI_IE         0x0110
#define R_PCH_H_PCR_GPIO_GPP_K_GPI_IE         0x0114
#define R_PCH_H_PCR_GPIO_GPP_J_GPI_GPE_STS    0x0120
#define R_PCH_H_PCR_GPIO_GPP_K_GPI_GPE_STS    0x0124
#define R_PCH_H_PCR_GPIO_GPP_J_GPI_GPE_EN     0x0130
#define R_PCH_H_PCR_GPIO_GPP_K_GPI_GPE_EN     0x0134
#define R_PCH_H_PCR_GPIO_GPP_J_PADCFG_OFFSET  0x400
#define R_PCH_H_PCR_GPIO_GPP_K_PADCFG_OFFSET  0x4C0

//
// GPIO Community 5 Private Configuration Registers
//

// SKX Server PCH
#define R_PCH_H_PCR_GPIO_GPP_G_PAD_OWN        0x20
#define R_PCH_H_PCR_GPIO_GPP_H_PAD_OWN        0x2C
#define R_PCH_H_PCR_GPIO_GPP_L_PAD_OWN        0x38
#define R_PCH_H_PCR_GPIO_GPP_G_GPI_VWM_EN     0x50
#define R_PCH_H_PCR_GPIO_GPP_H_GPI_VWM_EN     0x54
#define R_PCH_H_PCR_GPIO_GPP_L_GPI_VWM_EN     0x58
#define R_PCH_H_PCR_GPIO_GPP_G_PADCFGLOCK     0x60
#define R_PCH_H_PCR_GPIO_GPP_G_PADCFGLOCKTX   0x64
#define R_PCH_H_PCR_GPIO_GPP_H_PADCFGLOCK     0x68
#define R_PCH_H_PCR_GPIO_GPP_H_PADCFGLOCKTX   0x6C
#define R_PCH_H_PCR_GPIO_GPP_L_PADCFGLOCK     0x70
#define R_PCH_H_PCR_GPIO_GPP_L_PADCFGLOCKTX   0x74
#define R_PCH_H_PCR_GPIO_GPP_G_HOSTSW_OWN     0x80
#define R_PCH_H_PCR_GPIO_GPP_H_HOSTSW_OWN     0x84
#define R_PCH_H_PCR_GPIO_GPP_L_HOSTSW_OWN     0x88
#define R_PCH_H_PCR_GPIO_GPP_G_GPI_IS         0x0100
#define R_PCH_H_PCR_GPIO_GPP_H_GPI_IS         0x0104
#define R_PCH_H_PCR_GPIO_GPP_L_GPI_IS         0x0108
#define R_PCH_H_PCR_GPIO_GPP_G_GPI_IE         0x0110
#define R_PCH_H_PCR_GPIO_GPP_H_GPI_IE         0x0114
#define R_PCH_H_PCR_GPIO_GPP_L_GPI_IE         0x0118
#define R_PCH_H_PCR_GPIO_GPP_G_GPI_GPE_STS    0x0120
#define R_PCH_H_PCR_GPIO_GPP_H_GPI_GPE_STS    0x0124
#define R_PCH_H_PCR_GPIO_GPP_L_GPI_GPE_STS    0x0128
#define R_PCH_H_PCR_GPIO_GPP_G_GPI_GPE_EN     0x0130
#define R_PCH_H_PCR_GPIO_GPP_H_GPI_GPE_EN     0x0134
#define R_PCH_H_PCR_GPIO_GPP_L_GPI_GPE_EN     0x0138
#define R_PCH_H_PCR_GPIO_GPP_G_PADCFG_OFFSET  0x400
#define R_PCH_H_PCR_GPIO_GPP_H_PADCFG_OFFSET  0x4C0
#define R_PCH_H_PCR_GPIO_GPP_L_PADCFG_OFFSET  0x580




//
// Define Pad Number
//
#define V_GPIO_PAD0                                  0
#define V_GPIO_PAD1                                  1
#define V_GPIO_PAD2                                  2
#define V_GPIO_PAD3                                  3
#define V_GPIO_PAD4                                  4
#define V_GPIO_PAD5                                  5
#define V_GPIO_PAD6                                  6
#define V_GPIO_PAD7                                  7
#define V_GPIO_PAD8                                  8
#define V_GPIO_PAD9                                  9
#define V_GPIO_PAD10                                10
#define V_GPIO_PAD11                                11
#define V_GPIO_PAD12                                12
#define V_GPIO_PAD13                                13
#define V_GPIO_PAD14                                14
#define V_GPIO_PAD15                                15
#define V_GPIO_PAD16                                16
#define V_GPIO_PAD17                                17
#define V_GPIO_PAD18                                18
#define V_GPIO_PAD19                                19
#define V_GPIO_PAD20                                20
#define V_GPIO_PAD21                                21
#define V_GPIO_PAD22                                22
#define V_GPIO_PAD23                                23

//
// Host Software Pad Ownership modes
//
#define V_PCH_PCR_GPIO_HOSTSW_OWN_ACPI      0x00
#define V_PCH_PCR_GPIO_HOSTSW_OWN_GPIO      0x01

//
// Pad Ownership modes
//
#define V_PCH_PCR_GPIO_PAD_OWN_HOST         0x00
#define V_PCH_PCR_GPIO_PAD_OWN_CSME         0x01
#define V_PCH_PCR_GPIO_PAD_OWN_ISH          0x02

//
// Pad Configuration Register DW0
//

//Pad Reset Config
#define B_PCH_GPIO_RST_CONF             (BIT31 | BIT30)
#define N_PCH_GPIO_RST_CONF             30
#define V_PCH_GPIO_RST_CONF_POW_GOOD    0x00
#define V_PCH_GPIO_RST_CONF_DEEP_RST    0x01
#define V_PCH_GPIO_RST_CONF_GPIO_RST    0x02
#define V_PCH_GPIO_RST_CONF_RESUME_RST  0x03  // Only for GPD Group

//RX Pad State Select
#define B_PCH_GPIO_RX_PAD_STATE         BIT29
#define N_PCH_GPIO_RX_PAD_STATE         29
#define V_PCH_GPIO_RX_PAD_STATE_RAW     0x00
#define V_PCH_GPIO_RX_PAD_STATE_INT     0x01

//RX Raw Overrride to 1
#define B_PCH_GPIO_RX_RAW1              BIT28
#define N_PCH_GPIO_RX_RAW1              28
#define V_PCH_GPIO_RX_RAW1_DIS          0x00
#define V_PCH_GPIO_RX_RAW1_EN           0x01

//RX Level/Edge Configuration
#define B_PCH_GPIO_RX_LVL_EDG           (BIT26 | BIT25)
#define N_PCH_GPIO_RX_LVL_EDG           25
#define V_PCH_GPIO_RX_LVL_EDG_LVL       0x00
#define V_PCH_GPIO_RX_LVL_EDG_EDG       0x01
#define V_PCH_GPIO_RX_LVL_EDG_0         0x02
#define V_PCH_GPIO_RX_LVL_EDG_RIS_FAL   0x03

//RX Invert
#define B_PCH_GPIO_RXINV                BIT23
#define N_PCH_GPIO_RXINV                23
#define V_PCH_GPIO_RXINV_NO             0x00
#define V_PCH_GPIO_RXINV_YES            0x01

//GPIO Input Route IOxAPIC
#define B_PCH_GPIO_RX_APIC_ROUTE        BIT20
#define N_PCH_GPIO_RX_APIC_ROUTE        20
#define V_PCH_GPIO_RX_APIC_ROUTE_DIS    0x00
#define V_PCH_GPIO_RX_APIC_ROUTE_EN     0x01

//GPIO Input Route SCI
#define B_PCH_GPIO_RX_SCI_ROUTE         BIT19
#define N_PCH_GPIO_RX_SCI_ROUTE         19
#define V_PCH_GPIO_RX_SCI_ROUTE_DIS     0x00
#define V_PCH_GPIO_RX_SCI_ROUTE_EN      0x01

//GPIO Input Route SMI
#define B_PCH_GPIO_RX_SMI_ROUTE         BIT18
#define N_PCH_GPIO_RX_SMI_ROUTE         18
#define V_PCH_GPIO_RX_SMI_ROUTE_DIS     0x00
#define V_PCH_GPIO_RX_SMI_ROUTE_EN      0x01

//GPIO Input Route NMI
#define B_PCH_GPIO_RX_NMI_ROUTE         BIT17
#define N_PCH_GPIO_RX_NMI_ROUTE         17
#define V_PCH_GPIO_RX_NMI_ROUTE_DIS     0x00
#define V_PCH_GPIO_RX_NMI_ROUTE_EN      0x01

//GPIO Pad Mode
#define B_PCH_GPIO_PAD_MODE             (BIT12 | BIT11 | BIT10)
#define N_PCH_GPIO_PAD_MODE             10
#define V_PCH_GPIO_PAD_MODE_GPIO        0
#define V_PCH_GPIO_PAD_MODE_NAT_1       1
#define V_PCH_GPIO_PAD_MODE_NAT_2       2
#define V_PCH_GPIO_PAD_MODE_NAT_3       3
#define V_PCH_GPIO_PAD_MODE_NAT_4       4 // SPT-H only

//GPIO RX Disable
#define B_PCH_GPIO_RXDIS                BIT9
#define N_PCH_GPIO_RXDIS                9
#define V_PCH_GPIO_RXDIS_EN             0x00
#define V_PCH_GPIO_RXDIS_DIS            0x01

//GPIO TX Disable
#define B_PCH_GPIO_TXDIS                BIT8
#define N_PCH_GPIO_TXDIS                8
#define V_PCH_GPIO_TXDIS_EN             0x00
#define V_PCH_GPIO_TXDIS_DIS            0x01

//GPIO RX State
#define B_PCH_GPIO_RX_STATE             BIT1
#define N_PCH_GPIO_RX_STATE             1
#define V_PCH_GPIO_RX_STATE_LOW         0x00
#define V_PCH_GPIO_RX_STATE_HIGH        0x01

//GPIO TX State
#define B_PCH_GPIO_TX_STATE             BIT0
#define N_PCH_GPIO_TX_STATE             0
#define V_PCH_GPIO_TX_STATE_LOW         0x00
#define V_PCH_GPIO_TX_STATE_HIGH        0x01

//
// Pad Configuration Register DW1
//

//Padtol
#define B_PCH_GPIO_PADTOL               BIT25
#define N_PCH_GPIO_PADTOL               25
#define V_PCH_GPIO_PADTOL_NONE          0x00
#define V_PCH_GPIO_PADTOL_CLEAR         0x00
#define V_PCH_GPIO_PADTOL_SET           0x01

//Termination
#define B_PCH_GPIO_TERM                (BIT13 | BIT12 | BIT11 | BIT10)
#define N_PCH_GPIO_TERM                 10
#define V_PCH_GPIO_TERM_WPD_NONE        0x00
#define V_PCH_GPIO_TERM_WPD_5K          0x02
#define V_PCH_GPIO_TERM_WPD_20K         0x04
#define V_PCH_GPIO_TERM_WPU_NONE        0x08
#define V_PCH_GPIO_TERM_WPU_1K          0x09
#define V_PCH_GPIO_TERM_WPU_2K          0x0B
#define V_PCH_GPIO_TERM_WPU_5K          0x0A
#define V_PCH_GPIO_TERM_WPU_20K         0x0C
#define V_PCH_GPIO_TERM_WPU_1K_2K       0x0D
#define V_PCH_GPIO_TERM_NATIVE          0x0F

//Interrupt number
#define B_PCH_GPIO_INTSEL               0x7F
#define N_PCH_GPIO_INTSEL               0

//
// Ownership
//
#define V_PCH_GPIO_OWN_GPIO             0x01
#define V_PCH_GPIO_OWN_ACPI             0x00

//
// GPE
//
#define V_PCH_GPIO_GPE_EN               0x01
#define V_PCH_GPIO_GPE_DIS              0x00
//
// SMI
//
#define V_PCH_GPIO_SMI_EN               0x01
#define V_PCH_GPIO_SMI_DIS              0x00
//
// NMI
//
#define V_PCH_GPIO_NMI_EN               0x01
#define V_PCH_GPIO_NMI_DIS              0x00
//
// Reserved: RSVD1
//
#define V_PCH_GPIO_RSVD1                0x00

#endif
