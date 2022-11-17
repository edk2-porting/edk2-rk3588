/** @file
 *
 *  [DSDT] Serial devices (UART).
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

  Device(UAR2) {
    Name (_HID, "HISI0031")
    Name (_UID, 2)
    Name (_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xfeb50000, 0x100)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 365 }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "reg-shift", 2 },
        Package () { "reg-io-width", 4 },
        Package () { "clock-frequency", 24000000 },
      }
    })

    Method (_STA, 0, NotSerialized) {
      Return(0x0F)
    }

  }



