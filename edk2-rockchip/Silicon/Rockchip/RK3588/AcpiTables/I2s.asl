/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

#ifdef BOARD_ENABLE_I2S0
  Device (I2S0) {
    Name (_HID, "RKCP3003")
    Name (_UID, 0)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe470000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 212 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "rockchip,dma", "DMA0" },
        Package (2) { "rockchip,tx", 0 },
        Package (2) { "rockchip,rx", 1 },
        Package (2) { "rockchip,tplg", BOARD_I2S0_TPLG },
      }
    })
    Name (_DEP, Package () { \_SB.DMA0 })
  }
#endif

#ifdef BOARD_ENABLE_I2S1
  Device (I2S1) {
    Name (_HID, "RKCP3003")
    Name (_UID, 1)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe480000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 213 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "rockchip,dma", "DMA0" },
        Package (2) { "rockchip,tx", 2 },
        Package (2) { "rockchip,rx", 3 },
        Package (2) { "rockchip,tplg", BOARD_I2S1_TPLG },
        // XXX: Currently I2S1 is only used by Orange Pi 5.
        // The following properties should be board specific
        Package (2) { "rockchip,i2s-tx-route", 0x3210 },,
        Package (2) { "rockchip,i2s-rx-route", 0x1320 },
      }
    })
    Name (_DEP, Package () { \_SB.DMA0 })
  }
#endif