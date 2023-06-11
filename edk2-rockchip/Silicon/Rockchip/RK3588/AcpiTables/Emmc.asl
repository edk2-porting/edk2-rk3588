/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"


  Device (SDC3) {
    Name (_HID, "RKCP0D40")
    Name (_UID, 3)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe2e0000, 0x10000)
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
      Field(EMMC, DWordAcc, NoLock, WriteAsZeros) {
      PLLE, 32,
    }

    Method (_DSM, 4) {
      If (LEqual (Arg0, ToUUID("434addb0-8ff3-49d5-a724-95844b79ad1f"))) {
        Switch (ToInteger (Arg2)) {
          Case (0) {
            Return (0x3)
          }
          Case (1) {
            Local0 = DerefOf (Arg3 [0])
            If (Local0 >= 200000000) {
              Store (0xFF000500, PLLE)
              Return (200000000)
            }
            If (Local0 >= 150000000) {
              Store (0xFF000700, PLLE)
              Return (150000000)
            }
            If (Local0 >= 100000000) {
              Store (0xFF000B00, PLLE)
              Return (100000000)
            }
            If (Local0 >= 50000000) {
              Store (0xFF001700, PLLE)
              Return (50000000)
            }
            If (Local0 >= 24000000) {
              Store (0xFF008000, PLLE)
              Return (24000000)
            }
            if (Local0 >= 375000) {
              Store (0xFF00BF00, PLLE)
              Return (375000)
            }
            Return (0)
          }
        }
      }
      Return (0)
    }

    // Used by downstream Linux driver.
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

    /* TODO:
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
    */

    //
    // A child device that represents the non-removable eMMC.
    //
    Device (SDMM)
    {
      Method (_ADR)
      {
        Return (0)
      }
      Method (_RMV) // Is removable
      {
        Return (0) // 0 - fixed
      }
    }
  }