/** @file
*  DWC3 XHCI controller #2 in host mode.
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "AcpiTables.h"

// This only supports USB 3.0 devices.
// USB2 DP / DM are connected to one of the EHCI controllers instead (EHC1 usually).

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
