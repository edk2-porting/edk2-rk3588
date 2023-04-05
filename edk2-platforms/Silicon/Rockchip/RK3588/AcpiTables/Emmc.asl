/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"


  Device (SDC3) {
    Name (_HID, "PRP0001")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe2e0000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 237 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "compatible", "rockchip,rk3588-dwcmshc" },
        Package () { "max-frequency", 200000000 },
        Package () { "bus-width", 8 },
        Package () { "no-sd", 0x1 },
        Package () { "no-sdio", 0x1  },
        Package () { "mmc-hs400-1_8v", 0x1  },
        Package () { "mmc-hs400-enhanced-strobe", 0x1  },
        Package () { "non-removable", 0x1  },
      }
    })

    OperationRegion(EMMC, SystemMemory, 0xFD7C0434, 0x4)
      Field(EMMC, DWordAcc, Lock, WriteAsZeros) {
      PLLE, 32,
    }

    Method(SCLK, 1, Serialized) {
      If (Arg0 <= 400000)
      {
        Store (0xFF00BF00, PLLE)
      }
      ElseIF (Arg0 <= 50000000)
      {
        Store (0xFF008000, PLLE)
      }
      Else
      {
        Store (0xFF000600, PLLE)
      }
    }

    Method(_PS3) {

    }

    Method(_PS2) {
      Store (0xFF00BF00, PLLE)
    }

    Method(_PS1) {
      Store (0xFF008000, PLLE)
    }

    Method(_PS0) {
      Store (0xFF000600, PLLE)
    }

    Method(_PSC) {
      Return(0x01)
    }
  }