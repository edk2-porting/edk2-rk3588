/** @file
*  DWC3 XHCI controller #0 in host mode.
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
