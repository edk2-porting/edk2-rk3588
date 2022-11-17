/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __MV_SPI_FLASH_H__
#define __MV_SPI_FLASH_H__

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/Spi.h>
#include <Protocol/SpiFlash.h>

#define SPI_FLASH_SIGNATURE             SIGNATURE_32 ('F', 'S', 'P', 'I')

#define CMD_READ_ID                     0x9f
#define READ_STATUS_REG_CMD             0x0b
#define CMD_WRITE_ENABLE                0x06
#define CMD_READ_STATUS                 0x05
#define CMD_FLAG_STATUS                 0x70
#define CMD_WRITE_STATUS_REG            0x01
#define CMD_READ_ARRAY_FAST             0x0b
#define CMD_PAGE_PROGRAM                0x02
#define CMD_BANK_WRITE                  0xc5
#define CMD_BANKADDR_BRWR               0x17
#define CMD_ERASE_4K                    0x20
#define CMD_ERASE_32K                   0x52
#define CMD_ERASE_64K                   0xd8
#define CMD_4B_ADDR_ENABLE              0xb7

#define SPI_CMD_LEN                     1

#define STATUS_REG_POLL_WIP             (1 << 0)
#define STATUS_REG_POLL_PEC             (1 << 7)

#define SPI_TRANSFER_BEGIN              0x01  // Assert CS before transfer
#define SPI_TRANSFER_END                0x02  // Deassert CS after transfers

#define SPI_FLASH_16MB_BOUN             0x1000000

typedef enum {
  SPI_FLASH_READ_ID,
  SPI_FLASH_READ, // Read from SPI flash with address
  SPI_FLASH_WRITE, // Write to SPI flash with address
  SPI_FLASH_ERASE,
  SPI_FLASH_UPDATE,
  SPI_COMMAND_MAX
} SPI_COMMAND;

typedef struct {
  MARVELL_SPI_FLASH_PROTOCOL  SpiFlashProtocol;
  UINTN                   Signature;
  EFI_HANDLE              Handle;
} SPI_FLASH_INSTANCE;

EFI_STATUS
EFIAPI
SpiFlashReadId (
  IN     SPI_DEVICE *SpiDev,
  IN     UINT32     DataByteCount,
  IN OUT UINT8      *Buffer
  );

EFI_STATUS
SpiFlashRead (
  IN SPI_DEVICE   *Slave,
  IN UINT32       Offset,
  IN UINTN        Length,
  IN VOID         *Buf
  );

EFI_STATUS
SpiFlashWrite (
  IN SPI_DEVICE *Slave,
  IN UINT32     Offset,
  IN UINTN      Length,
  IN VOID       *Buf
  );

EFI_STATUS
SpiFlashUpdate (
  IN SPI_DEVICE *Slave,
  IN UINT32 Offset,
  IN UINTN ByteCount,
  IN UINT8 *Buf
  );

EFI_STATUS
SpiFlashErase (
  IN SPI_DEVICE *SpiDev,
  IN UINTN      Offset,
  IN UINTN      Length
  );

EFI_STATUS
EFIAPI
EfiSpiFlashInit (
  IN MARVELL_SPI_FLASH_PROTOCOL *This,
  IN SPI_DEVICE *Slave
  );

#endif // __MV_SPI_FLASH_H__
