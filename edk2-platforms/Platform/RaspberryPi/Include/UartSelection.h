/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef UART_SELECTION_H
#define UART_SELECTION_H

#define GPFSEL1_UART_MASK             (BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12)

#define PL011_UART_IN_USE_REG_VALUE   (BIT17 | BIT14)      // ALT0 - 0x24000
#define MINI_UART_IN_USE_REG_VALUE    (BIT16 | BIT13)      // ALT5 - 0x12000

#define PL011_UART_IN_USE             0
#define MINI_UART_IN_USE              1

#endif /* UART_SELECTION_H */
