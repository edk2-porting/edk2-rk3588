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

#define GMAC0_CONFIGURE_PHY
#define GMAC0_PHY_MODE 	"rgmii-rxid"
#define GMAC0_PHY_HANDLE 	MDIO.PHY0

#define GMAC1_CONFIGURE_PHY
#define GMAC1_PHY_MODE 	"rgmii-rxid"
#define GMAC1_PHY_HANDLE 	MDIO.PHY1

DefinitionBlock ("Dsdt.aml", "DSDT", 2, "RKCP  ", "RK3588  ", 2)
{
  Scope (\_SB_)
  {
    include ("Cpu.asl")
    include ("Cru.asl")

    include ("Pcie.asl")
    include ("Sata.asl")
    include ("Emmc.asl")
    include ("Sdhc.asl")
    include ("Dma.asl")
    include ("Gmac0.asl")
    include ("Gmac1.asl")
    include ("Gpio.asl")
    include ("I2c.asl")
    include ("Uart.asl")
    // include ("Spi.asl")

    include ("Usb2Host.asl")
    include ("Usb3Host0.asl")
    include ("Usb3Host1.asl")
    include ("Usb3Host2.asl")
    
    Scope (MAC0.MDIO)
    {
      Device (PHY0) {
        Name (_ADR, 0x0)
      }
    }
    
    Scope (MAC1.MDIO)
    {
      Device (PHY1) {
        Name (_ADR, 0x1)
      }
    }
  }
}
