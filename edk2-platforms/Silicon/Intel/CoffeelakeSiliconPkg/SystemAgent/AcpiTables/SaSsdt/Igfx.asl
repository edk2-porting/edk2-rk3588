/** @file
  This file contains the IGD OpRegion/Software ACPI Reference
  Code.
  It defines the methods to enable/disable output switching,
  store display switching and LCD brightness BIOS control
  and return valid addresses for all display device encoders
  present in the system, etc.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

External(\ECST, MethodObj)
External(\PBCL, MethodObj)
External(HDOS, MethodObj)
External(\ECON, IntObj)
External(\PNHM, IntObj)
External(OSYS, IntObj)
External(CPSC)
External(\GUAM, MethodObj)
External(DSEN)
External(S0ID)

Name(TMP1,Package() {0xFFFFFFFF})
Name(TMP2,Package() {0xFFFFFFFF, 0xFFFFFFFF})
Name(TMP3,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMP4,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMP5,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMP6,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMP7,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMP8,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF})
Name(TMP9,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF})
Name(TMPA,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF })
Name(TMPB,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMPC,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMPD,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMPE,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF})
Name(TMPF,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF})
Name(TMPG,Package() {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                     0xFFFFFFFF, 0xFFFFFFFF})

// Enable/Disable Output Switching.  In WIN2K/WINXP, _DOS = 0 will
// get called during initialization to prepare for an ACPI Display
// Switch Event.  During an ACPI Display Switch, the OS will call
// _DOS = 2 immediately after a Notify=0x80 to temporarily disable
// all Display Switching.  After ACPI Display Switching is complete,
// the OS will call _DOS = 0 to re-enable ACPI Display Switching.
Method(_DOS,1)
{
  //
  // Store Display Switching and LCD brightness BIOS control bit
  //
  Store(And(Arg0,7),DSEN)

  If(LEqual(And(Arg0,  0x3), 0))     // If _DOS[1:0]=0
  {
    If(CondRefOf(HDOS))
    {
      HDOS()
    }
  }
}

//
// Enumerate the Display Environment.  This method will return
// valid addresses for all display device encoders present in the
// system.  The Miniport Driver will reject the addresses for every
// encoder that does not have an attached display device.  After
// enumeration is complete, the OS will call the _DGS methods
// during a display switch only for the addresses accepted by the
// Miniport Driver.  For hot-insertion and removal of display
// devices, a re-enumeration notification will be required so the
// address of the newly present display device will be accepted by
// the Miniport Driver.
//
Method(_DOD,0)
{
  If (LEqual(IPTP,1)) {
    //
    // Increment number of devices if IPU is enabled
    //
    Store(1, NDID)
  } Else {
    Store(0, NDID)
  }

  If(LNotEqual(DIDL, Zero))
  {
    Store(SDDL(DIDL),DID1)
  }
  If(LNotEqual(DDL2, Zero))
  {
    Store(SDDL(DDL2),DID2)
  }
  If(LNotEqual(DDL3, Zero))
  {
    Store(SDDL(DDL3),DID3)
  }
  If(LNotEqual(DDL4, Zero))
  {
    Store(SDDL(DDL4),DID4)
  }
  If(LNotEqual(DDL5, Zero))
  {
    Store(SDDL(DDL5),DID5)
  }
  If(LNotEqual(DDL6, Zero))
  {
    Store(SDDL(DDL6),DID6)
  }
  If(LNotEqual(DDL7, Zero))
  {
    Store(SDDL(DDL7),DID7)
  }
  If(LNotEqual(DDL8, Zero))
  {
    Store(SDDL(DDL8),DID8)
  }
  If(LNotEqual(DDL9, Zero))
  {
    Store(SDDL(DDL9),DID9)
  }
  If(LNotEqual(DD10, Zero))
  {
    Store(SDDL(DD10),DIDA)
  }
  If(LNotEqual(DD11, Zero))
  {
    Store(SDDL(DD11),DIDB)
  }
  If(LNotEqual(DD12, Zero))
  {
    Store(SDDL(DD12),DIDC)
  }
  If(LNotEqual(DD13, Zero))
  {
    Store(SDDL(DD13),DIDD)
  }
  If(LNotEqual(DD14, Zero))
  {
    Store(SDDL(DD14),DIDE)
  }
  If(LNotEqual(DD15, Zero))
  {
    Store(SDDL(DD15),DIDF)
  }

  //
  // Enumerate the encoders. Note that for
  // current silicon, the maximum number of encoders
  // possible is 15.
  //
  If(LEqual(NDID,1))
  {
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP1,0))
    } Else {
      Store(Or(0x10000,DID1),Index(TMP1,0))
    }
    Return(TMP1)
  }

  If(LEqual(NDID,2))
  {
    Store(Or(0x10000,DID1),Index(TMP2,0))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP2,1))
    } Else {
      Store(Or(0x10000,DID2),Index(TMP2,1))
    }
    Return(TMP2)
  }

  If(LEqual(NDID,3))
  {
    Store(Or(0x10000,DID1),Index(TMP3,0))
    Store(Or(0x10000,DID2),Index(TMP3,1))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP3,2))
    } Else {
      Store(Or(0x10000,DID3),Index(TMP3,2))
    }
    Return(TMP3)
  }

  If(LEqual(NDID,4))
  {
    Store(Or(0x10000,DID1),Index(TMP4,0))
    Store(Or(0x10000,DID2),Index(TMP4,1))
    Store(Or(0x10000,DID3),Index(TMP4,2))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP4,3))
    } Else {
      Store(Or(0x10000,DID4),Index(TMP4,3))
    }
    Return(TMP4)
  }

  If(LEqual(NDID,5))
  {
    Store(Or(0x10000,DID1),Index(TMP5,0))
    Store(Or(0x10000,DID2),Index(TMP5,1))
    Store(Or(0x10000,DID3),Index(TMP5,2))
    Store(Or(0x10000,DID4),Index(TMP5,3))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP5,4))
    } Else {
      Store(Or(0x10000,DID5),Index(TMP5,4))
    }
    Return(TMP5)
  }

  If(LEqual(NDID,6))
  {
    Store(Or(0x10000,DID1),Index(TMP6,0))
    Store(Or(0x10000,DID2),Index(TMP6,1))
    Store(Or(0x10000,DID3),Index(TMP6,2))
    Store(Or(0x10000,DID4),Index(TMP6,3))
    Store(Or(0x10000,DID5),Index(TMP6,4))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP6,5))
    } Else {
      Store(Or(0x10000,DID6),Index(TMP6,5))
    }
    Return(TMP6)
  }

  If(LEqual(NDID,7))
  {
    Store(Or(0x10000,DID1),Index(TMP7,0))
    Store(Or(0x10000,DID2),Index(TMP7,1))
    Store(Or(0x10000,DID3),Index(TMP7,2))
    Store(Or(0x10000,DID4),Index(TMP7,3))
    Store(Or(0x10000,DID5),Index(TMP7,4))
    Store(Or(0x10000,DID6),Index(TMP7,5))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP7,6))
    } Else {
      Store(Or(0x10000,DID7),Index(TMP7,6))
    }
    Return(TMP7)
  }

  If(LEqual(NDID,8))
  {
    Store(Or(0x10000,DID1),Index(TMP8,0))
    Store(Or(0x10000,DID2),Index(TMP8,1))
    Store(Or(0x10000,DID3),Index(TMP8,2))
    Store(Or(0x10000,DID4),Index(TMP8,3))
    Store(Or(0x10000,DID5),Index(TMP8,4))
    Store(Or(0x10000,DID6),Index(TMP8,5))
    Store(Or(0x10000,DID7),Index(TMP8,6))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP8,7))
    } Else {
      Store(Or(0x10000,DID8),Index(TMP8,7))
    }
    Return(TMP8)
  }

  If(LEqual(NDID,9))
  {
    Store(Or(0x10000,DID1),Index(TMP9,0))
    Store(Or(0x10000,DID2),Index(TMP9,1))
    Store(Or(0x10000,DID3),Index(TMP9,2))
    Store(Or(0x10000,DID4),Index(TMP9,3))
    Store(Or(0x10000,DID5),Index(TMP9,4))
    Store(Or(0x10000,DID6),Index(TMP9,5))
    Store(Or(0x10000,DID7),Index(TMP9,6))
    Store(Or(0x10000,DID8),Index(TMP9,7))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMP9,8))
    } Else {
      Store(Or(0x10000,DID9),Index(TMP9,8))
    }
    Return(TMP9)
  }

  If(LEqual(NDID,0x0A))
  {
    Store(Or(0x10000,DID1),Index(TMPA,0))
    Store(Or(0x10000,DID2),Index(TMPA,1))
    Store(Or(0x10000,DID3),Index(TMPA,2))
    Store(Or(0x10000,DID4),Index(TMPA,3))
    Store(Or(0x10000,DID5),Index(TMPA,4))
    Store(Or(0x10000,DID6),Index(TMPA,5))
    Store(Or(0x10000,DID7),Index(TMPA,6))
    Store(Or(0x10000,DID8),Index(TMPA,7))
    Store(Or(0x10000,DID9),Index(TMPA,8))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMPA,9))
    } Else {
      Store(Or(0x10000,DIDA),Index(TMPA,9))
    }
    Return(TMPA)
  }

  If(LEqual(NDID,0x0B))
  {
    Store(Or(0x10000,DID1),Index(TMPB,0))
    Store(Or(0x10000,DID2),Index(TMPB,1))
    Store(Or(0x10000,DID3),Index(TMPB,2))
    Store(Or(0x10000,DID4),Index(TMPB,3))
    Store(Or(0x10000,DID5),Index(TMPB,4))
    Store(Or(0x10000,DID6),Index(TMPB,5))
    Store(Or(0x10000,DID7),Index(TMPB,6))
    Store(Or(0x10000,DID8),Index(TMPB,7))
    Store(Or(0x10000,DID9),Index(TMPB,8))
    Store(Or(0x10000,DIDA),Index(TMPB,9))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMPB,10))
    } Else {
      Store(Or(0x10000,DIDB),Index(TMPB,10))
    }
    Return(TMPB)
  }

  If(LEqual(NDID,0x0C))
  {
    Store(Or(0x10000,DID1),Index(TMPC,0))
    Store(Or(0x10000,DID2),Index(TMPC,1))
    Store(Or(0x10000,DID3),Index(TMPC,2))
    Store(Or(0x10000,DID4),Index(TMPC,3))
    Store(Or(0x10000,DID5),Index(TMPC,4))
    Store(Or(0x10000,DID6),Index(TMPC,5))
    Store(Or(0x10000,DID7),Index(TMPC,6))
    Store(Or(0x10000,DID8),Index(TMPC,7))
    Store(Or(0x10000,DID9),Index(TMPC,8))
    Store(Or(0x10000,DIDA),Index(TMPC,9))
    Store(Or(0x10000,DIDB),Index(TMPC,10))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMPC,11))
    } Else {
      Store(Or(0x10000,DIDC),Index(TMPC,11))
    }
    Return(TMPC)
  }

  If(LEqual(NDID,0x0D))
  {
    Store(Or(0x10000,DID1),Index(TMPD,0))
    Store(Or(0x10000,DID2),Index(TMPD,1))
    Store(Or(0x10000,DID3),Index(TMPD,2))
    Store(Or(0x10000,DID4),Index(TMPD,3))
    Store(Or(0x10000,DID5),Index(TMPD,4))
    Store(Or(0x10000,DID6),Index(TMPD,5))
    Store(Or(0x10000,DID7),Index(TMPD,6))
    Store(Or(0x10000,DID8),Index(TMPD,7))
    Store(Or(0x10000,DID9),Index(TMPD,8))
    Store(Or(0x10000,DIDA),Index(TMPD,9))
    Store(Or(0x10000,DIDB),Index(TMPD,10))
    Store(Or(0x10000,DIDC),Index(TMPD,11))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMPD,12))
    } Else {
      Store(Or(0x10000,DIDD),Index(TMPD,12))
    }
    Return(TMPD)
  }

  If(LEqual(NDID,0x0E))
  {
    Store(Or(0x10000,DID1),Index(TMPE,0))
    Store(Or(0x10000,DID2),Index(TMPE,1))
    Store(Or(0x10000,DID3),Index(TMPE,2))
    Store(Or(0x10000,DID4),Index(TMPE,3))
    Store(Or(0x10000,DID5),Index(TMPE,4))
    Store(Or(0x10000,DID6),Index(TMPE,5))
    Store(Or(0x10000,DID7),Index(TMPE,6))
    Store(Or(0x10000,DID8),Index(TMPE,7))
    Store(Or(0x10000,DID9),Index(TMPE,8))
    Store(Or(0x10000,DIDA),Index(TMPE,9))
    Store(Or(0x10000,DIDB),Index(TMPE,10))
    Store(Or(0x10000,DIDC),Index(TMPE,11))
    Store(Or(0x10000,DIDD),Index(TMPE,12))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMPE,13))
    } Else {
      Store(Or(0x10000,DIDE),Index(TMPE,13))
    }
    Return(TMPE)
  }

  If(LEqual(NDID,0x0F))
  {
    Store(Or(0x10000,DID1),Index(TMPF,0))
    Store(Or(0x10000,DID2),Index(TMPF,1))
    Store(Or(0x10000,DID3),Index(TMPF,2))
    Store(Or(0x10000,DID4),Index(TMPF,3))
    Store(Or(0x10000,DID5),Index(TMPF,4))
    Store(Or(0x10000,DID6),Index(TMPF,5))
    Store(Or(0x10000,DID7),Index(TMPF,6))
    Store(Or(0x10000,DID8),Index(TMPF,7))
    Store(Or(0x10000,DID9),Index(TMPF,8))
    Store(Or(0x10000,DIDA),Index(TMPF,9))
    Store(Or(0x10000,DIDB),Index(TMPF,10))
    Store(Or(0x10000,DIDC),Index(TMPF,11))
    Store(Or(0x10000,DIDD),Index(TMPF,12))
    Store(Or(0x10000,DIDE),Index(TMPF,13))
    If (LEqual(IPTP,1)) {
      //
      // IGFX need report IPUA as GFX0 child
      //
      Store(0x00023480,Index(TMPF,14))
    } Else {
      Store(Or(0x10000,DIDF),Index(TMPF,14))
    }
    Return(TMPF)
  }

  If(LEqual(NDID,0x10))
  {
    Store(Or(0x10000,DID1),Index(TMPG,0))
    Store(Or(0x10000,DID2),Index(TMPG,1))
    Store(Or(0x10000,DID3),Index(TMPG,2))
    Store(Or(0x10000,DID4),Index(TMPG,3))
    Store(Or(0x10000,DID5),Index(TMPG,4))
    Store(Or(0x10000,DID6),Index(TMPG,5))
    Store(Or(0x10000,DID7),Index(TMPG,6))
    Store(Or(0x10000,DID8),Index(TMPG,7))
    Store(Or(0x10000,DID9),Index(TMPG,8))
    Store(Or(0x10000,DIDA),Index(TMPG,9))
    Store(Or(0x10000,DIDB),Index(TMPG,10))
    Store(Or(0x10000,DIDC),Index(TMPG,11))
    Store(Or(0x10000,DIDD),Index(TMPG,12))
    Store(Or(0x10000,DIDE),Index(TMPG,13))
    Store(Or(0x10000,DIDF),Index(TMPG,14))
    //
    // IGFX need report IPUA as GFX0 child
    // NDID can only be 0x10 if IPU is enabled
    //
    Store(0x00023480,Index(TMPG,15))
    Return(TMPG)
  }

  //
  // If nothing else, return Unknown LFP.
  // (Prevents compiler warning.)
  //
  Return(Package() {0x00000400})
}

Device(DD01)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID1),0x400))
    {
      Store(0x1, EDPV)
      Store(NXD1, NXDX)
      Store(DID1, DIDX)
      Return(1)
    }
    If(LEqual(DID1,0))
    {
      Return(1)
    }
    Else
    {
      Return(And(0xFFFF,DID1))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    Return(CDDS(DID1))
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD1)
    }
    Return(NDDS(DID1))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD02)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID2),0x400))
    {
      Store(0x2, EDPV)
      Store(NXD2, NXDX)
      Store(DID2, DIDX)
      Return(2)
    }
    If(LEqual(DID2,0))
    {
      Return(2)
    }
    Else
    {
      Return(And(0xFFFF,DID2))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(LIDS,0))
    {
      Return(0x0)
    }
    Return(CDDS(DID2))
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    //
    // Return the Next State.
    //
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD2)
    }
    Return(NDDS(DID2))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD03)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID3),0x400))
    {
      Store(0x3, EDPV)
      Store(NXD3, NXDX)
      Store(DID3, DIDX)
      Return(3)
    }
    If(LEqual(DID3,0))
    {
      Return(3)
    }
    Else
    {
    Return(And(0xFFFF,DID3))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID3,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID3))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD3)
    }
    Return(NDDS(DID3))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD04)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID4),0x400))
    {
      Store(0x4, EDPV)
      Store(NXD4, NXDX)
      Store(DID4, DIDX)
      Return(4)
    }
    If(LEqual(DID4,0))
    {
      Return(4)
    }
    Else
    {
      Return(And(0xFFFF,DID4))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID4,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID4))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD4)
    }
    Return(NDDS(DID4))
  }

  //
  // Device Set State. (See table above.)
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD05)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID5),0x400))
    {
      Store(0x5, EDPV)
      Store(NXD5, NXDX)
      Store(DID5, DIDX)
      Return(5)
    }
    If(LEqual(DID5,0))
    {
      Return(5)
    }
    Else
    {
      Return(And(0xFFFF,DID5))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID5,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID5))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD5)
    }
    Return(NDDS(DID5))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD06)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID6),0x400))
    {
      Store(0x6, EDPV)
      Store(NXD6, NXDX)
      Store(DID6, DIDX)
      Return(6)
    }
    If(LEqual(DID6,0))
    {
      Return(6)
    }
    Else
    {
      Return(And(0xFFFF,DID6))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID6,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID6))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD6)
    }
    Return(NDDS(DID6))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD07)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID7),0x400))
    {
      Store(0x7, EDPV)
      Store(NXD7, NXDX)
      Store(DID7, DIDX)
      Return(7)
    }
    If(LEqual(DID7,0))
    {
      Return(7)
    }
    Else
    {
      Return(And(0xFFFF,DID7))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID7,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID7))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD7)
    }
    Return(NDDS(DID7))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD08)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID8),0x400))
    {
      Store(0x8, EDPV)
      Store(NXD8, NXDX)
      Store(DID8, DIDX)
      Return(8)
    }
    If(LEqual(DID8,0))
    {
      Return(8)
    }
    Else
    {
      Return(And(0xFFFF,DID8))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID8,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID8))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DID8))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD09)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DID9),0x400))
    {
      Store(0x9, EDPV)
      Store(NXD8, NXDX)
      Store(DID9, DIDX)
      Return(9)
    }
    If(LEqual(DID9,0))
    {
      Return(9)
    }
    Else
    {
      Return(And(0xFFFF,DID9))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DID9,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DID9))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DID9))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD0A)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DIDA),0x400))
    {
      Store(0xA, EDPV)
      Store(NXD8, NXDX)
      Store(DIDA, DIDX)
      Return(0x0A)
    }
    If(LEqual(DIDA,0))
    {
      Return(0x0A)
    }
    Else
    {
      Return(And(0xFFFF,DIDA))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DIDA,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DIDA))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DIDA))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD0B)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DIDB),0x400))
    {
      Store(0xB, EDPV)
      Store(NXD8, NXDX)
      Store(DIDB, DIDX)
      Return(0X0B)
    }
    If(LEqual(DIDB,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(And(0xFFFF,DIDB))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DIDB,0))
    {
      Return(0x0B)
    }
    Else
    {
      Return(CDDS(DIDB))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DIDB))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD0C)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DIDC),0x400))
    {
      Store(0xC, EDPV)
      Store(NXD8, NXDX)
      Store(DIDC, DIDX)
      Return(0X0C)
    }
    If(LEqual(DIDC,0))
    {
      Return(0x0C)
    }
    Else
    {
      Return(And(0xFFFF,DIDC))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DIDC,0))
    {
      Return(0x0C)
    }
    Else
    {
      Return(CDDS(DIDC))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DIDC))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD0D)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DIDD),0x400))
    {
      Store(0xD, EDPV)
      Store(NXD8, NXDX)
      Store(DIDD, DIDX)
      Return(0X0D)
    }
    If(LEqual(DIDD,0))
    {
      Return(0x0D)
    }
    Else
    {
      Return(And(0xFFFF,DIDD))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DIDD,0))
    {
      Return(0x0D)
    }
    Else
    {
      Return(CDDS(DIDD))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DIDD))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD0E)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DIDE),0x400))
    {
      Store(0xE, EDPV)
      Store(NXD8, NXDX)
      Store(DIDE, DIDX)
      Return(0X0E)
    }
    If(LEqual(DIDE,0))
    {
      Return(0x0E)
    }
    Else
    {
      Return(And(0xFFFF,DIDE))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DIDE,0))
    {
      Return(0x0E)
    }
    Else
    {
      Return(CDDS(DIDE))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DIDE))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

Device(DD0F)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(And(0x0F00,DIDF),0x400))
    {
      Store(0xF, EDPV)
      Store(NXD8, NXDX)
      Store(DIDF, DIDX)
      Return(0X0F)
    }
    If(LEqual(DIDF,0))
    {
      Return(0x0F)
    }
    Else
    {
      Return(And(0xFFFF,DIDF))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(DIDC,0))
    {
      Return(0x0F)
    }
    Else
    {
      Return(CDDS(DIDF))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXD8)
    }
    Return(NDDS(DIDF))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }
}

//
//Device for eDP
//
Device(DD1F)
{
  //
  // Return Unique ID.
  //
  Method(_ADR,0,Serialized)
  {
    If(LEqual(EDPV, 0x0))
    {
      Return(0x1F)
    }
    Else
    {
      Return(And(0xFFFF,DIDX))
    }
  }

  //
  // Return the Current Status.
  //
  Method(_DCS,0)
  {
    If(LEqual(EDPV, 0x0))
    {
      Return(0x00)
    }
    Else
    {
      Return(CDDS(DIDX))
    }
  }

  //
  // Query Graphics State (active or inactive).
  //
  Method(_DGS,0)
  {
    If(LAnd(LEqual(And(SGMD,0x7F),0x01),CondRefOf(SNXD)))
    {
      Return (NXDX)
    }
    Return(NDDS(DIDX))
  }

  //
  // Device Set State.
  //
  Method(_DSS,1)
  {
    DSST(Arg0)
  }

  //
  // Query List of Brightness Control Levels Supported.
  //
  Method(_BCL,0)
  {
    //
    // List of supported brightness levels in the following sequence.
    // Level when machine has full power.
    // Level when machine is on batteries.
    // Other supported levels.
    //
    If(CondRefOf(\PBCL)) {
      Return (PBCL())
    } Else {
      Return(Package(){80, 50, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100})
    }
  }

  //
  // Set the Brightness Level.
  //
  Method (_BCM,1)
  {
    //
    // Set the requested level if it is between 0 and 100%.
    //
    If(LAnd(LGreaterEqual(Arg0,0),LLessEqual(Arg0,100)))
    {
      \_SB.PCI0.GFX0.AINT(1, Arg0)
      Store(Arg0,BRTL)  // Store Brightness Level.
    }
  }

  //
  // Brightness Query Current level.
  //
  Method (_BQC,0)
  {
    Return(BRTL)
  }
}

Method(SDDL,1)
{
  Increment(NDID)
  Store(And(Arg0,0xF0F),Local0)
  Or(0x80000000,Local0, Local1)
  If(LEqual(DIDL,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL2,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL3,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL4,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL5,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL6,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL7,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL8,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DDL9,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DD10,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DD11,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DD12,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DD13,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DD14,Local0))
  {
    Return(Local1)
  }
  If(LEqual(DD15,Local0))
  {
    Return(Local1)
  }
  Return(0)
}

Method(CDDS,1)
{
  Store(And(Arg0,0xF0F),Local0)

  If(LEqual(0, Local0))
  {
    Return(0x1D)
  }
  If(LEqual(CADL, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL2, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL3, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL4, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL5, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL6, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL7, Local0))
  {
    Return(0x1F)
  }
  If(LEqual(CAL8, Local0))
  {
    Return(0x1F)
  }
  Return(0x1D)
}

Method(NDDS,1)
{
  Store(And(Arg0,0xF0F),Local0)

  If(LEqual(0, Local0))
  {
    Return(0)
  }
  If(LEqual(NADL, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL2, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL3, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL4, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL5, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL6, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL7, Local0))
  {
    Return(1)
  }
  If(LEqual(NDL8, Local0))
  {
    Return(1)
  }
  Return(0)
}

//
// Device Set State Table
//  BIT31  BIT30  Execution
//  0  0  Don't implement.
//  0  1  Cache change.  Nothing to Implement.
//  1  0  Don't Implement.
//  1  1  Display Switch Complete.  Implement.
//
Method(DSST,1)
{
  If(LEqual(And(Arg0,0xC0000000),0xC0000000))
  {
    //
    // State change was performed by the
    // Video Drivers.  Simply update the
    // New State.
    //
    Store(NSTE,CSTE)
  }
}

//
// Include IGD OpRegion/Software SCI interrupt handler/DSM which is used by
// the graphics drivers to request data from system BIOS.
//
include ("IgfxOpRn.asl")
include ("IgfxDsm.asl")
