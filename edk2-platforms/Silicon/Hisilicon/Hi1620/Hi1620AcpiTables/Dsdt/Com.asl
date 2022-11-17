/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

Scope(_SB)
{
  Device(COM0) {
    Name(_HID, "ARMH0011")
    Name(_UID, Zero)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x94080000, 0x1000)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 141 }
    })
  }
}
