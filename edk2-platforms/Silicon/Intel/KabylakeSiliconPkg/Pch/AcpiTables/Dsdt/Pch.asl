/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PchAccess.h>

Name (PNVB, 0xFFFF0000)  // PCH NVS Base address
Name (PNVL, 0xAA55)      // PCH NVS Length
Include ("PchNvs.asl")

//
// Include GPIO macros and access library
//
include("GpioAcpiDefines.h")
include("GpioLib.asl")

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
    \_SB.PCI0.XHC.GPEH()
    \_SB.PCI0.HDAS.GPEH()
    \_SB.PCI0.GLAN.GPEH()
    \_SB.PCI0.XDCI.GPEH()
  }
}

Scope (\_SB.PCI0) {

  Name(TEMP,0)                          // scratch variable that does not cause compiler warnings as writes to Localx variables do.

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
        Memory32Fixed(ReadWrite,0xFE000000,0x00020000) // 0xFE000000 - 0xFE01FFFF
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
    Method(_CRS) {
      Store(ResourceTemplate() { }, Local0)
      Name(BUF0,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG0) })
      Name(BUF1,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG1) })
      Name(BUF2,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG2) })
      Name(BUF3,ResourceTemplate(){ Io(Decode16,0x0,0x0,0x1,0xFF,TAG3) })
      CreateWordField(BUF0,TAG0._MIN,AMI0);CreateWordField(BUF0,TAG0._MAX,AMA0)
      CreateWordField(BUF1,TAG1._MIN,AMI1);CreateWordField(BUF1,TAG1._MAX,AMA1)
      CreateWordField(BUF2,TAG2._MIN,AMI2);CreateWordField(BUF2,TAG2._MAX,AMA2)
      CreateWordField(BUF3,TAG3._MIN,AMI3);CreateWordField(BUF3,TAG3._MAX,AMA3)
      Store(ITA0,AMI0);Store(ITA0,AMA0)
      Store(ITA1,AMI1);Store(ITA1,AMA1)
      Store(ITA2,AMI2);Store(ITA2,AMA2)
      Store(ITA3,AMI3);Store(ITA3,AMA3)
      // Win7 can't store result of ConcatenateResTemplate directly into one of its input parameters
      if(LEqual(ITS0,1)) { ConcatenateResTemplate(Local0, BUF0, Local1); Store(Local1, Local0) }
      if(LEqual(ITS1,1)) { ConcatenateResTemplate(Local0, BUF1, Local1); Store(Local1, Local0) }
      if(LEqual(ITS2,1)) { ConcatenateResTemplate(Local0, BUF2, Local1); Store(Local1, Local0) }
      if(LEqual(ITS3,1)) { ConcatenateResTemplate(Local0, BUF3, Local1); Store(Local1, Local0) }
      return (Local0)
    }
  }


  //
  // LPC Bridge - Device 31, Function 0, this is only for PCH register Memory Region declare,
  // it's better to be declared as early as possible since it's widely used in whole ACPI name space.
  // Please add any code which needs to reference any register of it after this
  //
  Device(LPCB) {
    Name(_ADR, 0x001F0000)
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}

    OperationRegion(LPC, PCI_Config, 0x00, 0x100)
    Field(LPC, AnyAcc, NoLock, Preserve)
    {
      Offset(0x02),
      CDID, 16,
      Offset(0x08),
      CRID,  8,
      Offset(R_PCH_LPC_IOD), // 0x80
      IOD0,  8,
      IOD1,  8,
      Offset(0xA0),
      , 9,
      PRBL,  1,
      Offset(R_PCH_LPC_BC), // 0xDC
      ,       2,
      ESPI,   1,
    }
  }

  //
  // PCH Power Management Controller
  //
  Device(PPMC) {
    Name(_ADR, 0x001F0002)
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
  }

  //
  // SMBus Controller - Device 31, Function 4
  //
  Device(SBUS) {
    Name(_ADR,0x001F0004)
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
  }
}

scope(\_SB) {
  Include ("IrqLink.asl")
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
    Store (PCRR (PID_LPC, R_PCH_PCR_RTC_RTCDCG), local0)
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

  Name (TCBV, 0)        // TCO I/O base address value
  Method (TCBS, 0) {
    If (LEqual(TCBV, 0)) {
      // PCR[DMI] + 2778h[15:5]
      Store(PCRR (PID_DMI, R_PCH_PCR_DMI_TCOBASE),Local0)
      And(Local0,B_PCH_PCR_DMI_TCOBASE_TCOBA,TCBV)
    }
    Return (TCBV)
  }

  //
  // Define PCH ACPIBASE I/O as an ACPI operating region.  The base address
  // can be found in Device 31, Function 2, Offset 40h.
  //
  OperationRegion(PMIO, SystemIo, PMBS, 0x60)
  Field(PMIO, ByteAcc, NoLock, Preserve) {
          ,  8,
      PBSS,  1,       // Power Button Status
    Offset(R_PCH_ACPI_GPE_CNTL),     // 0x40, General Purpose Event Control
          ,  17,
      GPEC,  1        // Software GPE Control
  }

  //
  // Define PCH TCOBASE I/O
  //
  OperationRegion(TCBA, SystemIo, TCBS, 0x10)
  Field(TCBA, ByteAcc, NoLock, Preserve) {
    Offset(R_PCH_TCO1_STS), // 0x04
        , 9,
    CPSC, 1,        // CPUSCI_STS
  }

  //
  // PWRM register definitions
  //
  OperationRegion(PWMR, SystemMemory, \PWRM, 0x800)
  Field(PWMR, AnyAcc, NoLock, Preserve) {
    Offset(R_PCH_PWRM_CFG3), // 0xE0
        , 16,
    DWLE, 1,          // Deep-Sx WLAN Phy Power Enable
    HWLE, 1,          // Host Wireless LAN Phy Power Enable
    Offset(R_PCH_PWRM_31C), // 0x31C
        , 13,
    SLS0, 1,          // SLP_S0# Low Voltage Mode Enable (SLPS0LVEN)
        , 8,
    XSQD, 1,          // 24MHz Crystal Shutdown Qualification Disable (XTALSDQDIS)
  }

  //
  //
  OperationRegion(PMST, SystemMemory, PWRM, 0x80)
  Field(PMST, DWordAcc, NoLock, Preserve) {
    Offset(R_PCH_PWRM_CFG),  // 0x18, Power Management Configuration Reg 1 (PM_CFG)
        , 25,      //
    USBP,  1,      // Allow USB2 PHY Core Power Gating (ALLOW_USB2_CORE_PG)
    Offset(R_PCH_PWRM_PCH_PM_STS),  // 0x1C, PCH Power Management Status (PCH_PM_STS)
        , 24,      //
    PMFS,  1,      // PMC Message Full Status (PMC_MSG_FULL_STS)
    Offset(R_PCH_PWRM_MTPMC),  // 0x20, Message to PMC (MTPMC)
    MPMC, 32,      // Message to PMC (MTPMC)
    Offset(R_PCH_PWRM_PCH_PM_STS2),  // 0x24, PCH Power Management Status (PCH_PM_STS2)
        , 20,      //
    UWAB,  1,      // USB2 Workaround Available Bit
  }

} //end Scope(\)

Scope (\_SB.PCI0) {
  Name(LTRZ, 0)
  Name(OBFZ, 0)

  Name(LMSL, 0)
  Name(LNSL, 0)

  //
  // LAN Controller - Device 31, Function 6
  //
  Device(GLAN) { // GbE Controller
    Name(_ADR, 0x001F0006)
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    OperationRegion(GLBA, PCI_Config, 0,0x100)
    Field(GLBA,AnyAcc,NoLock,Preserve)
    {
      DVID, 16
    }
    //
    // Byte access for PMCS field to avoid race condition on device D-state
    //
    Field(GLBA,ByteAcc,NoLock,Preserve)
    {
      Offset(R_PCH_LAN_PMCS), // 0xCC
          ,  8,
      PMEE,  1, // PME Enable
          ,  6,
      PMES,  1  // PME Status
    }

    Method(_PRW, 0) { Return(GPRW(0x6D, 4)) }  // can wakeup from S4 state

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
        Notify(GLAN, 0x02)
      }
    }
  } // end "GbE Controller"

} //scope

//
// xHCI Controller - Device 20, Function 0
//
Include("PchXhci.asl")

//
// xDCI (OTG) Controller - Device 20, Function 1
//
include("PchXdci.asl")

Scope(\_SB_.PCI0) {
  //
  // High Definition Audio Controller - Device 31, Function 3
  //
  include("PchHda.asl")

  //
  // PCIE Root Port #01
  //
  Device(RP01) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA1,0)) {
        Return (RPA1)
      } Else {
        Return (0x001C0000)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR1, LTRZ)
      Store (PML1, LMSL)
      Store (PNL1, LNSL)
      Store (OBF1, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #01"

  //
  // PCIE Root Port #02
  //
  Device(RP02) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA2,0)) {
        Return (RPA2)
      } Else {
        Return (0x001C0001)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR2, LTRZ)
      Store (PML2, LMSL)
      Store (PNL2, LNSL)
      Store (OBF2, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #02"

  //
  // PCIE Root Port #03
  //
  Device(RP03) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA3,0)) {
        Return (RPA3)
      } Else {
        Return (0x001C0002)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR3, LTRZ)
      Store (PML3, LMSL)
      Store (PNL3, LNSL)
      Store (OBF3, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #03"

  //
  // PCIE Root Port #04
  //
  Device(RP04) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA4,0)) {
        Return (RPA4)
      } Else {
        Return (0x001C0003)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR4, LTRZ)
      Store (PML4, LMSL)
      Store (PNL4, LNSL)
      Store (OBF4, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #04"

  //
  // PCIE Root Port #05
  //
  Device(RP05) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA5,0)) {
        Return (RPA5)
      } Else {
        Return (0x001C0004)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR5, LTRZ)
      Store (PML5, LMSL)
      Store (PNL5, LNSL)
      Store (OBF5, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #05"

  //
  // PCIE Root Port #06
  //
  Device(RP06) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA6,0)) {
        Return (RPA6)
      } Else {
        Return (0x001C0005)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR6, LTRZ)
      Store (PML6, LMSL)
      Store (PNL6, LNSL)
      Store (OBF6, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #06"

  //
  // PCIE Root Port #07
  //
  Device(RP07) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA7,0)) {
        Return (RPA7)
      } Else {
        Return (0x001C0006)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR7, LTRZ)
      Store (PML7, LMSL)
      Store (PNL7, LNSL)
      Store (OBF7, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR06) } // APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #07"

  //
  // PCIE Root Port #08
  //
  Device(RP08) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA8,0)) {
        Return (RPA8)
      } Else {
        Return (0x001C0007)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR8, LTRZ)
      Store (PML8, LMSL)
      Store (PNL8, LNSL)
      Store (OBF8, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #08"

  //
  // PCIE Root Port #09
  //
  Device(RP09) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA9,0)) {
        Return (RPA9)
      } Else {
        Return (0x001D0000)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR9, LTRZ)
      Store (PML9, LMSL)
      Store (PNL9, LNSL)
      Store (OBF9, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #09"

  //
  // PCIE Root Port #10
  //
  Device(RP10) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAA,0)) {
        Return (RPAA)
      } Else {
        Return (0x001D0001)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRA, LTRZ)
      Store (PMLA, LMSL)
      Store (PNLA, LNSL)
      Store (OBFA, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #10"

  //
  // PCIE Root Port #11
  //
  Device(RP11) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAB,0)) {
        Return (RPAB)
      } Else {
        Return (0x001D0002)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRB, LTRZ)
      Store (PMLB, LMSL)
      Store (PNLB, LNSL)
      Store (OBFB, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #11"

  //
  // PCIE Root Port #12
  //
  Device(RP12) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAC,0)) {
        Return (RPAC)
      } Else {
        Return (0x001D0003)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRC, LTRZ)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
      Store (OBFC, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #12"

  //
  // PCIE Root Port #13
  //
  Device(RP13) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAD,0)) {
        Return (RPAD)
      } Else {
        Return (0x001D0004)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRD, LTRZ)
      Store (PMLD, LMSL)
      Store (PNLD, LNSL)
      Store (OBFD, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #13"

  //
  // PCIE Root Port #14
  //
  Device(RP14) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAE,0)) {
        Return (RPAE)
      } Else {
        Return (0x001D0005)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRE, LTRZ)
      Store (PMLE, LMSL)
      Store (PNLE, LNSL)
      Store (OBFE, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #14"

  //
  // PCIE Root Port #15
  //
  Device(RP15) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAF,0)) {
        Return (RPAF)
      } Else {
        Return (0x001D0006)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRF, LTRZ)
      Store (PMLF, LMSL)
      Store (PNLF, LNSL)
      Store (OBFF, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #15"

  //
  // PCIE Root Port #16
  //
  Device(RP16) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAG,0)) {
        Return (RPAG)
      } Else {
        Return (0x001D0007)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRG, LTRZ)
      Store (PMLG, LMSL)
      Store (PNLG, LNSL)
      Store (OBFG, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #16"

  //
  // PCIE Root Port #17
  //
  Device(RP17) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAH,0)) {
        Return (RPAH)
      } Else {
        Return (0x001B0000)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRH, LTRZ)
      Store (PMLH, LMSL)
      Store (PNLH, LNSL)
      Store (OBFH, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #17"

  //
  // PCIE Root Port #18
  //
  Device(RP18) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAI,0)) {
        Return (RPAI)
      } Else {
        Return (0x001B0001)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRI, LTRZ)
      Store (PMLI, LMSL)
      Store (PNLI, LNSL)
      Store (OBFI, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #18"

  //
  // PCIE Root Port #19
  //
  Device(RP19) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAJ,0)) {
        Return (RPAJ)
      } Else {
        Return (0x001B0002)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRJ, LTRZ)
      Store (PMLJ, LMSL)
      Store (PNLJ, LNSL)
      Store (OBFJ, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #19"

  //
  // PCIE Root Port #20
  //
  Device(RP20) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAK,0)) {
        Return (RPAK)
      } Else {
        Return (0x001B0003)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRK, LTRZ)
      Store (PMLK, LMSL)
      Store (PNLK, LNSL)
      Store (OBFK, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #20"

  //
  // PCIE Root Port #21
  //
  Device(RP21) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAL,0)) {
        Return (RPAL)
      } Else {
        Return (0x001B0004)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRL, LTRZ)
      Store (PMLL, LMSL)
      Store (PNLL, LNSL)
      Store (OBFL, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #21"

  //
  // PCIE Root Port #21
  //
  Device(RP22) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAM,0)) {
        Return (RPAM)
      } Else {
        Return (0x001B0005)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRM, LTRZ)
      Store (PMLM, LMSL)
      Store (PNLM, LNSL)
      Store (OBFM, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #22"

  //
  // PCIE Root Port #22
  //
  Device(RP23) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAN,0)) {
        Return (RPAN)
      } Else {
        Return (0x001B0006)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRN, LTRZ)
      Store (PMLN, LMSL)
      Store (PNLN, LNSL)
      Store (OBFN, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #23"

  //
  // PCIE Root Port #21
  //
  Device(RP24) {
    Method (_ADR, 0) {
      If (LNotEqual(RPAO,0)) {
        Return (RPAO)
      } Else {
        Return (0x001B0007)
      }
    }
    //
    // Pass LTRx to LTRZ so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTRO, LTRZ)
      Store (PMLO, LMSL)
      Store (PNLO, LNSL)
      Store (OBFO, OBFZ)
    }
    Include("PchPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x69, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #24"

  //
  // Serial ATA Host Controller - Device 31, Function 2
  //
  Include ("PchSata.asl")

  //
  // CIO2 ACPI device - Device 20, Function 3
  //
  Device (CIO2) {
    Method (_STA) {
      If (LEqual(CIOE,1)){ Return (0x0F) }
      Else { Return (0x00) }
    }
    Name(_HID, "INT343E")
    Method(_CRS, 0, NotSerialized) {
      Name(CBUF, ResourceTemplate () {
        Interrupt(ResourceConsumer, Level, ActiveLow, Shared,,,CIOD) {0x10}
        Memory32Fixed (ReadWrite, PCH_CIO2_BASE_ADDRESS, PCH_CIO2_MMIO_SIZE)
      })
      CreateDWordField(CBUF,CIOD._INT,CIOV)
      Store(CIOI, CIOV)
      Return (CBUF)
    }
  }

  //
  // Thermal Device
  //
  Device(TERM) {
    Name (_HID, "INT343D")
    Name (_UID, 1)

    Method (_CRS, 0x0, NotSerialized) {
      Name (RBUF, ResourceTemplate () {
        Memory32Fixed (ReadWrite, PCH_THERMAL_BASE_ADDRESS, PCH_THERMAL_MMIO_SIZE, BAR0)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , IRQ) { 18 }
      })

      CreateDWordField(RBUF,IRQ._INT,IRQN)

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


//
//  Serial IO Controllers definitions
//
#define SERIAL_SCOPE \_SB.PCI0
Include ("PchSerialIo.asl")

//
// Storage and Communication Subsystems definitions
//
Include ("PchScs.asl")

//
// Integrated Sensor Hub definition
//
Include ("PchIsh.asl")

//
// MEI 1 definition
//
Include ("PchHeci.asl")

