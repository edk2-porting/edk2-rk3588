/** @file

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope (_TZ)
{
  Name (CRT0, 0)
  Name (PSV0, 0)
  ThermalZone (TZ01)
  {
    Method (_TMP, 0, Serialized)  // _TMP: Temperature
    {
#ifdef LGMR_ENABLED
      Local0 = \_SB.PCI0.LPCB.EC0.MS0T
//    Local1 = \_SB.PCI0.LPCB.EC0.MCSS
      Local2 = \_SB.PCI0.LPCB.EC0.MOSD
#else
      Local0 = \_SB.PCI0.LPCB.EC0.ES0T
//    Local1 = \_SB.PCI0.LPCB.EC0.ESSF  // "MCSS": Considering neighbouring bits, likely
                                        // "ESSF" in thermals, not "ECSS" in notify
      Local2 = \_SB.PCI0.LPCB.EC0.EOSD
#endif
      If (Local2)  // Thermal trip
      {
        If (Local0 <= CRT0)
        {
          Local0 = (CRT0 + 2)
        }
      }

      Return (C2K (Local0))
    }

    Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
    {
#ifdef LGMR_ENABLED
      Local0 = \_SB.PCI0.LPCB.EC0.MSC0
#else
      Local0 = \_SB.PCI0.LPCB.EC0.ESC0
#endif
      If ((Local0 >= 128) || (Local0 < 30))
      {
        Local0 = 120
      }

      CRT0 = Local0
      Return (C2K (Local0))
    }

    Method (_SCP, 1, Serialized)  // _SCP: Set Cooling Policy
    {
      If (ECOK)
      {
        \_SB.PCI0.LPCB.EC0.SCPM = Arg0
      }
      Else
      {
        /* MBEC: Called SMI function 0x12 */
//      \_SB.PCI0.LPCB.EC0.MBEC (0x90, 0xFE, Arg0)  // SCPM = Arg0
      }
    }

    Method (_PSV, 0, Serialized)  // _PSV: Passive Temperature
    {
#ifdef LGMR_ENABLED
      Local0 = \_SB.PCI0.LPCB.EC0.MSP0
#else
      Local0 = \_SB.PCI0.LPCB.EC0.ESP0
#endif
      If ((Local0 >= 128) || (Local0 < 30))
      {
        Local0 = 30
      }

      PSV0 = Local0
      Return (C2K (Local0))
    }
  }

  ThermalZone (TZ00)
  {
    Method (_TMP, 0, Serialized)  // _TMP: Temperature
    {
#ifdef LGMR_ENABLED
      Local0 = \_SB.PCI0.LPCB.EC0.MS1T
#else
      Local0 = \_SB.PCI0.LPCB.EC0.ES1T
#endif
      Return (C2K (Local0))
    }

    Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
    {
#ifdef LGMR_ENABLED
      Local0 = \_SB.PCI0.LPCB.EC0.MSC1
#else
      Local0 = \_SB.PCI0.LPCB.EC0.ESC1
#endif
      If ((Local0 >= 128) || (Local0 < 30))
      {
        Local0 = 120
      }

      Return (C2K (Local0))
    }
  }

  Method (C2K, 1, NotSerialized)
  {
    Local0 = Arg0
    If ((Local0 >= 127) || (Local0 <= 16))
    {
      Local0 = 30
    }

    Local0 = ((Local0 * 10) + 2732)  // Celsius to Kelvin
    Return (Local0)
  }
}
