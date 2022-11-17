/** @file

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

// TODO: Add HID support for touchpad, etc.
#include "thermal.asl"

External (\_SB.SLPB, DeviceObj)

// TODO: Need hooks from BoardAcpiDxe

Scope (_SB)
{
  Method (MPTS, 1, NotSerialized)  // _PTS: Prepare To Sleep
  {
    ^PCI0.LPCB.EC0.ECPS (Arg0)
  }

  Method (MWAK, 1, Serialized)  // _WAK: Wake
  {
    ^PCI0.LPCB.EC0.ECWK (Arg0)

    If ((Arg0 == 3) || (Arg0 == 4))
    {
      Notify (LID0, 0x80) // Status Change
    }
  }

  Method (MS0X, 1, NotSerialized)  // S0ix hook. Porting "GUAM" method - "Global User Absent Mode"
  {
    If (Arg0 == 0)
    {
      /* Exit "Connected Standby" */
#if 1  // EC Notification
      ^PCI0.LPCB.EC0.EOSS = 0
#endif
    }
    ElseIf (Arg0 == 1)
    {
      /* Enter "Connected Standby" */
#if 1  // EC Notification
      ^PCI0.LPCB.EC0.ECSS = 0x08
#endif
    }
  }

  Device (LID0)
  {
    Name (_HID, EisaId ("PNP0C0D") /* Lid Device */)  // _HID: Hardware ID
    Method (_LID, 0, NotSerialized)  // _LID: Lid Status
    {
#ifdef LGMR_ENABLED
      Return (^^PCI0.LPCB.EC0.MLID)
#else
      Return (^^PCI0.LPCB.EC0.ELID)
#endif
    }

    Method (_PSW, 1, NotSerialized)  // _PSW: Power State Wake
    {
      ^^PCI0.LPCB.EC0.EIDW = Arg0
    }

    Name (_PRW, Package () { 0x0A, 3 })  // _PRW: Power Resources for Wake
  }

  // Add a GPE to device
  Scope (SLPB)
  {
    Name (_PRW, Package () { 0x0A, 3 })  // _PRW: Power Resources for Wake
  }
}

Scope (_GPE)
{
  /* TODO - Remaining Level-Triggered GPEs: PCH GPE, PCIe PME, TBT, DTS, GFX SCI and tier-2 (RTD3) */
  Method (_L0A, 0, NotSerialized)
  {
    Notify (\_SB.SLPB, 0x02) // Device Wake
  }
}
