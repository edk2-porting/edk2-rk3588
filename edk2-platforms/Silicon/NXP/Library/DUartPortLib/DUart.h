/** DUart.h
*  Header defining the DUART constants (Base addresses, sizes, flags)
*
*  Based on Serial I/O Port library headers available in PL011Uart.h
*
*  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
*  Copyright (c) 2016, Freescale Semiconductor, Inc. All rights reserved.
*  Copyright 2017, 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef DUART_H_
#define DUART_H_

// FIFO Control Register
#define DUART_FCR_FIFO_EN          0x01 /* Fifo enable */
#define DUART_FCR_CLEAR_RCVR       0x02 /* Clear the RCVR FIFO */
#define DUART_FCR_CLEAR_XMIT       0x04 /* Clear the XMIT FIFO */
#define DUART_FCR_DMA_SELECT       0x08 /* For DMA applications */
#define DUART_FCR_TRIGGER_MASK     0xC0 /* Mask for the FIFO trigger range */
#define DUART_FCR_TRIGGER_1        0x00 /* Mask for trigger set at 1 */
#define DUART_FCR_TRIGGER_4        0x40 /* Mask for trigger set at 4 */
#define DUART_FCR_TRIGGER_8        0x80 /* Mask for trigger set at 8 */
#define DUART_FCR_TRIGGER_14       0xC0 /* Mask for trigger set at 14 */
#define DUART_FCR_RXSR             0x02 /* Receiver soft reset */
#define DUART_FCR_TXSR             0x04 /* Transmitter soft reset */

// Modem Control Register
#define DUART_MCR_DTR              0x01 /* Reserved  */
#define DUART_MCR_RTS              0x02 /* RTS   */
#define DUART_MCR_OUT1             0x04 /* Reserved */
#define DUART_MCR_OUT2             0x08 /* Reserved */
#define DUART_MCR_LOOP             0x10 /* Enable loopback test mode */
#define DUART_MCR_AFE              0x20 /* AFE (Auto Flow Control) */
#define DUART_MCR_DMA_EN           0x04
#define DUART_MCR_TX_DFR           0x08

// Line Control Register
/*
* Note: if the word length is 5 bits (DUART_LCR_WLEN5), then setting
* DUART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
*/
#define DUART_LCR_WLS_MSK          0x03 /* character length select mask */
#define DUART_LCR_WLS_5            0x00 /* 5 bit character length */
#define DUART_LCR_WLS_6            0x01 /* 6 bit character length */
#define DUART_LCR_WLS_7            0x02 /* 7 bit character length */
#define DUART_LCR_WLS_8            0x03 /* 8 bit character length */
#define DUART_LCR_STB              0x04 /* # stop Bits, off=1, on=1.5 or 2) */
#define DUART_LCR_PEN              0x08 /* Parity eneble */
#define DUART_LCR_EPS              0x10 /* Even Parity Select */
#define DUART_LCR_STKP             0x20 /* Stick Parity */
#define DUART_LCR_SBRK             0x40 /* Set Break */
#define DUART_LCR_BKSE             0x80 /* Bank select enable */
#define DUART_LCR_DLAB             0x80 /* Divisor latch access bit */

// Line Status Register
#define DUART_LSR_DR               0x01 /* Data ready */
#define DUART_LSR_OE               0x02 /* Overrun */
#define DUART_LSR_PE               0x04 /* Parity error */
#define DUART_LSR_FE               0x08 /* Framing error */
#define DUART_LSR_BI               0x10 /* Break */
#define DUART_LSR_THRE             0x20 /* Xmit holding register empty */
#define DUART_LSR_TEMT             0x40 /* Xmitter empty */
#define DUART_LSR_ERR              0x80 /* Error */

// Modem Status Register
#define DUART_MSR_DCTS             0x01 /* Delta CTS */
#define DUART_MSR_DDSR             0x02 /* Reserved */
#define DUART_MSR_TERI             0x04 /* Reserved */
#define DUART_MSR_DDCD             0x08 /* Reserved */
#define DUART_MSR_CTS              0x10 /* Clear to Send */
#define DUART_MSR_DSR              0x20 /* Reserved */
#define DUART_MSR_RI               0x40 /* Reserved */
#define DUART_MSR_DCD              0x80 /* Reserved */

// Interrupt Identification Register
#define DUART_IIR_NO_INT           0x01 /* No interrupts pending */
#define DUART_IIR_ID               0x06 /* Mask for the interrupt ID */
#define DUART_IIR_MSI              0x00 /* Modem status interrupt */
#define DUART_IIR_THRI             0x02 /* Transmitter holding register empty */
#define DUART_IIR_RDI              0x04 /* Receiver data interrupt */
#define DUART_IIR_RLSI             0x06 /* Receiver line status interrupt */

//  Interrupt Enable Register
#define DUART_IER_MSI              0x08 /* Enable Modem status interrupt */
#define DUART_IER_RLSI             0x04 /* Enable receiver line status interrupt */
#define DUART_IER_THRI             0x02 /* Enable Transmitter holding register int. */
#define DUART_IER_RDI              0x01 /* Enable receiver data interrupt */

// LCR defaults
#define DUART_LCR_8N1              0x03
#define DUART_LCRVAL               DUART_LCR_8N1          /* 8 data, 1 stop, no parity */
#define DUART_MCRVAL               (DUART_MCR_DTR | \
                                   DUART_MCR_RTS)         /* RTS/DTR */
#define DUART_FCRVAL               (DUART_FCR_FIFO_EN | \
                                   DUART_FCR_RXSR |    \
                                   DUART_FCR_TXSR)        /* Clear & enable FIFOs */

#define URBR         0x0
#define UTHR         0x0
#define UDLB         0x0
#define UDMB         0x1
#define UIER         0x1
#define UIIR         0x2
#define UFCR         0x2
#define UAFR         0x2
#define ULCR         0x3
#define UMCR         0x4
#define ULSR         0x5
#define UMSR         0x6
#define USCR         0x7
#define UDSR         0x10

#endif /* DUART_H_ */
