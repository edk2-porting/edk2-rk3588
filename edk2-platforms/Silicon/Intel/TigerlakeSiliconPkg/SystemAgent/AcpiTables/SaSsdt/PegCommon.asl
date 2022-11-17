/** @file
  This file contains the device definitions of the SystemAgent
  PCIE ACPI Reference Code.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

External(\_SB.ISME, MethodObj)
External(\_SB.SHPO, MethodObj)
External(\_SB.CAGS, MethodObj)
External(\_SB.GGOV, MethodObj)
External(\_SB.SGOV, MethodObj)
External(\_SB.PC00.PEG0, DeviceObj)
External(\_SB.PC00.PEG1, DeviceObj)
External(\_SB.PC00.PEG2, DeviceObj)
External(\_SB.PC00.PEG3, DeviceObj)
External(\_SB.PC00.PEG0.PPRW, MethodObj)
External(\_SB.PC00.PEG1.PPRW, MethodObj)
External(\_SB.PC00.PEG2.PPRW, MethodObj)
External(\_SB.PC00.PEG3.PPRW, MethodObj)
External(\_SB.PC00.PC2M, MethodObj)
External(P8XH, MethodObj)
External(SPCO, MethodObj)
External(PINI, MethodObj) // Platform specific PCIe root port initialization
External(PRES, MethodObj)
External(GPRW, MethodObj)
External(\SLOT)
External(\P0WK)
External(\P1WK)
External(\P2WK)
External(\P3WK)
External(\XBAS)
External(\SBN0)
External(\SBN1)
External(\SBN2)
External(\SBN3)
External(\EECP)
External(\EEC1)
External(\EEC2)
External(\EEC3)
External(\SGGP)
External(\HRE0)
External(\HRG0)
External(\HRA0)
External(\PWE0)
External(\PWG0)
External(\PWA0)
External(\P1GP)
External(\HRE1)
External(\HRG1)
External(\HRA1)
External(\PWE1)
External(\PWG1)
External(\PWA1)
External(\P2GP)
External(\HRE2)
External(\HRG2)
External(\HRA2)
External(\PWE2)
External(\PWG2)
External(\PWA2)
External(\P3GP)
External(\HRE3)
External(\HRG3)
External(\HRA3)
External(\PWE3)
External(\PWG3)
External(\PWA3)
External(\P0SC)
External(\P1SC)
External(\P2SC)
External(\P3SC)
External(\DLPW)
External(\DLHR)
External(\OBFX)
External(\OBFY)
External(\OBFZ)
External(\OBFA)
External(\OSYS)

//GPE Event handling - Start
Scope(\_GPE) {
  //
  // _L6F Method call for PEG0/1/2/3 ports to handle 2-tier RTD3 GPE events
  //
  Method(P0L6,0)
  {
    // PEG0 Device Wake Event
    If (\_SB.ISME(P0WK))
    {
      \_SB.SHPO(P0WK, 1)             // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
      Notify(\_SB.PC00.PEG0, 0x02)   // device wake
      \_SB.CAGS(P0WK)                // Clear GPE status bit for PEG0 WAKE
    }
  }

  Method(P1L6,0)
  {
    // PEG1 Device Wake Event
    If (\_SB.ISME(P1WK))
    {
      \_SB.SHPO(P1WK, 1)             // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
      Notify(\_SB.PC00.PEG1, 0x02)   // device wake
      \_SB.CAGS(P1WK)                // Clear GPE status bit for PEG1 WAKE
    }
  }

  Method(P2L6,0)
  {
    // PEG2 Device Wake Event
    If (\_SB.ISME(P2WK))
    {
      \_SB.SHPO(P2WK, 1)             // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
      Notify(\_SB.PC00.PEG2, 0x02)   // device wake
      \_SB.CAGS(P2WK)                // Clear GPE status bit for PEG2 WAKE
    }
  }

  If (CondRefOf(\_SB.PC00.PEG3)) {
    Method(P3L6,0)
    {
      // PEG2 Device Wake Event
      If (\_SB.ISME(P3WK))
      {
        \_SB.SHPO(P3WK, 1)             // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
        Notify(\_SB.PC00.PEG3, 0x02)   // device wake
        \_SB.CAGS(P3WK)                // Clear GPE status bit for PEG2 WAKE
      }
    }
  }
} //Scope(\_GPE)

If(LAnd((LEqual(HGMD,2)), (LEqual(HGST,1)))) {
///
/// P.E.G. Root Port D6F0
///
Scope(\_SB.PC00.PEG0) {
  Name(WKEN, 0)

  PowerResource(PG00, 0, 0) {
    Name(_STA, One)
    Method(_ON, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGON(0)
        Store(One, _STA)
      }
    }
    Method(_OFF, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGOF(0)
        Store(Zero, _STA)
      }
    }
  } //End of PowerResource(PG00, 0, 0)

  Name(_PR0,Package(){PG00})
  Name(_PR3,Package(){PG00})

  ///
  /// This method is used to enable/disable wake from PEG60 (WKEN)
  ///
  Method(_DSW, 3)
  {
    If(Arg1)
    {
      Store(0, WKEN)        /// If entering Sx, need to disable WAKE# from generating runtime PME
    }
    Else
    { /// If Staying in S0
      If(LAnd(Arg0, Arg2))  ///- Check if Exiting D0 and arming for wake
      {
        Store(1, WKEN)      ///- Set PME
      } Else {
        Store(0, WKEN)    ///- Disable runtime PME, either because staying in D0 or disabling wake
      }
    }
  } // End _DSW

  ///
  /// This method is used to change the GPIO ownership back to ACPI and will be called in PEG OFF Method
  ///
  Method(P0EW, 0)
  {
    If(WKEN)
    {
      If(LNotEqual(SGGP, 0x0))
      {
        If(LEqual(SGGP, 0x1))      // GPIO mode
        {
          \_SB.SGOV(P0WK, 0x1)
          \_SB.SHPO(P0WK, 0x0)     // set gpio ownership to ACPI(0=ACPI mode, 1=GPIO mode)
        }
      }
    }
  } // End P0EW

  Method(_S0W, 0) {
    Return(4) //D3cold is supported
  }
}// end "P.E.G. Root Port D6F0"

///
/// P.E.G. Root Port D1F0
///
Scope(\_SB.PC00.PEG1) {
  Name(WKEN, 0)

  PowerResource(PG01, 0, 0) {
    Name(_STA, One)
    Method(_ON, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGON(1)
        Store(One, _STA)
      }
    }
    Method(_OFF, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGOF(1)
        Store(Zero, _STA)
      }
    }
  } //End of PowerResource(PG01, 0, 0)

  Name(_PR0,Package(){PG01})
  Name(_PR3,Package(){PG01})

  ///
  /// This method is used to enable/disable wake from PEG10 (WKEN)
  ///
  Method(_DSW, 3)
  {
    If(Arg1)
    {
      Store(0, WKEN)        /// If entering Sx, need to disable WAKE# from generating runtime PME
    }
    Else
    { /// If Staying in S0
      If(LAnd(Arg0, Arg2))  ///- Check if Exiting D0 and arming for wake
      {
        Store(1, WKEN)      ///- Set PME
      } Else {
        Store(0, WKEN)    ///- Disable runtime PME, either because staying in D0 or disabling wake
      }
    }
  } // End _DSW

  ///
  /// This method is used to change the GPIO ownership back to ACPI and will be called in PEG OFF Method
  ///
  Method(P1EW, 0)
  {
    If(WKEN)
    {
      If(LNotEqual(P1GP, 0x0))
      {
        If(LEqual(P1GP, 0x1))      // GPIO mode
        {
          \_SB.SGOV(P1WK, 0x1)
          \_SB.SHPO(P1WK, 0x0)     // set gpio ownership to ACPI(0=ACPI mode, 1=GPIO mode)
        }
      }
    }
  } // End P1EW
}// end "P.E.G. Root Port D1F0"

///
/// P.E.G. Root Port D1F1
///
Scope(\_SB.PC00.PEG2) {
  Name(WKEN, 0)

  PowerResource(PG02, 0, 0) {
    Name(_STA, One)
    Method(_ON, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGON(2)
        Store(One, _STA)
      }
    }
    Method(_OFF, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGOF(2)
        Store(Zero, _STA)
      }
    }
  } //End of PowerResource(PG02, 0, 0)

  Name(_PR0,Package(){PG02})

  Name(_PR3,Package(){PG02})

  ///
  /// This method is used to enable/disable wake from PEG11 (WKEN)
  ///
  Method(_DSW, 3)
  {
    If(Arg1)
    {
      Store(0, WKEN)        /// If entering Sx, need to disable WAKE# from generating runtime PME
    }
    Else
    { /// If Staying in S0
      If(LAnd(Arg0, Arg2))  ///- Check if Exiting D0 and arming for wake
      {
        Store(1, WKEN)      ///- Set PME
      } Else {
        Store(0, WKEN)    ///- Disable runtime PME, either because staying in D0 or disabling wake
      }
    }
  } // End _DSW

  ///
  /// This method is used to change the GPIO ownership back to ACPI and will be called in PEG OFF Method
  ///
  Method(P2EW, 0)
  {
    If(WKEN)
    {
      If(LNotEqual(P2GP, 0x0))
      {
        If(LEqual(P2GP, 0x1))      // GPIO mode
        {
          \_SB.SGOV(P2WK, 0x1)
          \_SB.SHPO(P2WK, 0x0)     // set gpio ownership to ACPI(0=ACPI mode, 1=GPIO mode)
        }
      }
    }
  } // End P2EW
}// end "P.E.G. Root Port D1F1"

///
/// P.E.G. Root Port D1F2
///
Scope(\_SB.PC00.PEG3) {
  Name(WKEN, 0)

  PowerResource(PG03, 0, 0) {
    Name(_STA, One)
    Method(_ON, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGON(3)
        Store(One, _STA)
      }
    }
    Method(_OFF, 0, Serialized) {
      If(LGreater(OSYS,2009)) {
        PGOF(3)
        Store(Zero, _STA)
      }
    }
  } //End of PowerResource(PG03, 0, 0)

  Name(_PR0,Package(){PG03})
  Name(_PR3,Package(){PG03})

  ///
  /// This method is used to enable/disable wake from PEG12 (WKEN)
  ///
  Method(_DSW, 3)
  {
    If(Arg1)
    {
      Store(0, WKEN)        /// If entering Sx, need to disable WAKE# from generating runtime PME
    }
    Else
    { /// If Staying in S0
      If(LAnd(Arg0, Arg2))  ///- Check if Exiting D0 and arming for wake
      {
        Store(1, WKEN)      ///- Set PME
      } Else {
        Store(0, WKEN)    ///- Disable runtime PME, either because staying in D0 or disabling wake
      }
    }
  } // End _DSW

  ///
  /// This method is used to change the GPIO ownership back to ACPI and will be called in PEG OFF Method
  ///
  Method(P3EW, 0)
  {
    If(WKEN)
    {
      If(LNotEqual(P3GP, 0x0))
      {
        If(LEqual(P3GP, 0x1))      // GPIO mode
        {
          \_SB.SGOV(P3WK, 0x1)
          \_SB.SHPO(P3WK, 0x0)     // set gpio ownership to ACPI(0=ACPI mode, 1=GPIO mode)
        }
      }
    }
  } // End P3EW
}// end "P.E.G. Root Port D1F2"

Scope (\_SB.PC00) {

    Name(IVID, 0xFFFF) //Invalid Vendor ID

    Name(PEBA, 0) //PCIE base address

    Name(PION, 0) //PEG index for ON Method
    Name(PIOF, 0) //PEG index for OFF Method

    Name(PBUS, 0) //PEG Rootport bus no
    Name(PDEV, 0) //PEG Rootport device no
    Name(PFUN, 0) //PEG Rootport function no

    Name(EBUS, 0) //Endpoint bus no
    Name(EDEV, 0) //Endpoint device no
    Name(EFN0, 0) //Endpoint function no 0
    Name(EFN1, 1) //Endpoint function no 1

    Name(LTRS, 0)
    Name(OBFS, 0)

    Name(DSOF, 0x06) //Device status PCI offset
    Name(CPOF, 0x34) //Capabilities pointer PCI offset
    Name(SBOF, 0x19) //PCI-2-PCI Secondary Bus number

    // PEG0 Endpoint variable to save/restore Link Capability, Link control, Subsytem VendorId and Device Id
    Name (ELC0, 0x00000000)
    Name (ECP0, 0xffffffff)
    Name (H0VI, 0x0000)
    Name (H0DI, 0x0000)

    // PEG1 Endpoint variable to save/restore Link Capability, Link control, Subsytem VendorId and Device Id
    Name (ELC1, 0x00000000)
    Name (ECP1, 0xffffffff)
    Name (H1VI, 0x0000)
    Name (H1DI, 0x0000)

    // PEG2 Endpoint variable to save/restore Link Capability, Link control, Subsytem VendorId and Device Id
    Name (ELC2, 0x00000000)
    Name (ECP2, 0xffffffff)
    Name (H2VI, 0x0000)
    Name (H2DI, 0x0000)

    // PEG3 Endpoint variable to save/restore Link Capability, Link control, Subsytem VendorId and Device Id
    Name (ELC3, 0x00000000)
    Name (ECP3, 0xffffffff)
    Name (H3VI, 0x0000)
    Name (H3DI, 0x0000)

    // PEG_AFELN[15:0]VMTX2_OFFSET variables
    Name(AFL0, 0)
    Name(AFL1, 0)
    Name(AFL2, 0)
    Name(AFL3, 0)
    Name(AFL4, 0)
    Name(AFL5, 0)
    Name(AFL6, 0)
    Name(AFL7, 0)
    Name(AFL8, 0)
    Name(AFL9, 0)
    Name(AFLA, 0)
    Name(AFLB, 0)
    Name(AFLC, 0)
    Name(AFLD, 0)
    Name(AFLE, 0)
    Name(AFLF, 0)

    //
    // Define a Memory Region for PEG60 root port that will allow access to its
    // Register Block.
    //
    OperationRegion(OPG0, SystemMemory, Add(XBAS,0x30000), 0x1000)
    Field(OPG0, AnyAcc,NoLock,Preserve)
    {
      Offset(0),
      P0VI,   16,        //Vendor ID PCI offset
      P0DI,   16,        //Device ID PCI offset
      Offset(0x06),
      DSO0,   16,        //Device status PCI offset
      Offset(0x34),
      CPO0,   8,         //Capabilities pointer PCI offset
      Offset(0x0B0),
      ,       4,
      P0LD,   1,         //Link Disable
      Offset(0x11A),
      ,       1,
      P0VC,   1,         //VC0RSTS.VC0NP
      Offset(0x214),
      ,       16,
      P0LS,   4,         //PEGSTS.LKS
      Offset(0x248),
      ,       7,
      Q0L2,   1,         //L23_Rdy Entry Request for RTD3
      Q0L0,   1,         //L23 to Detect Transition for RTD3
      Offset(0x504),
      HST0,  32,
      Offset(0x508),
      P0TR,   1,         //TRNEN.TREN
      Offset(0xC74),
      P0LT,   4,         //LTSSM_FSM_RESTORE.LTSSM_FSM_PS
      Offset(0xD0C),
      LRV0,  32,
    }

    //
    // Define a Memory Region for Endpoint on PEG60 root port
    //
    OperationRegion (PCS0, SystemMemory, Add(XBAS,ShiftLeft(SBN0,20)), 0xF0)
    Field(PCS0, DWordAcc, Lock, Preserve)
    {
        Offset(0x0),
        D0VI, 16,
        Offset(0x2C),
        S0VI, 16,
        S0DI, 16,
    }

    OperationRegion(CAP0, SystemMemory, Add(Add(XBAS,ShiftLeft(SBN0,20)),EECP),0x14)
    Field(CAP0,DWordAcc, NoLock,Preserve)
    {
        Offset(0x0C),                    // Link Capabilities Register
        LCP0,   32,                      // Link Capabilities Register Data
        Offset(0x10),
        LCT0,   16,                      // Link Control register
    }

    //
    // Define a Memory Region for PEG10 root port that will allow access to its
    // Register Block.
    //
    OperationRegion(OPG1, SystemMemory, Add(XBAS,0x8000), 0x1000)
    Field(OPG1, AnyAcc,NoLock,Preserve)
    {
      Offset(0),
      P1VI,   16,        //Vendor ID PCI offset
      P1DI,   16,        //Device ID PCI offset
      Offset(0x06),
      DSO1,   16,        //Device status PCI offset
      Offset(0x34),
      CPO1,   8,         //Capabilities pointer PCI offset
      Offset(0x0B0),
      ,       4,
      P1LD,   1,         //Link Disable
      Offset(0x11A),
      ,       1,
      P1VC,   1,         //VC0RSTS.VC0NP
      Offset(0x214),
      ,       16,
      P1LS,   4,         //PEGSTS.LKS
      Offset(0x248),
      ,       7,
      Q1L2,   1,         //L23_Rdy Entry Request for RTD3
      Q1L0,   1,         //L23 to Detect Transition for RTD3
      Offset(0x504),
      HST1,  32,
      Offset(0x508),
      P1TR,   1,         //TRNEN.TREN
      Offset(0x70C),
      PA0V,  32,         //PEG_AFELN0VMTX2_OFFSET
      Offset(0x71C),
      PA1V,  32,         //PEG_AFELN1VMTX2_OFFSET
      Offset(0x72C),
      PA2V,  32,         //PEG_AFELN2VMTX2_OFFSET
      Offset(0x73C),
      PA3V,  32,         //PEG_AFELN3VMTX2_OFFSET
      Offset(0x74C),
      PA4V,  32,         //PEG_AFELN4VMTX2_OFFSET
      Offset(0x75C),
      PA5V,  32,         //PEG_AFELN5VMTX2_OFFSET
      Offset(0x76C),
      PA6V,  32,         //PEG_AFELN6VMTX2_OFFSET
      Offset(0x77C),
      PA7V,  32,         //PEG_AFELN7VMTX2_OFFSET
      Offset(0x78C),
      PA8V,  32,         //PEG_AFELN8VMTX2_OFFSET
      Offset(0x79C),
      PA9V,  32,         //PEG_AFELN9VMTX2_OFFSET
      Offset(0x7AC),
      PAAV,  32,         //PEG_AFELNAVMTX2_OFFSET
      Offset(0x7BC),
      PABV,  32,         //PEG_AFELNBVMTX2_OFFSET
      Offset(0x7CC),
      PACV,  32,         //PEG_AFELNCVMTX2_OFFSET
      Offset(0x7DC),
      PADV,  32,         //PEG_AFELNDVMTX2_OFFSET
      Offset(0x7EC),
      PAEV,  32,         //PEG_AFELNEVMTX2_OFFSET
      Offset(0x7FC),
      PAFV,  32,         //PEG_AFELNFVMTX2_OFFSET
      Offset(0x91C),
      ,       31,
      BSP1,   1,
      Offset(0x93C),
      ,       31,
      BSP2,   1,
      Offset(0x95C),
      ,       31,
      BSP3,   1,
      Offset(0x97C),
      ,       31,
      BSP4,   1,
      Offset(0x99C),
      ,       31,
      BSP5,   1,
      Offset(0x9BC),
      ,       31,
      BSP6,   1,
      Offset(0x9DC),
      ,       31,
      BSP7,   1,
      Offset(0x9FC),
       ,       31,
      BSP8,   1,
      Offset(0xC20),
      ,       4,
      P1AP,   2,         //AFEOVR.RXSQDETOVR
      Offset(0xC38),
      ,       3,
      P1RM,   1,         //CMNSPARE.PCUNOTL1
      Offset(0xC3C),
      ,       31,
      PRST,   1,
      Offset(0xC74),
      P1LT,   4,         //LTSSM_FSM_RESTORE.LTSSM_FSM_PS
      Offset(0xD0C),
      LRV1,  32,
    }

    //
    // Define a Memory Region for Endpoint on PEG10 root port
    //
    OperationRegion (PCS1, SystemMemory, Add(XBAS,ShiftLeft(SBN1,20)), 0xF0)
    Field(PCS0, DWordAcc, Lock, Preserve)
    {
        Offset(0x0),
        D1VI, 16,
        Offset(0x2C),
        S1VI, 16,
        S1DI, 16,
    }

    OperationRegion(CAP1, SystemMemory, Add(Add(XBAS,ShiftLeft(SBN1,20)),EEC1),0x14)
    Field(CAP0,DWordAcc, NoLock,Preserve)
    {
        Offset(0x0C),                    // Link Capabilities Register
        LCP1,   32,                      // Link Capabilities Register Data
        Offset(0x10),
        LCT1,   16,                      // Link Control register
    }

    //
    // Define a Memory Region for PEG11 root port that will allow access to its
    // Register Block.
    //
    OperationRegion(OPG2, SystemMemory, Add(XBAS,0x9000), 0x1000)
    Field(OPG2, AnyAcc,NoLock,Preserve)
    {
      Offset(0),
      P2VI,   16,        //Vendor ID PCI offset
      P2DI,   16,        //Device ID PCI offset
      Offset(0x06),
      DSO2,   16,        //Device status PCI offset
      Offset(0x34),
      CPO2,   8,         //Capabilities pointer PCI offset
      Offset(0x0B0),
      ,       4,
      P2LD,   1,         //Link Disable
      Offset(0x11A),
      ,       1,
      P2VC,   1,         //VC0RSTS.VC0NP
      Offset(0x214),
      ,       16,
      P2LS,   4,         //PEGSTS.LKS
      Offset(0x248),
      ,       7,
      Q2L2,   1,         //L23_Rdy Entry Request for RTD3
      Q2L0,   1,         //L23 to Detect Transition for RTD3
      Offset(0x504),
      HST2,  32,
      Offset(0x508),
      P2TR,   1,         //TRNEN.TREN
      Offset(0xC20),
      ,       4,
      P2AP,   2,         //AFEOVR.RXSQDETOVR
      Offset(0xC38),
      ,       3,
      P2RM,   1,         //CMNSPARE.PCUNOTL1
      Offset(0xC74),
      P2LT,   4,         //LTSSM_FSM_RESTORE.LTSSM_FSM_PS
      Offset(0xD0C),
      LRV2,  32,
    }

    //
    // Define a Memory Region for Endpoint on PEG11 root port
    //
    OperationRegion (PCS2, SystemMemory, Add(XBAS,ShiftLeft(SBN2,20)), 0xF0)
    Field(PCS2, DWordAcc, Lock, Preserve)
    {
        Offset(0x0),
        D2VI, 16,
        Offset(0x2C),
        S2VI, 16,
        S2DI, 16,
    }

    OperationRegion(CAP2, SystemMemory, Add(Add(XBAS,ShiftLeft(SBN2,20)),EEC2),0x14)
    Field(CAP2,DWordAcc, NoLock,Preserve)
    {
        Offset(0x0C),    // Link Capabilities Register
        LCP2,   32,      // Link Capabilities Register Data
        Offset(0x10),
        LCT2,   16,      // Link Control register
    }


    //
    // Define a Memory Region for PEG12 root port that will allow access to its
    // Register Block.
    //
    OperationRegion(OPG3, SystemMemory, Add(XBAS,0xA000), 0x1000)
    Field(OPG3, AnyAcc,NoLock,Preserve)
    {
      Offset(0),
      P3VI,   16,        //Vendor ID PCI offset
      P3DI,   16,        //Device ID PCI offset
      Offset(0x06),
      DSO3,   16,        //Device status PCI offset
      Offset(0x34),
      CPO3,   8,         //Capabilities pointer PCI offset
      Offset(0x0B0),
      ,       4,
      P3LD,   1,         //Link Disable
      Offset(0x11A),
      ,       1,
      P3VC,   1,         //VC0RSTS.VC0NP
      Offset(0x214),
      ,       16,
      P3LS,   4,         //PEGSTS.LKS
      Offset(0x248),
      ,       7,
      Q3L2,   1,         //L23_Rdy Entry Request for RTD3
      Q3L0,   1,         //L23 to Detect Transition for RTD3
      Offset(0x504),
      HST3,  32,
      Offset(0x508),
      P3TR,   1,         //TRNEN.TREN
      Offset(0xC20),
      ,       4,
      P3AP,   2,         //AFEOVR.RXSQDETOVR
      Offset(0xC38),
      ,       3,
      P3RM,   1,         //CMNSPARE.PCUNOTL1
      Offset(0xC74),
      P3LT,   4,         //LTSSM_FSM_RESTORE.LTSSM_FSM_PS
      Offset(0xD0C),
      LRV3,  32,
    }

    //
    // Define a Memory Region for Endpoint on PEG2 root port
    //
    OperationRegion (PCS3, SystemMemory, Add(XBAS,ShiftLeft(SBN3,20)), 0xF0)
    Field(PCS2, DWordAcc, Lock, Preserve)
    {
        Offset(0x0),
        D3VI, 16,
        Offset(0x2C),
        S3VI, 16,
        S3DI, 16,
    }

    OperationRegion(CAP3, SystemMemory, Add(Add(XBAS,ShiftLeft(SBN3,20)),EEC3),0x14)
    Field(CAP3,DWordAcc, NoLock,Preserve)
    {
        Offset(0x0C),    // Link Capabilities Register
        LCP3,   32,      // Link Capabilities Register Data
        Offset(0x10),
        LCT3,   16,      // Link Control register
    }

    //
    // Name: PGON
    // Description: Function to put the Pcie Endpoint in ON state
    // Input: Arg0 -> PEG index
    // Return: Nothing
    //
    Method(PGON,1,Serialized)
    {
      Store(Arg0, PION)

      //
      // Check for the GPIO support on PEG0/1/2/3 Configuration and Return if it is not supported.
      //
      If (LEqual(PION, 0))
      {
        If (LEqual(SGGP, 0x0))
        {
          Return ()
        }
      }
      ElseIf (LEqual(PION, 1))
      {
        If (LEqual(P1GP, 0x0))
        {
          Return ()
        }
      }
      ElseIf (LEqual(PION, 2))
      {
        If (LEqual(P2GP, 0x0))
        {
          Return ()
        }
      }
      ElseIf (LEqual(PION, 3))
      {
        If (LEqual(P3GP, 0x0))
        {
          Return ()
        }
      }
      Store(\XBAS, PEBA)
      Store(GDEV(PIOF), PDEV)
      Store(GFUN(PIOF), PFUN)

      /// de-assert CLK_REQ MSK
      PGSC(Arg0, 1)

      If (LEqual(CCHK(PION, 1), 0))
      {
        Return ()
      }

      //Power on the Endpoint
      GPPR(PION, 1)

      // Restore PEG Recipe before program L23R2DT
      //\_SB.PC00.PEG1.RAVR()

      // Enable link for RTD3
      RTEN()

      // Re-store the DGPU Subsystem VendorID, DeviceID & Link control register data
      If (LEqual(PION, 0))
      {
        Store(H0VI, S0VI)
        Store(H0DI, S0DI)
        Or(And(ELC0,0x0043),And(LCT0,0xFFBC),LCT0)
      }
      ElseIf (LEqual(PION, 1))
      {
        Store(H1VI, S1VI)
        Store(H1DI, S1DI)
        Or(And(ELC1,0x0043),And(LCT1,0xFFBC),LCT1)
      }
      ElseIf (LEqual(PION, 2))
      {
        Store(H2VI, S2VI)
        Store(H2DI, S2DI)
        Or(And(ELC2,0x0043),And(LCT2,0xFFBC),LCT2)
      }
      ElseIf (LEqual(PION, 3))
      {
        Store(H3VI, S3VI)
        Store(H3DI, S3DI)
        Or(And(ELC3,0x0043),And(LCT3,0xFFBC),LCT3)
      }
      Return ()
    } // End of Method(PGON,1,Serialized)

    //
    // Name: PGOF
    // Description: Function to put the Pcie Endpoint in OFF state
    // Input: Arg0 -> PEG index
    // Return: Nothing
    //
    Method(PGOF,1,Serialized)
    {

      Store(Arg0, PIOF)
      //
      // Check for the GPIO support on PEG0/1/2 Configuration and Return if it is not supported.
      //
      If (LEqual(PIOF, 0))
      {
        If (LEqual(SGGP, 0x0))
        {
          Return ()
        }
      }
      ElseIf (LEqual(PIOF, 1))
      {
        If (LEqual(P1GP, 0x0))
        {
          Return ()
        }
      }
      ElseIf (LEqual(PIOF, 2))
      {
        If (LEqual(P2GP, 0x0))
        {
          Return ()
        }
      }
      ElseIf (LEqual(PIOF, 3))
      {
        If (LEqual(P3GP, 0x0))
        {
          Return ()
        }
      }

      Store(\XBAS, PEBA)
      Store(GDEV(PIOF), PDEV)
      Store(GFUN(PIOF), PFUN)

      If (LEqual(CCHK(PIOF, 0), 0))
      {
        Return ()
      }

      // Save Endpoint Link Control register, Subsystem VendorID & Device ID, Link capability Data
      If (LEqual(Arg0, 0)) //PEG60
      {
        Store(LCT0, ELC0)
        Store(S0VI, H0VI)
        Store(S0DI, H0DI)
        Store(LCP0, ECP0)
      }
      ElseIf (LEqual(Arg0, 1)) //PEG10
      {
        Store(LCT1, ELC1)
        Store(S1VI, H1VI)
        Store(S1DI, H1DI)
        Store(LCP1, ECP1)
      }
      ElseIf (LEqual(Arg0, 2)) //PEG11
      {
        Store(LCT2, ELC2)
        Store(S2VI, H2VI)
        Store(S2DI, H2DI)
        Store(LCP2, ECP2)
      }
      ElseIf (LEqual(Arg0, 3)) //PEG12
      {
        Store(LCT3, ELC3)
        Store(S3VI, H3VI)
        Store(S3DI, H3DI)
        Store(LCP3, ECP3)
      }

      //\_SB.PC00.PEG0.SAVR()

      // Put link in L2
      RTDS()

      /// assert CLK_REQ MSK
      ///
      /// On RTD3 entry, BIOS will instruct the PMC to disable source clocks.
      /// This is done through sending a PMC IPC command.
      ///
      PGSC(Arg0, 0)

      //Power-off the Endpoint
      GPPR(PIOF, 0)
      //Method to set Wake GPIO ownership from GPIO to ACPI for Device Initiated RTD3
      DIWK(PIOF)

      Return ()
    } // End of Method(PGOF,1,Serialized)


    //
    // Name: GDEV
    // Description: Function to return the PEG device no for the given PEG index
    // Input: Arg0 -> PEG index
    // Return: PEG device no for the given PEG index
    //
    Method(GDEV,1)
    {
      If(LEqual(Arg0, 0))
      {
        Store(0x6, Local0) //Device6-Function0 = 00110.000
      }
      ElseIf(LEqual(Arg0, 1))
      {
        Store(0x1, Local0) //Device1-Function0 = 00001.000
      }
      ElseIf(LEqual(Arg0, 2))
      {
        Store(0x1, Local0) //Device1-Function2 = 00001.001
      }
      ElseIf(LEqual(Arg0, 3))
      {
        Store(0x1, Local0) //Device1-Function3 = 00001.010
      }

      Return(Local0)
    } // End of Method(GDEV,1)

    //
    // Name: GFUN
    // Description: Function to return the PEG function no for the given PEG index
    // Input: Arg0 -> PEG index
    // Return: PEG function no for the given PEG index
    //
    Method(GFUN,1)
    {
      If(LEqual(Arg0, 0))
      {
        Store(0x0, Local0) //Device6-Function0 = 00110.000
      }
      ElseIf(LEqual(Arg0, 1))
      {
        Store(0x0, Local0) //Device1-Function0 = 00001.000
      }
      ElseIf(LEqual(Arg0, 2))
      {
        Store(0x1, Local0) //Device1-Function1 = 00001.001
      }
      ElseIf(LEqual(Arg0, 2))
      {
        Store(0x2, Local0) //Device1-Function2 = 00001.010
      }

      Return(Local0)
    } // End of Method(GFUN,1)

    //
    // Name: CCHK
    // Description: Function to check whether _ON/_OFF sequence is allowed to execute for the given PEG controller or not
    // Input: Arg0 -> PEG index
    //        Arg1 -> 0 means _OFF sequence, 1 means _ON sequence
    // Return: 0 - Don't execute the flow, 1 - Execute the flow
    //
    Method(CCHK,2)
    {

      //Check for Referenced PEG controller is present or not
      If(LEqual(Arg0, 0))
      {
        Store(P0VI, Local7)
      }
      ElseIf(LEqual(Arg0, 1))
      {
        Store(P1VI, Local7)
      }
      ElseIf(LEqual(Arg0, 2))
      {
        Store(P2VI, Local7)
      }
      ElseIf(LEqual(Arg0, 3))
      {
        Store(P3VI, Local7)
      }

      If(LEqual(Local7, IVID))
      {
        Return(0)
      }

      If(LNotEqual(Arg0, 1))
      {
        //Check for PEG10 controller presence
        Store(P1VI, Local7)
        If(LEqual(Local7, IVID))
        {
          Return(0)
        }
      }

      //If Endpoint is not present[already disabled] before executing PGOF then don't call the PGOF method
      //If Endpoint is present[already enabled] before executing PGON then don't call the PGON method
      If(LEqual(Arg1, 0))
      {
        //_OFF sequence condition check
        If(LEqual(Arg0, 0))
        {
          If(LEqual(SGPI(SGGP, PWE0, PWG0, PWA0), 0))
          {
            Return(0)
          }
        }
        If(LEqual(Arg0, 1))
        {
          If(LEqual(SGPI(P1GP, PWE1, PWG1, PWA1), 0))
          {
            Return(0)
          }
        }
        If(LEqual(Arg0, 2))
        {
          If(LEqual(SGPI(P2GP, PWE2, PWG2, PWA2), 0))
          {
            Return(0)
          }
        }
        If(LEqual(Arg0, 3))
        {
          If(LEqual(SGPI(P3GP, PWE3, PWG3, PWA3), 0))
          {
            Return(0)
          }
        }
      }
      ElseIf(LEqual(Arg1, 1))
      {
        //_ON sequence condition check
        If(LEqual(Arg0, 0))
        {
          If(LEqual(SGPI(SGGP, PWE0, PWG0, PWA0), 1))
          {
            Return(0)
          }
        }
        If(LEqual(Arg0, 1))
        {
          If(LEqual(SGPI(P1GP, PWE1, PWG1, PWA1), 1))
          {
            Return(0)
          }
        }
        If(LEqual(Arg0, 2))
        {
          If(LEqual(SGPI(P2GP, PWE2, PWG2, PWA2), 1))
          {
            Return(0)
          }
        }
        If(LEqual(Arg0, 3))
        {
          If(LEqual(SGPI(P3GP, PWE3, PWG3, PWA3), 1))
          {
            Return(0)
          }
        }
      }

      Return(1)
    }


    //
    // Name: SGPI [PCIe GPIO Read]
    // Description: Function to Read from PCIe GPIO
    // Input: Arg0 -> Gpio Support
    //        Arg1 -> Expander Number
    //        Arg2 -> Gpio Number
    //        Arg3 -> Active Information
    // Return: GPIO value
    //
    Method(SGPI, 4, Serialized)
    {
      If (LEqual(Arg0, 0x01))
      {
        //
        // PCH based GPIO
        //
        If (CondRefOf(\_SB.GGOV))
        {
          Store(\_SB.GGOV(Arg2), Local0)
        }
      }
      //
      // Invert if Active Low
      //
      If (LEqual(Arg3,0))
      {
        Not(Local0, Local0)
        And (Local0, 0x01, Local0)
      }

      Return(Local0)
    }// End of Method(SGPI)

    // Name: PGSC [PEG port source clock control]
    // Description: Function to enable/disable PEG port source clocks
    // Input: Arg0 -> PEG index
    //        Arg1 -> Enable/Disable Clock (0 = Disable, 1 = Enable)
    // Return: Nothing
    //

    Method(PGSC, 2, Serialized)
    {
      If(LEqual(Arg0, 0)) { // PEG0
        Store (P0SC, Local0)
      } ElseIf(LEqual(Arg0, 1)) { // PEG1
        Store (P1SC, Local0)
      } ElseIf(LEqual(Arg0, 2)) { // PEG2
        Store (P2SC, Local0)
      } ElseIf(LEqual(Arg0, 3)) {// PEG3
        Store (P3SC, Local0)
      } Else {
        Return()
      }

      SPCO (Local0, Arg1)
    }// End of Method(PGSC)

    //
    // Name: GPPR
    // Description: Function to do Endpoint ON/OFF using GPIOs
    //              There are two GPIOs currently used to control Third Party Vendor[TPV] DGPU Endpoint devices:
    //              (1) DGPU_PWR_EN [used for Power control]
    //              (2) DGPU_HOLD_RST[used for Reset control]
    // Input: Arg0 -> PEG index
    //        Arg1 -> 0 means _OFF sequence, 1 means _ON sequence
    // Return: Nothing
    //
    Method(GPPR,2)
    {

      If(LEqual(Arg1, 0))
      {
        //_OFF sequence GPIO programming
        If(LEqual(Arg0, 0))
        {
          SGPO(SGGP, HRE0, HRG0, HRA0, 1) // Assert PCIe0/dGPU_HOLD_RST# (PERST#)
          //Sleep(DLHR)                     // As per the PCIe spec, Wait for 'given'ms after Assert the Reset
          SGPO(SGGP, PWE0, PWG0, PWA0, 0) // Deassert PCIe0/dGPU_PWR_EN#
        }

        If(LEqual(Arg0, 1))
        {
          SGPO(P1GP, HRE1, HRG1, HRA1, 1) // Assert PCIe1_HOLD_RST# (PERST#)
          //Sleep(DLHR)                     // As per the PCIe spec, Wait for 'given'ms after Assert the Reset
          SGPO(P1GP, PWE1, PWG1, PWA1, 0) // Deassert PCIe1_PWR_EN#
        }

        If(LEqual(Arg0, 2))
        {
          SGPO(P2GP, HRE2, HRG2, HRA2, 1) // Assert PCIe2_HOLD_RST# (PERST#)
          //Sleep(DLHR)                     // As per the PCIe spec, Wait for 'given'ms after Assert the Reset
          SGPO(P2GP, PWE2, PWG2, PWA2, 0) // Deassert PCIe2_PWR_EN#
        }

        If(LEqual(Arg0, 3))
        {
          SGPO(P3GP, HRE3, HRG3, HRA3, 1) // Assert PCIe3_HOLD_RST# (PERST#)
          //Sleep(DLHR)                     // As per the PCIe spec, Wait for 'given'ms after Assert the Reset
          SGPO(P3GP, PWE3, PWG3, PWA3, 0) // Deassert PCIe2_PWR_EN#
        }
      }
      ElseIf(LEqual(Arg1, 1))
      {
        //_ON sequence GPIO programming
        If(LEqual(Arg0, 0))
        {
          SGPO(SGGP, PWE0, PWG0, PWA0, 1) //Assert dGPU_PWR_EN#

          //Sleep(DLPW) // Wait for 'given'ms for power to get stable
          SGPO(SGGP, HRE0, HRG0, HRA0, 0) //Deassert dGPU_HOLD_RST# as per the PCIe spec

          //Sleep(DLHR) // Wait for 'given'ms after Deassert
        }

        If(LEqual(Arg0, 1))
        {
          SGPO(P1GP, PWE1, PWG1, PWA1, 1) //Assert dGPU_PWR_EN#

          //Sleep(DLPW) // Wait for 'given'ms for power to get stable
          SGPO(P1GP, HRE1, HRG1, HRA1, 0) //Deassert dGPU_HOLD_RST# as per the PCIe spec

          //Sleep(DLHR) // Wait for 'given'ms after Deassert
        }

        If(LEqual(Arg0, 2))
        {
          SGPO(P2GP, PWE2, PWG2, PWA2, 1) //Assert dGPU_PWR_EN#

          //Sleep(DLPW) // Wait for 'given'ms for power to get stable
          SGPO(P2GP, HRE2, HRG2, HRA2, 0) //Deassert dGPU_HOLD_RST# as per the PCIe spec

          //Sleep(DLHR) // Wait for 'given'ms after Deassert
        }

        If(LEqual(Arg0, 3))
        {
          SGPO(P3GP, PWE3, PWG3, PWA3, 1) //Assert dGPU_PWR_EN#

          //Sleep(DLPW) // Wait for 'given'ms for power to get stable
          SGPO(P3GP, HRE3, HRG3, HRA3, 0) //Deassert dGPU_HOLD_RST# as per the PCIe spec

          //Sleep(DLHR) // Wait for 'given'ms after Deassert
        }
      }
    } // End of Method(GPPR,2)

    //
    // Name: SGPO [PCIe GPIO Write]
    // Description: Function to write into PCIe GPIO
    // Input: Arg0 -> Gpio Support
    //        Arg1 -> Expander Number
    //        Arg2 -> Gpio Number
    //        Arg3 -> Active Information
    //        Arg4 -> Value to write
    // Return: Nothing
    //

    Method(SGPO, 5, Serialized)
    {
      //
      // Invert if Active Low
      //
      If (LEqual(Arg3,0))
      {
        Not(Arg4, Arg4)
        And(Arg4, 0x01, Arg4)
      }
      If (LEqual(Arg0, 0x01))
      {
        //
        // PCH based GPIO
        //
        If (CondRefOf(\_SB.SGOV))
        {
          \_SB.SGOV(Arg2, Arg4)
        }
      }
    } // End of Method(SGPO)

    //
    // Name: DIWK
    // Description: Function which set the GPIO ownership to ACPI for device initiated RTD3
    // Input: PEG Index
    // Return: Nothing
    //
    Method(DIWK,1)
    {
      If (LEqual(Arg0, 0))
      {
        \_SB.PC00.PEG0.P0EW()
      }
      ElseIf (LEqual(Arg0, 1))
      {
        \_SB.PC00.PEG1.P1EW()
      }
      ElseIf (LEqual(Arg0, 2))
      {
        \_SB.PC00.PEG2.P2EW()
      }
      ElseIf (LEqual(Arg0, 3))
      {
        \_SB.PC00.PEG3.P3EW()
      }
    }
  }// End of Scope (\_SB.PC00)
}
