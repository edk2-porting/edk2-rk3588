/** @file
  This file contains the CPU PCIe Root Port configuration

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
External(LTRX) // CPU PCIe Root Port 0 Latency Tolerance Reporting Enable
External(LTRY) // CPU PCIe Root Port 1 Latency Tolerance Reporting Enable
External(LTRZ) // CPU PCIe Root Port 2 Latency Tolerance Reporting Enable
External(LTRW) // CPU PCIe Root Port 3 Latency Tolerance Reporting Enable
External(SMSL) // CPU PCIe Root Port Latency Tolerance Reporting Max Snoop Latency
External(SNSL) // CPU PCIe Root Port Latency Tolerance Reporting Max No Snoop Latency
External(PG0E) // CpuPcieRp0Enable <b>0: Disable</b>; 1: Enable
External(PG1E) // CpuPcieRp1Enable <b>0: Disable</b>; 1: Enable
External(PG2E) // CpuPcieRp2Enable <b>0: Disable</b>; 1: Enable
External(PG3E) // CpuPcieRp3Enable <b>0: Disable</b>; 1: Enable
External(\_SB.PC00.PEG0, DeviceObj)
External(\_SB.PC00.PEG1, DeviceObj)
External(\_SB.PC00.PEG2, DeviceObj)
External(\_SB.PC00.PEG3, DeviceObj)
External(\_SB.PC00.PEG0.PEGP, DeviceObj)
External(\_SB.PC00.PEG1.PEGP, DeviceObj)
External(\_SB.PC00.PEG2.PEGP, DeviceObj)
External(\_SB.PC00.PEG3.PEGP, DeviceObj)
External(\AR02)
External(\PD02)
External(\AR0A)
External(\PD0A)
External(\AR0B)
External(\PD0B)
External(\AR0C)
External(\PD0C)
External(VMDE)
External(VMCP)
External(MPGN)
External(PBR1)
External(PBR2)
External(PBR3)

Scope (\_SB.PC00.PEG0) {

  Name(SLOT, 0) // CPU PCIe root port index 0 corresponds to PEG60 (0/6/0)

  Method (_STA, 0x0, NotSerialized) {
    if(PG0E == 1) { // If CPU PCIe RP0 enabled?
      Return(0x0F)
    }
    Return(0x00)
  }

  Name(LTEN, 0)
  Name(LMSL, 0)
  Name(LNSL, 0)

  Method(_INI)
  {
    Store (LTRX, LTEN)
    Store (SMSL, LMSL)
    Store (SNSL, LNSL)
    If(LAnd(CondRefOf(VMCP),CondRefOf(VMDE))) {
      If(LAnd(LEqual(VMDE,1),LNotEqual(And(VMCP,0x8),0))) {
        Store (1, CPMV)
      }
    }
  }

  Method(_PRT,0) {
    If(PICM) {
      Return(AR02)
    } // APIC mode
    Return (PD02) // PIC Mode
  } // end _PRT

  Include("CpuPcieRpCommon.asl")
} // PEG0 scope end

Scope (\_SB.PC00.PEG1) {

  Name(SLOT, 1) // CPU PCIe root port index 1 corresponds to PEG10 (0/1/0)

  Method (_STA, 0x0, NotSerialized) {
    if(PG1E == 1) { // If CPU PCIe RP1 enabled?
      Return(0x0F)
    }
    Return(0x00)
  }

  Name(LTEN, 0)
  Name(LMSL, 0)
  Name(LNSL, 0)

  Method(_INI)
  {
    Store (LTRY, LTEN)
    Store (SMSL, LMSL)
    Store (SNSL, LNSL)
    If(LAnd(CondRefOf(VMCP),CondRefOf(VMDE))) {
      If(LAnd(LEqual(VMDE,1),LNotEqual(And(VMCP,0x1),0))) {
        Store (1, CPMV)
      }
    }
  }

  Method(_PRT,0) {
    If(PICM) {
      Return(AR0A)
    } // APIC mode
    Return (PD0A) // PIC Mode
  } // end _PRT

  Include("CpuPcieRpCommon.asl")
} // PEG1 scope end

Scope (\_SB.PC00.PEG2) {

  Name(SLOT, 2) // CPU PCIe root port index 2 corresponds to PEG11 (0/1/1)

  Method (_STA, 0x0, NotSerialized) {
    if(PG2E == 1) { // If CPU PCIe RP2 enabled?
      Return(0x0F)
    }
    Return(0x00)
  }

  Name(LTEN, 0)
  Name(LMSL, 0)
  Name(LNSL, 0)

  Method(_INI)
  {
    Store (LTRZ, LTEN)
    Store (SMSL, LMSL)
    Store (SNSL, LNSL)
    If(LAnd(CondRefOf(VMCP),CondRefOf(VMDE))) {
      If(LAnd(LEqual(VMDE,1),LNotEqual(And(VMCP,0x2),0))) {
        Store (1, CPMV)
      }
    }
  }

  Method(_PRT,0) {
    If(PICM) {
      Return(AR0B)
    } // APIC mode
    Return (PD0B) // PIC Mode
  } // end _PRT

  Include("CpuPcieRpCommon.asl")
} // PEG2 scope end

If (CondRefOf(\_SB.PC00.PEG3)) {
  Scope (\_SB.PC00.PEG3) {

    Name(SLOT, 3) // CPU PCIe root port index 3 corresponds to PEG12 (0/1/2)

    Method (_STA, 0x0, NotSerialized) {
      if(PG3E == 1) { // If CPU PCIe RP3 enabled?
        Return(0x0F)
      }
      Return(0x00)
    }

    Name(LTEN, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)

    Method(_INI)
    {
      Store (LTRW, LTEN)
      Store (SMSL, LMSL)
      Store (SNSL, LNSL)
      If(LAnd(CondRefOf(VMCP),CondRefOf(VMDE))) {
        If(LAnd(LEqual(VMDE,1),LNotEqual(And(VMCP,0x4),0))) {
          Store (1, CPMV)
        }
      }
    }

    Method(_PRT,0) {
      If(PICM) {
        Return(AR0C)
      } // APIC mode
      Return (PD0C) // PIC Mode
    } // end _PRT

    Include("CpuPcieRpCommon.asl")
  } // PEG3 scope end
}

Scope(\_SB.PC00.PEG0.PEGP) {
  Method(_PRW, 0) {
    Return(GPRW(0x69, 4)) // can wakeup from S4 state
  }
}


If (PBR1) {
  Scope(\_SB.PC00.PEG1.PEGP) {
    Method(_S0W, 0) { Return(4)} //D3cold is supported

    Device (PEGD) {
      Method(_S0W, 0) { Return(4)} //D3cold is supported
      Name(_ADR, 0x00000000)
      Method(_PRW, 0) { Return(GPRW(0x69, 4)) } // can wakeup from S4 state
    }
  } // end "P.E.G. Port Slot x16"
}

Scope(\_SB.PC00.PEG1.PEGP) {
  Method(_PRW, 0) {
    Return(GPRW(0x69, 4)) // can wakeup from S4 state
  }
}


If (PBR2) {
  Scope(\_SB.PC00.PEG2.PEGP) {
    Method(_S0W, 0) { Return(4)} //D3cold is supported

    Device (PEGD) {
      Method(_S0W, 0) { Return(4)} //D3cold is supported
      Name(_ADR, 0x00000000)
      Method(_PRW, 0) { Return(GPRW(0x69, 4)) } // can wakeup from S4 state
    }
  } // end "P.E.G. Port Slot 2x8"
}

Scope(\_SB.PC00.PEG2.PEGP) {
  Method(_PRW, 0) {
    Return(GPRW(0x69, 4)) // can wakeup from S4 state
  }
}

If (PBR3) {
  Scope(\_SB.PC00.PEG3.PEGP) {
    Method(_S0W, 0) { Return(4)} //D3cold is supported

    Device (PEGD) {
      Method(_S0W, 0) { Return(4)} //D3cold is supported
      Name(_ADR, 0x00000000)
      Method(_PRW, 0) { Return(GPRW(0x69, 4)) } // can wakeup from S4 state
    }
  } // end "P.E.G. Port Slot 1x8 - 2x4"
}

If (CondRefOf(\_SB.PC00.PEG3)) {
  Scope(\_SB.PC00.PEG3.PEGP) {
    Method(_PRW, 0) {
      Return(GPRW(0x69, 4)) // can wakeup from S4 state
    }
  }
}
