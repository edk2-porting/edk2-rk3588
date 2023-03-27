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
    Device (CPU0)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x0)
      Method (_STA)
      {
        Return (0xf)
      }
    }
    Device (CPU1)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x1)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    Device (CPU2)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x2)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    Device (CPU3)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x3)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    Device (CPU4)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x4)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    Device (CPU5)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x5)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    Device (CPU6)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x6)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    Device (CPU7)
    {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x7)
      Method (_STA)
      {
        Return (0xf)
      }
    }

    // include ("Pcie.asl")
    // include ("Sata.asl")
    // include ("Emmc.asl")
    include ("Sdhc.asl")
    // include ("Gmac.asl")
    // include ("Gpio.asl")
    // include ("I2c.asl")
    include ("Uart.asl")
    // include ("Spi.asl")

    // won't work on Windows, will trigger bugcheck by usbehci
    // include ("Usb2.asl")

    include ("Usb3.asl")
  }
}
