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
    //
    // A57x16 Processor declaration
    //
    Device(CPU0) {
      Name(_HID, "ACPI0007")
      Name(_UID, 0)
    }
    Device(CPU1) {
      Name(_HID, "ACPI0007")
      Name(_UID, 1)
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
    Device(CPU8) {
      Name(_HID, "ACPI0007")
      Name(_UID, 8)
    }
    Device(CPU9) {
      Name(_HID, "ACPI0007")
      Name(_UID, 9)
    }
    Device(CP10) {
      Name(_HID, "ACPI0007")
      Name(_UID, 10)
    }
    Device(CP11) {
      Name(_HID, "ACPI0007")
      Name(_UID, 11)
    }
    Device(CP12) {
      Name(_HID, "ACPI0007")
      Name(_UID, 12)
    }
    Device(CP13) {
      Name(_HID, "ACPI0007")
      Name(_UID, 13)
    }
    Device(CP14) {
      Name(_HID, "ACPI0007")
      Name(_UID, 14)
    }
    Device(CP15) {
      Name(_HID, "ACPI0007")
      Name(_UID, 15)
    }
}
