/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/
*
**/

//#include "ArmPlatform.h"
Scope(_SB)
{
  /* 0xD000E014:Hi1616 chip version reg[19:8], 0x102-after EC, 0x101/0-before EC. */
  OperationRegion (ECRA, SystemMemory, 0xD000E014, 0x4)
  Field (ECRA, AnyAcc, NoLock, Preserve)
  {
    VECA, 32,
  }

  /* RBYV:Return by chip version
   * the pcie device should be disable for chip's reason before EC,
   * and the pcie device should be enable after EC for OS */
  Method (RBYV)
  {
    Store(VECA, local0)
    And (local0, 0xFFF00, local1)
    If (LEqual (local1, 0x10200)) {
      Return (0xf)
    } Else {
      Return (0x0)
    }
  }

/*
  See ACPI 6.1 Spec, 6.2.11, PCI Firmware Spec 3.0, 4.5
*/
#define PCI_OSC_SUPPORT() \
  Name(SUPP, Zero) /* PCI _OSC Support Field value */ \
  Name(CTRL, Zero) /* PCI _OSC Control Field value */ \
  Method(_OSC,4) { \
    If(LEqual(Arg0,ToUUID("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) { \
      /* Create DWord-adressable fields from the Capabilities Buffer */ \
      CreateDWordField(Arg3,0,CDW1) \
      CreateDWordField(Arg3,4,CDW2) \
      CreateDWordField(Arg3,8,CDW3) \
      /* Save Capabilities DWord2 & 3 */ \
      Store(CDW2,SUPP) \
      Store(CDW3,CTRL) \
      /* Only allow native hot plug control if OS supports: */ \
      /* ASPM */ \
      /* Clock PM */ \
      /* MSI/MSI-X */ \
      If(LNotEqual(And(SUPP, 0x16), 0x16)) { \
        And(CTRL,0x1E,CTRL) \
      }\
      \
      /* Do not allow native PME, AER */ \
      /* Never allow SHPC (no SHPC controller in this system)*/ \
      And(CTRL,0x10,CTRL) \
      If(LNotEqual(Arg1,One)) { /* Unknown revision */ \
        Or(CDW1,0x08,CDW1) \
      } \
      \
      If(LNotEqual(CDW3,CTRL)) { /* Capabilities bits were masked */ \
        Or(CDW1,0x10,CDW1) \
      } \
      \
      /* Update DWORD3 in the buffer */ \
      Store(CTRL,CDW3) \
      Return(Arg3) \
    } Else { \
      Or(CDW1,4,CDW1) /* Unrecognized UUID */ \
      Return(Arg3) \
    } \
  } // End _OSC

  // 1P NA PCIe2
  Device (PCI2)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 2) // Segment of this Root complex
    Name(_BBN, 0xF8) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0xF8, // AddressMinimum - Minimum Bus Number
          0xFF, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x8 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xa8000000, // Min Base Address
          0xaf7effff, // Max Base Address
          0x0, // Translate
          0x77f0000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0xaf7f0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RES2)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        Memory32Fixed (ReadWrite, 0xa00a0000 , 0x10000)
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (0xf)
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x00)
    }
  } // Device(PCI2)

  Device (RES2)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0x2)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xa00a0000 , 0x10000) //host bridge register space
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (0xf)
    }
  }

  Device (R1NA) // reserve 1p NA ECAM resource
  {
    Name (_HID, "PNP0C02")  // Motherboard reserved resource
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xaf800000 , 0x800000) //ECAM space for [bus f8-ff]
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (0xf)
    }
  }

  // 1p NB PCIe0
  Device (PCI4)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 4) // Segment of this Root complex
    Name(_BBN, 0x88) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x88, // AddressMinimum - Minimum Bus Number
          0x8f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x8 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xa9000000, // Min Base Address
          0xabfeffff, // Max Base Address
          0x800000000, // Translate
          0x2ff0000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x8abff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RES4)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x8a0090000, // Min Base Address
          0x8a009ffff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
       )
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x01)
    }
  } // Device(PCI4)
  Device (RES4)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0x4)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
        QwordMemory ( //host bridge register space
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8a0090000, // Min Base Address
        0x8a009ffff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }

  // 1P NB PCI1
  Device (PCI5)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 5) // Segment of this Root complex
    Name(_BBN, 0x78) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x78, // AddressMinimum - Minimum Bus Number
          0x7f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x8 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xb0000000, // Min Base Address
          0xb77effff, // Max Base Address
          0x800000000, // Translate
          0x77f0000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x8b77f0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RES5)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x8a0200000, // Min Base Address
          0x8a020ffff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
       )
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x01)
    }
  } // Device(PCI5)
  Device (RES5)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0x5)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      QwordMemory (  //host bridge register space
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8a0200000, // Min Base Address
        0x8a020ffff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }

  // 1P NB PCIe2
  Device (PCI6)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 0x6) // Segment of this Root complex
    Name(_BBN, 0xc0) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0xc0, // AddressMinimum - Minimum Bus Number
          0xc7, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x8 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xac900000, // Min Base Address
          0xaffeffff, // Max Base Address
          0x800000000, // Translate
          0x36f0000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x8afff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RES6)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x8a00a0000, // Min Base Address
          0x8a00affff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
    )
     })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x01)
    }
  } // Device(PCI6)
  Device (RES6)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0x6)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      QwordMemory ( //host bridge register space
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8a00a0000, // Min Base Address
        0x8a00affff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }
  // 1P NB PCIe3
  Device (PCI7)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 0x7) // Segment of this Root complex
    Name(_BBN, 0x90) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x90, // AddressMinimum - Minimum Bus Number
          0x97, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x8 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xb9800000, // Min Base Address
          0xbffeffff, // Max Base Address
          0x800000000, // Translate
          0x67f0000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x8bfff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RES7)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x8a00b0000, // Min Base Address
          0x8a00bffff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
        )
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x01)
    }
  } // Device(PCI7)
  Device (RES7)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0x7)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      QwordMemory (  //host bridge register space
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8a00b0000, // Min Base Address
        0x8a00bffff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }

  Device (R1NB) // reserve 1p NB ECAM resource
  {
    Name (_HID, "PNP0C02")  // Motherboard reserved resource
    Name (_CRS, ResourceTemplate (){
      QwordMemory ( //ECAM space for [bus 88-8f]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8a8800000, // Min Base Address
        0x8a8ffffff, // Max Base Address
        0x0, // Translate
        0x800000 // Length
      )
      QwordMemory ( //ECAM space for [bus 78-7f]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8b7800000, // Min Base Address
        0x8b7ffffff, // Max Base Address
        0x0, // Translate
        0x800000 // Length
      )
      QwordMemory (  //ECAM space for [bus c0-c7]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8ac000000, // Min Base Address
        0x8ac7fffff, // Max Base Address
        0x0, // Translate
        0x800000 // Length
      )
      QwordMemory (   //ECAM space for [bus 90-97]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x8b9000000, // Min Base Address
        0x8b97fffff, // Max Base Address
        0x0, // Translate
        0x800000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }
  // 2P NA PCIe2
  Device (PCIa)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 0xa) // Segment of this Root complex
    Name(_BBN, 0x10) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x10, // AddressMinimum - Minimum Bus Number
          0x1f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x10 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0x40000000, // Min Base Address
          0xefffffff, // Max Base Address
          0x65000000000, // Translate
          0xb0000000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x67fffff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RESa)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x600a00a0000, // Min Base Address
          0x600a00affff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
        )
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (0xf)
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x02)
    }
  } // Device(PCIa)
  Device (RESa)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0xa)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      QwordMemory ( //host bridge register space
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x600a00a0000, // Min Base Address
        0x600a00affff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (0xf)
    }
  }

  Device (R2NA) //reserve for 2p NA ecam resource
  {
    Name (_HID, "PNP0C02") // Motherboard reserved resource
    Name (_CRS, ResourceTemplate (){
      QwordMemory ( //ECAM space for [bus 10-1f]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x64001000000, // Min Base Address
        0x64001ffffff, // Max Base Address
        0x0, // Translate
        0x1000000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (0xf)
    }
  }
  // 2P NB PCIe0
  Device (PCIc)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 0xc) // Segment of this Root complex
    Name(_BBN, 0x20) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x20, // AddressMinimum - Minimum Bus Number
          0x2f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x10 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0x40000000, // Min Base Address
          0xefffffff, // Max Base Address
          0x75000000000, // Translate
          0xb0000000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x77fffff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RESc)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x700a0090000, // Min Base Address
          0x700a009ffff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
        )
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x03)
    }
  } // Device(PCIc)

  Device (RESc)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0xc)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      QwordMemory ( //host bridge register space
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x700a0090000, // Min Base Address
        0x700a009ffff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }
  //2P NB PCIe1
  Device (PCId)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 0xd) // Segment of this Root complex
    Name(_BBN, 0x30) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x30, // AddressMinimum - Minimum Bus Number
          0x3f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x10 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0x40000000, // Min Base Address
          0xefffffff, // Max Base Address
          0x79000000000, // Translate
          0xB0000000 // Length
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0, // Granularity
          0x0, // Min Base Address
          0xffff, // Max Base Address
          0x7bfffff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    Device (RESd)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        QwordMemory (
          ResourceConsumer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0, // Granularity
          0x700a0200000, // Min Base Address
          0x700a020ffff, // Max Base Address
          0x0, // Translate
          0x10000 // Length
        )
      })
    }
    PCI_OSC_SUPPORT()
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x03)
    }
  } // Device(PCId)
  Device (RESd)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02")  // Motherboard reserved resource
    Name (_UID, 0xd)  //  Unique ID
    Name (_CRS, ResourceTemplate (){ //host bridge register space
      QwordMemory (
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x700a0200000, // Min Base Address
        0x700a020ffff, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }

  Device (R2NB) //reserve for 2p NB ecam resource
  {
    Name (_HID, "PNP0C02")  // Motherboard reserved resource
    Name (_CRS, ResourceTemplate (){
      QwordMemory (  //ECAM space for [bus 20-2f]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x74002000000, // Min Base Address
        0x74002ffffff, // Max Base Address
        0x0, // Translate
        0x1000000 // Length
      )
      QwordMemory (  //ECAM space for [bus 30-3f]
        ResourceConsumer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x78003000000, // Min Base Address
        0x78003ffffff, // Max Base Address
        0x0, // Translate
        0x1000000 // Length
      )
    })
    Method (_STA, 0x0, NotSerialized)
    {
      Return (RBYV())
    }
  }
}

