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

DefinitionBlock ("Dsdt.aml", "DSDT", 2, "RKCP  ", "RK3588  ", 2)
{
  Scope (\_SB_)
  {
    Name (PSUB, "GDYL8X2023")

    include ("ylplatform.asl")
    include ("Cpu.asl")

    // include ("Sdhc.asl")
    include ("ylsdhc.asl")
    // include ("Gmac.asl")
    // include ("Gpio.asl")
    include ("ylgpio.asl")
    // include ("I2c.asl")
    include ("Uart.asl")
    // include ("Spi.asl")
    // include ("Dbg2.asl") included in AcpiTables.inf
    
    include ("ylbatt.asl")
    // include ("ylaudio.asl")
    include ("UsbHc2.asl") // gedu private definition
    include ("yllid.asl") 
    
    // include ("Usb1Host.asl")
    include ("UsbHc1.asl")
    include ("Usb3Host0.asl")
    include ("Usb3Host1.asl")
    include ("Usb3Host2.asl")
    // include ("ylgewu.asl")
    include ("yltypec.asl")
    include ("ylsleep.asl")
  }
}
