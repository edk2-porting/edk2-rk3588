/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

Device (ATA0) {
  Name (_HID, "RKCP0161")
  Name (_UID, 0)
  Name (_CLS, Package() { 0x01, 0x06, 0x01 })
  Name (_CCA, 0)

  Method (_CRS, 0x0, Serialized) {
    Name (RBUF, ResourceTemplate() {
      Memory32Fixed (ReadWrite, 0xfe210000, 0x1000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 305 }
    })
    Return (RBUF)
  }
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package (2) { "compatible", "rockchip,rk-ahci" },
    }
  })
}
