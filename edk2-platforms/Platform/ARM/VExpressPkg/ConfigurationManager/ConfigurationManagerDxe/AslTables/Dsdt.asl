/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014-2021, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2013, Al Stone <al.stone@linaro.org>
  All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "ARMLTD", "ARM-VEXP", 1) {
  Scope(_SB) {
    //
    // Processor
    //
    Device(CPU0) {
      Name(_HID, "ACPI0007")
      Name(_UID, Zero)
    }
    Device(CPU1) {
      Name(_HID, "ACPI0007")
      Name(_UID, One)
    }
    Device(CPU2) {
      Name(_HID, "ACPI0007")
      Name(_UID, 2)
    }
    Device(CPU3) {
      Name(_HID, "ACPI0007")
      Name(_UID, 3)
    }
    Device(CPU4) {
      Name(_HID, "ACPI0007")
      Name(_UID, 4)
    }
    Device(CPU5) {
      Name(_HID, "ACPI0007")
      Name(_UID, 5)
    }
    Device(CPU6) {
      Name(_HID, "ACPI0007")
      Name(_UID, 6)
    }
    Device(CPU7) {
      Name(_HID, "ACPI0007")
      Name(_UID, 7)
    }

    // SMC91X
    Device (NET0) {
      Name (_HID, "LNRO0003")
      Name (_UID, 0)

      Name (_CRS, ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0x1a000000, 0x00010000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {0x2F}
      })
    }

    // VIRTIO block device
    Device (VIRT) {
      Name (_HID, "LNRO0005")
      Name (_UID, 0)

      Name (_CRS, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0x1c130000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {0x4A}
      })
    }
  } // Scope(_SB)
}
