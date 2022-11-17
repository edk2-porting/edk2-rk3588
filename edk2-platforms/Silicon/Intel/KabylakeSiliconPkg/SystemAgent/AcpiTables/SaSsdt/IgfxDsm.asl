/** @file
  IGD OpRegion/_DSM Reference Code.
  This file contains Get BIOS Data and Callback functions for
  the Integrated Graphics Device (IGD) OpRegion/DSM mechanism

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// _DSM Device Specific Method
//
// Arg0: UUID Unique function identifier
// Arg1: Integer Revision Level
// Arg2: Integer Function Index (1 = Return Supported Functions)
// Arg3: Additional Inputs/Package Parameters Bits [31:0] input as {Byte0, Byte1, Byte2, Byte3} to BIOS which is passed as 32 bit DWORD by Driver
//
Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {

  If (LEqual(Arg0, ToUUID ("3E5B41C6-EB1D-4260-9D15-C71FBADAE414"))) {
    //
    // _DSM Definition for Igd functions
    // Arguments:
    // Arg0: UUID: 3E5B41C6-EB1D-4260-9D15-C71FBADAE414
    // Arg1: Revision ID: 1
    // Arg2: Function Index: 16
    // Arg3: Additional Inputs Bits[31:0] Arg3 {Byte0, Byte1, Byte2, Byte3}
    //
    // Return:
    // Success for simple notification, Opregion update for some routines and a Package for AKSV
    //
    //
    // Switch by function index
    //
    Switch(ToInteger(Arg2)) {
      //
      // Function Index: 0
      // Standard query - A bitmask of functions supported
      //
      // Return: A bitmask of functions supported
      //
      Case (0)
      {
        If (LEqual(Arg1, 1)) { // test Arg1 for Revision ID: 1
          Store("iGfx Supported Functions Bitmap ", Debug)
          Return (0x1E7FF)   // For SKL bit 11 and 12 is not supported
        }
      }

      //
      // Function Index: 1
      // Adapter Power State Notification
      // Arg3 Bits [7:0]: Adapter Power State bits [7:0] from Driver 00h = D0; 01h = D1; 02h = D2; 04h = D3 (Cold/Hot); 08h = D4 (Hibernate Notification)
      // Return: Success
      //
      Case(1) {
        If (LEqual(Arg1, 1)) { // test Arg1 for Revision ID: 1
          Store(" Adapter Power State Notification ", Debug)

          //
          // Handle Low Power S0 Idle Capability if enabled
          //
          If(LAnd(LEqual(S0ID, 1),LLess(OSYS, 2015))) {
            //
            // Call GUAM to trigger CS Entry
            //   If Adapter Power State Notification = D1 (Arg3[0]=0x01)
            //
            If (LEqual (And(DerefOf (Index (Arg3,0)), 0xFF), 0x01)) {
              // GUAM - Global User Absent Mode Notification Method
              \GUAM(One) // 0x01 - Power State Standby (CS Entry)
            }
            Store(And(DerefOf (Index (Arg3,1)), 0xFF), Local0)
            //
            // Call GUAM
            // If Display Turn ON Notification (Arg3 [1] == 0) for CS Exit
            //
            If (LEqual (Local0, 0)) {
              // GUAM - Global User Absent Mode Notification Method
              \GUAM(0)
            }
          }

          // Upon notification from driver that the Adapter Power State = D0,
          // check if previous lid event failed.  If it did, retry the lid
          // event here.
          If(LEqual(DerefOf (Index (Arg3,0)), 0)) {
            Store(CLID, Local0)
            If(And(0x80000000,Local0)) {
              And(CLID, 0x0000000F, CLID)
              GLID(CLID)
            }
          }
          Return(0x01)
        }
      }
      //
      // Function Index: 2
      // Display Power State Notification
      // Arg3: Display Power State Bits [15:8]
      // 00h = On
      // 01h = Standby
      // 02h = Suspend
      // 04h = Off
      // 08h = Reduced On
      // Return: Success
      //
     Case(2) {
        if (LEqual(Arg1, 1)) { // test Arg1 for Revision ID: 1

          Store("Display Power State Notification ", Debug)
          Return(0x01)
        }
      }

      //
      // Function Index: 3
      // BIOS POST Completion Notification
      // Return: Success
      //
      Case(3) {
        if (LEqual(Arg1, 1)) { // test Arg1 for Revision ID: 1
          Store("BIOS POST Completion Notification ", Debug)
          Return(0x01)      // Not supported, but no failure
        }
      }

      //
      // Function Index: 4
      // Pre-Hires Set Mode
      // Return: Success
      //
      Case(4) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          Store("Pre-Hires Set Mode ", Debug)
          Return(0x01)      // Not supported, but no failure
        }
      }

      //
      // Function Index: 5
      // Post-Hires Set Mode
      // Return: Success
      //
      Case(5) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          Store("Post-Hires Set Mode ", Debug)
          Return(0x01)      // Not supported, but no failure
        }
      }

      //
      // Function Index: 6
      // SetDisplayDeviceNotification (Display Switch)
      // Return: Success
      //
      Case(6) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          Store("SetDisplayDeviceNotification", Debug)
          Return(0x01)      // Not supported, but no failure
        }
      }

      //
      // Function Index: 7
      // SetBootDevicePreference
      // Return: Success
      //
      Case(7) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          //<TODO> An OEM may elect to implement this method.  In that case,
          // the input values must be saved into non-volatile storage for
          // parsing during the next boot.  The following Sample code is Intel
          // validated implementation.

          Store("SetBootDevicePreference ", Debug)
          And(DerefOf (Index (Arg3,0)), 0xFF, IBTT) // Save the boot display to NVS
          Return(0x01)
        }
      }

      //
      // Function Index: 8
      // SetPanelPreference
      // Return: Success
      //
      Case(8) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          // An OEM may elect to implement this method.  In that case,
          // the input values must be saved into non-volatile storage for
          // parsing during the next boot.  The following Sample code is Intel
          // validated implementation.

          Store("SetPanelPreference ", Debug)

          // Set the panel-related NVRAM variables based the input from the driver.
          And(DerefOf (Index (Arg3,0)), 0xFF, IPSC)

          // Change panel type if a change is requested by the driver (Change if
          // panel type input is non-zero).  Zero=No change requested.
          If(And(DerefOf (Index (Arg3,1)), 0xFF)) {
            And(DerefOf (Index (Arg3,1)), 0xFF, IPAT)
            Decrement(IPAT)    // 0 = no change, so fit to CMOS map
          }
          And(ShiftRight(DerefOf (Index (Arg3,2)), 4), 0x7, IBIA)
          Return(0x01)         // Success
        }
      }

      //
      // Function Index: 9
      // FullScreenDOS
      // Return: Success
      //
      Case(9) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          Store("FullScreenDOS ", Debug)
          Return(0x01)      // Not supported, but no failure
        }
      }

      //
      // Function Index: 10
      // APM Complete
      // Return: Adjusted Lid State
      //
     Case(10) {
        if (LEqual(Arg1, 1)) { // test Arg1 for Revision ID: 1

          Store("APM Complete ", Debug)
          Store(ShiftLeft(LIDS, 8), Local0) // Report the lid state
          Add(Local0, 0x100, Local0)        // Adjust the lid state, 0 = Unknown
          Return(Local0)
        }
      }

      //
      //
      // Function Index: 13
      // GetBootDisplayPreference
      // Arg3 Bits [30:16] : Boot Device Ports
      // Arg3 Bits [7:0] : Boot Device Type
      // Return: Boot device port and Boot device type from saved configuration
      //
     Case(13) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1

          Store("GetBootDisplayPreference ", Debug)
          Or(ShiftLeft(DerefOf (Index (Arg3,3)), 24), ShiftLeft(DerefOf (Index (Arg3,2)), 16), Local0) // Combine Arg3 Bits [31:16]
          And(Local0, 0xEFFF0000, Local0)
          And(Local0, ShiftLeft(DeRefOf(Index(DBTB, IBTT)), 16), Local0)
          Or(IBTT, Local0, Local0) // Arg3 Bits [7:0] = Boot device type
          Return(Local0)
        }
      }

      //
      // Function Index: 14
      // GetPanelDetails
      // Return: Different Panel Settings
      //
      Case(14) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          Store("GetPanelDetails ", Debug)

          // Report the scaling setting
          // Bits [7:0] - Panel Scaling
          // Bits contain the panel scaling user setting from CMOS
          // 00h = On: Auto
          // 01h = On: Force Scaling
          // 02h = Off
          // 03h = Maintain Aspect Ratio

          Store(IPSC, Local0)
          Or(Local0, ShiftLeft(IPAT, 8), Local0)

          // Adjust panel type, 0 = VBT default
          // Bits [15:8] - Panel Type
          // Bits contain the panel type user setting from CMOS
          // 00h = Not Valid, use default Panel Type & Timings from VBT
          // 01h - 0Fh = Panel Number

          Add(Local0, 0x100, Local0)

          // Report the lid state and Adjust it
          // Bits [16] - Lid State
          // Bits contain the current panel lid state
          // 0 = Lid Open
          // 1 = Lid Closed

          Or(Local0, ShiftLeft(LIDS, 16), Local0)
          Add(Local0, 0x10000, Local0)

         // Report the BIA setting
         // Bits [22:20] - Backlight Image Adaptation (BIA) Control
         // Bits contain the backlight image adaptation control user setting from CMOS
         // 000 = VBT Default
         // 001 = BIA Disabled (BLC may still be enabled)
         // 010 - 110 = BIA Enabled at Aggressiveness Level [1 - 5]

          Or(Local0, ShiftLeft(IBIA, 20), Local0)
          Return(Local0)
        }
      }

      //
      // Function Index: 15
      // GetInternalGraphics
      // Return: Different Internal Grahics Settings
      //

      Case(15) {
        if (LEqual(Arg1, 1)){ // test Arg1 for Revision ID: 1
          Store("GetInternalGraphics ", Debug)

          Store(GIVD, Local0)                    // Local0[0]      - VGA mode(1=VGA)
          Xor(Local0, 1, Local0)                 // Invert the VGA mode polarity
          Or(Local0, ShiftLeft(3, 11), Local0)   // Local0[12:11]  - DVMT version (11b = 5.0)

          //
          // Report DVMT 5.0 Total Graphics memory size.
          //
          Or(Local0, ShiftLeft(IDMS, 17), Local0) // Bits 20:17 are for Gfx total memory size
          Return(Local0)
        }
      }

      //
      // Function Index: 16
      // GetAKSV
      // Retrun: 5 bytes of AKSV
      //
      Case(16) {
        if (LEqual(Arg1, 1)) { // test Arg1 for Revision ID: 1

          Store("GetAKSV ", Debug)
          Name (KSVP, Package()
          {
             0x80000000,
             0x8000
          })
          Store(KSV0, Index(KSVP,0)) // First four bytes of AKSV
          Store(KSV1, Index(KSVP,1)) // Fifth byte of AKSV
          Return(KSVP) // Success
        }
      }
    } // End of switch(Arg2)

  } // End of if (ToUUID("3E5B41C6-EB1D-4260-9D15-C71FBADAE414D"))


  Return (Buffer () {0x00})
} // End of _DSM
