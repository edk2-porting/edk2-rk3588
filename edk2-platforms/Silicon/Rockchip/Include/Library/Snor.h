/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef _HAL_SFC_NOR_H_
#define _HAL_SFC_NOR_H_

/***************************** Structure Definition **************************/

#define SPI_NOR_MAX_CMD_SIZE 8
#define SNOR_SPEED_MAX       133000000
#define SNOR_SPEED_DEFAULT   80000000

#define SNOR_PROTO_STR(a, b, c) (((a) << 8) | ((b) << 4) | (c))

#define SNOR_GET_PROTOCOL_ADDR_BITS(proto) (((proto) >> 4) & 0xf)
#define SNOR_GET_PROTOCOL_DATA_BITS(proto) ((proto) & 0xf)

enum SPI_NOR_PROTOCOL {
    SNOR_PROTO_1_1_1 = SNOR_PROTO_STR(1, 1, 1),
    SNOR_PROTO_1_1_2 = SNOR_PROTO_STR(1, 1, 2),
    SNOR_PROTO_1_1_4 = SNOR_PROTO_STR(1, 1, 4),
    SNOR_PROTO_1_1_8 = SNOR_PROTO_STR(1, 1, 8),
    SNOR_PROTO_1_2_2 = SNOR_PROTO_STR(1, 2, 2),
    SNOR_PROTO_1_4_4 = SNOR_PROTO_STR(1, 4, 4),
    SNOR_PROTO_1_8_8 = SNOR_PROTO_STR(1, 8, 8),
    SNOR_PROTO_2_2_2 = SNOR_PROTO_STR(2, 2, 2),
    SNOR_PROTO_4_4_4 = SNOR_PROTO_STR(4, 4, 4),
    SNOR_PROTO_8_8_8 = SNOR_PROTO_STR(8, 8, 8),
};

/* Flash opcodes. */
#define SPINOR_OP_WREN       0x06 /**< Write enable */
#define SPINOR_OP_RDSR       0x05 /**< Read status register */
#define SPINOR_OP_WRSR       0x01 /**< Write status register 1 byte */
#define SPINOR_OP_RDSR1      0x35 /**< Read status register 1 */
#define SPINOR_OP_WRSR1      0x31 /**< Write status register 1 */
#define SPINOR_OP_RDSR2      0x15 /**< Read status register 2 */
#define SPINOR_OP_WRSR2      0x3e /**< Write status register 2 */
#define SPINOR_OP_READ       0x03 /**< Read data bytes (low frequency) */
#define SPINOR_OP_READ_FAST  0x0b /**< Read data bytes (high frequency) */
#define SPINOR_OP_READ_1_1_2 0x3b /**< Read data bytes (Dual Output SPI) */
#define SPINOR_OP_READ_1_2_2 0xbb /**< Read data bytes (Dual I/O SPI) */
#define SPINOR_OP_READ_1_1_4 0x6b /**< Read data bytes (Quad Output SPI) */
#define SPINOR_OP_READ_1_4_4 0xeb /**< Read data bytes (Quad I/O SPI) */
#define SPINOR_OP_READ_EC    0xec /**< Read data bytes (Quad I/O SPI) 4 byte address */
#define SPINOR_OP_PP         0x02 /**< Page program (up to 256 bytes) */
#define SPINOR_OP_PP_1_1_4   0x32 /**< Quad page program */
#define SPINOR_OP_PP_1_4_4   0x38 /**< Quad page program */
#define SPINOR_OP_BE_4K      0x20 /**< Erase 4KiB block */
#define SPINOR_OP_BE_4K_PMC  0xd7 /**< Erase 4KiB block on PMC chips */
#define SPINOR_OP_BE_32K     0x52 /**< Erase 32KiB block */
#define SPINOR_OP_CHIP_ERASE 0xc7 /**< Erase whole flash chip */
#define SPINOR_OP_SE         0xd8 /**< Sector erase (usually 64KiB) */
#define SPINOR_OP_RDID       0x9f /**< Read JEDEC ID */
#define SPINOR_OP_RDSFDP     0x5a /**< Read SFDP */
#define SPINOR_OP_RDCR       0x15 /**< Read configuration register */
#define SPINOR_OP_WRCR       0x11 /**< Write configuration register */
#define SPINOR_OP_RDFSR      0x70 /**< Read flag status register */
#define SPINOR_OP_CLFSR      0x50 /**< Clear flag status register */
#define SPINOR_OP_RDEAR      0xc8 /**< Read Extended Address Register */
#define SPINOR_OP_WREAR      0xc5 /**< Write Extended Address Register */
#define SPINOR_OP_READ_UUID  0x4b /**< Read SPI Nor UUID */
#define SPINOR_OP_READ_SFDP  0x5A /**< Read SPI Nor SFDP */

struct SPI_NOR {
    struct HAL_FSPI_HOST *spi;
    const struct FLASH_INFO *info;
    UINT32 pageSize;
    UINT8 addrWidth;
    UINT8 eraseOpcodeSec;
    UINT8 eraseOpcodeBlk;
    UINT8 readOpcode;
    UINT8 readDummy;
    UINT8 programOpcode;

    enum SPI_NOR_PROTOCOL readProto;
    enum SPI_NOR_PROTOCOL writeProto;
    UINT8 cmdBuf[SPI_NOR_MAX_CMD_SIZE];

    UINT32 size;
    UINT32 sectorSize;
    UINT32 eraseSize;
};

typedef enum {
    ERASE_SECTOR = 0,
    ERASE_BLOCK64K,
    ERASE_CHIP
} NOR_ERASE_TYPE;

RETURN_STATUS HAL_SNOR_Init(struct SPI_NOR *nor);
RETURN_STATUS HAL_SNOR_DeInit(struct SPI_NOR *nor);
UINT32 HAL_SNOR_GetCapacity(struct SPI_NOR *nor);
RETURN_STATUS HAL_SNOR_ReadID(struct SPI_NOR *nor, UINT8 *data);
RETURN_STATUS HAL_SNOR_ReadData(struct SPI_NOR *nor, UINT32 from, void *buf, UINT32 len);
RETURN_STATUS HAL_SNOR_ProgData(struct SPI_NOR *nor, UINT32 to, void *buf, UINT32 len);
RETURN_STATUS HAL_SNOR_Read(struct SPI_NOR *nor, UINT32 sec, UINT32 nSec, void *pData);
RETURN_STATUS HAL_SNOR_Write(struct SPI_NOR *nor, UINT32 sec, UINT32 nSec, void *pData);
RETURN_STATUS HAL_SNOR_OverWrite(struct SPI_NOR *nor, UINT32 sec, UINT32 nSec, void *pData);
RETURN_STATUS HAL_SNOR_Erase(struct SPI_NOR *nor, UINT32 addr, NOR_ERASE_TYPE EraseType);
BOOLEAN HAL_SNOR_IsFlashSupported(UINT8 *flashId);
RETURN_STATUS HAL_SNOR_ReadUUID(struct SPI_NOR *nor, void *buf);

#endif
