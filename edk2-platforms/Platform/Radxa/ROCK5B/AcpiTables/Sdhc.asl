/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"


  Device (SDHC)
  {
    Name (_HID, "RKCPFE2C")
    Name (_UID, 0x1)
    Name (_CCA, 0x0)
    Name (_S1D, 0x1)
    Name (_S2D, 0x1)
    Name (_S3D, 0x1)
    Name (_S4D, 0x1)
    Method (_STA)
    {
      Return(0xf)
    }
    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe2c0000, 0x4000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 235 }
      })
      Return (RBUF)
    }
  } //SDHC