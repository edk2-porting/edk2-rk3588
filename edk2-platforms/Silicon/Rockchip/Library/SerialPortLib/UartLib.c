/** @file
  Serial I/O Port library functions with no library constructor/destructor

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011 - 2016, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Rockchip Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include <Library/UartLib.h>

/* uart some key registers offset */
#define UART_RBR	0x00
#define UART_THR	0x00
#define UART_DLL	0x00
#define UART_DLH	0x04
#define UART_IER	0x04
#define UART_LCR	0x0c
#define UART_MCR	0x10
#define UART_LSR	0x14
#define UART_USR	0x7c
#define UART_SRR	0x88
#define UART_SFE	0x98
#define UART_SRT	0x9c
#define UART_STET	0xa0

#define  UART_LSR_TEMT                0x40 /* Transmitter empty */

/* UART_IER */
#define THRE_INT_ENABLE                    (1<<7)
#define THRE_INT_DISABLE                   (0)
#define ENABLE_MODEM_STATUS_INT            (1<<3)
#define DISABLE_MODEM_STATUS_INT           (0)
#define ENABLE_RECEIVER_LINE_STATUS_INT    (1<<2)
#define DISABLE_RECEIVER_LINE_STATUS_INT   (0)
#define ENABLE_TRANSMIT_HOLDING_EM_INT     (1<<1) /* Enable Transmit Holding Register Empty Interrupt. */
#define DISABLE_TRANSMIT_HOLDING_EM_INT    (0)
#define ENABLE_RECEIVER_DATA_INT           (1) /* Enable Received Data Available Interrupt. */
#define DISABLE_RECEIVER_DATA_INT          (0)

/* UART_IIR */
#define IR_MODEM_STATUS                    (0)
#define NO_INT_PENDING                     (1)
#define THR_EMPTY                          (2)
#define RECEIVER_DATA_AVAILABLE            (0x04)
#define RECEIVER_LINE_AVAILABLE            (0x06)
#define BUSY_DETECT                        (0x07)
#define CHARACTER_TIMEOUT                  (0x0c)

/* UART_LCR */
#define LCR_DLA_EN                         (1<<7)
#define BREAK_CONTROL_BIT                  (1<<6)
#define PARITY_DISABLED                    (0)
#define PARITY_ENABLED                     (1<<3)
#define ONE_STOP_BIT                       (0)
#define ONE_HALF_OR_TWO_BIT                (1<<2)
#define LCR_WLS_5                          (0x00)
#define LCR_WLS_6                          (0x01)
#define LCR_WLS_7                          (0x02)
#define LCR_WLS_8                          (0x03)
#define UART_DATABIT_MASK                  (0x03)

/* UART_MCR */
#define IRDA_SIR_DISABLED                  (0)
#define IRDA_SIR_ENSABLED                  (1<<6)
#define AUTO_FLOW_DISABLED                 (0)
#define AUTO_FLOW_ENSABLED                 (1<<5)

/* UART_LSR */
#define THRE_BIT_EN                        (1<<5)

/* UART_USR */
#define UART_RECEIVE_FIFO_EMPTY            (0)
#define UART_RECEIVE_FIFO_NOT_EMPTY        (1<<3)
#define UART_TRANSMIT_FIFO_FULL            (0)
#define UART_TRANSMIT_FIFO_NOT_FULL        (1<<1)
#define UART_TRANSMIT_FIFO_EMPTY           (1<<2)
#define UART_IS_BUSY                       (1<<0)

/* UART_SFE */
#define SHADOW_FIFI_ENABLED                (1)
#define SHADOW_FIFI_DISABLED               (0)

/* UART_SRT */
#define RCVR_TRIGGER_ONE                   (0)
#define RCVR_TRIGGER_QUARTER_FIFO          (1)
#define RCVR_TRIGGER_HALF_FIFO             (2)
#define RCVR_TRIGGER_TWO_LESS_FIFO         (3)

/* UART_STET */
#define TX_TRIGGER_EMPTY                   (0)
#define TX_TRIGGER_TWO_IN_FIFO             (1)
#define TX_TRIGGER_ONE_FOUR_FIFO           (2)
#define TX_TRIGGER_HALF_FIFO               (3)

/* UART_SRR */
#define UART_RESET                         (1)
#define RCVR_FIFO_REST                     (1<<1)
#define XMIT_FIFO_RESET                    (1<<2)

#define UART_MODE_X_DIV		16

#define UART_BIT5		5
#define UART_BIT6		6
#define UART_BIT7		7
#define UART_BIT8		8

/**

  Initialise the serial port to the specified settings.
  The serial port is re-configured only if the specified settings
  are different from the current settings.
  All unspecified settings will be set to the default values.

  @param  UartBase                The base address of the serial device.
  @param  UartClkInHz             The clock in Hz for the serial device.
                                  Ignored if the PCD PL011UartInteger is not 0
  @param  BaudRate                The baud rate of the serial device. If the
                                  baud rate is not supported, the speed will be
                                  reduced to the nearest supported one and the
                                  variable's value will be updated accordingly.
  @param  ReceiveFifoDepth        The number of characters the device will
                                  buffer on input.  Value of 0 will use the
                                  device's default FIFO depth.
  @param  Parity                  If applicable, this is the EFI_PARITY_TYPE
                                  that is computed or checked as each character
                                  is transmitted or received. If the device
                                  does not support parity, the value is the
                                  default parity value.
  @param  DataBits                The number of data bits in each character.
  @param  StopBits                If applicable, the EFI_STOP_BITS_TYPE number
                                  of stop bits per character.
                                  If the device does not support stop bits, the
                                  value is the default stop bit value.

  @retval RETURN_SUCCESS            All attributes were set correctly on the
                                    serial device.
  @retval RETURN_INVALID_PARAMETER  One or more of the attributes has an
                                    unsupported value.

**/
RETURN_STATUS
EFIAPI
UartInitializePort (
  IN     UINTN               UartBase,
  IN     UINT32              UartClkInHz,
  IN OUT UINT64              *BaudRate,
  IN OUT UINT32              *ReceiveFifoDepth,
  IN OUT EFI_PARITY_TYPE     *Parity,
  IN OUT UINT8               *DataBits,
  IN OUT EFI_STOP_BITS_TYPE  *StopBits
  )
{
  UINT32 Lcr, Rate;

  //do {} while ((MmioRead32(UartBase + UART_USR) & UART_IS_BUSY));

  // UART reset, rx fifo & tx fifo reset
  MmioWrite32(UartBase + UART_SRR, UART_RESET | RCVR_FIFO_REST | XMIT_FIFO_RESET);

  // UART interrupt disable
  MmioWrite32(UartBase + UART_IER, 0x00);

  // UART set iop
  MmioWrite32(UartBase + UART_MCR, IRDA_SIR_DISABLED);

  // UART set lcr
  Lcr = MmioRead32(UartBase + UART_LCR);
  Lcr &= ~UART_DATABIT_MASK;
   // byte set
  switch (*DataBits) {
  case UART_BIT5:
    Lcr |= LCR_WLS_5;
    break;
  case UART_BIT6:
    Lcr |= LCR_WLS_6;
    break;
  case UART_BIT7:
    Lcr |= LCR_WLS_7;
    break;
  case UART_BIT8:
  default:
    Lcr |= LCR_WLS_8;
    break;
  }

  // Parity set
  switch (*Parity) {
  case NoParity:
    Lcr |= PARITY_DISABLED;
    break;
  case DefaultParity:
  default:
    Lcr |= PARITY_ENABLED;
    break;
  }
  // stopbits set
  switch (*StopBits) {
  case TwoStopBits:
  case OneFiveStopBits:
	  Lcr |= ONE_HALF_OR_TWO_BIT;
	  break;
  case OneStopBit:
  default:
		Lcr |= ONE_STOP_BIT;
		break;
  }

  MmioWrite32(UartBase + UART_LCR, Lcr);

  // UART set baudrate
  /* uart rate is div for 24M input clock */
  Rate = PcdGet32 (UartClkInHz) / UART_MODE_X_DIV / *BaudRate;
  
  Lcr = MmioRead32(UartBase + UART_LCR);
  MmioWrite32(UartBase + UART_LCR, Lcr | LCR_DLA_EN);

  MmioWrite32(UartBase + UART_DLL, Rate & 0xFF);
  MmioWrite32(UartBase + UART_DLH, (Rate >> 8) & 0xff);
  
  Lcr = MmioRead32(UartBase + UART_LCR);
  MmioWrite32(UartBase + UART_LCR, Lcr & (~LCR_DLA_EN));

  // UART set fifo
  /* shadow FIFO enable */
  MmioWrite32(UartBase + UART_SFE, SHADOW_FIFI_ENABLED);
  /* fifo 2 less than */
  MmioWrite32(UartBase + UART_SRT, RCVR_TRIGGER_TWO_LESS_FIFO);
  /* 2 char in tx fifo */
  MmioWrite32(UartBase + UART_STET, TX_TRIGGER_TWO_IN_FIFO);

  return RETURN_SUCCESS;
}

/**

  Assert or deassert the control signals on a serial port.
  The following control signals are set according their bit settings :
  . Request to Send
  . Data Terminal Ready

  @param[in]  UartBase  UART registers base address
  @param[in]  Control   The following bits are taken into account :
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

  @retval  RETURN_SUCCESS      The new control bits were set on the device.
  @retval  RETURN_UNSUPPORTED  The device does not support this operation.

**/
RETURN_STATUS
EFIAPI
UartSetControl (
    IN UINTN   UartBase,
    IN UINT32  Control
  )
{

  return RETURN_SUCCESS;
}

/**

  Retrieve the status of the control bits on a serial device.

  @param[in]   UartBase  UART registers base address
  @param[out]  Control   Status of the control bits on a serial device :

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

  @retval RETURN_SUCCESS  The control bits were read from the serial device.

**/
RETURN_STATUS
EFIAPI
UartGetControl (
    IN UINTN     UartBase,
    OUT UINT32  *Control
  )
{

  return RETURN_SUCCESS;
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
UartWrite (
  IN  UINTN    UartBase,
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  UINT8* CONST Final = &Buffer[NumberOfBytes];

  while (Buffer < Final) {
    do {} while ((MmioRead32(UartBase + UART_USR) & UART_TRANSMIT_FIFO_NOT_FULL) == 0);
    MmioWrite32(UartBase + UART_THR, *Buffer++);
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
UartRead (
  IN  UINTN     UartBase,
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
  )
{
  UINTN   Count;

  for (Count = 0; Count < NumberOfBytes; Count++, Buffer++) {
    do {} while ((MmioRead32(UartBase + UART_USR) & UART_RECEIVE_FIFO_NOT_EMPTY) == 0);
    *Buffer = (UINT8)MmioRead32(UartBase + UART_RBR);
  }

  return NumberOfBytes;
}

/**
  Check to see if any data is available to be read from the debug device.

  @retval TRUE       At least one byte of data is available to be read
  @retval FALSE      No data is available to be read

**/
BOOLEAN
EFIAPI
UartPoll (
  IN  UINTN     UartBase
  )
{
  return (MmioRead32(UartBase + UART_USR) & UART_RECEIVE_FIFO_NOT_EMPTY) == UART_RECEIVE_FIFO_NOT_EMPTY;
}
