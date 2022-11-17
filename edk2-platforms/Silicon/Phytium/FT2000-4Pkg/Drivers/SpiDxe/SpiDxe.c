/** @file
  Phytium Spi Master Drivers.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Uefi/UefiBaseType.h>
#include "SpiDxe.h"

PHYT_SPI_MASTER *pSpiMasterInstance;
static UINTN     mSpiControlBase;

/**
  This function inited a spi driver.

  @param  None.

  @retval None.

**/
EFI_STATUS
EFIAPI
SpiMasterInit (
  VOID
  )
{
  return EFI_SUCCESS;
}


/**
  This function seted config to spi registers.

  @param[in]  CmdId    The id of command.

  @param[in]  Config   The value to be seted.

  @param[in]  RegAddr  The address of spi registers.

  @retval EFI_SUCCESS  SpiMasterSetConfig() is executed successfully.

**/
EFI_STATUS
EFIAPI
SpiMasterSetConfig (
  IN UINT8  CmdId,
  IN UINT32 Config,
  IN UINTN  RegAddr
  )
{
  UINTN SpiAddr;
  UINT32 Value;

  SpiAddr = 0;
  Value   = 0;

  if (CmdId != 0) {
    Value = (CmdId << 24) | (Config & 0xffffff);
  } else {
    Value = Config;
  }

  SpiAddr = mSpiControlBase + RegAddr;
  MmioWrite32 (SpiAddr, Value);

  return EFI_SUCCESS;
}


/**
  This function geted config from spi registers.

  @param[in]  CmdId    The id of command.

  @param[out] Config   The pointer of the config.

  @param[in]  RegAddr  The address of spi registers.

  @retval EFI_SUCCESS  SpiMasterGetConfig() is executed successfully.

**/
EFI_STATUS
EFIAPI
SpiMasterGetConfig (
  IN  UINT8  CmdId,
  OUT UINT32 *Config,
  IN  UINTN  RegAddr
  )
{
  UINTN SpiAddr;
  UINT32 Value;

  SpiAddr = 0;
  Value   = 0;

  SpiAddr = mSpiControlBase + RegAddr;
  Value = MmioRead32 (SpiAddr);

  if (CmdId != 0) {
    *Config = Value & 0xffffff;
  } else {
    *Config = Value;
  }

  return EFI_SUCCESS;
}


/**
  This function seted spi mode.

  @param[in] Config    The value to seted.

  @retval EFI_SUCCESS  SpiMasterSetMode() is executed successfully.

**/
EFI_STATUS
EFIAPI
SpiMasterSetMode (
  IN UINT32 Config
  )
{

  SpiMasterSetConfig (0, Config, REG_MODE_REG);

  return EFI_SUCCESS;
}


/**
  This function inited the spi driver protocol.

  @param[in] SpiMasterProtocol    A pointer to the master protocol struct.

  @retval EFI_SUCCESS             SpiMasterInitProtocol() is executed successfully.

**/
STATIC
EFI_STATUS
EFIAPI
SpiMasterInitProtocol (
  IN EFI_SPI_DRV_PROTOCOL *SpiMasterProtocol
  )
{

  SpiMasterProtocol->SpiInit        = SpiMasterInit;
  SpiMasterProtocol->SpiSetConfig   = SpiMasterSetConfig;
  SpiMasterProtocol->SpiGetConfig   = SpiMasterGetConfig;
  SpiMasterProtocol->SpiSetMode     = SpiMasterSetMode;

  return EFI_SUCCESS;
}


/**
  This function is the entrypoint of the spi driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.

  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
SpiMasterDrvEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;

  pSpiMasterInstance = AllocateRuntimeZeroPool (sizeof (PHYT_SPI_MASTER));
  if (pSpiMasterInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mSpiControlBase = FixedPcdGet64 (PcdSpiControllerBase);

  SpiMasterInitProtocol (&pSpiMasterInstance->SpiMasterProtocol);

  pSpiMasterInstance->Signature = SPI_MASTER_SIGNATURE;

  Status = gBS->InstallMultipleProtocolInterfaces (
                &(pSpiMasterInstance->Handle),
                &gSpiMasterProtocolGuid,
                &(pSpiMasterInstance->SpiMasterProtocol),
                NULL
                );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
