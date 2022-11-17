/** @file
  GPIO definition table for CometlakeURvp


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IO_EXPANDER_H_
#define _IO_EXPANDER_H_

typedef struct {
  UINT32 IoExpanderNumber   : 1;  // IO Expander Number (0/1)
  UINT32 GpioPinNumber      : 5;  // GPIO Pin Number (0 to 23)
  UINT32 GpioDirection      : 1;  // GPIO Pin Direction (Input/Output)
  UINT32 GpioLevel          : 1;  // GPIO Pin Output Level (High/Low)
  UINT32 GpioInversion     : 1;  // GPIO Pin Inversion (Enabled/Disabled)
  UINT32 Reserved           : 23; // Reserved
} IO_EXPANDER_GPIO_CONFIG;

//CML PCH LP GPIO Expander Number
#define IO_EXPANDER_0            0
#define IO_EXPANDER_1            1

//CML PCH LP GPIO Pin Mapping
#define IO_EXPANDER_GPIO_0        0   // P00
#define IO_EXPANDER_GPIO_1        1   // P01
#define IO_EXPANDER_GPIO_2        2   // P02
#define IO_EXPANDER_GPIO_3        3   // P03
#define IO_EXPANDER_GPIO_4        4   // P04
#define IO_EXPANDER_GPIO_5        5   // P05
#define IO_EXPANDER_GPIO_6        6   // P06
#define IO_EXPANDER_GPIO_7        7   // P07
#define IO_EXPANDER_GPIO_8        8   // P10
#define IO_EXPANDER_GPIO_9        9   // P11
#define IO_EXPANDER_GPIO_10       10  // P12
#define IO_EXPANDER_GPIO_11       11  // P13
#define IO_EXPANDER_GPIO_12       12  // P14
#define IO_EXPANDER_GPIO_13       13  // P15
#define IO_EXPANDER_GPIO_14       14  // P16
#define IO_EXPANDER_GPIO_15       15  // P17
#define IO_EXPANDER_GPIO_16       16  // P20
#define IO_EXPANDER_GPIO_17       17  // P21
#define IO_EXPANDER_GPIO_18       18  // P22
#define IO_EXPANDER_GPIO_19       19  // P23
#define IO_EXPANDER_GPIO_20       20  // P24
#define IO_EXPANDER_GPIO_21       21  // P25
#define IO_EXPANDER_GPIO_22       22  // P26
#define IO_EXPANDER_GPIO_23       23  // P27

//CML PCH LP GPIO Expander GPIO Direction
#define IO_EXPANDER_GPIO_OUTPUT   0
#define IO_EXPANDER_GPIO_INPUT    1

//CML PCH LP GPIO Expaner GPIO Output Level
#define IO_EXPANDER_GPO_LEVEL_LOW    0
#define IO_EXPANDER_GPO_LEVEL_HIGH   1

//CML PCH LP GPIO Expaner GPIO Inversion Status
#define IO_EXPANDER_GPI_INV_DISABLED  0
#define IO_EXPANDER_GPI_INV_ENABLED   1
#define IO_EXPANDER_GPIO_RESERVED     0x00

//GPIO Table Terminator
#define END_OF_GPIO_TABLE 0xFFFFFFFF

#endif

