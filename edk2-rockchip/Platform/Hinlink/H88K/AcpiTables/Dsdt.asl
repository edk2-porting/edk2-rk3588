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

DefinitionBlock ("Dsdt.aml", "DSDT", 2, "RPIFDN", "RPI", 2)
{
  Scope (\_SB_)
  {
    include ("Cpu.asl")

    include ("Emmc.asl")
    include ("Sdhc.asl")
    include ("Gmac0.asl")
    // include ("Gpio.asl")
    // include ("I2c.asl")
    include ("Uart.asl")
    // include ("Spi.asl")

    include ("Usb1Host.asl")
    include ("Usb3Host0.asl")
    include ("Usb3Host1.asl")
    include ("Usb3Host2.asl")
  }
}
