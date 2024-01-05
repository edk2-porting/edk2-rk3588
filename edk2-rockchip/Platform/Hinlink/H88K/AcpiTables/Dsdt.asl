/** @file
 *
 *  Differentiated System Definition Table (DSDT)
 *
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2018-2020, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

#define BOARD_ENABLE_I2S0
#define BOARD_I2S0_TPLG "i2s-jack"

DefinitionBlock ("Dsdt.aml", "DSDT", 2, "RKCP  ", "RK3588  ", 2)
{
  Scope (\_SB_)
  {
    include ("Cpu.asl")

    include ("Emmc.asl")
    include ("Sdhc.asl")
    include ("Dma.asl")
    include ("Gmac0.asl")
    include ("Gpio.asl")
    include ("I2c.asl")
    include ("Uart.asl")
    // include ("Spi.asl")

    include ("I2s.asl")

    include ("Usb1Host.asl")
    include ("Usb3Host0.asl")
    include ("Usb3Host1.asl")
    include ("Usb3Host2.asl")
  }
}
