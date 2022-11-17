/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2017, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2017, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

Scope(_SB)
{
    // L3T0 for S0_TB(DieID:3)
    Device (L300) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x90180000, // Min Base Address
        0x9018FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB30")
      {
        832,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ccl-id", 0x00},
      }
    })

  }
  // L3T1 for S0_TB(DieID:3)
  Device (L301) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 1) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x90190000, // Min Base Address
        0x9019FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB31")
      {
        833,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ccl-id", 0x01},
      }
    })

  }

  // L3T2 for S0_TB(DieID:3)
  Device (L302) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 2) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x901A0000, // Min Base Address
        0x901AFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB32")
      {
        834,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ccl-id", 0x02},
      }
    })

  }

  // L3T3 for S0_TB(DieID:3)
  Device (L303) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 3) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x901B0000, // Min Base Address
        0x901BFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB33")
      {
        835,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ccl-id", 0x03},
      }
    })

  }
  // L3T4 for S0_TB(DieID:3)
  Device (L304) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 4) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x901C0000, // Min Base Address
        0x901CFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB34")
      {
        836,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ccl-id", 0x04},
      }
    })

  }
  // L3T5 for S0_TB(DieID:3)
  Device (L305) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 5) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x901D0000, // Min Base Address
        0x901DFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB35")
      {
        837,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ccl-id", 0x05},
      }
    })

  }

  // DDRC0 for S0_TB(DieID:3)
  Device (DDR0) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x94D20000, // Min Base Address
        0x94D2FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB38")
      {
        844,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ch-id", 0x0},
      }
    })

  }
  // DDRC1 for S0_TB(DieID:3)
  Device (DDR1) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 1) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x94D30000, // Min Base Address
        0x94D3FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB39")
      {
        845,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ch-id", 0x1},
      }
    })

  }
  // DDRC2 for S0_TB(DieID:3)
  Device (DDR2) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 2) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x94D40000, // Min Base Address
        0x94D4FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB3A")
      {
        846,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ch-id", 0x2},
      }
    })

  }
  // DDRC3 for S0_TB(DieID:3)
  Device (DDR3) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 3) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x94D50000, // Min Base Address
        0x94D5FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB3B")
      {
        847,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03},
        Package () {"hisilicon,ch-id", 0x3},
      }
    })

  }

  // HHA0 for S0_TB(DieID:3)
  Device (HHA0) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 0)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x90120000, // Min Base Address
        0x9012FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB3C")
      {
        848,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03}
      }
    })
  }

  // HHA1 for S0_TB(DieID:3)
  Device (HHA1) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 1)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x90130000, // Min Base Address
        0x9013FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

    Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB3D")
    {
      849,
    }
  })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x03}
      }
    })
  }

  // L3T0 for S0_TA(DieID:1)
  Device (L308) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x08) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x98180000, // Min Base Address
        0x9818FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB10")
      {
        832,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ccl-id", 0x00},
      }
    })

  }
  // L3T1 for S0_TA(DieID:1)
  Device (L309) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x09) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x98190000, // Min Base Address
        0x9819FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB11")
      {
        833,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ccl-id", 0x01},
      }
    })

  }

  // L3T2 for S0_TA(DieID:1)
  Device (L30A) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x0A) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x981A0000, // Min Base Address
        0x981AFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB12")
      {
        834,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ccl-id", 0x02},
      }
    })

  }

  // L3T3 for S0_TA(DieID:1)
  Device (L30B) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x0B) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x981B0000, // Min Base Address
        0x981BFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB13")
      {
        835,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ccl-id", 0x03},
      }
    })

  }
  // L3T4 for S0_TA(DieID:1)
  Device (L30C) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x0C) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x981C0000, // Min Base Address
        0x981CFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB14")
      {
        836,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ccl-id", 0x04},
      }
    })

  }
  // L3T5 for S0_TA(DieID:1)
  Device (L30D) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x0D) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x981D0000, // Min Base Address
        0x981DFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB15")
      {
        837,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ccl-id", 0x05},
      }
    })

  }

  // DDRC0 for S0_TA(DieID:1)
  Device (DDR4) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 4) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x9CD20000, // Min Base Address
        0x9CD2FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB18")
      {
        844,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ch-id", 0x0},
      }
    })

  }
  // DDRC1 for S0_TA(DieID:1)
  Device (DDR5) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 5) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x9CD30000, // Min Base Address
        0x9CD3FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB19")
      {
        845,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ch-id", 0x1},
      }
    })

  }
  // DDRC2 for S0_TA(DieID:1)
  Device (DDR6) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 6) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x9CD40000, // Min Base Address
        0x9CD4FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB1A")
      {
        846,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ch-id", 0x2},
      }
    })

  }
  // DDRC3 for S0_TA(DieID:1)
  Device (DDR7) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 7) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x9CD50000, // Min Base Address
        0x9CD5FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB1B")
      {
        847,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01},
        Package () {"hisilicon,ch-id", 0x3},
      }
    })
  }

  // HHA0 for S0_TA(DieID:1)
  Device (HHA2) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 2)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x98120000, // Min Base Address
        0x9812FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB1C")
      {
        848,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01}
      }
    })
  }

  // HHA1 for S0_TA(DieID:1)
  Device (HHA3) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 3)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x98130000, // Min Base Address
        0x9813FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

    Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB1D")
    {
      849,
    }
  })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x01}
      }
    })
  }

  // It is the list PMU node of Socket1
  // L3T0 for S1_TB(DieID:7)
    Device (L310) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x10) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400090180000, // Min Base Address
        0x40009018FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB70")
      {
        832,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ccl-id", 0x00},
      }
    })

  }
  // L3T1 for S1_TB(DieID:7)
  Device (L311) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x11) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400090190000, // Min Base Address
        0x40009019FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB71")
      {
        833,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ccl-id", 0x01},
      }
    })

  }

  // L3T2 for S1_TB(DieID:7)
  Device (L312) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x12) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000901A0000, // Min Base Address
        0x4000901AFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB72")
      {
        834,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ccl-id", 0x02},
      }
    })

  }

  // L3T3 for S1_TB(DieID:7)
  Device (L313) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x13) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000901B0000, // Min Base Address
        0x4000901BFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB73")
      {
        835,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ccl-id", 0x03},
      }
    })

  }
  // L3T4 for S1_TB(DieID:7)
  Device (L314) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x14) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000901C0000, // Min Base Address
        0x4000901CFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB74")
      {
        836,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ccl-id", 0x04},
      }
    })

  }
  // L3T5 for S1_TB(DieID:7)
  Device (L315) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x15) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000901D0000, // Min Base Address
        0x4000901DFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB75")
      {
        837,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ccl-id", 0x05},
      }
    })

  }

  // DDRC0 for S1_TB(DieID:7)
  Device (DDR8) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 8) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400094D20000, // Min Base Address
        0x400094D2FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB78")
      {
        844,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ch-id", 0x0},
      }
    })

  }
  // DDRC1 for S1_TB(DieID:7)
  Device (DDR9) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 9) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400094D30000, // Min Base Address
        0x400094D3FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB79")
      {
        845,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ch-id", 0x1},
      }
    })

  }
  // DDRC2 for S1_TB(DieID:7)
  Device (DDRA) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0xA) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400094D40000, // Min Base Address
        0x400094D4FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB7A")
      {
        846,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ch-id", 0x2},
      }
    })

  }
  // DDRC3 for S1_TB(DieID:7)
  Device (DDRB) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0xB) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // DDRC address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400094D50000, // Min Base Address
        0x400094D5FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB7B")
      {
        847,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07},
        Package () {"hisilicon,ch-id", 0x3},
      }
    })

  }

  // HHA0 for S1_TB(DieID:7)
  Device (HHA4) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 4)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400090120000, // Min Base Address
        0x40009012FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB7C")
      {
        848,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07}
      }
    })
  }

  // HHA1 for S1_TB(DieID:7)
  Device (HHA5) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 5)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400090130000, // Min Base Address
        0x40009013FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

    Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB7D")
    {
      849,
    }
  })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x07}
      }
    })
  }

  // L3T0 for S1_TA(DieID:5)
  Device (L318) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x18) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400098180000, // Min Base Address
        0x40009818FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB50")
      {
        832,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ccl-id", 0x00},
      }
    })

  }
  // L3T1 for S1_TA(DieID:5)
  Device (L319) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x19) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400098190000, // Min Base Address
        0x40009819FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB51")
      {
        833,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ccl-id", 0x01},
      }
    })

  }

  // L3T2 for S1_TA(DieID:5)
  Device (L31A) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x1A) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000981A0000, // Min Base Address
        0x4000981AFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB52")
      {
        834,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ccl-id", 0x02},
      }
    })

  }

  // L3T3 for S1_TA(DieID:5)
  Device (L31B) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x1B) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000981B0000, // Min Base Address
        0x4000981BFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB53")
      {
        835,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ccl-id", 0x03},
      }
    })

  }
  // L3T4 for S1_TA(DieID:5)
  Device (L31C) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x1C) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000981C0000, // Min Base Address
        0x4000981CFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB54")
      {
        836,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ccl-id", 0x04},
      }
    })

  }
  // L3T5 for S1_TA(DieID:5)
  Device (L31D) {
    Name (_HID, "HISI0213") // _HID: Hardware ID
    Name (_UID, 0x1D) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x4000981D0000, // Min Base Address
        0x4000981DFFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB55")
      {
        837,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ccl-id", 0x05},
      }
    })

  }

  // DDRC0 for S1_TA(DieID:5)
  Device (DDRC) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0xC) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x40009CD20000, // Min Base Address
        0x40009CD2FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB58")
      {
        844,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ch-id", 0x0},
      }
    })

  }
  // DDRC1 for S1_TA(DieID:5)
  Device (DDRD) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0xD) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x40009CD30000, // Min Base Address
        0x40009CD3FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB59")
      {
        845,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ch-id", 0x1},
      }
    })

  }
  // DDRC2 for S1_TA(DieID:5)
  Device (DDRE) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0xE) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x40009CD40000, // Min Base Address
        0x40009CD4FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB5A")
      {
        846,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ch-id", 0x2},
      }
    })

  }
  // DDRC3 for S1_TA(DieID:5)
  Device (DDRF) {
    Name (_HID, "HISI0233") // _HID: Hardware ID
    Name (_UID, 0xF) // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // L3T address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x40009CD50000, // Min Base Address
        0x40009CD5FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB5B")
      {
        847,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05},
        Package () {"hisilicon,ch-id", 0x3},
      }
    })
  }

    // HHA0 for S1_TA(DieID:5)
  Device (HHA6) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 6)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400098120000, // Min Base Address
        0x40009812FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB5C")
      {
        848,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05}
      }
    })
  }
  // HHA1 for S0_TA(DieID:5)
  Device (HHA7) {
    Name (_HID, "HISI0243")  // _HID: Hardware ID
    Name (_UID, 7)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
      QWordMemory ( // HHA address base
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0, // Granularity
        0x400098130000, // Min Base Address
        0x40009813FFFF, // Max Base Address
        0x0, // Translate
        0x10000 // Length
      )

    Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MB5D")
    {
      849,
    }
  })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"hisilicon,scl-id", 0x05}
      }
    })
  }
}
