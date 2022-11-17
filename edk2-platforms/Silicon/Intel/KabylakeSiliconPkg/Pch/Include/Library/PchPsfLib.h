/** @file
  Header file for PchPsfLib.
  This is helper library of RC for PSF register programming.
  It's not expected to be used in platform code.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_PSF_LIB_H_
#define _PCH_PSF_LIB_H_

#include <Library/PchSerialIoLib.h>

/**
  This procedure will enable SerialIO device BAR1 at PSF level

  @param[in] SerialIoDevice  SERIAL IO device (I2C0-5, SPI0-1, UART0-2)

  @retval None
**/
VOID
PsfEnableSerialIoDeviceBar1 (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoDevice
  );

/**
  This procedure will disable SerialIO device BAR1 at PSF level

  @param[in] SerialIoDevice  SERIAL IO device (I2C0-5, SPI0-1, UART0-2)

  @retval None
**/
VOID
PsfDisableSerialIoDeviceBar1 (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoDevice
  );

/**
  This procedure will disable SerailIO device at PSF level

  @param[in] SerialIoDevice  SERIAL IO device (I2C0-5, SPI0-1, UART0-2)

  @retval None
**/
VOID
PsfDisableSerialIoDevice (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoDevice
  );

/**
  This procedure will hide SerialIo device PciCfgSpace at PSF level

  @param[in] SerialIoDevice  SERIAL IO device (I2C0-5, SPI0-1, UART0-2)

  @retval None
**/
VOID
PsfHideSerialIoDevice (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoDevice
  );

/**
  This procedure will reveal SerialIo device PciCfgSpace at PSF level

  @param[in] SerialIoDevice  SERIAL IO device (I2C0-5, SPI0-1, UART0-2)

  @retval None
**/
VOID
PsfRevealSerialIoDevice (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoDevice
  );

/**
  This procedure will set BARx value for TraceHub ACPI device at PSF level

  @param[in] BarNum          BAR Number (0:BAR0, 1:BAR1)
  @param[in] BarValue        32bit BAR value

  @retval None
**/
VOID
PsfSetTraceHubAcpiDeviceBarValue (
  IN UINT8   BarNum,
  IN UINT32  BarValue
  );

/**
  This procedure will enable MSE for TraceHub ACPI device at PSF level

  @param[in] None

  @retval None
**/
VOID
PsfEnableTraceHubAcpiDeviceMemorySpace (
  VOID
  );

/**
  Enable HECI device at PSF level

  @param[in] HeciDevice       HECIx Device (HECI1-3)

  @retval None
**/
VOID
PsfEnableHeciDevice (
  IN UINT8      HeciDevice
  );

/**
  Disable HECI device at PSF level

  @param[in] HeciDevice       HECIx Device (HECI1-3)

  @retval None
**/
VOID
PsfDisableHeciDevice (
  IN UINT8      HeciDevice
  );

/**
  Enable IDER device at PSF level

  @retval None
**/
VOID
PsfEnableIderDevice (
  VOID
  );

/**
  Disable IDER device at PSF level

  @retval None
**/
VOID
PsfDisableIderDevice (
  VOID
  );

/**
  Enable SOL device at PSF level

  @retval None
**/
VOID
PsfEnableSolDevice (
  VOID
  );

/**
  Disable SOL device at PSF level

  @retval None
**/
VOID
PsfDisableSolDevice (
  VOID
  );
#endif // _PCH_PSF_LIB_H_
