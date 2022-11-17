/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef _HAL_SPI_MEM_H_
#define _HAL_SPI_MEM_H_

#define BIT(nr)       (1UL << (nr))
/** SPI Memory host mode */
#define HAL_SPI_CPHA      BIT(0)            /**< clock phase */
#define HAL_SPI_CPOL      BIT(1)            /**< clock polarity */
#define HAL_SPI_MODE_0    (0 | 0)               /**< (original MicroWire) */
#define HAL_SPI_MODE_1    (0 | HAL_SPI_CPHA)
#define HAL_SPI_MODE_2    (HAL_SPI_CPOL | 0)
#define HAL_SPI_MODE_3    (HAL_SPI_CPOL | HAL_SPI_CPHA)
#define HAL_SPI_CS_HIGH   BIT(2)            /**< CS active high */
#define HAL_SPI_LSB_FIRST BIT(3)            /**< per-word bits-on-wire */
#define HAL_SPI_3WIRE     BIT(4)            /**< SI/SO signals shared */
#define HAL_SPI_LOOP      BIT(5)            /**< loopback mode */
#define HAL_SPI_SLAVE     BIT(6)            /**< slave mode */
#define HAL_SPI_PREAMBLE  BIT(7)            /**< Skip preamble bytes */
#define HAL_SPI_TX_BYTE   BIT(8)            /**< transmit with 1 wire byte */
#define HAL_SPI_TX_DUAL   BIT(9)            /**< transmit with 2 wires */
#define HAL_SPI_TX_QUAD   BIT(10)           /**< transmit with 4 wires */
#define HAL_SPI_RX_SLOW   BIT(11)           /**< receive with 1 wire slow */
#define HAL_SPI_RX_DUAL   BIT(12)           /**< receive with 2 wires */
#define HAL_SPI_RX_QUAD   BIT(13)           /**< receive with 4 wires */
#define HAL_SPI_XIP       BIT(14)           /**< support spi flash xip mode */

/** SPI Memory host xfer flags */
#define HAL_SPI_XFER_BEGIN BIT(0)   /**< Assert CS before transfer */
#define HAL_SPI_XFER_END   BIT(1)   /**< Deassert CS after transfer */
#define HAL_SPI_XFER_ONCE  (HAL_SPI_XFER_BEGIN | HAL_SPI_XFER_END)

#define JEDEC_MFR(id) (((id) >> 16) & 0xff)

#define HAL_SPI_MEM_OP_FORMAT(__cmd, __addr, __dummy, __data) \
    {                                                         \
        .cmd = __cmd,                                         \
        .addr = __addr,                                       \
        .dummy = __dummy,                                     \
        .data = __data,                                       \
    }

#define HAL_SPI_MEM_OP_CMD(__opcode, __buswidth) \
    {                                            \
        .buswidth = __buswidth,                  \
        .opcode = __opcode,                      \
    }

#define HAL_SPI_MEM_OP_ADDR(__nbytes, __val, __buswidth) \
    {                                                    \
        .nbytes = __nbytes,                              \
        .val = __val,                                    \
        .buswidth = __buswidth,                          \
    }

#define HAL_SPI_MEM_OP_NO_ADDR \
    {                          \
        .nbytes = 0,           \
        .val = 0,              \
        .buswidth = 0,         \
    }

#define HAL_SPI_MEM_OP_DUMMY(__nbytes, __buswidth) \
    {                                              \
        .a2dIdle = 0,                              \
        .nbytes = __nbytes,                        \
        .buswidth = __buswidth,                    \
    }

#define HAL_SPI_MEM_OP_NO_DUMMY \
    {                           \
        .a2dIdle = 0,           \
        .nbytes = 0,            \
        .buswidth = 0,          \
    }

#define HAL_SPI_MEM_OP_DATA_IN(__nbytes, __buf, __buswidth) \
    {                                                       \
        .dir = HAL_SPI_MEM_DATA_IN,                         \
        .nbytes = __nbytes,                                 \
        .buf.in = __buf,                                    \
        .buswidth = __buswidth,                             \
    }

#define HAL_SPI_MEM_OP_DATA_OUT(__nbytes, __buf, __buswidth) \
    {                                                        \
        .dir = HAL_SPI_MEM_DATA_OUT,                         \
        .nbytes = __nbytes,                                  \
        .buf.out = __buf,                                    \
        .buswidth = __buswidth,                              \
    }

#define HAL_SPI_MEM_OP_NO_DATA      \
    {                               \
        .dir = HAL_SPI_MEM_DATA_IN, \
        .nbytes = 0,                \
        .buf.out = NULL,            \
        .buswidth = 0,              \
    }

/* Max len case: cmd(1) + addr(4) + dummy(4) */
#define HAL_SPI_OP_LEN_MAX 0x10

/***************************** Structure Definition **************************/

enum SPI_MEM_DATA_DIR {
    HAL_SPI_MEM_DATA_IN,
    HAL_SPI_MEM_DATA_OUT,
};

struct HAL_SPI_MEM_OP {
    struct {
        uint8_t buswidth;
        uint8_t opcode;
    } cmd;

    struct {
        uint8_t nbytes;
        uint8_t buswidth;
        uint32_t val;
    } addr;

    struct {
        uint8_t a2dIdle;
        uint8_t nbytes;
        uint8_t buswidth;
    } dummy;

    struct {
        uint8_t buswidth;
        enum SPI_MEM_DATA_DIR dir;
        unsigned int nbytes;
        /**< buf.{in,out} must be DMA-able. */
        union {
            void *in;
            const void *out;
        } buf;
    } data;
};

#define HAL_SPI_MEM_OP(__cmd, __addr, __dummy, __data) \
    {                                                  \
        .cmd = __cmd,                                  \
        .addr = __addr,                                \
        .dummy = __dummy,                              \
        .data = __data,                                \
    }

#endif
