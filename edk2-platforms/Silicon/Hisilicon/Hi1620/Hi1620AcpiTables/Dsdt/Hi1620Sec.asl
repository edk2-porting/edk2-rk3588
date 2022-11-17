/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2018, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

Scope(_SB)
{
  Device(SEC0) {
    Name (_HID, "HISI0200")
    Name(_UID, 0)
    Name(_CCA, 1)
    Name (_CRS, ResourceTemplate ()  {
      //Memory32Fixed (ReadWrite, 0x602E0000, 0x10000)
      QWordMemory (
        ResourceConsumer,
        ,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0,
        0x141000000,
        0x141ffffff,
        0x0,
        0x01000000
      )
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI3")
      {
        624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,
        640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655
      }
    })
  }

  Device(SEC1) {
    Name(_HID, "HISI0200")
    Name(_UID, 1)
    Name (_CRS, ResourceTemplate ()  {
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive)
      { 466,467
      }
    })
  }
}
