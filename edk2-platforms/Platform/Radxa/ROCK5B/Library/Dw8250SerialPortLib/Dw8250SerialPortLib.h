/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2015-2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015-2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under ArmPlatformPkg/Library/PL011SerialPortLib/
**/

#ifndef __DW8250_SERIALPORTLIB_H__
#define __DW8250_SERIALPORTLIB_H__


#define SERIAL_0_BASE_ADR     (PcdGet64(PcdSerialRegisterBase))


#define UART_SEND_DELAY      (PcdGet32(PcdSerialPortSendDelay))
#define BAUDRATE             (PcdGet64(PcdUartDefaultBaudRate))


#define UART_THR_REG         (SERIAL_0_BASE_ADR + UART_THR)
#define UART_RBR_REG         (SERIAL_0_BASE_ADR + UART_RBR)
#define UART_DLL_REG         (SERIAL_0_BASE_ADR + UART_DLL)
#define UART_DLH_REG         (SERIAL_0_BASE_ADR + UART_DLH)
#define UART_IEL_REG         (SERIAL_0_BASE_ADR + UART_IEL)
#define UART_IIR_REG         (SERIAL_0_BASE_ADR + UART_IIR)
#define UART_FCR_REG         (SERIAL_0_BASE_ADR + UART_FCR)
#define UART_LCR_REG         (SERIAL_0_BASE_ADR + UART_LCR)
#define UART_LSR_REG         (SERIAL_0_BASE_ADR + UART_LSR)
#define UART_USR_REG         (SERIAL_0_BASE_ADR + UART_USR)


#define UART_RBR     0x00
#define UART_THR     0x00
#define UART_DLL     0x00
#define UART_DLH     0x04
#define UART_IEL     0x04
#define UART_IIR     0x08
#define UART_FCR     0x08
#define UART_LCR     0x0C
#define UART_MCR     0x10
#define UART_LSR     0x14
#define UART_USR     0x7C

/* register definitions */

#define UART_FCR_EN          0x01
#define UART_FCR_RXCLR       0x02
#define UART_FCR_TXCLR       0x04
#define UART_FCR_CLEARFIFO   0x00
#define UART_FCR_RXL1        0x00
#define UART_FCR_RXL4        0x40
#define UART_FCR_RXL8        0x80
#define UART_FCR_RXL14       0xc0
#define UART_FCR_TXL0        0x00
#define UART_FCR_TXL4        0x20
#define UART_FCR_TXL8        0x30
#define UART_FCR_TXL14       0x10

/*LCR Name: Line Control Register fields*/
#define UART_LCR_DLAB   0x80
#define UART_LCR_EPS    0x10
#define UART_LCR_PEN    0x08
#define UART_LCR_STOP   0x04
#define UART_LCR_DLS8   0x03
#define UART_LCR_DLS7   0x02
#define UART_LCR_DLS6   0x01
#define UART_LCR_DLS5   0x00


#define UART_DLH_AND_DLL_WIDTH 0xFF


#define UART_IER_PTIME  0x80
#define UART_IER_ELSI   0x04
#define UART_IER_ETBEI  0x02
#define UART_IER_ERBFI  0x01


#define UART_IIR_FIFOSE         0xC0


#define UART_IIR_InterruptID   0x01
#define UART_IIR_INTIDTE       0x02
#define UART_IIR_INTIDRA       0x04
#define UART_IIR_INTIDRLS      0x06
#define UART_IIR_INTMASK       0x0f
#define UART_IIR_RDA           0x04
#define UART_IIR_TE            0x02

#define UART_LSR_TEMT       0x40
#define UART_LSR_THRE       0x20
#define UART_LSR_BI         0x10
#define UART_LSR_FE         0x08
#define UART_LSR_PE         0x04
#define UART_LSR_R          0x02
#define UART_LSR_DR         0x01


#define UART_USR_BUSY  0x01

#define FIFO_MAXSIZE    32

extern UINT8 SerialPortReadChar(VOID);
extern VOID SerialPortWriteChar(UINT8 scShowChar);

#endif

