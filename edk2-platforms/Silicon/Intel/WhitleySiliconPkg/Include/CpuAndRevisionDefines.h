/** @file
  Defines for CPU and Revision library.

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_AND_REVISION_DEFINES_H_// #ifndef _CPU_AND_REVISION_LIB_H_
#define _CPU_AND_REVISION_DEFINES_H_

// CPU types
#define CPU_SKX        0     // Skylake:           Mainstream Xeon – 14nm
#define CPU_CLX        1     // CascadeLake:       Mainstream Xeon – 14nm
#define CPU_CPX        2     // CooperLake:        Mainstream Xeon – 14nm
#define CPU_ICXSP      4     // Icelake-SP:        Mainstream Xeon – 10nm Wave 1
#define CPU_MAX        0xFF  // MAX CPU limit


// CPU Physical chop types
#define CPU_CHOP_TYPE_LCC       0
#define CPU_CHOP_TYPE_MCC       1
#define CPU_CHOP_TYPE_HCC       2
#define CPU_CHOP_TYPE_XCC       3

#define MINOR_REV_MASK       0x000F
#define MINOR_REV_BIT_WIDTH  4
#define MAJOR_REV_MASK       0xFFF0

#define MAJOR_REV_A   (0x01 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_B   (0x02 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_C   (0x03 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_D   (0x04 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_E   (0x05 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_F   (0x06 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_G   (0x07 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_H   (0x08 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_I   (0x09 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_J   (0x0A << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_K   (0x0B << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_L   (0x0C << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_M   (0x0D << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_N   (0x0E << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_O   (0x0F << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_P   (0x10 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_Q   (0x11 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_R   (0x12 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_S   (0x13 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_T   (0x14 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_U   (0x15 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_V   (0x16 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_W   (0x17 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_X   (0x18 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_Y   (0x19 << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_Z   (0x1A << (MINOR_REV_BIT_WIDTH))
#define MAJOR_REV_ALL (0x1F << (MINOR_REV_BIT_WIDTH))

#define MINOR_REV_0   0
#define MINOR_REV_1   1
#define MINOR_REV_2   2
#define MINOR_REV_3   3
#define MINOR_REV_4   4
#define MINOR_REV_5   5
#define MINOR_REV_6   6
#define MINOR_REV_7   7
#define MINOR_REV_8   8
#define MINOR_REV_ALL 0xF

//
// Revision defines - Generic for all Cpu Types
// If new revisions are needed, follow same pattern
//
#define   REV_A0    (MAJOR_REV_A | MINOR_REV_0)
#define   REV_A1    (MAJOR_REV_A | MINOR_REV_1)
#define   REV_A2    (MAJOR_REV_A | MINOR_REV_2)
#define   REV_A3    (MAJOR_REV_A | MINOR_REV_3)
#define   REV_A4    (MAJOR_REV_A | MINOR_REV_4)
#define   REV_A5    (MAJOR_REV_A | MINOR_REV_5)
#define   REV_AX    (MAJOR_REV_A | MINOR_REV_ALL)

#define   REV_B0    (MAJOR_REV_B | MINOR_REV_0)
#define   REV_B1    (MAJOR_REV_B | MINOR_REV_1)
#define   REV_B2    (MAJOR_REV_B | MINOR_REV_2)
#define   REV_B3    (MAJOR_REV_B | MINOR_REV_3)
#define   REV_B4    (MAJOR_REV_B | MINOR_REV_4)
#define   REV_B5    (MAJOR_REV_B | MINOR_REV_5)
#define   REV_BX    (MAJOR_REV_B | MINOR_REV_ALL)

#define   REV_C0    (MAJOR_REV_C | MINOR_REV_0)
#define   REV_C1    (MAJOR_REV_C | MINOR_REV_1)
#define   REV_C2    (MAJOR_REV_C | MINOR_REV_2)
#define   REV_C3    (MAJOR_REV_C | MINOR_REV_3)
#define   REV_C4    (MAJOR_REV_C | MINOR_REV_4)
#define   REV_C5    (MAJOR_REV_C | MINOR_REV_5)
#define   REV_CX    (MAJOR_REV_C | MINOR_REV_ALL)

#define   REV_D0    (MAJOR_REV_D | MINOR_REV_0)
#define   REV_D1    (MAJOR_REV_D | MINOR_REV_1)
#define   REV_D2    (MAJOR_REV_D | MINOR_REV_2)
#define   REV_D3    (MAJOR_REV_D | MINOR_REV_3)
#define   REV_D4    (MAJOR_REV_D | MINOR_REV_4)
#define   REV_D5    (MAJOR_REV_D | MINOR_REV_5)
#define   REV_DX    (MAJOR_REV_D | MINOR_REV_ALL)

#define   REV_E0    (MAJOR_REV_E | MINOR_REV_0)
#define   REV_E1    (MAJOR_REV_E | MINOR_REV_1)
#define   REV_E2    (MAJOR_REV_E | MINOR_REV_2)
#define   REV_E3    (MAJOR_REV_E | MINOR_REV_3)
#define   REV_E4    (MAJOR_REV_E | MINOR_REV_4)
#define   REV_E5    (MAJOR_REV_E | MINOR_REV_5)
#define   REV_EX    (MAJOR_REV_E | MINOR_REV_ALL)

#define   REV_F0    (MAJOR_REV_F | MINOR_REV_0)
#define   REV_F1    (MAJOR_REV_F | MINOR_REV_1)
#define   REV_F2    (MAJOR_REV_F | MINOR_REV_2)
#define   REV_F3    (MAJOR_REV_F | MINOR_REV_3)
#define   REV_F4    (MAJOR_REV_F | MINOR_REV_4)
#define   REV_F5    (MAJOR_REV_F | MINOR_REV_5)
#define   REV_FX    (MAJOR_REV_F | MINOR_REV_ALL)

#define   REV_G0    (MAJOR_REV_G | MINOR_REV_0)
#define   REV_G1    (MAJOR_REV_G | MINOR_REV_1)
#define   REV_G2    (MAJOR_REV_G | MINOR_REV_2)
#define   REV_G3    (MAJOR_REV_G | MINOR_REV_3)
#define   REV_G4    (MAJOR_REV_G | MINOR_REV_4)
#define   REV_G5    (MAJOR_REV_G | MINOR_REV_5)
#define   REV_GX    (MAJOR_REV_G | MINOR_REV_ALL)

#define   REV_H0    (MAJOR_REV_H | MINOR_REV_0)
#define   REV_H1    (MAJOR_REV_H | MINOR_REV_1)
#define   REV_H2    (MAJOR_REV_H | MINOR_REV_2)
#define   REV_H3    (MAJOR_REV_H | MINOR_REV_3)
#define   REV_H4    (MAJOR_REV_H | MINOR_REV_4)
#define   REV_H5    (MAJOR_REV_H | MINOR_REV_5)
#define   REV_HX    (MAJOR_REV_H | MINOR_REV_ALL)

#define   REV_I0    (MAJOR_REV_I | MINOR_REV_0)
#define   REV_I1    (MAJOR_REV_I | MINOR_REV_1)
#define   REV_I2    (MAJOR_REV_I | MINOR_REV_2)
#define   REV_I3    (MAJOR_REV_I | MINOR_REV_3)
#define   REV_I4    (MAJOR_REV_I | MINOR_REV_4)
#define   REV_I5    (MAJOR_REV_I | MINOR_REV_5)
#define   REV_IX    (MAJOR_REV_I | MINOR_REV_ALL)

#define   REV_J0    (MAJOR_REV_J | MINOR_REV_0)
#define   REV_J1    (MAJOR_REV_J | MINOR_REV_1)
#define   REV_J2    (MAJOR_REV_J | MINOR_REV_2)
#define   REV_J3    (MAJOR_REV_J | MINOR_REV_3)
#define   REV_J4    (MAJOR_REV_J | MINOR_REV_4)
#define   REV_J5    (MAJOR_REV_J | MINOR_REV_5)
#define   REV_JX    (MAJOR_REV_J | MINOR_REV_ALL)

#define   REV_K0    (MAJOR_REV_K | MINOR_REV_0)
#define   REV_K1    (MAJOR_REV_K | MINOR_REV_1)
#define   REV_K2    (MAJOR_REV_K | MINOR_REV_2)
#define   REV_K3    (MAJOR_REV_K | MINOR_REV_3)
#define   REV_K4    (MAJOR_REV_K | MINOR_REV_4)
#define   REV_K5    (MAJOR_REV_K | MINOR_REV_5)
#define   REV_KX    (MAJOR_REV_K | MINOR_REV_ALL)

#define   REV_L0    (MAJOR_REV_L | MINOR_REV_0)
#define   REV_L1    (MAJOR_REV_L | MINOR_REV_1)
#define   REV_L2    (MAJOR_REV_L | MINOR_REV_2)
#define   REV_L3    (MAJOR_REV_L | MINOR_REV_3)
#define   REV_L4    (MAJOR_REV_L | MINOR_REV_4)
#define   REV_L5    (MAJOR_REV_L | MINOR_REV_5)
#define   REV_LX    (MAJOR_REV_L | MINOR_REV_ALL)

#define   REV_M0    (MAJOR_REV_M | MINOR_REV_0)
#define   REV_M1    (MAJOR_REV_M | MINOR_REV_1)
#define   REV_M2    (MAJOR_REV_M | MINOR_REV_2)
#define   REV_M3    (MAJOR_REV_M | MINOR_REV_3)
#define   REV_M4    (MAJOR_REV_M | MINOR_REV_4)
#define   REV_M5    (MAJOR_REV_M | MINOR_REV_5)
#define   REV_MX    (MAJOR_REV_M | MINOR_REV_ALL)

#define   REV_N0    (MAJOR_REV_N | MINOR_REV_0)
#define   REV_N1    (MAJOR_REV_N | MINOR_REV_1)
#define   REV_N2    (MAJOR_REV_N | MINOR_REV_2)
#define   REV_N3    (MAJOR_REV_N | MINOR_REV_3)
#define   REV_N4    (MAJOR_REV_N | MINOR_REV_4)
#define   REV_N5    (MAJOR_REV_N | MINOR_REV_5)
#define   REV_NX    (MAJOR_REV_N | MINOR_REV_ALL)

#define   REV_O0    (MAJOR_REV_O | MINOR_REV_0)
#define   REV_O1    (MAJOR_REV_O | MINOR_REV_1)
#define   REV_O2    (MAJOR_REV_O | MINOR_REV_2)
#define   REV_O3    (MAJOR_REV_O | MINOR_REV_3)
#define   REV_O4    (MAJOR_REV_O | MINOR_REV_4)
#define   REV_O5    (MAJOR_REV_O | MINOR_REV_5)
#define   REV_OX    (MAJOR_REV_O | MINOR_REV_ALL)

#define   REV_P0    (MAJOR_REV_P | MINOR_REV_0)
#define   REV_P1    (MAJOR_REV_P | MINOR_REV_1)
#define   REV_P2    (MAJOR_REV_P | MINOR_REV_2)
#define   REV_P3    (MAJOR_REV_P | MINOR_REV_3)
#define   REV_P4    (MAJOR_REV_P | MINOR_REV_4)
#define   REV_P5    (MAJOR_REV_P | MINOR_REV_5)
#define   REV_PX    (MAJOR_REV_P | MINOR_REV_ALL)

#define   REV_Q0    (MAJOR_REV_Q | MINOR_REV_0)
#define   REV_Q1    (MAJOR_REV_Q | MINOR_REV_1)
#define   REV_Q2    (MAJOR_REV_Q | MINOR_REV_2)
#define   REV_Q3    (MAJOR_REV_Q | MINOR_REV_3)
#define   REV_Q4    (MAJOR_REV_Q | MINOR_REV_4)
#define   REV_Q5    (MAJOR_REV_Q | MINOR_REV_5)
#define   REV_QX    (MAJOR_REV_Q | MINOR_REV_ALL)

#define   REV_R0    (MAJOR_REV_R | MINOR_REV_0)
#define   REV_R1    (MAJOR_REV_R | MINOR_REV_1)
#define   REV_R2    (MAJOR_REV_R | MINOR_REV_2)
#define   REV_R3    (MAJOR_REV_R | MINOR_REV_3)
#define   REV_R4    (MAJOR_REV_R | MINOR_REV_4)
#define   REV_R5    (MAJOR_REV_R | MINOR_REV_5)
#define   REV_RX    (MAJOR_REV_R | MINOR_REV_ALL)

#define   REV_S0    (MAJOR_REV_S | MINOR_REV_0)
#define   REV_S1    (MAJOR_REV_S | MINOR_REV_1)
#define   REV_S2    (MAJOR_REV_S | MINOR_REV_2)
#define   REV_S3    (MAJOR_REV_S | MINOR_REV_3)
#define   REV_S4    (MAJOR_REV_S | MINOR_REV_4)
#define   REV_S5    (MAJOR_REV_S | MINOR_REV_5)
#define   REV_SX    (MAJOR_REV_S | MINOR_REV_ALL)

#define   REV_T0    (MAJOR_REV_T | MINOR_REV_0)
#define   REV_T1    (MAJOR_REV_T | MINOR_REV_1)
#define   REV_T2    (MAJOR_REV_T | MINOR_REV_2)
#define   REV_T3    (MAJOR_REV_T | MINOR_REV_3)
#define   REV_T4    (MAJOR_REV_T | MINOR_REV_4)
#define   REV_T5    (MAJOR_REV_T | MINOR_REV_5)
#define   REV_TX    (MAJOR_REV_T | MINOR_REV_ALL)

#define   REV_U0    (MAJOR_REV_U | MINOR_REV_0)
#define   REV_U1    (MAJOR_REV_U | MINOR_REV_1)
#define   REV_U2    (MAJOR_REV_U | MINOR_REV_2)
#define   REV_U3    (MAJOR_REV_U | MINOR_REV_3)
#define   REV_U4    (MAJOR_REV_U | MINOR_REV_4)
#define   REV_U5    (MAJOR_REV_U | MINOR_REV_5)
#define   REV_UX    (MAJOR_REV_U | MINOR_REV_ALL)

#define   REV_V0    (MAJOR_REV_V | MINOR_REV_0)
#define   REV_V1    (MAJOR_REV_V | MINOR_REV_1)
#define   REV_V2    (MAJOR_REV_V | MINOR_REV_2)
#define   REV_V3    (MAJOR_REV_V | MINOR_REV_3)
#define   REV_V4    (MAJOR_REV_V | MINOR_REV_4)
#define   REV_V5    (MAJOR_REV_V | MINOR_REV_5)
#define   REV_VX    (MAJOR_REV_V | MINOR_REV_ALL)

#define   REV_W0    (MAJOR_REV_W | MINOR_REV_0)
#define   REV_W1    (MAJOR_REV_W | MINOR_REV_1)
#define   REV_W2    (MAJOR_REV_W | MINOR_REV_2)
#define   REV_W3    (MAJOR_REV_W | MINOR_REV_3)
#define   REV_W4    (MAJOR_REV_W | MINOR_REV_4)
#define   REV_W5    (MAJOR_REV_W | MINOR_REV_5)
#define   REV_WX    (MAJOR_REV_W | MINOR_REV_ALL)

#define   REV_X0    (MAJOR_REV_X | MINOR_REV_0)
#define   REV_X1    (MAJOR_REV_X | MINOR_REV_1)
#define   REV_X2    (MAJOR_REV_X | MINOR_REV_2)
#define   REV_X3    (MAJOR_REV_X | MINOR_REV_3)
#define   REV_X4    (MAJOR_REV_X | MINOR_REV_4)
#define   REV_X5    (MAJOR_REV_X | MINOR_REV_5)
#define   REV_XX    (MAJOR_REV_X | MINOR_REV_ALL)

#define   REV_Y0    (MAJOR_REV_Y | MINOR_REV_0)
#define   REV_Y1    (MAJOR_REV_Y | MINOR_REV_1)
#define   REV_Y2    (MAJOR_REV_Y | MINOR_REV_2)
#define   REV_Y3    (MAJOR_REV_Y | MINOR_REV_3)
#define   REV_Y4    (MAJOR_REV_Y | MINOR_REV_4)
#define   REV_Y5    (MAJOR_REV_Y | MINOR_REV_5)
#define   REV_YX    (MAJOR_REV_Y | MINOR_REV_ALL)

#define   REV_Z0    (MAJOR_REV_Z | MINOR_REV_0)
#define   REV_Z1    (MAJOR_REV_Z | MINOR_REV_1)
#define   REV_Z2    (MAJOR_REV_Z | MINOR_REV_2)
#define   REV_Z3    (MAJOR_REV_Z | MINOR_REV_3)
#define   REV_Z4    (MAJOR_REV_Z | MINOR_REV_4)
#define   REV_Z5    (MAJOR_REV_Z | MINOR_REV_5)
#define   REV_ZX    (MAJOR_REV_Z | MINOR_REV_ALL)

#define   REV_ALL   (MAJOR_REV_ALL | MINOR_REV_ALL)
#endif