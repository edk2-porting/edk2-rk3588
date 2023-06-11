/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

  Device (I2C2) {
    Name (_HID, "PRP0001")
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
        Package (2) { "compatible", "rockchip,rk3399-i2c" },
        Package (2) { "#address-cells", 1 },
        Package (2) { "#size-cells", 0 },
      }
    })

    OperationRegion(I2PC, SystemMemory, 0xFD7C0828, 0x4)
      Field(I2PC, DWordAcc, Lock, WriteAsZeros) {
      CG10, 32,
    }

    OperationRegion(I2SC, SystemMemory, 0xFD7C082c, 0x4)
      Field(I2SC, DWordAcc, Lock, WriteAsZeros) {
      CG11, 32,
    }

    Method(_PS3) {
      Store (0x02000200, CG10)
      Store (0x00020002, CG11)
    }

    Method(_PS0) {
      Store (0x02000000, CG10)
      Store (0x00020000, CG11)
    }

    Method(_PSC) {
      Return(0x01)
    }
  }
