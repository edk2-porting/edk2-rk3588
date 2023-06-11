/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2022 Rockchip Electronics Co., Ltd.
 */
#include <Uefi.h>
#include "Soc.h"
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <string.h>
#include <Library/TimerLib.h>
#include <Library/SpiMem.h>
#include <Library/Fspi.h>
#include <Library/Snor.h>
#include <Protocol/NorFlashProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/BaseMemoryLib.h>

#define HAL_SNOR_DEBUG
#ifdef HAL_SNOR_DEBUG
#define DEBUG_SNOR DEBUG_WARN
#else
#define DEBUG_SNOR DEBUG_INFO
#endif

struct FLASH_INFO {
    UINT32 id;

    UINT8 blockSize;
    UINT8 sectorSize;
    UINT8 readCmd;
    UINT8 progCmd;

    UINT8 readCmd_4;
    UINT8 progCmd_4;
    UINT8 sectorEraseCmd;
    UINT8 blockEraseCmd;

    UINT8 feature;
    UINT8 density; /* (1 << density) sectors */
    UINT8 QEBits;
    UINT8 reserved2;
};

/* FLASH_INFO feature */
#define FEA_STATUE_MASK     (0x3 << 0)
#define FEA_STATUE_MODE0    0  /* Readstatus0 05h-SR1, 35h-SR2, Writestatus0 01h-SR1, 31h-SR2 */
#define FEA_STATUE_MODE1    1  /* Readstatus0 05h-SR1, 35h-SR2, Writestatus1 01h-SR1-SR2 */
#define FEA_STATUE_MODE2    2  /* Readstatus1 05h-SR1, 15h-CR1, Writestatus1 01h-SR1-CR1 */
#define FEA_4BIT_READ       BIT(2)
#define FEA_4BIT_PROG       BIT(3)
#define FEA_4BYTE_ADDR      BIT(4)
#define FEA_4BYTE_ADDR_MODE BIT(5)

/*Manufactory ID*/
#define MID_WINBOND  0xEF
#define MID_GIGADEV  0xC8
#define MID_MICRON   0x2C
#define MID_MACRONIX 0xC2
#define MID_SPANSION 0x01
#define MID_EON      0x1C
#define MID_ST       0x20
#define MID_XTX      0x0B
#define MID_PUYA     0x85
#define MID_XMC      0x20

/* Used for Macronix and Winbond flashes. */
#define SPINOR_OP_EN4B 0xb7 /* Enter 4-byte mode */
#define SPINOR_OP_EX4B 0xe9 /* Exit 4-byte mode */

/* Used for SST flashes only. */
#define SPINOR_OP_WRDI 0x04 /* Write disable */

#define SPINOR_OP_MAX_SIZE 0x40
#define UINT_MAX           (~0U)

#define READ_MAX_IOSIZE (1024 * 8) /* 8KB */

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/
static const struct FLASH_INFO s_spiFlashbl[] = {
    /* GD25Q32B */
    { 0xc84016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 13, 9, 0 },
    /* GD25Q64B */
    { 0xc84017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
    /* GD25Q127C and GD25Q128C*/
    { 0xc84018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
    /* GD25Q256B/C/D */
    { 0xc84019, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1C, 16, 6, 0 },
    /* GD25LQ64C */
    { 0xc86017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
    /* GD25LQ32E */
    { 0xc86016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 13, 9, 0 },
    /* GD25Q256E */
    { 0xc86019, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 16, 9, 0 },

    /* W25Q32JV */
    { 0xef4016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
    /* W25Q64JVSSIQ */
    { 0xef4017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
    /* W25Q128FV and W25Q128JV*/
    { 0xef4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
    /* W25Q256F/J */
    { 0xef4019, 128, 8, 0x13, 0x02, 0x6C, 0x32, 0x20, 0xD8, 0x3C, 16, 9, 0 },
    /* 25Q32JW */
    { 0xef6016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
    /* W25Q256JWEQ*/
    { 0xef6019, 128, 8, 0x13, 0x02, 0x6C, 0x32, 0x20, 0xD8, 0x3C, 16, 9, 0 },
    /* W25Q64FWSSIG */
    { 0xef6017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
    /* W25Q64JVSIQ */
    { 0xef7017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
    /* W25Q128JVSIM */
    { 0xef7018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },

    /* MX25L3233FM2I-08G */
    { 0xc22016, 128, 8, 0x03, 0x02, 0x6B, 0x38, 0x20, 0xD8, 0x0E, 13, 6, 0 },
    /* MX25L6433F */
    { 0xc22017, 128, 8, 0x03, 0x02, 0x6B, 0x38, 0x20, 0xD8, 0x0E, 14, 6, 0 },
    /* MX25L12835E/F MX25L12833FMI-10G */
    { 0xc22018, 128, 8, 0x03, 0x02, 0x6B, 0x38, 0x20, 0xD8, 0x0E, 15, 6, 0 },
    /* MX25U12832F */
    { 0xc22538, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0E, 15, 6, 0 },
    /* MX25L25635E/F MX25L25645G MX25L25645GMI-08G */
    { 0xc22019, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1E, 16, 6, 0 },

    /* XM25QH32C */
    { 0x204016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
    /* XM25QH64B */
    { 0x206017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 6, 0 },
    /* XM25QH128B */
    { 0x206018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 6, 0 },
    /* XM25QH(QU)256B */
    { 0x206019, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1D, 16, 6, 0 },
    /* XM25QH64A */
    { 0x207017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 0, 0 },
    /* XM25QU128C */
    { 0x204118, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
    /* XM25QU64C */
    { 0x204117, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

    /* XT25F64BSSIGU-5 */
    { 0x0b4017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
    /* XT25F128BSSIGU */
    { 0x0b4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 9, 0 },
    /* XT25F32BS */
    { 0x0b4016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 13, 9, 0 },
    /* XT25F16BS */
    { 0x0b4015, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 12, 9, 0 },

    /* EN25QH64A */
    { 0x1c7017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 0, 0 },
    /* EN25QH128A */
    { 0x1c7018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 0, 0 },
    /* EN25QH32B */
    { 0x1c7016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 0, 0 },
    /* EN25S16A */
    { 0x1c3815, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 12, 0, 0 },
    /* EN25S32A */
    { 0x1c3816, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 0, 0 },
    /* EN25S64A */
    { 0x1c3817, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 0, 0 },
    /* EN25QH256A */
    { 0x1c7019, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x3C, 16, 0, 0 },

    /* P25Q64H */
    { 0x856017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
    /* P25Q128H */
    { 0x856018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
    /* P25Q16H-SUH-IT */
    { 0x856015, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 12, 9, 0 },
    /* FM25Q64A */
    { 0xf83217, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
    /* FM25M64C */
    { 0xf84317, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
    /* P25Q32SL */
    { 0x856016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },

    /* ZB25VQ64 */
    { 0x5e4017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
    /* ZB25VQ128 */
    { 0x5e4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
    /* ZB25LQ128 */
    { 0x5e5018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },

    /* BH25Q128AS */
    { 0x684018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
    /* BH25Q64BS */
    { 0x684017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

    /* P25Q64H */
    { 0x856017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

    /* FM25Q64-SOB-T-G */
    { 0xA14017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

    /* FM25Q64A */
    { 0xf83217, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
    /* FM25M4AA */
    { 0xf84218, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 9, 0 },
};
STATIC struct HAL_FSPI_HOST *g_spi;
STATIC struct SPI_NOR *g_nor;
STATIC EFI_EVENT  mNorVirtualAddrChangeEvent;
/* Support single line case
 * - id: get from SPI Nor device information
 * - capacity: initial by SPI Nor id byte
 * - QE bits: no need
 * */
static struct FLASH_INFO s_commonSpiFlash = { 0, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x00, 0, 0, 0 };

/********************* Private Function Definition ***************************/
static RETURN_STATUS SNOR_ReadWriteReg(struct SPI_NOR *nor, struct HAL_SPI_MEM_OP *op, void *buf)
{
    if (op->data.dir == HAL_SPI_MEM_DATA_IN) {
        op->data.buf.in = buf;
    } else {
        op->data.buf.out = buf;
    }

    return HAL_FSPI_SpiXfer(nor->spi, op);
}

static RETURN_STATUS SNOR_ReadReg(struct SPI_NOR *nor, UINT8 code, UINT8 *val, UINT32 len)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(code, 1),
                                                     HAL_SPI_MEM_OP_NO_ADDR,
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_DATA_IN(len, NULL, 1));
    RETURN_STATUS ret;

    ret = SNOR_ReadWriteReg(nor, &op, val);
    if (ret) {
        DEBUG ((DEBUG_SNOR, "error %d reading %x\n", ret, code));
    }

    return ret;
}

static RETURN_STATUS SNOR_WriteReg(struct SPI_NOR *nor, UINT8 opcode, UINT8 *buf, UINT32 len)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(opcode, 1),
                                                     HAL_SPI_MEM_OP_NO_ADDR,
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_DATA_OUT(len, NULL, 1));

    /* DEBUG ((DEBUG_SNOR, "%s %x %ld\n", __func__, opcode, len)); */

    return SNOR_ReadWriteReg(nor, &op, buf);
}

static INT32 SNOR_ReadData(struct SPI_NOR *nor, UINT32 from, UINT32 len, UINT8 *buf)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(nor->readOpcode, 1),
                                                     HAL_SPI_MEM_OP_ADDR(nor->addrWidth, from, 1),
                                                     HAL_SPI_MEM_OP_DUMMY(nor->readDummy, 1),
                                                     HAL_SPI_MEM_OP_DATA_IN(len, buf, 1));
    INT32 ret;

    /* get transfer protocols. */
    op.cmd.buswidth = 1;
    op.addr.buswidth = SNOR_GET_PROTOCOL_ADDR_BITS(nor->readProto);
    op.dummy.buswidth = op.addr.buswidth;
    op.data.buswidth = SNOR_GET_PROTOCOL_DATA_BITS(nor->readProto);

    /* DEBUG ((DEBUG_SNOR, "%s %x %lx %lx %lx\n", __func__, nor->readDummy, op.data.nbytes, from, op.addr.val)); */
    /* convert the dummy cycles to the number of bytes */
    op.dummy.nbytes = (nor->readDummy * op.dummy.buswidth) >> 3;

    ret = HAL_FSPI_SpiXfer(nor->spi, &op);
    if (ret) {
        return 0;
    }

    return len;
}

static INT32 SNOR_WriteData(struct SPI_NOR *nor, UINT32 to, UINT32 len, const UINT8 *buf)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(nor->programOpcode, 1),
                                                     HAL_SPI_MEM_OP_ADDR(nor->addrWidth, to, 1),
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_DATA_OUT(len, buf, 1));
    INT32 ret;

    /* get transfer protocols. */
    op.cmd.buswidth = 1;
    op.addr.buswidth = SNOR_GET_PROTOCOL_ADDR_BITS(nor->writeProto);
    op.data.buswidth = SNOR_GET_PROTOCOL_DATA_BITS(nor->writeProto);

    op.data.nbytes = len < op.data.nbytes ? len : op.data.nbytes;

    ret = HAL_FSPI_SpiXfer(nor->spi, &op);
    if (ret) {
        return 0;
    }

    return op.data.nbytes;
}

/*
 * Initiate the erasure of a single sector
 */
static RETURN_STATUS SNOR_EraseSec(struct SPI_NOR *nor, UINT32 addr)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(nor->eraseOpcodeSec, 1),
                                                     HAL_SPI_MEM_OP_ADDR(nor->addrWidth, addr, 1),
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_NO_DATA);

    /*
     * Default implementation, if driver doesn't have a specialized HW
     * control
     */
    return HAL_FSPI_SpiXfer(nor->spi, &op);
}

/*
 * Initiate the erasure of a single sector
 */
static RETURN_STATUS SNOR_EraseBlk(struct SPI_NOR *nor, UINT32 addr)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(nor->eraseOpcodeBlk, 1),
                                                     HAL_SPI_MEM_OP_ADDR(nor->addrWidth, addr, 1),
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_NO_DATA);

    /*
     * Default implementation, if driver doesn't have a specialized HW
     * control
     */
    return HAL_FSPI_SpiXfer(nor->spi, &op);
}

static RETURN_STATUS SNOR_EraseChip(struct SPI_NOR *nor, UINT32 addr)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(SPINOR_OP_CHIP_ERASE, 1),
                                                     HAL_SPI_MEM_OP_NO_ADDR,
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_NO_DATA);
    /*
     * Default implementation, if driver doesn't have a specialized HW
     * control
     */
    return HAL_FSPI_SpiXfer(nor->spi, &op);
}

static const struct FLASH_INFO *SNOR_GerFlashInfo(UINT8 *flashId)
{
    UINT32 i;
    UINT32 id = (flashId[0] << 16) | (flashId[1] << 8) | (flashId[2] << 0);

    for (i = 0; i < ARRAY_SIZE(s_spiFlashbl); i++) {
        if (s_spiFlashbl[i].id == id) {
            return &s_spiFlashbl[i];
        }
    }

    return NULL;
}

static RETURN_STATUS SNOR_WriteEnable(struct SPI_NOR *nor)
{
    return SNOR_WriteReg(nor, SPINOR_OP_WREN, NULL, 0);
}

/*
 * Service routine to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static RETURN_STATUS SNOR_WaitBusy(struct SPI_NOR *nor, unsigned long timeout)
{
    RETURN_STATUS ret;
    UINT32 i;
    UINT8 status;

    /* DEBUG ((DEBUG_SNOR, "%s %lx\n", __func__, timeout)); */
    for (i = 0; i < timeout; i++) {
        ret = SNOR_ReadReg(nor, SPINOR_OP_RDSR, &status, 1);
        if (ret != RETURN_SUCCESS) {
            return ret;
        }

        if ((status & 0x01) == 0) {
            return RETURN_SUCCESS;
        }

        NanoSecondDelay(500);
    }
    DEBUG ((DEBUG_SNOR, "%s error %ld\n", __func__, timeout));

    return RETURN_NOT_READY;
}

static RETURN_STATUS SNOR_ReadStatus(struct SPI_NOR *nor, UINT32 regIndex, UINT8 *status)
{
    UINT8 readStatCmd[] = { SPINOR_OP_RDSR, SPINOR_OP_RDSR1, SPINOR_OP_RDSR2 };
    UINT8 i = nor->info->feature & FEA_STATUE_MASK;

    if (i == FEA_STATUE_MODE2) { /* Readstatus1 */
        readStatCmd[1] = SPINOR_OP_RDCR;
    }

    return SNOR_ReadReg(nor, readStatCmd[regIndex], status, 1);
}

static RETURN_STATUS SNOR_WriteStatus(struct SPI_NOR *nor, UINT32 regIndex, UINT8 *status)
{
    UINT8 WriteStatCmd[2] = { SPINOR_OP_WRSR, SPINOR_OP_WRSR1 };
    UINT8 i = nor->info->feature & FEA_STATUE_MASK;
    RETURN_STATUS ret;

    if (i == FEA_STATUE_MODE0) { /* Writestatus0 */
        SNOR_WriteEnable(nor);

        ret = SNOR_WriteReg(nor, WriteStatCmd[regIndex], status, 1);
        if (ret) {
            DEBUG ((DEBUG_SNOR, "error while writing configuration register\n"));

            return RETURN_DEVICE_ERROR;
        }

        ret = SNOR_WaitBusy(nor, 10000);
        if (ret) {
            DEBUG ((DEBUG_SNOR, "timeout while writing configuration register\n"));

            return ret;
        }
    } else { /* Writestatus1 */
        UINT8 readIndex;
        UINT8 status2[2];

        status2[regIndex] = *status;
        readIndex = (regIndex == 0) ? 1 : 0;
        ret = SNOR_ReadStatus(nor, readIndex, &status2[readIndex]);
        if (ret != RETURN_SUCCESS) {
            return ret;
        }

        SNOR_WriteEnable(nor);
        ret = SNOR_WriteReg(nor, SPINOR_OP_WRSR, &status2[0], 2);
        if (ret != RETURN_SUCCESS) {
            return ret;
        }

        ret = SNOR_WaitBusy(nor, 10000);
    }

    return RETURN_SUCCESS;
}

static RETURN_STATUS SNOR_EnableQE(struct SPI_NOR *nor)
{
    RETURN_STATUS ret = RETURN_SUCCESS;
    int regIndex;
    int bitOffset;
    UINT8 status;

    regIndex = nor->info->QEBits >> 3;
    bitOffset = nor->info->QEBits & 0x7;
    ret = SNOR_ReadStatus(nor, regIndex, &status);
    if (ret != RETURN_SUCCESS) {
        return ret;
    }

    if (status & (1 << bitOffset)) { //is QE bit set
        return RETURN_SUCCESS;
    }

    status |= (1 << bitOffset);
    ret = SNOR_WriteStatus(nor, regIndex, &status);

    return ret;
}

/* Enable/disable 4-byte addressing mode. */
static RETURN_STATUS SNOR_Enter4byte(struct SPI_NOR *nor)
{
    return SNOR_WriteReg(nor, SPINOR_OP_EN4B, NULL, 0);
}

RETURN_STATUS SNOR_ReadSFDP(struct SPI_NOR *nor, UINT32 addr, UINT8 *data)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(SPINOR_OP_READ_SFDP, 1),
                                                     HAL_SPI_MEM_OP_ADDR(3, addr, 1),
                                                     HAL_SPI_MEM_OP_DUMMY(1, 1),
                                                     HAL_SPI_MEM_OP_DATA_IN(1, data, 1));

    return HAL_FSPI_SpiXfer(nor->spi, &op);
}

static void *SNOR_InfoAdjust(struct SPI_NOR *nor, struct FLASH_INFO *info)
{
    UINT32 addr;
    UINT8 para;

    if (info->id == 0xc84019) {
        addr = 0x09;
        SNOR_ReadSFDP(nor, addr, &para);
        if (para == 0x06) {
            info->QEBits = 9;
            info->progCmd_4 = 0x34;
        }
    }

    return 0;
}

/**
 * @brief  Flash continuous writing.
 * @param  nor: nor dev.
 * @param  from: byte address.
 * @param  buf: source address.
 * @param  len: number of bytes.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
RETURN_STATUS HAL_SNOR_ReadData(struct SPI_NOR *nor, UINT32 from, void *buf, UINT32 len)
{
    RETURN_STATUS ret;
    UINT8 *pBuf = (UINT8 *)buf;
    UINT32 size, remain = len;

    /* DEBUG ((DEBUG_SNOR, "%s from 0x%08lx, len %lx\n", __func__, from, len)); */
    if (from >= nor->size || len > nor->size || (from + len) > nor->size) {
        return RETURN_DEVICE_ERROR;
    }

    while (remain) {
        size = MIN(READ_MAX_IOSIZE, remain);
        ret = SNOR_ReadData(nor, from, size, pBuf);
        if (ret != (RETURN_STATUS)size) {
            DEBUG ((DEBUG_SNOR, "%s %lu ret= %ld\n", __func__, from >> 9, ret));

            return ret;
        }
        remain -= size;
        from += size;
        pBuf += size;
    }

    return len;
}

/**
 * @brief  Flash continuous reading.
 * @param  nor: nor dev.
 * @param  to: byte address.
 * @param  buf: source address.
 * @param  len: number of bytes.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
RETURN_STATUS HAL_SNOR_ProgData(struct SPI_NOR *nor, UINT32 to, void *buf, UINT32 len)
{
    RETURN_STATUS ret;
    UINT8 *pBuf = (UINT8 *)buf;
    UINT32 size, remain = len, pageOffset;

    /* DEBUG ((DEBUG_SNOR, "%s to 0x%08lx, len %lx\n", __func__, to, len)); */
    if (to >= nor->size || len > nor->size || (to + len) > nor->size) {
        return RETURN_DEVICE_ERROR;
    }

    while (remain) {
        pageOffset = to & (nor->pageSize - 1);
        size = MIN(nor->pageSize - pageOffset, remain);
        SNOR_WriteEnable(nor);
        ret = SNOR_WriteData(nor, to, size, pBuf);
        if (ret != (RETURN_STATUS)size) {
            DEBUG ((DEBUG_SNOR, "%s %lu ret= %ld\n", __func__, to >> 9, ret));

            return ret;
        }
        SNOR_WaitBusy(nor, 10000);
        remain -= size;
        to += size;
        pBuf += size;
    }

    return len;
}

/**
 * @brief  Flash erase with erase type.
 * @param  nor: nor dev.
 * @param  addr: byte address.
 * @param  eraseType: erase type.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_SNOR_Erase(struct SPI_NOR *nor, UINT32 addr, NOR_ERASE_TYPE eraseType)
{
    RETURN_STATUS ret;
    INT32 timeout[] = { 400, 2000, 40000 };

    /* DEBUG ((DEBUG_SNOR, "%s addr %lx\n", __func__, addr)); */
    if (addr >= nor->size) {
        return RETURN_DEVICE_ERROR;
    }

    SNOR_WriteEnable(nor);
    if (eraseType == ERASE_SECTOR) {
        ret = SNOR_EraseSec(nor, addr);
    } else if (eraseType == ERASE_BLOCK64K) {
        ret = SNOR_EraseBlk(nor, addr);
    } else {
        ret = SNOR_EraseChip(nor, addr);
    }
    if (ret != RETURN_SUCCESS) {
        return ret;
    }

    return SNOR_WaitBusy(nor, timeout[eraseType] * 1000);
}

/**
 * @brief  Flash continuous reading according to sectors.
 * @param  nor: nor dev.
 * @param  sec: sector address.
 * @param  nSec: number of sectors.
 * @param  pData: destination address.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
RETURN_STATUS HAL_SNOR_Read(struct SPI_NOR *nor, UINT32 sec, UINT32 nSec, void *pData)
{
    RETURN_STATUS ret = RETURN_SUCCESS;

    /* DEBUG ((DEBUG_SNOR, "%s sec 0x%08lx, nSec %lx\n", __func__, sec, nSec)); */
    ret = HAL_SNOR_ReadData(nor, sec * nor->sectorSize, pData, nSec * nor->sectorSize);
    if (ret != (RETURN_STATUS)(nSec * nor->sectorSize)) {
        return ret;
    }

    return (RETURN_STATUS)nSec;
}

/**
 * @brief  Flash continuous writing according to sectors.
 * @param  nor: nor dev.
 * @param  sec: sector address.
 * @param  nSec: number of sectors.
 * @param  pData: source address.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
RETURN_STATUS HAL_SNOR_Write(struct SPI_NOR *nor, UINT32 sec, UINT32 nSec, void *pData)
{
    RETURN_STATUS ret = RETURN_SUCCESS;

    /* DEBUG ((DEBUG_SNOR, "%s sec 0x%08lx, nSec %lx\n", __func__, sec, nSec)); */
    ret = HAL_SNOR_ProgData(nor, sec * nor->sectorSize, pData, nSec * nor->sectorSize);
    if (ret != (RETURN_STATUS)(nSec * nor->sectorSize)) {
        return ret;
    }

    return (RETURN_STATUS)nSec;
}

/**
 * @brief  Flash continuous writing according to sectors.
 * @param  nor: nor dev.
 * @param  sec: sector address.
 * @param  nSec: number of sectors.
 * @param  pData: source address.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
RETURN_STATUS HAL_SNOR_OverWrite(struct SPI_NOR *nor, UINT32 sec, UINT32 nSec, void *pData)
{
    RETURN_STATUS ret = RETURN_SUCCESS;
    UINT8 *pBuf = (UINT8 *)pData;
    UINT32 remaining = nSec;

    /* DEBUG ((DEBUG_SNOR, "%s sec 0x%08lx, nSec %lx\n", __func__, sec, nSec)); */
    while (remaining) {
        ret = HAL_SNOR_Erase(nor, sec * nor->sectorSize, ERASE_SECTOR);
        if (ret != RETURN_SUCCESS) {
            return ret;
        }

        ret = HAL_SNOR_ProgData(nor, sec * nor->sectorSize, (void *)pBuf, nor->sectorSize);
        if (ret != (RETURN_STATUS)(nor->sectorSize)) {
            return ret;
        }

        pBuf += nor->sectorSize;
        remaining--;
        sec++;
    }

    return (RETURN_STATUS)nSec;
}

/** @} */

/** @defgroup SNOR_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  SFC NOR flash module init.
 * @param  nor: nor dev.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_SNOR_Init(struct SPI_NOR *nor)
{
    UINT8 idByte[5];
    const struct FLASH_INFO *info;
    INT32 ret = RETURN_SUCCESS;

    if (!nor->spi) {
        DEBUG ((DEBUG_SNOR, "%a no host\n", __func__));

        return RETURN_DEVICE_ERROR;
    }
    //nor->read = SNOR_ReadData;
    //nor->write = SNOR_WriteData;
    //nor->readReg = SNOR_ReadReg;
    //nor->writeReg = SNOR_WriteReg;

    HAL_SNOR_ReadID(nor, idByte);
    DEBUG ((DEBUG_SNOR, "SPI Nor ID: %x %x %x\n", idByte[0], idByte[1], idByte[2]));

    if ((idByte[0] == 0xFF) || (idByte[0] == 0x00)) {
        return RETURN_DEVICE_ERROR;
    }

    info = SNOR_GerFlashInfo(idByte);
    if (!info) {
        if (nor->spi->mode & HAL_SPI_RX_QUAD ||
            nor->spi->mode & HAL_SPI_TX_QUAD) {
            return RETURN_NO_MEDIA;
        }
        s_commonSpiFlash.id = (idByte[0] << 16) | (idByte[1] << 8) | idByte[2];
        s_commonSpiFlash.density = idByte[2] - 9;
        info = &s_commonSpiFlash;
    } else {
        SNOR_InfoAdjust(nor, (struct FLASH_INFO *)info);
    }

    nor->info = info;
    nor->pageSize = 256;
    nor->addrWidth = 3;
    nor->eraseOpcodeSec = info->sectorEraseCmd;
    nor->eraseOpcodeBlk = info->blockEraseCmd;
    nor->readOpcode = info->readCmd;
    nor->readProto = SNOR_PROTO_1_1_1;
    nor->readDummy = 0;
    nor->programOpcode = info->progCmd;
    nor->writeProto = SNOR_PROTO_1_1_1;
    //nor->name = "spi-nor";
    nor->sectorSize = info->sectorSize * 512;
    nor->size = 1 << (info->density + 9);
    nor->eraseSize = nor->sectorSize;
    if (nor->spi->mode & HAL_SPI_RX_QUAD) {
        ret = RETURN_SUCCESS;
        if (info->QEBits) {
            ret = SNOR_EnableQE(nor);
        }
        if (ret == RETURN_SUCCESS) {
            nor->readOpcode = info->readCmd_4;
            switch (nor->readOpcode) {
            case SPINOR_OP_READ_1_4_4:
                nor->readDummy = 6;
                nor->readProto = SNOR_PROTO_1_4_4;
                break;
            default:
                nor->readDummy = 8;
                nor->readProto = SNOR_PROTO_1_1_4;
                break;
            }
        }
    } else if (nor->spi->mode & HAL_SPI_RX_DUAL) {
        nor->readOpcode = SPINOR_OP_READ_1_1_2;
        nor->readDummy = 8;
        nor->readProto = SNOR_PROTO_1_1_2;
    }
    if (nor->spi->mode & HAL_SPI_TX_QUAD &&
        info->QEBits) {
        if (SNOR_EnableQE(nor) == RETURN_SUCCESS) {
            nor->programOpcode = info->progCmd_4;
            switch (nor->programOpcode) {
            case SPINOR_OP_PP_1_4_4:
                nor->writeProto = SNOR_PROTO_1_4_4;
                break;
            default:
                nor->writeProto = SNOR_PROTO_1_1_4;
                break;
            }
        }
    }
    if (info->feature & FEA_4BYTE_ADDR) {
        nor->addrWidth = 4;
    }

    if (info->feature & FEA_4BYTE_ADDR_MODE) {
        SNOR_Enter4byte(nor);
    }

    DEBUG ((DEBUG_SNOR, "nor->addrWidth: %x\n", nor->addrWidth));
    DEBUG ((DEBUG_SNOR, "nor->readProto: %x\n", nor->readProto));
    DEBUG ((DEBUG_SNOR, "nor->writeProto: %x\n", nor->writeProto));
    DEBUG ((DEBUG_SNOR, "nor->readCmd: %x\n", nor->readOpcode));
    DEBUG ((DEBUG_SNOR, "nor->programCmd: %x\n", nor->programOpcode));
    DEBUG ((DEBUG_SNOR, "nor->eraseOpcodeBlk: %x\n", nor->eraseOpcodeBlk));
    DEBUG ((DEBUG_SNOR, "nor->eraseOpcodeSec: %x\n", nor->eraseOpcodeSec));
    DEBUG ((DEBUG_SNOR, "nor->size: %ldMB\n", nor->size >> 20));
    DEBUG ((DEBUG_SNOR, "nor->size: %ldMB\n", nor->size >> 20));

    return RETURN_SUCCESS;
}

/**
 * @brief  SFC NOR flash module deinit.
 * @param  nor: nor dev.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_SNOR_DeInit(struct SPI_NOR *nor)
{
    return RETURN_SUCCESS;
}

/** @} */

/** @defgroup SNOR_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Read flash ID.
 * @param  nor: nor dev.
 * @param  data: address to storage flash ID value.
 * @return RETURN_STATUS.
 */
RETURN_STATUS HAL_SNOR_ReadID(struct SPI_NOR *nor, UINT8 *data)
{
    INT32 ret;
    UINT8 *id = data;

    ret = SNOR_ReadReg(nor, SPINOR_OP_RDID, id, 3);
    if (ret) {
        DEBUG ((DEBUG_SNOR, "error reading JEDEC ID%x %x %x\n", id[0], id[1], id[2]));

        return RETURN_DEVICE_ERROR;
    }

    return RETURN_SUCCESS;
}

/**
 * @brief  Get flash capacity.
 * @param  nor: nor dev.
 * @return UINT32: flash capacity, n bytes.
 */
UINT32 HAL_SNOR_GetCapacity(struct SPI_NOR *nor)
{
    return nor->size;
}

/**
 * @brief  Check if the flash support.
 * @param  flashId: flash id.
 * @return HAL_Check.
 */
BOOLEAN HAL_SNOR_IsFlashSupported(UINT8 *flashId)
{
    UINT32 i;
    UINT32 id = (flashId[0] << 16) | (flashId[1] << 8) | (flashId[2] << 0);

    for (i = 0; i < ARRAY_SIZE(s_spiFlashbl); i++) {
        if (s_spiFlashbl[i].id == id) {
            return TRUE;
        }
    }

    return FALSE;
}

RETURN_STATUS HAL_SNOR_ReadUUID(struct SPI_NOR *nor, void *buf)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(SPINOR_OP_READ_UUID, 1),
                                                     HAL_SPI_MEM_OP_ADDR(4, 0, 1),
                                                     HAL_SPI_MEM_OP_DUMMY(0, 1),
                                                     HAL_SPI_MEM_OP_DATA_IN(8, buf, 1));

    return HAL_FSPI_SpiXfer(nor->spi, &op);
}

EFI_STATUS Erase(
   IN UNI_NOR_FLASH_PROTOCOL   *This,
   IN  UINT32                   Offset,
   IN  UINT32                   ulLen
  )
{
  EFI_STATUS Status;
  UINTN EraseSize;

  if (EfiAtRuntime ())
    NorFspiEnableClock(g_nor->spi->CruBase);

  EraseSize = g_nor->sectorSize;

  // Check input parameters
  if (Offset % EraseSize || ulLen % EraseSize) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Either erase offset or length is not multiple of erase size\n"));
    return EFI_DEVICE_ERROR;
  }

  while (ulLen) {
    Status = HAL_SNOR_Erase(g_nor, Offset, ERASE_SECTOR);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Error while erase target address\n"));
      return Status;
    }
    Offset += EraseSize;
    ulLen -= EraseSize;
  }
  return EFI_SUCCESS;
}

UINT32 GetSize(
    IN UNI_NOR_FLASH_PROTOCOL   *This
    )
{
  return HAL_SNOR_GetCapacity(g_nor);
}

EFI_STATUS  Write(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN  UINT32                  Offset,
  IN  UINT8                  *Buffer,
  UINT32                     ulLen
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (EfiAtRuntime ())
    NorFspiEnableClock(g_nor->spi->CruBase);

  //DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: %x!......................\n", __FUNCTION__,__LINE__,Offset));
  Status = HAL_SNOR_ProgData(g_nor, Offset, Buffer, ulLen);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Error while programming target address\n"));
  }
  return Status;
}

EFI_STATUS Read(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN UINT32                   Offset,
  IN OUT UINT8               *Buffer,
  IN UINT32                   ulLen
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (EfiAtRuntime ())
    NorFspiEnableClock(g_nor->spi->CruBase);

  //DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: %x!......................\n", __FUNCTION__,__LINE__,Offset));
  Status = HAL_SNOR_ReadData(g_nor, Offset, Buffer, ulLen);
  return Status;
}

STATIC
EFI_STATUS
SpiFlashUpdateBlock (
  IN UINT32 Offset,
  IN UINT32 Align,
  IN UINTN ToUpdate,
  IN UINT8 *Buf,
  IN UINT8 *TmpBuf,
  IN UINTN EraseSize
  )
{

  EFI_STATUS Status;

  if (EfiAtRuntime ())
    NorFspiEnableClock(g_nor->spi->CruBase);

  // Read backup
  if (ToUpdate != EraseSize) {
    Status = HAL_SNOR_ReadData (g_nor, Offset - Align, TmpBuf, EraseSize);
      if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while reading old data\n"));
        return Status;
      }
  }
  // Erase entire sector
  Status = HAL_SNOR_Erase (g_nor, Offset, ERASE_SECTOR);
  if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while erasing block\n"));
      return Status;
    }

  if (Align) {
    memcpy(&TmpBuf[Align], Buf, ToUpdate);
    Status = HAL_SNOR_ProgData (g_nor, Offset - Align, TmpBuf, EraseSize);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while writing new data\n"));
      return Status;
    }
    return EFI_SUCCESS;
  }

  // Write new data
  Status = HAL_SNOR_ProgData (g_nor, Offset, Buf, ToUpdate);
  if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while writing new data\n"));
      return Status;
    }

  // Write backup
  if (ToUpdate != EraseSize) {
    Status = HAL_SNOR_ProgData (g_nor, Offset + ToUpdate, &TmpBuf[ToUpdate], EraseSize - ToUpdate);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while writing backup\n"));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS  Update(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN  UINT32                  Offset,
  IN  UINT8                  *Buffer,
  UINT32                     ulLength
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 SectorSize, ToUpdate, Align, Scale = 1;
  UINT8 *TmpBuf, *End;
  //DEBUG ((EFI_D_ERROR, "[%a]:%x %x!......................\n", __FUNCTION__, Offset, ulLength));

  SectorSize = g_nor->sectorSize;
  Align = Offset & (SectorSize -1);
  End = Buffer + ulLength;

  TmpBuf = (UINT8 *)AllocateZeroPool (SectorSize);
  if (TmpBuf == NULL) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Cannot allocate memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  if (End - Buffer >= 200)
    Scale = (End - Buffer) / 100;

  for (; Buffer < End; Buffer += ToUpdate, Offset += ToUpdate) {
    ToUpdate = MIN((UINT64)(End - Buffer), SectorSize);
    Print (L"   \rUpdating, %d%%", 100 - (End - Buffer) / Scale);
    Status = SpiFlashUpdateBlock (Offset, Align, ToUpdate, Buffer, TmpBuf, SectorSize);

    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Error while updating\n"));
      break;
    }
  }

  Print(L"\n");
  FreePool (TmpBuf);

  return Status;
}

UNI_NOR_FLASH_PROTOCOL gUniNorFlash = {
    GetSize,
    Erase,
    Write,
    Read,
    Update
};

#if 0
#define maxest_sector 4
static UINT32 pread32[maxest_sector * 4096 + 64];
static UINT32 pwrite32[maxest_sector * 4096 + 64];
#define FLASH_SKIP_LBA 0x100 /* About 1M space skip */

static RETURN_STATUS SNOR_STRESS_RANDOM_TEST(UINT32 testEndLBA)
{
    UINT32 ret, j;
    UINT32 testCount, testLBA = 0;
    UINT32 testSecCount = 1;

    DEBUG ((EFI_D_ERROR, "---------%a %lx---------\n", __func__, testEndLBA));
    DEBUG ((EFI_D_ERROR, "---------%a---------\n", __func__));
    for (j = 0; j < testSecCount * (UINT32)g_nor->sectorSize / 4; j++)
      pwrite32[j] = j + (0xFFFF0000 - j);

    for (testCount = 0; testCount < testEndLBA;) {
        testLBA = testCount;
        pwrite32[0] = testLBA;
        ret = HAL_SNOR_OverWrite(g_nor, testLBA, testSecCount, pwrite32);
        if (ret != testSecCount) {
            return RETURN_DEVICE_ERROR;
        }
        pread32[0] = -1;
        ret = HAL_SNOR_Read(g_nor, testLBA, testSecCount, pread32);
        if (ret != testSecCount) {
            return RETURN_DEVICE_ERROR;
        }
        for (j = 0; j < testSecCount * (UINT32)g_nor->sectorSize / 4; j++) {
            if (pwrite32[j] != pread32[j]) {
                DEBUG ((EFI_D_ERROR, 
                    "check not match:row=%lx, num=%lx, write=%lx, read=%lx %lx %lx %lx\n",
                    testLBA, j, pwrite32[j], pread32[j], pread32[j + 1], pread32[j + 2], pread32[j - 1]));
                while (1) {
                    ;
                }
            }
        }
        DEBUG ((EFI_D_ERROR, "testCount= %lx testLBA= %lx\n", testCount, testLBA));
        testCount += testSecCount;
    }
    DEBUG ((EFI_D_ERROR, "---------%a SUCCESS---------\n", __func__));

    return RETURN_SUCCESS;
}
#endif

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
STATIC
VOID
EFIAPI
NorVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  // Convert SPI device description
  EfiConvertPointer (0, (VOID**)&g_nor->spi->instance);
  EfiConvertPointer (0, (VOID**)&g_nor->spi->CruBase);
  EfiConvertPointer (0, (VOID**)&g_nor->spi);
  EfiConvertPointer (0, (VOID**)&g_nor->info);
  EfiConvertPointer (0, (VOID**)&g_nor);

  return;
}


EFI_STATUS
EFIAPI InitializeFlash (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable)
{
  EFI_STATUS Status = RETURN_SUCCESS;

  g_spi = AllocateRuntimeZeroPool (sizeof (struct HAL_FSPI_HOST));
  g_nor = AllocateRuntimeZeroPool (sizeof (struct SPI_NOR));
  if (g_spi == NULL || g_nor == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  g_spi->instance = (struct FSPI_REG *)FixedPcdGet32(FspiBaseAddr);
  g_spi->CruBase = (UINT32 *)FixedPcdGet32(CruBaseAddr);

  NorFspiIomux();
  HAL_FSPI_Init(g_spi);
  g_nor->spi = g_spi;
  g_nor->spi->mode = HAL_SPI_MODE_3;
  g_nor->spi->mode |= (HAL_SPI_TX_QUAD | HAL_SPI_RX_QUAD);
  Status = HAL_SNOR_Init(g_nor);

  Status = gBS->InstallProtocolInterface (
                            &ImageHandle,
                            &gUniNorFlashProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &gUniNorFlash);
  if(EFI_SUCCESS != Status)
  {
    DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Install Protocol Interface %r!\n", __FUNCTION__,__LINE__,Status));
  }

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  NorVirtualNotifyEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mNorVirtualAddrChangeEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to register VA change event\n", __FUNCTION__));
    goto ErrorSetMemAttr;
  }

  return Status;
ErrorSetMemAttr:
  gBS->UninstallProtocolInterface (gImageHandle,
         &gUniNorFlashProtocolGuid,
         NULL);

  FreePool (g_nor);
  FreePool (g_spi);

  return Status;
  
}
