/** @file
  ACPI DSDT table

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  // General Purpose Events.  This Scope handles the Run-time and
  // Wake-time SCIs.  The specific method called will be determined by
  // the _Lxx value, where xx equals the bit location in the General
  // Purpose Event register(s).


  External(D1F0)
  External(D1F1)
  External(D1F2)
  External(\_SB.PCI0.PEG0.HPME, MethodObj)
  External(\_SB.PCI0.PEG1.HPME, MethodObj)
  External(\_SB.PCI0.PEG2.HPME, MethodObj)
  External(\_GPE.AL6F, MethodObj)
  External(\_SB.THDR, MethodObj)
  External(\_GPE.P0L6, MethodObj)
  External(\_GPE.P1L6, MethodObj)
  External(\_GPE.P2L6, MethodObj)
  External(SGGP)
  External(P1GP)
  External(P2GP)
  External(P0WK)
  External(P1WK)
  External(P2WK)
  External(\CPG0)
  External(\RPS0)
  External(\RPT0)
  External(\_PR.HWPI, IntObj)
  External(\_PR.DTSI, IntObj)

  Scope(\_GPE)
  {
  // Note:
  // Originally, the two GPE methods below are automatically generated, but, for ASL code restructuring,
  // disabled the automatic generation and declare the ASL code here.
  //

  //
  // This PME event (PCH's GPE 69h) is received on one or more of the PCI Express* ports or
  // an assert PMEGPE message received via DMI
  //
  Method(_L69, 0, serialized) {
    \_SB.PCI0.RP01.HPME()
    \_SB.PCI0.RP02.HPME()
    \_SB.PCI0.RP03.HPME()
    \_SB.PCI0.RP04.HPME()
    \_SB.PCI0.RP05.HPME()
    \_SB.PCI0.RP06.HPME()
    \_SB.PCI0.RP07.HPME()
    \_SB.PCI0.RP08.HPME()
    \_SB.PCI0.RP09.HPME()
    \_SB.PCI0.RP10.HPME()
    \_SB.PCI0.RP11.HPME()
    \_SB.PCI0.RP12.HPME()
    \_SB.PCI0.RP13.HPME()
    \_SB.PCI0.RP14.HPME()
    \_SB.PCI0.RP15.HPME()
    \_SB.PCI0.RP16.HPME()
    \_SB.PCI0.RP17.HPME()
    \_SB.PCI0.RP18.HPME()
    \_SB.PCI0.RP19.HPME()
    \_SB.PCI0.RP20.HPME()
    \_SB.PCI0.RP21.HPME()
    \_SB.PCI0.RP22.HPME()
    \_SB.PCI0.RP23.HPME()
    \_SB.PCI0.RP24.HPME()

    If(LEqual(D1F0,1))
    {
      \_SB.PCI0.PEG0.HPME()
      Notify(\_SB.PCI0.PEG0, 0x02)
      Notify(\_SB.PCI0.PEG0.PEGP, 0x02)
    }

    If(LEqual(D1F1,1))
    {
      \_SB.PCI0.PEG1.HPME()
      Notify(\_SB.PCI0.PEG1, 0x02)
    }

    If(LEqual(D1F2,1))
    {
      \_SB.PCI0.PEG2.HPME()
      Notify(\_SB.PCI0.PEG2, 0x02)
    }
  }

  // PCI Express Hot-Plug caused the wake event.

  Method(_L61)
  {
    Add(L01C,1,L01C)  // Increment L01 Entry Count.

    P8XH(0,0x01)      // Output information to Port 80h.
    P8XH(1,L01C)


    // Check Root Port 1 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP01.VDID,0xFFFFFFFF),\_SB.PCI0.RP01.HPSX))
    {
      // Delay for 100ms to meet the timing requirements
      // of the PCI Express Base Specification, Revision
      // 1.0A, Section 6.6 ("...software must wait at
      // least 100ms from the end of reset of one or more
      // device before it is permitted to issue
      // Configuration Requests to those devices").

      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x1),LNotEqual(TBS1,0x1)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP01.PDCX)
      {
        // Clear all status bits first.

        Store(1,\_SB.PCI0.RP01.PDCX)
        Store(1,\_SB.PCI0.RP01.HPSX)

        //
        // PCH BIOS Spec Update Rev 1.03, Section 8.9 PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If(LNot(\_SB.PCI0.RP01.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store(0,\_SB.PCI0.RP01.L0SE)

        }

        // Perform proper notification
        // to the OS.
        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x1),LNotEqual(TBS1,0x1)))) {
          Notify(\_SB.PCI0.RP01,0)
        }
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store(1,\_SB.PCI0.RP01.HPSX)
      }
    }

    // Check Root Port 2 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP02.VDID,0xFFFFFFFF),\_SB.PCI0.RP02.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x2),LNotEqual(TBS1,0x2)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP02.PDCX)
      {
        Store(1,\_SB.PCI0.RP02.PDCX)
        Store(1,\_SB.PCI0.RP02.HPSX)

        If(LNot(\_SB.PCI0.RP02.PDSX)) {
          Store(0,\_SB.PCI0.RP02.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x2),LNotEqual(TBS1,0x2)))) {
          Notify(\_SB.PCI0.RP02,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP02.HPSX)
      }
    }

    // Check Root Port 3 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP03.VDID,0xFFFFFFFF),\_SB.PCI0.RP03.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x3),LNotEqual(TBS1,0x3)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP03.PDCX)
      {
        Store(1,\_SB.PCI0.RP03.PDCX)
        Store(1,\_SB.PCI0.RP03.HPSX)

        If(LNot(\_SB.PCI0.RP03.PDSX)) {
          Store(0,\_SB.PCI0.RP03.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x3),LNotEqual(TBS1,0x3)))) {
          Notify(\_SB.PCI0.RP03,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP03.HPSX)
      }
    }

    // Check Root Port 4 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP04.VDID,0xFFFFFFFF),\_SB.PCI0.RP04.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x4),LNotEqual(TBS1,0x4)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP04.PDCX)
      {
        Store(1,\_SB.PCI0.RP04.PDCX)
        Store(1,\_SB.PCI0.RP04.HPSX)

        If(LNot(\_SB.PCI0.RP04.PDSX)) {
          Store(0,\_SB.PCI0.RP04.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x4),LNotEqual(TBS1,0x4)))) {
          Notify(\_SB.PCI0.RP04,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP04.HPSX)
      }
    }

    // Check Root Port 5 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP05.VDID,0xFFFFFFFF),\_SB.PCI0.RP05.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x5),LNotEqual(TBS1,0x5)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP05.PDCX)
      {
        Store(1,\_SB.PCI0.RP05.PDCX)
        Store(1,\_SB.PCI0.RP05.HPSX)

        If(LNot(\_SB.PCI0.RP05.PDSX)) {
          Store(0,\_SB.PCI0.RP05.L0SE)
        }

      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x5),LNotEqual(TBS1,0x5)))) {
          Notify(\_SB.PCI0.RP05,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP05.HPSX)
      }
    }

    // Check Root Port 6 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP06.VDID,0xFFFFFFFF),\_SB.PCI0.RP06.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x6),LNotEqual(TBS1,0x6)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP06.PDCX)
      {
        Store(1,\_SB.PCI0.RP06.PDCX)
        Store(1,\_SB.PCI0.RP06.HPSX)

        If(LNot(\_SB.PCI0.RP06.PDSX)) {
          Store(0,\_SB.PCI0.RP06.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x6),LNotEqual(TBS1,0x6)))) {
          Notify(\_SB.PCI0.RP06,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP06.HPSX)
      }
    }

    // Check Root Port 7 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP07.VDID,0xFFFFFFFF),\_SB.PCI0.RP07.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x7),LNotEqual(TBS1,0x7)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP07.PDCX)
      {
        Store(1,\_SB.PCI0.RP07.PDCX)
        Store(1,\_SB.PCI0.RP07.HPSX)

        If(LNot(\_SB.PCI0.RP07.PDSX)) {
          Store(0,\_SB.PCI0.RP07.L0SE)
        }

      }
      Else
      {
        Store(1,\_SB.PCI0.RP07.HPSX)
      }
    }

    // Check Root Port 8 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP08.VDID,0xFFFFFFFF),\_SB.PCI0.RP08.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x8),LNotEqual(TBS1,0x8)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP08.PDCX)
      {
        Store(1,\_SB.PCI0.RP08.PDCX)
        Store(1,\_SB.PCI0.RP08.HPSX)

        If(LNot(\_SB.PCI0.RP08.PDSX)) {
          Store(0,\_SB.PCI0.RP08.L0SE)
        }

      }
      Else
      {
        Store(1,\_SB.PCI0.RP08.HPSX)
      }
    }

    // Check Root Port 9 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP09.VDID,0xFFFFFFFF),\_SB.PCI0.RP09.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x9),LNotEqual(TBS1,0x9)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP09.PDCX)
      {
        Store(1,\_SB.PCI0.RP09.PDCX)
        Store(1,\_SB.PCI0.RP09.HPSX)

        If(LNot(\_SB.PCI0.RP09.PDSX)) {
          Store(0,\_SB.PCI0.RP09.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x9),LNotEqual(TBS1,0x9)))) {
          Notify(\_SB.PCI0.RP09,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP09.HPSX)
      }
    }

    // Check Root Port 10 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP10.VDID,0xFFFFFFFF),\_SB.PCI0.RP10.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xA),LNotEqual(TBS1,0xA)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP10.PDCX)
      {
        Store(1,\_SB.PCI0.RP10.PDCX)
        Store(1,\_SB.PCI0.RP10.HPSX)

        If(LNot(\_SB.PCI0.RP10.PDSX)) {
          Store(0,\_SB.PCI0.RP10.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xA),LNotEqual(TBS1,0xA)))) {
          Notify(\_SB.PCI0.RP10,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP10.HPSX)
      }
    }

    // Check Root Port 11 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP11.VDID,0xFFFFFFFF),\_SB.PCI0.RP11.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xB),LNotEqual(TBS1,0xB)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP11.PDCX)
      {
        Store(1,\_SB.PCI0.RP11.PDCX)
        Store(1,\_SB.PCI0.RP11.HPSX)

        If(LNot(\_SB.PCI0.RP11.PDSX)) {
          Store(0,\_SB.PCI0.RP11.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xB),LNotEqual(TBS1,0xB)))) {
          Notify(\_SB.PCI0.RP11,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP11.HPSX)
      }
    }

    // Check Root Port 12 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP12.VDID,0xFFFFFFFF),\_SB.PCI0.RP12.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xC),LNotEqual(TBS1,0xC)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP12.PDCX)
      {
        Store(1,\_SB.PCI0.RP12.PDCX)
        Store(1,\_SB.PCI0.RP12.HPSX)

        If(LNot(\_SB.PCI0.RP12.PDSX)) {
          Store(0,\_SB.PCI0.RP12.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xC),LNotEqual(TBS1,0xC)))) {
          Notify(\_SB.PCI0.RP12,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP12.HPSX)
      }
    }

    // Check Root Port 13 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP13.VDID,0xFFFFFFFF),\_SB.PCI0.RP13.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xD),LNotEqual(TBS1,0xD)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP13.PDCX)
      {
        Store(1,\_SB.PCI0.RP13.PDCX)
        Store(1,\_SB.PCI0.RP13.HPSX)

        If(LNot(\_SB.PCI0.RP13.PDSX)) {
          Store(0,\_SB.PCI0.RP13.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xD),LNotEqual(TBS1,0xD)))) {
          Notify(\_SB.PCI0.RP13,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP13.HPSX)
      }
    }

    // Check Root Port 14 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP14.VDID,0xFFFFFFFF),\_SB.PCI0.RP14.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xE),LNotEqual(TBS1,0xE)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP14.PDCX)
      {
        Store(1,\_SB.PCI0.RP14.PDCX)
        Store(1,\_SB.PCI0.RP14.HPSX)

        If(LNot(\_SB.PCI0.RP14.PDSX)) {
          Store(0,\_SB.PCI0.RP14.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xE),LNotEqual(TBS1,0xE)))) {
          Notify(\_SB.PCI0.RP14,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP14.HPSX)
      }
    }

    // Check Root Port 15 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP15.VDID,0xFFFFFFFF),\_SB.PCI0.RP15.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xF),LNotEqual(TBS1,0xF)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP15.PDCX)
      {
        Store(1,\_SB.PCI0.RP15.PDCX)
        Store(1,\_SB.PCI0.RP15.HPSX)

        If(LNot(\_SB.PCI0.RP15.PDSX)) {
          Store(0,\_SB.PCI0.RP15.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0xF),LNotEqual(TBS1,0xF)))) {
          Notify(\_SB.PCI0.RP15,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP15.HPSX)
      }
    }

    // Check Root Port 16 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP16.VDID,0xFFFFFFFF),\_SB.PCI0.RP16.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x10),LNotEqual(TBS1,0x10)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP16.PDCX)
      {
        Store(1,\_SB.PCI0.RP16.PDCX)
        Store(1,\_SB.PCI0.RP16.HPSX)

        If(LNot(\_SB.PCI0.RP16.PDSX)) {
          Store(0,\_SB.PCI0.RP16.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x10),LNotEqual(TBS1,0x10)))) {
          Notify(\_SB.PCI0.RP16,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP16.HPSX)
      }
    }

    // Check Root Port 17 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP17.VDID,0xFFFFFFFF),\_SB.PCI0.RP17.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x11),LNotEqual(TBS1,0x11)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP17.PDCX)
      {
        Store(1,\_SB.PCI0.RP17.PDCX)
        Store(1,\_SB.PCI0.RP17.HPSX)

        If(LNot(\_SB.PCI0.RP17.PDSX)) {
          Store(0,\_SB.PCI0.RP17.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x11),LNotEqual(TBS1,0x11)))) {
          Notify(\_SB.PCI0.RP17,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP17.HPSX)
      }
    }

    // Check Root Port 18 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP18.VDID,0xFFFFFFFF),\_SB.PCI0.RP18.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x12),LNotEqual(TBS1,0x12)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP18.PDCX)
      {
        Store(1,\_SB.PCI0.RP18.PDCX)
        Store(1,\_SB.PCI0.RP18.HPSX)

        If(LNot(\_SB.PCI0.RP18.PDSX)) {
          Store(0,\_SB.PCI0.RP18.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x12),LNotEqual(TBS1,0x12)))) {
          Notify(\_SB.PCI0.RP18,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP18.HPSX)
      }
    }

    // Check Root Port 19 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP19.VDID,0xFFFFFFFF),\_SB.PCI0.RP19.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x13),LNotEqual(TBS1,0x13)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP19.PDCX)
      {
        Store(1,\_SB.PCI0.RP19.PDCX)
        Store(1,\_SB.PCI0.RP19.HPSX)

        If(LNot(\_SB.PCI0.RP19.PDSX)) {
          Store(0,\_SB.PCI0.RP19.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x13),LNotEqual(TBS1,0x13)))) {
          Notify(\_SB.PCI0.RP19,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP19.HPSX)
      }
    }

    // Check Root Port 20 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP20.VDID,0xFFFFFFFF),\_SB.PCI0.RP20.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x14),LNotEqual(TBS1,0x14)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP20.PDCX)
      {
        Store(1,\_SB.PCI0.RP20.PDCX)
        Store(1,\_SB.PCI0.RP20.HPSX)

        If(LNot(\_SB.PCI0.RP20.PDSX)) {
          Store(0,\_SB.PCI0.RP20.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x14),LNotEqual(TBS1,0x14)))) {
          Notify(\_SB.PCI0.RP20,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP20.HPSX)
      }
    }

    // Check Root Port 21 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP21.VDID,0xFFFFFFFF),\_SB.PCI0.RP21.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x21),LNotEqual(TBS1,0x21)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP21.PDCX)
      {
        Store(1,\_SB.PCI0.RP21.PDCX)
        Store(1,\_SB.PCI0.RP21.HPSX)

        If(LNot(\_SB.PCI0.RP21.PDSX)) {
          Store(0,\_SB.PCI0.RP21.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x21),LNotEqual(TBS1,0x21)))) {
          Notify(\_SB.PCI0.RP21,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP21.HPSX)
      }
    }

    // Check Root Port 22 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP22.VDID,0xFFFFFFFF),\_SB.PCI0.RP22.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x22),LNotEqual(TBS1,0x22)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP22.PDCX)
      {
        Store(1,\_SB.PCI0.RP22.PDCX)
        Store(1,\_SB.PCI0.RP22.HPSX)

        If(LNot(\_SB.PCI0.RP22.PDSX)) {
          Store(0,\_SB.PCI0.RP22.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x22),LNotEqual(TBS1,0x22)))) {
          Notify(\_SB.PCI0.RP22,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP22.HPSX)
      }
    }

    // Check Root Port 23 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP23.VDID,0xFFFFFFFF),\_SB.PCI0.RP23.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x23),LNotEqual(TBS1,0x23)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP23.PDCX)
      {
        Store(1,\_SB.PCI0.RP23.PDCX)
        Store(1,\_SB.PCI0.RP23.HPSX)

        If(LNot(\_SB.PCI0.RP23.PDSX)) {
          Store(0,\_SB.PCI0.RP23.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x23),LNotEqual(TBS1,0x23)))) {
          Notify(\_SB.PCI0.RP23,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP23.HPSX)
      }
    }

    // Check Root Port 24 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LNotEqual(\_SB.PCI0.RP24.VDID,0xFFFFFFFF),\_SB.PCI0.RP24.HPSX))
    {
      If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x24),LNotEqual(TBS1,0x24)))) {
        Sleep(100)
      }

      If(\_SB.PCI0.RP24.PDCX)
      {
        Store(1,\_SB.PCI0.RP24.PDCX)
        Store(1,\_SB.PCI0.RP24.HPSX)

        If(LNot(\_SB.PCI0.RP24.PDSX)) {
          Store(0,\_SB.PCI0.RP24.L0SE)
        }

        If(LOr(LNotEqual(TBTS, 0x01),LOr(LNotEqual(TBSE,0x24),LNotEqual(TBS1,0x24)))) {
          Notify(\_SB.PCI0.RP24,0)
        }
      }
      Else
      {
        Store(1,\_SB.PCI0.RP24.HPSX)
      }
    }
  }

  //
  // Software GPE caused the event.
  //
  Method(_L62)
  {
    // Clear GPE status bit.
    Store(0,GPEC)

    //
    // Handle DTS Thermal SCI Event.
    //
    If(CondRefOf(\_PR.DTSE)){
      If(LGreaterEqual(\_PR.DTSE, 0x01)){
        If(LEqual(\_PR.DTSI, 1)){
          Notify(\_TZ.TZ00,0x80)
          Notify(\_TZ.TZ01,0x80)
          ///
          /// Clear HWP interrupt status
          ///
          Store(0,\_PR.DTSI)
        }
      }
    }
    ///
    /// Handle HWP SCI event
    ///
    External(\_GPE.HLVT, MethodObj)
    If(LEqual(\_PR.HWPI, 1)){
      If(CondRefOf(\_GPE.HLVT)){
        \_GPE.HLVT()
      }
      ///
      /// Clear HWP interrupt status
      ///
      Store(0,\_PR.HWPI)
    }
  }

  // IGD OpRegion SCI event (see IGD OpRegion/Software SCI BIOS SPEC).

  Method(_L66)
  {
    If(LAnd(\_SB.PCI0.GFX0.GSSE, LNot(GSMI))) // Graphics software SCI event?
    {
      \_SB.PCI0.GFX0.GSCI() // Handle the SWSCI
    }
  }
  //
  // BIOS Needs to implement appropriate handler based on CIO_PLUG_EVENT GPIO
  // This is generic 2-tier GPIO handler
  //
  Method(_L6F)
  {
    \_SB.THDR(\CPG0,\RPS0,\RPT0) // Check for TBT Hotplug Handler event (2-tier GPI GPE event architecture)
  }
}
