/** @file
  Phytium Spi Drivers Header

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef SPI_DXE_H_
#define SPI_DXE_H_

#include <Protocol/SpiProtocol.h>

#define SPI_MASTER_SIGNATURE         SIGNATURE_32 ('M', 'S', 'P', 'I')
#define REG_MODE_REG 0x02C

EFI_STATUS
EFIAPI
SpiMasterGetConfig (
  IN   UINT8    CmdId,
  OUT  UINT32   *Config,
  IN   UINTN    RegAddr
  );

EFI_STATUS
EFIAPI
SpiMasterSetConfig (
  IN  UINT8     CmdId,
  IN  UINT32    Config,
  IN  UINTN     RegAddr
  );

EFI_STATUS
EFIAPI
SpiMasterSetMode (
  IN  UINT32    Config
  );

EFI_STATUS
EFIAPI
SpiMasterInit (
  VOID
  );

typedef struct {
  EFI_SPI_DRV_PROTOCOL    SpiMasterProtocol;
  UINTN                   Signature;
  EFI_HANDLE              Handle;
} PHYT_SPI_MASTER;

EFI_STATUS
EFIAPI
SpiMasterDrvEntryPoint (
  IN  EFI_HANDLE          ImageHandle,
  IN  EFI_SYSTEM_TABLE    *SystemTable
  );

#endif // SPI_DXE_H_
