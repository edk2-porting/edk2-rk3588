/** @file
*  DWC3 XHCI controller #1 in host mode. (only exposed on full RK3588)
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "AcpiTables.h"

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
