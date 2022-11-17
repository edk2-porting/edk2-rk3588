/** @file
  ACPI RTD3 SSDT table for SPT PCIe

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define PID_ICC                                   0xDC
#define R_PCH_PCR_ICC_MSKCKRQ                     0x100C                  ///< Mask Control CLKREQ

External(PCRA,MethodObj)
External(PCRO,MethodObj)
External(\MMRP, MethodObj)
External(\MMTB, MethodObj)
External(\TRDO, IntObj)
External(\TRD3, IntObj)
External(\TBPE, IntObj)
External(\TOFF, IntObj)
External(\TBSE, IntObj)
External(\TBOD, IntObj)
External(\TBRP, IntObj)
External(\TBHR, IntObj)
External(\RTBC, IntObj)
External(\TBCD, IntObj)

Name(G2SD, 0) // Go2Sx done, set by GO2S, cleaned by _ON

Name(WKEN, 0)

  Method(_S0W, 0)
  {
  /// This method returns the lowest D-state supported by PCIe root port during S0 state

   ///- PMEs can be generated from D3hot for ULT
      Return(4)

  /** @defgroup pcie_s0W PCIE _S0W **/
  } // End _S0W

  Method (_DSD, 0) {
    Return (
      Package () {
        ToUUID("6211E2C0-58A3-4AF3-90E1-927A4E0C55A4"),
        Package () {
          Package (2) {"HotPlugSupportInD3", 1},
        }
      }
    ) // End of Return ()
  }

    Method(_DSW, 3)
    {
    /// This method is used to enable/disable wake from PCIe (WKEN)
      If (LGreaterEqual(Arg1, 1)) { /// If entering Sx, need to disable WAKE# from generating runtime PME
                                    /// Also set 2 to TOFF.
        Store(0, WKEN)
        Store (2, TOFF)
      } Else {  /// If Staying in S0
        If(LAnd(Arg0, Arg2)) ///- Check if Exiting D0 and arming for wake
        { ///- Set PME
          Store(1, WKEN)
          Store (1, TOFF)
        } Else { ///- Disable runtime PME, either because staying in D0 or disabling wake
          Store(0, WKEN)
          Store(0, TOFF)
        }
      }

    /** @defgroup pcie_dsw PCIE _DSW **/
    } // End _DSW


    PowerResource(PXP, 0, 0)
    {
    /// Define the PowerResource for PCIe slot
    /// Method: _STA(), _ON(), _OFF()
    /** @defgroup pcie_pxp PCIE Power Resource **/

      Method(_STA, 0)
      {
          Return(PSTA())
      }  /** @defgroup pcie_sta PCIE _STA method **/

      Method(_ON) /// Turn on core power to PCIe Slot
      {
        Store(1, TRDO)
        PON()
        Store(0, TRDO)
      } /** @defgroup pcie_on PCIE _ON method **/

      Method(_OFF) /// Turn off core power to PCIe Slot
      {
        Store(1, TRD3)
        POFF()
        Store(0, TRD3)
      } // End of Method_OFF
    } // End PXP

    Method(PSTA, 0)
    {
    /// Returns the status of PCIe slot core power
      // detect power pin status
      if(LNotEqual(DeRefOf(Index(PWRG, 0)),0)) {
        if(LEqual(DeRefOf(Index(PWRG, 0)),1)) { // GPIO mode
          if(LEqual(\_SB.GGOV(DeRefOf(Index(PWRG, 2))),DeRefOf(Index(PWRG, 3)))){
            Return (1)
          } Else {
            Return (0)
          }
        } // GPIO mode
        if(LEqual(DeRefOf(Index(PWRG, 0)),2))  { // IOEX mode
          if(LEqual(\_SB.PCI0.GEXP.GEPS(DeRefOf(Index(PWRG, 1)),DeRefOf(Index(PWRG, 2))),DeRefOf(Index(PWRG, 3)))){
            Return (1)
          } Else {
            Return (0)
          }
        } // IOEX mode
      }
      // detect reset pin status
      if(LNotEqual(DeRefOf(Index(RSTG, 0)),0)) {
        if(LEqual(DeRefOf(Index(RSTG, 0)),1)) { // GPIO mode
          if(LEqual(\_SB.GGOV(DeRefOf(Index(RSTG, 2))),DeRefOf(Index(RSTG, 3)))){
            Return (1)
          } Else {
            Return (0)
          }
        } // GPIO mode
        if(LEqual(DeRefOf(Index(RSTG, 0)),2))  { // IOEX mode
          if(LEqual(\_SB.PCI0.GEXP.GEPS(DeRefOf(Index(RSTG, 1)),DeRefOf(Index(RSTG, 2))),DeRefOf(Index(RSTG, 3)))){
            Return (1)
          } Else {
            Return (0)
          }
        } // IOEX mode
      }
      Return (0)
    }  /** @defgroup pcie_sta PCIE _STA method **/

    Method (SXEX, 0, Serialized) {

      Store(\MMTB(TBSE), Local7)
      OperationRegion(TBDI, SystemMemory, Local7, 0x550)// TBT HR PCICFG MMIO
      Field(TBDI,DWordAcc, NoLock, Preserve) {
        DIVI, 32,
        CMDR, 32,
        Offset(0x548),
        TB2P, 32,
        P2TB, 32
      }

      Store(100, Local1)
      Store(0x09, P2TB) // Write SX_EXIT_TBT_CONNECTED to PCIe2TBT
      While (LGreater(Local1, 0)) {

        Store(Subtract(Local1, 1), Local1)
        Store(TB2P, Local2)
        If (LEqual(Local2, 0xFFFFFFFF)) { // Device gone
          Return()
        }
        If (And(Local2, 1)) { // Done
          break
        }
        Sleep(5)
      }
      Store(0x0, P2TB) // Write 0 to PCIe2TBT

      // Fast Link bring-up flow
      Store(500, Local1)
      While (LGreater(Local1, 0)) {
        Store(Subtract(Local1, 1), Local1)
        Store(TB2P, Local2)
        If (LEqual(Local2, 0xFFFFFFFF)) {// Device gone
          Return()
        }
        If (LNotEqual(DIVI, 0xFFFFFFFF)) {
          break
        }
        Sleep(10)
      }
    } // End of Method(SXEX, 0, Serialized)

    Method(PON) /// Turn on core power to PCIe Slot
    {

      Store(\MMRP(\TBSE), Local7)
      OperationRegion(L23P,SystemMemory,Local7,0xE4)
      Field(L23P,WordAcc, NoLock, Preserve)
      {
        Offset(0xA4),// PMCSR
        PSD0, 2, // PowerState
        Offset(0xE2),// 0xE2, RPPGEN - Root Port Power Gating Enable
        , 2,
        L2TE, 1,      // 2,   L23_Rdy Entry Request (L23ER)
        L2TR, 1,       // 3,   L23_Rdy to Detect Transition (L23R2DT)
      }

      Store(\MMTB(\TBSE), Local6)
      OperationRegion(TBDI, SystemMemory, Local6, 0x550)// TBT HR PCICFG MMIO
      Field(TBDI,DWordAcc, NoLock, Preserve) {
        DIVI, 32,
        CMDR, 32,
        Offset(0xA4),
        TBPS, 2, // PowerState of TBT
        Offset(0x548),
        TB2P, 32,
        P2TB, 32
      }
 
      Store(0, TOFF)
      // Check RTD3 power enable, if already ON, no need to execute sx_exit
      If (TBPE) {
        Return()
      }

      Store(0,G2SD)
      If (\RTBC) {
        /// de-assert CLK_REQ MSK
        if(LNotEqual(DeRefOf(Index(SCLK, 0)),0)) { // if power gating enabled
          PCRA(PID_ICC,R_PCH_PCR_ICC_MSKCKRQ,Not(DeRefOf(Index(SCLK, 1))))  // And ~SCLK to clear bit
        }
        Sleep(\TBCD)
      }

      /// Turn ON Power for PCIe Slot
      if(LNotEqual(DeRefOf(Index(PWRG, 0)),0)) { // if power gating enabled
        if(LEqual(DeRefOf(Index(PWRG, 0)),1)) { // GPIO mode
          \_SB.SGOV(DeRefOf(Index(PWRG, 2)),DeRefOf(Index(PWRG, 3)))
          Store(1, TBPE)
          Sleep(PEP0)     /// Sleep for programmable delay
        }
        if(LEqual(DeRefOf(Index(PWRG, 0)),2))  { // IOEX mode
          \_SB.PCI0.GEXP.SGEP(DeRefOf(Index(PWRG, 1)),DeRefOf(Index(PWRG, 2)),DeRefOf(Index(PWRG, 3)))
          Store(1, TBPE)
          Sleep(PEP0)     /// Sleep for programmable delay
        }
      }

      /// De-Assert Reset Pin
      if(LNotEqual(DeRefOf(Index(RSTG, 0)),0)) { // if reset pin enabled
        if(LEqual(DeRefOf(Index(RSTG, 0)),1)) { // GPIO mode
          \_SB.SGOV(DeRefOf(Index(RSTG, 2)),DeRefOf(Index(RSTG, 3)))
        }
        if(LEqual(DeRefOf(Index(RSTG, 0)),2)) { // IOEX mode
          \_SB.PCI0.GEXP.SGEP(DeRefOf(Index(RSTG, 1)),DeRefOf(Index(RSTG, 2)),DeRefOf(Index(RSTG, 3)))
        }
      }

      /// Clear DLSULPPGE, then set L23_Rdy to Detect Transition  (L23R2DT)
      Store(0, DPGE)
      Store(1, L2TR)
      Sleep(16)
      Store(0, Local0)
      /// Wait up to 12 ms for transition to Detect
      While(L2TR) {
        If(Lgreater(Local0, 4))    // Debug - Wait for 5 ms
        {
          Break
        }
        Sleep(16)
        Increment(Local0)
      }
      /// Once in Detect, wait up to 124 ms for Link Active (typically happens in under 70ms)
      /// Worst case per PCIe spec from Detect to Link Active is:
      /// 24ms in Detect (12+12), 72ms in Polling (24+48), 28ms in Config (24+2+2+2+2)
      Store(1, DPGE)
      Store(0, Local0)
      While(LEqual(LASX,0)) {
        If(Lgreater(Local0, 8))
        {
          Break
        }
        Sleep(16)
        Increment(Local0)
      }
      Store(0, LEDM) /// Set PCIEDBG.DMIL1EDM (324[3]) = 0

      // TBT special sleep.
      Store(PSD0, Local1)
      Store(0, PSD0)// D0
      Store(20, Local2) // Poll for TBT, up to 200 ms

      While (LGreater(Local2, 0)) {
        Store(Subtract(Local2, 1), Local2)
        Store(TB2P, Local3)
        If (LNotEqual(Local3, 0xFFFFFFFF)) { // Done
          break
        }
        Sleep(10)
      }

      If (LLessEqual(Local2, 0)) {
      }
      SXEX()
      Store(Local1, PSD0) // Back to Local1
    } /** @defgroup pcie_on PCIE _ON method **/

    Method(POFF) { /// Turn off core power to PCIe Slot
      If (LEqual(TOFF, 0)) {
        Return()
      }
      Store(\MMRP(\TBSE), Local7)
      OperationRegion(L23P, SystemMemory, Local7, 0xE4)
      Field(L23P,WordAcc, NoLock, Preserve)
      {
        Offset(0xA4),// PMCSR
        PSD0, 2, // PowerState
        Offset(0xE2),// 0xE2, RPPGEN - Root Port Power Gating Enable
        , 2,
        L2TE, 1,      // 2,   L23_Rdy Entry Request (L23ER)
        L2TR, 1,       // 3,   L23_Rdy to Detect Transition (L23R2DT)
      }

      Store(\MMTB(TBSE), Local6)
      OperationRegion(TBDI, SystemMemory, Local6, 0x550)// TBT HR PCICFG MMIO
      Field(TBDI,DWordAcc, NoLock, Preserve) {
        DIVI, 32,
        CMDR, 32,
        Offset(0xA4),
        TBPS, 2, // PowerState of TBT
        Offset(0x548),
        TB2P, 32,
        P2TB, 32
      }

      Store(PSD0, Local1)
      Store(0, PSD0)// D0

      Store(P2TB, Local3)

      If (Lgreater(TOFF, 1)) {
        Sleep(10)
        Store(Local1, PSD0) // Back to Local1
        Return()
      }
      Store(0, TOFF)

      Store(Local1, PSD0) // Back to Local1

      /// Set L23_Rdy Entry Request (L23ER)
      Store(1, L2TE)
      Sleep(16)
      Store(0, Local0)
      While(L2TE) {
        If(Lgreater(Local0, 4))    /// Debug - Wait for 5 ms
        {
          Break
        }
        Sleep(16)
        Increment(Local0)
      }
      Store(1, LEDM) /// PCIEDBG.DMIL1EDM (324[3]) = 1

      /// Assert Reset Pin
      if(LNotEqual(DeRefOf(Index(RSTG, 0)),0)) { // if reset pin enabled
        if(LEqual(DeRefOf(Index(RSTG, 0)),1)) { // GPIO mode
          \_SB.SGOV(DeRefOf(Index(RSTG, 2)),Xor(DeRefOf(Index(RSTG, 3)),1))
        }
        if(LEqual(DeRefOf(Index(RSTG, 0)),2)) { // IOEX mode
          \_SB.PCI0.GEXP.SGEP(DeRefOf(Index(RSTG, 1)),DeRefOf(Index(RSTG, 2)),Xor(DeRefOf(Index(RSTG, 3)),1))
        }
      }
      If (\RTBC) {
        /// assert CLK_REQ MSK
        if(LNotEqual(DeRefOf(Index(SCLK, 0)),0)) { // if power gating enabled
          PCRO(PID_ICC,R_PCH_PCR_ICC_MSKCKRQ,DeRefOf(Index(SCLK, 1)))    // Or SCLK to set bit
          Sleep(16)
        }
      }

      /// Power OFF for TBT
      if(LNotEqual(DeRefOf(Index(PWRG, 0)),0)) { // if power gating enabled
        if(LEqual(DeRefOf(Index(PWRG, 0)),1)) { // GPIO mode
          \_SB.SGOV(DeRefOf(Index(PWRG, 2)),Xor(DeRefOf(Index(PWRG, 3)),1))
        }
        if(LEqual(DeRefOf(Index(PWRG, 0)),2))  { // IOEX mode
          \_SB.PCI0.GEXP.SGEP(DeRefOf(Index(PWRG, 1)),DeRefOf(Index(PWRG, 2)),Xor(DeRefOf(Index(PWRG, 3)),1))
        }
      }

      Store(0, TBPE)

      Store(1, LDIS) /// Set Link Disable
      Store(0, LDIS) /// Toggle link disable

      /// enable WAKE
      If (WKEN) {
        If (LNotEqual(DeRefOf(Index(WAKG, 0)),0)) { // if power gating enabled
          If (LEqual(DeRefOf(Index(WAKG, 0)),1)) { // GPIO mode
            \_SB.SGOV(DeRefOf(Index(WAKG, 2)),DeRefOf(Index(WAKG, 3)))
            \_SB.SHPO(DeRefOf(Index(WAKG, 2)), 0) // set gpio ownership to ACPI(0=ACPI mode, 1=GPIO mode)
          }
          If (LEqual(DeRefOf(Index(WAKG, 0)),2))  { // IOEX mode
            \_SB.PCI0.GEXP.SGEP(DeRefOf(Index(WAKG, 1)),DeRefOf(Index(WAKG, 2)),DeRefOf(Index(WAKG, 3)))
          }
        }
      }
      Sleep(\TBOD)
      /** @defgroup pcie_off PCIE _OFF method **/
    } // End of Method_OFF

    Name(_PR0, Package(){PXP})
    Name(_PR3, Package(){PXP})

