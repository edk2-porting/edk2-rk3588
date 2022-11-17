/** @file
  This file contains the system BIOS call back functionality for the
  OpRegion/Software SCI mechanism.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

Method (SBCB, 0, Serialized)
{
  //
  // Supported Callbacks: Sub-function 0
  //
  If (LEqual(GESF, 0x0))
  {
    //
    //<NOTE> An OEM may support the driver->SBIOS status callbacks, but
    // the supported callbacks value must be modified.  The code that is
    // executed upon reception of the callbacks must be also be updated
    // to perform the desired functionality.
    //
    Store(0x00000000, PARM)   // No callbacks supported
    //Store(0x000787FD, PARM) // Used for Intel test implementaion
    Store(0x000F87DD, PARM)
    Store(Zero, GESF)         // Clear the exit parameter
    Return(SUCC)              // "Success"
  }
  //
  // BIOS POST Completion: Sub-function 1
  //
  If (LEqual(GESF, 1))
  {
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // Pre-Hires Set Mode: Sub-function 3
  //
  If (LEqual(GESF, 3))
  {
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // Post-Hires Set Mode: Sub-function 4
  //
  If (LEqual(GESF, 4))
  {
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // Display Switch: Sub-function 5
  //
  If (LEqual(GESF, 5))
  {
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // Adapter Power State: Sub-function 7
  //
  If (LEqual(GESF, 7))
  {
    //
    // Handle Low Power S0 Idle Capability if enabled
    //
    If(LAnd(LEqual(S0ID, 1),LLess(OSYS, 2015))) {
      //
      // Call GUAM to trigger CS Entry
      //   If Adapter Power State Notification = D1 (PARM[7:0]=0x01)
      //
      If (LEqual (And(PARM,0xFF), 0x01)) {
        // GUAM - Global User Absent Mode Notification Method
        \GUAM(One) // 0x01 - Power State Standby (CS Entry)
      }
      If (LEqual (And(PARM,0xFF), 0x00)) {
        // GUAM - Global User Absent Mode Notification Method
        \GUAM(0)
      }
    }
    //
    // Upon notification from driver that the Adapter Power State = D0,
    // check if previous lid event failed.  If it did, retry the lid
    // event here.
    //
    If(LEqual(PARM, 0))
    {
      Store(CLID, Local0)
      If(And(0x80000000,Local0))
      {
        And(CLID, 0x0000000F, CLID)
        GLID(CLID)
      }
    }
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // Display Power State: Sub-function 8
  //
  If (LEqual(GESF, 8))
  {
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // Set Boot Display: Sub-function 9
  //
  If (LEqual(GESF, 9))
  {
    //
    //<NOTE> An OEM may elect to implement this method.  In that case,
    // the input values must be saved into non-volatile storage for
    // parsing during the next boot.  The following Sample code is Intel
    // validated implementation.
    //
    And(PARM, 0xFF, IBTT) // Save the boot display to NVS
    Store(Zero, GESF)     // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)          // Reserved, "Critical failure"
  }
  //
  // Set Panel Details: Sub-function 10 (0Ah)
  //
  If (LEqual(GESF, 10))
  {
    //
    //<NOTE> An OEM may elect to implement this method.  In that case,
    // the input values must be saved into non-volatile storage for
    // parsing during the next boot.  The following Sample code is Intel
    // validated implementation.
    // Set the panel-related NVRAM variables based the input from the driver.
    //
    And(PARM, 0xFF, IPSC)
    //
    // Change panel type if a change is requested by the driver (Change if
    // panel type input is non-zero).  Zero=No change requested.
    //
    If(And(ShiftRight(PARM, 8), 0xFF))
    {
      And(ShiftRight(PARM, 8), 0xFF, IPAT)
      Decrement(IPAT)    // 0 = no change, so fit to CMOS map
    }
    And(ShiftRight(PARM, 20), 0x7, IBIA)
    Store(Zero, GESF)    // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)         // Success
  }
  //
  // Set Internal Graphics: Sub-function 11 (0Bh)
  //
  If (LEqual(GESF, 11))
  {
    //
    //<NOTE> An OEM may elect to implement this method.  In that case,
    // the input values must be saved into non-volatile storage for
    // parsing during the next boot.  The following Sample code is Intel
    // validated implementation.
    //
    And(ShiftRight(PARM, 1), 1, IF1E)      // Program the function 1 option
    If(And(PARM, ShiftLeft(0xF, 13)))      // Use fixed memory if fixed size != 0
    {
      //
      // Fixed memory
      //
      And(ShiftRight(PARM, 13), 0xF, IDMS) // Program fixed memory size
    }
    Else
    {
      //
      // DVMT memory
      //
      And(ShiftRight(PARM, 17), 0xF, IDMS) // Program fixed memory size
    }
    Store(Zero, GESF)                      // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)                           // Success
  }
  //
  // Post-Hires to DOS FS: Sub-function 16 (10h)
  //
  If (LEqual(GESF, 16))
  {
    Store(Zero, GESF) // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)      // Not supported, but no failure
  }
  //
  // APM Complete:  Sub-function 17 (11h)
  //
  If (LEqual(GESF, 17))
  {
    Store(ShiftLeft(LIDS, 8), PARM) // Report the lid state
    Add(PARM, 0x100, PARM)          // Adjust the lid state, 0 = Unknown
    Store(Zero, GESF)               // Clear the exit parameter
    Return(SUCC)                    // Not supported, but no failure
  }
  //
  // Set Spread Spectrum Clocks: Sub-function 18 (12h)
  //
  If (LEqual(GESF, 18))
  {
    //
    //<NOTE> An OEM may elect to implement this method.  In that case,
    // the input values must be saved into non-volatile storage for
    // parsing during the next boot.  The following Sample code is Intel
    // validated implementation.
    //
    If(And(PARM, 1))
    {
      If(LEqual(ShiftRight(PARM, 1), 1))
      {
        Store(1, ISSC)  // Enable HW SSC, only for clock 1
      }
      Else
      {
        Store(Zero, GESF)
        Return(CRIT)    // Failure, as the SSC clock must be 1
      }
    }
    Else
    {
      Store(0, ISSC)    // Disable SSC
    }
    Store(Zero, GESF)   // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)        // Success
  }
  //
  // Post VBE/PM Callback: Sub-function 19 (13h)
  //
  If (LEqual(GESF, 19))
  {
    Store(Zero, GESF)  // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)       // Not supported, but no failure
  }
  //
  // Set PAVP Data: Sub-function 20 (14h)
  //
  If (LEqual(GESF, 20))
  {
    And(PARM, 0xF, PAVP) // Store PAVP info
    Store(Zero, GESF)    // Clear the exit parameter
    Store(Zero, PARM)
    Return(SUCC)         // Success
  }

  //
  // A call to a reserved "System BIOS callbacks" function was received
  //
  Store(Zero, GESF) // Clear the exit parameter
  Return(SUCC)      // Reserved, "Critical failure"
}
