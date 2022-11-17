/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2021, ARM Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "ConfigurationManager.h"

DefinitionBlock("Dsdt.aml", "DSDT", 1, "ARMLTD", "MORELLO", CFG_MGR_OEM_REVISION) {
  Scope(_SB) {
    Device(CP00) { // Cluster 0, Cpu 0
      Name(_HID, "ACPI0007")
      Name(_UID, 0)
      Name(_STA, 0xF)
    }

    Device(CP01) { // Cluster 0, Cpu 1
      Name(_HID, "ACPI0007")
      Name(_UID, 1)
      Name(_STA, 0xF)
    }

    Device(CP02) { // Cluster 1, Cpu 0
      Name(_HID, "ACPI0007")
      Name(_UID, 2)
      Name(_STA, 0xF)
    }

    Device(CP03) { // Cluster 1, Cpu 1
      Name(_HID, "ACPI0007")
      Name(_UID, 3)
      Name(_STA, 0xF)
    }

    // VIRTIO DISK
    Device(VR00) {
      Name(_HID, "LNRO0005")
      Name(_UID, 0)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(
          ReadWrite,
          FixedPcdGet32 (PcdVirtioBlkBaseAddress),
          FixedPcdGet32 (PcdVirtioBlkSize)
        )
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {
          FixedPcdGet32 (PcdVirtioBlkInterrupt)
        }
      })
    }

    // VIRTIO NET
    Device(VR01) {
      Name(_HID, "LNRO0005")
      Name(_UID, 1)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1C180000, 0x00000200)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 134 }
      })
    }

    // VIRTIO RANDOM
    Device(VR02) {
      Name(_HID, "LNRO0005")
      Name(_UID, 2)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1C190000, 0x00000200)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 133 }
      })
    }

    // VIRTIO P9 Device
    Device(VR03) {
      Name(_HID, "LNRO0005")
      Name(_UID, 3)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1C1A0000, 0x00000200)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 135 }
      })
    }

    // SMC91X
    Device(NET0) {
      Name(_HID, "LNRO0003")
      Name(_UID, 0)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1D100000, 0x00001000)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 130 }
      })
    }
  } // Scope(_SB)
}
