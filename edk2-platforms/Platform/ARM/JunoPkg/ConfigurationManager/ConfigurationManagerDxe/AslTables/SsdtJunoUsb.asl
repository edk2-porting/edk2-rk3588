/** @file
  SSDT for Juno USB

  Copyright (c) 2014-2019, ARM Ltd. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ArmPlatform.h"

DefinitionBlock("SsdtJunoUSB.aml", "SSDT", 1, "ARMLTD", "ARM-JUNO", EFI_ACPI_ARM_OEM_REVISION) {
  Scope(_SB) {
    //
    // USB EHCI Host Controller
    //
    Device(USB0) {
      Name(_HID, "ARMH0D20")
      Name(_CID, "PNP0D20")
      Name(_UID, 2)

      Name(_CCA, ZERO)   // Cache-incoherent bus-master

      Method(_CRS, 0x0, Serialized) {
        Name(RBUF, ResourceTemplate() {
          // 0x7FFC0000 is the Juno SoC USB EHCI controller base address
          Memory32Fixed(ReadWrite, 0x7FFC0000, 0x000000B0)
          // INT ID=149 GIC IRQ ID=117 for Juno SoC USB EHCI Controller
          Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {149}
        })
        Return(RBUF)
      }

      //
      // Root Hub
      //
      Device(RHUB) {
        Name(_ADR, Zero)  // Address of Root Hub should be 0 as per ACPI 5.0 spec

        //
        // Ports connected to Root Hub
        //
        Device(HUB1) {
          Name(_ADR, One)
          Name(_UPC, Package(){
            Zero,       // Port is NOT connectable
            0xFF,       // Don't care
            Zero,       // Reserved 0 must be zero
            Zero        // Reserved 1 must be zero
          })

          Device(PRT1) {
            Name(_ADR, One)
            Name(_UPC, Package(){
              0xFF,        // Port is connectable
              Zero,        // Port connector is A
              Zero,
              Zero
            })
            Name(_PLD, Package(One) {
              Buffer(0x10){
                0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
              }
            })
          } // USB0_RHUB_HUB1_PRT1
          Device(PRT2) {
            Name(_ADR, 0x00000002)
            Name(_UPC, Package() {
              0xFF,        // Port is connectable
              Zero,        // Port connector is A
              Zero,
              Zero
            })
            Name(_PLD, Package(One) {
              Buffer(0x10){
                0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
              }
            })
          } // USB0_RHUB_HUB1_PRT2

          Device(PRT3) {
            Name(_ADR, 0x00000003)
            Name(_UPC, Package() {
              0xFF,        // Port is connectable
              Zero,        // Port connector is A
              Zero,
              Zero
            })
            Name(_PLD, Package(One) {
              Buffer(0x10){
                0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
              }
            })
          } // USB0_RHUB_HUB1_PRT3

          Device(PRT4) {
            Name(_ADR, 0x00000004)
            Name(_UPC, Package(){
              0xFF,        // Port is connectable
              Zero,        // Port connector is A
              Zero,
              Zero
            })
            Name(_PLD, Package(One) {
              Buffer(0x10){
                0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
              }
            })
          } // USB0_RHUB_HUB1_PRT4
        } // USB0_RHUB_HUB1
      } // USB0_RHUB
    } // USB0
  }
}
