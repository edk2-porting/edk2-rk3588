/** @file
  Copyright (c) 2019, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock ("SsdtTOS0.aml", "SSDT", 1, "SNI", "SynQTOS0",
                 FixedPcdGet32 (PcdAcpiDefaultOemRevision)) {
  Scope (_SB) {
    Device (TOS0) {
      Name (_HID, "PRP0001")
      Name (_UID, 0x0)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package (2) { "compatible", "linaro,optee-tz" },
          Package (2) { "method", "smc" },
        }
      })
    }
  } // Scope (_SB)
}
