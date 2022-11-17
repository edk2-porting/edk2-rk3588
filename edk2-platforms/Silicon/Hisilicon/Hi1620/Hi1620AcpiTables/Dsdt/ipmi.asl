/** @file
*
*  Copyright (c) 2018 Hisilicon Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

//
// LPC
//

Scope(_SB) {
  Device (IPI0) {
  Name (_HID, "IPI0001")
  Name (_UID, 0)
  Name (_STR, Unicode("IPMI_BT"))
  Name(_CCA, 1)
  //Name (_CID, "IPI0001")
  Method (_IFT) {
    Return (0x03)
  }
  Method (_SRV) {
    Return (0x0200)   // IPMI Spec Revision 2.0
  }
  Name (_CRS, ResourceTemplate () { // _CRS: Current Resource Settings
    QWordMemory ( // BMC memory region
      ResourceConsumer,
      PosDecode,
      MinFixed,
      MaxFixed,
      Cacheable,
      ReadWrite,
      0x0, // Granularity
      0x3f00000e4, // Min Base Address
      0x3f00000e7, // Max Base Address
      0x0, // Translate
      0x4 // Length
    )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 484 }
  })
  }
}
