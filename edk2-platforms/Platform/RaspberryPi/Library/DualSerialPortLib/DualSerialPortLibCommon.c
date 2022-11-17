/** @file
  16550 and PL011 Serial Port library functions for Raspberry Pi

  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
  Copyright (c) 2018, AMD Incorporated. All rights reserved.<BR>
  Copyright (c) 2014, Hewlett-Packard Development Company, L.P.<BR>
  Copyright (c) 2012 - 2016, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PL011UartLib.h>
#include <Library/SerialPortLib.h>

#include "DualSerialPortLib.h"

BOOLEAN UsePl011Uart          = FALSE;
BOOLEAN UsePl011UartSet       = FALSE;

/**
  Read an 8-bit 16550 register.

  @param  Base    The base address register of UART device.
  @param  Offset  The offset of the 16550 register to read.

  @return The value read from the 16550 register.

**/
UINT8
SerialPortReadRegister (
  UINTN  Base,
  UINTN  Offset
  )
{
  return MmioRead8 (Base + Offset * PcdGet32 (PcdSerialRegisterStride));
}

/**
  Write an 8-bit 16550 register.

  @param  Base    The base address register of UART device.
  @param  Offset  The offset of the 16550 register to write.
  @param  Value   The value to write to the 16550 register specified by Offset.

  @return The value written to the 16550 register.

**/
UINT8
SerialPortWriteRegister (
  UINTN  Base,
  UINTN  Offset,
  UINT8  Value
  )
{
  return MmioWrite8 (Base + Offset * PcdGet32 (PcdSerialRegisterStride), Value);
}

/**
  Return whether the hardware flow control signal allows writing.

  @param  SerialRegisterBase The base address register of UART device.

  @retval TRUE  The serial port is writable.
  @retval FALSE The serial port is not writable.
**/
STATIC
BOOLEAN
SerialPortWritable (
  UINTN  SerialRegisterBase
  )
{
  if (PcdGetBool (PcdSerialUseHardwareFlowControl)) {
    if (PcdGetBool (PcdSerialDetectCable)) {
      //
      // Wait for both DSR and CTS to be set
      //   DSR is set if a cable is connected.
      //   CTS is set if it is ok to transmit data
      //
      //   DSR  CTS  Description                               Action
      //   ===  ===  ========================================  ========
      //    0    0   No cable connected.                       Wait
      //    0    1   No cable connected.                       Wait
      //    1    0   Cable connected, but not clear to send.   Wait
      //    1    1   Cable connected, and clear to send.       Transmit
      //
      return (BOOLEAN) ((SerialPortReadRegister (SerialRegisterBase, R_UART_MSR) & (B_UART_MSR_DSR | B_UART_MSR_CTS)) == (B_UART_MSR_DSR | B_UART_MSR_CTS));
    } else {
      //
      // Wait for both DSR and CTS to be set OR for DSR to be clear.
      //   DSR is set if a cable is connected.
      //   CTS is set if it is ok to transmit data
      //
      //   DSR  CTS  Description                               Action
      //   ===  ===  ========================================  ========
      //    0    0   No cable connected.                       Transmit
      //    0    1   No cable connected.                       Transmit
      //    1    0   Cable connected, but not clear to send.   Wait
      //    1    1   Cable connected, and clar to send.        Transmit
      //
      return (BOOLEAN) ((SerialPortReadRegister (SerialRegisterBase, R_UART_MSR) & (B_UART_MSR_DSR | B_UART_MSR_CTS)) != (B_UART_MSR_DSR));
    }
  }

  return TRUE;
}

/**
  Write data from buffer to serial device.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.

  If Buffer is NULL, then ASSERT().

  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the write operation failed.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  UINTN  SerialRegisterBase;
  UINTN  Result;
  UINTN  Index;
  UINTN  FifoSize;

  //
  // Serial writes may happen *before* the UART has been initialized
  // and if we use the wrong UART then, all kind of bad things happen.
  // To alleviate this, we add UART detection in SerialPortWrite and
  // guard the UART detection with a second boolean.
  //
  if (!UsePl011UartSet) {
    UsePl011Uart = ((MmioRead32(GPIO_BASE_ADDRESS + 4) & 0x0003F000) == 0x00024000);
    UsePl011UartSet = TRUE;
  }

  if (UsePl011Uart) {
    return PL011UartWrite (PL011_UART_REGISTER_BASE, Buffer, NumberOfBytes);
  } else {
    if (Buffer == NULL) {
      return 0;
    }

    SerialRegisterBase = MINI_UART_REGISTER_BASE;

    if (NumberOfBytes == 0) {
      //
      // Flush the hardware
      //

      //
      // Wait for both the transmit FIFO and shift register empty.
      //
      while ((SerialPortReadRegister (SerialRegisterBase, R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY));

      //
      // Wait for the hardware flow control signal
      //
      while (!SerialPortWritable (SerialRegisterBase));
      return 0;
    }

    //
    // Compute the maximum size of the Tx FIFO
    //
    FifoSize = 1;
    if ((PcdGet8 (PcdSerialFifoControl) & B_UART_FCR_FIFOE) != 0) {
      if ((PcdGet8 (PcdSerialFifoControl) & B_UART_FCR_FIFO64) == 0) {
        FifoSize = 16;
      } else {
        FifoSize = PcdGet32 (PcdSerialExtendedTxFifoSize);
      }
    }

    Result = NumberOfBytes;
    while (NumberOfBytes != 0) {
      //
      // Wait for the serial port to be ready, to make sure both the transmit FIFO
      // and shift register empty.
      //
      while ((SerialPortReadRegister (SerialRegisterBase, R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY));

      //
      // Fill then entire Tx FIFO
      //
      for (Index = 0; Index < FifoSize && NumberOfBytes != 0; Index++, NumberOfBytes--, Buffer++) {
        //
        // Wait for the hardware flow control signal
        //
        while (!SerialPortWritable (SerialRegisterBase));

        //
        // Write byte to the transmit buffer.
        //
        SerialPortWriteRegister (SerialRegisterBase, R_UART_TXBUF, *Buffer);
      }
    }
    return Result;
  }
}
