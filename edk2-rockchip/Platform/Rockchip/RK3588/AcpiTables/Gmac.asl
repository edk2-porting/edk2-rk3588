/** @file
 *
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

  Device (MAC0) {
    Name (_HID, "PRP0001")
    Name (_UID, 0)
    Name (_CCA, 0)

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
        Package (2) { "interrupt-names", Package () { "macirq", "eth_wake_irq" }, },
        Package (2) { "tx_delay", 67 },
        Package (2) { "compatible", package () { "rockchip,rk3588-gmac", "snps,dwmac-4.20a" }, },
        Package (2) { "phy-mode", "rgmii-rxid" },
        Package (2) { "clock_in_out", "output" },
        Package (2) { "csr-freq", 150000000 },
        Package (2) { "snps,reset-delays-us", package () { 0, 20000, 100000 } },
        Package (2) { "phy-handle", \_SB.MAC0.MDIO.PHY0 },
        Package (2) { "snps,axi-config", \_SB.MAC0.AST0 },
        Package (2) { "snps,mtl-rx-config", \_SB.MAC0.MRX0 },
        Package (2) { "snps,mtl-tx-config", \_SB.MAC0.MTX0 },
        Package () { "snps,mixed-burst", " " },
        Package () { "snps,tso", " " },
        Package () { "snps,reset-active-low", " " },
      }
    })

    Method(PRST, 0, Serialized) {
      OperationRegion(CTRL, SystemMemory, 0xfec50000, 0x10)
      Field(CTRL, DWordAcc,Lock, Preserve) {
        DRL, 32,
        DRH, 32,
        DDRL, 32,
        DDRH, 32,
      }

      Store(0x08000800, DDRL)
      sleep(20)
      Store(0x08000000, DRL)
      sleep(200)
      Store(0x08000800, DRL)
      sleep(1000)
    }

    Method(RGMI, 2, Serialized) {
      OperationRegion(PGRF, SystemMemory, 0xfd5b0008, 0x70)
      Field(PGRF, DWordAcc,Lock, Preserve) {
        CON0, 32,
        Offset(0x68),
        CLK1, 32,
      }

      OperationRegion(SGRF, SystemMemory, 0xfd58c31c, 0x8)
      Field(SGRF, DWordAcc,Lock, Preserve) {
        CON7, 32,
        CON8, 32,
      }

      Store(0x00380008, CON0)
      Store(0x00010000, CLK1)

      if (LGreater(ToInteger(Arg0), 0)) {
        Store(0x00040004, CON7)
        local0 = Arg0 & 0xff
        local0 = local0 | 0x00ff0000
        Store(local0, CON8)
      }

      if (LGreater(ToInteger(Arg1), 0)) {
        Store(0x00080008, CON7)
        local1 = Arg1 & 0x00
        local1 = local1 << 8
        local1 = local1 | 0xff000000
        Store(local1, CON8)
      }
    }

    Method(RMII, 0, Serialized) {
      OperationRegion(PGRF, SystemMemory, 0xfd5b0008, 0x70)
      Field(PGRF, DWordAcc,Lock, Preserve) {
        CON0, 32,
        Offset(0x68),
        CLK1, 32,
      }

      Store(0x00380020, CON0)
      Store(0x00010001, CLK1)
    }

    Method(SPED, 1, Serialized) {
      OperationRegion(PGRF, SystemMemory, 0xfd5b0070, 0x4)
      Field(PGRF, DWordAcc,Lock, Preserve) {
        CLK1, 32,
      }

      if (LEqual(Arg0, 1000)) {
        Store(0x000c0000, CLK1)
      }

      if (LEqual(Arg0, 100)) {
        Store(0x000c000c, CLK1)
      }

      if (LEqual(Arg0, 10)) {
        Store(0x000c0008, CLK1)
      }
    }

    Method(CLKS, 2, Serialized) {
      OperationRegion(PGRF, SystemMemory, 0xfd5b0070, 0x4)
      Field(PGRF, DWordAcc,Lock, Preserve) {
        CLK1, 32,
      }

      if (LEqual(Arg0, 0)) {
        Store(0x00100010, CLK1)
      }

      if (LEqual(Arg1, 1)) {
        Store(0x00020000, CLK1)
      }
    }

    Device (AST0)
    {
      Name (_DSD, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "snps,wr_osr_lmt", 4 },
          Package () { "snps,rd_osr_lmt", 8 },
          Package () { "snps,blen", package () { 0, 0, 0, 0, 16, 8, 4 }, },
        }
      })
    }

    Device (MRX0)
    {
      Name (_DSD, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "snps,rx-queues-to-use", 2 },
        }
      })

      Device (rxq0) {
        Name (_ADR, 0)
      }

      Device (rxq1) {
        Name (_ADR, 1)
      }
    }

    Device (MTX0)
    {
      Name (_DSD, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "snps,tx-queues-to-use", 2 },
        }
      })

      Device (txq0) {
        Name (_ADR, 0)
      }

      Device (txq1) {
        Name (_ADR, 1)
      }
    }

    Device (MDIO)
    {
      Name (_HID, "PRP0001")
      Name (_UID, 0)

      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "compatible", "snps,dwmac-mdio" },
          Package () { "#address-cells", 1},
        }
      })

      Device (PHY0)
      {
        Name (_HID, "PRP0001")
        Name (_UID, 0)
        Name (_ADR, 1)

        Name (_DSD, Package () {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
            Package () { "compatible", "ethernet-phy-ieee802.3-c22" },
            Package () { "reg", 1},
          }
        })
      }
    }
  }
