/** @file

  SSDT for peripherals that are only enabled on B1 silicon

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>
  Copyright (c) 2018, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock ("SsdtB1.aml", "SSDT", 2, "AMDINC", "StyxB1  ", 3)
{
    Scope (_SB)
    {
        Device (AHC1)
        {
            Name (_HID, "AMDI0600")  // _HID: Hardware ID
            Name (_UID, 0x01)  // _UID: Unique ID
            Name (_CCA, 0x01)  // _CCA: Cache Coherency Attribute
            Name (_CLS, Package (0x03)  // _CLS: Class Code
            {
                0x01,
                0x06,
                0x01
            })
            Method (_STA)
            {
                If (LEqual (FixedPcdGet8 (PcdSata1PortCount), 0)) {
                    Return (0x0)
                }
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xE0D00000,         // Address Base (MMIO)
                    0x00010000,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE000007C,         // Address Base (SGPIO)
                    0x00000001,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x00000182, }
            })
        }

        Device (GIO2)
        {
            Name (_HID, "AMDI0400")  // _HID: Hardware ID
            Name (_CID, "ARMH0061")  // _CID: Compatible ID
            Name (_UID, 0x02)  // _UID: Unique ID

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xE0020000,         // Address Base
                    0x00001000,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x0000018E, }
            })
        }

        Device (GIO3)
        {
            Name (_HID, "AMDI0400")  // _HID: Hardware ID
            Name (_CID, "ARMH0061")  // _CID: Compatible ID
            Name (_UID, 0x03)  // _UID: Unique ID
            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xE0030000,         // Address Base
                    0x00001000,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x0000018D, }
            })
        }
    }
}
