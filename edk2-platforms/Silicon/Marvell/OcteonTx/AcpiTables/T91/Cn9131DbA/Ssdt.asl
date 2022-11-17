/** @file

  Secondary System Description Table Fields (SSDT)

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019, Marvell International Ltd. and its affiliates.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "IcuInterrupts.h"

DefinitionBlock ("Cn9131DbASsdt.aml", "SSDT", 2, "MVEBU ", "CN9131", 3)
{
    Scope (_SB)
    {
        Device (AHC1)
        {
            Name (_HID, "LNRO001E")     // _HID: Hardware ID
            Name (_UID, 0x01)           // _UID: Unique ID
            Name (_CCA, 0x01)           // _CCA: Cache Coherency Attribute
            Method (_STA)               // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CLS, Package (0x03)  // _CLS: Class Code
            {
                0x01,
                0x06,
                0x01
            })

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xF4540000,         // Address Base (MMIO)
                    0x00030000,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  CP_GIC_SPI_CP1_SATA_H0
                }
            })
        }

        Device (XHC2)
        {
            Name (_HID, "PNP0D10")      // _HID: Hardware ID
            Name (_UID, 0x02)           // _UID: Unique ID
            Name (_CCA, 0x01)           // _CCA: Cache Coherency Attribute
            Method (_STA)               // _STA: Device status
            {
                Return (0xF)
            }

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xF4510000,         // Address Base (MMIO)
                    0x00004000,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  CP_GIC_SPI_CP1_USB_H1
                }
            })
        }
        Device (PP21)
        {
            Name (_HID, "MRVL0110")                             // _HID: Hardware ID
            Name (_CCA, 0x01)                                   // Cache-coherent controller
            Name (_UID, 0x01)                                   // _UID: Unique ID
            Method (_STA)                                       // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite, 0xf4000000 , 0x100000)
                Memory32Fixed (ReadWrite, 0xf4129000 , 0xb000)
                Memory32Fixed (ReadWrite, 0xf4220000 , 0x800)
            })
            Name (_DSD, Package () {
                ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package () {
                  Package () { "clock-frequency", 333333333 },
                }
            })
            Device (ETH0)
            {
              Name (_ADR, 0x0)
              Name (_CRS, ResourceTemplate ()
              {
                  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                  {
                    CP_GIC_SPI_PP2_CP1_PORT0
                  }
              })
              Name (_DSD, Package () {
                  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                  Package () {
                    Package () { "port-id", 0 },
                    Package () { "gop-port-id", 0 },
                    Package () { "phy-mode", "10gbase-kr"},
                    Package () { "managed", "in-band-status"},
                  }
              })
            }
        }
    }
}
