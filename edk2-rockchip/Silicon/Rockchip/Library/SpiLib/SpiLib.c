/**
  Copyright (c) 2021, Rockchip Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include "Soc.h"
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SpiLib.h>

/** @defgroup How_To_Use How To Use
 *

 The SPI HAL driver can be used as follows:

 - Declare a SPI_Handle handle structure, for example:
   ```
   SPI_Handle instance;
   ```

 - Invoke SPI_Init() API to configure default config:
     - opMode: slave or master
     - apbTransform
     - endianMode
     - ssd
     - Clock rate

 - Invoke SPI_Configure() API to program other mode:
     - Data size
     - Clock polarity and phase
     - FirstBit
     - Clock div
     - Number of data frames received at RX only mode
     - IT FIFO Level and DMA FIFO Level
     - Transfer Mode

 - Blocking transfer:
     - The communication is performed in polling mode by calling SPI_PioTransfer().
     - after transfer done, invoke SPI_Stop to release the chip select.

 */

#define HAL_SPI_FIFO_LENGTH 64
/* Bit fields in SR */
#define HAL_SPI_SR_BUSY     (0x1 << SPI_SR_BSF_SHIFT)
#define HAL_SPI_SR_STB_BUSY (0x1 << SPI_SR_STB_SHIFT)

/* Bit fields in ISR, IMR, RISR, 7 bits */
#define SPI_INT_TXEI  (1 << SPI_IMR_TFEIM_SHIFT)
#define SPI_INT_TXOI  (1 << SPI_IMR_TFOIM_SHIFT)
#define SPI_INT_RXUI  (1 << SPI_IMR_RFUIM_SHIFT)
#define SPI_INT_RXOI  (1 << SPI_IMR_RFOIM_SHIFT)
#define SPI_INT_RXFI  (1 << SPI_IMR_RFFIM_SHIFT)
#define SPI_INT_TOI   (1 << SPI_IMR_TOIM_SHIFT)
#define SPI_INT_SSPI  (1 << SPI_IMR_SSPIM_SHIFT)
#define SPI_INT_TXFIM (1 << SPI_IMR_TXFIM_SHIFT)

/* Bit fields in ICR */
#define SPI_CLEAR_INT_ALL  (1 << SPI_ICR_CCI_SHIFT)
#define SPI_CLEAR_INT_RXUI (1 << SPI_ICR_CRFUI_SHIFT)
#define SPI_CLEAR_INT_RXOI (1 << SPI_ICR_CRFOI_SHIFT)
#define SPI_CLEAR_INT_TXOI (1 << SPI_ICR_CTFOI_SHIFT)
#define SPI_CLEAR_INT_TOI  (1 << SPI_ICR_CTOI_SHIFT)
#define SPI_ICR_SSPI_SHIFT (1 << SPI_ICR_CSSPI_SHIFT)
#define SPI_CLEAR_INT_TXFI (1 << SPI_ICR_CTXFI_SHIFT)

/* Bit fields in DMACR */
#define SPI_DMACR_TX_ENABLE (1 << SPI_DMACR_TDE_SHIFT)
#define SPI_DMACR_RX_ENABLE (1 << SPI_DMACR_RDE_SHIFT)

/* Bit fields in SPI TIMEOUT */
#define SPI_TIMEOUT_ENABLE  (1 << SPI_TIMEOUT_TOE_SHIFT)
#define SPI_TIMEOUT_DISABLE 0

#define IS_SPI_MODE(__MODE__) (((__MODE__) == CR0_OPM_SLAVE) || \
                               ((__MODE__) == CR0_OPM_MASTER))

#define IS_SPI_DIRECTION(__MODE__) (((__MODE__) == CR0_XFM_TR)        || \
                                    ((__MODE__) == CR0_XFM_TO) ||        \
                                    ((__MODE__) == CR0_XFM_RO))

#define IS_SPI_DATASIZE(__DATASIZE__) (((__DATASIZE__) == CR0_DATA_FRAME_SIZE_4BIT) || \
                                       ((__DATASIZE__) == CR0_DATA_FRAME_SIZE_8BIT) || \
                                       ((__DATASIZE__) == CR0_DATA_FRAME_SIZE_16BIT))

#define IS_SPI_CPOL(__CPOL__) (((__CPOL__) == CR0_POLARITY_LOW) || \
                               ((__CPOL__) == CR0_POLARITY_HIGH))

#define IS_SPI_CPHA(__CPHA__) (((__CPHA__) == CR0_PHASE_1EDGE) || \
                               ((__CPHA__) == CR0_PHASE_2EDGE))

#define IS_SPI_FIRST_BIT(__BIT__) (((__BIT__) == CR0_FIRSTBIT_MSB) || \
                                   ((__BIT__) == CR0_FIRSTBIT_LSB))

#define IS_SPI_APBTRANSFORM(__MODE__) (((__MODE__) == CR0_BHT_16BIT) || \
                                      ((__MODE__) == CR0_BHT_8BIT))

#define IS_SPI_ENDIAN_MODE(__MODE__) (((__MODE__) == CR0_EM_BIG) || \
                                      ((__MODE__) == CR0_EM_LITTLE))

#define IS_SPI_SSD_BIT(__MODE__) (((__MODE__) == CR0_SSD_HALF) || \
                                  ((__MODE__) == CR0_SSD_ONE))

#define IS_SPI_CSM(__NCYCLES__) (((__NCYCLES__) == CR0_CSM_0CYCLE) ||  \
                                 ((__NCYCLES__) == CR0_CSM_1CYCLE) ||  \
                                 ((__NCYCLES__) == CR0_CSM_2CYCLES) || \
                                 ((__NCYCLES__) == CR0_CSM_3CYCLES))

/**
  * @brief  Initialize the SPI according to the specified parameters.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  base: SPI controller register base address.
  * @return status
  */
RETURN_STATUS
EFIAPI
SPI_Init(struct SPI_HANDLE *pSPI, UINT32 base)
{
  pSPI->pReg = (struct SPI_REG *)(long)base;

  pSPI->config.opMode = CR0_OPM_MASTER;

  /* Default config */
  pSPI->config.apbTransform = CR0_BHT_8BIT;
  pSPI->config.endianMode = CR0_EM_BIG;
  pSPI->config.ssd = CR0_SSD_ONE;
  pSPI->config.csm = CR0_CSM_0CYCLE;

  return RETURN_SUCCESS;
}

/**
  * @brief  Start or stop the spi module.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  enable: start or stop the spi module.
  * @return status
  */
static inline RETURN_STATUS SPI_EnableChip(struct SPI_HANDLE *pSPI, int enable)
{
  WRITE_REG(pSPI->pReg->ENR, (enable ? 1 : 0));

  return RETURN_SUCCESS;
}

/**
  * @brief  Configure the spi clock division.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  div: clock division.
  * @return status
  */
static inline RETURN_STATUS SPI_SetClock(struct SPI_HANDLE *pSPI, UINT16 div)
{
  WRITE_REG(pSPI->pReg->BAUDR, div);

  return RETURN_SUCCESS;
}

/**
  * @brief  Configure the cs signal.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param select: cs number select.
  * @param  enable: active or inactive the cs signal.
  * @return status
  */
RETURN_STATUS
EFIAPI
SPI_SetCS(struct SPI_HANDLE *pSPI, UINT8 select, UINT8 enable)
{
  UINT32 ser;

  ASSERT(pSPI != NULL);

  ser = READ_REG(pSPI->pReg->SER) & SPI_SER_SER_MASK;

  if (enable) {
    ser |= 1 << select;
  } else {
    ser &= ~(1 << select);
  }

  WRITE_REG(pSPI->pReg->SER, ser);

  return RETURN_SUCCESS;
}

/**
  * @brief  Wait for the transfer finished.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
RETURN_STATUS
EFIAPI
SPI_FlushFifo(struct SPI_HANDLE *pSPI)
{
  ASSERT(pSPI != NULL);

  while (READ_REG(pSPI->pReg->RXFLR)) {
    READ_REG(pSPI->pReg->RXDR);
  }

  return RETURN_SUCCESS;
}

/**
  * @brief  The max amount of data can be written in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return Max bytes can xfer.
  */
static inline UINT32 SPI_TxMax(struct SPI_HANDLE *pSPI)
{
  UINT32 txLeft, txRoom;

  txLeft = (pSPI->pTxBufferEnd - pSPI->pTxBuffer) / pSPI->config.nBytes;
  txRoom = HAL_SPI_FIFO_LENGTH - READ_REG(pSPI->pReg->TXFLR);

  return MIN(txLeft, txRoom);
}

/**
  * @brief  Send an amount of data in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
static RETURN_STATUS SPI_PioWrite(struct SPI_HANDLE *pSPI)
{
  UINT32 max = SPI_TxMax(pSPI);
  UINT32 txw = 0;

  while (max--) {
    if (pSPI->config.nBytes == 1) {
      txw = *(const UINT8 *)(pSPI->pTxBuffer);
    } else {
      txw = *(const UINT16 *)(pSPI->pTxBuffer);
    }

    WRITE_REG(pSPI->pReg->TXDR, txw);
    pSPI->pTxBuffer += pSPI->config.nBytes;
  }

  return RETURN_SUCCESS;
}

/**
  * @brief  Read an amount of data(byte) in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
static RETURN_STATUS SPI_PioReadByte(struct SPI_HANDLE *pSPI)
{
  UINT32 rxLeft = pSPI->pRxBufferEnd - pSPI->pRxBuffer;
  UINT32 rxRoom = READ_REG(pSPI->pReg->RXFLR);
  UINT32 max = MIN(rxLeft, rxRoom);

  while (max > 7) {
    *(pSPI->pRxBuffer + 0) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 1) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 2) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 3) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 4) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 5) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 6) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    *(pSPI->pRxBuffer + 7) = (UINT8)READ_REG(pSPI->pReg->RXDR);
    pSPI->pRxBuffer += 8;
    max -= 8;
  }

  while (max--) {
    *pSPI->pRxBuffer = (UINT8)READ_REG(pSPI->pReg->RXDR);
    pSPI->pRxBuffer++;
  }

  return RETURN_SUCCESS;
}

/**
  * @brief  Read an amount of data(short) in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
static RETURN_STATUS SPI_PioReadShort(struct SPI_HANDLE *pSPI)
{
  UINT32 rxLeft = (pSPI->pRxBufferEnd - pSPI->pRxBuffer) >> 1;
  UINT32 rxRoom = READ_REG(pSPI->pReg->RXFLR);
  UINT32 max = MIN(rxLeft, rxRoom);

  while (max > 7) {
    *((UINT16 *)pSPI->pRxBuffer + 0) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 1) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 2) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 3) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 4) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 5) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 6) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    *((UINT16 *)pSPI->pRxBuffer + 7) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    pSPI->pRxBuffer += 16;
    max -= 8;
  }

  while (max--) {
    *((UINT16 *)pSPI->pRxBuffer) = (UINT16)READ_REG(pSPI->pReg->RXDR);
    pSPI->pRxBuffer += 2;
  }

  return RETURN_SUCCESS;
}

/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
RETURN_STATUS
EFIAPI
SPI_PioTransfer(struct SPI_HANDLE *pSPI)
{
  UINT32 remain = 0;

  ASSERT(pSPI != NULL);

  pSPI->type = SPI_POLL;
  SPI_EnableChip(pSPI, 1);

  do {
    if (pSPI->pTxBuffer) {
      remain = pSPI->pTxBufferEnd - pSPI->pTxBuffer;
      SPI_PioWrite(pSPI);
    }

    if (pSPI->pRxBuffer) {
      remain = pSPI->pRxBufferEnd - pSPI->pRxBuffer;

      if (pSPI->config.nBytes == 1) {
        SPI_PioReadByte(pSPI);
      } else {
        SPI_PioReadShort(pSPI);
      }
    }
  } while (remain);

  return RETURN_SUCCESS;
}

/**
  * @brief  Query the SPI bus state is idle or busy.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
RETURN_STATUS SPI_QueryBusState(struct SPI_HANDLE *pSPI)
{
  HAL_ASSERT(pSPI != NULL);
  if (!(READ_REG(pSPI->pReg->SR) & HAL_SPI_SR_BUSY)) {
    return RETURN_SUCCESS;
  }

  return -2;
}

/**
  * @brief  Stop the transmit.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
RETURN_STATUS
EFIAPI
SPI_Stop(struct SPI_HANDLE *pSPI)
{
  UINT32 ret;
  ASSERT(pSPI != NULL);

  do
  {
    ret = SPI_QueryBusState(pSPI);
    if (ret == RETURN_SUCCESS)
      break;
  } while(1);

  SPI_EnableChip(pSPI, 0);

  return RETURN_SUCCESS;
}

/**
  * @brief  Configure the SPI transfer mode depend on the tx/rx buffer.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return status
  */
static RETURN_STATUS HAL_SPI_ConfigureTransferMode(struct SPI_HANDLE *pSPI)
{
    UINT32 cr0;

    if (pSPI->pTxBuffer && pSPI->pRxBuffer) {
        pSPI->config.xfmMode = CR0_XFM_TR;
    } else if (pSPI->pTxBuffer) {
        pSPI->config.xfmMode = CR0_XFM_TO;
    } else if (pSPI->pRxBuffer) {
        pSPI->config.xfmMode = CR0_XFM_RO;
    }

    cr0 = READ_REG(pSPI->pReg->CTRLR[0]);
    cr0 &= ~SPI_CTRLR0_XFM_MASK;
    cr0 |= pSPI->config.xfmMode;

    WRITE_REG(pSPI->pReg->DMARDLR, pSPI->dmaBurstSize - 1);
    WRITE_REG(pSPI->pReg->CTRLR[0], cr0);

    return RETURN_SUCCESS;
}

/**
  * @brief  Program the SPI config via this api.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData: pointer to TX buffer.
  * @param  pRxData: pointer to RX buffer.
  * @param  size: amount of data to be sent.
  * @return status
  */
RETURN_STATUS
EFIAPI
SPI_Configure(struct SPI_HANDLE *pSPI, const UINT8 *pTxData, UINT8 *pRxData, UINT32 size)
{
  UINT32 cr0 = 0;
  UINT32 div = 0;

  cr0 |= pSPI->config.opMode;

  cr0 |= pSPI->config.apbTransform | pSPI->config.endianMode | pSPI->config.ssd;
  /* Data width */
  cr0 |= pSPI->config.nBytes;

  /* Mode for polarity, phase, first bit and endian */
  cr0 |= pSPI->config.clkPolarity | pSPI->config.clkPhase | pSPI->config.firstBit;

  /* Config CSM cycles */
  cr0 |= pSPI->config.csm;

  /* div doesn't support odd number */
  div = DIV_ROUND_UP(pSPI->maxFreq, pSPI->config.speed);
  div = (div + 1) & 0xfffe;

  WRITE_REG(pSPI->pReg->CTRLR[0], cr0);

  WRITE_REG(pSPI->pReg->TXFTLR, HAL_SPI_FIFO_LENGTH / 2 - 1);
  WRITE_REG(pSPI->pReg->RXFTLR, HAL_SPI_FIFO_LENGTH / 2 - 1);

  WRITE_REG(pSPI->pReg->DMATDLR, HAL_SPI_FIFO_LENGTH / 2 - 1);
  WRITE_REG(pSPI->pReg->DMARDLR, 0);

  SPI_SetClock(pSPI, div);

  pSPI->pTxBuffer = pTxData;
  pSPI->pTxBufferEnd = pTxData + size;
  pSPI->pRxBuffer = pRxData;
  pSPI->pRxBufferEnd = pRxData + size;
  pSPI->len = size;

  HAL_SPI_ConfigureTransferMode(pSPI);

  if (pSPI->config.xfmMode == CR0_XFM_RO) {
    if (pSPI->config.nBytes == 1) {
      WRITE_REG(pSPI->pReg->CTRLR[1], pSPI->len - 1);
    } else if (pSPI->config.nBytes == 2) {
      WRITE_REG(pSPI->pReg->CTRLR[1], (pSPI->len / 2) - 1);
    } else {
      WRITE_REG(pSPI->pReg->CTRLR[1], (pSPI->len * 2) - 1);
    }
  }

  return RETURN_SUCCESS;
}
