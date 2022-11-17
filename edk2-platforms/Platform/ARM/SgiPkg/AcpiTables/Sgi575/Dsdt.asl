/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright (c) 2018 - 2021, ARM Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "SgiPlatform.h"
#include "SgiAcpiHeader.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 2, "ARMLTD", "ARMSGI", EFI_ACPI_ARM_OEM_REVISION) {
  Scope(_SB) {
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

    Device (CLU0) {   // Cluster 0
      Name (_HID, "ACPI0010")
      Name (_UID, 0)
      Method (_LPI, 0, NotSerialized) {
        Return (\_SB.CLPI)
      }

      Device (CP00) { // A75-0: Cluster 0, Cpu 0
        Name (_HID, "ACPI0007")
        Name (_UID, 0)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP01) { // A75-0: Cluster 0, Cpu 1
        Name (_HID, "ACPI0007")
        Name (_UID, 1)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP02) { // A75-0: Cluster 0, Cpu 2
        Name (_HID, "ACPI0007")
        Name (_UID, 2)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP03) { // A75-0: Cluster 0, Cpu 3
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

      Device (CP04) { // A75-0: Cluster 1, Cpu 0
        Name (_HID, "ACPI0007")
        Name (_UID, 4)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP05) { // A75-0: Cluster 1, Cpu 1
        Name (_HID, "ACPI0007")
        Name (_UID, 5)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP06) { // A75-0: Cluster 1, Cpu 2
        Name (_HID, "ACPI0007")
        Name (_UID, 6)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }

      Device (CP07) { // A75-0: Cluster 1, Cpu 3
        Name (_HID, "ACPI0007")
        Name (_UID, 7)
        Name (_STA, 0xF)
        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    // UART PL011
    Device(COM0) {
      Name(_HID, "ARMH0011")
      Name(_CID, "ARMH0011")
      Name(_UID, Zero)
      Name(_STA, 0xF)
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x7FF80000, 0x1000)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 147 }
      })
    }

    // SMSC 91C111
    Device(ETH0) {
      Name(_HID, "LNRO0003")
      Name(_UID, Zero)
      Name(_STA, 0xF)
      Name(_CRS, ResourceTemplate() {
            Memory32Fixed(ReadWrite, 0x18000000, 0x1000)
            Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 111 }
      })
      Name(_DSD, Package() {
                 ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                     Package() {
                     Package(2) {"reg-io-width", 4 },
                    }
      })
    }
  } // Scope(_SB)

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
      Memory32Fixed (
        ReadWrite,
        FixedPcdGet32 (PcdVirtioNetBaseAddress),
        FixedPcdGet32 (PcdVirtioNetSize)
      )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
        FixedPcdGet32 (PcdVirtioNetInterrupt)
      }
    })
  }
}
