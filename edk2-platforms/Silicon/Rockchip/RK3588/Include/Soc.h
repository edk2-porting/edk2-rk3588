/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif
#define uint8_t UINT8
#define uint16_t UINT16
#define uint32_t UINT32
#define uint64_t UINT64
#define HAL_ASSERT ASSERT
#define HAL_CPUDelayUs MicroSecondDelay
#define __WEAK
#define HAL_DivU64 DivU64x32
/* IO definitions (access restrictions to peripheral registers) */
/*!< brief Defines 'read only' permissions */
#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
/*!< brief Defines 'write only' permissions */
#define     __O     volatile
/*!< brief Defines 'read / write' permissions */
#define     __IO    volatile

/**< Write the register */
#define WRITE_REG(REG, VAL)  ((*(volatile uint32_t *)&(REG)) = (VAL))
/**< Read the register */
#define READ_REG(REG)        ((*(volatile const uint32_t *)&(REG)))
#define DIV_ROUND_UP(x, y) (((x) + (y) - 1) / (y))
/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */
#include "RK3588.h"
#define RK3588_PERIPH_BASE      0xF0000000
#define RK3588_PERIPH_SZ        0x10000000

/******************************************CRU*******************************************/
#define PLL_INPUT_OSC_RATE (24 * 1000 * 1000)

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_LPLL,
    PLL_B0PLL,
    PLL_B1PLL,
    PLL_CPLL,
    PLL_GPLL,
    PLL_NPLL,
    PLL_V0PLL,
    PLL_PPLL,
    PLL_AUPLL,
    CCLK_EMMC             = CLK(CCLK_EMMC_SEL, CCLK_EMMC_DIV),
    SCLK_SFC             = CLK(SCLK_SFC_SEL, SCLK_SFC_DIV),
    CCLK_SRC_SDIO             = CLK(CCLK_SRC_SDIO_SEL, CCLK_SRC_SDIO_DIV),
    BCLK_EMMC              = CLK(BCLK_EMMC_SEL, BCLK_EMMC_DIV),
    CLK_REF_PIPE_PHY0              = CLK(CLK_REF_PIPE_PHY0_SEL, CLK_REF_PIPE_PHY0_PLL_SRC_DIV),
    CLK_REF_PIPE_PHY1              = CLK(CLK_REF_PIPE_PHY1_SEL, CLK_REF_PIPE_PHY1_PLL_SRC_DIV),
    CLK_REF_PIPE_PHY2              = CLK(CLK_REF_PIPE_PHY2_SEL, CLK_REF_PIPE_PHY2_PLL_SRC_DIV),
} eCLOCK_Name;
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
