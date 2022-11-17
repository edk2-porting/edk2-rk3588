/** DuartPortLib.c
  DUART (NS16550) library functions

  Based on Serial I/O Port library functions available in PL011SerialPortLib.c

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2012 - 2013, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2016, Freescale Semiconductor, Inc. All rights reserved.
  Copyright 2017, 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Ppi/NxpPlatformGetClock.h>

#include "DUart.h"

STATIC CONST UINT32 mInvalidControlBits = (EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE | \
                                           EFI_SERIAL_DATA_TERMINAL_READY);

/**
  Assert or deassert the control signals on a serial port.
  The following control signals are set according their bit settings :
  . Request to Send
  . Data Terminal Ready

  @param[in]  Control     The following bits are taken into account :
                          . EFI_SERIAL_REQUEST_TO_SEND : assert/deassert the
                            "Request To Send" control signal if this bit is
                            equal to one/zero.
                          . EFI_SERIAL_DATA_TERMINAL_READY : assert/deassert
                            the "Data Terminal Ready" control signal if this
                            bit is equal to one/zero.
                          . EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE : enable/disable
                            the hardware loopback if this bit is equal to
                            one/zero.
                          . EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE : not supported.
                          . EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE : enable/
                            disable the hardware flow control based on CTS (Clear
                            To Send) and RTS (Ready To Send) control signals.

  @retval  EFI_SUCCESS      The new control bits were set on the device.
  @retval  EFI_UNSUPPORTED  The device does not support this operation.

**/
EFI_STATUS
EFIAPI
SerialPortSetControl (
  IN  UINT32  Control
  )
{
  UINT32  McrBits;
  UINTN   UartBase;

  UartBase = (UINTN)PcdGet64 (PcdSerialRegisterBase);

  if (Control & (mInvalidControlBits)) {
    return EFI_UNSUPPORTED;
  }

  McrBits = MmioRead8 (UartBase + UMCR);

  if (Control & EFI_SERIAL_REQUEST_TO_SEND) {
    McrBits |= DUART_MCR_RTS;
  } else {
    McrBits &= ~DUART_MCR_RTS;
  }

  if (Control & EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE) {
    McrBits |= DUART_MCR_LOOP;
  } else {
    McrBits &= ~DUART_MCR_LOOP;
  }

  if (Control & EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE) {
    McrBits |= DUART_MCR_AFE;
  } else {
    McrBits &= ~DUART_MCR_AFE;
  }

  MmioWrite32 (UartBase + UMCR, McrBits);

  return EFI_SUCCESS;
}

/**
  Retrieve the status of the control bits on a serial device.

  @param[out]  Control     Status of the control bits on a serial device :

                         . EFI_SERIAL_DATA_CLEAR_TO_SEND,
                           EFI_SERIAL_DATA_SET_READY,
                           EFI_SERIAL_RING_INDICATE,
                           EFI_SERIAL_CARRIER_DETECT,
                           EFI_SERIAL_REQUEST_TO_SEND,
                           EFI_SERIAL_DATA_TERMINAL_READY
                           are all related to the DTE (Data Terminal Equipment)
                           and DCE (Data Communication Equipment) modes of
                           operation of the serial device.
                         . EFI_SERIAL_INPUT_BUFFER_EMPTY : equal to one if the
                           receive buffer is empty, 0 otherwise.
                         . EFI_SERIAL_OUTPUT_BUFFER_EMPTY : equal to one if the
                           transmit buffer is empty, 0 otherwise.
                         . EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE : equal to one if
                           the hardware loopback is enabled (the ouput feeds the
                           receive buffer), 0 otherwise.
                         . EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE : equal to one if
                           a loopback is accomplished by software, 0 otherwise.
                         . EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE : equal to
                           one if the hardware flow control based on CTS (Clear
                           To Send) and RTS (Ready To Send) control signals is
                           enabled, 0 otherwise.

  @retval EFI_SUCCESS      The control bits were read from the serial device.

**/
EFI_STATUS
EFIAPI
SerialPortGetControl (
  OUT  UINT32   *Control
  )
{
  UINT32        MsrRegister;
  UINT32        McrRegister;
  UINT32        LsrRegister;
  UINTN         UartBase;

  UartBase = (UINTN)PcdGet64 (PcdSerialRegisterBase);

  MsrRegister = MmioRead8 (UartBase + UMSR);
  McrRegister = MmioRead8 (UartBase + UMCR);
  LsrRegister = MmioRead8 (UartBase + ULSR);

  *Control = 0;

  if ((MsrRegister & DUART_MSR_CTS) == DUART_MSR_CTS) {
    *Control |= EFI_SERIAL_CLEAR_TO_SEND;
  }

  if ((McrRegister & DUART_MCR_RTS) == DUART_MCR_RTS) {
    *Control |= EFI_SERIAL_REQUEST_TO_SEND;
  }

  if ((LsrRegister & DUART_LSR_TEMT) == DUART_LSR_TEMT) {
    *Control |= EFI_SERIAL_OUTPUT_BUFFER_EMPTY;
  }

  if ((McrRegister & DUART_MCR_AFE) == DUART_MCR_AFE) {
    *Control |= EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE;
  }

  if ((McrRegister & DUART_MCR_LOOP) == DUART_MCR_LOOP) {
    *Control |= EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE;
  }

  return EFI_SUCCESS;
}

/*
 * Return Baud divisor on basis of Baudrate
 */
UINT32
CalculateBaudDivisor (
  IN UINT64 BaudRate
  )
{
  UINTN DUartClk;

  DUartClk = gPlatformGetClockPpi.PlatformGetClock (NXP_UART_CLOCK, 0);

  return ((DUartClk)/(BaudRate * 16));
}

/*
   Initialise the serial port to the specified settings.
   All unspecified settings will be set to the default values.

   @return    Always return EFI_SUCCESS or EFI_INVALID_PARAMETER.

 **/
VOID
EFIAPI
DuartInitializePort (
  IN  UINT64  BaudRate
  )
{
  UINTN   UartBase;
  UINT32  BaudDivisor;

  UartBase = (UINTN)PcdGet64 (PcdSerialRegisterBase);
  BaudDivisor = CalculateBaudDivisor (BaudRate);


  while (!(MmioRead8 (UartBase + ULSR) & DUART_LSR_TEMT));

  //
  // Enable and assert interrupt when new data is available on
  // external device,
  // setup data format, setup baud divisor
  //
  MmioWrite8 (UartBase + UIER, 0x1);
  MmioWrite8 (UartBase + ULCR, DUART_LCR_BKSE | DUART_LCRVAL);
  MmioWrite8 (UartBase + UDLB, 0);
  MmioWrite8 (UartBase + UDMB, 0);
  MmioWrite8 (UartBase + ULCR, DUART_LCRVAL);
  MmioWrite8 (UartBase + UMCR, DUART_MCRVAL);
  MmioWrite8 (UartBase + UFCR, DUART_FCRVAL);
  MmioWrite8 (UartBase + ULCR, DUART_LCR_BKSE | DUART_LCRVAL);
  MmioWrite8 (UartBase + UDLB, BaudDivisor & 0xff);
  MmioWrite8 (UartBase + UDMB, (BaudDivisor >> 8) & 0xff);
  MmioWrite8 (UartBase + ULCR, DUART_LCRVAL);

  return;
}

/**
  Programmed hardware of Serial port.

  @return    Always return EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  UINT64  BaudRate;
  BaudRate = (UINTN)PcdGet64 (PcdUartDefaultBaudRate);


  DuartInitializePort (BaudRate);

  return EFI_SUCCESS;
}

/**
  Write data to serial device.

  @param  Buffer           Point of data buffer which need to be written.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Write data failed.
  @retval !0               Actual number of bytes written to serial device.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN  UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
  )
{
  UINT8         *Final;
  UINTN         UartBase;

  Final = &Buffer[NumberOfBytes];
  UartBase = (UINTN)PcdGet64 (PcdSerialRegisterBase);

  while (Buffer < Final) {
    while ((MmioRead8 (UartBase + ULSR) & DUART_LSR_THRE) == 0);
    MmioWrite8 (UartBase + UTHR, *Buffer++);
  }

  return NumberOfBytes;
}

/**
  Read data from serial device and save the data in buffer.

  @param  Buffer           Point of data buffer which need to be written.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Read data failed.
  @retval !0               Actual number of bytes read from serial device.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
  )
{
  UINTN   Count;
  UINTN   UartBase;

  UartBase = (UINTN)PcdGet64 (PcdSerialRegisterBase);

  for (Count = 0; Count < NumberOfBytes; Count++, Buffer++) {
     // Loop while waiting for a new char(s) to arrive in the
     // RxFIFO
    while ((MmioRead8 (UartBase + ULSR) & DUART_LSR_DR) == 0);

    *Buffer = MmioRead8 (UartBase + URBR);
  }

  return NumberOfBytes;
}

/**
  Check to see if any data is available to be read from the debug device.

  @retval EFI_SUCCESS       At least one byte of data is available to be read
  @retval EFI_NOT_READY     No data is available to be read
  @retval EFI_DEVICE_ERROR  The serial device is not functioning properly

**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  UINTN   UartBase;

  UartBase = (UINTN)PcdGet64 (PcdSerialRegisterBase);

  return ((MmioRead8 (UartBase + ULSR) & DUART_LSR_DR) != 0);
}

/**
  Set new attributes to LS1043a.

  @param  BaudRate                The baud rate of the serial device. If the baud rate is not supported,
                                  the speed will be reduced down to the nearest supported one and the
                                  variable's value will be updated accordingly.
  @param  ReceiveFifoDepth        The number of characters the device will buffer on input. If the specified
                                  value is not supported, the variable's value will be reduced down to the
                                  nearest supported one.
  @param  Timeout                 If applicable, the number of microseconds the device will wait
                                  before timing out a Read or a Write operation.
  @param  Parity                  If applicable, this is the EFI_PARITY_TYPE that is computed or checked
                                  as each character is transmitted or received. If the device does not
                                  support parity, the value is the default parity value.
  @param  DataBits                The number of data bits in each character
  @param  StopBits                If applicable, the EFI_STOP_BITS_TYPE number of stop bits per character.
                                  If the device does not support stop bits, the value is the default stop
                                  bit value.

  @retval EFI_SUCCESS             All attributes were set correctly on the serial device.

**/
EFI_STATUS
EFIAPI
SerialPortSetAttributes (
  IN  OUT  UINT64              *BaudRate,
  IN  OUT  UINT32              *ReceiveFifoDepth,
  IN  OUT  UINT32              *Timeout,
  IN  OUT  EFI_PARITY_TYPE     *Parity,
  IN  OUT  UINT8               *DataBits,
  IN  OUT  EFI_STOP_BITS_TYPE  *StopBits
  )
{
  DuartInitializePort (*BaudRate);

  return EFI_SUCCESS;
}
