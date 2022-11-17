/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


External(\_SB.PCI0.XHC.PS0X, MethodObj)
External(\_SB.PCI0.XHC.PS3X, MethodObj)
External(\_SB.PCI0.XHC.RHUB.PS0X, MethodObj)
External(\_SB.PCI0.XHC.RHUB.PS2X, MethodObj)
External(\_SB.PCI0.XHC.RHUB.PS3X, MethodObj)
External(\_SB.PCI0.XHC.RHUB.INIR, MethodObj)

Scope(\_SB_.PCI0) {
  Device(XHC) {

    Name(_ADR, 0x00140000)

    OperationRegion(XPRT,PCI_Config,0x00,0x100)
    Field(XPRT,AnyAcc,NoLock,Preserve)
    {
      DVID, 16,
      Offset(0x10),
      XADL,  32,
      XADH,  32,
      Offset(0x50), // XHCLKGTEN - Clock Gating
          ,  2,
      STGE,  1,     // Clock Gating Bit SSLTCGE
    }
    //
    // Byte access for PMCS field to avoid race condition on device D-state
    //
    Field(XPRT,ByteAcc,NoLock,Preserve)
    {
      Offset(R_PCH_XHCI_PWR_CNTL_STS), // 0x74
      D0D3,  2,  // 0x74 BIT[1:0]
          ,  6,
      PMEE,  1,  // PME Enable
          ,  6,
      PMES,  1,  // PME Status
    }

    //
    // Variable to store the maximum D state supported in S0.
    //
    Name (XFLT, 0)
    //
    // XHCI controller won't go into D3Hot during S0 until _DSM method is evaluated by filter driver.
    //
    Method(_DSM,4,serialized){
      ADBG("_DSM")
      ShiftLeft(XADH, 32, Local0)
      Or (Local0, XADL, Local0)
      And(Local0,0xFFFFFFFFFFFFFFF0,Local0)
      OperationRegion(XMIO,SystemMemory,Local0,0x9000)
      Field(XMIO,AnyAcc,Lock,Preserve)
      {
        Offset(R_PCH_LP_XHCI_PORTSC02USB3),  // PORTSCXUSB3 - Port X Status and Control USB3
        PCCS,  1,       // Current Connect Status
            ,  4,
        PPLS,  4,       // Port Link Status
        PTPP,  1,       // Port Power
      }

      If(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }
      //
      // Check GUID ac340cb7-e901-45bf-b7e6-2b34ec931e23
      //
      If(LEqual(Arg0, Buffer(0x10) { 0xb7, 0x0c, 0x34, 0xac, 0x01, 0xe9, 0xbf, 0x45, 0xb7, 0xe6, 0x2b, 0x34, 0xec, 0x93, 0x1e, 0x23 }))
      {
        //
        // Only Supported value is "0x3"
        //
        If(LEqual(Arg1, 0x3))
        {
          Store(Arg1,XFLT)
        }
      }
      Return(Buffer() {0})
    }

    Method(_S3D, 0, NotSerialized)
    {
      Return(3)
    }
    Method(_S4D, 0, NotSerialized)
    {
      Return(3)
    }
    Method(_S3W, 0, NotSerialized)
    {
      Return(3)
    }
    Method(_S4W, 0, NotSerialized)
    {
      Return(3)
    }

    //
    // Once the filter driver is installed, D3 is allowed.
    //
    Method(_S0W, 0x0, NotSerialized)
    {
      If(LEqual(XFLT, Zero))
      {
        Return(0x0)
      }
      Else
      {
        Return(0x3)
      }
    }

    Method(_PRW, 0)
    {
      Return(GPRW(0x6D, 4)) // can wakeup from S4 state
    }

    Method(_DSW, 3)
    {
      Store(Arg0, PMEE)
    }

    Method(_INI) {
      If(CondRefOf(\_SB.PCI0.XHC.RHUB.INIR)) {      // _INI for RTD3
        \_SB.PCI0.XHC.RHUB.INIR()
      }
    }

    //
    // GPE handler for XHCI, this is part of _Lxx handler for bus 0 PME
    //
    Method(GPEH)
    {
      If(LEqual(DVID, 0xFFFF)) {
        Return()
      }
      Store(PMES, Local0)
      Store(1, PMES) // clear PME Status
      If(LAnd(PMEE, Local0)) {
        Notify(XHC, 0x02)
      }
    }

    OperationRegion(XHCP, SystemMemory, Add(\_SB.PCI0.GPCB(), 0xA0000), 0x100)
    Field(XHCP,AnyAcc,Lock,Preserve)
    {
      Offset(0x4),
      PDBM, 16,
      Offset(0x10),
      MEMB, 64
    }

    //
    // USRA (USbR port Address), method for calculating address of first USBR port in XHCI controller
    //
    Method(USRA,0,Serialized) {
      If(LEqual(PCHV, SPTH)) { // SPT-H
        Return (15)
      } Else {                 // SPT-LP
        Return (11)
      }
    }

    //
    // SSPA (SuperSpeed Port Address), method for calculating address of first SS port in XHCI controller
    //
    Method(SSPA,0,Serialized) {
      If(LEqual(PCHV, SPTH)) { // SPT-H
        Return (17)
      } Else {                 // SPT-LP
        Return (13)
      }
    }

    Name(XRST, Zero)

    Method(_PS0,0,Serialized)
    {

      If(LEqual(^DVID,0xFFFF))
      {
        Return()
      }

      Store(^MEMB,Local2)         // Save MBAR
      Store(^PDBM,Local1)         // Save CMD

      And(^PDBM,Not(0x06),^PDBM)  // Clear MSE/BME
      Store(0,STGE)

      //
      // Switch to D0
      //
      Store(0,^D0D3)

      Store(\XWMB,^MEMB)          // Set MBAR
      Or(Local1,0x0002,^PDBM)     // Set MSE

      OperationRegion(MC11,SystemMemory,\XWMB,0x9000)
      Field(MC11,DWordAcc,Lock,Preserve)
      {
        Offset(R_PCH_XHCI_USB2PMCTRL),   // 0x81C4, USB2PMCTRL - USB2 Power Management Control
            ,  2,
        UPSW,  2,         // U2PSUSPGP
      }

      Store(0x0,UPSW)

      And(^PDBM,Not(0x02),^PDBM)  // Clear MSE

      Store(Local2,^MEMB)         // Restore MBAR
      Store(Local1,^PDBM)         // Restore CMD

      //
      // Call platform XHC PS0 method if present
      //
      If(CondRefOf(\_SB.PCI0.XHC.PS0X))
      {
        \_SB.PCI0.XHC.PS0X()
      }

      //
      //
      If(LAnd(UWAB,LOr(LEqual(D0D3,0),LEqual(\_SB.PCI0.XDCI.D0I3,0)))) {
        //
        // If the USB WA Bit is set and any XHCI or XDCI controller is in D0
        //
        //
        // USB2 PHPY Power Gating - SW WA
        //
        // 1. BIOS writes a new command to the MTPMC register to cause the PMC to disable power gating
        Store(1,MPMC)

        // 2. BIOS waits for PCH_PM_STS.MSG_FULL_STS to be 0
        //      In parallel and in response to the previous command from BIOS, PMC FW will:
        //         * Make the USB2 force common lane PG match the BIOS policy (common lane PG disallowed)
        //         * Disable SUS power gating in XHCI
        //         * Wait for USB2 PHY side_pok to be 1
        //         * Disable core power gating in USB2 PHY
        //         * Disable common lane power gating in USB2 PHY (probably not necessary, consider removal)
        //         * Wait for USB2 SUS restoration status to be set, and do USB2 SUS power gating restoration
        //         * Get the PHY in the correct state before allowing transition to D0.
        //        * Clear MSG_FULL_STS
        //    BIOS sees MSG_FULL_STS clear and exits the method
        //
        While(PMFS) {
          Sleep(10)
        }
      } // End If(UWAB)

    }

    Method(_PS3,0,Serialized)
    {

      If(LEqual(^DVID,0xFFFF))
      {
        Return()
      }

      Store(^MEMB,Local2)         // Save MBAR
      Store(^PDBM,Local1)         // Save CMD

      And(^PDBM,Not(0x06),^PDBM)  // Clear MSE/BME

      //
      // Switch back to D0
      //
      Store(0,^D0D3)

      Store(\XWMB,^MEMB)          // Set MBAR
      Or(Local1,0x0002,^PDBM)     // Set MSE

      OperationRegion(MC11,SystemMemory,\XWMB,0x9000)
      Field(MC11,DWordAcc,Lock,Preserve)
      {
        Offset(R_PCH_XHCI_USB2PMCTRL),   // 0x81C4, USB2PMCTRL - USB2 Power Management Control
            ,  2,
        UPSW,  2,         // U2PSUSPGP
      }

      Store(0x3,UPSW)

      And(^PDBM,Not(0x02),^PDBM)  // Clear MSE

      Store(1,STGE)

      //
      // Switch back to D3
      //
      Store(3,^D0D3)

      Store(Local2,^MEMB)         // Restore MBAR
      Store(Local1,^PDBM)         // Restore CMD

      //
      // Call platform XHC PS3 method if present
      //
      If(CondRefOf(\_SB.PCI0.XHC.PS3X))
      {
        \_SB.PCI0.XHC.PS3X()
      }

      If(LAnd(UWAB,LAnd(LEqual(D0D3,3),LOr(LEqual(\_SB.PCI0.XDCI.D0I3,3),LEqual(\_SB.PCI0.XDCI.DVID,0xFFFF))))) {
        //
        // If the USB WA Bit is set and ((XHCI is in D3) AND ((XDCI is in D3) OR (XDCI is function disabled)))
        //
        // 1. BIOS writes a new command to the MTPMC register to enable power gating
        Store(3,MPMC)

        // 2. BIOS waits for PCH_PM_STS.MSG_FULL_STS to be 0
        //      In parallel and in response to the previous command from BIOS, PMC FW will:
        //        * Make the USB2 force common lane PG match the BIOS policy (common lane PG allowed)
        //        * Enable SUS power gating in XHCI
        //        * Enable core power gating in USB2 PHY
        //        * Enable common lane power gating in the USB2 PHY
        //        * Clear MSG_FULL_STS
        //    BIOS sees MSG_FULL_STS clear and exits the method
        //
        While(PMFS) {
          Sleep(10)
        }
      } // End If(UWAB)
    }

    //
    //
    // Check for XHCI switch UUID
    //
    // Arguments:
    //  Arg0 (Buffer) : UUID
    //
    // Returns:
    //  1: It's valid UUID
    //  0: Invalid UUID
    //
    Method(CUID,1,Serialized) {
      If(LEqual(Arg0,ToUUID("7c9512a9-1705-4cb4-af7d-506a2423ab71"))) {
        Return(1)
      }
      Return(0)
    }

    Device(RHUB)
    {
      Name(_ADR, Zero)

      // PS0 Method for xHCI Root Hub
      Method(_PS0,0,Serialized)
      {
        If(LEqual(\_SB.PCI0.XHC.DVID,0xFFFF))
        {
          Return()
        }
        //
        // Call platform XHC.RHUB PS0 method if present.
        //
        If(CondRefOf(\_SB.PCI0.XHC.RHUB.PS0X))
        {
          \_SB.PCI0.XHC.RHUB.PS0X()
        }
      }

      // PS2 Method for xHCI Root Hub
      Method(_PS2,0,Serialized)
      {
        If(LEqual(\_SB.PCI0.XHC.DVID,0xFFFF))
        {
          Return()
        }
        //
        // Call platform XHC.RHUB PS2 method if present.
        //
        If(CondRefOf(\_SB.PCI0.XHC.RHUB.PS2X))
        {
          \_SB.PCI0.XHC.RHUB.PS2X()
        }
      }

      // PS3 Method for xHCI Root Hub
      Method(_PS3,0,Serialized)
      {
        If(LEqual(\_SB.PCI0.XHC.DVID,0xFFFF))
        {
          Return()
        }
        //
        // Call platform XHC.RHUB PS3 method if present.
        //
        If(CondRefOf(\_SB.PCI0.XHC.RHUB.PS3X))
        {
          \_SB.PCI0.XHC.RHUB.PS3X()
        }
      }

      //
      // High Speed Ports (without USBR)
      //
      Device(HS01)
      {
        Name(_ADR, 0x01)
      }

      Device(HS02)
      {
        Name(_ADR, 0x02)
      }

      Device(HS03)
      {
        Name(_ADR, 0x03)
      }

      Device(HS04)
      {
        Name(_ADR, 0x04)
      }

      Device(HS05)
      {
        Name(_ADR, 0x05)
      }

      Device(HS06)
      {
        Name(_ADR, 0x06)
      }

      Device(HS07)
      {
        Name(_ADR, 0x07)
      }

      Device(HS08)
      {
        Name(_ADR, 0x08)
      }

      Device(HS09)
      {
        Name(_ADR, 0x09)
      }

      Device(HS10)
      {
        Name(_ADR, 0x0A)
      }

      //
      // USBR port will be known as USBR instead of HS09 / HS15
      //
      Device(USR1)
      {
        Method(_ADR) { Return (Add(USRA(),0)) }
      }

      //
      // USBR port 2 will be known as USBR instead of xxxx
      //
      Device(USR2)
      {
        Method(_ADR) { Return (Add(USRA(),1)) }
      }

      //
      // Super Speed Ports
      //
      Device(SS01)
      {
        Method(_ADR) { Return (Add(SSPA(),0)) }
      }

      Device(SS02)
      {
        Method(_ADR) { Return (Add(SSPA(),1)) }
      }

      Device(SS03)
      {
        Method(_ADR) { Return (Add(SSPA(),2)) }
      }

      Device(SS04)
      {
        Method(_ADR) { Return (Add(SSPA(),3)) }
      }

      Device(SS05)
      {
        Method(_ADR) { Return (Add(SSPA(),4)) }
      }

      Device(SS06)
      {
        Method(_ADR) { Return (Add(SSPA(),5)) }
      }

    } // device rhub
  } // device xhc
} //scope

//
// SPT-H
//
If(LEqual(PCHV(),SPTH)) {
  Scope(\_SB_.PCI0.XHC.RHUB) {

    Device(HS11)
    {
      Name(_ADR, 0xB)
    }

    Device(HS12)
    {
      Name(_ADR, 0xC)
    }

    Device(HS13)
    {
      Name(_ADR, 0xD)
    }

    Device(HS14)
    {
      Name(_ADR, 0xE)
    }

    Device(SS07)
    {
      Method(_ADR) { Return (Add(SSPA(),6)) }
    }

    Device(SS08)
    {
      Method(_ADR) { Return (Add(SSPA(),7)) }
    }

    Device(SS09)
    {
      Method(_ADR) { Return (Add(SSPA(),8)) }
    }

    Device(SS10)
    {
      Method(_ADR) { Return (Add(SSPA(),9)) }
    }

  } //scope
} // if
