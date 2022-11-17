/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK_SPI_H
#define __RK_SPI_H

#define HAL_SPI_MASTER_MAX_SCLK_OUT 50000000 /**< Max io clock in master mode */
#define HAL_SPI_SLAVE_MAX_SCLK_OUT  20000000 /**< Max io in clock in slave mode */

#define CR0_DATA_FRAME_SIZE_4BIT  (0x00 << SPI_CTRLR0_DFS_SHIFT)
#define CR0_DATA_FRAME_SIZE_8BIT  (0x01 << SPI_CTRLR0_DFS_SHIFT)
#define CR0_DATA_FRAME_SIZE_16BIT (0x02 << SPI_CTRLR0_DFS_SHIFT)

/* serial clock toggles in middle of first data bit */
#define CR0_PHASE_1EDGE (0x00 << SPI_CTRLR0_SCPH_SHIFT)
/* serial clock toggles at start of first data bit */
#define CR0_PHASE_2EDGE (0x01 << SPI_CTRLR0_SCPH_SHIFT)

#define CR0_POLARITY_LOW  (0x00 << SPI_CTRLR0_SCPOL_SHIFT)
#define CR0_POLARITY_HIGH (0x01 << SPI_CTRLR0_SCPOL_SHIFT)

/*
 * The period between ss_n active and
 * sclk_out active is half sclk_out cycles
 */
#define CR0_SSD_HALF (0x00 << SPI_CTRLR0_SSD_SHIFT)
/*
 * The period between ss_n active and
 * sclk_out active is one sclk_out cycle
 */
#define CR0_SSD_ONE (0x01 << SPI_CTRLR0_SSD_SHIFT)

#define CR0_EM_LITTLE (0x0 << SPI_CTRLR0_EM_SHIFT)
#define CR0_EM_BIG    (0x1 << SPI_CTRLR0_EM_SHIFT)

#define CR0_FIRSTBIT_MSB (0x0 << SPI_CTRLR0_FBM_SHIFT)
#define CR0_FIRSTBIT_LSB (0x1 << SPI_CTRLR0_FBM_SHIFT)

#define CR0_BHT_16BIT (0x0 << SPI_CTRLR0_BHT_SHIFT)
#define CR0_BHT_8BIT  (0x1 << SPI_CTRLR0_BHT_SHIFT)

#define CR0_XFM_TR (0x00 << SPI_CTRLR0_XFM_SHIFT)
#define CR0_XFM_TO (0x01 << SPI_CTRLR0_XFM_SHIFT)
#define CR0_XFM_RO (0x02 << SPI_CTRLR0_XFM_SHIFT)

#define CR0_OPM_MASTER (0x00 << SPI_CTRLR0_OPM_SHIFT)
#define CR0_OPM_SLAVE  (0x01 << SPI_CTRLR0_OPM_SHIFT)

#define CR0_CSM(nCycles) (((nCycles) << SPI_CTRLR0_CSM_SHIFT) & SPI_CTRLR0_CSM_MASK)
#define CR0_CSM_0CYCLE   CR0_CSM(0)
#define CR0_CSM_1CYCLE   CR0_CSM(1)
#define CR0_CSM_2CYCLES  CR0_CSM(2)
#define CR0_CSM_3CYCLES  CR0_CSM(3)

/***************************** Structure Definition **************************/

/** @brief  SPI Type definition */
typedef enum {
    SSI_MOTO_SPI = 0,
    SSI_TI_SSP,
    SSI_NS_MICROWIRE
} eSPI_SSIType;

/** @brief  SPI Transfer type definition */
typedef enum {
    SPI_POLL = 0,
    SPI_IT,
    SPI_DMA
} eSPI_TransferType;

/** @brief  SPI Configuration Structure definition */
struct SPI_CONFIG {
    UINT32 opMode;           /**< Specifies the SPI operating mode, master or slave. */
    UINT32 xfmMode;          /**< Specifies the SPI bidirectional mode state, tx only, rx only or trx mode. */
    UINT32 nBytes;           /**< Specifies the SPI data size. */
    UINT32 clkPolarity;      /**< Specifies the serial clock steady state. */
    UINT32 clkPhase;         /**< Specifies the clock active edge for the bit capture. */
    UINT32 firstBit;         /**< Specifies whether data transfers start from MSB or LSB bit. */
    UINT32 endianMode;       /**< Specifies whether data transfers start from little or big endian. */
    UINT32 apbTransform;     /**< Specifies apb transform type. */
    UINT32 ssd;              /**< Specifies period between ss_n active and sclk_out. */
    UINT32 speed;            /**< Specifies the Baud Rate prescaler value which will be
                                  used to configure the transmit and receive SCK clock. */
    UINT32 ssiType;          /**< Specifies if the TI mode is enabled or not.*/
    UINT32 csm;              /**< Specifies Motorola SPI Master SS_N high cycles for each frame data is transfer. */
};

/* We have 2 DMA channels per SPI, one for RX and one for TX */
struct HAL_SPI_DMA_INFO {
    UINT8 channel;
    UINT8 direction;
    UINT32 addr;
};

struct HAL_SPI_DEV {
    const UINT32 base;
    const UINT32 clkId;
    const UINT32 clkGateID;
    const UINT32 pclkGateID;
    const UINT8 irqNum;
    const UINT8 isSlave;
    const struct HAL_SPI_DMA_INFO txDma;
    const struct HAL_SPI_DMA_INFO rxDma;
};

/** @brief  SPI handle Structure definition */
struct SPI_HANDLE {
    struct SPI_REG *pReg;          /**< Specifies SPI registers base address. */
    UINT32 maxFreq;              /**< Specifies SPI clock frequency. */
    struct SPI_CONFIG config;      /**< Specifies SPI communication parameters. */
    const UINT8 *pTxBuffer;      /**< Specifies pointer to SPI Tx transfer Buffer. */
    const UINT8 *pTxBufferEnd;   /**< Specifies pointer to SPI Tx End transfer Buffer. */
    UINT8 *pRxBuffer;            /**< Specifies pointer to SPI Rx transfer Buffer. */
    UINT8 *pRxBufferEnd;         /**< Specifies pointer to SPI Rx End transfer Buffer. */
    UINT32 len;                  /**< Specifies the transfer length . */
    eSPI_TransferType type;        /**< Specifies the transfer type: POLL/IT/DMA. */
    UINT32 dmaBurstSize;         /**< Specifies Dma Burst size */
};


RETURN_STATUS SPI_Init(struct SPI_HANDLE *pSPI, UINT32 base);
RETURN_STATUS SPI_DeInit(struct SPI_HANDLE *pSPI);
RETURN_STATUS SPI_FlushFifo(struct SPI_HANDLE *pSPI);
RETURN_STATUS SPI_SetCS(struct SPI_HANDLE *pSPI, UINT8 select, UINT8 enable);
RETURN_STATUS SPI_PioTransfer(struct SPI_HANDLE *pSPI);
RETURN_STATUS SPI_Stop(struct SPI_HANDLE *pSPI);
RETURN_STATUS SPI_Configure(struct SPI_HANDLE *pSPI, const UINT8 *pTxData,
                            UINT8 *pRxData, UINT32 Size);
UINT32 SPI_CalculateTimeout(struct SPI_HANDLE *pSPI);

#endif
