/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

  Device (I2C1) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 1)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfea90000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 350 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C2) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 2)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeaa0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 351 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C3) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeab0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 352 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C4) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 4)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeac0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 353 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C5) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 5)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfead0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 354 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C6) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 6)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfec80000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 355 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C7) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 7)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfec90000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 356 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }

  Device (I2C8) {
    Name (_HID, "RKCP3001")
    Name (_CID, "PRP0001")
    Name (_UID, 8)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeca0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 357 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "i2c,clk-rate", 198000000 },
        Package (2) { "rockchip,bclk", 198000000 },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })
  }
