/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
extern "C" {
#endif
#define uint8_t         UINT8
#define uint16_t        UINT16
#define uint32_t        UINT32
#define uint64_t        UINT64
#define HAL_ASSERT      ASSERT
#define HAL_CPUDelayUs  MicroSecondDelay
#define __WEAK
#define HAL_DivU64  DivU64x32
/* IO definitions (access restrictions to peripheral registers) */
/*!< brief Defines 'read only' permissions */
#ifdef __cplusplus
#define   __I  volatile
#else
#define   __I  volatile const
#endif
/*!< brief Defines 'write only' permissions */
#define     __O  volatile
/*!< brief Defines 'read / write' permissions */
#define     __IO  volatile

/**< Write the register */
#define WRITE_REG(REG, VAL)  ((*(volatile uint32_t *)&(REG)) = (VAL))
/**< Read the register */
#define READ_REG(REG)       ((*(volatile const uint32_t *)&(REG)))
#define DIV_ROUND_UP(x, y)  (((x) + (y) - 1) / (y))
/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */
#include "RK3588.h"
#define RK3588_PERIPH_BASE  0xF0000000
#define RK3588_PERIPH_SZ    0x10000000

#define I2C_BASE(id)  ((id == 0) ? 0xFD880000 :                                        \
                                 (id <= 5) ? (0xFEA90000 + ((id - 1) * 0x10000)) :     \
                                 (id <= 8) ? (0xFEC80000 + ((id - 6) * 0x10000)) : 0)
#define I2C_COUNT  9

/******************************************CRU*******************************************/
#define PLL_INPUT_OSC_RATE  (24 * 1000 * 1000)

typedef enum {
  PLL_LPLL = 0,
  PLL_B0PLL,
  PLL_B1PLL,
  PLL_CPLL,
  PLL_GPLL,
  PLL_NPLL,
  PLL_V0PLL,
  PLL_PPLL,
  PLL_AUPLL,
  CCLK_EMMC,
  SCLK_SFC,
  CCLK_SRC_SDIO,
  BCLK_EMMC,
  CLK_REF_PIPE_PHY0,
  CLK_REF_PIPE_PHY1,
  CLK_REF_PIPE_PHY2,
  DCLK_VOP2_SRC,
  CLK_I2S0_8CH_TX_SRC,
  MCLK_I2S0_8CH_TX,
  MCLK_I2S1_8CH_TX,
  CLK_SARADC,
  CLK_COUNT
} RK3588_CLOCK_IDS;

typedef enum {
  RESET_SRST_P_SARADC = 0,
  RESET_COUNT
} RK3588_RESET_IDS;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
