/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#define ROOTPORT_READ           0
#define ROOTPORT_WRITE          1
#define ENDPOINT_READ           2
#define ENDPOINT_WRITE          3
#define GPIO_NO_CONTROL 0
#define GPIO_INTERNAL 1
#define GPIO_EXTERNAL 2
#define EXPANDER0 0
#define EXPENDER1 1
#define INDEX_CONTROL 0
#define INDEX_EXPANDER_NR 1
#define INDEX_PIN_NR 2
#define INDEX_POLARITY 3


External(\_SB.PCI0.SAT0.NVM1.RAID, MethodObj)
External(\_SB.PCI0.SAT0.NVM1.IR3E, MethodObj)
External(\_SB.PCI0.SAT0.NVM1.VLPM)
External(\_SB.PCI0.SAT0.NVM2.RAID, MethodObj)
External(\_SB.PCI0.SAT0.NVM2.IR3E, MethodObj)
External(\_SB.PCI0.SAT0.NVM2.VLPM)
External(\_SB.PCI0.SAT0.NVM3.RAID, MethodObj)
External(\_SB.PCI0.SAT0.NVM3.IR3E, MethodObj)
External(\_SB.PCI0.SAT0.NVM3.VLPM)


//
// SDSM is Device Specific Method supporting AHCI DEVSLP
// It is not guaranteed to be available on every boot
//
External(\_SB.PCI0.SAT0.SDSM, MethodObj)
External(\_SB.PCI0.RP05.PWRG)
External(\_SB.PCI0.RP05.RSTG)
External(\_SB.PCI0.RP05.SCLK)
External(\_SB.PCI0.RP09.PWRG)
External(\_SB.PCI0.RP09.RSTG)
External(\_SB.PCI0.RP09.SCLK)
External(\_SB.PCI0.RP13.PWRG)
External(\_SB.PCI0.RP13.RSTG)
External(\_SB.PCI0.RP13.SCLK)
External(\_SB.PCI0.RP17.PWRG)
External(\_SB.PCI0.RP17.RSTG)
External(\_SB.PCI0.RP17.SCLK)

  // PCH SATA Controller
  Device (SAT0)
  {
    //Bus 0x00, Device 0x17, Function 0x00
    Name(_ADR, 0x00170000)

    Method(_DSM,4,serialized){
      if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) };
      if(CondRefOf(\_SB.PCI0.SAT0.SDSM)) { return (\_SB.PCI0.SAT0.SDSM(Arg0,Arg1,Arg2,Arg3)) };
      Return(Buffer() {0})
    }

    Device(PRT0)
    {
      Name(_ADR,0x0000FFFF)  // Port 0
    }

    Device(PRT1)
    {
      Name(_ADR,0x0001FFFF)  // Port 1
    }

    Device(PRT2)
    {
      Name(_ADR,0x0002FFFF)  // Port 2
    }

    Device(PRT3)
    {
      Name(_ADR,0x0003FFFF)  // Port 3
    }

    Device(PRT4)
    {
      Name(_ADR,0x0004FFFF)  // Port 4
    }

    Device(PRT5)
    {
      Name(_ADR,0x0005FFFF)  // Port 5
    }
    Device(VOL0)
    {
      Name(_ADR,0x0080FFFF)  // RST Raid Volume 0 or SRT Cache
    }
    Device(VOL1)
    {
      Name(_ADR,0x0081FFFF)  // RST Raid Volume 1
    }
    Device(VOL2)
    {
      Name(_ADR,0x0082FFFF)  // RST Raid Volume 2
    }
    // Asign Root Port Configuration for remapped drive.
    // 4 arguments: NRPN, PWRG, RSTG, SCLK
    Method(ARPC, 4, Serialized,,, {FieldUnitObj, PkgObj, PkgObj, PkgObj})
    {
      ADBG(Concatenate("NRPN: ", ToHexString(Arg0)))
      Switch(ToInteger(Arg0))
      {
        Case(4)
        {
          if(condrefof(\_SB.PCI0.RP05.PWRG)) {CopyObject(\_SB.PCI0.RP05.PWRG,Arg1)}
          if(condrefof(\_SB.PCI0.RP05.RSTG)) {CopyObject(\_SB.PCI0.RP05.RSTG,Arg2)}
          if(condrefof(\_SB.PCI0.RP05.SCLK)) {CopyObject(\_SB.PCI0.RP05.SCLK,Arg3)}
        }
        Case(8)
        {
          if(condrefof(\_SB.PCI0.RP09.PWRG)) {CopyObject(\_SB.PCI0.RP09.PWRG,Arg1)}
          if(condrefof(\_SB.PCI0.RP09.RSTG)) {CopyObject(\_SB.PCI0.RP09.RSTG,Arg2)}
          if(condrefof(\_SB.PCI0.RP09.SCLK)) {CopyObject(\_SB.PCI0.RP09.SCLK,Arg3)}

        }
        Case(12)
        {
          if(condrefof(\_SB.PCI0.RP13.PWRG)) {CopyObject(\_SB.PCI0.RP13.PWRG,Arg1)}
          if(condrefof(\_SB.PCI0.RP13.RSTG)) {CopyObject(\_SB.PCI0.RP13.RSTG,Arg2)}
          if(condrefof(\_SB.PCI0.RP13.SCLK)) {CopyObject(\_SB.PCI0.RP13.SCLK,Arg3)}
        }
        Case(16)
        {
          if(condrefof(\_SB.PCI0.RP17.PWRG)) {CopyObject(\_SB.PCI0.RP17.PWRG,Arg1)}
          if(condrefof(\_SB.PCI0.RP17.RSTG)) {CopyObject(\_SB.PCI0.RP17.RSTG,Arg2)}
          if(condrefof(\_SB.PCI0.RP17.SCLK)) {CopyObject(\_SB.PCI0.RP17.SCLK,Arg3)}
        }
        Default
        {
            ADBG(Concatenate("ERR!NRPN: ", ToHexString(Arg0)))
        }
      }
    }




  }
