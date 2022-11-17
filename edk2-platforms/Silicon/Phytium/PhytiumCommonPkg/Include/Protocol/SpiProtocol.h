/** @file
  The Header of Protocol For SPI.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef SPI_H_
#define SPI_H_

extern EFI_GUID gSpiMasterProtocolGuid;
typedef struct _EFI_SPI_DRV_PROTOCOL EFI_SPI_DRV_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *SPI_DRV_INIT_INTERFACE)  (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *SPI_DRV_SET_CONFIG_INTERFACE)(
  IN  UINT8  CmdId,
  IN  UINT32 Config,
  IN  UINTN  RegAddr
  );

typedef
EFI_STATUS
(EFIAPI *SPI_DRV_GET_CONFIG_INTERFACE)(
  IN  UINT8  CmdId,
  OUT UINT32 *Config,
  IN  UINTN  RegAddr
  );

typedef
EFI_STATUS
(EFIAPI *SPI_DRV_CONFIG_MODE_INTERFACE)(
  IN  UINT32 Config
  );

struct _EFI_SPI_DRV_PROTOCOL{
  SPI_DRV_INIT_INTERFACE        SpiInit;
  SPI_DRV_SET_CONFIG_INTERFACE  SpiSetConfig;
  SPI_DRV_GET_CONFIG_INTERFACE  SpiGetConfig;
  SPI_DRV_CONFIG_MODE_INTERFACE SpiSetMode;
};

#endif // SPI_H_
