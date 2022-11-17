/** @file
  This file contains the device definitions of the SystemAgent
  PCIE ACPI Reference Code.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

Scope (\_SB.PC00) {

  OperationRegion(PXCS,PCI_Config,0x00,0x480)
  Field(PXCS,AnyAcc, NoLock, Preserve)
  {
    Offset(0),
    VDID, 32,
    Offset(0x50), // LCTL - Link Control Register
    L0SE, 1,      // 0, L0s Entry Enabled
    , 3,
    LDIS, 1,
    , 3,
    Offset(0x52), // LSTS - Link Status Register
    , 13,
    LASX, 1,      // 0, Link Active Status
    Offset(0x5A), // SLSTS[7:0] - Slot Status Register
    ABPX, 1,      // 0, Attention Button Pressed
    , 2,
    PDCX, 1,      // 3, Presence Detect Changed
    , 2,
    PDSX, 1,      // 6, Presence Detect State
    , 1,
    Offset(0x60), // RSTS - Root Status Register
    , 16,
    PSPX, 1,      // 16,  PME Status
    Offset(0xA4),
    D3HT, 2,      // Power State
    Offset(0xD8), // 0xD8, MPC - Miscellaneous Port Configuration Register
    , 30,
    HPEX, 1,      // 30,  Hot Plug SCI Enable
    PMEX, 1,      // 31,  Power Management SCI Enable
    Offset(0xE0), // 0xE0, SPR - Scratch Pad Register
    , 0,
    SCB0, 1,      // Sticky Scratch Pad Bit SCB0
    Offset(0xE2), // 0xE2, RPPGEN - Root Port Power Gating Enable
    , 2,
    L23E, 1,       // 2,   L23_Rdy Entry Request (L23ER)
    L23R, 1,       // 3,   L23_Rdy to Detect Transition (L23R2DT)
    Offset(0x324), // 0x324 - PCIEDBG
    , 3,
    LEDM, 1,       // PCIEDBG.DMIL1EDM
    Offset(0x328), // 0x328 - PCIESTS1
    , 24,
    LTSM, 8,
  }
  Field(PXCS,AnyAcc, NoLock, WriteAsZeros)
  {
    Offset(0xDC), // 0xDC, SMSCS - SMI/SCI Status Register
    , 30,
    HPSX, 1,      // 30,  Hot Plug SCI Status
    PMSX, 1       // 31,  Power Management SCI Status
  }

    //
    // Name: RTEN
    // Description: Function to Enable the link for RTD3 [RCTL.L22DT]
    // Input: PEG Index
    // Return: Nothing
    //
    Method(RTEN, 0, Serialized)
    {
      If (LNotEqual (SCB0,0x1)) {
        Return ()
      }

      /// Set L23_Rdy to Detect Transition  (L23R2DT)
      Store(1, L23R)
      Store(0, Local0)
      /// Wait for transition to Detect
      While(L23R) {
        If(Lgreater(Local0, 4))
        {
          Break
        }
        Sleep(16)
        Increment(Local0)
      }
      Store(0,SCB0)

      /// Once in Detect, wait up to 124 ms for Link Active (typically happens in under 70ms)
      /// Worst case per PCIe spec from Detect to Link Active is:
      /// 24ms in Detect (12+12), 72ms in Polling (24+48), 28ms in Config (24+2+2+2+2)
      Store(0, Local0)
      While(LEqual(LASX,0)) {
        If(Lgreater(Local0, 8))
        {
          Break
        }
        Sleep(16)
        Increment(Local0)
      }
    }

    //
    // Name: RTDS
    // Description: Function to Disable link for RTD3 [RCTL.L23ER]
    // Input: PEG Index
    // Return: Nothing
    //
    Method(RTDS, 0, Serialized)
    {
      Store(1, L23E)
      Sleep(16)
      Store(0, Local0)
      While(L23E) {
        If(Lgreater(Local0, 4))
        {
          Break
        }
        Sleep(16)
        Increment(Local0)
      }
      Store(1,SCB0)
    }

} // End of Scope (\_SB.PC00)
