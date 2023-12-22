/** @file
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

// Gigabit Media Access Controller (GMAC0)
Device (MAC0) {
  Name (_HID, "RKCP6543")
  Name (_CID, Package() {
    "PRP0001"
  })
  Name (_UID, 60)
  Name (_CCA, 0)
  Name (_STA, 0xF)

  Method (_CRS, 0x0, Serialized) {
    Name (RBUF, ResourceTemplate() {
      Memory32Fixed (ReadWrite, 0xfe1b0000, 0x10000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 259, 258 }
    })
    Return (RBUF)
  }

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "compatible", Package () { "snps,dwmac-4.20a", "snps,dwmac" } },
      Package () { "interrupt-names", Package () { "macirq", "eth_wake_irq" } },
      Package () { "snps,mixed-burst", 1 },
      Package () { "snps,tso", 1 },
      Package () { "snps,axi-config", "AXIC" },
    }
  })

  Name (AXIC, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "snps,wr_osr_lmt", 4 },
      Package () { "snps,rd_osr_lmt", 8 },
      Package () { "snps,blen", Package () { 0, 0, 0, 0, 16, 8, 4 } },
    }
  })
}
