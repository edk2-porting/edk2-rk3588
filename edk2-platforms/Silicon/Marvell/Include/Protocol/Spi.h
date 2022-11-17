/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __MARVELL_SPI_MASTER_PROTOCOL_H__
#define __MARVELL_SPI_MASTER_PROTOCOL_H__

#include <Library/NorFlashInfoLib.h>

extern EFI_GUID gMarvellSpiMasterProtocolGuid;

typedef struct _MARVELL_SPI_MASTER_PROTOCOL MARVELL_SPI_MASTER_PROTOCOL;

typedef enum {
  SPI_MODE0, // CPOL = 0 & CPHA = 0
  SPI_MODE1, // CPOL = 0 & CPHA = 1
  SPI_MODE2, // CPOL = 1 & CPHA = 0
  SPI_MODE3  // CPOL = 1 & CPHA = 1
} SPI_MODE;

typedef struct {
  INTN Cs;
  INTN MaxFreq;
  SPI_MODE Mode;
  UINT32 AddrSize;
  NOR_FLASH_INFO *Info;
  UINTN HostRegisterBaseAddress;
  UINTN CoreClock;
} SPI_DEVICE;

typedef
EFI_STATUS
(EFIAPI *MV_SPI_INIT) (
  IN MARVELL_SPI_MASTER_PROTOCOL *This
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_TRANSFER) (
  IN MARVELL_SPI_MASTER_PROTOCOL *This,
  IN SPI_DEVICE *Slave,
  IN UINTN DataByteCount,
  IN VOID *DataOut,
  IN VOID *DataIn,
  IN UINTN Flag
  );

typedef
EFI_STATUS
(EFIAPI * MV_SPI_READ_WRITE) (
  IN  MARVELL_SPI_MASTER_PROTOCOL *This,
  IN  SPI_DEVICE *Slave,
  IN  UINT8 *Cmd,
  IN  UINTN CmdSize,
  IN  UINT8 *DataOut,
  OUT UINT8 *DataIn,
  IN  UINTN DataSize
  );

typedef
SPI_DEVICE *
(EFIAPI *MV_SPI_SETUP_DEVICE) (
  IN MARVELL_SPI_MASTER_PROTOCOL *This,
  IN SPI_DEVICE *Slave,
  IN UINTN    Cs,
  IN SPI_MODE Mode
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_FREE_DEVICE) (
  IN SPI_DEVICE *SpiDev
  );

typedef
EFI_STATUS
(EFIAPI *MV_SPI_CONFIG_RT) (
  IN SPI_DEVICE *SpiDev
  );

struct _MARVELL_SPI_MASTER_PROTOCOL {
  MV_SPI_INIT         Init;
  MV_SPI_READ_WRITE   ReadWrite;
  MV_SPI_TRANSFER     Transfer;
  MV_SPI_SETUP_DEVICE SetupDevice;
  MV_SPI_FREE_DEVICE  FreeDevice;
  MV_SPI_CONFIG_RT    ConfigRuntime;
};

#endif // __MARVELL_SPI_MASTER_PROTOCOL_H__
