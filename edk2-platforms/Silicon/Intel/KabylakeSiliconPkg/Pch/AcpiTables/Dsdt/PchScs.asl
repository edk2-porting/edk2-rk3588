/**@file
  ACPI DSDT table for SCS Controllers

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

If(LEqual(PCHV(), SPTL)) { // SCS available on PCH-LP only

Scope(\_SB.PCI0) {
  //
  // SCS eMMC (PCI Mode)
  //
  Device(PEMC) {
    Name(_ADR, 0x001E0004)


    // Memory Region to access to the eMMC PCI Configuration Space
    OperationRegion(SCSR, PCI_Config, 0x00, 0x100)
    Field(SCSR, WordAcc, NoLock, Preserve) {
      Offset(R_PCH_SCS_DEV_PCS),         // 0x84, PMCSR - Power Management Control and Status
      PSTA,32,
      Offset(R_PCH_SCS_DEV_PG_CONFIG),   // 0xA2, Device PG config
          , 2,
      PGEN, 1         // [BIT2] PGE - PG Enable
    }

    Method(_PS0, 0, Serialized) {
      Store(0, PGEN) // Disable PG
      // Register address: 0xFD000000 SB offset | 0xC0<<16 PID_SCS | 0x0600 GPPRVRW1
      PCRA(PID_SCS, R_PCH_PCR_SCS_GPPRVRW1, 0x7FFFFFBA) // Clear bits 31, 6, 2, 0
      Sleep(2) // Wait 2ms
      // Register address: 0xFD000000 SB offset | 0xC0<<16 PID_SCS | 0x0600 GPPRVRW1
      PCRO(PID_SCS, R_PCH_PCR_SCS_GPPRVRW1, 0x80000045) // Set bits 31, 6, 2, 0

      And(PSTA, 0xFFFFFFFC, PSTA) // Set BIT[1:0] = 00b - Power State D0
      Store(PSTA, TEMP) // Read Back PMCSR
    }

    Method(_PS3, 0, Serialized) {
      Store(1, PGEN) // Enable PG

      Or(PSTA, 0x3, PSTA) // Set BIT[1:0] = 11b - Power State D3
      Store(PSTA, TEMP) // Read Back PMCSR
    }

    // _DSM x86 Device Specific Method
    // Arg0: UUID Unique function identifier
    // Arg1: Integer Revision Level
    // Arg2: Integer Function Index (0 = Return Supported Functions)
    // Arg3: Package Parameters
    Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      If(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }

      ADBG(Concatenate("EMH4=",ToDecimalString(EMH4)))
      // Check the UUID
      If(LEqual(Arg0, ToUUID("f6c13ea5-65cd-461f-ab7a-29f7e8d5bd61"))) {
        // Check the revision
        If(LGreaterEqual(Arg1, Zero)) {
          //Switch statement based on the function index.
          Switch(ToInteger(Arg2)) {
            //
            // Function Index 0 the return value is a buffer containing
            // one bit for each function index, starting with zero.
            // Bit 0 - Indicates whether there is support for any functions other than function 0.
            // Bit 1 - Indicates support to clear power control register
            // Bit 2 - Indicates support to set power control register
            // Bit 3 - Indicates support to set 1.8V signalling
            // Bit 4 - Indicates support to set 3.3V signalling
            // Bit 5 - Indicates support for HS200 mode
            // Bit 6 - Indicates support for HS400 mode
            // Bit 9 - Indicates eMMC I/O Driver Strength
            //
            // On SPT, for eMMC we have to support functions for
            // HS200 and HS400 and I/O Driver Strength
            //
            Case(0) {
              //
              // The return value 0x261 (BIT6 set) for HS400 enabled and
              // 0x221 (BIT6 not set) when HS400 is disabled in Setup menu.
              //
              If(LEqual(EMH4, 1)) {
                Return(Buffer() {0x61, 0x02}) // HS400 support enabled
              }
              Return(Buffer() {0x21, 0x02})   // HS400 support disabled
            }

            //
            // Function index 5 - corresponds to HS200 mode
            // Return value from this function is used to program
            // the UHS Mode Select bits in Host Control 2.
            // 011b - corresponds to SDR104 and according to the
            // SD Host Controller Spec and this value is overloaded
            // to program the controller to select HS200 mode for eMMC.
            //
            Case(5) {
              Return(Buffer() {0x3})
            }

            //
            // Function index 6 - corresponds to HS400 mode
            // Return value from this function is used to program
            // the UHS Mode Select bits in Host Control 2.
            // 101b is a reserved value according to the SD Host
            // Controller Spec and we use this value for HS400 mode
            // selection.
            //
            Case(6) {
              Return(Buffer() {0x5})
            }

            //
            // Function index 9 - corresponds to I/O Driver Strength
            // Return value from this function represents the values
            // of the Driver Strength selection
            // (eMMC 5.01 Specification JESD84-B50.1, Table 186)
            // that shall be programmed by the Host Driver (OS)
            // as part of the Initialization flows.
            //
            Case(9) {
              Switch(EMDS) {
                Case(0x0) { Return(Buffer() {0x0}) } // 50 Ohm
                Case(0x1) { Return(Buffer() {0x1}) } // 33 Ohm
                Case(0x4) { Return(Buffer() {0x4}) } // 40 Ohm
              }
            }
          } // End - Switch(Arg2)
        }
      } // End - UUID check
      Return(Buffer() {0x0})
    } // End - _DSM

    Device (CARD) {
      Name (_ADR, 0x00000008)
      Method(_RMV, 0x0, NotSerialized) { Return (0) } // Device cannot be removed
    }
  } // Device(PEMC)

  //
  // SCS SD Card (PCI Mode)
  //
  Device(PSDC) {
    Name(_ADR, 0x001E0006)


    // Memory Region to access to the SDHC PCI Configuration Space
    OperationRegion(SCSR, PCI_Config, 0x00, 0x100)
    Field(SCSR, WordAcc, NoLock, Preserve) {
      Offset(R_PCH_SCS_DEV_PCS),         // 0x84, PMCSR - Power Management Control and Status
      PSTA,32,
      Offset(R_PCH_SCS_DEV_PG_CONFIG),   // 0xA2, Device PG config
          , 2,
      PGEN, 1         // [BIT2] PGE - PG Enable
    }

    Method(_PS0, 0, Serialized) {
      Store(0, PGEN) // Disable PG
      // Register address: 0xFD000000 SB offset | 0xC0<<16 PID_SCS | 0x0600 GPPRVRW1
      PCRA(PID_SCS, R_PCH_PCR_SCS_GPPRVRW1, 0xFFFFFE7A) // Clear bits 8, 7, 2, 0
      Sleep(2) // Wait 2ms
      // Register address: 0xFD000000 SB offset | 0xC0<<16 PID_SCS | 0x0600 GPPRVRW1
      PCRO(PID_SCS, R_PCH_PCR_SCS_GPPRVRW1, 0x00000185) // Set bits 8, 7, 2, 0

      And(PSTA, 0xFFFFFFFC, PSTA) // Set BIT[1:0] = 00b - Power State D0
      Store(PSTA, TEMP) // Read Back PMCSR
    }

    Method(_PS3, 0, Serialized) {
      Store(1, PGEN) // Enable PG

      Or(PSTA, 0x3, PSTA) // Set BIT[1:0] = 11b - Power State D3
      Store(PSTA, TEMP) // Read Back PMCSR
    }

    // _DSM x86 Device Specific Method
    // Arg0: UUID Unique function identifier
    // Arg1: Integer Revision Level
    // Arg2: Integer Function Index (0 = Return Supported Functions)
    // Arg3: Package Parameters
    Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      If(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }

      // Check the UUID
      If(LEqual(Arg0, ToUUID("f6c13ea5-65cd-461f-ab7a-29f7e8d5bd61"))) {
        // Check the revision
        If(LGreaterEqual(Arg1, Zero)) {
          // Switch statement based on the function index.
          Switch(ToInteger(Arg2)) {
            //
            // Function Index 0 the return value is a buffer containing
            // one bit for each function index, starting with zero.
            // Bit 0 - Indicates whether there is support for any functions other than function 0.
            // Bit 1 - Indicates support to clear power control register
            // Bit 2 - Indicates support to set power control register
            // Bit 3 - Indicates support to set 1.8V signalling
            // Bit 4 - Indicates support to set 3.3V signalling
            // Bit 5 - Indicates support for HS200 mode
            // Bit 6 - Indicates support for HS400 mode
            // Bit 9 - Indicates eMMC I/O Driver Strength
            //
            // On SPT, for SD we have to support functions to
            // set 1.8V signalling and 3.3V signalling
            //
            Case(0) {
              Return (Buffer() {0x19})
            }

            //
            // Function Index 3: Set 1.8v signalling.
            // We put a sleep of 100ms in this method to
            // work around a known issue with detecting
            // UHS SD card on SPT. This is to compensate
            // for the SD VR slowness.
            //
            Case(3) {
              Sleep (100) // Sleep 100ms
              Return(Buffer(){0x00})
            }

            //
            // Function Index 4: Set 3.3v signalling.
            // We put a sleep of 100ms in this method to
            // work around a known issue with detecting
            // UHS SD card on SPT. This is to compensate
            // for the SD VR slowness.
            //
            Case(4) {
              Sleep (100) // Sleep 100ms
              Return(Buffer(){0x00})
            }
          } // End - Switch(Arg2)
        }
      } // End - UUID check
      Return(Buffer() {0x0})
    } // End _DSM
  }
}

}// End If(LEqual(PCHV(), SPTL)) - PCH-LP only

