/** @file
  PCH Serial IO UART Lib implementation.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchSerialIoUartLib.h>
#include <IndustryStandard/Pci30.h>

#define MAX_BAUD_RATE      115200

#define R_PCH_SERIAL_IO_8BIT_UART_RXBUF      0x00
#define R_PCH_SERIAL_IO_8BIT_UART_TXBUF      0x00
#define R_PCH_SERIAL_IO_8BIT_UART_BAUD_LOW   0x00
#define R_PCH_SERIAL_IO_8BIT_UART_BAUD_HIGH  0x01
#define R_PCH_SERIAL_IO_8BIT_UART_FCR        0x02
#define R_PCH_SERIAL_IO_8BIT_UART_IIR        0x02
#define R_PCH_SERIAL_IO_8BIT_UART_LCR        0x03
#define R_PCH_SERIAL_IO_8BIT_UART_MCR        0x04
#define R_PCH_SERIAL_IO_8BIT_UART_LSR        0x05
#define R_PCH_SERIAL_IO_8BIT_UART_USR        0x1F
#define R_PCH_SERIAL_IO_32BIT_UART_CTR       0xFC //Component Type Register contains identification code
#define UART_COMPONENT_IDENTIFICATION_CODE   0x44570110

#define B_PCH_SERIAL_IO_UART_IIR_FIFOSE   BIT7|BIT6
#define B_PCH_SERIAL_IO_UART_LSR_RXDA     BIT0
#define B_PCH_SERIAL_IO_UART_LSR_BI       BIT4
#define B_PCH_SERIAL_IO_UART_LSR_TXRDY    BIT5
#define B_PCH_SERIAL_IO_UART_LCR_DLAB     BIT7
#define B_PCH_SERIAL_IO_UART_FCR_FCR      BIT0
#define B_PCH_SERIAL_IO_UART_MCR_RTS      BIT1
#define B_PCH_SERIAL_IO_UART_MCR_AFCE     BIT5
#define B_PCH_SERIAL_IO_UART_USR_TFNF     BIT1

/**
  Returns UART's currently active access mode, 8 or 32 bit

  @param[in]  MmioBase    Base address of UART MMIO space

  @retval     AccessMode8bit
  @retval     AccessMode32bit
**/
UART_ACCESS_MODE
DetectAccessMode (
  IN UINTN  MmioBase
  )
{
  if (MmioRead32 (MmioBase + R_PCH_SERIAL_IO_32BIT_UART_CTR) == UART_COMPONENT_IDENTIFICATION_CODE) {
    return AccessMode32bit;
  } else {
    return AccessMode8bit;
  }
}


/**
  Register access helper. Depending on SerialIO UART mode,
  its registers are aligned to 1 or 4 bytes and have 8 or 32bit size

  @param[in]  AccessMode     Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  BaseAddress    Base address of UART MMIO space
  @param[in]  Offset         Register offset in 8bit mode
  @param[in]  Data           Data to be written
**/
STATIC
VOID
WriteRegister (
  IN UART_ACCESS_MODE AccessMode,
  IN UINTN            BaseAddress,
  IN UINTN            Offset,
  IN UINT8            Data
  )
{
  if (AccessMode == AccessMode32bit) {
    MmioWrite32 (BaseAddress + 4*Offset, Data);
  } else {
    MmioWrite8 (BaseAddress + Offset, Data);
  }
}

/**
  Register access helper. Depending on SerialIO UART mode,
  its registers are aligned to 1 or 4 bytes and have 8 or 32bit size

  @param[in]  AccessMode     Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  BaseAddress    Base address of UART MMIO space
  @param[in]  Offset         Register offset in 8bit mode
  @retval                    retrieved register value, always 8bit regardless of access mode
**/
STATIC
UINT8
ReadRegister (
  IN UART_ACCESS_MODE AccessMode,
  IN UINTN            BaseAddress,
  IN UINTN            Offset
  )
{
  if (AccessMode == AccessMode32bit) {
    return (UINT8) (0xFF & MmioRead32 (BaseAddress + 4*Offset));
  } else {
    return MmioRead8 (BaseAddress + Offset);
  }
}

/**
  SerialIo UART in PCI mode will become unavailable when PCI enumerator
  disables its memory space. This function re-enables it

  @param[in]  UartNumber           Selects Serial IO UART device (0-2)
**/
STATIC
VOID
EnablePciMse (
  IN UINT8  UartNumber
  )
{
  UINTN  CfgSpace;

  CfgSpace = FindSerialIoBar (UartNumber + PchSerialIoIndexUart0, 1);
  if (MmioRead16 (CfgSpace + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }
  if ((MmioRead16 (CfgSpace + PCI_COMMAND_OFFSET) & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
    if ((MmioRead32 (CfgSpace + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFF000) != 0x0 &&
        (MmioRead32 (CfgSpace + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFF000) != 0xFFFFF000 ) {
      MmioOr8 (CfgSpace + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
    }
  }
}

/**
  Initialize selected SerialIo UART.
  This init function MUST be used prior any SerialIo UART functions to init serial io controller if platform is going use serialio UART as debug output.

  @param  UartNumber           Selects Serial IO UART device (0-2)
  @param  FifoEnable           When TRUE, enables 64-byte FIFOs.
  @param  BaudRate             Baud rate.
  @param  LineControl          Data length, parity, stop bits.
  @param  HardwareFlowControl  Automated hardware flow control. If TRUE, hardware automatically checks CTS when sending data, and sets RTS when receiving data.
**/
VOID
EFIAPI
PchSerialIoUartInit (
  UINT8            UartNumber,
  BOOLEAN          FifoEnable,
  UINT32           BaudRate,
  UINT8            LineControl,
  BOOLEAN          HardwareFlowControl
  )
{
  UINTN            Base;
  UINTN            Divisor;
  UART_ACCESS_MODE AccessMode;

  Base = FindSerialIoBar (UartNumber + PchSerialIoIndexUart0, 0);
  if ((Base & 0xFFFFFF00) == 0x0 || (Base & 0xFFFFF000) == 0xFFFFF000) {
    //
    // Base is not programmed, skip it.
    //
    return;
  }
  EnablePciMse (UartNumber);
  AccessMode = DetectAccessMode (Base);

  Divisor = MAX_BAUD_RATE / BaudRate;

  //
  // Configure baud rate
  //
  WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_LCR, B_PCH_SERIAL_IO_UART_LCR_DLAB);
  WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_BAUD_HIGH, (UINT8) (Divisor >> 8));
  WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_BAUD_LOW, (UINT8) (Divisor & 0xff));
  //
  //  Configure Line control and switch back to bank 0
  //
  WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_LCR, LineControl & 0x1F);
  //
  // Enable and reset FIFOs
  //
  WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_FCR, FifoEnable?B_PCH_SERIAL_IO_UART_FCR_FCR:0 );
  //
  // Put Modem Control Register(MCR) into its reset state of 0x00.
  //
  WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_MCR, B_PCH_SERIAL_IO_UART_MCR_RTS | (HardwareFlowControl?B_PCH_SERIAL_IO_UART_MCR_AFCE:0));
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param  UartNumber       Selects Serial IO UART device (0-2)
  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                  Actual number of bytes writed to serial device.
**/
UINTN
EFIAPI
PchSerialIoUartOut (
  IN UINT8            UartNumber,
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  UINTN            BytesLeft;
  UINTN            Base;
  UART_ACCESS_MODE AccessMode;

  if (NULL == Buffer) {
    return 0;
  }

  Base = FindSerialIoBar (UartNumber + PchSerialIoIndexUart0, 0);
  //
  // Sanity checks to avoid infinite loop when trying to print through uninitialized UART
  //
  if ((Base & 0xFFFFFF00) == 0x0 || (Base & 0xFFFFF000) == 0xFFFFF000) {
    return 0;
  }
  EnablePciMse (UartNumber);
  AccessMode = DetectAccessMode (Base);

  if (ReadRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_USR) == 0xFF) {
    return 0;
  }

  BytesLeft = NumberOfBytes;

  while (BytesLeft != 0) {
    //
    // Write data while there's room in TXFIFO. If HW Flow Control was enabled, it happens automatically on hardware level.
    //
    if (ReadRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_USR) & B_PCH_SERIAL_IO_UART_USR_TFNF) {
      WriteRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_TXBUF, *Buffer);
      Buffer++;
      BytesLeft--;
    }
  }

  return NumberOfBytes;
}

/*
  Read data from serial device and save the datas in buffer.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param  UartNumber           Selects Serial IO UART device (0-2)
  @param  Buffer               Point of data buffer which need to be writed.
  @param  NumberOfBytes        Number of output bytes which are cached in Buffer.
  @param  WaitUntilBufferFull  When TRUE, function waits until whole buffer is filled. When FALSE, function returns as soon as no new characters are available.

  @retval                      Actual number of bytes raed to serial device.

**/
UINTN
EFIAPI
PchSerialIoUartIn (
  IN  UINT8            UartNumber,
  OUT UINT8            *Buffer,
  IN  UINTN            NumberOfBytes,
  IN  BOOLEAN          WaitUntilBufferFull
  )
{
  UINTN            BytesReceived;
  UINTN            Base;
  UART_ACCESS_MODE AccessMode;
  UINT8            Lsr;
  UINT8            Byte;

  if (NULL == Buffer) {
    return 0;
  }

  Base = FindSerialIoBar (UartNumber + PchSerialIoIndexUart0, 0);
  //
  // Sanity checks to avoid infinite loop when trying to print through uninitialized UART
  //
  if ((Base & 0xFFFFFF00) == 0x0 || (Base & 0xFFFFF000) == 0xFFFFF000) {
    return 0;
  }
  EnablePciMse (UartNumber);
  AccessMode = DetectAccessMode (Base);

  BytesReceived = 0;

  while (BytesReceived != NumberOfBytes) {
    //
    // Read the line status register
    //
    Lsr = ReadRegister(AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_LSR);

    //
    // If there is data in the RX buffer, read it.
    //
    if ((Lsr & B_PCH_SERIAL_IO_UART_LSR_RXDA) != 0) {
      Byte = ReadRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_RXBUF);
      //
      // Check if the break interrupt bit is set. If set, the byte read from the
      // RX buffer is invalid and should be ignored. If not set, copy the byte into
      // the receive buffer.
      //
      if ((Lsr & B_PCH_SERIAL_IO_UART_LSR_BI) == 0) {
        *Buffer = Byte;
        Buffer++;
        BytesReceived++;
      }
    } else {
      if (!WaitUntilBufferFull) {
        //
        // If there's no data and function shouldn't wait, exit early
        //
        return BytesReceived;
      }
    }
  }
  return BytesReceived;
}

/**
  Polls a serial device to see if there is any data waiting to be read.

  Polls a serial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @param  UartNumber       Selects Serial IO UART device (0-2)

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.

**/
BOOLEAN
EFIAPI
PchSerialIoUartPoll (
  IN  UINT8            UartNumber
  )
{
  UINTN Base;
  UART_ACCESS_MODE AccessMode;

  Base = FindSerialIoBar (UartNumber + PchSerialIoIndexUart0, 0);
  //
  // Sanity checks to avoid infinite loop when trying to print through uninitialized UART
  //
  if ((Base & 0xFFFFFF00) == 0x0 || (Base & 0xFFFFF000) == 0xFFFFF000) {
    return 0;
  }
  EnablePciMse (UartNumber);
  AccessMode = DetectAccessMode (Base);

  //
  // Read the serial port status
  //
  if ((ReadRegister (AccessMode, Base, R_PCH_SERIAL_IO_8BIT_UART_LSR) & B_PCH_SERIAL_IO_UART_LSR_RXDA) != 0) {
    return TRUE;
  }
  return FALSE;
}
