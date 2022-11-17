/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "Soc.h"
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/SpiMem.h>
#include <Library/Fspi.h>

#define HAL_IS_ALIGNED(x, a) (((x) & (a - 1)) == 0)
/********************* Private MACRO Definition ******************************/
// #define FSPI_DEBUG
#ifdef FSPI_DEBUG
#define FSPI_DBG(...)
#else
#define FSPI_DBG(...)
#endif

/* FSPI_CTRL */
#define FSPI_CTRL_SHIFTPHASE_NEGEDGE 1

/* FSPI_RCVR */
#define FSPI_RCVR_RCVR_RESET (1 << FSPI_RCVR_RCVR_SHIFT) /* Recover The FSPI State Machine */

/* FSPI_ISR */
#define FSPI_ISR_DMAS_ACTIVE   (1 << FSPI_ISR_DMAS_SHIFT) /* DMA Finish Interrupt Active */
#define FSPI_ISR_NSPIS_ACTIVE  (1 << FSPI_ISR_NSPIS_SHIFT) /* SPI Error Interrupt Active */
#define FSPI_ISR_AHBS_ACTIVE   (1 << FSPI_ISR_AHBS_SHIFT) /* AHB Error Interrupt Active */
#define FSPI_ISR_TRANSS_ACTIVE (1 << FSPI_ISR_TRANSS_SHIFT) /* Transfer finish Interrupt Active */
#define FSPI_ISR_TXES_ACTIVE   (1 << FSPI_ISR_TXES_SHIFT) /* Transmit FIFO Empty Interrupt Active */
#define FSPI_ISR_TXOS_ACTIVE   (1 << FSPI_ISR_TXOS_SHIFT) /* Transmit FIFO Overflow Interrupt Active */
#define FSPI_ISR_RXUS_ACTIVE   (1 << FSPI_ISR_RXUS_SHIFT) /* Receive FIFO Underflow Interrupt Active */
#define FSPI_ISR_RXFS_ACTIVE   (1 << FSPI_ISR_RXFS_SHIFT) /* Receive FIFO Full Interrupt Active */

/* FSPI_FSR */
#define FSPI_FSR_RXFS_EMPTY (1 << FSPI_FSR_RXFS_SHIFT) /* Receive FIFO Full */
#define FSPI_FSR_RXES_EMPTY (1 << FSPI_FSR_RXES_SHIFT) /* Receive FIFO Empty */
#define FSPI_FSR_TXFS_FULL  (1 << FSPI_FSR_TXFS_SHIFT) /* Transmit FIFO Full */
#define FSPI_FSR_TXES_EMPTY (1 << FSPI_FSR_TXES_SHIFT) /* Transmit FIFO Empty */

/* FSPI_SR */
#define FSPI_SR_SR_BUSY (1 << FSPI_SR_SR_SHIFT) /* When busy, do not set the control register. */

/* FSPI_DMATR */
#define FSPI_DMATR_DMATR_START (1 << FSPI_DMATR_DMATR_SHIFT) /* Write 1 to start the dma transfer. */

/* FSPI_RISR */
#define FSPI_RISR_TRANSS_ACTIVE (1 << FSPI_RISR_TRANSS_SHIFT)

/* FSPI attributes */
#define FSPI_VER_VER_1 1
#define FSPI_VER_VER_3 3
#define FSPI_VER_VER_5 5

#define FSPI_NOR_FLASH_PAGE_SIZE 0x100

#define FSPI_MAX_IOSIZE_VER3 (1024U * 8)
#define FSPI_MAX_IOSIZE_VER4 (0xFFFFFFFFU)

/********************* Private Structure Definition **************************/

typedef union {
  UINT32 d32;
  struct {
    unsigned txempty : 1; /* tx fifo empty */
    unsigned txfull : 1; /* tx fifo full */
    unsigned rxempty : 1; /* rx fifo empty */
    unsigned rxfull : 1; /* tx fifo empty interrupt mask */
    unsigned reserved7_4 : 4;
    unsigned txlevel : 5; /* tx fifo: 0: full; 1: left 1 entry; ... */
    unsigned reserved15_13 : 3;
    unsigned rxlevel : 5; /* rx fifo: 0: full; 1: left 1 entry; ... */
    unsigned reserved31_21 : 11;
  } b;
} FSPIFSR_DATA;

/** FSPI_XMMC bit union */
typedef union {
  UINT32 d32;
  struct {
    unsigned reserverd1 : 5;
    unsigned devHwEn : 1; /* device Hwrite Enable */
    unsigned prefetch : 1; /* prefetch enable */
    unsigned uincrPrefetchEn : 1; /* undefine INCR Burst Prefetch Enable */
    unsigned uincrLen : 4; /* undefine INCR length */
    unsigned devWrapEn : 1; /* device Wrap Enable */
    unsigned devIncrEn : 1; /* device INCR2/4/8/16 Enable */
    unsigned devUdfincrEn : 1; /* device Undefine INCR Enable */
    unsigned reserved2 : 17;
  } b;
} FSPIXMMCCTRL_DATA;
/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
static void FSPI_Reset(struct HAL_FSPI_HOST *host)
{
  INT32 timeout = 10000;

  host->instance->RCVR = FSPI_RCVR_RCVR_RESET;
  while ((host->instance->RCVR == FSPI_RCVR_RCVR_RESET) && (timeout > 0)) {
    HAL_CPUDelayUs(1);
    timeout--;
  }
  host->instance->ICLR = 0xFFFFFFFF;
}

static void FSPI_ContModeInit(struct HAL_FSPI_HOST *host)
{
  /* Setup when FSPI->AX = 0xa5, Cancel when FSPI->AX = 0x0 */
  WRITE_REG(host->instance->EXT_AX, 0xa5 << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT |
            0x5a << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT);
  /* Skip Continuous mode in default */
  switch (host->cs) {
  case 0:
    WRITE_REG(host->instance->AX0, 0 << FSPI_AX0_AX_SHIFT);
    break;
  case 1:
    WRITE_REG(host->instance->AX1, 0 << FSPI_AX1_AX_SHIFT);
    break;
  default:
    break;
  }
}

static void FSPI_ContModeDeInit(struct HAL_FSPI_HOST *host)
{
  /* FSPI avoid setup pattern match with FSPI->AX */
  WRITE_REG(host->instance->EXT_AX, 0x77 << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT |
            0x88 << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT);
}

/**
 * @brief  Configuration register with flash operation protocol.
 * @param  host: FSPI host.
 * @param  op: flash operation protocol.
 * @return RETURN_STATUS.
 * @attention Set host->cs to select chip.
 */
RETURN_STATUS HAL_FSPI_XferStart(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    struct FSPI_REG *pReg = host->instance;
    FSPICMD_DATA FSPICmd;
    FSPICTRL_DATA FSPICtrl;

    FSPICmd.d32 = 0;
    FSPICtrl.d32 = 0;

    /* set CMD */
    FSPICmd.b.cmd = op->cmd.opcode;
    if (op->cmd.buswidth == 4) {
        FSPICtrl.b.cmdlines = FSPI_LINES_X4;
        FSPICtrl.b.datalines = FSPI_LINES_X4; /* cmdlines work with datalines */
    }

    /* set ADDR */
    if (op->addr.nbytes) {
        if (op->addr.nbytes == 4) {
            FSPICmd.b.addrbits = FSPI_ADDR_32BITS;
        } else if (op->addr.nbytes == 3) {
            FSPICmd.b.addrbits = FSPI_ADDR_24BITS;
        } else {
            FSPICmd.b.addrbits = FSPI_ADDR_XBITS;
            pReg->ABIT0 = op->addr.nbytes * 8 - 1;
        }

        FSPICtrl.b.addrlines = op->addr.buswidth == 4 ? FSPI_LINES_X4 : FSPI_LINES_X1;
    }

    /* set DUMMY*/
    if (op->dummy.nbytes) {
        switch (op->dummy.buswidth) {
        case 4:
            FSPICmd.b.dummybits = op->dummy.nbytes * 2;
            break;
        case 2:
            FSPICmd.b.dummybits = op->dummy.nbytes * 4;
            break;
        default:
            FSPICmd.b.dummybits = op->dummy.nbytes * 8;
            break;
        }
    }

    /* set DATA */
    WRITE_REG(pReg->LEN_EXT, op->data.nbytes);
    if (op->data.nbytes) {
        if (op->data.dir == HAL_SPI_MEM_DATA_OUT) {
            FSPICmd.b.rw = FSPI_WRITE;
        }
        if (op->data.buswidth == 4) {
            FSPICtrl.b.datalines = FSPI_LINES_X4;
        } else if (op->data.buswidth == 2) {
            FSPICtrl.b.datalines = FSPI_LINES_X2;
        } else {
            FSPICtrl.b.datalines = FSPI_LINES_X1;
        }
    }

    /* spitial setting */
    FSPICtrl.b.sps = host->mode & HAL_SPI_CPHA;
    FSPICmd.b.cs = host->cs;
    if (op->data.nbytes == 0 && op->addr.nbytes) {
        FSPICmd.b.rw = FSPI_WRITE;
    }

    if (!(pReg->FSR & FSPI_FSR_TXES_EMPTY) || !(pReg->FSR & FSPI_FSR_RXES_EMPTY) || (pReg->SR & FSPI_SR_SR_BUSY)) {
        FSPI_Reset(host);
    }

    // FSPI_DBG("%s 1 %x %x %x\n", __func__, op->addr.nbytes, op->dummy.nbytes, op->data.nbytes);
    // FSPI_DBG("%s 2 %lx %lx %lx %x\n", __func__, FSPICtrl.d32, FSPICmd.d32, op->addr.val, host->cs);

    /* config FSPI */
    switch (host->cs) {
    case 0:
        pReg->CTRL0 = FSPICtrl.d32;
        break;
    case 1:
        pReg->CTRL1 = FSPICtrl.d32;
        break;
    default:
        break;
    }
    pReg->CMD = FSPICmd.d32;
    if (op->addr.nbytes) {
        pReg->ADDR = op->addr.val;
    }

    return RETURN_SUCCESS;
}

/**
 * @brief  IO transfer.
 * @param  host: FSPI host.
 * @param  len: data n bytes.
 * @param  data: transfer buffer.
 * @param  dir: transfer direction.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_FSPI_XferData(struct HAL_FSPI_HOST *host, UINT32 len, void *data, UINT32 dir)
{
    RETURN_STATUS ret = RETURN_SUCCESS;
    __IO FSPIFSR_DATA fifostat;
    INT32 timeout = 0;
    UINT32 i, words;
    UINT32 *pData = (UINT32 *)data;
    struct FSPI_REG *pReg = host->instance;
    UINT32 temp = 0;

    HAL_ASSERT(data && len);

    if (len && len < 4 && dir == FSPI_WRITE) {
        if (len == 1) {
            temp = *((uint8_t *)data);
        } else if (len == 2) {
            temp = *((uint16_t *)data);
        } else {
            temp = ((uint8_t *)data)[0] | ((uint8_t *)data)[1] << 8 | ((uint8_t *)data)[2] << 16;
        }
        pData = &temp;
    } else if (len >= 4 && !HAL_IS_ALIGNED((long)data, 4)) {
        FSPI_DBG("%s data unaligned access\n", __func__);
    }

    /* FSPI_DBG("%s %p len %lx word0 %lx dir %lx\n", __func__, pData, len, pData[0], dir); */
    if (dir == FSPI_WRITE) {
        words = (len + 3) >> 2;
        while (words) {
            fifostat.d32 = pReg->FSR;
            if (fifostat.b.txlevel > 0) {
                UINT32 count = MIN(words, fifostat.b.txlevel);

                for (i = 0; i < count; i++) {
                    pReg->DATA = *pData++;
                    words--;
                }
                if (words == 0) {
                    break;
                }
                timeout = 0;
            } else {
                HAL_CPUDelayUs(1);
                if (timeout++ > 10000) {
                    ret = RETURN_TIMEOUT;
                    break;
                }
            }
        }
    } else {
        UINT32 bytes = len & 0x3;

        words = len >> 2;
        while (words) {
            fifostat.d32 = pReg->FSR;
            if (fifostat.b.rxlevel > 0) {
                UINT32 count = MIN(words, fifostat.b.rxlevel);
                if (count == 15) { /* Reduce CPU cost */
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;

                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    words -= 15;
                } else {
                    for (i = 0; i < count; i++) {
                        *pData++ = pReg->DATA;
                    }
                    words -= count;
                }

                if (0 == words) {
                    break;
                }
                timeout = 0;
            } else {
                HAL_CPUDelayUs(1);
                if (timeout++ > 10000) {
                    ret = RETURN_TIMEOUT;
                    break;
                }
            }
        }

        timeout = 0;
        while (bytes) {
            fifostat.d32 = pReg->FSR;
            if (fifostat.b.rxlevel > 0) {
                uint8_t *pData1 = (uint8_t *)pData;
                words = pReg->DATA;
                for (i = 0; i < bytes; i++) {
                    pData1[i] = (uint8_t)((words >> (i * 8)) & 0xFF);
                }
                break;
            } else {
                HAL_CPUDelayUs(1);
                if (timeout++ > 10000) {
                    ret = RETURN_TIMEOUT;
                    break;
                }
            }
        }
    }

    return ret;
}

/**
 * @brief  Wait for FSPI host transfer finished.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_FSPI_XferDone(struct HAL_FSPI_HOST *host)
{
    RETURN_STATUS ret = RETURN_SUCCESS;
    INT32 timeout = 0;
    struct FSPI_REG *pReg = host->instance;

    while (pReg->SR & FSPI_SR_SR_BUSY) {
        HAL_CPUDelayUs(1);
        if (timeout++ > 100000) { /*wait 100ms*/
            ret = RETURN_TIMEOUT;
            break;
        }
    }
    HAL_CPUDelayUs(1); //CS# High Time (read/write) >100ns

    return ret;
}

/**
 * @brief  SPI Nor flash data transmission interface to support open source specifications SNOR.
 * @param  host: FSPI host.
 * @param  op: flash operation protocol.
 * @return RETURN_STATUS.
 * @attention Set host->cs to select chip.
 */
RETURN_STATUS HAL_FSPI_SpiXfer(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    RETURN_STATUS ret = RETURN_SUCCESS;
    UINT32 dir = op->data.dir;
    void *pData = NULL;

    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    if (op->data.buf.in) {
        pData = (void *)op->data.buf.in;
    } else if (op->data.buf.out) {
        pData = (void *)op->data.buf.out;
    }

    HAL_FSPI_XferStart(host, op);
    if (pData) {
        ret = HAL_FSPI_XferData(host, op->data.nbytes, pData, dir);
        if (ret) {
            FSPI_DBG("%s xfer data failed ret %d\n", __func__, ret);

            return ret;
        }
    }

    return HAL_FSPI_XferDone(host);
}

/** @} */

/** @defgroup FSPI_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Init FSPI.
 * @param  host: FSPI host.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_FSPI_Init(struct HAL_FSPI_HOST *host)
{
    INT32 timeout = 0;
    struct FSPI_REG *pReg;
    RETURN_STATUS ret = RETURN_SUCCESS;

    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    pReg = host->instance;
    pReg->MODE = 0;
    while (pReg->SR & FSPI_SR_SR_BUSY) {
        HAL_CPUDelayUs(1);
        if (timeout++ > 1000) {
            return RETURN_TIMEOUT;
        }
    }
    FSPI_ContModeInit(host);
    pReg->CTRL0 = 0;

#if (FSPI_VER > FSPI_VER_VER_3)
    WRITE_REG(pReg->LEN_CTRL, FSPI_LEN_CTRL_TRB_SEL_MASK);
#endif

    return ret;
}

/**
 * @brief  DeInit FSPI.
 * @param  host: FSPI host.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_FSPI_DeInit(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    host->instance->MODE = 0;
    FSPI_ContModeDeInit(host);
    FSPI_Reset(host);

    return RETURN_SUCCESS;
}

/**
 * @brief  Set FSPI delay line.
 * @param  host: FSPI host.
 * @param  cells: delay line cells.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_FSPI_SetDelayLines(struct HAL_FSPI_HOST *host, uint8_t cells)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));
    if (host->cs == 0) {
        WRITE_REG(host->instance->DLL_CTRL0, 1 << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT | cells);
    } else {
        WRITE_REG(host->instance->DLL_CTRL1, 1 << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT | cells);
    }

    return RETURN_SUCCESS;
}

UINT32 HAL_FSPI_GetMaxIoSize(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    return FSPI_MAX_IOSIZE_VER4;
}
