/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __MV_SPI_FLASH__
#define __MV_SPI_FLASH__

#include <Protocol/FirmwareManagement.h>
#include <Protocol/Spi.h>

extern EFI_GUID gMarvellSpiFlashProtocolGuid;

typedef struct _MARVELL_SPI_FLASH_PROTOCOL MARVELL_SPI_FLASH_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_INIT) (
  IN MARVELL_SPI_FLASH_PROTOCOL *This,
  IN SPI_DEVICE             *SpiDev
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_READ_ID) (
  IN     SPI_DEVICE *SpiDev,
  IN     BOOLEAN     UseInRuntime
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_READ) (
  IN SPI_DEVICE *SpiDev,
  IN UINT32     Address,
  IN UINTN      DataByteCount,
  IN VOID       *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_WRITE) (
  IN SPI_DEVICE *SpiDev,
  IN UINT32     Address,
  IN UINTN      DataByteCount,
  IN VOID       *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_ERASE) (
  IN SPI_DEVICE *SpiDev,
  IN UINTN      Address,
  IN UINTN      DataByteCount
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_UPDATE) (
  IN SPI_DEVICE *SpiDev,
  IN UINT32     Address,
  IN UINTN      DataByteCount,
  IN UINT8      *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FLASH_UPDATE_WITH_PROGRESS) (
  IN SPI_DEVICE                                    *Slave,
  IN UINT32                                         Offset,
  IN UINTN                                          ByteCount,
  IN UINT8                                         *Buffer,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

struct _MARVELL_SPI_FLASH_PROTOCOL {
  MV_SPI_FLASH_INIT    Init;
  MV_SPI_FLASH_READ_ID ReadId;
  MV_SPI_FLASH_READ    Read;
  MV_SPI_FLASH_WRITE   Write;
  MV_SPI_FLASH_ERASE   Erase;
  MV_SPI_FLASH_UPDATE  Update;
  MV_SPI_FLASH_UPDATE_WITH_PROGRESS  UpdateWithProgress;
};

#endif // __MV_SPI_FLASH__
