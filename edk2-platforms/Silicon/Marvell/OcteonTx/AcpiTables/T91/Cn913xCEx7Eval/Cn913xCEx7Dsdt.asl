/** @file

  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>
  Copyright (C) 2019, Marvell International Ltd. and its affiliates.<BR>
  Copyright (C) 2021, Semihalf.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock ("Cn913xCEx7.aml", "DSDT", 2, "MRVL", "CN9130", 3)
{
    Scope (_SB)
    {
        Device (CPU0)
        {
            Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
            Name (_UID, 0x000)  // _UID: Unique ID
            Method (_STA)       // _STA: Device status
            {
                Return (0xF)
            }
        }
        Device (CPU1)
        {
            Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
            Name (_UID, 0x001)  // _UID: Unique ID
            Method (_STA)       // _STA: Device status
            {
                Return (0xF)
            }
        }
        Device (CPU2)
        {
            Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
            Name (_UID, 0x100)  // _UID: Unique ID
            Method (_STA)       // _STA: Device status
            {
                Return (0xF)
            }
        }
        Device (CPU3)
        {
            Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
            Name (_UID, 0x101)  // _UID: Unique ID
            Method (_STA)       // _STA: Device status
            {
                Return (0xF)
            }
        }

        Device (MMC0)
        {
            Name (_HID, "MRVL0003")     // _HID: Hardware ID
            Name (_UID, 0x00)           // _UID: Unique ID
            Name (_CCA, 0x01)           // _CCA: Cache Coherency Attribute
            Method (_STA)               // _STA: Device status
            {
                Return (0xF)
            }

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xF06E0000,         // Address Base (MMIO)
                    0x00000300,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  48
                }
            })
            Name (_DSD, Package () {
                ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package () {
                      Package () { "clock-frequency", 400000000 },
                      Package () { "bus-width", 8 },
                      Package () { "no-sd", 0x1 },
                      Package () { "no-sdio", 0x1  },
                      Package () { "mmc-ddr-1_8v", 0x1  },
                      Package () { "mmc-hs400-1_8v", 0x1  },
                      Package () { "non-removable", 0x1  },
                }
            })
        }

        Device (COM1)
        {
            Name (_HID, "MRVL0001")                             // _HID: Hardware ID
            Name (_CID, "HISI0031")                             // _CID: Compatible ID
            Name (_UID, 0x00)                                   // _UID: Unique ID
            Method (_STA)                                       // _STA: Device status
            {
                Return (0xF)
            }
            Name (_ADR, FixedPcdGet64(PcdSerialRegisterBase))   // _ADR: Address
            Name (_CRS, ResourceTemplate ()                     // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    FixedPcdGet64(PcdSerialRegisterBase),       // Address Base
                    0x00000100,                                 // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  51
                }
            })
            Name (_DSD, Package () {
                ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package () {
                      Package () { "clock-frequency", FixedPcdGet32 (PcdSerialClockRate) },
                      Package () { "reg-io-width", 1 },
                      Package () { "reg-shift", 2 },
                }
            })
        }
    }
}
