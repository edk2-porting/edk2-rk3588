/** @file
 *
 *  Copyright (c) 2022 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

  Device (SPI0)
  {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeb00000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 358 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,rk3036-spi" },
        Package () { "clock-frequency", 200000000 },
        Package () { "num-cs", 2 },
      }
    })
  }

  Device (SPI1)
  {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeb10000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 359 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,rk3036-spi" },
        Package () { "clock-frequency", 200000000 },
        Package () { "num-cs", 2 },
      }
    })
  }

  Device (SPI2)
  {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeb20000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 360 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,rk3036-spi" },
        Package () { "clock-frequency", 200000000 },
        Package () { "num-cs", 2 },
      }
    })
  }

  Device (SPI3)
  {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfeb30000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 361 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,rk3036-spi" },
        Package () { "clock-frequency", 200000000 },
        Package () { "num-cs", 2 },
      }
    })
  }

  Device (SPI4)
  {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfecb0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 362 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,rk3036-spi" },
        Package () { "clock-frequency", 200000000 },
        Package () { "num-cs", 2 },
      }
    })
  }

  Device (SFC0)
  {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe2b0000, 0x4000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 238 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,sfc" },
        Package () { "clock-frequency", 100000000 },
      }
    })
  }