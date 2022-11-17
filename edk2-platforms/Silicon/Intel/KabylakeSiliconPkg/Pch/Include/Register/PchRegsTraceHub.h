/** @file
  Register names for PCH TraceHub device

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

#ifndef _PCH_REGS_TRACE_HUB_H_
#define _PCH_REGS_TRACE_HUB_H_

//
// TraceHub Registers (D31:F7)
//
#define PCI_DEVICE_NUMBER_PCH_TRACE_HUB               31
#define PCI_FUNCTION_NUMBER_PCH_TRACE_HUB             7

#define R_PCH_TRACE_HUB_CSR_MTB_LBAR                  0x10
#define B_PCH_TRACE_HUB_CSR_MTB_RBAL                  0xFFF00000
#define R_PCH_TRACE_HUB_CSR_MTB_UBAR                  0x14
#define B_PCH_TRACE_HUB_CSR_MTB_RBAU                  0xFFFFFFFF
#define R_PCH_TRACE_HUB_SW_LBAR                       0x18
#define B_PCH_TRACE_HUB_SW_RBAL                       0xFFE00000
#define R_PCH_TRACE_HUB_SW_UBAR                       0x1C
#define B_PCH_TRACE_HUB_SW_RBAU                       0xFFFFFFFF
#define R_PCH_TRACE_HUB_RTIT_LBAR                     0x20
#define B_PCH_TRACE_HUB_RTIT_RBAL                     0xFFFFFF00
#define R_PCH_TRACE_HUB_RTIT_UBAR                     0x24
#define B_PCH_TRACE_HUB_RTIT_RBAU                     0xFFFFFFFF
#define R_PCH_TRACE_HUB_MSICID                        0x40
#define R_PCH_TRACE_HUB_MSINCP                        0x41
#define R_PCH_TRACE_HUB_MSIMC                         0x42
#define R_PCH_TRACE_HUB_MSILMA                        0x44
#define R_PCH_TRACE_HUB_MSIUMA                        0x48
#define R_PCH_TRACE_HUB_MSIMD                         0x4C
#define B_PCH_TRACE_HUB_FW_RBAL                       0xFFFC0000
#define B_PCH_TRACE_HUB_FW_RBAU                       0xFFFFFFFF
#define R_PCH_TRACE_HUB_DSC                           0x80
#define B_PCH_TRACE_HUB_BYP                           BIT0 //< TraceHub Bypass
#define R_PCH_TRACE_HUB_DSS                           0x81
#define R_PCH_TRACE_HUB_ISTOT                         0x84
#define R_PCH_TRACE_HUB_ICTOT                         0x88
#define R_PCH_TRACE_HUB_IPAD                          0x8C
#define R_PCH_TRACE_HUB_DSD                           0x90

//
// Offsets from CSR_MTB_BAR
//
#define R_PCH_TRACE_HUB_MTB_GTHOPT0                   0x00
#define B_PCH_TRACE_HUB_MTB_GTHOPT0_P0FLUSH           BIT7
#define B_PCH_TRACE_HUB_MTB_GTHOPT0_P1FLUSH           BIT15
#define V_PCH_TRACE_HUB_MTB_SWDEST_PTI                0x0A
#define V_PCH_TRACE_HUB_MTB_SWDEST_MEMEXI             0x08
#define V_PCH_TRACE_HUB_MTB_SWDEST_DISABLE            0x00
#define R_PCH_TRACE_HUB_MTB_SWDEST_1                  0x0C
#define B_PCH_TRACE_HUB_MTB_SWDEST_CSE_1              0x0000000F
#define B_PCH_TRACE_HUB_MTB_SWDEST_CSE_2              0x000000F0
#define B_PCH_TRACE_HUB_MTB_SWDEST_CSE_3              0x00000F00
#define B_PCH_TRACE_HUB_MTB_SWDEST_ISH_1              0x0000F000
#define B_PCH_TRACE_HUB_MTB_SWDEST_ISH_2              0x000F0000
#define B_PCH_TRACE_HUB_MTB_SWDEST_ISH_3              0x00F00000
#define B_PCH_TRACE_HUB_MTB_SWDEST_AUDIO              0x0F000000
#define B_PCH_TRACE_HUB_MTB_SWDEST_PMC                0xF0000000
#define R_PCH_TRACE_HUB_MTB_SWDEST_2                  0x10
#define B_PCH_TRACE_HUB_MTB_SWDEST_FTH                0x0000000F
#define R_PCH_TRACE_HUB_MTB_SWDEST_3                  0x14
#define B_PCH_TRACE_HUB_MTB_SWDEST_MAESTRO            0x00000F00
#define B_PCH_TRACE_HUB_MTB_SWDEST_SKYCAM             0x0F000000
#define B_PCH_TRACE_HUB_MTB_SWDEST_AET                0xF0000000
#define R_PCH_TRACE_HUB_MTB_SWDEST_4                  0x18
#define R_PCH_TRACE_HUB_MTB_MSC0CTL                   0xA0100
#define R_PCH_TRACE_HUB_MTB_MSC1CTL                   0xA0200
#define V_PCH_TRACE_HUB_MTB_MSCNMODE_DCI              0x2
#define V_PCH_TRACE_HUB_MTB_MSCNMODE_DEBUG            0x3
#define B_PCH_TRACE_HUB_MTB_MSCNLEN                   (BIT10 | BIT9 | BIT8)
#define B_PCH_TRACE_HUB_MTB_MSCNMODE                  (BIT5 | BIT4)
#define N_PCH_TRACE_HUB_MTB_MSCNMODE                  0x4
#define B_PCH_TRACE_HUB_MTB_MSCN_RD_HDR_OVRD          BIT2
#define B_PCH_TRACE_HUB_MTB_WRAPENN                   BIT1
#define B_PCH_TRACE_HUB_MTB_MSCNEN                    BIT0
#define R_PCH_TRACE_HUB_MTB_GTHSTAT                   0xD4
#define R_PCH_TRACE_HUB_MTB_SCR2                      0xD8
#define B_PCH_TRACE_HUB_MTB_SCR2_FCD                  BIT0
#define B_PCH_TRACE_HUB_MTB_SCR2_FSEOFF2              BIT2
#define B_PCH_TRACE_HUB_MTB_SCR2_FSEOFF3              BIT3
#define B_PCH_TRACE_HUB_MTB_SCR2_FSEOFF4              BIT4
#define B_PCH_TRACE_HUB_MTB_SCR2_FSEOFF5              BIT5
#define B_PCH_TRACE_HUB_MTB_SCR2_FSEOFF6              BIT6
#define B_PCH_TRACE_HUB_MTB_SCR2_FSEOFF7              BIT7
#define R_PCH_TRACE_HUB_MTB_MSC0BAR                   0xA0108
#define R_PCH_TRACE_HUB_MTB_MSC0SIZE                  0xA010C
#define R_PCH_TRACE_HUB_MTB_MSC1BAR                   0xA0208
#define R_PCH_TRACE_HUB_MTB_MSC1SIZE                  0xA020C
#define R_PCH_TRACE_HUB_MTB_STREAMCFG1                0xA1000
#define B_PCH_TRACE_HUB_MTB_STREAMCFG1_ENABLE         BIT28
#define R_PCH_TRACE_HUB_MTB_PTI_CTL                   0x1C00
#define B_PCH_TRACE_HUB_MTB_PTIMODESEL                0xF0
#define B_PCH_TRACE_HUB_MTB_PTICLKDIV                 (BIT17 | BIT16)
#define B_PCH_TRACE_HUB_MTB_PATGENMOD                 (BIT22 | BIT21 | BIT20)
#define B_PCH_TRACE_HUB_MTB_PTI_EN                    BIT0
#define R_PCH_TRACE_HUB_MTB_SCR                       0xC8
#define R_PCH_TRACE_HUB_MTB_GTH_FREQ                  0xCC
#define V_PCH_TRACE_HUB_MTB_SCR                       0x00130000
#define R_PCH_TRACE_HUB_CSR_MTB_SCRATCHPAD0           0xE0
#define R_PCH_TRACE_HUB_CSR_MTB_SCRATCHPAD1           0xE4
#define R_PCH_TRACE_HUB_CSR_MTB_SCRATCHPAD10          0xE40
#define R_PCH_TRACE_HUB_MTB_CTPGCS                    0x1C14
#define B_PCH_TRACE_HUB_MTB_CTPEN                     BIT0
#define V_PCH_TRACE_HUB_MTB_CHLCNT                    0x80
#define V_PCH_TRACE_HUB_MTB_STHMSTR                   0x20
#define R_PCH_TRACE_HUB_CSR_MTB_TSUCTRL               0x2000
#define B_PCH_TRACE_HUB_CSR_MTB_TSUCTRL_CTCRESYNC     BIT0

#endif
