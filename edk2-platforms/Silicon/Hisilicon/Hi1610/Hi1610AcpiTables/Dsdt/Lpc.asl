/** @file
*
*  Copyright (c) 2016 Hisilicon Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

//
// LPC
//

Scope(_SB) {
  Device (LPC0) {
    Name (_HID, "HISI0191")  // HiSi LPC
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0xa01b0000, 0x1000)
    })
  }

  Device (LPC0.IPMI) {
    Name (_HID, "IPI0001")
    Method (_IFT) {
      Return (0x03)
    }
    Name (LORS, ResourceTemplate() {
      QWordIO (
        ResourceConsumer,
        MinNotFixed,     // _MIF
        MaxNotFixed,     // _MAF
        PosDecode,
        EntireRange,
        0x0,             // _GRA
        0xe4,            // _MIN
        0x3fff,          // _MAX
        0x0,             // _TRA
        0x04,            // _LEN
        , ,
        BTIO
      )
    })
    CreateQWordField (LORS, BTIO._MIN, CMIN)
    CreateQWordField (LORS, BTIO._MAX, CMAX)
    CreateQWordField (LORS, BTIO._LEN, CLEN)

    Method (_PRS, 0) {
      Return (LORS)
    }

    Method (_CRS, 0) {
      Return (LORS)
    }
    Method (_SRS, 1) {
      CreateQWordField (Arg0, \_SB.LPC0.IPMI.BTIO._MIN, IMIN)
      Store (IMIN, CMIN)
      CreateQWordField (Arg0, \_SB.LPC0.IPMI.BTIO._MAX, IMAX)
      Store (IMAX, CMAX)
    }
  }

  Device (LPC0.CON0) {
    Name (_HID, "HISI1031")
    Name (_CID, "PNP0501")
    Name (LORS, ResourceTemplate() {
      QWordIO (
        ResourceConsumer,
        MinNotFixed,     // _MIF
        MaxNotFixed,     // _MAF
        PosDecode,
        EntireRange,
        0x0,             // _GRA
        0x2F8,           // _MIN
        0x3fff,          // _MAX
        0x0,             // _TRA
        0x08,            // _LEN
        , ,
        IO02
      )
    })
    CreateQWordField (LORS, IO02._MIN, CMIN)
    CreateQWordField (LORS, IO02._MAX, CMAX)
    CreateQWordField (LORS, IO02._LEN, CLEN)

    Method (_PRS, 0) {
      Return (LORS)
    }

    Method (_CRS, 0) {
      Return (LORS)
    }
    Method (_SRS, 1) {
      CreateQWordField (Arg0, \_SB.LPC0.CON0.IO02._MIN, IMIN)
      Store (IMIN, CMIN)
      CreateQWordField (Arg0, \_SB.LPC0.CON0.IO02._MAX, IMAX)
      Store (IMAX, CMAX)
    }
  }
}
