/** @file
*  USB2 devices.
*
*  Copyright (c) 2022, Rockchip Electronics Co., Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "AcpiTables.h"

// USB OHCI Host Controller
Device (OHC0) {
    Name (_HID, "PRP0001")
    Name (_CLS, Package() { 0x0c, 0x03, 0x10 })
    Name (_UID, Zero)
    Name (_CCA, Zero)

    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "compatible", "generic-ohci" },
        }
    })

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xFC840000, 0x40000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 248 }
        })
        Return (RBUF)
    }

//    Name (_STA, FixedPcdGet8(PcdOhc0Status))

    Device (RHUB) {
        Name (_ADR, 0)
        Device (PRT1) {
            Name (_ADR, 1)
            Name (_UPC, Package() {
                0xFF,         // Port is connectable
                0x00,         // Connector type - Type 'A'
                0x00000000,   // Reserved, must be zero
                0x00000000    // Reserved, must be zero
            })
            Name (_PLD, Package (0x01) {
                ToPLD (
                PLD_Revision            = 0x2,
                PLD_IgnoreColor         = 0x1,
                PLD_UserVisible         = 0x1,
                PLD_Panel               = "UNKNOWN",
                PLD_VerticalPosition    = "UPPER",
                PLD_HorizontalPosition  = "LEFT",
                PLD_Shape               = "HORIZONTALRECTANGLE",
                PLD_Ejectable           = 0x1,
                PLD_EjectRequired       = 0x1,
                )
            })
        } // PRT1
    } // RHUB
} // OHC0

// USB EHCI Host Controller
Device (EHC0) {
    Name (_HID, "PNP0D20")
    Name (_UID, Zero)
    Name (_CCA, Zero)

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xFC800000, 0x40000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 247 }
        })
        Return (RBUF)
    }

//    Name (_STA, FixedPcdGet8(PcdEhc0Status))

    Device (RHUB) {
        Name (_ADR, 0)
        Device (PRT1) {
            Name (_ADR, 1)
            Name (_UPC, Package() {
            0xFF,         // Port is connectable
            0x00,         // Connector type - Type 'A'
            0x00000000,   // Reserved, must be zero
            0x00000000    // Reserved, must be zero
            })
            Name (_PLD, Package (0x01) {
            ToPLD (
                PLD_Revision            = 0x2,
                PLD_IgnoreColor         = 0x1,
                PLD_UserVisible         = 0x1,
                PLD_Panel               = "UNKNOWN",
                PLD_VerticalPosition    = "UPPER",
                PLD_HorizontalPosition  = "LEFT",
                PLD_Shape               = "HORIZONTALRECTANGLE",
                PLD_Ejectable           = 0x1,
                PLD_EjectRequired       = 0x1,
            )
            })
        } // PRT1
    } // RHUB
} // EHC0

// USB OHCI Host Controller
Device (OHC1) {
    Name (_HID, "PRP0001")
    Name (_CLS, Package() { 0x0c, 0x03, 0x10 })
    Name (_UID, One)
    Name (_CCA, Zero)

    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "compatible", "generic-ohci" },
        }
    })

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xFC8C0000, 0x40000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 251 }
        })
        Return (RBUF)
    }

//    Name (_STA, FixedPcdGet8(PcdOhc1Status))

    Device (RHUB) {
        Name (_ADR, 0)
        Device (PRT1) {
            Name (_ADR, 1)
            Name (_UPC, Package() {
                0xFF,         // Port is connectable
                0x00,         // Connector type - Type 'A'
                0x00000000,   // Reserved, must be zero
                0x00000000    // Reserved, must be zero
            })
            Name (_PLD, Package (0x01) {
                ToPLD (
                PLD_Revision            = 0x2,
                PLD_IgnoreColor         = 0x1,
                PLD_UserVisible         = 0x1,
                PLD_Panel               = "UNKNOWN",
                PLD_VerticalPosition    = "LOWER",
                PLD_HorizontalPosition  = "LEFT",
                PLD_Shape               = "HORIZONTALRECTANGLE",
                PLD_Ejectable           = 0x1,
                PLD_EjectRequired       = 0x1,
                )
            })
        } // PRT1
    } // RHUB
} // OHC1

// USB EHCI Host Controller
Device (EHC1) {
    Name (_HID, "PNP0D20")
    Name (_UID, One)
    Name (_CCA, Zero)

    Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
            Memory32Fixed (ReadWrite, 0xFC880000, 0x40000)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 250 }
        })
        Return (RBUF)
    }

//    Name (_STA, FixedPcdGet8(PcdEhc1Status))

    Device (RHUB) {
        Name (_ADR, 0)
        Device (PRT1) {
            Name (_ADR, 1)
            Name (_UPC, Package() {
                0xFF,         // Port is connectable
                0x00,         // Connector type - Type 'A'
                0x00000000,   // Reserved, must be zero
                0x00000000    // Reserved, must be zero
            })
            Name (_PLD, Package (0x01) {
                ToPLD (
                PLD_Revision            = 0x2,
                PLD_IgnoreColor         = 0x1,
                PLD_UserVisible         = 0x1,
                PLD_Panel               = "UNKNOWN",
                PLD_VerticalPosition    = "LOWER",
                PLD_HorizontalPosition  = "LEFT",
                PLD_Shape               = "HORIZONTALRECTANGLE",
                PLD_Ejectable           = 0x1,
                PLD_EjectRequired       = 0x1,
                )
            })
        } // PRT1
    } // RHUB
} // EHC1
