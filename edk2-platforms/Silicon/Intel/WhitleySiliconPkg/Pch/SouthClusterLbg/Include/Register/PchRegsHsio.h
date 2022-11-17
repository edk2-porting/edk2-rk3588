/** @file
  Register definition for HSIO

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_HSIO_H_
#define _PCH_REGS_HSIO_H_

#define B_HSIO_PCR_ACCESS_TYPE                          (BIT15 | BIT14)
#define N_HSIO_PCR_ACCESS_TYPE              14
#define V_HSIO_PCR_ACCESS_TYPE_BDCAST                   (BIT15 | BIT14)
#define V_HSIO_PCR_ACCESS_TYPE_MULCAST      BIT15
#define B_HSIO_PCR_LANE_GROUP_NO                        (BIT13 | BIT12 | BIT11 | BIT10 | BIT9)
#define B_HSIO_PCR_FUNCTION_NO                          (BIT8  | BIT7)
#define N_HSIO_PCR_FUNCTION_NO                          7
#define B_HSIO_PCR_REG_OFFSET                           (BIT6  | BIT5  | BIT4  | BIT3  | BIT2  | BIT1  | BIT0)

#define V_HSIO_PCR_ACCESS_TYPE_BCAST        0x03
#define V_HSIO_PCR_ACCESS_TYPE_MCAST        0x02
#define V_HSIO_PCR_ACCESS_TYPE_UCAST        0x00

#define V_HSIO_PCR_LANE_GROUP_NO_CMN_LANE   0x00

#define V_HSIO_PCR_FUNCTION_NO_PCS          0x00
#define V_HSIO_PCR_FUNCTION_NO_TX           0x01
#define V_HSIO_PCR_FUNCTION_NO_RX           0x02

#define V_HSIO_PCR_FUNCTION_NO_CMNDIG       0x00
#define V_HSIO_PCR_FUNCTION_NO_CMNANA       0x01
#define V_HSIO_PCR_FUNCTION_NO_PLL          0x02

#define R_HSIO_PCR_PCS_DWORD4                           0x10

#define R_HSIO_PCR_PCS_DWORD8                           0x20
#define B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_PTR_INIT_4_0     0x1F000000
#define B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0      0x001F0000
#define N_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0      16
#define B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_HIWATER_4_0      0x00001F00
#define N_HSIO_PCR_PCS_DWORD8_CRI_RXEB_HIWATER_4_0      8

#define R_HSIO_PCR_PCS_DWORD9                           0x24
#define B_HSIO_PCR_PCS_DWORD9_REG_ENABLE_PWR_GATING     BIT29

#define R_HSIO_PCR_RX_DWORD8                            0x120
#define B_HSIO_PCR_RX_DWORD8_ICFGDFETAP3_EN             BIT10

#define R_HSIO_PCR_RX_DWORD9                            0x124
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP4_OVERRIDE_EN     BIT24
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP3_OVERRIDE_EN     BIT26
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP2_OVERRIDE_EN     BIT28
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP1_OVERRIDE_EN     BIT30

#define R_HSIO_PCR_RX_DWORD12                           0x130
#define B_HSIO_PCR_RX_DWORD12_O_CFGEWMARGINSEL          BIT14

#define R_HSIO_PCR_RX_DWORD20                               0x150
#define B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0  (BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0  24

#define R_HSIO_PCR_RX_DWORD21                               0x154
#define B_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_QUATRATE_5_0  (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_QUATRATE_5_0  8
#define B_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_HALFRATE_5_0  (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define N_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_HALFRATE_5_0  0

#define R_HSIO_PCR_RX_DWORD23                               0x15C
#define B_HSIO_PCR_RX_DWORD23_ICFGVGABLWTAP_OVERRIDE_EN     BIT2
#define B_HSIO_PCR_RX_DWORD23_CFGVGATAP_ADAPT_OVERRIDE_EN   BIT4

#define R_HSIO_PCR_RX_DWORD25                           0x164
#define B_HSIO_PCR_RX_DWORD25_RX_TAP_CFG_CTRL           BIT3
#define B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0  0x1F0000
#define N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0  16

#define R_HSIO_PCR_RX_DWORD26                           0x168
#define B_HSIO_PCR_RX_DWORD26_SATA_EQ_DIS               BIT16

#define R_HSIO_PCR_RX_DWORD34                           0x188
#define B_HSIO_PCR_RX_DWORD34_MM_PH_OFC_SCALE_2_0       (BIT14 | BIT13 | BIT12)
#define N_HSIO_PCR_RX_DWORD34_MM_PH_OFC_SCALE_2_0       12

#define R_HSIO_PCR_RX_DWORD44                           0x1B0
#define B_HSIO_PCR_RX_DWORD44_0_DFE_DATASUMCAL0_7_0     0xFF0000
#define N_HSIO_PCR_RX_DWORD44_0_DFE_DATASUMCAL0_7_0     16

#define R_HSIO_PCR_RX_DWORD39                           0x19C
#define B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0          0x7C0000
#define N_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0          18

#define R_HSIO_PCR_RX_DWORD40                           0x1A0
#define B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP     0xFF000000
#define N_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP     24

#define R_HSIO_PCR_RX_DWORD41                           0x1A4
#define B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS     0xFFFFFF
#define N_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS     0

#define R_HSIO_PCR_RX_DWORD7                            0x11C
#define B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL               0xF8000000
#define N_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL               27


#define R_HSIO_PCR_RX_DWORD56                           0x1E0
#define B_HSIO_PCR_RX_DWORD56_ICFGPIDACCFGVALID         BIT16

#define R_HSIO_PCR_RX_DWORD57                           0x1E4
#define B_HSIO_PCR_RX_DWORD57_JIM_COURSE                BIT30
#define B_HSIO_PCR_RX_DWORD57_JIM_ENABLE                BIT29
#define B_HSIO_PCR_RX_DWORD57_JIMMODE                   BIT28
#define B_HSIO_PCR_RX_DWORD57_JIMNUMCYCLES_3_0          0x0F000000
#define N_HSIO_PCR_RX_DWORD57_JIMNUMCYCLES_3_0          24
#define B_HSIO_PCR_RX_DWORD57_ICFGMARGINEN              BIT0

#define R_HSIO_PCR_RX_DWORD59                           0x1EC
#define R_HSIO_PCR_RX_DWORD60                           0x1F0

#define R_HSIO_PCR_TX_DWORD5                            0x94
#define B_HSIO_PCR_TX_DWORD5_OW2TAPGEN2DEEMPH3P5_5_0    (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_TX_DWORD5_OW2TAPGEN2DEEMPH3P5_5_0    16
#define B_HSIO_PCR_TX_DWORD5_OW2TAPGEN1DEEMPH3P5_5_0    (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_TX_DWORD5_OW2TAPGEN1DEEMPH3P5_5_0    8

#define R_HSIO_PCR_TX_DWORD6                            0x98
#define B_HSIO_PCR_TX_DWORD6_OW2TAPGEN3DEEMPH6P0_5_0    (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_TX_DWORD6_OW2TAPGEN3DEEMPH6P0_5_0    16
#define B_HSIO_PCR_TX_DWORD6_OW2TAPGEN2DEEMPH6P0_5_0    (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_TX_DWORD6_OW2TAPGEN2DEEMPH6P0_5_0    8
#define B_HSIO_PCR_TX_DWORD6_OW2TAPGEN1DEEMPH6P0_5_0    (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

#define R_HSIO_PCR_TX_DWORD8                            0xA0
#define B_HSIO_PCR_TX_DWORD8_ORATE10MARGIN_5_0          (BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define N_HSIO_PCR_TX_DWORD8_ORATE10MARGIN_5_0          24
#define B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0          (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0          16
#define B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0          (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0          8

#define R_HSIO_PCR_TX_DWORD19                           0xCC

#define R_PCH_LP_HSIO_LANE10_PCS_DWORD8                 0x020
#define R_PCH_LP_HSIO_LANE11_PCS_DWORD8                 0x220
#define R_PCH_LP_HSIO_LANE14_PCS_DWORD8                 0x820
#define R_PCH_LP_HSIO_LANE15_PCS_DWORD8                 0xA20
#define R_PCH_H_HSIO_LANE18_PCS_DWORD8                  0x820
#define R_PCH_H_HSIO_LANE19_PCS_DWORD8                  0xA20
#define R_PCH_H_HSIO_LANE22_PCS_DWORD8                  0x020
#define R_PCH_H_HSIO_LANE23_PCS_DWORD8                  0x220
#define R_PCH_H_HSIO_LANE24_PCS_DWORD8                  0x420
#define R_PCH_H_HSIO_LANE25_PCS_DWORD8                  0x620
#define R_PCH_H_HSIO_LANE26_PCS_DWORD8                  0x820
#define R_PCH_H_HSIO_LANE27_PCS_DWORD8                  0xA20
#define R_PCH_H_HSIO_LANE28_PCS_DWORD8                  0xC20
#define R_PCH_H_HSIO_LANE29_PCS_DWORD8                  0xE20

#define R_HSIO_PCR_CLANE0_CMN_ANA_DWORD2                0x8088
#define B_HSIO_PCR_CLANE0_CMN_ANA_DWORD2_O_DTPLL1_lC_PLLEN_H_OVRDEN                BIT5
#define B_HSIO_PCR_CLANE0_CMN_ANA_DWORD2_O_DTPLL1_lC_FULLCALRESET_L_OVERDEN        BIT3

#define R_HSIO_PCR_PLL_SSC_DWORD2                       0x8108
#define B_HSIO_PCR_PLL_SSC_DWORD2_SSCSTEPSIZE_7_0       (BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_PLL_SSC_DWORD2_SSCSTEPSIZE_7_0       16
#define B_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN                BIT10
#define N_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN                10

#define R_HSIO_PCR_PLL_SSC_DWORD3                       0x810C
#define B_HSIO_PCR_PLL_SSC_DWORD3_SSC_PROPAGATE         BIT0

#define R_PCH_PCR_MODPHY0_COM0_CMN_DIG_DWORD12          0x8030
#define B_PCH_PCR_MODPHY0_COM0_CMN_DIG_DWORD12_O_CFG_PWR_GATING_CTRL BIT0

//
// xHCI SSIC Private Configuration Register, but with opcode 4/5 for read/write access
//
#define R_PCH_PCR_MMP0_LANE_0_OFFSET                    0x0
#define R_PCH_PCR_MMP0_LANE_1_OFFSET                    0x2000
#define R_PCH_PCR_MMP0_IMPREG21                         0x1050
#define R_PCH_PCR_MMP0_IMPREG22                         0x1054
#define R_PCH_PCR_MMP0_IMPREG23                         0x1058
#define R_PCH_PCR_MMP0_IMPREG24                         0x105C
#define R_PCH_PCR_MMP0_IMPREG25                         0x1060
#define R_PCH_PCR_MMP0_CMNREG4                          0xF00C
#define R_PCH_PCR_MMP0_CMNREG15                         0xF038
#define R_PCH_PCR_MMP0_CMNREG16                         0xF03C

#endif //_PCH_REGS_HSIO_H_

