/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */
#include "Include/Library/CruLib.h"
#include <Library/DebugLib.h>
/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRU
 *  @{
 */

/** @defgroup CRU_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/

static struct PLL_CONFIG PLL_TABLE[] = {
    /* _mhz, _refDiv, _fbDiv, _postdDv1, _postDiv2, _dsmpd, _frac */
    RK_PLL_RATE(1608000000, 1, 67, 1, 1, 1, 0),
    RK_PLL_RATE(1512000000, 1, 126, 2, 1, 1, 0),
    RK_PLL_RATE(1416000000, 1, 118, 2, 1, 1, 0),
    RK_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
    RK_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
    RK_PLL_RATE(1000000000, 3, 250, 2, 1, 1, 0),
    RK_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
    RK_PLL_RATE(200000000, 1, 100, 3, 4, 1, 0),
    { /* sentinel */ },
};

static struct PLL_SETUP APLL = {
    .conOffset0 = &(CRU->APLL_CON[0]),
    .conOffset1 = &(CRU->APLL_CON[1]),
    .conOffset2 = &(CRU->APLL_CON[2]),
    .conOffset3 = &(CRU->APLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP CPLL = {
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .conOffset3 = &(CRU->CPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 4,
    .lockShift = 10,
    .modeMask = 0x3 << 4,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL = {
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .conOffset3 = &(CRU->GPLL_CON[3]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 6,
    .lockShift = 10,
    .modeMask = 0x3 << 6,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP NPLL = {
    .conOffset0 = &(CRU->NPLL_CON[0]),
    .conOffset1 = &(CRU->NPLL_CON[1]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 10,
    .lockShift = 10,
    .modeMask = 0x3 << 10,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP VPLL = {
    .conOffset0 = &(CRU->VPLL_CON[0]),
    .conOffset1 = &(CRU->VPLL_CON[1]),
    .modeOffset = &(CRU->MODE_CON00),
    .modeShift = 12,
    .lockShift = 10,
    .modeMask = 0x3 << 12,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP PPLL = {
    .conOffset0 = &(PMUCRU->PPLL_CON[0]),
    .conOffset1 = &(PMUCRU->PPLL_CON[1]),
    .conOffset2 = &(PMUCRU->PPLL_CON[2]),
    .conOffset3 = &(PMUCRU->PPLL_CON[3]),
    .modeOffset = &(PMUCRU->MODE_CON00),
    .modeShift = 0,
    .lockShift = 10,
    .modeMask = 0x3 << 0,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP HPLL = {
    .conOffset0 = &(PMUCRU->HPLL_CON[0]),
    .conOffset1 = &(PMUCRU->HPLL_CON[1]),
    .conOffset2 = &(PMUCRU->HPLL_CON[2]),
    .conOffset3 = &(PMUCRU->HPLL_CON[3]),
    .modeOffset = &(PMUCRU->MODE_CON00),
    .modeShift = 2,
    .lockShift = 10,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

/********************* Private Variable Definition ***************************/

static uint32_t s_apllFreq;
static uint32_t s_cpllFreq;
static uint32_t s_gpllFreq = 1188 * 1000 * 1000;
static uint32_t s_npllFreq;
static uint32_t s_vpllFreq;
static uint32_t s_ppllFreq;
static uint32_t s_hpllFreq;
static uint32_t s_clk_gmac_xpcs_mii;
static uint32_t s_clk_gmac_in;

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CRU_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief Get frac clk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 * How to calculate the Frac clk divider:
 *     numerator is frac register[31:16]
 *     denominator is frac register[15:0]
 *     clk rate = pRate * numerator / denominator
 *     for a better jitter, pRate > 20 * rate
 */
static uint32_t HAL_CRU_ClkFracGetFreq(eCLOCK_Name clockName)
{
    uint32_t freq = 0;
    uint32_t muxSrc = 0, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc = 0, divFrac = 0;
    uint32_t n, m, pRate;

    switch (clockName) {
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        break;
    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        break;
    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        break;
    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        break;
    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_SRC);
        divSrc = CLK_GET_DIV(CLK_UART6_SRC);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        break;
    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_SRC);
        divSrc = CLK_GET_DIV(CLK_UART7_SRC);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        break;
    case CLK_UART8:
        muxSrc = CLK_GET_MUX(CLK_UART8_SRC);
        divSrc = CLK_GET_DIV(CLK_UART8_SRC);
        divFrac = CLK_GET_DIV(CLK_UART8_FRAC);
        break;
    case CLK_UART9:
        muxSrc = CLK_GET_MUX(CLK_UART9_SRC);
        divSrc = CLK_GET_DIV(CLK_UART9_SRC);
        divFrac = CLK_GET_DIV(CLK_UART9_FRAC);
        break;
    default:

        return 0;
    }

    n = (CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0xffff0000) >> 16;
    m = CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] & 0x0000ffff;

    if (HAL_CRU_ClkGetMux(muxSrc)) {
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    } else {
        pRate = s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc);
    }

    if (HAL_CRU_ClkGetMux(mux) == 0) {
        freq = pRate;
    } else if (HAL_CRU_ClkGetMux(mux) == 1) {
        freq = (pRate / m) * n;
    } else if (HAL_CRU_ClkGetMux(mux) == 2) {
        freq = PLL_INPUT_OSC_RATE;
    }

    return freq;
}

/**
 * @brief Set frac clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 * How to calculate the Frac clk divider:
 *     if pRate > 20 * rate, select frac divider
 *     else select normal divider, but the clk rate may be not accurate
 */
static HAL_Status HAL_CRU_ClkFracSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t n = 0, m = 0;
    uint32_t gateId, fracGateId;

    switch (clockName) {
    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        gateId = CLK_UART1_GATE;
        fracGateId = CLK_UART1_FRAC_GATE;
        break;
    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        gateId = CLK_UART2_GATE;
        fracGateId = CLK_UART2_FRAC_GATE;
        break;
    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        gateId = CLK_UART3_GATE;
        fracGateId = CLK_UART3_FRAC_GATE;
        break;
    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        gateId = CLK_UART4_GATE;
        fracGateId = CLK_UART4_FRAC_GATE;
        break;
    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        gateId = CLK_UART5_GATE;
        fracGateId = CLK_UART5_FRAC_GATE;
        break;
    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_SRC);
        divSrc = CLK_GET_DIV(CLK_UART6_SRC);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        gateId = CLK_UART6_GATE;
        fracGateId = CLK_UART6_FRAC_GATE;
        break;
    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_SRC);
        divSrc = CLK_GET_DIV(CLK_UART7_SRC);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        gateId = CLK_UART7_GATE;
        fracGateId = CLK_UART7_FRAC_GATE;
        break;
    case CLK_UART8:
        muxSrc = CLK_GET_MUX(CLK_UART8_SRC);
        divSrc = CLK_GET_DIV(CLK_UART8_SRC);
        divFrac = CLK_GET_DIV(CLK_UART8_FRAC);
        gateId = CLK_UART8_GATE;
        fracGateId = CLK_UART8_FRAC_GATE;
        break;
    case CLK_UART9:
        muxSrc = CLK_GET_MUX(CLK_UART9_SRC);
        divSrc = CLK_GET_DIV(CLK_UART9_SRC);
        divFrac = CLK_GET_DIV(CLK_UART9_FRAC);
        gateId = CLK_UART9_GATE;
        fracGateId = CLK_UART9_FRAC_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);
    if (PLL_INPUT_OSC_RATE == rate) {
        HAL_CRU_ClkSetMux(mux, 2);
        HAL_CRU_ClkDisable(gateId);
    } else if ((!(s_gpllFreq % rate)) && ((s_gpllFreq / rate) < 31)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else if ((!(s_cpllFreq % rate)) && ((s_cpllFreq / rate) < 31)) {
        HAL_CRU_ClkSetDiv(divSrc, s_cpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_gpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        CRU->CRU_CLKSEL_CON[CLK_DIV_GET_REG_OFFSET(divFrac)] = (n << 16) | m;
        HAL_CRU_ClkSetMux(mux, 1);
    }

    return HAL_OK;
}

/**
 * @brief Get gmac clk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 */
static uint32_t HAL_CRU_ClkGmacGetFreq(eCLOCK_Name clockName)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t freq;

    switch (clockName) {
    case ACLK_PHP:
    case ACLK_USB:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 297000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 198000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 99000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;
    case HCLK_PHP:
    case HCLK_USB:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 148500000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 99000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 74250000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;
    case PCLK_PHP:
        freq = HAL_CRU_ClkGmacGetFreq(ACLK_PHP) / (HAL_CRU_ClkGetDiv(clkDiv));

        return freq;
    case PCLK_USB:
        freq = HAL_CRU_ClkGmacGetFreq(ACLK_USB) / (HAL_CRU_ClkGetDiv(clkDiv));

        return freq;
    case CLK_MAC0_2TOP:
    case CLK_MAC1_2TOP:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 125000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 50000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 25000000;
        } else {
            freq = HAL_CRU_ClkGetFreq(PLL_PPLL);
        }

        return freq;

    case CLK_MAC0_OUT:
    case CLK_MAC1_OUT:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 125000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 50000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 25000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case CLK_GMAC0_PTP_REF:
    case CLK_GMAC1_PTP_REF:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 62500;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 50000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case SCLK_GMAC0_RGMII_SPEED:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 50;
        } else {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 5;
        }

        return freq;

    case SCLK_GMAC1_RGMII_SPEED:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 50;
        } else {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 5;
        }

        return freq;

    case SCLK_GMAC0_RMII_SPEED:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 2;
        } else {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 20;
        }

        return freq;

    case SCLK_GMAC1_RMII_SPEED:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 2;
        } else {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 20;
        }

        return freq;
    case SCLK_GMAC0:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP);
        } else {
            freq = s_clk_gmac_in;
        }

        return freq;

    case SCLK_GMAC1:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP);
        } else {
            freq = s_clk_gmac_in;
        }

        return freq;

    case SCLK_GMAC0_RX_TX:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(SCLK_GMAC0_RGMII_SPEED);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = HAL_CRU_ClkGetFreq(SCLK_GMAC0_RMII_SPEED);
        } else {
            freq = s_clk_gmac_xpcs_mii;
        }

        return freq;
    case SCLK_GMAC1_RX_TX:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = HAL_CRU_ClkGetFreq(SCLK_GMAC1_RGMII_SPEED);
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = HAL_CRU_ClkGetFreq(SCLK_GMAC1_RMII_SPEED);
        } else {
            freq = s_clk_gmac_xpcs_mii;
        }

        return freq;
    default:

        return 0;
    }
}

/**
 * @brief Set gmac clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 */
static HAL_Status HAL_CRU_ClkGmacSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t mux = 0, div = 0;

    switch (clockName) {
    case ACLK_PHP:
    case ACLK_USB:
        if (rate >= 297000000) {
            mux = 0;
        } else if (rate >= 198000000) {
            mux = 1;
        } else if (rate >= 99000000) {
            mux = 2;
        } else {
            mux = 3;
        }
        break;
    case HCLK_PHP:
    case HCLK_USB:
        if (rate >= 148500000) {
            mux = 0;
        } else if (rate >= 99000000) {
            mux = 1;
        } else if (rate >= 74250000) {
            mux = 2;
        } else {
            mux = 3;
        }
        break;
    case PCLK_PHP:
        div = HAL_DIV_ROUND_UP(HAL_CRU_ClkGmacGetFreq(ACLK_PHP), rate);

        break;
    case PCLK_USB:
        div = HAL_DIV_ROUND_UP(HAL_CRU_ClkGmacGetFreq(ACLK_USB), rate);

        break;

    case CLK_MAC0_2TOP:
    case CLK_MAC1_2TOP:
    case CLK_MAC0_OUT:
    case CLK_MAC1_OUT:
        if (rate == 125000000) {
            mux = 0;
        } else if (rate == 50000000) {
            mux = 1;
        } else if (rate == 25000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;

    case CLK_GMAC0_PTP_REF:
    case CLK_GMAC1_PTP_REF:
        if (rate == 62500) {
            mux = 0;
        } else if (rate == 100000000) {
            mux = 1;
        } else if (rate == 50000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;

    case SCLK_GMAC0_RGMII_SPEED:
        if (rate == HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 50) {
            mux = 2;
        } else if (rate == HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 5) {
            mux = 3;
        } else {
            mux = 0;
        }
        break;

    case SCLK_GMAC1_RGMII_SPEED:
        if (rate == HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 50) {
            mux = 2;
        } else if (rate == HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 5) {
            mux = 3;
        } else {
            mux = 0;
        }
        break;

    case SCLK_GMAC0_RMII_SPEED:
        if (rate == HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP) / 2) {
            mux = 1;
        } else {
            mux = 0;
        }
        break;

    case SCLK_GMAC1_RMII_SPEED:
        if (rate == HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP) / 2) {
            mux = 1;
        } else {
            mux = 0;
        }
        break;
    case SCLK_GMAC0:
        if (rate == HAL_CRU_ClkGetFreq(CLK_MAC0_2TOP)) {
            mux = 0;
        } else {
            mux = 1;
        }
        break;

    case SCLK_GMAC1:
        if (rate == HAL_CRU_ClkGetFreq(CLK_MAC1_2TOP)) {
            mux = 0;
        } else {
            mux = 1;
        }
        break;

    case SCLK_GMAC0_RX_TX:
        if (rate == HAL_CRU_ClkGetFreq(SCLK_GMAC0_RGMII_SPEED)) {
            mux = 0;
        } else if (rate == HAL_CRU_ClkGetFreq(SCLK_GMAC0_RMII_SPEED)) {
            mux = 1;
        } else {
            mux = 2;
        }
        break;
    case SCLK_GMAC1_RX_TX:
        if (rate == HAL_CRU_ClkGetFreq(SCLK_GMAC1_RGMII_SPEED)) {
            mux = 0;
        } else if (rate == HAL_CRU_ClkGetFreq(SCLK_GMAC1_RMII_SPEED)) {
            mux = 1;
        } else {
            mux = 2;
        }
        break;
    default:

        return HAL_INVAL;
    }

    if (clkMux) {
        HAL_CRU_ClkSetMux(clkMux, mux);
    }
    if (clkDiv) {
        HAL_CRU_ClkSetDiv(clkDiv, div);
    }

    return HAL_OK;
}

/**
 * @brief Get mmc clk freq.
 * @param  clockName: CLOCK_Name id
 * @return clk rate.
 */
static uint32_t HAL_CRU_ClkMmcGetFreq(eCLOCK_Name clockName)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t freq;

    switch (clockName) {
    case BCLK_EMMC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 198000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 148500000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 125000000;
        }

        return freq;
    case CCLK_EMMC:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = PLL_INPUT_OSC_RATE;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 198000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 148500000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 3) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 4) {
            freq = 50000000;
        } else {
            freq = 375000;
        }

        return freq;

    case DCLK_SDMMC_BUFFER:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = 99000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 74250000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 50000000;
        }

        return freq;

    case CLK_SDMMC0:
    case CLK_SDMMC1:
    case CLK_SDMMC2:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = PLL_INPUT_OSC_RATE;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 396000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 297000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 3) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 4) {
            freq = 50000000;
        } else {
            freq = 750000;
        }

        return freq;

    default:

        return 0;
    }
}

/**
 * @brief Set mmc clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk set rate
 * @return HAL_Status.
 */
static HAL_Status HAL_CRU_ClkMmcSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t mux = 0;

    switch (clockName) {
    case BCLK_EMMC:
        if (rate >= 198000000) {
            mux = 0;
        } else if (rate >= 148000000) {
            mux = 1;
        } else {
            mux = 2;
        }
        break;

    case CCLK_EMMC:
        if (rate >= 198000000) {
            mux = 1;
        } else if (rate >= 148000000) {
            mux = 2;
        } else if (rate >= 100000000) {
            mux = 3;
        } else if (rate >= 50000000) {
            mux = 4;
        } else if (rate >= 24000000) {
            mux = 0;
        } else {
            mux = 5;
        }
        break;

    case DCLK_SDMMC_BUFFER:
        if (rate >= 99000000) {
            mux = 0;
        } else if (rate >= 74000000) {
            mux = 1;
        } else {
            mux = 2;
        }
        break;

    case CLK_SDMMC0:
    case CLK_SDMMC1:
    case CLK_SDMMC2:
        if (rate >= 396000000) {
            mux = 1;
        } else if (rate >= 297000000) {
            mux = 2;
        } else if (rate >= 100000000) {
            mux = 3;
        } else if (rate >= 50000000) {
            mux = 4;
        } else if (rate >= 24000000) {
            mux = 0;
        } else {
            mux = 5;
        }
        break;

    default:

        return 0;
    }
    if (clkMux) {
        HAL_CRU_ClkSetMux(clkMux, mux);
    }

    return HAL_OK;
}

/**
 * @brief Get clk freq.
 * @param  clockName: CLOCK_Name id.
 * @return rate.
 * @attention these APIs allow direct use in the HAL layer.
 */
uint32_t
EFIAPI
HAL_CRU_ClkGetFreq(eCLOCK_Name clockName)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t pRate = 0, freq;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }

    switch (clockName) {
    case PLL_APLL:
        freq = HAL_CRU_GetPllFreq(&APLL);
        s_apllFreq = freq;

        return freq;
    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;
    case PLL_NPLL:
        freq = HAL_CRU_GetPllFreq(&NPLL);
        s_npllFreq = freq;

        return freq;
    case PLL_VPLL:
        freq = HAL_CRU_GetPllFreq(&VPLL);
        s_vpllFreq = freq;

        return freq;
    case PLL_HPLL:
        freq = HAL_CRU_GetPllFreq(&HPLL);
        s_hpllFreq = freq;

        return freq;
    case PLL_PPLL:
        freq = HAL_CRU_GetPllFreq(&PPLL);
        s_ppllFreq = freq;

        return freq;
    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return s_gpllFreq;
    case CLK_WDT:

        return PLL_INPUT_OSC_RATE;
    case CLK_I2C:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = s_gpllFreq / 6;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = s_gpllFreq / 12;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = PLL_INPUT_OSC_RATE;
        } else {
            freq = s_cpllFreq / 10;
        }

        return freq;
    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_PWM3:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = s_gpllFreq / 12;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = PLL_INPUT_OSC_RATE;
        } else {
            freq = s_cpllFreq / 10;
        }

        return freq;
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
    case CLK_SPI3:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = s_gpllFreq / 6;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = PLL_INPUT_OSC_RATE;
        } else {
            freq = s_cpllFreq / 10;
        }

        return freq;
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_UART8:
    case CLK_UART9:
        freq = HAL_CRU_ClkFracGetFreq(clockName);

        return freq;
    case CLK_CAN0:
    case CLK_CAN1:
    case CLK_CAN2:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = s_cpllFreq;
        } else {
            pRate = s_gpllFreq;
        }

        break;

    case CLK_TSADC_TSEN:
        if (HAL_CRU_ClkGetMux(clkMux)) {
            pRate = s_gpllFreq / 12;
        } else {
            pRate = PLL_INPUT_OSC_RATE;
        }

        break;
    case CLK_TSADC:
        pRate = HAL_CRU_ClkGetFreq(CLK_TSADC_TSEN);

        break;

    case SCLK_SFC:
        if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = 50000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = 75000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 3) {
            freq = 100000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 4) {
            freq = 125000000;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 5) {
            freq = 150000000;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;
    case ACLK_USB:
    case HCLK_USB:
    case PCLK_USB:
    case ACLK_PHP:
    case HCLK_PHP:
    case PCLK_PHP:
    case CLK_MAC0_2TOP:
    case CLK_MAC1_2TOP:
    case CLK_MAC0_OUT:
    case CLK_MAC1_OUT:
    case CLK_GMAC0_PTP_REF:
    case CLK_GMAC1_PTP_REF:
    case SCLK_GMAC0_RGMII_SPEED:
    case SCLK_GMAC1_RGMII_SPEED:
    case SCLK_GMAC0_RMII_SPEED:
    case SCLK_GMAC1_RMII_SPEED:
    case SCLK_GMAC0_RX_TX:
    case SCLK_GMAC1_RX_TX:
        freq = HAL_CRU_ClkGmacGetFreq(clockName);

        return freq;

    case BCLK_EMMC:
    case CCLK_EMMC:
    case DCLK_SDMMC_BUFFER:
    case CLK_SDMMC0:
    case CLK_SDMMC1:
    case CLK_SDMMC2:
        freq = HAL_CRU_ClkMmcGetFreq(clockName);

        return freq;

    case ACLK_SECURE_FLASH:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = s_gpllFreq / 6;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = s_gpllFreq / 8;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = s_gpllFreq / 10;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case HCLK_SECURE_FLASH:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = s_gpllFreq / 8;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = s_gpllFreq / 10;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = s_gpllFreq / 16;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case ACLK_PIPE:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            freq = s_gpllFreq / 3;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 1) {
            freq = s_gpllFreq / 4;
        } else if (HAL_CRU_ClkGetMux(clkMux) == 2) {
            freq = s_gpllFreq / 6;
        } else {
            freq = PLL_INPUT_OSC_RATE;
        }

        return freq;

    case PCLK_PIPE:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PIPE);

        break;

    case CLK_PCIEPHY0_REF:
    case CLK_PCIEPHY1_REF:
    case CLK_PCIEPHY2_REF:
        if (HAL_CRU_ClkGetMux(clkMux) == 0) {
            pRate = 0;
        } else {
            pRate = s_ppllFreq / 2;
        }

        break;

    default:
        break;
    }

    if ((clkMux == 0) && (clkDiv == 0)) {
        return 0;
    }

    if (clkDiv) {
        freq = pRate / (HAL_CRU_ClkGetDiv(clkDiv));
    } else {
        freq = pRate;
    }

    return freq;
}

/**
 * @brief Set clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk rate.
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status
EFIAPI
HAL_CRU_ClkSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    HAL_Status error = HAL_OK;
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t mux = 0, div = 0, pRate = 0;

    if (!s_cpllFreq) {
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    }

    switch (clockName) {
    case PLL_APLL:

        return error;
    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;
    case PLL_HPLL:

        return error;
    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        DEBUG ((DEBUG_INIT, "GPLL set rate: %d %x\n", rate, error));
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
        return error;

    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_UART8:
    case CLK_UART9:
        error = HAL_CRU_ClkFracSetFreq(clockName, rate);

        return error;

    case CLK_CAN0:
    case CLK_CAN1:
    case CLK_CAN2:
        if (s_cpllFreq % rate == 0) {
            pRate = s_cpllFreq;
            mux = 1;
        } else {
            pRate = s_gpllFreq;
            mux = 0;
        }

        break;

    case CLK_TSADC_TSEN:
        if (PLL_INPUT_OSC_RATE % rate == 0) {
            pRate = PLL_INPUT_OSC_RATE;
            mux = 0;
        } else {
            pRate = s_gpllFreq / 12;
            mux = 1;
        }

        break;
    case CLK_TSADC:
        pRate = HAL_CRU_ClkGetFreq(CLK_TSADC_TSEN);

        break;
    case SCLK_SFC:
        if (rate == 50000000) {
            mux = 1;
        } else if (rate == 75000000) {
            mux = 2;
        } else if (rate == 100000000) {
            mux = 3;
        } else if (rate == 125000000) {
            mux = 4;
        } else if (rate == 150000000) {
            mux = 5;
        } else {
            mux = 0;
        }

        break;
    case ACLK_USB:
    case HCLK_USB:
    case PCLK_USB:
    case ACLK_PHP:
    case HCLK_PHP:
    case PCLK_PHP:
    case CLK_MAC0_2TOP:
    case CLK_MAC1_2TOP:
    case CLK_MAC0_OUT:
    case CLK_MAC1_OUT:
    case CLK_GMAC0_PTP_REF:
    case CLK_GMAC1_PTP_REF:
    case SCLK_GMAC0_RGMII_SPEED:
    case SCLK_GMAC1_RGMII_SPEED:
    case SCLK_GMAC0_RMII_SPEED:
    case SCLK_GMAC1_RMII_SPEED:
    case SCLK_GMAC0_RX_TX:
    case SCLK_GMAC1_RX_TX:
        error = HAL_CRU_ClkGmacSetFreq(clockName, rate);

        return error;

    case BCLK_EMMC:
    case CCLK_EMMC:
    case DCLK_SDMMC_BUFFER:
    case CLK_SDMMC0:
    case CLK_SDMMC1:
    case CLK_SDMMC2:
        error = HAL_CRU_ClkMmcSetFreq(clockName, rate);

        return error;

    case ACLK_SECURE_FLASH:
        if (rate >= 198000000) {
            mux = 0;
        } else if (rate >= 148000000) {
            mux = 1;
        } else if (rate >= 99000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;

    case HCLK_SECURE_FLASH:
        if (rate >= 148000000) {
            mux = 0;
        } else if (rate >= 99000000) {
            mux = 1;
        } else if (rate >= 74000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;

    case ACLK_PIPE:
        if (rate >= 396000000) {
            mux = 0;
        } else if (rate >= 297000000) {
            mux = 1;
        } else if (rate >= 198000000) {
            mux = 2;
        } else {
            mux = 3;
        }

        break;

    case PCLK_PIPE:
        pRate = HAL_CRU_ClkGetFreq(ACLK_PIPE);

        break;

    case CLK_PCIEPHY0_REF:
    case CLK_PCIEPHY1_REF:
    case CLK_PCIEPHY2_REF:
        pRate = s_ppllFreq / 2;
        mux = 1;

        break;

    default:
        break;
    }

    if ((clkMux == 0) && (clkDiv == 0)) {
        return HAL_INVAL;
    }

    div = HAL_DIV_ROUND_UP(pRate, rate);
    if (clkMux) {
        HAL_CRU_ClkSetMux(clkMux, mux);
    }
    if (clkDiv) {
        HAL_CRU_ClkSetDiv(clkDiv, div);
    }

    return HAL_OK;
}

/**
 * @brief wdt glbrst enable.
 * @param  wdtType: wdt reset type.
 * @return HAL_OK.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status
EFIAPI
HAL_CRU_WdtGlbRstEnable(eCRU_WdtRstType wdtType)
{
    uint32_t mask = CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK;
    uint32_t val = 0;

    switch (wdtType) {
    case GLB_RST_FST_WDT0:
        val |= (1 << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT);
        break;
    case GLB_RST_SND_WDT0:
        break;
    default:

        return HAL_INVAL;
    }

    CRU->GLB_RST_CON = VAL_MASK_WE(mask, val);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */
