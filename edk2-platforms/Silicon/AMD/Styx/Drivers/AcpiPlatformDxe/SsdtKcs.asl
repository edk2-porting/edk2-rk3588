/** @file

  SSDT for IPMI controller

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


DefinitionBlock ("SsdtKcs.aml", "SSDT", 2, "AMDINC", "StyxKcs ", 3)
{
    Scope (_SB)
    {
        //
        // IPMI/KCS
        //
        Device (KCS0)
        {
            Name (_HID, "AMDI0300")
            Name (_CID, "IPI0001")
            Name (_STR, Unicode("IPMI_KCS"))
            Name (_UID, 0)
            Name (_CRS, ResourceTemplate() {
                Memory32Fixed(ReadWrite, 0xE0010000, 0x1) // KCS Data In/Out
                Memory32Fixed(ReadWrite, 0xE0010004, 0x1) // KCS Control/Status
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive,,,) { 421 } // GSIV
            })
            Method (_IFT) {     // Interface Type
                Return ( 0x01)   // IPMI KCS
            }

            Method (_SRV) {     // Spec Revision
                Return (0x200)  // IPMI Spec v2.0
            }
        }
    }
}

