/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright (c) 2021, ARM Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  @par Specification Reference:
*  - ACPI 6.3, Section 5.2.11.1, Differentiated System Description Table
**/

#include "SgiAcpiHeader.h"
#include "SgiPlatform.h"

DefinitionBlock ("DsdtTable.aml", "DSDT", 2, "ARMLTD", "ARMSGI",
                 EFI_ACPI_ARM_OEM_REVISION) {
  Scope (_SB) {
    /* _OSC: Operating System Capabilities */
    Method (_OSC, 4, Serialized) {
      CreateDWordField (Arg3, 0x00, STS0)
      CreateDWordField (Arg3, 0x04, CAP0)

      /* Platform-wide Capabilities */
      If (LEqual (Arg0, ToUUID("0811b06e-4a27-44f9-8d60-3cbbc22e7b48"))) {
        /* OSC rev 1 supported, for other version, return failure */
        If (LEqual (Arg1, One)) {
          And (STS0, Not (OSC_STS_MASK), STS0)

          If (And (CAP0, OSC_CAP_OS_INITIATED_LPI)) {
            /* OS initiated LPI not supported */
            And (CAP0, Not (OSC_CAP_OS_INITIATED_LPI), CAP0)
            Or (STS0, OSC_STS_CAPABILITY_MASKED, STS0)
          }

          If (And (CAP0, OSC_CAP_PLAT_COORDINATED_LPI)) {
            if (LEqual (FixedPcdGet32 (PcdOscLpiEnable), Zero)) {
              And (CAP0, Not (OSC_CAP_PLAT_COORDINATED_LPI), CAP0)
              Or (STS0, OSC_STS_CAPABILITY_MASKED, STS0)
            }
          }

        } Else {
          And (STS0, Not (OSC_STS_MASK), STS0)
          Or (STS0, Or (OSC_STS_FAILURE, OSC_STS_UNRECOGNIZED_REV), STS0)
        }
      } Else {
        And (STS0, Not (OSC_STS_MASK), STS0)
        Or (STS0, Or (OSC_STS_FAILURE, OSC_STS_UNRECOGNIZED_UUID), STS0)
      }

      Return (Arg3)
    }

    Name (CLPI, Package () {  /* LPI for Cluster, support 1 LPI state */
      0,                      // Version
      0,                      // Level Index
      1,                      // Count
      Package () {            // Power Gating state for Cluster
        2500,                 // Min residency (uS)
        1150,                 // Wake latency (uS)
        1,                    // Flags
        1,                    // Arch Context Flags
        100,                  // Residency Counter Frequency
        0,                    // No Parent State
        0x00000020,           // Integer Entry method
        ResourceTemplate () { // Null Residency Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        ResourceTemplate () { // Null Usage Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        "LPI2-Cluster"
      },
    })

    Name (PLPI, Package () {  /* LPI for Processor, support 2 LPI states */
      0,                      // Version
      1,                      // Level Index
      2,                      // Count
      Package () {            // WFI for CPU
        1,                    // Min residency (uS)
        1,                    // Wake latency (uS)
        1,                    // Flags
        0,                    // Arch Context lost Flags (no loss)
        100,                  // Residency Counter Frequency
        0,                    // No parent state
        ResourceTemplate () { // Register Entry method
          Register (FFixedHW,
            32,               // Bit Width
            0,                // Bit Offset
            0xFFFFFFFF,       // Address
            3,                // Access Size
          )
        },
        ResourceTemplate () { // Null Residency Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        ResourceTemplate () { // Null Usage Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        "LPI1-Core"
      },
      Package () {            // Power Gating state for CPU
        150,                  // Min residency (uS)
        350,                  // Wake latency (uS)
        1,                    // Flags
        1,                    // Arch Context lost Flags (Core context lost)
        100,                  // Residency Counter Frequency
        1,                    // Parent node can be in any shallower state
        ResourceTemplate () { // Register Entry method
          Register (FFixedHW,
            32,               // Bit Width
            0,                // Bit Offset
            0x40000002,       // Address (PwrLvl:core, StateTyp:PwrDn)
            3,                // Access Size
          )
        },
        ResourceTemplate () { // Null Residency Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        ResourceTemplate () { // Null Usage Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        "LPI3-Core"
      },
    })

    /* Chip 0 CPUs */
    Device (CLU0) {   // Cluster 0
      Name (_HID, "ACPI0010")
      Name (_UID, 0)
      Method (_LPI, 0, NotSerialized) {
        Return (\_SB.CLPI)
      }

      Device (CP00) { // Neoverse-N1: Cluster 0, Cpu 0
        Name (_HID, "ACPI0007")
        Name (_UID, 0)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP01) { // Neoverse-N1: Cluster 0, Cpu 1
        Name (_HID, "ACPI0007")
        Name (_UID, 1)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP02) { // Neoverse-N1: Cluster 0, Cpu 2
        Name (_HID, "ACPI0007")
        Name (_UID, 2)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP03) { // Neoverse-N1: Cluster 0, Cpu 3
        Name (_HID, "ACPI0007")
        Name (_UID, 3)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CLU1) {   // Cluster 1
      Name (_HID, "ACPI0010")
      Name (_UID, 1)
      Method (_LPI, 0, NotSerialized) {
        Return (\_SB.CLPI)
      }

      Device (CP04) { // Neoverse-N1: Cluster 1, Cpu 0
        Name (_HID, "ACPI0007")
        Name (_UID, 4)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP05) { // Neoverse-N1: Cluster 1, Cpu 1
        Name (_HID, "ACPI0007")
        Name (_UID, 5)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP06) { // Neoverse-N1: Cluster 1, Cpu 2
        Name (_HID, "ACPI0007")
        Name (_UID, 6)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP07) { // Neoverse-N1: Cluster 1, Cpu 3
        Name (_HID, "ACPI0007")
        Name (_UID, 7)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    /* Chip 1 CPUs */
    Device (CLU2) {   // Cluster 2
      Name (_HID, "ACPI0010")
      Name (_UID, 2)
      Method (_LPI, 0, NotSerialized) {
        Return (\_SB.CLPI)
      }

      Device (CP08) { // Neoverse-N1: Cluster 2, Cpu 0
        Name (_HID, "ACPI0007")
        Name (_UID, 8)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP09) { // Neoverse-N1: Cluster 2, Cpu 1
        Name (_HID, "ACPI0007")
        Name (_UID, 9)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP10) { // Neoverse-N1: Cluster 2, Cpu 2
        Name (_HID, "ACPI0007")
        Name (_UID, 10)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP11) { // Neoverse-N1: Cluster 2, Cpu 3
        Name (_HID, "ACPI0007")
        Name (_UID, 11)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CLU3) {   // Cluster 3
      Name (_HID, "ACPI0010")
      Name (_UID, 3)
      Method (_LPI, 0, NotSerialized) {
        Return (\_SB.CLPI)
      }

      Device (CP12) { // Neoverse-N1: Cluster 3, Cpu 0
        Name (_HID, "ACPI0007")
        Name (_UID, 12)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP13) { // Neoverse-N1: Cluster 3, Cpu 1
        Name (_HID, "ACPI0007")
        Name (_UID, 13)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP14) { // Neoverse-N1: Cluster 3, Cpu 2
        Name (_HID, "ACPI0007")
        Name (_UID, 14)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP15) { // Neoverse-N1: Cluster 3, Cpu 3
        Name (_HID, "ACPI0007")
        Name (_UID, 15)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }
  } // Scope(_SB)
}
