/** @file
  UART Serial Port library functions

  Copyright (c) 2006 - 2009, Intel Corporation
  Copyright (c) 2015 - 2016, Hisilicon Limited. All rights reserved.
  Copyright (c) 2015 - 2016, Linaro Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/Library/PL011SerialPortLib/
**/
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/IoLib.h>
#include <Protocol/SerialIo.h>

#include "Dw8250SerialPortLib.h"

/**
  Initialize the serial device hardware.

  If no initialization is required, then return RETURN_SUCCESS.
  If the serial device was successfuly initialized, then return RETURN_SUCCESS.
  If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serail device could not be initialized.

**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  UINT32  ulUartClkFreq;

  MmioWrite8 (UART_LCR_REG, UART_LCR_DLS8);
  MmioWrite8 (UART_FCR_REG, UART_FCR_EN | UART_FCR_RXCLR | UART_FCR_TXCLR);
  MmioWrite8 (UART_LCR_REG, UART_LCR_DLAB | UART_LCR_DLS8);

  ulUartClkFreq = PcdGet32 (PcdUartClkInHz);

  MmioWrite8 (UART_DLL_REG, (ulUartClkFreq / (16 * (UINT32)BAUDRATE)) & 0xff);
  MmioWrite8 (UART_DLH_REG, ((ulUartClkFreq/ (16 * (UINT32)BAUDRATE)) >> 8) & 0xff);
  MmioWrite8 (UART_LCR_REG, UART_LCR_DLS8);
  MmioWrite8 (UART_IEL_REG, 0x00);

  return RETURN_SUCCESS;
}
