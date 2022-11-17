/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
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

#define PCI_OSC_SUPPORT_HOTPLUG() \
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
      /* Always allow native PME, AER (no dependencies) */ \
      /* Never allow SHPC (no SHPC controller in this system)*/ \
      And(CTRL,0x1D,CTRL) \
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
  Device (PCI0)
  {                                          // PCI0 indicate host bridge 0
    Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
    Name (_UID, 0)
    Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
    Name(_SEG, 0)                            // Segment of this Root complex
    Name (_BBN, 0x0)                         // Base Bus Number
    Name (_CCA, 1)                           // cache coherence attribute

    Name (_PRT, Package (){
      // INTx configure for RP0, whoes device number is 0
      // For ESL/FPGA debug, we should modify this according to
      // specific hardware configuration.
      Package () {0xFFFF,0,0,640},         // INT_A
      Package () {0xFFFF,1,0,641},         // INT_B
      Package () {0xFFFF,2,0,642},         // INT_C
      Package () {0xFFFF,3,0,643},         // INT_D

      // This is an example of RP1 INTx configure. Adding or not
      // adding RPx INTx configure deponds on hardware board topology,
      // if UEFI enables RPx, RPy, RPz... related INTx configure
      // should be added
      Package () {0x2FFFF,0,0,640},         // INT_A
      Package () {0x2FFFF,1,0,641},         // INT_B
      Package () {0x2FFFF,2,0,642},         // INT_C
      Package () {0x2FFFF,3,0,643},         // INT_D

      Package () {0x4FFFF,0,0,640},         // INT_A
      Package () {0x4FFFF,1,0,641},         // INT_B
      Package () {0x4FFFF,2,0,642},         // INT_C
      Package () {0x4FFFF,3,0,643},         // INT_D

      Package () {0x6FFFF,0,0,640},         // INT_A
      Package () {0x6FFFF,1,0,641},         // INT_B
      Package () {0x6FFFF,2,0,642},         // INT_C
      Package () {0x6FFFF,3,0,643},         // INT_D

      Package () {0x8FFFF,0,0,640},         // INT_A
      Package () {0x8FFFF,1,0,641},         // INT_B
      Package () {0x8FFFF,2,0,642},         // INT_C
      Package () {0x8FFFF,3,0,643},         // INT_D

      Package () {0xCFFFF,0,0,640},         // INT_A
      Package () {0xCFFFF,1,0,641},         // INT_B
      Package () {0xCFFFF,2,0,642},         // INT_C
      Package () {0xCFFFF,3,0,643},         // INT_D

      Package () {0xEFFFF,0,0,640},         // INT_A
      Package () {0xEFFFF,1,0,641},         // INT_B
      Package () {0xEFFFF,2,0,642},         // INT_C
      Package () {0xEFFFF,3,0,643},         // INT_D

      Package () {0x10FFFF,0,0,640},         // INT_A
      Package () {0x10FFFF,1,0,641},         // INT_B
      Package () {0x10FFFF,2,0,642},         // INT_C
      Package () {0x10FFFF,3,0,643},         // INT_D

      Package () {0x12FFFF,0,0,640},         // INT_A
      Package () {0x12FFFF,1,0,641},         // INT_B
      Package () {0x12FFFF,2,0,642},         // INT_C
      Package () {0x12FFFF,3,0,643},         // INT_D
      })

    Method (_CRS, 0, Serialized) {
      // Method is defined in 19.6.82 in ACPI 6.0 spec
      Name (RBUF, ResourceTemplate () {
        // 19.3.3 in ACPI 6.0 spec
        WordBusNumber (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          0,                                 // AddressGranularity
          0x00,                              // AddressMinimum - Minimum Bus Number
          0x3f,                              // AddressMaximum - Maximum Bus Number
          0,                                 // AddressTranslation - Set to 0
          0x40                               // RangeLength - Number of Busses
        )
        QWordMemory (                        // 64-bit prefetch BAR windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Prefetchable,
          ReadWrite,
          0x0,                               // Granularity
          0x80000000000,                     // Min Base Address pci address
          0x83fffffffff,                     // Max Base Address
          0x0,                               // Translate
          0x4000000000                       // Length, 256G
        )
        QWordMemory (                        // 32-bit non-prefetch BAR windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          Cacheable,
          ReadWrite,
          0x0,                               // Granularity
          0xe0000000,                        // Min Base Address pci address
          0xeffeffff,                        // Max Base Address
          0x0,                               // Translate
          0xfff0000                          // Length, 256M - 64K
        )
        QWordIO (
          ResourceProducer,
          MinFixed,
          MaxFixed,
          PosDecode,
          EntireRange,
          0x0,                               // Granularity
          0x0,                               // Min Base Address
          0xffff,                            // Max Base Address
          0xefff0000,                        // Translate
          0x10000                            // Length, 64K
        )}
      )                                      // Name(RBUF)
      Return (RBUF)
    }                                        // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT_HOTPLUG ()

  Method (_HPX, 0) {
    Return (Package(2) {
      Package(6) {    // PCI Setting Record
        0x00,         // Type 0
        0x01,         // Revision 1
        0x08,         // CacheLineSize in DWORDS
        0x40,         // LatencyTimer in PCI clocks
        0x01,         // Enable SERR (Boolean)
        0x01          // Enable PERR (Boolean)
      },

      Package(18){   // PCI-X Setting Record
        0x02,        // Type 2
        0x01,        // Revision 1
        0xFFFFFFFF,  // Uncorrectable Error Mask Register AND Mask, Keep ->1
        0x00000000,  // Uncorrectable Error Mask Register OR Mask, keep ->0
        0xFFFFFFFF,  // Uncorrectable Error Severity Register AND Mask
        0x00000000,  // Uncorrectable Error Severity Register OR Mask
        0xFFFFFFFF,  // Correctable Error Mask Register AND Mask
        0x00000000,  // Correctable Error Mask Register OR Mask
        0xFFFFFFFF,  // Advanced Error Capabilities and Control Register AND Mask
        0x00000000,  // Advanced Error Capabilities and Control Register OR Mask
        0xFFF7,      // Device Control Register AND Mask
        0x0007,      // Device Control Register OR Mask
        0xFFFF,      // Link Control Register AND Mask
        0x0000,      // Link Control Register OR Mask
        0xFFFFFFFF,  // Secondary Uncorrectable Error Severity Register AND Mask
        0x00000000,  // Secondary Uncorrectable Error Severity Register OR Mask
        0xFFFFFFFF,  // Secondary Uncorrectable Error Mask Register AND Mask
        0x00000000   // Secondary Uncorrectable Error Mask Register OR Mask
      }
    })
  }

  Method (_STA, 0x0, NotSerialized) {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x00)
  }
} // Device(PCI0)


Device (PCI1)
{                                            // PCI1 indicate host bridge 1
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 1)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0x7b)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                          // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0x7b,                              // AddressMinimum - Minimum Bus Number
        0x7b,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x1                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows, where to show this ??
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x148800000,                       // Min Base Address pci address ??
        0x148ffffff,                       // Max Base Address
        0x0,                               // Translate
        0x800000                           // Length, 8M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x00)
  }
} // Device(PCI1)

Device (PCI2)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 2)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0x7a)                         // Base Bus Number
  Name(_CCA, 1)                            // cache coherence attribute ??
  Name (_PRT, Package (){
    Package () {0xFFFF,0,0,640},         // INT_A
    Package () {0xFFFF,1,0,641},         // INT_B
    Package () {0xFFFF,2,0,642},         // INT_C
    Package () {0xFFFF,3,0,643},         // INT_D
    Package () {0x1FFFF,0,0,640},         // INT_A
    Package () {0x1FFFF,1,0,641},         // INT_B
    Package () {0x1FFFF,2,0,642},         // INT_C
    Package () {0x1FFFF,3,0,643},         // INT_D
    Package () {0x2FFFF,0,0,640},         // INT_A
    Package () {0x2FFFF,1,0,641},         // INT_B
    Package () {0x2FFFF,2,0,642},         // INT_C
    Package () {0x2FFFF,3,0,643},         // INT_D
  })
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource //                               setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0x7a,                              // AddressMinimum - Minimum Bus Number
        0x7a,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x1                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x20c000000,                       // Min Base Address pci address
        0x20c1fffff,                       // Max Base Address
        0x0,                               // Translate
        0x200000                           // Length, 2M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method(_DMA, 0, Serialized)
  {
    Return (ResourceTemplate()
    {
      QWORDMemory(
      ResourceConsumer,
      PosDecode,          // _DEC
      MinFixed,           // _MIF
      MaxFixed,           // _MAF
      Prefetchable,       // _MEM
      ReadWrite,          // _RW
      0,                  // _GRA
      0x00000000,         // _MIN
      0xFFFFFFFF,         // _MAX
      0x00000000,        // _TRA
      0x100000000,         // _LEN
      ,
      ,
      ,
      )
    })
  }

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x00)
  }
}

Device (PCI3)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 3)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0x78)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0x78,                              // AddressMinimum - Minimum Bus Number
        0x79,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x2                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x208000000,                       // Min Base Address pci address
        0x208ffffff,                       // Max Base Address
        0x0,                               // Translate
        0x1000000                          // Length, 16M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x00)
  }
}

Device (PCI4)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 4)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0x7c)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0x7c,                              // AddressMinimum - Minimum Bus Number
        0x7d,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x2                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x120000000,                       // Min Base Address pci address
        0x13fffffff,                       // Max Base Address
        0x0,                               // Translate
        0x20000000                         // Length, 512M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0x0F)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x00)
  }
}

Device (PCI5)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 5)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0x74)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??

  Name (_PRT, Package (){
    Package () {0x2FFFF,0,0,640},         // INT_A
    Package () {0x2FFFF,1,0,641},         // INT_B
    Package () {0x2FFFF,2,0,642},         // INT_C
    Package () {0x2FFFF,3,0,643},         // INT_D
    Package () {0x3FFFF,0,0,640},         // INT_A
    Package () {0x3FFFF,1,0,641},         // INT_B
    Package () {0x3FFFF,2,0,642},         // INT_C
    Package () {0x3FFFF,3,0,643},         // INT_D
  })

  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
      ResourceProducer,
      MinFixed,
      MaxFixed,
      PosDecode,
      0,                                 // AddressGranularity
      0x74,                              // AddressMinimum - Minimum Bus Number
      0x76,                              // AddressMaximum - Maximum Bus Number
      0,                                 // AddressTranslation - Set to 0
      0x3                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
      ResourceProducer,
      PosDecode,
      MinFixed,
      MaxFixed,
      Prefetchable,
      ReadWrite,
      0x0,                               // Granularity
      0x144000000,                       // Min Base Address pci address
      0x147ffffff,                       // Max Base Address
      0x0,                               // Translate
      0x4000000                          // Length, 32M
      )
      QWordMemory (                        // 32-bit non-prefetch BAR Windows
      ResourceProducer,
      PosDecode,
      MinFixed,
      MaxFixed,
      Cacheable,
      ReadWrite,
      0x0,                               // Granularity
      0xa2000000,                        // Min Base Address pci address
      0xa2ffffff,                        // Max Base Address
      0x0,                               // Translate
      0x1000000                          // Length, 16M
      )
    })                                 // Name(RBUF)
    Return (RBUF)
  }                                    // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Device (SAS0)
  {
    Name (_ADR, 0x00020000)
    Name (_DSD, Package ()
    {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package ()
      {
        Package (2) {"sas-addr", Package() {0x50, 0x01, 0x88, 0x20, 0x16, 0x00, 0x00, 0x00}},
        Package ()  {"queue-count", 16},
        Package ()  {"phy-count", 8},
      }
    })

    OperationRegion (CTL, SystemMemory, 0x140070000, 0x1000)
    Field (CTL, DWordAcc, NoLock, Preserve)
    {
      Offset (0xa18),
      RST, 32,
      DRST, 32,
    }

    OperationRegion (TXD, SystemMemory, 0xA2000000, 0x4000)
    Field (TXD, DwordAcc, NoLock, Preserve)
    {
      Offset (0x2350),  //port0
      ST00, 32,   //0x2350
      ST01, 32,   //0x2354
      ST02, 32,   //0x2358
      ST03, 32,   //0x235c
      ST04, 32,   //0x2360
      ST05, 32,   //0x2364
      ST06, 32,   //0x2368
      ST07, 32,   //0x236c
      Offset (0x2750),  //port1
      ST10, 32,   //0x2750
      ST11, 32,   //0x2754
      ST12, 32,   //0x2758
      ST13, 32,   //0x275c
      ST14, 32,   //0x2760
      ST15, 32,   //0x2764
      ST16, 32,   //0x2768
      ST17, 32,   //0x276c
      Offset (0x2b50),  //port2
      ST20, 32,   //0x2b50
      ST21, 32,   //0x2b54
      ST22, 32,   //0x2b58
      ST23, 32,   //0x2b5c
      ST24, 32,   //0x2b60
      ST25, 32,   //0x2b64
      ST26, 32,   //0x2b68
      ST27, 32,   //0x2b6c
      Offset (0x2f50),  //port3
      ST30, 32,   //0x2f50
      ST31, 32,   //0x2f54
      ST32, 32,   //0x2f58
      ST33, 32,   //0x2f5c
      ST34, 32,   //0x2f60
      ST35, 32,   //0x2f64
      ST36, 32,   //0x2f68
      ST37, 32,   //0x2f6c
      Offset (0x3350),  //port4
      ST40, 32,   //0x3350
      ST41, 32,   //0x3354
      ST42, 32,   //0x3358
      ST43, 32,   //0x335c
      ST44, 32,   //0x3360
      ST45, 32,   //0x3364
      ST46, 32,   //0x3368
      ST47, 32,   //0x336c
      Offset (0x3750),//port5
      ST50, 32,   //0x3750
      ST51, 32,   //0x3754
      ST52, 32,   //0x3758
      ST53, 32,   //0x375c
      ST54, 32,   //0x3760
      ST55, 32,   //0x3764
      ST56, 32,   //0x3768
      ST57, 32,   //0x376c
      Offset (0x3b50),  //port6
      ST60, 32,   //0x3b50
      ST61, 32,   //0x3b54
      ST62, 32,   //0x3b58
      ST63, 32,   //0x3b5c
      ST64, 32,   //0x3b60
      ST65, 32,   //0x3b64
      ST66, 32,   //0x3b68
      ST67, 32,   //0x3b6c
      Offset (0x3f50),  //port7
      ST70, 32,   //0x3f50
      ST71, 32,   //0x3f54
      ST72, 32,   //0x3f58
      ST73, 32,   //0x3f5c
      ST74, 32,   //0x3f60
      ST75, 32,   //0x3f64
      ST76, 32,   //0x3f68
      ST77, 32    //0x3f6c
    }

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7FFFFFF, RST)
      Sleep(1)
      Store(0x7FFFFFF, DRST)
      Sleep(1)

      //port0
      Store (0x8D04, ST00)
      Sleep(1)
      Store (0x8D04, ST01)
      Sleep(1)
      Store (0x8D04, ST02)
      Sleep(1)
      Store (0x8D04, ST03)
      Sleep(1)
      Store (0x8D04, ST05)
      Sleep(1)
      Store (0x8D04, ST06)
      Sleep(1)
      Store (0x8D04, ST07)
      Sleep(1)

      //port1
      Store (0x8D04, ST10)
      Sleep(1)
      Store (0x8D04, ST11)
      Sleep(1)
      Store (0x8D04, ST12)
      Sleep(1)
      Store (0x8D04, ST13)
      Sleep(1)
      Store (0x8D04, ST15)
      Sleep(1)
      Store (0x8D04, ST16)
      Sleep(1)
      Store (0x8D04, ST17)
      Sleep(1)

      //port2
      Store (0x8D04, ST20)
      Sleep(1)
      Store (0x8D04, ST21)
      Sleep(1)
      Store (0x8D04, ST22)
      Sleep(1)
      Store (0x8D04, ST23)
      Sleep(1)
      Store (0x8D04, ST25)
      Sleep(1)
      Store (0x8D04, ST26)
      Sleep(1)
      Store (0x8D04, ST27)
      Sleep(1)

      //port3
      Store (0x8D04, ST30)
      Sleep(1)
      Store (0x8D04, ST31)
      Sleep(1)
      Store (0x8D04, ST32)
      Sleep(1)
      Store (0x8D04, ST33)
      Sleep(1)
      Store (0x8D04, ST35)
      Sleep(1)
      Store (0x8D04, ST36)
      Sleep(1)
      Store (0x8D04, ST37)
      Sleep(1)

      //port4
      Store (0x8D04, ST40)
      Sleep(1)
      Store (0x8D04, ST41)
      Sleep(1)
      Store (0x8D04, ST42)
      Sleep(1)
      Store (0x8D04, ST43)
      Sleep(1)
      Store (0x8D04, ST45)
      Sleep(1)
      Store (0x8D04, ST46)
      Sleep(1)
      Store (0x8D04, ST47)
      Sleep(1)

      //port5
      Store (0x8D04, ST50)
      Sleep(1)
      Store (0x8D04, ST51)
      Sleep(1)
      Store (0x8D04, ST52)
      Sleep(1)
      Store (0x8D04, ST53)
      Sleep(1)
      Store (0x8D04, ST55)
      Sleep(1)
      Store (0x8D04, ST56)
      Sleep(1)
      Store (0x8D04, ST57)
      Sleep(1)

      //port6
      Store (0x8D04, ST60)
      Sleep(1)
      Store (0x8D04, ST61)
      Sleep(1)
      Store (0x8D04, ST62)
      Sleep(1)
      Store (0x8D04, ST63)
      Sleep(1)
      Store (0x8D04, ST65)
      Sleep(1)
      Store (0x8D04, ST66)
      Sleep(1)
      Store (0x8D04, ST67)
      Sleep(1)

      //port7
      Store (0x8D04, ST70)
      Sleep(1)
      Store (0x8D04, ST71)
      Sleep(1)
      Store (0x8D04, ST72)
      Sleep(1)
      Store (0x8D04, ST73)
      Sleep(1)
      Store (0x8D04, ST75)
      Sleep(1)
      Store (0x8D04, ST76)
      Sleep(1)
      Store (0x8D04, ST77)
      Sleep(1)
    }
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x00)
  }
}

Device (PCI6)
{                                            // PCI0 indicate host bridge 0
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 6)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0x80)                          // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??

  Name (_PRT, Package (){
    // INTx configure for RP0, whoes device number is 0
    // For ESL/FPGA debug, we should modify this according to
    // specific hardware configuration.
    Package () {0xFFFF,0,0,640},         // INT_A
    Package () {0xFFFF,1,0,641},         // INT_B
    Package () {0xFFFF,2,0,642},         // INT_C
    Package () {0xFFFF,3,0,643},         // INT_D

    // This is an example of RP1 INTx configure. Adding or not
    // adding RPx INTx configure deponds on hardware board topology,
    // if UEFI enables RPx, RPy, RPz... related INTx configure
    // should be added
    Package () {0x2FFFF,0,0,640},         // INT_A
    Package () {0x2FFFF,1,0,641},         // INT_B
    Package () {0x2FFFF,2,0,642},         // INT_C
    Package () {0x2FFFF,3,0,643},         // INT_D

    Package () {0x4FFFF,0,0,640},         // INT_A
    Package () {0x4FFFF,1,0,641},         // INT_B
    Package () {0x4FFFF,2,0,642},         // INT_C
    Package () {0x4FFFF,3,0,643},         // INT_D

    Package () {0x6FFFF,0,0,640},         // INT_A
    Package () {0x6FFFF,1,0,641},         // INT_B
    Package () {0x6FFFF,2,0,642},         // INT_C
    Package () {0x6FFFF,3,0,643},         // INT_D

    Package () {0x8FFFF,0,0,640},         // INT_A
    Package () {0x8FFFF,1,0,641},         // INT_B
    Package () {0x8FFFF,2,0,642},         // INT_C
    Package () {0x8FFFF,3,0,643},         // INT_D

    Package () {0xCFFFF,0,0,640},         // INT_A
    Package () {0xCFFFF,1,0,641},         // INT_B
    Package () {0xCFFFF,2,0,642},         // INT_C
    Package () {0xCFFFF,3,0,643},         // INT_D

    Package () {0xEFFFF,0,0,640},         // INT_A
    Package () {0xEFFFF,1,0,641},         // INT_B
    Package () {0xEFFFF,2,0,642},         // INT_C
    Package () {0xEFFFF,3,0,643},         // INT_D

    Package () {0x10FFFF,0,0,640},         // INT_A
    Package () {0x10FFFF,1,0,641},         // INT_B
    Package () {0x10FFFF,2,0,642},         // INT_C
    Package () {0x10FFFF,3,0,643},         // INT_D

    Package () {0x12FFFF,0,0,640},         // INT_A
    Package () {0x12FFFF,1,0,641},         // INT_B
    Package () {0x12FFFF,2,0,642},         // INT_C
    Package () {0x12FFFF,3,0,643},         // INT_D
    })

  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0x80,                              // AddressMinimum - Minimum Bus Number
        0x9f,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x20                               // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit prefetch BAR windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x480000000000,                     // Min Base Address pci address
        0x483fffffffff,                     // Max Base Address
        0x0,                               // Translate
        0x4000000000                       // Length, 256G
      )
      QWordMemory (                        // 32-bit non-prefetch BAR windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Cacheable,
        ReadWrite,
        0x0,                               // Granularity
        0xf0000000,                        // Min Base Address pci address
        0xfffeffff,                        // Max Base Address
        0x0,                               // Translate
        0xfff0000                          // Length, 256M - 64K
      )
      QWordIO (
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        EntireRange,
        0x0,                               // Granularity
        0x0,                               // Min Base Address
        0xffff,                            // Max Base Address
        0xffff0000,                        // Translate
        0x10000                            // Length, 64K
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT_HOTPLUG ()

  Method (_HPX, 0) {
    Return (Package(2) {
      Package(6) { // PCI Setting Record
        0x00, // Type 0
        0x01, // Revision 1
        0x08, // CacheLineSize in DWORDS
        0x40, // LatencyTimer in PCI clocks
        0x01, // Enable SERR (Boolean)
        0x01  // Enable PERR (Boolean)
       },

       Package(18){ // PCI-X Setting Record
       0x02,      // Type 2
       0x01,      // Revision 1
       0xFFFFFFFF,  // Uncorrectable Error Mask Register AND Mask, Keep ->1
       0x00000000,  // Uncorrectable Error Mask Register OR Mask, keep ->0
       0xFFFFFFFF,  // Uncorrectable Error Severity Register AND Mask
       0x00000000,  // Uncorrectable Error Severity Register OR Mask
       0xFFFFFFFF,  // Correctable Error Mask Register AND Mask
       0x00000000,  // Correctable Error Mask Register OR Mask
       0xFFFFFFFF,  // Advanced Error Capabilities and Control Register AND Mask
       0x00000000,  // Advanced Error Capabilities and Control Register OR Mask
       0xFFF7,    // Device Control Register AND Mask
       0x0007,    // Device Control Register OR Mask
       0xFFFF,    // Link Control Register AND Mask
       0x0000,    // Link Control Register OR Mask
       0xFFFFFFFF,  // Secondary Uncorrectable Error Severity Register AND Mask
       0x00000000,  // Secondary Uncorrectable Error Severity Register OR Mask
       0xFFFFFFFF,  // Secondary Uncorrectable Error Mask Register AND Mask
       0x00000000   // Secondary Uncorrectable Error Mask Register OR Mask
     }
   })
 }

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x02)
  }
} // Device(PCI6)


Device (PCI7)
{                                          // PCI1 indicate host bridge 1
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 7)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0xbb)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0xbb,                              // AddressMinimum - Minimum Bus Number
        0xbb,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x1                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows, where to show this ??
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x400148800000,                       // Min Base Address pci address ??
        0x400148ffffff,                       // Max Base Address
        0x0,                               // Translate
        0x800000                           // Length, 8M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x02)
  }
} // Device(PCI7)

Device (PCI8)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 8)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0xba)                         // Base Bus Number
  Name(_CCA, 1)                            // cache coherence attribute ??
  Name (_PRT, Package (){
    Package () {0xFFFF,0,0,640},         // INT_A
    Package () {0xFFFF,1,0,641},         // INT_B
    Package () {0xFFFF,2,0,642},         // INT_C
    Package () {0xFFFF,3,0,643},         // INT_D
    Package () {0x1FFFF,0,0,640},         // INT_A
    Package () {0x1FFFF,1,0,641},         // INT_B
    Package () {0x1FFFF,2,0,642},         // INT_C
    Package () {0x1FFFF,3,0,643},         // INT_D
    Package () {0x2FFFF,0,0,640},         // INT_A
    Package () {0x2FFFF,1,0,641},         // INT_B
    Package () {0x2FFFF,2,0,642},         // INT_C
    Package () {0x2FFFF,3,0,643},         // INT_D
  })

  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource //                               setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0xba,                              // AddressMinimum - Minimum Bus Number
        0xba,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x1                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x40020c000000,                       // Min Base Address pci address
        0x40020c1fffff,                       // Max Base Address
        0x0,                               // Translate
        0x200000                           // Length, 2M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method(_DMA, 0, Serialized)
  {
    Return (ResourceTemplate()
    {
      QWORDMemory(
      ResourceConsumer,
      PosDecode,          // _DEC
      MinFixed,           // _MIF
      MaxFixed,           // _MAF
      Prefetchable,       // _MEM
      ReadWrite,          // _RW
      0,                  // _GRA
      0x00000000,         // _MIN
      0xFFFFFFFF,         // _MAX
      0x00000000,        // _TRA
      0x100000000,         // _LEN
      ,
      ,
      ,
      )
    })
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x02)
  }
}// Device(PCI8)

Device (PCI9)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 9)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0xb8)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource //                               setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0xb8,                              // AddressMinimum - Minimum Bus Number
        0xb9,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x2                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x400208000000,                       // Min Base Address pci address
        0x400208ffffff,                       // Max Base Address
        0x0,                               // Translate
        0x1000000                          // Length, 16M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x02)
  }
}// Device(PCI9)

Device (PCIA)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 0xA)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0xbc)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??
  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0xbc,                              // AddressMinimum - Minimum Bus Number
        0xbd,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x2                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x400120000000,                       // Min Base Address pci address
        0x40013fffffff,                       // Max Base Address
        0x0,                               // Translate
        0x20000000                         // Length, 512M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0x0F)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x02)
  }
}// Device(PCIA)

Device (PCIB)
{
  Name (_HID, "PNP0A08")                   // PCI Express Root Bridge
  Name (_UID, 0xB)
  Name (_CID, "PNP0A03")                   // Compatible PCI Root Bridge, Compatible ID
  Name(_SEG, 0)                            // Segment of this Root complex
  Name(_BBN, 0xb4)                         // Base Bus Number ??
  Name(_CCA, 1)                            // cache coherence attribute ??

  Name (_PRT, Package (){
    Package () {0x2FFFF,0,0,640},         // INT_A
    Package () {0x2FFFF,1,0,641},         // INT_B
    Package () {0x2FFFF,2,0,642},         // INT_C
    Package () {0x2FFFF,3,0,643},         // INT_D
    Package () {0x3FFFF,0,0,640},         // INT_A
    Package () {0x3FFFF,1,0,641},         // INT_B
    Package () {0x3FFFF,2,0,642},         // INT_C
    Package () {0x3FFFF,3,0,643},         // INT_D
  })

  Method (_CRS, 0, Serialized) {           // Root complex resources, _CRS: current resource setting
    Name (RBUF, ResourceTemplate () {      // Name: 19.6.87, ResourceTemplate: 19.6.111,
      WordBusNumber (                      // Bus numbers assigned to this root,
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        0,                                 // AddressGranularity
        0xb4,                              // AddressMinimum - Minimum Bus Number
        0xb6,                              // AddressMaximum - Maximum Bus Number
        0,                                 // AddressTranslation - Set to 0
        0x3                                // RangeLength - Number of Busses
      )
      QWordMemory (                        // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Prefetchable,
        ReadWrite,
        0x0,                               // Granularity
        0x400144000000,                       // Min Base Address pci address
        0x400147ffffff,                       // Max Base Address
        0x0,                               // Translate
        0x4000000                          // Length, 32M
      )
      QWordMemory (                        // 32-bit non-prefetch BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        Cacheable,
        ReadWrite,
        0x0,                                // Granularity
        0xa3000000,                        // Min Base Address pci address
        0xa3ffffff,                        // Max Base Address
        0x0,                                // Translate
        0x1000000                          // Length, 16M
      )
    })                                      // Name(RBUF)
    Return (RBUF)
  }                                         // Method(_CRS), this method return RBUF!

  PCI_OSC_SUPPORT ()

  Method (_STA, 0x0, NotSerialized)
  {
    Return (0xf)
  }

  Method (_PXM, 0, NotSerialized)
  {
    Return(0x02)
  }
}

Device (RESP)  //reserve for ecam resource
  {
    Name (_HID, "PNP0C02")
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, 0xd0000000, 0x10000000)
    })
  }
}
