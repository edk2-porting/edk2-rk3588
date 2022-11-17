/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  // General Purpose Events.  This Scope handles the Run-time and
  // Wake-time SCIs.  The specific method called will be determined by
  // the _Lxx value, where xx equals the bit location in the General
  // Purpose Event register(s).

    //
    // If the Root Port is enabled, run PCI_EXP_STS handler
    //
    If(LNotEqual(\_SB.PC00.RP01.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP01.HPME()
      Notify(\_SB.PC00.RP01, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP02.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP02.HPME()
      Notify(\_SB.PC00.RP02, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP03.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP03.HPME()
      Notify(\_SB.PC00.RP03, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP04.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP04.HPME()
      Notify(\_SB.PC00.RP04, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP05.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP05.HPME()
      Notify(\_SB.PC00.RP05, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP06.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP06.HPME()
      Notify(\_SB.PC00.RP06, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP07.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP07.HPME()
      Notify(\_SB.PC00.RP07, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP08.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP08.HPME()
      Notify(\_SB.PC00.RP08, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP09.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP09.HPME()
      Notify(\_SB.PC00.RP09, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP10.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP10.HPME()
      Notify(\_SB.PC00.RP10, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP11.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP11.HPME()
      Notify(\_SB.PC00.RP11, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP12.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP12.HPME()
      Notify(\_SB.PC00.RP12, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP13.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP13.HPME()
      Notify(\_SB.PC00.RP13, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP14.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP14.HPME()
      Notify(\_SB.PC00.RP14, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP15.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP15.HPME()
      Notify(\_SB.PC00.RP15, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP16.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP16.HPME()
      Notify(\_SB.PC00.RP16, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP17.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP17.HPME()
      Notify(\_SB.PC00.RP17, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP18.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP18.HPME()
      Notify(\_SB.PC00.RP18, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP19.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP19.HPME()
      Notify(\_SB.PC00.RP19, 0x02)
    }

    If(LNotEqual(\_SB.PC00.RP20.VDID,0xFFFFFFFF))
    {
      \_SB.PC00.RP20.HPME()
      Notify(\_SB.PC00.RP20, 0x02)
    }
