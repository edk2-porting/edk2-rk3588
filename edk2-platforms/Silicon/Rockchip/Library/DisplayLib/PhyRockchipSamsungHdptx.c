/** @file

  Rockchip HDMI/DP Combo PHY with Samsung IP block
  Copyright (c) 2022, Linaro, Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AnalogixDpLib.h>
#include <Library/PWMLib.h>
#include <Library/DrmModes.h>

#define BITS_PER_LONG   32
#define GENMASK(h, l)  \
  (((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
#define RO_REF_CLK_SEL				GENMASK(11, 10)
#define LC_REF_CLK_SEL				GENMASK(9, 8)
#define PLL_EN					BIT(7)
#define BIAS_EN					BIT(6)
#define BGR_EN					BIT(5)
#define HDPTX_MODE_SEL				BIT(0)
#define PLL_LOCK_DONE				BIT(3)
#define PHY_CLK_RDY				BIT(2)
#define PHY_RDY					BIT(1)
#define SB_RDY					BIT(0)

/* cmn_reg0008 */
#define OVRD_LCPLL_EN				BIT(7)
#define LCPLL_EN				BIT(6)

/* cmn_reg003C */
#define ANA_LCPLL_RESERVED7			BIT(7)

/* cmn_reg003D */
#define OVRD_ROPLL_EN				BIT(7)
#define ROPLL_EN				BIT(6)

/* cmn_reg0046 */
#define ROPLL_ANA_CPP_CTRL_COARSE		GENMASK(7, 4)
#define ROPLL_ANA_CPP_CTRL_FINE			GENMASK(3, 0)

/* cmn_reg0047 */
#define ROPLL_ANA_LPF_C_SEL_COARSE		GENMASK(5, 3)
#define ROPLL_ANA_LPF_C_SEL_FINE		GENMASK(2, 0)

/* cmn_reg004E */
#define ANA_ROPLL_PI_EN				BIT(5)

/* cmn_reg0051 */
#define ROPLL_PMS_MDIV				GENMASK(7, 0)

/* cmn_reg0055 */
#define ROPLL_PMS_MDIV_AFC			GENMASK(7, 0)

/* cmn_reg0059 */
#define ANA_ROPLL_PMS_PDIV			GENMASK(7, 4)
#define ANA_ROPLL_PMS_REFDIV			GENMASK(3, 0)

/* cmn_reg005A */
#define ROPLL_PMS_SDIV_RBR			GENMASK(7, 4)
#define ROPLL_PMS_SDIV_HBR			GENMASK(3, 0)

/* cmn_reg005B */
#define ROPLL_PMS_SDIV_HBR2			GENMASK(7, 4)
#define ROPLL_PMS_SDIV_HBR3			GENMASK(3, 0)

/* cmn_reg005D */
#define OVRD_ROPLL_REF_CLK_SEL			BIT(5)
#define ROPLL_REF_CLK_SEL			GENMASK(4, 3)

/* cmn_reg005E */
#define ANA_ROPLL_SDM_EN			BIT(6)
#define OVRD_ROPLL_SDM_RSTN			BIT(5)
#define ROPLL_SDM_RSTN				BIT(4)
#define ROPLL_SDC_FRACTIONAL_EN_RBR		BIT(3)
#define ROPLL_SDC_FRACTIONAL_EN_HBR		BIT(2)
#define ROPLL_SDC_FRACTIONAL_EN_HBR2		BIT(1)
#define ROPLL_SDC_FRACTIONAL_EN_HBR3		BIT(0)

/* cmn_reg005F */
#define OVRD_ROPLL_SDC_RSTN			BIT(5)
#define ROPLL_SDC_RSTN				BIT(4)

/* cmn_reg0060 */
#define ROPLL_SDM_DENOMINATOR			GENMASK(7, 0)

/* cmn_reg0064 */
#define ROPLL_SDM_NUMERATOR_SIGN_RBR		BIT(3)
#define ROPLL_SDM_NUMERATOR_SIGN_HBR		BIT(2)
#define ROPLL_SDM_NUMERATOR_SIGN_HBR2		BIT(1)
#define ROPLL_SDM_NUMERATOR_SIGN_HBR3		BIT(0)

/* cmn_reg0065 */
#define ROPLL_SDM_NUMERATOR			GENMASK(7, 0)

/* cmn_reg0069 */
#define ROPLL_SDC_N_RBR				GENMASK(2, 0)

/* cmn_reg006A */
#define ROPLL_SDC_N_HBR				GENMASK(5, 3)
#define ROPLL_SDC_N_HBR2			GENMASK(2, 0)

/* cmn_reg006B */
#define ROPLL_SDC_N_HBR3			GENMASK(3, 1)

/* cmn_reg006C */
#define ROPLL_SDC_NUMERATOR			GENMASK(5, 0)

/* cmn_reg0070 */
#define ROPLL_SDC_DENOMINATOR			GENMASK(5, 0)

/* cmn_reg0074 */
#define OVRD_ROPLL_SDC_NDIV_RSTN		BIT(3)
#define ROPLL_SDC_NDIV_RSTN			BIT(2)
#define OVRD_ROPLL_SSC_EN			BIT(1)
#define ROPLL_SSC_EN				BIT(0)

/* cmn_reg0075 */
#define ANA_ROPLL_SSC_FM_DEVIATION		GENMASK(5, 0)

/* cmn_reg0076 */
#define ANA_ROPLL_SSC_FM_FREQ			GENMASK(6, 2)

/* cmn_reg0077 */
#define ANA_ROPLL_SSC_CLK_DIV_SEL		GENMASK(6, 3)

/* cmn_reg0081 */
#define ANA_PLL_CD_TX_SER_RATE_SEL		BIT(3)
#define ANA_PLL_CD_HSCLK_WEST_EN		BIT(1)
#define ANA_PLL_CD_HSCLK_EAST_EN		BIT(0)

/* cmn_reg0082 */
#define ANA_PLL_CD_VREG_GAIN_CTRL		GENMASK(3, 0)

/* cmn_reg0083 */
#define ANA_PLL_CD_VREG_ICTRL			GENMASK(6, 5)

/* cmn_reg0084 */
#define PLL_LCRO_CLK_SEL			BIT(5)

/* cmn_reg0085 */
#define ANA_PLL_SYNC_LOSS_DET_MODE		GENMASK(1, 0)

/* cmn_reg0087 */
#define ANA_PLL_TX_HS_CLK_EN			BIT(2)

/* cmn_reg0095 */
#define DP_TX_LINK_BW				GENMASK(1, 0)

/* cmn_reg0097 */
#define DIG_CLK_SEL				BIT(1)

/* cmn_reg0099 */
#define SSC_EN					GENMASK(7, 6)
#define CMN_ROPLL_ALONE_MODE			BIT(2)

/* cmn_reg009A */
#define HS_SPEED_SEL				BIT(0)

/* cmn_reg009B */
#define LS_SPEED_SEL				BIT(4)

/* sb_reg0102 */
#define OVRD_SB_RXTERM_EN			BIT(5)
#define SB_RXRERM_EN				BIT(4)
#define ANA_SB_RXTERM_OFFSP			GENMASK(3, 0)

/* sb_reg0103 */
#define ANA_SB_RXTERM_OFFSN			GENMASK(6, 3)
#define OVRD_SB_RX_RESCAL_DONE			BIT(1)
#define SB_RX_RESCAL_DONE			BIT(0)

/* sb_reg0104 */
#define OVRD_SB_EN				BIT(5)
#define SB_EN					BIT(4)
#define OVRD_SB_AUX_EN				BIT(1)
#define SB_AUX_EN				BIT(0)

/* sb_reg0105 */
#define ANA_SB_TX_HLVL_PROG			GENMASK(2, 0)

/* sb_reg0106 */
#define ANA_SB_TX_LLVL_PROG			GENMASK(6, 4)

/* sb_reg010D */
#define ANA_SB_DMRX_LPBK_DATA			BIT(4)

/* sb_reg010F */
#define OVRD_SB_VREG_EN				BIT(7)
#define SB_VREG_EN				BIT(6)
#define ANA_SB_VREG_GAIN_CTRL			GENMASK(3, 0)

/* sb_reg0110 */
#define ANA_SB_VREG_OUT_SEL			BIT(1)
#define ANA_SB_VREG_REF_SEL			BIT(0)

/* sb_reg0113 */
#define SB_RX_RCAL_OPT_CODE			GENMASK(5, 4)
#define SB_RX_RTERM_CTRL			GENMASK(3, 0)

/* sb_reg0114 */
#define SB_TG_SB_EN_DELAY_TIME			GENMASK(5, 3)
#define SB_TG_RXTERN_EN_DELAY_TIME		GENMASK(2, 0)

/* sb_reg0115 */
#define SB_READY_DELAY_TIME			GENMASK(5, 3)
#define SB_TG_OSC_EN_DELAY_TIME			GENMASK(2, 0)

/* sb_reg0116 */
#define SB_TG_OSC_EN_TO_AFC_RSTN_DELAT_TIME	GENMASK(6, 4)

/* sb_reg0117 */
#define SB_TG_PLL_CD_VREG_FAST_PULSE_TIME	GENMASK(3, 0)

/* sb_reg0118 */
#define SB_TG_EARC_DMRX_RECVRD_CLK_CNT		GENMASK(7, 0)

/* sb_reg011A */
#define SB_TG_CNT_RUN_NO_7_0			GENMASK(7, 0)

/* sb_reg011B */
#define SB_EARC_SIG_DET_BYPASS			BIT(4)
#define SB_AFC_TOL				GENMASK(3, 0)

/* sb_reg011C */
#define SB_AFC_STB_NUM				GENMASK(3, 0)

/* sb_reg011D */
#define SB_TG_OSC_CNT_MIN			GENMASK(7, 0)

/* sb_reg011E */
#define SB_TG_OSC_CNT_MAX			GENMASK(7, 0)

/* sb_reg011F */
#define SB_PWM_AFC_CTRL				GENMASK(7, 2)
#define SB_RCAL_RSTN				BIT(1)

/* sb_reg0120 */
#define SB_AUX_EN_IN				BIT(7)

/* sb_reg0123 */
#define OVRD_SB_READY				BIT(5)
#define SB_READY				BIT(4)

/* lntop_reg0200 */
#define PROTOCOL_SEL				BIT(2)

/* lntop_reg0206 */
#define DATA_BUS_WIDTH				GENMASK(2, 1)
#define BUS_WIDTH_SEL				BIT(0)

/* lntop_reg0207 */
#define LANE_EN					GENMASK(3, 0)

/* LANE_reg0301 */
#define OVRD_LN_TX_DRV_EI_EN			BIT(7)
#define LN_TX_DRV_EI_EN				BIT(6)

/* LANE_reg0303 */
#define OVRD_LN_TX_DRV_LVL_CTRL			BIT(5)
#define LN_TX_DRV_LVL_CTRL			GENMASK(4, 0)

/* LANE_reg0304 */
#define OVRD_LN_TX_DRV_POST_LVL_CTRL    BIT(4)
#define LN_TX_DRV_POST_LVL_CTRL     GENMASK(3, 0)
#define LN_TX_DRV_POST_LVL_CTRL			GENMASK(3, 0)

/* LANE_reg0305 */
#define OVRD_LN_TX_DRV_PRE_LVL_CTRL		BIT(6)
#define LN_TX_DRV_PRE_LVL_CTRL			GENMASK(5, 2)

/* LANE_reg0306 */
#define LN_ANA_TX_DRV_IDRV_IDN_CTRL		GENMASK(7, 5)
#define LN_ANA_TX_DRV_IDRV_IUP_CTRL		GENMASK(4, 2)
#define LN_ANA_TX_DRV_ACCDRV_EN			BIT(0)

/* LANE_reg0307 */
#define LN_ANA_TX_DRV_ACCDRV_POL_SEL		BIT(6)
#define LN_ANA_TX_DRV_ACCDRV_CTRL		GENMASK(5, 3)

/* LANE_reg030A */
#define LN_ANA_TX_JEQ_EN			BIT(4)
#define LN_TX_JEQ_EVEN_CTRL_RBR			GENMASK(3, 0)

/* LANE_reg030B */
#define LN_TX_JEQ_EVEN_CTRL_HBR			GENMASK(7, 4)
#define LN_TX_JEQ_EVEN_CTRL_HBR2		GENMASK(3, 0)

/* LANE_reg030C */
#define LN_TX_JEQ_EVEN_CTRL_HBR3		GENMASK(7, 4)
#define LN_TX_JEQ_ODD_CTRL_RBR			GENMASK(3, 0)

/* LANE_reg030D */
#define LN_TX_JEQ_ODD_CTRL_HBR			GENMASK(7, 4)
#define LN_TX_JEQ_ODD_CTRL_HBR2			GENMASK(3, 0)

/* LANE_reg030E */
#define LN_TX_JEQ_ODD_CTRL_HBR3			GENMASK(7, 4)

/* LANE_reg0310 */
#define LN_ANA_TX_SYNC_LOSS_DET_MODE		GENMASK(1, 0)

/* LANE_reg0311 */
#define LN_TX_SER_40BIT_EN_RBR			BIT(3)
#define LN_TX_SER_40BIT_EN_HBR			BIT(2)
#define LN_TX_SER_40BIT_EN_HBR2			BIT(1)
#define LN_TX_SER_40BIT_EN_HBR3			BIT(0)

/* LANE_reg0316 */
#define LN_ANA_TX_SER_VREG_GAIN_CTRL		GENMASK(3, 0)

/* LANE_reg031B */
#define LN_ANA_TX_RESERVED			GENMASK(7, 0)

/* LANE_reg031E */
#define LN_POLARITY_INV				BIT(2)

#define LANE_REG(LANE, offset)			(0x400 * (LANE) + (offset))

enum {
  DP_BW_RBR,
  DP_BW_HBR,
  DP_BW_HBR2,
  DP_BW_HBR3,
};

struct TxDrvCtrl {
  UINT8 Tx_Drv_Lvl_Ctrl;
  UINT8 Tx_Drv_Post_Lvl_Ctrl;
  UINT8 Ana_Tx_Drv_Idrv_Idn_Ctrl;
  UINT8 Ana_Tx_Drv_Idrv_Iup_Ctrl;
  UINT8 Ana_Tx_Drv_Accdrv_En;
  UINT8 Ana_Tx_Drv_Accdrv_Ctrl;
};

STATIC CONST struct TxDrvCtrl TX_DRV_CTRL_RBR[4][4] = {
  /* VOLTAGE swing 0, pre-emphasis 0->3 */
  {
    { 0x1, 0x0, 0x4, 0x6, 0x0, 0x4 },
    { 0x4, 0x3, 0x4, 0x6, 0x0, 0x4 },
    { 0x7, 0x6, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0xb, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 1, pre-emphasis 0->2 */
  {
    { 0x4, 0x0, 0x4, 0x6, 0x0, 0x4 },
    { 0xa, 0x5, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0x8, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 2, pre-emphasis 0->1 */
  {
    { 0x8, 0x0, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0x5, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 3, pre-emphasis 0 */
  {
    { 0xd, 0x0, 0x7, 0x7, 0x1, 0x4 },
  }
};

STATIC CONST struct TxDrvCtrl TX_DRV_CTRL_HBR[4][4] = {
  /* VOLTAGE swing 0, pre-emphasis 0->3 */
  {
    { 0x2, 0x1, 0x4, 0x6, 0x0, 0x4 },
    { 0x5, 0x4, 0x4, 0x6, 0x0, 0x4 },
    { 0x9, 0x8, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0xb, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 1, pre-emphasis 0->2 */
  {
    { 0x6, 0x1, 0x4, 0x6, 0x0, 0x4 },
    { 0xb, 0x6, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0x8, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 2, pre-emphasis 0->1 */
  {
    { 0x9, 0x1, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0x6, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 3, pre-emphasis 0 */
  {
    { 0xd, 0x1, 0x7, 0x7, 0x1, 0x4 },
  }
};

STATIC CONST struct TxDrvCtrl TX_DRV_CTRL_HBR2[4][4] = {
  /* VOLTAGE swing 0, pre-emphasis 0->3 */
  {
    { 0x2, 0x1, 0x4, 0x6, 0x0, 0x4 },
    { 0x5, 0x4, 0x4, 0x6, 0x0, 0x4 },
    { 0x9, 0x8, 0x4, 0x6, 0x1, 0x4 },
    { 0xd, 0xb, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 1, pre-emphasis 0->2 */
  {
    { 0x6, 0x1, 0x4, 0x6, 0x0, 0x4 },
    { 0xc, 0x7, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0x8, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 2, pre-emphasis 0->1 */
  {
    { 0x9, 0x1, 0x4, 0x6, 0x0, 0x4 },
    { 0xd, 0x6, 0x7, 0x7, 0x1, 0x7 },
  },

  /* VOLTAGE swing 3, pre-emphasis 0 */
  {
    { 0xd, 0x0, 0x7, 0x7, 0x1, 0x4 },
  }
};

INLINE
VOID
PhyWrite (
  OUT struct RockchipHdptxPhy *Hdptx,
  UINTN Reg,
  UINTN Val
  )
{
  UINT32 Shift;

  if (!Hdptx->Id)
    Shift = EDP0TX_PHY_BASE;
  else
    Shift = EDP1TX_PHY_BASE;

  MmioWrite32(Shift + Reg, Val );
}

INLINE
UINTN
PhyRead (
  OUT struct RockchipHdptxPhy *Hdptx,
  UINTN Reg
  )
{
  UINT32 Shift;

  if(!Hdptx->Id)
    Shift = EDP0TX_PHY_BASE;
  else
    Shift = EDP1TX_PHY_BASE;

  return MmioRead32(Shift + Reg);
}

STATIC 
VOID 
PhyUpdateBits (
  OUT struct RockchipHdptxPhy *Hdptx, 
  UINTN Reg,
  UINTN Mask, 
  UINTN Val
  )
{
  UINTN Orig, Tmp;

  Orig = PhyRead(Hdptx, Reg);
  Tmp = Orig & ~Mask;
  Tmp |= Val & Mask;
  PhyWrite(Hdptx, Reg, Tmp);
};

STATIC
VOID
GrfWrite (
  OUT struct RockchipHdptxPhy *Hdptx,
  UINTN Reg,
  UINTN Mask,
  UINTN Val
  )
{
  UINT32 TempVal = 0;
  UINT32 Shift;

  if (!Hdptx->Id)
    Shift = HDPTXPHY0_GRF_BASE;
  else
    Shift = HDPTXPHY1_GRF_BASE;

  TempVal = (Mask << 16) | (Val & Mask);
  MmioWrite32(Shift + Reg, TempVal);
};

STATIC
VOID
RockchipHdptxPhyDpPllInit (
  struct RockchipHdptxPhy *Hdptx
  )
{
  PhyUpdateBits(Hdptx, 0x20, OVRD_LCPLL_EN |
      LCPLL_EN,
      0x80); 
  PhyWrite(Hdptx, 0xf4, 0xC0);
  PhyUpdateBits(Hdptx, 0x0138, ANA_ROPLL_PI_EN,
      0x20);
  PhyWrite(Hdptx, 0x0144, 0x87);
  PhyWrite(Hdptx, 0x0148, 0x71);
  PhyWrite(Hdptx, 0x014c, 0x71);
  PhyWrite(Hdptx, 0x0154, 0x87);
  PhyWrite(Hdptx, 0x0158, 0x71);
  PhyWrite(Hdptx, 0x015c, 0x71);
  PhyWrite(Hdptx, 0x0164, 0x11);
  PhyWrite(Hdptx, 0x0168, 0x31);
  PhyUpdateBits(Hdptx, 0x016c, ROPLL_PMS_SDIV_HBR2, 0x0);
  PhyWrite(Hdptx, 0x0178, 0x7f);
  PhyWrite(Hdptx, 0x017c, 0x31);
  PhyWrite(Hdptx, 0x0180, 0x21);
  PhyWrite(Hdptx, 0x0184, 0x27);
  PhyWrite(Hdptx, 0x0188, 0x27);
  PhyWrite(Hdptx, 0x0190, 0x7);
  PhyWrite(Hdptx, 0x0194, 0x0);
  PhyWrite(Hdptx, 0x0198, 0xd);
  PhyWrite(Hdptx, 0x019c, 0xd);
  PhyWrite(Hdptx, 0x01a4, 0x2);
  PhyWrite(Hdptx, 0x01a8, 0x09);
  PhyWrite(Hdptx, 0x01b0, 0x3);
  PhyWrite(Hdptx, 0x01b4, 0x7);
  PhyWrite(Hdptx, 0x01b8, 0x7);
  PhyWrite(Hdptx, 0x01c0, 0x8);
  PhyWrite(Hdptx, 0x01c4, 0x18);
  PhyWrite(Hdptx, 0x01c8, 0x18);
  PhyWrite(Hdptx, 0x01d0, 0x0f);
  PhyWrite(Hdptx, 0x01dc, 0x08);
  PhyWrite(Hdptx, 0x0118, 0xee);
  PhyWrite(Hdptx, 0x011c, 0x24);
  PhyWrite(Hdptx, 0x0204, 0x01);
  PhyUpdateBits(Hdptx, 0x025c, DIG_CLK_SEL, 0x2);
  PhyUpdateBits(Hdptx, 0x021c, ANA_PLL_TX_HS_CLK_EN, 0x4);
  PhyUpdateBits(Hdptx, 0x0204, ANA_PLL_CD_HSCLK_EAST_EN |
      ANA_PLL_CD_HSCLK_WEST_EN,
      0x1);
  PhyWrite(Hdptx, 0x0264, 0x84);
  PhyWrite(Hdptx, 0x0208, 0x04);
  PhyUpdateBits(Hdptx, 0x00f0, ANA_LCPLL_RESERVED7,
      0x80);
  PhyWrite(Hdptx, 0x020c, 0x24);
  PhyWrite(Hdptx, 0x0214, 0x03);
  PhyUpdateBits(Hdptx, 0x0210, PLL_LCRO_CLK_SEL,
      0x20);
  PhyUpdateBits(Hdptx, 0x0268, HS_SPEED_SEL,
      0x1);
  PhyUpdateBits(Hdptx, 0x026c, LS_SPEED_SEL,
      0x10);
}

STATIC
INTN
RockchipHdptxPhyDpAuxInit (
  struct RockchipHdptxPhy *Hdptx
  )
{
  PhyUpdateBits(Hdptx, 0x0414, ANA_SB_TX_HLVL_PROG, 0x7);
  PhyUpdateBits(Hdptx, 0x0418, ANA_SB_TX_LLVL_PROG, 0x70);
  PhyWrite(Hdptx, 0x044c, 0x13);
  PhyWrite(Hdptx, 0x0450, 0x12);
  PhyWrite(Hdptx, 0x0454, 0x12);
  PhyWrite(Hdptx, 0x0458, 0x20);
  PhyWrite(Hdptx, 0x045c, 0x04);
  PhyWrite(Hdptx, 0x0460, 0x0a);
  PhyWrite(Hdptx, 0x0468, 0x03);
  PhyWrite(Hdptx, 0x046c, 0x13);
  PhyWrite(Hdptx, 0x0470, 0x04);
  PhyUpdateBits(Hdptx, 0x0474, SB_TG_OSC_CNT_MIN, 0x67);
  PhyUpdateBits(Hdptx, 0x0478, SB_TG_OSC_CNT_MAX, 0x6a);
  PhyWrite(Hdptx, 0x047c, 0x16);
  PhyUpdateBits(Hdptx, 0x0434, ANA_SB_DMRX_LPBK_DATA, 0x10);
  PhyWrite(Hdptx, 0x0440, 0x03);
  PhyUpdateBits(Hdptx, 0x043c, ANA_SB_VREG_GAIN_CTRL, 0x0);
  PhyUpdateBits(Hdptx, 0x0408, ANA_SB_RXTERM_OFFSP, 0x3);
  PhyWrite(Hdptx, 0x040c, 0x1b);
  PhyUpdateBits(Hdptx, 0x047c, SB_RCAL_RSTN, 0x2);
  PhyUpdateBits(Hdptx, 0x0410, SB_AUX_EN, 0x0);
  PhyUpdateBits(Hdptx, 0x0480, SB_AUX_EN_IN, 0x80);
  PhyUpdateBits(Hdptx, 0x0410, OVRD_SB_EN, 0x20);
  PhyUpdateBits(Hdptx, 0x0408, OVRD_SB_RXTERM_EN, 0x20);
  PhyUpdateBits(Hdptx, 0x0410, OVRD_SB_AUX_EN, 0x2);

  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, BGR_EN, 0x21);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, BIAS_EN, 0x61);
  MicroSecondDelay(10);

  PhyUpdateBits(Hdptx, 0x0410, SB_EN, 0x10);
  PhyUpdateBits(Hdptx, 0x0408, SB_RXRERM_EN, 0x10);
  PhyWrite(Hdptx, 0x043c, 0xc0);
  PhyUpdateBits(Hdptx, 0x0410, SB_AUX_EN, 0x1);
  NanoSecondDelay (100000);
  GrfWrite(Hdptx, HDPTXPHY_GRF_STATUS0, SB_RDY, 0x1);
  MicroSecondDelay(10);
  return 0;
}

STATIC
VOID
RockchipHdptxPhyReset (
  struct RockchipHdptxPhy *Hdptx
  )
{
  UINT32 LANE;

  for (LANE = 0; LANE < 4; LANE++)
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c04),
        OVRD_LN_TX_DRV_EI_EN | LN_TX_DRV_EI_EN,
        0x80);

  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, PLL_EN, 0x0);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, BIAS_EN, 0x0);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, BGR_EN, 0x0);
};

VOID
RockchipHdptxPhyInit (
  OUT struct RockchipHdptxPhy *Hdptx
  )
{
  RockchipHdptxPhyReset(Hdptx);

  MmioWrite32(0xFD7F0A0C, 0xFFFF8000);

  /* Todo lane-polarity-invert  */
  PhyWrite(Hdptx, 0x0C78, 0x00);
  PhyWrite(Hdptx, 0x1078, 0x00);
  PhyWrite(Hdptx, 0x1478, 0x00);
  PhyWrite(Hdptx, 0x1878, 0x00);

  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, HDPTX_MODE_SEL, 0x01);

  /* Todo RockchipHdptxPhyPowerOn */
  PhyWrite(Hdptx, 0x0800, 0x00);
  PhyWrite(Hdptx, 0x0818, 0x02);

  RockchipHdptxPhyDpPllInit(Hdptx);
  RockchipHdptxPhyDpAuxInit(Hdptx);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, BGR_EN, 0x21);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, BIAS_EN, 0x61);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, PLL_EN, 0xe1);
  MmioWrite32(0xFD7F0A0C, 0xFFFF8000);
  MmioWrite32(0xFD7F0A0C, 0xFFFF0000);
  MmioWrite32(0xFD7F0A10, 0xFFFF0000);
};

INTN
RockchipHdptxPhySetRate (
  OUT struct RockchipHdptxPhy *Hdptx,
  OUT struct PhyConfigureOptsDp *Dp
  )
{
  UINT32 BW;

	GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, PLL_EN, 0x0);
	PhyUpdateBits(Hdptx, 0x081c, LANE_EN, 0x0);
  DEBUG ((DEBUG_INFO, "[DRIVER]enter %a rate %d Dp->SSC %d \n",
        __func__, Dp->LINKRATE, Dp->SSC));

  switch (Dp->LINKRATE) {
  case 1620:
    BW = DP_BW_RBR;
    break;
  case 2700:
    BW = DP_BW_HBR;
    break;
  case 5400:
    BW = DP_BW_HBR2;
    break;
  default:
    return -EINVAL;
  }

  PhyUpdateBits(Hdptx, 0x0254, DP_TX_LINK_BW, BW);

  if (Dp->SSC) {
    PhyUpdateBits(Hdptx, 0x01d0, OVRD_ROPLL_SSC_EN | ROPLL_SSC_EN,
        0x3);
    PhyUpdateBits(Hdptx, 0x01d4, ANA_ROPLL_SSC_FM_DEVIATION,
        0xc);
    PhyUpdateBits(Hdptx, 0x01d8, ANA_ROPLL_SSC_FM_FREQ,
        0x7c);
    PhyUpdateBits(Hdptx, 0x0264, SSC_EN,
        0x80);
  } else {
    PhyUpdateBits(Hdptx, 0x01d0, OVRD_ROPLL_SSC_EN | ROPLL_SSC_EN,
        0x3);
    PhyUpdateBits(Hdptx, 0x01d4, ANA_ROPLL_SSC_FM_DEVIATION,
        0xe);
    PhyUpdateBits(Hdptx, 0x01d8, ANA_ROPLL_SSC_FM_FREQ,
        0xd);
    PhyUpdateBits(Hdptx, 0x0264, SSC_EN,
        0x20);
    PhyUpdateBits(Hdptx, 0x01d8, ANA_ROPLL_SSC_FM_FREQ,
        0x30);
  }
  PhyWrite(Hdptx, 0x081C, 0x03);
  PhyWrite(Hdptx, 0x0C0C, 0x26);
  PhyWrite(Hdptx, 0x0C10, 0x11);
  PhyWrite(Hdptx, 0x100C, 0x26);
  PhyWrite(Hdptx, 0x101C, 0x60);
  PhyWrite(Hdptx, 0x140C, 0x00);
  PhyWrite(Hdptx, 0x1410, 0x00);
  PhyWrite(Hdptx, 0x141C, 0x00);
  PhyWrite(Hdptx, 0x1428, 0x00);
  PhyWrite(Hdptx, 0x142C, 0x00);
  PhyWrite(Hdptx, 0x1434, 0x00);
  PhyWrite(Hdptx, 0x1444, 0x00);
  PhyWrite(Hdptx, 0x1458, 0x00);
  PhyWrite(Hdptx, 0x146C, 0x00);
  PhyWrite(Hdptx, 0x180C, 0x00);
  PhyWrite(Hdptx, 0x1810, 0x00);
  PhyWrite(Hdptx, 0x181C, 0x00);
  PhyWrite(Hdptx, 0x1828, 0x00);
  PhyWrite(Hdptx, 0x182C, 0x00);
  PhyWrite(Hdptx, 0x1834, 0x00);
  PhyWrite(Hdptx, 0x1844, 0x00);
  PhyWrite(Hdptx, 0x1858, 0x00);
  PhyWrite(Hdptx, 0x186C, 0x00);
  GrfWrite(Hdptx, HDPTXPHY_GRF_CON0, PLL_EN, 0x80);
  GrfWrite(Hdptx, HDPTXPHY_GRF_STATUS0, PLL_LOCK_DONE, 0x8);
  PhyUpdateBits(Hdptx, 0x081c, LANE_EN,
      GENMASK(Dp->LANES - 1, 0));
  NanoSecondDelay (100000);
  GrfWrite(Hdptx, HDPTXPHY_GRF_STATUS0, PHY_RDY, 0x2);
  NanoSecondDelay (10000);
  return 0;
}

VOID
RockchipHdptxPhySetVoltage (
  OUT struct RockchipHdptxPhy *Hdptx,
  OUT struct PhyConfigureOptsDp *Dp,
  UINT8 LANE
  )
{
  CONST struct TxDrvCtrl *ctrl;
  DEBUG ((DEBUG_INFO, "[DRIVER]enter %a Dp->LINKRATE %d \n",
        __func__, Dp->LINKRATE));

  switch (Dp->LINKRATE) {
  case 1620:
    ctrl = &TX_DRV_CTRL_RBR[Dp->VOLTAGE[LANE]][Dp->PRE[LANE]];
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c44),
        LN_TX_SER_40BIT_EN_RBR,
        0x8);
    break;
  case 2700:
    ctrl = &TX_DRV_CTRL_HBR[Dp->VOLTAGE[LANE]][Dp->PRE[LANE]];
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c2c),
        LN_TX_JEQ_EVEN_CTRL_HBR,
        0x70);
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c34),
        LN_TX_JEQ_ODD_CTRL_HBR,
        0x70);
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c44),
        LN_TX_SER_40BIT_EN_HBR,
        0x4);
    break;
  case 5400:
  default:
    ctrl = &TX_DRV_CTRL_HBR2[Dp->VOLTAGE[LANE]][Dp->PRE[LANE]];
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c2c),
        LN_TX_JEQ_EVEN_CTRL_HBR2,
        0x7);
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c34),
        LN_TX_JEQ_ODD_CTRL_HBR2,
        0x7);
    PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c44),
        LN_TX_SER_40BIT_EN_HBR2,
        0x2);
    break;
  }
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c0c),
      OVRD_LN_TX_DRV_LVL_CTRL | LN_TX_DRV_LVL_CTRL,
      0x2d);
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c10),
      OVRD_LN_TX_DRV_POST_LVL_CTRL | LN_TX_DRV_POST_LVL_CTRL,
      0x10 | ctrl->Tx_Drv_Post_Lvl_Ctrl
      );
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c18),
      LN_ANA_TX_DRV_IDRV_IDN_CTRL |
      LN_ANA_TX_DRV_IDRV_IUP_CTRL |
      LN_ANA_TX_DRV_ACCDRV_EN,
      0x00); 
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c1c),
      LN_ANA_TX_DRV_ACCDRV_POL_SEL | LN_ANA_TX_DRV_ACCDRV_CTRL,
      0x40 | ctrl->Ana_Tx_Drv_Accdrv_Ctrl);
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c6c),
      LN_ANA_TX_RESERVED,
      0x1);
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c58),
      LN_ANA_TX_SER_VREG_GAIN_CTRL,
      0x2);
  PhyUpdateBits(Hdptx, LANE_REG(LANE, 0x0c40),
      LN_ANA_TX_SYNC_LOSS_DET_MODE,
      0x3);
}

INTN 
RockchipHdptxPhySetVoltages (
  OUT struct RockchipHdptxPhy *Hdptx,
  OUT struct PhyConfigureOptsDp *Dp
  )
{
  UINT8 LANE;

  for (LANE = 0; LANE < Dp->LANES; LANE++)
    RockchipHdptxPhySetVoltage(Hdptx, Dp, LANE);

  return 0;
}

STATIC
INTN
RockchipHdptxPhyVerifyConfig (
  OUT struct RockchipHdptxPhy *Hdptx,
  OUT struct PhyConfigureOptsDp *Dp
  )
{
  INTN i;

  if (Dp->SETRATE) {
    switch (Dp->LINKRATE) {
    case 1620:
    case 2700:
    case 5400:
      break;
    default:
      return -EINVAL;
    }
  }

  switch (Dp->LANES) {
  case 1:
  case 2:
  case 4:
    break;
  default:
    return -EINVAL;
  }

  if (Dp->SETVOLTAGES) {
    for (i = 0; i < Dp->LANES; i++) {
      if (Dp->VOLTAGE[i] > 3 || Dp->PRE[i] > 3)
        return -EINVAL;

      if (Dp->VOLTAGE[i] + Dp->PRE[i] > 3)
        return -EINVAL;
		}
	}

  return 0;
}

INTN 
RockchipHdptxPhyConfigure (
  OUT struct RockchipHdptxPhy *Hdptx,
  OUT struct PhyConfigureOptsDp *Dp
  )
{
  INTN Ret;

  Ret = RockchipHdptxPhyVerifyConfig(Hdptx, Dp);
  if (Ret) {
    DEBUG ((DEBUG_WARN, "Invalid params for phy configure\n"));
    return Ret;
  }

  if (Dp->SETRATE) {
    Ret = RockchipHdptxPhySetRate(Hdptx, Dp);
    if (Ret) {
      DEBUG ((DEBUG_WARN, "Failed to set rate: %d\n",Ret));
      return Ret;
    }
  }

  if (Dp->SETVOLTAGES) {
    Ret = RockchipHdptxPhySetVoltages(Hdptx, Dp);
    if (Ret) {
      DEBUG ((DEBUG_WARN, "Failed to set Voltages: %d\n",Ret));
      return Ret;
    }
  }

  return 0;
}
