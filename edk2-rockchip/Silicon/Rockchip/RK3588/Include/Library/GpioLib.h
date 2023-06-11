/** @file
 *
 *  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef GPIOLIB_H__
#define GPIOLIB_H__

#define GPIO_PIN_PA0    0
#define GPIO_PIN_PA1    1
#define GPIO_PIN_PA2    2
#define GPIO_PIN_PA3    3
#define GPIO_PIN_PA4    4
#define GPIO_PIN_PA5    5
#define GPIO_PIN_PA6    6
#define GPIO_PIN_PA7    7
#define GPIO_PIN_PB0    8
#define GPIO_PIN_PB1    9
#define GPIO_PIN_PB2    10
#define GPIO_PIN_PB3    11
#define GPIO_PIN_PB4    12
#define GPIO_PIN_PB5    13
#define GPIO_PIN_PB6    14
#define GPIO_PIN_PB7    15
#define GPIO_PIN_PC0    16
#define GPIO_PIN_PC1    17
#define GPIO_PIN_PC2    18
#define GPIO_PIN_PC3    19
#define GPIO_PIN_PC4    20
#define GPIO_PIN_PC5    21
#define GPIO_PIN_PC6    22
#define GPIO_PIN_PC7    23
#define GPIO_PIN_PD0    24
#define GPIO_PIN_PD1    25
#define GPIO_PIN_PD2    26
#define GPIO_PIN_PD3    27
#define GPIO_PIN_PD4    28
#define GPIO_PIN_PD5    29
#define GPIO_PIN_PD6    30
#define GPIO_PIN_PD7    31

typedef enum {
  GPIO_PIN_INPUT  = 0,
  GPIO_PIN_OUTPUT = 1
} GPIO_PIN_DIRECTION;

typedef enum {
  GPIO_PIN_PULL_NONE  = 0,
  GPIO_PIN_PULL_UP    = 1,
  GPIO_PIN_PULL_DOWN  = 2
} GPIO_PIN_PULL;

typedef enum {
  GPIO_PIN_DRIVE_DEFAULT = 0xFF,
  GPIO_PIN_DRIVE_DISABLE = 0x0,
  GPIO_PIN_DRIVE_0       = 0x1,
  GPIO_PIN_DRIVE_1       = 0x3,
  GPIO_PIN_DRIVE_2       = 0x7,
  GPIO_PIN_DRIVE_3       = 0xF,
  GPIO_PIN_DRIVE_4       = 0x1F,
  GPIO_PIN_DRIVE_5       = 0x3F
} GPIO_PIN_DRIVE;

typedef enum {
  GPIO_PIN_INPUT_DEFAULT      = 0xFF,
  GPIO_PIN_INPUT_DISABLE      = 0x0,
  GPIO_PIN_INPUT_NON_SCHMITT  = 0x1,
  GPIO_PIN_INPUT_SCHMITT      = 0x2
} GPIO_PIN_INPUT_ENABLE;

typedef struct {
  CONST char *Name;
  UINT8 Group;
  UINT8 Pin;
  UINT8 Function;
  GPIO_PIN_PULL Pull;
  GPIO_PIN_DRIVE Drive;
} GPIO_IOMUX_CONFIG;

VOID
GpioPinSetDirection (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_DIRECTION Direction
  );

VOID
GpioPinWrite (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN BOOLEAN Value
  );

BOOLEAN
GpioPinRead (
  IN UINT8 Group,
  IN UINT8 Pin
  );

VOID
GpioPinSetFunction (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN UINT8 Function
  );

VOID
GpioPinSetPull (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_PULL Pull
  );

VOID
GpioPinSetDrive (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_DRIVE Drive
  );

VOID
GpioPinSetInput (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_INPUT_ENABLE InputEnable
  );

VOID
GpioSetIomuxConfig (
  IN CONST GPIO_IOMUX_CONFIG *Configs,
  IN UINT32 NumConfigs
  );

#endif /* GPIOLIB_H__ */
