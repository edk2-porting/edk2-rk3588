/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */
#ifndef _HAL_FSPI_H_
#define _HAL_FSPI_H_

#define HAL_FSPI_QUAD_ENABLE
#define HAL_FSPI_SPEED_THRESHOLD 100000000

/***************************** Structure Definition **************************/
/** FSPI_CTRL register datalines, addrlines and cmdlines value */
#define FSPI_LINES_X1 (0)
#define FSPI_LINES_X2 (1)
#define FSPI_LINES_X4 (2)

/** FSPI_CTRL bit union */
typedef union {
    UINT32 d32;
    struct {
        unsigned mode : 1; /**< spi mode select */
        unsigned sps : 1; /**< shift in phase at: posedge 1: negedge */
        unsigned reserved3_2 : 2;
        unsigned scic : 4; /**< sclk_idle_level_cycles */
        unsigned cmdlines : 2; /**< cmd bits number */
        unsigned addrlines : 2; /**< address bits number */
        unsigned datalines : 2; /**< data bits number */
        unsigned reserved14_15 : 2;
        unsigned addrbits : 5;
        unsigned reserved31_21 : 11;
    } b;
} FSPICTRL_DATA;

/** FSPI_CMD register rw value without shift */
#define FSPI_READ  (0)
#define FSPI_WRITE (1)

/** FSPI_CMD regitser addrbits value without shift */
#define FSPI_ADDR_0BITS  (0)
#define FSPI_ADDR_24BITS (1)
#define FSPI_ADDR_32BITS (2)
#define FSPI_ADDR_XBITS  (3)

/** FSPI_CMD bit union */
typedef union {
    UINT32 d32;
    struct {
        unsigned cmd : 8; /**< command that will send to Serial Flash */
        unsigned dummybits : 4; /**< dummy bits number */
        unsigned rw : 1; /**< 0:read, 1: write */
        unsigned readmode : 1; /**< continuous read mode */
        unsigned addrbits : 2; /**< address bits number */
        unsigned datasize : 14; /**< transferred bytes number */
        unsigned cs : 2; /**< chip select */
    } b;
} FSPICMD_DATA;

typedef enum {
    DEV_UNKNON = 0,
    DEV_NOR,
    DEV_PSRAM,
} eFSPI_devType;

struct HAL_FSPI_XMMC_DEV {
    eFSPI_devType type;
    UINT32 ctrl;
    UINT32 readCmd;
    UINT32 writeCmd;
};

/** XIP may be not accessble, so place it in sram or psram */
struct HAL_FSPI_HOST {
    struct FSPI_REG *instance;
    UINT32 *CruBase;
    UINT8 cs; /**< Should be defined by user in each operation */
    UINT8 mode; /**< Should be defined by user, referring to hal_spi_mem.h */
    UINT8 cell; /**< Record DLL cell for PM resume, Set depend on corresponding device */
};

#define HAL_FSPI_MAX_DELAY_LINE_CELLS (0xFFU)
RETURN_STATUS HAL_FSPI_Init(struct HAL_FSPI_HOST *host);
RETURN_STATUS HAL_FSPI_DeInit(struct HAL_FSPI_HOST *host);
RETURN_STATUS HAL_FSPI_XferStart(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op);
RETURN_STATUS HAL_FSPI_XferData(struct HAL_FSPI_HOST *host, UINT32 len, void *data, UINT32 dir);
RETURN_STATUS HAL_FSPI_XferData_DMA(struct HAL_FSPI_HOST *host, UINT32 len, void *data, UINT32 dir);
RETURN_STATUS HAL_FSPI_XferDone(struct HAL_FSPI_HOST *host);
RETURN_STATUS HAL_FSPI_SpiXfer(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op);
RETURN_STATUS HAL_FSPI_IRQHelper(struct HAL_FSPI_HOST *host);
RETURN_STATUS HAL_FSPI_MaskDMAInterrupt(struct HAL_FSPI_HOST *host);
RETURN_STATUS HAL_FSPI_UnmaskDMAInterrupt(struct HAL_FSPI_HOST *host);
RETURN_STATUS HAL_FSPI_XmmcSetting(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op);
RETURN_STATUS HAL_FSPI_XmmcRequest(struct HAL_FSPI_HOST *host, UINT8 on);
RETURN_STATUS HAL_FSPI_SetDelayLines(struct HAL_FSPI_HOST *host, UINT8 cells);
RETURN_STATUS HAL_FSPI_DLLDisable(struct HAL_FSPI_HOST *host);
UINT32 HAL_FSPI_GetXMMCStatus(struct HAL_FSPI_HOST *host);
UINT32 HAL_FSPI_GetMaxIoSize(struct HAL_FSPI_HOST *host);

#endif
