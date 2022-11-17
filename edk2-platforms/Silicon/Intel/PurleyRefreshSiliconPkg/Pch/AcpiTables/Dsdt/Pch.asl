/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Name (PNVB, 0xFFFF0000)  // PCH NVS Base address
Name (PNVL, 0xAA55)      // PCH NVS Length
Include ("PchNvs.asl")


//
// Trace Hub debug library
// Include it earlier so the debug function can be used as soon as possible
//
Include ("TraceHubDebug.asl")

Name(SPTH,1)
Name(SPTL,2)
Method(PCHV) {
  If(LEqual(PCHS, 1)) { Return (SPTH) } // series=H  -> SPT-H
  If(LEqual(PCHS, 2)) { Return (SPTL) } // series=LP -> SPT-LP
  Return (0)
}

//
// This PME event (PCH's GPE 6Dh) is received when any PCH internal device with
// PCI Power Management capabilities on bus 0 asserts the equivalent of the PME# signal.
//
Scope(\_GPE) {
  Method(_L6D, 0, Serialized) {
    \_SB.PC00.XHCI.GPEH()
    \_SB.PC00.CAVS.GPEH()
    \_SB.PC00.GBE1.GPEH()
  }
}

Scope (\_SB.PC00) {

  //
  // PCH reserved resource
  //
  Device(PRRE) {
    Name(_HID,EISAID("PNP0C02")) // motherboard resource
    Name(_UID,"PCHRESV")
    Name(_STA,0x3) // device present and decodes its resources, but not to be displayed in OSPM

    Method(_CRS,0,Serialized)
    {
      Name(BUF0,ResourceTemplate(){
        //
        // PCH RESERVED MMIO RANGE
        // 0xFD000000 to 0xFE7FFFFF
        // to skip over address range that might be claimed by the GPIO, Intel Serial IO, Thermal, TraceHub and CIO2 devices
        // need to split this into 5 ranges
        // The GPIO COMM0,1,3 and SerialIO ranges will be handled by SIRC device.
        //
        Memory32Fixed(ReadWrite,0xFD000000,0x00AC0000) // 0xFD000000 - 0xFDABFFFF
        // Skip 0xFDAC0000 - 0xFDACFFFF for GPIO_COMM3
        Memory32Fixed(ReadWrite,0xFDAD0000,0x00010000) // 0xFDAD0000 - 0xFDADFFFF, only cover GPIO_COMM2 range
        // Skip 0xFDAE0000 - 0xFDAFFFFF for GPIO_COMM0 and GPIO_COMM1
        Memory32Fixed(ReadWrite,0xFDB00000,0x00500000) // 0xFDB00000 - 0xFDFFFFFF
        Memory32Fixed(ReadWrite,0xFE000000,0x00010000) // 0xFE000000 - 0xFE00FFFF
        Memory32Fixed(ReadWrite,0xFE011000,0x0000f000) // 0xFE011000 - 0xFE01FFFF
        // Skip 0xFE020000 - 0xFE035FFF for Serial IO
        Memory32Fixed(ReadWrite,0xFE036000,0x00006000) // 0xFE036000 - 0xFE03BFFF
        // Skip 0xFE03C000 - 0xFE03CFFF for Thermal Device in ACPI mode
        Memory32Fixed(ReadWrite,0xFE03D000,0x003C3000) // 0xFE03D000 - 0xFE3FFFFF
        // Skip 0xFE400000 - 0xFE40FFFF for CIO2 in ACPI mode
        Memory32Fixed(ReadWrite,0xFE410000,0x003F0000) // 0xFE410000 - 0xFE7FFFFF
      })
      Return(BUF0)
    }
  }
  Device(IOTR) {
    //
    // This device claims IO range reserved for IO traps
    // to prevent OS from reusing it for other purposes
    //
    Name(_HID,EISAID("PNP0C02"))
    Name(_UID,"IoTraps")
    Name(BUF0,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG0) })
    Name(BUF1,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG1) })
    Name(BUF2,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG2) })
    Name(BUF3,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG3) })
    CreateWordField(BUF0,TAG0._MIN,AMI0)
    CreateWordField(BUF0,TAG0._MAX,AMA0)
    CreateWordField(BUF1,TAG1._MIN,AMI1)
    CreateWordField(BUF1,TAG1._MAX,AMA1)
    CreateWordField(BUF2,TAG2._MIN,AMI2)
    CreateWordField(BUF2,TAG2._MAX,AMA2)
    CreateWordField(BUF3,TAG3._MIN,AMI3)
    CreateWordField(BUF3,TAG3._MAX,AMA3)
    Method(_CRS) {
      Store(ResourceTemplate() { }, Local0)
      Store(ITA0,AMI0);Store(ITA0,AMA0)
      Store(ITA1,AMI1);Store(ITA1,AMA1)
      Store(ITA2,AMI2);Store(ITA2,AMA2)
      Store(ITA3,AMI3);Store(ITA3,AMA3)
      if(LEqual(ITS0,1)) { ConcatenateResTemplate(Local0, BUF0, Local0) }
      if(LEqual(ITS1,1)) { ConcatenateResTemplate(Local0, BUF1, Local0) }
      if(LEqual(ITS2,1)) { ConcatenateResTemplate(Local0, BUF2, Local0) }
      if(LEqual(ITS3,1)) { ConcatenateResTemplate(Local0, BUF3, Local0) }
      return (Local0)
    }
  }


  //
  // LPC Bridge - Device 31, Function 0, this is only for PCH register Memory Region declare,
  // it's better to be declared as early as possible since it's widely used in whole ACPI name space.
  // Please add any code which needs to reference any register of it after this
  //
  Scope (\_SB.PC00.LPC0) {
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; return(0)}

    OperationRegion(LPC, PCI_Config, 0x00, 0x100)
    Field(LPC, AnyAcc, NoLock, Preserve)
    {
      Offset(0x02),
      CDID, 16,
      Offset(0x08),
      CRID,  8,
      Offset(0x80),
      IOD0,  8,
      IOD1,  8,
      Offset(0xA0),
      , 9,
      PRBL,  1,
      Offset(0xAC),
      ,      8,
      ,      8,
      XUSB,  1,
      Offset(0xB8),
          ,  22,
      GR0B,  2,
      ,      8,
      Offset(0xBC),
      ,      2,
      GR19,  2,
      ,     28,
      Offset(0xDC),
      ,       2,
      ESPI,   1,
    }
  }

  //
  // PCH Power Management Controller
  //
  Scope(\_SB.PC00.PMC1) {
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; return(0)}

    OperationRegion(PMCB, PCI_Config, 0x00, 0x100)
    Field(PMCB, AnyAcc, NoLock, Preserve) {
      VDID, 32,
      Offset(0x40),
          , 8,
      ACBA, 8,
      Offset(0x48),
          , 12,
      PWBA, 20,
    }
  }

  //
  // SMBus Controller - Device 31, Function 4
  //
  Device(SBUS) {
    Name(_ADR,0x001F0004)
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; return(0)}
  }
}


Scope(\)
{
  //
  //  PCR Register Access Methods
  //
  // PCR Dword Read
  // arg0: PID
  // arg1: Offset
  //
  Method (PCRR, 2, Serialized) {
    Add (ShiftLeft (arg0, 16), arg1, Local0)
    Add (SBRG, Local0, Local0)
    OperationRegion (PCR0, SystemMemory, Local0, 0x4)
    Field(PCR0,DWordAcc,Lock,Preserve) {
      Offset(0x00),
      DAT0, 32
    } // End Field PCR0
    Return (DAT0)
  } // End Method PCRR

  //
  // PCR Dword Write
  // arg0: PID
  // arg1: Offset
  // arg2: write data
  //
  Method (PCRW, 3, Serialized) {
    Add (ShiftLeft (arg0, 16), arg1, Local0)
    Add (SBRG, Local0, Local0)
    OperationRegion (PCR0, SystemMemory, Local0, 0x4)
    Field(PCR0,DWordAcc,Lock,Preserve) {
      Offset(0x00),
      DAT0, 32
    } // End Field PCR0
    Store (arg2, DAT0)

    // read back for PCR back to back limitation
    OperationRegion (PCR1, SystemMemory, ADD (SBRG, 0x00C73418), 0x4)
    Field(PCR1,DWordAcc,Lock,Preserve) {
      Offset(0x00),
      DAT1, 32
    } // End Field PCR1
  } // End Method PCRW

  //
  // PCR Dword Or
  // arg0: PID
  // arg1: Offset
  // arg2: Or data
  //
  Method (PCRO, 3, Serialized) {
    Store(PCRR(arg0,arg1),Local0)    // Store PCR Read data in Local0
    Store(Or(Local0,arg2),Local1)    // Or data
    PCRW(arg0,arg1,Local1)           // Write data back
  }

  //
  // PCR Dword And
  // arg0: PID
  // arg1: Offset
  // arg2: And data
  //
  Method (PCRA, 3, Serialized) {
    Store(PCRR(arg0,arg1),Local0)    // Store PCR Read data in Local0
    Store(And(Local0,arg2),Local1)   // And data
    PCRW(arg0,arg1,Local1)           // Write data back
  }

  //
  // PCR Dword AndThenOr
  // arg0: PID
  // arg1: Offset
  // arg2: And data
  // arg3: Or data
  //
  Method (PCAO, 4, Serialized) {
    Store(PCRR(arg0,arg1),Local0)           // Store PCR Read data in Local0
    Store(Or(And(Local0,arg2),arg3),Local1) // AndThenOr
    PCRW(arg0,arg1,Local1)                  // Write data back
  }

  Name (PMBV, 0)        // ACPI I/O base address value
  Method (PMB1, 0) {
    If (LEqual(PMBV, 0)) {
      Store (ShiftLeft (\_SB.PC00.PMC1.ACBA, 8), PMBV)
    }
    Return (PMBV)
  }

  Name (PWRV, 0)        // PWRM base address value
  Method (PWRM, 0) {
    If (LEqual(PWRV, 0)) {
      Store (ShiftLeft (\_SB.PC00.PMC1.PWBA, 12), PWRV)
    }
    Return (PWRV)
  }


  //
  // Define PCH ACPIBASE I/O as an ACPI operating region.  The base address
  // can be found in Device 31, Function 2, Offset 40h.
  //
  OperationRegion(PMIO, SystemIo, PMB1, 0x80)
  Field(PMIO, ByteAcc, NoLock, Preserve) {
          ,  8,
      PBSS,  1,       // Power Button Status
    Offset(0x40),     // General Purpose Event Control
          ,  17,
      GPEC,  1        // Software GPE Control
  }
  OperationRegion(PMLP, SystemIo, Add(\PMB1,0x80), 0x20)
  Field(PMLP, ByteAcc, NoLock, Preserve) {
    Offset(0x10),     // GPE0 Enable
      ,      8,
      GE08,  1,
      ,      8,
      GE17,  1,
      ,     17,
      GE35,  1,
      ,      9,
      GE45,  1,
      ,      2,
      GE48,  1,
      ,      2,
      GE51,  1,
      ,     76,
  }
  Field(PMLP, ByteAcc, NoLock, WriteAsZeros) {
      Offset(0x00),     // GPE0 Status
      ,      8,
      GS08,  1,
      ,      8,
      GS17,  1,
      ,     17,
      GS35,  1,
      ,      9,
      GS45,  1,
      ,      2,
      GS48,  1,
      ,      2,
      GS51,  1,
      ,      2,
      GS54,  1,
      GS55,  1,
      ,     72,
  }



  //
  // PWRM register definitions
  //
  OperationRegion(PWMR, SystemMemory, \PWRM, 0x800)
  Field(PWMR, AnyAcc, NoLock, Preserve) {
    Offset(0x0E0),
        , 16,
    DWLE, 1,          // Deep-Sx WLAN Phy Power Enable
    HWLE, 1,          // Host Wireless LAN Phy Power Enable
  }

  //
  //
  OperationRegion(PMST, SystemMemory, PWRV, 0x80)
  Field(PMST, DWordAcc, NoLock, Preserve) {
    Offset(0x18),  // Power Management Configuration Reg 1 (PM_CFG)
        , 25,      //
    USBP,  1,      // Allow USB2 PHY Core Power Gating (ALLOW_USB2_CORE_PG)
    Offset(0x1C),  // PCH Power Management Status (PCH_PM_STS)
        , 24,      //
    PMFS,  1,      // PMC Message Full Status (PMC_MSG_FULL_STS)
    Offset(0x20),  // Message to PMC (MTPMC)
    MPMC, 32,      // Message to PMC (MTPMC)
    Offset(0x24),  // PCH Power Management Status (PCH_PM_STS2)
        , 20,      //
    UWAB,  1,      // USB2 Workaround Available Bit
  }

} //end Scope(\)

Scope (\_SB.PC00) {
  Name(LTRN, 0)
  Name(OBFN, 0)

  Name(LMSL, 0)
  Name(LNSL, 0)

  //
  // LAN Controller - Device 31, Function 6
  //
  Scope(\_SB.PC00.GBE1) {
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; return(0)}
    OperationRegion(GLBA, PCI_Config, 0,0x100)
    Field(GLBA,AnyAcc,NoLock,Preserve)
    {
      DVID, 16,
      Offset(0xCC),
          ,  8,
      PMEE,  1, // PME Enable
          ,  6,
      PMES,  1, // PME Status
    }

    Method(_PRW, 0) { Return(GPRW(0x0D, 4)) }  // can wakeup from S4 state

    Method(_DSW, 3)
    {
      Store(Arg0, PMEE)
    }

    //
    // GPE handler for GbE, this is part of _Lxx handler for bus 0 PME
    //
    Method(GPEH)
    {
      If(LEqual(DVID, 0xFFFF)) {
        Return()
      }
      If(LAnd(PMEE, PMES)) {
        Store(1, PMES) // clear PME Status
        Notify(GBE1, 0x02)
      }
    }
  } // end "GbE Controller"

} //scope

//
// xHCI Controller - Device 20, Function 0
//
Include("PchXhci.asl")

// xDCI (OTG) Controller is not used in Server
// Comment out as ifdefs don't work at Trim stage of ASL preparation

Scope(\_SB_.PC00) {
  //
  // High Definition Audio Controller - Device 31, Function 3
  //
  include("PchHda.asl")

  //
  // PCIE Root Port #01
  //
  Scope(\_SB.PC00.RP01) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR1, LTRN)
      Store (PML1, LMSL)
      Store (PNL1, LNSL)
      Store (OBF1, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #01"

  //
  // PCIE Root Port #02
  //
  Scope(\_SB.PC00.RP02) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR2, LTRN)
      Store (PML2, LMSL)
      Store (PNL2, LNSL)
      Store (OBF2, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #02"

  //
  // PCIE Root Port #03
  //
  Scope(\_SB.PC00.RP03) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR3, LTRN)
      Store (PML3, LMSL)
      Store (PNL3, LNSL)
      Store (OBF3, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #03"

  //
  // PCIE Root Port #04
  //
  Scope(\_SB.PC00.RP04) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR4, LTRN)
      Store (PML4, LMSL)
      Store (PNL4, LNSL)
      Store (OBF4, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #04"

  //
  // PCIE Root Port #05
  //
  Scope(\_SB.PC00.RP05) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR5, LTRN)
      Store (PML5, LMSL)
      Store (PNL5, LNSL)
      Store (OBF5, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #05"

  //
  // PCIE Root Port #06
  //
  Scope(\_SB.PC00.RP06) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR6, LTRN)
      Store (PML6, LMSL)
      Store (PNL6, LNSL)
      Store (OBF6, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #06"

  //
  // PCIE Root Port #07
  //
  Scope(\_SB.PC00.RP07) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR7, LTRN)
      Store (PML7, LMSL)
      Store (PNL7, LNSL)
      Store (OBF7, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #07"

  //
  // PCIE Root Port #08
  //
  Scope(\_SB.PC00.RP08) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR8, LTRN)
      Store (PML8, LMSL)
      Store (PNL8, LNSL)
      Store (OBF8, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #08"

  //
  // PCIE Root Port #09
  //
  Scope(\_SB.PC00.RP09) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR9, LTRN)
      Store (PML9, LMSL)
      Store (PNL9, LNSL)
      Store (OBF9, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #09"

  //
  // PCIE Root Port #10
  //
  Scope(\_SB.PC00.RP10) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRA, LTRN)
      Store (PMLA, LMSL)
      Store (PNLA, LNSL)
      Store (OBFA, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #10"

  //
  // PCIE Root Port #11
  //
  Scope(\_SB.PC00.RP11) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRB, LTRN)
      Store (PMLB, LMSL)
      Store (PNLB, LNSL)
      Store (OBFB, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #11"

  //
  // PCIE Root Port #12
  //
  Scope(\_SB.PC00.RP12) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRC, LTRN)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
      Store (OBFC, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #12"

  //
  // PCIE Root Port #13
  //
  Scope(\_SB.PC00.RP13) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRD, LTRN)
      Store (PMLD, LMSL)
      Store (PNLD, LNSL)
      Store (OBFD, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #13"

  //
  // PCIE Root Port #14
  //
  Scope(\_SB.PC00.RP14) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRE, LTRN)
      Store (PMLE, LMSL)
      Store (PNLE, LNSL)
      Store (OBFE, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #14"

  //
  // PCIE Root Port #15
  //
  Scope(\_SB.PC00.RP15) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRF, LTRN)
      Store (PMLF, LMSL)
      Store (PNLF, LNSL)
      Store (OBFF, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #15"

  //
  // PCIE Root Port #16
  //
  Scope(\_SB.PC00.RP16) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRG, LTRN)
      Store (PMLG, LMSL)
      Store (PNLG, LNSL)
      Store (OBFG, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #16"

  //
  // PCIE Root Port #17
  //
  Scope(\_SB.PC00.RP17) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRH, LTRN)
      Store (PMLH, LMSL)
      Store (PNLH, LNSL)
      Store (OBFH, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #17"

  //
  // PCIE Root Port #18
  //
  Scope(\_SB.PC00.RP18) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRI, LTRN)
      Store (PMLI, LMSL)
      Store (PNLI, LNSL)
      Store (OBFI, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #18"

  //
  // PCIE Root Port #19
  //
  Scope(\_SB.PC00.RP19) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRJ, LTRN)
      Store (PMLJ, LMSL)
      Store (PNLJ, LNSL)
      Store (OBFJ, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #19"

  //
  // PCIE Root Port #20
  //
  Scope(\_SB.PC00.RP20) {
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRK, LTRN)
      Store (PMLK, LMSL)
      Store (PNLK, LNSL)
      Store (OBFK, OBFN)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  } // end "PCIE Root Port #20"

  //
  // Serial ATA Host Controller - Device 31, Function 2
  //
External(\_SB.PC00.SAT0.SDSM, MethodObj)

Scope (\_SB.PC00.SAT1) {
  Include ("PchSata.asl")
  Device(PRT6)
  {
    Name(_ADR,0x0006FFFF)  // Port 6
  }
  Device(PRT7)
  {
    Name(_ADR,0x0007FFFF)  // Port 7
  }
}
Scope (\_SB.PC00.SAT2) {
  Include ("PchSata.asl")
}
  //Server does not support CIO Camera I/O

  //
  // Thermal Device
  //
  Scope(\_SB.PC00.TERM) {
    Name (_HID, "INT343D")
    Name (_UID, 1)
      Name (RBUF, ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0xFE03C000, 0x00001000, BAR0)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , IRQ) { 18 }
      })

      CreateDWordField(RBUF,IRQ._INT,IRQN)
    Method (_CRS, 0x0, NotSerialized) {
      Store(TIRQ, IRQN)

      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If(LEqual(TAEN, 0))   { Return(0x0) } // device not enabled in ACPI mode
      If(LEqual(TIRQ, 0))   { Return(0x0) } // IRQ number not updated
      Return(0xF)
    }
  }
}

// Comment out as ifdefs don't work at Trim stage of ASL preparation

//
// Storage and Communication Subsystems definitions
//
Include ("PchScs.asl")



