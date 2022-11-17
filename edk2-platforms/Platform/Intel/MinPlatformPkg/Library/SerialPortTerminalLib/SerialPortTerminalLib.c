/** @file
  Main file for NULL named library for Serial Port Terminal Redirection library.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SerialPortTerminalLib.h"

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_DEVICE_PATH mSerialDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) sizeof (VENDOR_DEVICE_PATH),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EDKII_SERIAL_PORT_LIB_VENDOR_GUID
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_UART_DP,
      {
        (UINT8) sizeof (UART_DEVICE_PATH),
        (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8)
      }
    },
    0,                  // Reserved
    0,                  // BaudRate
    0,                  // DataBits
    0,                  // Parity
    0                   // StopBits
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8),
      }
    },
    DEVICE_PATH_MESSAGING_PC_ANSI
  },
  gEndEntire
};

/**
  Updates the ConOut, ConIn, ErrOut variables with the serial terminal device path
  @param                        none
  @retval                       none
**/
VOID
AddSerialTerminal (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[AddSerialPortTerminal]\n"));

  //
  // Append Serial Terminal into "ConIn"
  //
  EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *) &mSerialDevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *) &mSerialDevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ErrOut, (EFI_DEVICE_PATH_PROTOCOL *) &mSerialDevicePath, NULL);
}


/**
  Constructor for the Serial Port Device controller library.

  @param ImageHandle    the image handle of the process
  @param SystemTable    the EFI System Table pointer

  @retval EFI_SUCCESS        the shell command handlers were installed sucessfully
  @retval EFI_UNSUPPORTED    the shell level required was not found.
**/
EFI_STATUS
EFIAPI
SerialPortTerminalLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  mSerialDevicePath.Uart.BaudRate = PcdGet64(PcdUartDefaultBaudRate);
  mSerialDevicePath.Uart.DataBits = PcdGet8(PcdUartDefaultDataBits);
  mSerialDevicePath.Uart.Parity   = PcdGet8(PcdUartDefaultParity);
  mSerialDevicePath.Uart.StopBits = PcdGet8(PcdUartDefaultStopBits);
  DEBUG ((DEBUG_INFO, "[SerialPortTerminalLibConstructor] [%d, %d, %d, %d]\n",
      mSerialDevicePath.Uart.BaudRate,
      mSerialDevicePath.Uart.DataBits,
      mSerialDevicePath.Uart.Parity,
      mSerialDevicePath.Uart.StopBits));

  AddSerialTerminal();

  return EFI_SUCCESS;
}
