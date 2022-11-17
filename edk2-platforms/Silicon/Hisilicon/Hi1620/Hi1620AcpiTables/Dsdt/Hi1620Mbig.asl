/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  //This is for S0-TB-L3T0 PMU implementation
  Device(MB30) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x30)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-L3T1 PMU implementation
  Device(MB31) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x31)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-L3T2 PMU implementation
  Device(MB32) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x32)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-L3T3 PMU implementation
  Device(MB33) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x33)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-L3T4 PMU implementation
  Device(MB34) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x34)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-L3T5 PMU implementation
  Device(MB35) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x35)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-DDRC0 PMU implementation
  Device(MB38) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x38)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-DDRC1 PMU implementation
  Device(MB39) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x39)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-DDRC2 PMU implementation
  Device(MB3A) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x3A)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-DDRC3 PMU implementation
  Device(MB3B) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x3B)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-HHA0 PMU implementation
  Device(MB3C) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x3C)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TB-HHA1 PMU implementation
  Device(MB3D) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x3D)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xA8080000,
          0xA808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-L3T0 PMU implementation
  Device(MB10) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x10)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-L3T1 PMU implementation
  Device(MB11) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x11)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-L3T2 PMU implementation
  Device(MB12) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x12)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-L3T3 PMU implementation
  Device(MB13) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x13)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }


  //This is for S0-TA-L3T4 PMU implementation
  Device(MB14) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x14)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-L3T5 PMU implementation
  Device(MB15) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x15)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-DDRC0 PMU implementation
  Device(MB18) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x18)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-DDRC1 PMU implementation
  Device(MB19) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x19)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-DDRC2 PMU implementation
  Device(MB1A) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x1A)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-DDRC3 PMU implementation
  Device(MB1B) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x1B)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-HHA0 PMU implementation
  Device(MB1C) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x1C)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S0-TA-HHA1 PMU implementation
  Device(MB1D) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x1D)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0xAC080000,
          0xAC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-L3T0 PMU implementation
  Device(MB70) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x70)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-L3T1 PMU implementation
  Device(MB71) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x71)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-L3T2 PMU implementation
  Device(MB72) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x72)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-L3T3 PMU implementation
  Device(MB73) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x73)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }


  //This is for S1-TB-L3T4 PMU implementation
  Device(MB74) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x74)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-L3T5 PMU implementation
  Device(MB75) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x75)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-DDRC0 PMU implementation
  Device(MB78) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x78)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-DDRC1 PMU implementation
  Device(MB79) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x79)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-DDRC2 PMU implementation
  Device(MB7A) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x7A)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-DDRC3 PMU implementation
  Device(MB7B) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x7B)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-HHA0 PMU implementation
  Device(MB7C) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x7C)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TB-HHA1 PMU implementation
  Device(MB7D) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x7D)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000A8080000,
          0x4000A808ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-L3T0 PMU implementation
  Device(MB50) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x50)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-L3T1 PMU implementation
  Device(MB51) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x51)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-L3T2 PMU implementation
  Device(MB52) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x52)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-L3T3 PMU implementation
  Device(MB53) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x53)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }


  //This is for S1-TA-L3T4 PMU implementation
  Device(MB54) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x54)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-L3T5 PMU implementation
  Device(MB55) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x55)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-DDRC0 PMU implementation
  Device(MB58) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x58)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-DDRC1 PMU implementation
  Device(MB59) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x59)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-DDRC2 PMU implementation
  Device(MB5A) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x5A)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-DDRC3 PMU implementation
  Device(MB5B) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x5B)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-HHA0 PMU implementation
  Device(MB5C) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x5C)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }

  //This is for S1-TA-HHA1 PMU implementation
  Device(MB5D) {
    Name(_HID, "HISI0152")
    Name(_UID, 0x5D)
    Name(_CRS, ResourceTemplate() {
      QWordMemory (
          ResourceConsumer,
          ,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x4000AC080000,
          0x4000AC08ffff,
          0x0,
          0x10000
      )
  })

   Name(_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
          Package () {"num-pins", 1}
        }
   })
  }
}
