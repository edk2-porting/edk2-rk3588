/** @file
  This file contains the device definition of the System Agent
  ACPI reference code.
  Currently defines the device objects for the
  System Agent IPU device

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Device IPUA is the IPU AVStream virtual device and it appears under GFX0
//
Scope (\_SB.PCI0.GFX0)
{
  Device(IPUA) // IPU AVStream virtual device name
  {
    /*
      The identifier for this device (Same as in
      _DOD above). This is required so GFX driver can
      associate a matching device ID for the AVStream
      driver and provide it to PnP (this device ID
      should appear in the INF file of the AVStream
      driver).
    */
    Name(_ADR, 0x00003480)
    /*
      The following is a technique that may be used (per OEM needs) to prevent
      the load of the camera device in one of the following cases:
      - Camera device is fused out
      - If the platform setup requires that in a secured boot the camera device
      should not be enabled
    */
    Method (_STA, 0, NotSerialized) {
      If(LEqual(IPTP,1)){ // IGFX need report IPU AVStream virtual device as GFX0 child
        Return (0xF)
      } Else { // IGFX should NOT report IPU AVStream virtual device as GFX0 child
        Return (0x0)
      }
    }
  } // End SKC0
} // end I.G.D

Scope(\_SB.PCI0.IPU0)
{
//----------------------------------------------------------------------------------------
//  Intel Proprietary Passing LTR information from BIOS to IPU Driver. DSM Method
//
//  Method(_DSM, 0x4, Serialized, 0, {IntObj, BuffObj}, {BuffObj, IntObj, IntObj, PkgObj})
//  Arguments:
//  Arg0: GUID: "9A9E6AB4-E3FC-475D-AD1C-C4789E4CFE90"
//  Arg1: Integer Revision Level (Current revision is 0)
//  Arg2: Integer Function Index
//                0x1 - return UINT 32bit LTR values
//                0x2 - return UINT 32bit Fill Time
//
//-----------------------------------------------------------------------------------------
Method (_DSM, 4, NotSerialized) { // _DSM: Device-Specific Method
    If (LEqual(Arg0, ToUUID("9A9E6AB4-E3FC-475D-AD1C-C4789E4CFE90")))
    {
      // Function 0 : Query Function
      If (LEqual(Arg2, 0))
      {
        // Revision 0
        If (LEqual(Arg1, 0)) // The current revision is 0
        {
          Return(Buffer() { 0x07 }) // There are 2 function defined other than Query.
        } Else {
          Return(0) // Revision mismatch
        }
      }
      // Function 1 : Return UINT 32bit LTR values
      If(LEqual(Arg2, 1))
      {
        Return(0x64503C19)
      }
      // Function 2 : Return UINT 32bit Fill Time
      If(LEqual(Arg2, 2))
      {
        Return(0xFFF0783C)
      }
    }

    Return(0) // Function number or GUID mismatch but normal return.
  }
}
