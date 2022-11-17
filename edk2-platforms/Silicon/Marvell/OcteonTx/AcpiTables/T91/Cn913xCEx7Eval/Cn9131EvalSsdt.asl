/** @file

  Secondary System Description Table Fields (SSDT)

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019, Marvell International Ltd. and its affiliates.<BR>
  Copyright (C) 2021, Semihalf.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Cn913xCEx7Eval/Pcie.h"
#include "IcuInterrupts.h"

DefinitionBlock ("Cn9131CEx7EvalSsdt.aml", "SSDT", 2, "MRVL", "CN913X", 3)
{
    Scope (_SB)
    {
        Device (AHC0)
        {
            Name (_HID, "LNRO001E")     // _HID: Hardware ID
            Name (_UID, 0x00)           // _UID: Unique ID
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

        Device (XSM1)
        {
            Name (_HID, "MRVL0101")                             // _HID: Hardware ID
            Name (_UID, 0x00)                                   // _UID: Unique ID
            Method (_STA)                                       // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite,
                    0xf412a600,                                 // Address Base
                    0x00000010,                                 // Address Length
                    )
            })
            Device (PHY0)
            {
                Name (_ADR, 0x0)
                Name (_DSD, Package () {
                    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                    Package () {
                        Package () { "compatible", "ethernet-phy-ieee802.3-c45" },
                    }
                })
            }
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
                    Package () { "phy-mode", "5gbase-r"},
                    Package () { "phy-handle", \_SB.XSM1.PHY0},
                  }
              })
            }
        }

        Device (RNG1)
        {
            Name (_HID, "PRP0001")                                 // _HID: Hardware ID
            Name (_UID, 0x01)                                      // _UID: Unique ID
            Method (_STA)                                          // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite, 0xF4760000, 0x7D)
                Interrupt (ResourceConsumer, Level, ActiveHigh, Shared)
                {
                  CP_GIC_SPI_CP1_EIP_RNG0
                }
            })
            Name (_DSD, Package () {
                ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package () {
                    Package () { "compatible", "inside-secure,safexcel-eip76" },
                }
            })
        }

        Device (PCI1)
        {
            Name (_HID, "PNP0A08" /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, "PNP0A03" /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, 0x01)  // _SEG: PCI Segment
            Name (_BBN, 0x00)  // _BBN: BIOS Bus Number
            Name (_UID, 0x01)  // _UID: Unique ID
            Name (_CCA, 0x01)  // _CCA: Cache Coherency Attribute
            Method (_STA)      // _STA: Device status
            {
                Return (0xF)
            }
            Name (_PRT, Package ()  // _PRT: PCI Routing Table
            {
                Package () { 0xFFFF, 0x0, 0x0, 0x40 },
                Package () { 0xFFFF, 0x1, 0x0, 0x40 },
                Package () { 0xFFFF, 0x2, 0x0, 0x40 },
                Package () { 0xFFFF, 0x3, 0x0, 0x40 }
            })

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                        0x0000,                             // Granularity
                        CP1_PCI0_BUS_MIN,                   // Range Minimum
                        CP1_PCI0_BUS_MAX,                   // Range Maximum
                        0x0000,                             // Translation Offset
                        CP1_PCI0_BUS_COUNT                  // Length
                        )
                    DWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x00000000,                         // Granularity
                        CP1_PCI0_MMIO32_BASE,               // Range Minimum
                        CP1_PCI0_MMIO32_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP1_PCI0_MMIO32_SIZE                // Length
                        )
                    QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x0000000000000000,                 // Granularity
                        CP1_PCI0_MMIO64_BASE,               // Range Minimum
                        CP1_PCI0_MMIO64_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP1_PCI0_MMIO64_SIZE                // Length
                        )
                    DWordIo (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                        0x00000000,                         // Granularity
                        CP1_PCI0_IO_BASE,                   // Range Minimum
                        0x0000FFFF,                         // Range Maximum
                        CP1_PCI0_IO_TRANSLATION,            // Translation Address
                        CP1_PCI0_IO_SIZE,                   // Length
                        ,
                        ,
                        ,
                        TypeTranslation
                        )
                })
                Return (RBUF) /* \_SB_.PCI1._CRS.RBUF */
            } // Method(_CRS)

            Device (RES0)
            {
                Name (_HID, "PNP0C02")
                Name (_CRS, ResourceTemplate ()
                {
                    Memory32Fixed (ReadWrite,
                        CP1_PCI0_ECAM_BASE,                 // Range Minimum
                        CP1_PCI0_ECAM_SIZE                  // Length
                        )
                })
            }
            Name (SUPP, 0x00)
            Name (CTRL, 0x00)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                CreateDWordField (Arg3, 0x00, CDW1)
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI1.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI1.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI1.CTRL */
                    }

                    And (CTRL, 0x1D, CTRL) /* \_SB_.PCI1.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI1._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI1._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI1._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI1._OSC.CDW1 */
                    Return (Arg3)
                }
            } // Method(_OSC)
        }

        Device (PCI2)
        {
            Name (_HID, "PNP0A08" /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, "PNP0A03" /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, 0x02)  // _SEG: PCI Segment
            Name (_BBN, 0x00)  // _BBN: BIOS Bus Number
            Name (_UID, 0x02)  // _UID: Unique ID
            Name (_CCA, 0x01)  // _CCA: Cache Coherency Attribute
            Method (_STA)      // _STA: Device status
            {
                Return (0xF)
            }
            Name (_PRT, Package ()  // _PRT: PCI Routing Table
            {
                Package () { 0xFFFF, 0x0, 0x0, 0x40 },
                Package () { 0xFFFF, 0x1, 0x0, 0x40 },
                Package () { 0xFFFF, 0x2, 0x0, 0x40 },
                Package () { 0xFFFF, 0x3, 0x0, 0x40 }
            })

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                        0x0000,                             // Granularity
                        CP1_PCI1_BUS_MIN,                   // Range Minimum
                        CP1_PCI1_BUS_MAX,                   // Range Maximum
                        0x0000,                             // Translation Offset
                        CP1_PCI1_BUS_COUNT                  // Length
                        )
                    DWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x00000000,                         // Granularity
                        CP1_PCI1_MMIO32_BASE,               // Range Minimum
                        CP1_PCI1_MMIO32_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP1_PCI1_MMIO32_SIZE                // Length
                        )
                    QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x0000000000000000,                 // Granularity
                        CP1_PCI1_MMIO64_BASE,               // Range Minimum
                        CP1_PCI1_MMIO64_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP1_PCI1_MMIO64_SIZE                // Length
                        )
                    DWordIo (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                        0x00000000,                         // Granularity
                        CP1_PCI1_IO_BASE,                   // Range Minimum
                        0x0000FFFF,                         // Range Maximum
                        CP1_PCI1_IO_TRANSLATION,            // Translation Address
                        CP1_PCI1_IO_SIZE,                   // Length
                        ,
                        ,
                        ,
                        TypeTranslation
                        )
                })
                Return (RBUF) /* \_SB_.PCI2._CRS.RBUF */
            } // Method(_CRS)

            Device (RES0)
            {
                Name (_HID, "PNP0C02")
                Name (_CRS, ResourceTemplate ()
                {
                    Memory32Fixed (ReadWrite,
                        CP1_PCI1_ECAM_BASE,                 // Range Minimum
                        CP1_PCI1_ECAM_SIZE                  // Length
                        )
                })
            }
            Name (SUPP, 0x00)
            Name (CTRL, 0x00)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                CreateDWordField (Arg3, 0x00, CDW1)
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI2.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI2.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI2.CTRL */
                    }

                    And (CTRL, 0x1D, CTRL) /* \_SB_.PCI2.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI2._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI2._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI2._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI2._OSC.CDW1 */
                    Return (Arg3)
                }
            } // Method(_OSC)
        }

        Device (PCI3)
        {
            Name (_HID, "PNP0A08" /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, "PNP0A03" /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, 0x03)  // _SEG: PCI Segment
            Name (_BBN, 0x00)  // _BBN: BIOS Bus Number
            Name (_UID, 0x03)  // _UID: Unique ID
            Name (_CCA, 0x01)  // _CCA: Cache Coherency Attribute
            Method (_STA)      // _STA: Device status
            {
                Return (0xF)
            }
            Name (_PRT, Package ()  // _PRT: PCI Routing Table
            {
                Package () { 0xFFFF, 0x0, 0x0, 0x40 },
                Package () { 0xFFFF, 0x1, 0x0, 0x40 },
                Package () { 0xFFFF, 0x2, 0x0, 0x40 },
                Package () { 0xFFFF, 0x3, 0x0, 0x40 }
            })

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                        0x0000,                             // Granularity
                        CP1_PCI2_BUS_MIN,                   // Range Minimum
                        CP1_PCI2_BUS_MAX,                   // Range Maximum
                        0x0000,                             // Translation Offset
                        CP1_PCI2_BUS_COUNT                  // Length
                        )
                    DWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x00000000,                         // Granularity
                        CP1_PCI2_MMIO32_BASE,               // Range Minimum
                        CP1_PCI2_MMIO32_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP1_PCI2_MMIO32_SIZE                // Length
                        )
                    QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x0000000000000000,                 // Granularity
                        CP1_PCI2_MMIO64_BASE,               // Range Minimum
                        CP1_PCI2_MMIO64_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP1_PCI2_MMIO64_SIZE                // Length
                        )
                    DWordIo (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                        0x00000000,                         // Granularity
                        CP1_PCI2_IO_BASE,                   // Range Minimum
                        0x0000FFFF,                         // Range Maximum
                        CP1_PCI2_IO_TRANSLATION,            // Translation Address
                        CP1_PCI2_IO_SIZE,                   // Length
                        ,
                        ,
                        ,
                        TypeTranslation
                        )
                })
                Return (RBUF) /* \_SB_.PCI3._CRS.RBUF */
            } // Method(_CRS)

            Device (RES0)
            {
                Name (_HID, "PNP0C02")
                Name (_CRS, ResourceTemplate ()
                {
                    Memory32Fixed (ReadWrite,
                        CP1_PCI2_ECAM_BASE,                 // Range Minimum
                        CP1_PCI2_ECAM_SIZE                  // Length
                        )
                })
            }
            Name (SUPP, 0x00)
            Name (CTRL, 0x00)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                CreateDWordField (Arg3, 0x00, CDW1)
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI3.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI3.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI3.CTRL */
                    }

                    And (CTRL, 0x1D, CTRL) /* \_SB_.PCI3.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI3._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI3._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI3._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI3._OSC.CDW1 */
                    Return (Arg3)
                }
            } // Method(_OSC)
        }
    }
}
