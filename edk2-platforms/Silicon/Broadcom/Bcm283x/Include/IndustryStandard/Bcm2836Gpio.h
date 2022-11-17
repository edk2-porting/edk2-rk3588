/** @file
 *
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Bcm2836.h>

#ifndef __BCM2836_GPIO_H__
#define __BCM2836_GPIO_H__

#define GPIO_OFFSET        0x00200000
#define GPIO_BASE_ADDRESS  (BCM2836_SOC_REGISTERS + GPIO_OFFSET)
#define GPIO_LENGTH        0x000000B4

#define GPIO_GPFSEL0       (GPIO_BASE_ADDRESS + 0x00)
#define GPIO_GPFSEL1       (GPIO_BASE_ADDRESS + 0x04)
#define GPIO_GPFSEL2       (GPIO_BASE_ADDRESS + 0x08)
#define GPIO_GPFSEL3       (GPIO_BASE_ADDRESS + 0x0C)
#define GPIO_GPFSEL4       (GPIO_BASE_ADDRESS + 0x10)
#define GPIO_GPFSEL5       (GPIO_BASE_ADDRESS + 0x14)

#define GPIO_GPSET0        (GPIO_BASE_ADDRESS + 0x1C)
#define GPIO_GPSET1        (GPIO_BASE_ADDRESS + 0x20)

#define GPIO_GPCLR0        (GPIO_BASE_ADDRESS + 0x28)
#define GPIO_GPCLR1        (GPIO_BASE_ADDRESS + 0x2C)

#define GPIO_GPLEV0        (GPIO_BASE_ADDRESS + 0x34)
#define GPIO_GPLEV1        (GPIO_BASE_ADDRESS + 0x38)

#define GPIO_GPEDS0        (GPIO_BASE_ADDRESS + 0x40)
#define GPIO_GPEDS1        (GPIO_BASE_ADDRESS + 0x44)

#define GPIO_GPREN0        (GPIO_BASE_ADDRESS + 0x4C)
#define GPIO_GPREN1        (GPIO_BASE_ADDRESS + 0x50)

#define GPIO_GPFEN0        (GPIO_BASE_ADDRESS + 0x58)
#define GPIO_GPFEN1        (GPIO_BASE_ADDRESS + 0x5C)

#define GPIO_GPHEN0        (GPIO_BASE_ADDRESS + 0x64)
#define GPIO_GPHEN1        (GPIO_BASE_ADDRESS + 0x68)

#define GPIO_GPLEN0        (GPIO_BASE_ADDRESS + 0x70)
#define GPIO_GPLEN1        (GPIO_BASE_ADDRESS + 0x74)

#define GPIO_GPAREN0       (GPIO_BASE_ADDRESS + 0x7C)
#define GPIO_GPAREN1       (GPIO_BASE_ADDRESS + 0x80)

#define GPIO_GPAFEN0       (GPIO_BASE_ADDRESS + 0x88)
#define GPIO_GPAFEN1       (GPIO_BASE_ADDRESS + 0x8C)

#define GPIO_GPPUD         (GPIO_BASE_ADDRESS + 0x94)
#define GPIO_GPPUDCLK0     (GPIO_BASE_ADDRESS + 0x98)
#define GPIO_GPPUDCLK1     (GPIO_BASE_ADDRESS + 0x9C)

#define GPIO_GPPUPPDN0     (GPIO_BASE_ADDRESS + 0xE4)
#define GPIO_GPPUPPDN1     (GPIO_BASE_ADDRESS + 0xE8)
#define GPIO_GPPUPPDN2     (GPIO_BASE_ADDRESS + 0xEC)
#define GPIO_GPPUPPDN3     (GPIO_BASE_ADDRESS + 0xF0)

#define GPIO_FSEL_INPUT    0x0
#define GPIO_FSEL_OUTPUT   0x1
#define GPIO_FSEL_ALT0     0x4
#define GPIO_FSEL_ALT1     0x5
#define GPIO_FSEL_ALT2     0x6
#define GPIO_FSEL_ALT3     0x7
#define GPIO_FSEL_ALT4     0x3
#define GPIO_FSEL_ALT5     0x2

#define GPIO_FSEL_PINS_PER_REGISTER 10
#define GPIO_FSEL_BITS_PER_PIN      3
#define GPIO_FSEL_MASK              ((1 << GPIO_FSEL_BITS_PER_PIN) - 1)

#define GPIO_PINS          54

#define GPIO_PULL_NONE     0x00
#define GPIO_PULL_DOWN     0x01
#define GPIO_PULL_UP       0x02

#endif /* __BCM2836_GPIO_H__ */
