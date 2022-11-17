/** @file
*
*  Copyright (c) 2018 Hisilicon Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

//
// GPIO
//

//#include "ArmPlatform.h"
Scope(_SB)
{
Device(GPO0) {
  Name(_HID, "HISI0181")
  Name(_ADR, 0)
  Name(_UID, 0)

  Name (_CRS, ResourceTemplate ()  {
    //Memory32Fixed (ReadWrite, 0x602E0000, 0x10000)
    QWordMemory (
      ResourceConsumer,
      ,
      MinFixed,
      MaxFixed,
      NonCacheable,
      ReadWrite,
      0x0,
      0x201120000,
      0x20112ffff,
      0x0,
      0x10000
    )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive,,,)
    {
      476,
    }
  })

  Device(PRTa) {
    Name(_ADR, 0)
    Name(_UID, 0)
    Name(_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 0},
        Package () {"snps,nr-gpios", 32},
      }
    })
  }
}

/**
*I2C for 100k release
**/
Device(I2C0) {
  Name(_HID, "HISI02A2")
  Name(_ADR, 0)
  Name(_UID, 0)
  Name(_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () {"i2c-sda-falling-time-ns", 913},
      Package () {"i2c-scl-falling-time-ns", 303},
      Package () {"i2c-sda-hold-time-ns", 1000},
      Package () {"clock-frequency", 100000},
    }
  })

  Name (_CRS, ResourceTemplate ()  {
    //Memory32Fixed (ReadWrite, 0x602E0000, 0x10000)
    QWordMemory (
      ResourceConsumer,
      ,
      MinFixed,
      MaxFixed,
      NonCacheable,
      ReadWrite,
      0x0,
      0x201160000,
      0x20116ffff,
      0x0,
      0x10000
    )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive,,,)
    {
      480,
    }
  })
}


/**
*I2C for 100k vtof
**/
Device(I2C2) {
  Name(_HID, "HISI0182")
  Name(_ADR, 0)
  Name(_UID, 0)
  Name(_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () {"i2c-sda-falling-time-ns", 913},
      Package () {"i2c-scl-falling-time-ns", 303},
      Package () {"i2c-sda-hold-time-ns", 1000},
      Package () {"clock-frequency", 100000},
    }
    })

  Name (_CRS, ResourceTemplate ()  {
    //Memory32Fixed (ReadWrite, 0x602E0000, 0x10000)
    QWordMemory (
      ResourceConsumer,
      ,
      MinFixed,
      MaxFixed,
      NonCacheable,
      ReadWrite,
      0x0,
      0x201160000,
      0x20116ffff,
      0x0,
      0x10000
    )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive,,,)
    {
      480,
    }
  })
}

/**
*I2C for 400k fpga
**/
Device(I2C3) {
  Name(_HID, "HISI0183")
  Name(_ADR, 0)
  Name(_UID, 0)
  Name(_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () {"i2c-sda-falling-time-ns", 300},
      Package () {"i2c-scl-falling-time-ns", 100},
      Package () {"i2c-sda-hold-time-ns", 250},
      Package () {"clock-frequency", 400000},
    }
    })

  Name (_CRS, ResourceTemplate ()  {
  //Memory32Fixed (ReadWrite, 0x602E0000, 0x10000)
  QWordMemory (
    ResourceConsumer,
    ,
    MinFixed,
    MaxFixed,
    NonCacheable,
    ReadWrite,
    0x0,
    0x201160000,
    0x20116ffff,
    0x0,
    0x10000
  )
  Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive,,,)
  {
    480,
  }
  })
}

Device(LPC) {
  Name(_HID, "HISI0191")
  Name(_ADR, 0)
  Name(_UID, 0)
  Name(_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
    }
  })

  Name (_CRS, ResourceTemplate ()  {
  Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive,,,)
  {
    484,
    490
  }
  })
}

Device(NAD) {
  Name(_HID, "HISI0192")
  Name(_ADR, 0)
  Name(_UID, 0)
  Name(_CCA, 1)
  Name(_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () {"nand-bus-width", 8},
      Package () {"nand-ecc-mode", "hw"},
      Package () {"nand-ecc-strength", 24},
      Package () {"nand-ecc-step-size", 1024},
    }
  })

  Name (_CRS, ResourceTemplate ()  {
    //Memory32Fixed (ReadWrite, 0x602E0000, 0x10000)
    QWordMemory (
      ResourceConsumer,
      ,
      MinFixed,
      MaxFixed,
      NonCacheable,
      ReadWrite,
      0x0,
      0x206220000,
      0x20622ffff,
      0x0,
      0x10000
    )

    QWordMemory (
      ResourceConsumer,
      ,
      MinFixed,
      MaxFixed,
      NonCacheable,
      ReadWrite,
      0x0,
      0x206210000,
      0x20621ffff,
      0x0,
      0x10000
    )

    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive,,,)
    {
      483,
    }
  })
}
}
