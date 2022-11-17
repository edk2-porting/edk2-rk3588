/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

Scope(_SB)
{
  Device(COM0) {
    Name(_HID, "HISI0031") //it is not 16550 compatible
    Name(_UID, Zero)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x80300000, 0x1000)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 349 }
    })
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", 200000000},
      }
    })
  }
}
