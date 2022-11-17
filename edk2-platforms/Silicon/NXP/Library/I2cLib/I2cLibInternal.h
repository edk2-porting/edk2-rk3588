/** @file
  I2c Lib to control I2c controller.

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef I2C_LIB_INTERNAL_H__
#define I2C_LIB_INTERNAL_H__

#include <Pi/PiI2c.h>
#include <Uefi.h>

/** Module Disable
  0b - The module is enabled. You must clear this field before any other IBCR
       fields have any effect.
  1b - The module is reset and disabled. This is the power-on reset situation.
       When high, the interface is held in reset, but registers can still be
       accessed. Status register fields (IBSR) are not valid when the module
       is disabled.
**/
#define I2C_IBCR_MDIS      BIT7
// I2c Bus Interrupt Enable
#define I2C_IBCR_IBIE      BIT6
/** Master / Slave Mode 0b - Slave mode 1b - Master mode
  When you change this field from 0 to 1, the module generates a START signal
  on the bus and selects the master mode. When you change this field from 1 to
  0, the module generates a STOP signal and changes the operation mode from
  master to slave. You should generate a STOP signal only if IBSR[IBIF]=1.
  The module clears this field without generating a STOP signal when the
  master loses arbitration.
*/
#define I2C_IBCR_MSSL      BIT5
// 0b - Receive 1b - Transmit
#define I2C_IBCR_TXRX      BIT4
/** Data acknowledge disable
  Values written to this field are only used when the I2C module is a receiver,
  not a transmitter.
  0b - The module sends an acknowledge signal to the bus at the 9th clock bit
       after receiving one byte of data.
  1b - The module does not send an acknowledge-signal response (that is,
       acknowledge bit = 1).
**/
#define I2C_IBCR_NOACK     BIT3
/**Repeat START
  If the I2C module is the current bus master, and you program RSTA=1, the I2C
  module generates a repeated START condition. This field always reads as a 0.
  If you attempt a repeated START at the wrong time, if the bus is owned by
  another master the result is loss of arbitration.
**/
#define I2C_IBCR_RSTA      BIT2
// DMA enable
#define I2C_IBCR_DMAEN     BIT1

// Transfer Complete
#define I2C_IBSR_TCF       BIT7
// I2C bus Busy. 0b - Bus is idle, 1b - Bus is busy
#define I2C_IBSR_IBB       BIT5
// Arbitration Lost. software must clear this field by writing a one to it.
#define I2C_IBSR_IBAL      BIT4
// I2C bus interrupt flag
#define I2C_IBSR_IBIF      BIT1
// Received acknowledge 0b - Acknowledge received 1b - No acknowledge received
#define I2C_IBSR_RXAK      BIT0

//Bus idle interrupt enable
#define I2C_IBIC_BIIE      BIT7

// Glitch filter enable
#define I2C_IBDBG_GLFLT_EN BIT3

#define I2C_BUS_TEST_BUSY       TRUE
#define I2C_BUS_TEST_IDLE       !I2C_BUS_TEST_BUSY
#define I2C_BUS_TEST_RX_ACK     TRUE
#define I2C_BUS_NO_TEST_RX_ACK  !I2C_BUS_TEST_RX_ACK

#define ARRAY_LAST_ELEM(x)      (x)[ARRAY_SIZE (x) - 1]
#define I2C_NUM_RETRIES         500

typedef struct _I2C_REGS {
  UINT8 Ibad; // I2c Bus Address Register
  UINT8 Ibfd; // I2c Bus Frequency Divider Register
  UINT8 Ibcr; // I2c Bus Control Register
  UINT8 Ibsr; // I2c Bus Status Register
  UINT8 Ibdr; // I2C Bus Data I/O Register
  UINT8 Ibic; // I2C Bus Interrupt Config Register
  UINT8 Ibdbg; // I2C Bus Debug Register
} I2C_REGS;

/*
 * sorted list of clock divisor, Ibfd register value pairs
 */
typedef struct _I2C_CLOCK_DIVISOR_PAIR {
  UINT16  Divisor;
  UINT8   Ibfd; // I2c Bus Frequency Divider Register value
} I2C_CLOCK_DIVISOR_PAIR;

typedef struct {
  UINTN                           OperationCount;
  EFI_I2C_OPERATION               Operation[2];
} I2C_REG_REQUEST;

#endif // I2C_LIB_INTERNAL_H__
