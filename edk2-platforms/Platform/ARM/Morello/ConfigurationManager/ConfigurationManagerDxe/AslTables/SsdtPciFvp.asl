/** @file
  Secondary System Description Table (SSDT)

  Copyright (c) 2021, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "ConfigurationManager.h"

#define LNK_DEVICE(Unique_Id, Link_Name, irq)                           \
  Device(Link_Name) {                                                   \
  Name(_HID, EISAID("PNP0C0F"))                                         \
  Name(_UID, Unique_Id)                                                 \
  Name(_PRS, ResourceTemplate() {                                       \
    Interrupt(ResourceProducer, Level, ActiveHigh, Exclusive) { irq }   \
    })                                                                  \
  Method (_CRS, 0) { Return (_PRS) }                                    \
  Method (_SRS, 1) { }                                                  \
  Method (_DIS) { }                                                     \
}

#define PRT_ENTRY(Address, Pin, Link)                                                   \
  Package (4) {                                                                         \
    Address,  /* uses the same format as _ADR */                                        \
    Pin,      /* The PCI pin number of the device (0-INTA, 1-INTB, 2-INTC, 3-INTD)  */  \
    Link,     /* Interrupt allocated via Link device  */                                \
    Zero      /* global system interrupt number (no used) */                            \
}

#define ROOT_PRT_ENTRY(Pin, Link)   PRT_ENTRY(0x0000FFFF, Pin, Link)  // Device 0 for Bridge.

DefinitionBlock("SsdtPci.aml", "SSDT", 1, "ARMLTD", "MORELLO",
                CFG_MGR_OEM_REVISION)
{
  Scope (_SB) {
    //
    // PCI Root Complex
    //
    LNK_DEVICE(1, LNKA, 201)
    LNK_DEVICE(2, LNKB, 202)
    LNK_DEVICE(3, LNKC, 203)
    LNK_DEVICE(4, LNKD, 204)

    // PCI Root Complex
    Device(PCI0) {
      Name (_HID, EISAID("PNP0A08")) // PCI Express Root Bridge
      Name (_CID, EISAID("PNP0A03")) // Compatible PCI Root Bridge
      Name (_SEG, Zero)              // PCI Segment Group number
      Name (_BBN, Zero)              // PCI Base Bus Number
      Name (_CCA, 1)                 // Cache Coherency Attribute

      // Root Complex 0
      Device (RP0) {
        Name(_ADR, 0xF0000000)    // Dev 0, Func 0
      }

      // PCI Routing Table
      Name(_PRT, Package() {
        ROOT_PRT_ENTRY(0, LNKA),   // INTA
        ROOT_PRT_ENTRY(1, LNKB),   // INTB
        ROOT_PRT_ENTRY(2, LNKC),   // INTC
        ROOT_PRT_ENTRY(3, LNKD),   // INTD
      })

      // Root complex resources
      Method (_CRS, 0, Serialized) {
        Name (RBUF, ResourceTemplate () {
          WordBusNumber (                             // Bus numbers assigned to this root
            ResourceProducer,
            MinFixed,
            MaxFixed,
            PosDecode,
            0,                                        // AddressGranularity
            FixedPcdGet32 (PcdPciBusMin),             // AddressMinimum - Minimum Bus Number
            FixedPcdGet32 (PcdPciBusMax),             // AddressMaximum - Maximum Bus Number
            0,                                        // AddressTranslation - Set to 0
            FixedPcdGet32 (PcdPciBusCount)            // RangeLength - Number of Busses
          )

          DWordMemory (                               // 32-bit BAR Windows
            ResourceProducer,
            PosDecode,
            MinFixed,
            MaxFixed,
            Cacheable,
            ReadWrite,
            0x00000000,                              // Granularity
            FixedPcdGet32 (PcdPciMmio32Base),        // Min Base Address
            FixedPcdGet32 (PcdPciMmio32MaxBase),     // Max Base Address
            FixedPcdGet32 (PcdPciMmio32Translation), // Translate
            FixedPcdGet32 (PcdPciMmio32Size)         // Length
          )

          QWordMemory (                               // 64-bit BAR Windows
            ResourceProducer,
            PosDecode,
            MinFixed,
            MaxFixed,
            Cacheable,
            ReadWrite,
            0x00000000,                              // Granularity
            FixedPcdGet64 (PcdPciMmio64Base),        // Min Base Address
            FixedPcdGet64 (PcdPciMmio64MaxBase),     // Max Base Address
            FixedPcdGet64 (PcdPciMmio64Translation), // Translate
            FixedPcdGet64 (PcdPciMmio64Size)         // Length
          )

          DWordIo (                                   // IO window
            ResourceProducer,
            MinFixed,
            MaxFixed,
            PosDecode,
            EntireRange,
            0x00000000,                              // Granularity
            FixedPcdGet32 (PcdPciIoBase),            // Min Base Address
            FixedPcdGet32 (PcdPciIoMaxBase),         // Max Base Address
            FixedPcdGet32 (PcdPciIoTranslation),     // Translate
            FixedPcdGet32 (PcdPciIoSize),            // Length
            ,
            ,
            ,
            TypeTranslation
          )
        }) // Name(RBUF)
        Return (RBUF)
      } // Method (_CRS)
    } // Device (PCI0)
  } // _SB
} // DB
