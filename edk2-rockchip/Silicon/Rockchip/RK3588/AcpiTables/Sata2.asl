/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

DefinitionBlock (__FILE__, "SSDT", 5, "RKCP  ", "AHCSATA2", 2)
{
  Scope (\_SB_)
  {
    Device (ATA2) {
      Name (_HID, "RKCP0161")
      Name (_UID, 2)
      Name (_CLS, Package() { 0x01, 0x06, 0x01 })
      Name (_CCA, 0)

      Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate() {
          Memory32Fixed (ReadWrite, 0xfe230000, 0x1000)
          Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 307 }
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
  } // Scope (\_SB_)
} // DefinitionBlock
