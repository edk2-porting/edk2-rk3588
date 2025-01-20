/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRU
 *  @{
 */

#ifndef _HAL_CRU_H_
#define _HAL_CRU_H_

#include "HalDef.h"

/*************************** MACRO Definition ****************************/

/** @defgroup CRU_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define MHZ  1000000
#define KHZ  1000

#ifndef PLL_INPUT_OSC_RATE
#define PLL_INPUT_OSC_RATE  (24 * MHZ)
#endif

#define CRU_CON_REG_ADDRESS(_regBase, _offset, _index) \
    ((_regBase) + (_offset) + ((_index) * sizeof(uint32_t)))

#define CLK_RESET_GET_REG_OFFSET(x)  ((uint32_t)((x) / 16))
#define CLK_RESET_GET_BITS_SHIFT(x)  ((uint32_t)((x) % 16))

#define CLK_GATE_GET_REG_OFFSET(x)  ((uint32_t)((x) / 16))
#define CLK_GATE_GET_BITS_SHIFT(x)  ((uint32_t)((x) % 16))

#define WIDTH_TO_MASK(width)  ((1 << (width)) - 1)

#define CLK_MUX_REG_OFFSET_SHIFT  0U
#define CLK_MUX_REG_OFFSET_MASK   0x0000FFFFU
#define CLK_MUX_SHIFT_SHIFT       16U
#define CLK_MUX_SHIFT_MASK        0x00FF0000U
#define CLK_MUX_WIDTH_SHIFT       24U
#define CLK_MUX_WIDTH_MASK        0xFF000000U

#define CLK_MUX_GET_REG_OFFSET(x) \
    (((uint32_t)(x) & CLK_MUX_REG_OFFSET_MASK) >> CLK_MUX_REG_OFFSET_SHIFT)
#define CLK_MUX_GET_BITS_SHIFT(x) \
    (((uint32_t)(x) & CLK_MUX_SHIFT_MASK) >> CLK_MUX_SHIFT_SHIFT)
#define CLK_MUX_GET_MASK(x)                                                      \
    WIDTH_TO_MASK((((uint32_t)(x) & CLK_MUX_WIDTH_MASK) >> CLK_MUX_WIDTH_SHIFT)) \
        << CLK_MUX_GET_BITS_SHIFT(x)

#define CLK_DIV_REG_OFFSET_SHIFT  0U
#define CLK_DIV_REG_OFFSET_MASK   0x0000FFFFU
#define CLK_DIV_SHIFT_SHIFT       16U
#define CLK_DIV_SHIFT_MASK        0x00FF0000U
#define CLK_DIV_WIDTH_SHIFT       24U
#define CLK_DIV_WIDTH_MASK        0xFF000000U

#define CLK_DIV_GET_REG_OFFSET(x) \
    (((uint32_t)(x) & CLK_DIV_REG_OFFSET_MASK) >> CLK_DIV_REG_OFFSET_SHIFT)
#define CLK_DIV_GET_BITS_SHIFT(x) \
    (((uint32_t)(x) & CLK_DIV_SHIFT_MASK) >> CLK_DIV_SHIFT_SHIFT)
#define CLK_DIV_GET_MASK(x)                                                      \
    WIDTH_TO_MASK((((uint32_t)(x) & CLK_DIV_WIDTH_MASK) >> CLK_DIV_WIDTH_SHIFT)) \
        << CLK_DIV_GET_BITS_SHIFT(x)

#define RK_PLL_RATE(_rate, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, \
                    _frac)                                                \
    {                                                                     \
        .rate = _rate##U, .fbDiv = _fbdiv, .postDiv1 = _postdiv1,         \
        .refDiv = _refdiv, .postDiv2 = _postdiv2, .dsmpd = _dsmpd,        \
        .frac = _frac,                                                    \
    }

#define RK3588_PLL_RATE(_rate, _p, _m, _s, _k) \
    {                                                                     \
        .rate = _rate##U, .p = _p, .m = _m,         \
        .s = _s, .k = _k,        \
    }

struct PLL_CONFIG {
  uint32_t    rate;
  uint32_t    fbDiv;
  uint32_t    postDiv1;
  uint32_t    refDiv;
  uint32_t    postDiv2;
  uint32_t    dsmpd;
  uint32_t    frac;
  uint32_t    m;
  uint32_t    p;
  uint32_t    s;
  uint32_t    k;
};

struct PLL_SETUP {
  __IO uint32_t              *conOffset0;
  __IO uint32_t              *conOffset1;
  __IO uint32_t              *conOffset2;
  __IO uint32_t              *conOffset3;
  __IO uint32_t              *conOffset6;
  __IO uint32_t              *modeOffset;
  __I uint32_t               *stat0;
  uint32_t                   modeShift;
  uint32_t                   lockShift;
  uint32_t                   modeMask;
  const struct PLL_CONFIG    *rateTable;
};

typedef enum {
  GLB_SRST_FST = 0xfdb9,
  GLB_SRST_SND = 0xeca8,
} eCRU_GlbSrstType;

typedef enum {
  GLB_RST_FST_WDT0 = 0U,
  GLB_RST_SND_WDT0,
  GLB_RST_FST_WDT1,
  GLB_RST_SND_WDT1,
  GLB_RST_FST_WDT2,
  GLB_RST_SND_WDT2,
} eCRU_WdtRstType;

typedef enum {
  CLOCK_SUPPORT_MUX  = 1 << 0,
  CLOCK_SUPPORT_DIV  = 1 << 1,
  CLOCK_SUPPORT_GATE = 1 << 2,
  CLOCK_SUPPORT_ALL  = 0xFFFFFFFF
} CRU_CLOCK_SUPPORT_FLAGS;

#define CRU_CLOCK_INIT(_id, _regBase, _muxOffset, _mux,                  \
                       _divOffset, _div, _gateOffset, _gate)                \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_ALL,                                      \
        .muxOffset = _muxOffset,                                         \
        .mux = _mux,                                                     \
        .divOffset = _divOffset,                                         \
        .div = _div,                                                     \
        .gateOffset = _gateOffset,                                       \
        .gate = _gate,                                                   \
    }

#define CRU_CLOCK_NOMUX_INIT(_id, _regBase,                              \
                             _divOffset, _div, _gateOffset, _gate)                \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_ALL & ~CLOCK_SUPPORT_MUX,                 \
        .divOffset = _divOffset,                                         \
        .div = _div,                                                     \
        .gateOffset = _gateOffset,                                       \
        .gate = _gate,                                                   \
    }

#define CRU_CLOCK_NODIV_INIT(_id, _regBase, _muxOffset, _mux,            \
                             _gateOffset, _gate)                                  \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_ALL & ~CLOCK_SUPPORT_DIV,                 \
        .muxOffset = _muxOffset,                                         \
        .mux = _mux,                                                     \
        .gateOffset = _gateOffset,                                       \
        .gate = _gate,                                                   \
    }

#define CRU_CLOCK_NOGATE_INIT(_id, _regBase, _muxOffset, _mux,           \
                              _divOffset, _div)                                    \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_ALL & ~CLOCK_SUPPORT_GATE,                \
        .muxOffset = _muxOffset,                                         \
        .mux = _mux,                                                     \
        .divOffset = _divOffset,                                         \
        .div = _div,                                                     \
    }

#define CRU_CLOCK_MUX_INIT(_id, _regBase, _muxOffset, _mux)              \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_MUX,                                      \
        .muxOffset = _muxOffset,                                         \
        .mux = _mux,                                                     \
    }

#define CRU_CLOCK_DIV_INIT(_id, _regBase, _divOffset, _div)              \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_DIV,                                      \
        .divOffset = _divOffset,                                         \
        .div = _div,                                                     \
    }

#define CRU_CLOCK_GATE_INIT(_id, _regBase, _gateOffset, _gate)           \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .flags = CLOCK_SUPPORT_GATE,                                     \
        .gateOffset = _gateOffset,                                       \
        .gate = _gate,                                                   \
    }

#define CRU_RESET_INIT(_id, _regBase, _srstOffset, _srst)                 \
    [_id] = {                                                            \
        .regBase = _regBase,                                             \
        .srstOffset = _srstOffset,                                       \
        .srst = _srst,                                                   \
    }

/***************************** Structure Definition **************************/

typedef struct {
  uint64_t                   regBase;
  CRU_CLOCK_SUPPORT_FLAGS    flags;
  uint32_t                   muxOffset;
  uint32_t                   mux;
  uint32_t                   divOffset;
  uint32_t                   div;
  uint32_t                   gateOffset;
  uint32_t                   gate;
} CRU_CLOCK;

typedef struct {
  uint64_t    regBase;
  uint32_t    srstOffset;
  uint32_t    srst;
} CRU_RESET;

/** @} */
/***************************** Function Declare ******************************/

/** @defgroup CRU_Public_Function_Declare Public Function Declare
 *  @{
 */

CRU_CLOCK *
HAL_CRU_ClkGetById (
  uint32_t  clockId
  );

CRU_RESET *
HAL_CRU_RstGetById (
  uint32_t  resetId
  );

uint32_t
HAL_CRU_GetPllFreq (
  struct PLL_SETUP  *pSetup
  );

HAL_Status
HAL_CRU_SetPllFreq (
  struct PLL_SETUP  *pSetup,
  uint32_t          rate
  );

HAL_Status
HAL_CRU_SetPllPowerUp (
  struct PLL_SETUP  *pSetup
  );

HAL_Status
HAL_CRU_SetPllPowerDown (
  struct PLL_SETUP  *pSetup
  );

uint32_t
HAL_CRU_GetPllV1Freq (
  struct PLL_SETUP  *pSetup
  );

HAL_Status
HAL_CRU_SetPllV1Freq (
  struct PLL_SETUP  *pSetup,
  uint32_t          rate
  );

HAL_Check
HAL_CRU_ClkIsEnabled (
  uint32_t  clockId
  );

HAL_Status
HAL_CRU_ClkEnable (
  uint32_t  clockId
  );

HAL_Status
HAL_CRU_ClkDisable (
  uint32_t  clockId
  );

HAL_Status
HAL_CRU_ClkSetDiv (
  uint32_t  clockId,
  uint32_t  divValue
  );

uint32_t
HAL_CRU_ClkGetDiv (
  uint32_t  clockId
  );

HAL_Status
HAL_CRU_ClkSetMux (
  uint32_t  clockId,
  uint32_t  muxValue
  );

uint32_t
HAL_CRU_ClkGetMux (
  uint32_t  clockId
  );

HAL_Status
HAL_CRU_FracdivGetConfig (
  uint32_t  rateOut,
  uint32_t  rate,
  uint32_t  *numerator,
  uint32_t  *denominator
  );

uint32_t
HAL_CRU_ClkGetFreq (
  uint32_t  clockId
  );

HAL_Status
HAL_CRU_ClkSetFreq (
  uint32_t  clockId,
  uint32_t  rate
  );

HAL_Status
HAL_CRU_VopDclkEnable (
  uint32_t  gateId
  );

HAL_Status
HAL_CRU_VopDclkDisable (
  uint32_t  gateId
  );

HAL_Status
HAL_CRU_ClkNp5BestDiv (
  uint32_t  clockId,
  uint32_t  rate,
  uint32_t  pRate,
  uint32_t  *bestdiv
  );

HAL_Status
HAL_CRU_SetGlbSrst (
  eCRU_GlbSrstType  type
  );

HAL_Status
HAL_CRU_WdtGlbRstEnable (
  eCRU_WdtRstType  wdtType
  );

HAL_Status
HAL_CRU_PllCompensation (
  uint32_t  clockId,
  int       ppm
  );

HAL_Check
HAL_CRU_RstIsAsserted (
  uint32_t  resetId
  );

HAL_Status
HAL_CRU_RstAssert (
  uint32_t  resetId
  );

HAL_Status
HAL_CRU_RstDeassert (
  uint32_t  resetId
  );

#ifdef HAL_CRU_AS_FEATURE_ENABLED

/**
 * @brief  it is for AS init.
 */
void
HAL_CRU_AsInit (
  void
  );

/**
 * @brief  it is for AS enable.
 * @param  ch: channel
 * @param  en: 1 is enable, 0 is disable.
 */
void
HAL_CRU_AsEnable (
  uint8_t  ch,
  uint8_t  en
  );

#endif

/** @} */

#endif

/** @} */

/** @} */
