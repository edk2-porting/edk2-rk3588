/** @file
  Phytium ACPI ASL Sources.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope (_SB)
{
  //UART 0
  Device (UAR0) {
    Name (_HID, "ARMH0011")
    Name (_UID, 0)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x28000000, 0x1000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 38 }
    })

    Method (_STA, 0, NotSerialized) {
    Return (0x0F)
    }
  }

  //UART 1
  Device (UAR1) {
    Name (_HID, "ARMH0011")
    Name (_UID, 1)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x28001000, 0x1000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {39}
    })

    Method (_STA, 0, NotSerialized) { Return (0x0F) }
  }

  //UART 2
  Device (UAR2) {
    Name (_HID, "ARMH0011")
    Name (_UID, 2)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x28002000, 0x1000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {40}
    })

    Method (_STA, 0, NotSerialized) {
    Return (0x0F)
    }
  }

  //UART 3
  Device (UAR3) {
    Name (_HID, "ARMH0011")
    Name (_UID, 3)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x28003000, 0x1000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {41}
    })

    Method (_STA, 0, NotSerialized) {
    Return (0x0F)
    }
  }
}
