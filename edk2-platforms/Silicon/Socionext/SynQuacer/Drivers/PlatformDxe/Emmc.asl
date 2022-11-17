/** @file
  SSDT describing the SynQuacer eMMC controller

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Platform/MemoryMap.h>

DefinitionBlock ("SsdtEmmc.aml", "SSDT", 1, "SNI", "SynQeMMC",
                 FixedPcdGet32 (PcdAcpiDefaultOemRevision)) {
  Scope (_SB) {
    Device (MMC0) {
      Name (_HID, "SCX0002")
      Name (_UID, Zero)
      Name (_CCA, 1)
      Name (_CRS, ResourceTemplate () {
        Memory32Fixed (ReadWrite, SYNQUACER_EMMC_BASE, SYNQUACER_EMMC_BASE_SZ)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 184 }
      })

      Name (_DSD, Package ()  // _DSD: Device-Specific Data
      {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package (2) { "bus-width", 8 },
          Package (2) { "cap-mmc-highspeed", 0x1 },
          Package (2) { "fujitsu,cmd-dat-delay-select", 0x1 },
        }
      })
    }
  } // Scope (_SB)
}
