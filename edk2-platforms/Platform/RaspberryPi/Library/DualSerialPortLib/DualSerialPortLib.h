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

#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/Bcm2836Gpio.h>

#define PL011_UART_REGISTER_BASE      BCM2836_PL011_UART_BASE_ADDRESS
#define MINI_UART_REGISTER_BASE       (BCM2836_MINI_UART_BASE_ADDRESS + 0x40)

//
// 16550 UART register offsets and bitfields
//
#define R_UART_RXBUF          0   // LCR_DLAB = 0
#define R_UART_TXBUF          0   // LCR_DLAB = 0
#define R_UART_BAUD_LOW       0   // LCR_DLAB = 1
#define R_UART_BAUD_HIGH      1   // LCR_DLAB = 1
#define R_UART_IER            1   // LCR_DLAB = 0
#define R_UART_FCR            2
#define   B_UART_FCR_FIFOE    BIT0
#define   B_UART_FCR_FIFO64   BIT5
#define R_UART_LCR            3
#define   B_UART_LCR_DLAB     BIT7
#define R_UART_MCR            4
#define   B_UART_MCR_DTRC     BIT0
#define   B_UART_MCR_RTS      BIT1
#define R_UART_LSR            5
#define   B_UART_LSR_RXRDY    BIT0
#define   B_UART_LSR_TXRDY    BIT5
#define   B_UART_LSR_TEMT     BIT6
#define R_UART_MSR            6
#define   B_UART_MSR_CTS      BIT4
#define   B_UART_MSR_DSR      BIT5
#define   B_UART_MSR_RI       BIT6
#define   B_UART_MSR_DCD      BIT7

extern BOOLEAN UsePl011Uart;
extern BOOLEAN UsePl011UartSet;

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
  );

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
  );
