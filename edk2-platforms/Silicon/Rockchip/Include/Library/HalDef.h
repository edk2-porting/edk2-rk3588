/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef _HAL_DEF_H_
#define _HAL_DEF_H_

#include "Soc.h"

/* SOC OPS Marco */
#define SET_BIT(REG, BIT)    ((*(volatile uint32_t *)&(REG)) |= (BIT))
#define CLEAR_BIT(REG, MASK) ((*(volatile uint32_t *)&(REG)) &= ~(MASK))
#define READ_BIT(REG, MASK)  ((*(volatile const uint32_t *)&(REG)) & (MASK))
#define CLEAR_REG(REG)       ((*(volatile uint32_t *)&(REG)) = (0x0))
#define WRITE_REG(REG, VAL)  ((*(volatile uint32_t *)&(REG)) = (VAL))
#define READ_REG(REG)        ((*(volatile const uint32_t *)&(REG)))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
        WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#define POSITION_VAL(VAL) (__CLZ(__RBIT(VAL)))

#if defined(__GNUC__) || defined(__CC_ARM)
#define MASK_TO_WE(msk) (__builtin_constant_p(msk) ? ((msk) > 0xFFFFU ? 0 : ((msk) << 16)) : ((msk) << 16))
#else
#define MASK_TO_WE(msk) ((msk) << 16)
#endif
#define VAL_MASK_WE(msk, val)            ((MASK_TO_WE(msk)) | (val))
#define WRITE_REG_MASK_WE(reg, msk, val) WRITE_REG(reg, (VAL_MASK_WE(msk, val)))

/* Misc OPS Marco */
#define HAL_MAX_DELAY 0xFFFFFFFFU

#define RESET                     0
#define HAL_IS_BIT_SET(REG, MASK) (((*(volatile uint32_t *)&(REG)) & (MASK)) != RESET)
#define HAL_IS_BIT_CLR(REG, MASK) (((*(volatile uint32_t *)&(REG)) & (MASK)) == RESET)

#define HAL_BIT(nr)       (1UL << (nr))
#define HAL_ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#define HAL_MAX(x, y)     ((x) > (y) ? (x) : (y))
#define HAL_MIN(x, y)     ((x) < (y) ? (x) : (y))

#define HAL_DIV_ROUND_UP(x, y) (((x) + (y) - 1) / (y))

#define HAL_IS_ALIGNED(x, a) (((x) & (a - 1)) == 0)
#ifdef CACHE_LINE_SIZE
#define HAL_IS_CACHELINE_ALIGNED(x) HAL_IS_ALIGNED((uint32_t)(x), CACHE_LINE_SIZE)
#else
#define HAL_IS_CACHELINE_ALIGNED(x) HAL_IS_ALIGNED((uint32_t)(x), 4)
#endif

/* Compiller Macro */
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM) || defined(__ICCARM__)
#define HAL_UNUSED __attribute__((__unused__))
#else
#define HAL_UNUSED
#endif

#ifdef CACHE_LINE_SIZE
#define HAL_CACHELINE_ALIGNED __ALIGNED(CACHE_LINE_SIZE)
#else
#define HAL_CACHELINE_ALIGNED
#endif

#ifdef HAL_SRAM_SECTION_ENABLED
#define HAL_SECTION_SRAM_CODE   __attribute__((section(".sram_code")))
#define HAL_SECTION_SRAM_RODATA __attribute__((section(".sram_rodata")))
#define HAL_SECTION_SRAM_DATA   __attribute__((section(".sram_data")))
#define HAL_SECTION_SRAM_BSS    __attribute__((section(".sram_bss")))
#else
#define HAL_SECTION_SRAM_CODE
#define HAL_SECTION_SRAM_RODATA
#define HAL_SECTION_SRAM_DATA
#define HAL_SECTION_SRAM_BSS
#endif

#ifdef HAL_PSRAM_SECTION_ENABLED
#define HAL_SECTION_PSRAM_CODE   __attribute__((section(".psram_code")))
#define HAL_SECTION_PSRAM_RODATA __attribute__((section(".psram_rodata")))
#define HAL_SECTION_PSRAM_DATA   __attribute__((section(".psram_data")))
#define HAL_SECTION_PSRAM_BSS    __attribute__((section(".psram_bss")))
#else
#define HAL_SECTION_PSRAM_CODE
#define HAL_SECTION_PSRAM_RODATA
#define HAL_SECTION_PSRAM_DATA
#define HAL_SECTION_PSRAM_BSS
#endif

#ifdef HAL_XIP_SECTION_ENABLED
#define HAL_SECTION_XIP_CODE   __attribute__((section(".xip_code")))
#define HAL_SECTION_XIP_RODATA __attribute__((section(".xip_rodata")))
#else
#define HAL_SECTION_XIP_CODE
#define HAL_SECTION_XIP_RODATA
#endif

typedef enum {
    HAL_SYSTICK_CLKSRC_CORE,
    HAL_SYSTICK_CLKSRC_EXT
} eHAL_systickClkSource;

#define IS_SYSTICK_SOURCE(s) (((s) == HAL_SYSTICK_CLKSRC_CORE) || ((s) == HAL_SYSTICK_CLKSRC_EXT))

/***************************** Structure Definition **************************/
typedef enum {
    HAL_FALSE = 0x00U,
    HAL_TRUE  = 0x01U
} HAL_Check;

typedef enum {
    HAL_OK      = 0x00U,
    HAL_ERROR   = (-1),
    HAL_BUSY    = (-16),
    HAL_NODEV   = (-19),
    HAL_INVAL   = (-22),
    HAL_NOSYS   = (-38),
    HAL_TIMEOUT = (-110)
} HAL_Status;

typedef enum {
    HAL_DISABLE = 0x00U,
    HAL_ENABLE  = 0x01U
} HAL_FuncStatus;

typedef enum {
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED   = 0x01U
} HAL_LockStatus;

typedef enum {
    GPIO_BANK0 = 0,
    GPIO_BANK1 = 1,
    GPIO_BANK2 = 2,
    GPIO_BANK3 = 3,
    GPIO_BANK4 = 4,
    GPIO_BANK_NUM
} eGPIO_bankId;

typedef void (*pFunc)(void);

/***************************** Function Declare ******************************/

#endif
