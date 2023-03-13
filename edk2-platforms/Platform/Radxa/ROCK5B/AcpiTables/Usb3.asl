/** @file
*  DWC3 XHCI Controllers.
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "AcpiTables.h"

Device (XHC0) {
    Name (_HID, "PNP0D10")
    Name (_UID, Zero)
    Name (_CCA, Zero)

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xfc000000, 0x400000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 252 }
        })
        Return (RBUF)
    }   
}

Device (XHC1) {
    Name (_HID, "PNP0D10")
    Name (_UID, One)
    Name (_CCA, Zero)

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xfc400000, 0x400000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 253 }
        })
        Return (RBUF)
    }   
}

// This only supports USB 3.0 devices.
// USB2 DP / DM are connected to one of the EHCI controllers instead (EHC1 on ROCK 5B).

Device (XHC2) {
    Name (_HID, "PNP0D10")
    Name (_UID, 2)
    Name (_CCA, Zero)

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xfcd00000, 0x400000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 254 }
        })
        Return (RBUF)
    }   
}
