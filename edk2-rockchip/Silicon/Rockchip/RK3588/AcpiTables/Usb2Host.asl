/** @file
 *
 *  USB 2.0 (EHCI) controllers.
 *
 *  Copyright (c) 2022, Rockchip Electronics Co., Ltd
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

DefinitionBlock (__FILE__, "SSDT", 5, "RKCP  ", "USB2HOST", 2)
{
  Scope (\_SB_)
  {
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
        }
      }
    }

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
        }
      }
    }
  } // Scope (\_SB_)
} // DefinitionBlock
