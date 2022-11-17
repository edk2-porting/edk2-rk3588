/** @file

  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>
  Copyright (C) 2019, Marvell International Ltd. and its affiliates.<BR>
  Copyright (C) 2021, Semihalf.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Cn913xCEx7Eval/Dbg2.h"
#include "Cn913xCEx7Eval/Pcie.h"
#include "IcuInterrupts.h"

DefinitionBlock ("Cn9130CEx7EvalSsdt.aml", "SSDT", 2, "MRVL", "CN913X", 3)
{
    Scope (_SB)
    {
        Device (MMC1)
        {
            Name (_HID, "MRVL0004")     // _HID: Hardware ID
            Name (_UID, 0x01)           // _UID: Unique ID
            Name (_CCA, 0x01)           // _CCA: Cache Coherency Attribute
            Method (_STA)               // _STA: Device status
            {
                Return (0xF)
            }

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xF2780000,         // Address Base (MMIO)
                    0x00000300,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  CP_GIC_SPI_CP0_SDMMC
                }
            })
            Name (_DSD, Package () {
                ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package () {
                      Package () { "clock-frequency", 400000000 },
                      Package () { "bus-width", 4 },
                      Package () { "no-1-8-v", 0x1 },
                      Package () { "broken-cd", 0x1 },
                }
            })
        }

        Device (XHC0)
        {
            Name (_HID, "PNP0D10")      // _HID: Hardware ID
            Name (_UID, 0x00)           // _UID: Unique ID
            Name (_CCA, 0x01)           // _CCA: Cache Coherency Attribute
            Method (_STA)               // _STA: Device status
            {
                Return (0xF)
            }

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xF2500000,         // Address Base (MMIO)
                    0x00004000,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  CP_GIC_SPI_CP0_USB_H0
                }
            })
        }

        Device (XHC1)
        {
            Name (_HID, "PNP0D10")      // _HID: Hardware ID
            Name (_UID, 0x01)           // _UID: Unique ID
            Name (_CCA, 0x01)           // _CCA: Cache Coherency Attribute
            Method (_STA)               // _STA: Device status
            {
                Return (0xF)
            }

            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xF2510000,         // Address Base (MMIO)
                    0x00004000,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  CP_GIC_SPI_CP0_USB_H1
                }
            })
        }

        Device (COM2)
        {
            Name (_HID, "MRVL0001")                             // _HID: Hardware ID
            Name (_CID, "HISI0031")                             // _CID: Compatible ID
            Name (_UID, 0x01)                                   // _UID: Unique ID
            Method (_STA)                                       // _STA: Device status
            {
                Return (0xF)
            }
            Name (_ADR, CN913X_DBG2_UART_REG_BASE)              // _ADR: Address
            Name (_CRS, ResourceTemplate ()                     // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    CN913X_DBG2_UART_REG_BASE,                  // Address Base
                    0x00000100,                                 // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                  CP_GIC_SPI_CP0_UART2
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

        Device (SMI0)
        {
            Name (_HID, "MRVL0100")                             // _HID: Hardware ID
            Name (_UID, 0x00)                                   // _UID: Unique ID
            Method (_STA)                                       // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite,
                    0xf212a200,                                 // Address Base
                    0x00000010,                                 // Address Length
                    )
            })
            Device (PHY0)
            {
                Name (_ADR, 0x0)
            }
        }

        Device (PP20)
        {
            Name (_HID, "MRVL0110")                             // _HID: Hardware ID
            Name (_CCA, 0x01)                                   // Cache-coherent controller
            Name (_UID, 0x00)                                   // _UID: Unique ID
            Method (_STA)                                       // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite, 0xf2000000 , 0x100000)
                Memory32Fixed (ReadWrite, 0xf2129000 , 0xb000)
                Memory32Fixed (ReadWrite, 0xf2220000 , 0x800)
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
                    CP_GIC_SPI_PP2_CP0_PORT0
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
            Device (ETH1)
            {
              Name (_ADR, 0x0)
              Name (_CRS, ResourceTemplate ()
              {
                  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                  {
                    CP_GIC_SPI_PP2_CP0_PORT1
                  }
              })
              Name (_DSD, Package () {
                  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                  Package () {
                    Package () { "port-id", 1 },
                    Package () { "gop-port-id", 2 },
                    Package () { "phy-mode", "rgmii-id"},
                    Package () { "phy-handle", \_SB.SMI0.PHY0},
                  }
              })
            }
            Device (ETH2)
            {
              Name (_ADR, 0x0)
              Name (_CRS, ResourceTemplate ()
              {
                  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                  {
                    CP_GIC_SPI_PP2_CP0_PORT2
                  }
              })
              Name (_DSD, Package () {
                  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                  Package () {
                    Package () { "port-id", 2 },
                    Package () { "gop-port-id", 3 },
                    Package () { "phy-mode", "2500base-x"},
                  },
                  ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
                  Package () {
                    Package () {"fixed-link", "LNK0"}
                  }
              })
              Name (LNK0, Package(){ // Data-only subnode of port
                  ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                  Package () {
                    Package () {"speed", 2500},
                    Package () {"full-duplex", 1}
                  }
              })
            }
        }

        Device (RNG0)
        {
            Name (_HID, "PRP0001")                                 // _HID: Hardware ID
            Name (_UID, 0x00)                                      // _UID: Unique ID
            Method (_STA)                                          // _STA: Device status
            {
                Return (0xF)
            }
            Name (_CRS, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite, 0xF2760000, 0x7D)
                Interrupt (ResourceConsumer, Level, ActiveHigh, Shared)
                {
                  CP_GIC_SPI_CP0_EIP_RNG0
                }
            })
            Name (_DSD, Package () {
                ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package () {
                    Package () { "compatible", "inside-secure,safexcel-eip76" },
                }
            })
        }

        //
        // PCIe Root Bus
        //
        Device (PCI0)
        {
            Name (_HID, "PNP0A08" /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, "PNP0A03" /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, 0x00)  // _SEG: PCI Segment
            Name (_BBN, 0x00)  // _BBN: BIOS Bus Number
            Name (_UID, 0x00)  // _UID: Unique ID
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
                        CP0_PCI0_BUS_MIN,                   // Range Minimum
                        CP0_PCI0_BUS_MAX,                   // Range Maximum
                        0x0000,                             // Translation Offset
                        CP0_PCI0_BUS_COUNT                  // Length
                        )
                    DWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x00000000,                         // Granularity
                        CP0_PCI0_MMIO32_BASE,               // Range Minimum
                        CP0_PCI0_MMIO32_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP0_PCI0_MMIO32_SIZE                // Length
                        )
                    QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x0000000000000000,                 // Granularity
                        CP0_PCI0_MMIO64_BASE,               // Range Minimum
                        CP0_PCI0_MMIO64_MAX,                // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP0_PCI0_MMIO64_SIZE                // Length
                        )
                    QWordIo (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                        0x00000000,                         // Granularity
                        CP0_PCI0_IO_BASE,                   // Range Minimum
                        0x0000FFFF,                         // Range Maximum
                        CP0_PCI0_IO_TRANSLATION,            // Translation Address
                        CP0_PCI0_IO_SIZE,                   // Length
                        ,
                        ,
                        ,
                        TypeTranslation
                        )
                })
                Return (RBUF) /* \_SB_.PCI0._CRS.RBUF */
            } // Method(_CRS)

            Device (RES0)
            {
                Name (_HID, "PNP0C02")
                Name (_CRS, ResourceTemplate ()
                {
                    QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                        0x0000000000000000,                 // Granularity
                        CP0_PCI0_ECAM_BASE,                 // Range Minimum
                        CP0_PCI0_ECAM_MAX,                  // Range Maximum
                        0x00000000,                         // Translation Offset
                        CP0_PCI0_ECAM_SIZE                  // Length
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
                    Store (CDW2, SUPP) /* \_SB_.PCI0.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI0.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI0.CTRL */
                    }

                    And (CTRL, 0x1D, CTRL) /* \_SB_.PCI0.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI0._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI0._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI0._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI0._OSC.CDW1 */
                    Return (Arg3)
                }
            } // Method(_OSC)
        }
    }
}
