/** @file
*  Secondary System Description Table Fields (SSDT)
*
*  Copyright (c) 2020-2021, Arm Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "SgiPlatform.h"
#include "SgiAcpiHeader.h"

DefinitionBlock ("SsdtRosTable.aml", "SSDT", 2, "ARMLTD", "ARMSGI",
                 EFI_ACPI_ARM_OEM_REVISION) {
  Scope (_SB) {
    // UART PL011
    Device (COM0) {
      Name (_HID, "ARMH0011")
      Name (_CID, "ARMH0011")
      Name (_UID, Zero)
      Name (_STA, 0xF)
      Name (_CRS, ResourceTemplate () {
        Memory32Fixed (
          ReadWrite,
          FixedPcdGet64 (PcdSerialDbgRegisterBase),
          0x1000
          )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FixedPcdGet32 (PL011UartInterrupt) }
      })
    }

    // VIRTIO DISK
    Device (VR00) {
      Name (_HID, "LNRO0005")
      Name (_UID, 0)
      Name (_CCA, 1)    // mark the device coherent

      Name (_CRS, ResourceTemplate() {
        Memory32Fixed (
          ReadWrite,
          FixedPcdGet32 (PcdVirtioBlkBaseAddress),
          FixedPcdGet32 (PcdVirtioBlkSize)
        )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
          FixedPcdGet32 (PcdVirtioBlkInterrupt)
        }
      })
    }

    // VIRTIO NET
    Device (VR01) {
      Name (_HID, "LNRO0005")
      Name (_UID, 1)
      Name (_CCA, 1)    // mark the device coherent

      Name (_CRS, ResourceTemplate() {
        Memory32Fixed (ReadWrite,
          FixedPcdGet32 (PcdVirtioNetBaseAddress),
          FixedPcdGet32 (PcdVirtioNetSize)
        )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
          FixedPcdGet32 (PcdVirtioNetInterrupt)
        }
      })
    }
  } // Scope(_SB)
}
