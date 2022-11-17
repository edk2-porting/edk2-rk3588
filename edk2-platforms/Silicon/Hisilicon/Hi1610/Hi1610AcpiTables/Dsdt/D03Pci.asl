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

Scope(_SB)
{
  // PCIe Root bus
  Device (PCI0)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 0) // Segment of this Root complex
    Name(_BBN, 0) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x0, // AddressMinimum - Minimum Bus Number
          0x1f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x20 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xb2000000, // Min Base Address pci address
          0xb7feffff, // Max Base Address
          0x0, // Translate
          0x5ff0000 // Length
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
          0xb7ff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    PCI_OSC_SUPPORT()

    Device (RES0)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        Memory32Fixed (ReadWrite, 0xa0090000 , 0x10000)
      })
    }

  } // Device(PCI0)

  Device (RES0)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02") // Motherboard reserved resource
    Name (_UID, 0x0)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xa0090000 , 0x10000)
    })
  }

  // PCIe Root bus
  Device (PCI1)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 1) // Segment of this Root complex
    Name(_BBN, 0xe0) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0xe0, // AddressMinimum - Minimum Bus Number
          0xff, // AddressMaximum - Maximum Bus Number
          0,   // AddressTranslation - Set to 0
          0x20 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xb8000000, // Min Base Address pci address
          0xbdfeffff, // Max Base Address
          0x0, // Translate
          0x5ff0000 // Length
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
          0xbdff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    PCI_OSC_SUPPORT()

    Device (RES1)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CID, "PNP0C02") // Motherboard reserved resource
      Name (_UID, 0x1)  //  Unique ID
      Name (_CRS, ResourceTemplate (){
        Memory32Fixed (ReadWrite, 0xa0200000 , 0x10000)
      })
    }


  } // Device(PCI1)

  Device (RES1)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xa0200000 , 0x10000)
    })
  }

  // PCIe Root bus
  Device (PCI2)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 2) // Segment of this Root complex
    Name(_BBN, 0x80) // Base Bus Number
    Name(_CCA, 1)
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0, // AddressGranularity
          0x80, // AddressMinimum - Minimum Bus Number
          0x9f, // AddressMaximum - Maximum Bus Number
          0, // AddressTranslation - Set to 0
          0x20 // RangeLength - Number of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0, // Granularity
          0xaa000000, // Min Base Address
          0xaffeffff, // Max Base Address
          0x0, // Translate
          0x5ff0000 // Length
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
          0xafff0000, // Translate
          0x10000 // Length
        )
      }) // Name(RBUF)
      Return (RBUF)
    } // Method(_CRS)
    PCI_OSC_SUPPORT()

    Device (RES2)
    {
      Name (_HID, "HISI0081") // HiSi PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        Memory32Fixed (ReadWrite, 0xa00a0000, 0x10000)
      })
    }

  } // Device(PCI2)

  Device (RES2)
  {
    Name (_HID, "HISI0081") // HiSi PCIe RC config base address
    Name (_CID, "PNP0C02") // Motherboard reserved resource
    Name (_UID, 0x2)  // Unique ID
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xa00a0000, 0x10000)
    })
  }

  Device (RESP)  //reserve for ecam resource
  {
    Name (_HID, "PNP0C02")
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xb0000000, 0x2000000) //ECAM space for PCI0 [bus 00-1f]
      Memory32Fixed (ReadWrite, 0xbe000000, 0x2000000) //ECAM space for PCI1 [bus e0-ff]
      Memory32Fixed (ReadWrite, 0xa8000000, 0x2000000) //ECAM space for PCI2 [bus 80-9f]
    })
  }
}

