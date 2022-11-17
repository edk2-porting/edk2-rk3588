/** @file
  Header file for PCH Serial IO UART Lib implementation.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SERIAL_IO_UART_LIB_H_
#define _PCH_SERIAL_IO_UART_LIB_H_

typedef enum {
  AccessMode8bit,
  AccessMode32bit
} UART_ACCESS_MODE;

/**
  Returns UART's currently active access mode, 8 or 32 bit

  @param[in]  MmioBase    Base address of UART MMIO space

  @retval     AccessMode8bit
  @retval     AccessMode32bit
**/
UART_ACCESS_MODE
DetectAccessMode (
  IN UINTN  MmioBase
  );

/**
  Initialize selected SerialIo UART.

  @param[in]  UartNumber           Selects Serial IO UART device (0-2)
  @param[in]  FifoEnable           When TRUE, enables 64-byte FIFOs.
  @param[in]  BaudRate             Baud rate.
  @param[in]  LineControl          Data length, parity, stop bits.
  @param[in]  HardwareFlowControl  Automated hardware flow control. If TRUE, hardware automatically checks CTS when sending data, and sets RTS when receiving data.
**/
VOID
EFIAPI
PchSerialIoUartInit (
  IN UINT8   UartNumber,
  IN BOOLEAN FifoEnable,
  IN UINT32  BaudRate,
  IN UINT8   LineControl,
  IN BOOLEAN HardwareFlowControl
  );


/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  UartNumber       Selects Serial IO UART device (0-2)
  @param[in]  Buffer           Point of data buffer which need to be writed.
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                  Actual number of bytes writed to serial device.
**/
UINTN
EFIAPI
PchSerialIoUartOut (
  IN UINT8  UartNumber,
  IN UINT8  *Buffer,
  IN UINTN  NumberOfBytes
);

/*
  Read data from serial device and save the datas in buffer.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]   UartNumber           Selects Serial IO UART device (0-2)
  @param[out]  Buffer               Point of data buffer which need to be writed.
  @param[in]   NumberOfBytes        Number of output bytes which are cached in Buffer.
  @param[in]   WaitUntilBufferFull  When TRUE, function waits until whole buffer is filled. When FALSE, function returns as soon as no new characters are available.

  @retval                      Actual number of bytes raed from serial device.

**/
UINTN
EFIAPI
PchSerialIoUartIn (
  IN  UINT8     UartNumber,
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes,
  IN  BOOLEAN   WaitUntilBufferFull
);

/**
  Polls a serial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @param[in]  UartNumber       Selects Serial IO UART device (0-2)

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.

**/
BOOLEAN
EFIAPI
PchSerialIoUartPoll (
  IN  UINT8     UartNumber
  );


#endif // _PEI_DXE_SMM_PCH_SERIAL_IO_UART_LIB_H_
