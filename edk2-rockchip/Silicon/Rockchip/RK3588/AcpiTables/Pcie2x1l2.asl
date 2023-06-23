/** @file
 *
 *  Copyright (c) 2022-2023, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

DefinitionBlock (__FILE__, "SSDT", 5, "RKCP  ", "PCIE21L2", 2)
{
  Scope (\_SB_)
  {
    Device (PCI4) {
      Name (_HID, "PNP0A08")
      Name (_CID, "PNP0A03")
      Name (_CCA, Zero)
      Name (_UID, 4)
      Name (_SEG, 4)
      Name (_BBN, One)

      Name (_PRT, Package() {
        Package (4) { 0x0FFFF, 0, Zero, 282 },
        Package (4) { 0x0FFFF, 1, Zero, 282 },
        Package (4) { 0x0FFFF, 2, Zero, 282 },
        Package (4) { 0x0FFFF, 3, Zero, 282 }
      })

      Method (_CRS, 0, Serialized) {
        Name (RBUF, ResourceTemplate () {
          WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
            0,    // Granularity
            1,    // Range Minimum
            1,    // Range Maximum
            0,    // Translation Offset
            1,    // Length
          )
          DWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
            0x00000000,   // Granularity
            0xF4000000,   // Range Minimum
            0xF4FFFFFF,   // Range Maximum
            0x00000000,   // Translation Offset
            0x01000000,   // Length
          )
          QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
            0x0000000000000000,   // Granularity
            0x0000000A10000000,   // Range Minimum
            0x0000000A3FFEFFFF,   // Range Maximum
            0x0000000000000000,   // Translation Offset
            0x000000002FFF0000,   // Length
          )
          QWordIO (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
            0,                    // Granularity
            0x0000,               // Range Minimum
            0xFFFF,               // Range Maximum
            0x0000000A3FFF0000,   // Translation Offset
            0x10000,              // Length
          )
        })
        return (RBUF)
      }

      Device (RES0) {
        Name (_HID, "PNP0C02")
        Name (_CRS, ResourceTemplate () {
          QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
            0x0000000000000000,   // Granularity
            0x0000000A00000000,   // Range Minimum
            0x0000000A0FFFFFFF,   // Range Maximum
            0x0000000000000000,   // Translation Offset
            0x0000000010000000,   // Length
          )
          QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
            0x0000000000000000,   // Granularity
            0x0000000A41000000,   // Range Minimum
            0x0000000A413FFFFF,   // Range Maximum
            0x0000000000000000,   // Translation Offset
            0x0000000000400000,   // Length
          )
        })
      }

      // OS Control Handoff
      Name(SUPP, Zero) // PCI _OSC Support Field value
      Name(CTRL, Zero) // PCI _OSC Control Field value

      // See [1] 6.2.10, [2] 4.5
      Method(_OSC,4) {
        // Note, This code is very similar to the code in the PCIe firmware
        // specification which can be used as a reference
        // Check for proper UUID
        If(LEqual(Arg0,ToUUID("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) {
        // Create DWord-adressable fields from the Capabilities Buffer
        CreateDWordField(Arg3,0,CDW1)
        CreateDWordField(Arg3,4,CDW2)
        CreateDWordField(Arg3,8,CDW3)
        // Save Capabilities DWord2 & 3
        Store(CDW2,SUPP)
        Store(CDW3,CTRL)
        // Mask out Native HotPlug
        And(CTRL,0x1E,CTRL)
        // Always allow native PME, AER (no dependencies)
        // Never allow SHPC (no SHPC controller in this system)
        And(CTRL,0x1D,CTRL)

        If(LNotEqual(Arg1,One)) { // Unknown revision
            Or(CDW1,0x08,CDW1)
        }

        If(LNotEqual(CDW3,CTRL)) {  // Capabilities bits were masked
            Or(CDW1,0x10,CDW1)
        }
        // Update DWORD3 in the buffer
        Store(CTRL,CDW3)
        Return(Arg3)
        } Else {
        Or(CDW1,4,CDW1) // Unrecognized UUID
        Return(Arg3)
        }
      } // End _OSC
    } // PCI4
  } // Scope (\_SB_)
} // DefinitionBlock
