/** @file
  IGD OpRegion/Software SCI Reference Code.
  This file contains Get BIOS Data Area funciton support for
  the Integrated Graphics Device (IGD) OpRegion/Software SCI mechanism

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
Method (GBDA, 0, Serialized)
{
  //
  // Supported calls: Sub-function 0
  //
  If (LEqual(GESF, 0))
  {
    //
    //<NOTE> Reference code is set to Intel's validated implementation.
    //
    Store(0x0000659, PARM)
    Store(Zero, GESF) // Clear the exit parameter
    Return(SUCC) // Success
  }
  //
  // Requested callbacks: Sub-function 1
  //
  If (LEqual(GESF, 1))
  {
    //
    //<NOTE> Call back functions are where the driver calls the
    // system BIOS at function indicated event.
    //
    Store(0x300482, PARM)
    If(LEqual(S0ID, One)){
      Or(PARM, 0x100, PARM) //Request Fn 8 callback in CS systems
    }
    Store(Zero, GESF) // Clear the exit parameter
    Return(SUCC) // Success
  }
  //
  // Get Boot display Preferences: Sub-function 4
  //
  If (LEqual(GESF, 4))
  {
    //
    //<NOTE> Get Boot Display Preferences function.
    //
    And(PARM, 0xEFFF0000, PARM) // PARM[30:16] = Boot device ports
    And(PARM, ShiftLeft(DeRefOf(Index(DBTB, IBTT)), 16), PARM)
    Or(IBTT, PARM, PARM) // PARM[7:0] = Boot device type
    Store(Zero, GESF) // Clear the exit parameter
    Return(SUCC) // Success
  }
  //
  // Panel details: Sub-function 5
  //
  If (LEqual(GESF, 5))
  {
    //
    //<NOTE> Get Panel Details function.
    //
    Store(IPSC, PARM) // Report the scaling setting
    Or(PARM, ShiftLeft(IPAT, 8), PARM)
    Add(PARM, 0x100, PARM) // Adjust panel type, 0 = VBT default
    Or(PARM, ShiftLeft(LIDS, 16), PARM) // Report the lid state
    Add(PARM, 0x10000, PARM) // Adjust the lid state, 0 = Unknown
    Or(PARM, ShiftLeft(IBIA, 20), PARM) // Report the BIA setting
    Store(Zero, GESF)
    Return(SUCC)
  }
  //
  // Internal graphics: Sub-function 7
  //
  If (LEqual(GESF, 7))
  {
    Store(GIVD, PARM) // PARM[0]      - VGA mode(1=VGA)
    Xor(PARM, 1, PARM) // Invert the VGA mode polarity
    Or(PARM, ShiftLeft(3, 11), PARM) // PARM[12:11] - DVMT mode(11b = 5.0)

    //
    // Report DVMT 5.0 Total Graphics memory size.
    //
    Or(PARM, ShiftLeft(IDMS, 17), PARM) // Bits 20:17 are for Gfx total memory size
    Store(1, GESF) // Set the modified settings flag
    Return(SUCC)
  }
  //
  // Spread spectrum clocks: Sub-function 10
  //
  If (LEqual(GESF, 10))
  {
    Store(0, PARM) // Assume SSC is disabled
    If(ISSC)
    {
      Or(PARM, 3, PARM) // If SSC enabled, return SSC1+Enabled
    }
    Store(0, GESF) // Set the modified settings flag
    Return(SUCC) // Success
  }

  If (LEqual(GESF, 11))
  {
    Store(KSV0, PARM) // First four bytes of AKSV
    Store(KSV1, GESF) // Fifth byte of AKSV

    Return(SUCC) // Success
  }
  //
  // A call to a reserved "Get BIOS data" function was received.
  //
  Store(Zero, GESF) // Clear the exit parameter
  Return(CRIT) // Reserved, "Critical failure"
}
