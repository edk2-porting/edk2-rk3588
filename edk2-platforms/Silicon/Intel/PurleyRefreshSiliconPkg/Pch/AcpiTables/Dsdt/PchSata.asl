/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define ROOTPORT_READ           0
#define ROOTPORT_WRITE          1
#define ENDPOINT_READ           2
#define ENDPOINT_WRITE          3

//
// SDSM is Device Specific Method supporting AHCI DEVSLP
// It is not guaranteed to be available on every boot
//
// move one level up to Pch.asl

    Method(_DSM,4,serialized){
      if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) };
      if(CondRefOf(\_SB.PC00.SAT0.SDSM)) { return (\_SB.PC00.SAT0.SDSM(Arg0,Arg1,Arg2,Arg3)) };
      return(0)
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

    //
    // Method to perform RST PCIe Storage Remapping read or write access to the remapped device / hidden root port configuration space
    // This method takes 5 parameters as below:
    // Arg0 - RST PCIe Storage Cycle Router#
    // Arg1 - PCI Offset
    // Arg2 - WriteData (AndMask)
    // Arg3 - WriteData (OrMask)
    // Arg4 - Access Methods: ROOTPORT_READ, ROOTPORT_WRITE, ENDPOINT_READ, ENDPOINT_WRITE
    //
    Method(RDCA, 5, Serialized)
    {
      //
      // Operation Region for Sata Extended Config Space for Hidden Root Port Access
      //
      OperationRegion(RPAL, SystemMemory, Add(\_SB.PC00.GPCB(), Add(0xB8100,Arg1)), 0x4)
      Field(RPAL,DWordAcc,Lock,Preserve)
      {
        RPCD, 32,
      }

      //
      // Operation Region for Endpoint Index-Data Pair for Remapped Device Access
      //
      OperationRegion(EPAC, SystemMemory, Add(\_SB.PC00.GPCB(),0xB8308), 0x8)
      Field(EPAC,DWordAcc,Lock,Preserve)
      {
        CAIR, 32,
        CADR, 32,
      }

      //
      // Operation Region for RST PCIe Storage Cycle Router Global configuration registers
      //
      OperationRegion(NCRG, SystemMemory, Add(\_SB.PC00.GPCB(),0xB8FC0), 0x4)
      Field(NCRG,DWordAcc,Lock,Preserve)
      {
        CRGC, 32,
      }

      If (LGreater(Arg0, 2))     // Return if RST PCIe Storage Cycle Router# is invalid
      {
        Return(0)
      }
      Else                       // Set RST PCIe Storage Cycle Router Accessibility based on Arg0 - RST PCIe Storage Cycle Router#
      {
        Store(Arg0,CRGC)
      }

      Switch(ToInteger(Arg4))
      {
        Case(ROOTPORT_READ)     // Read access to the Hidden Root Port
        {
          Return(RPCD)
        }
        Case(ENDPOINT_READ)     // Read access to the Remapped Device
        {
          Store(Arg1,CAIR)
          Return(CADR)
        }
        Case(ROOTPORT_WRITE)     // Write access to the Hidden Root Port
        {
          And(Arg2,RPCD,Local0)
          Or(Arg3,Local0,Local0)
          Store(Local0,RPCD)
        }
        Case(ENDPOINT_WRITE)     // Write access to the Remapped Device
        {
          Store(Arg1,CAIR)
          And(Arg2,CADR,Local0)
          Or(Arg3,Local0,Local0)
          Store(Local0,CADR)
        }
        Default
        {
          Return(0)
        }
      }
      //Fix warning: not all control paths return a value
      Return(0)
    }

    Method(RPD0, 1, Serialized) // Put Hidden Root Port into D0 state
    {
      RDCA(Arg0,0xA4,0xFFFFFFFC,0x0,ROOTPORT_WRITE)
    }

    Method(RPD3, 1, Serialized) // Put Hidden Root Port into D3 state
    {
      RDCA(Arg0,0xA4,0xFFFFFFFC,0x3,ROOTPORT_WRITE)
    }

    Device(NVM1)
    {
      Name(_ADR,0x00C1FFFF)

      Include("PchRstPcieStorage.asl")
      Method(_INI)
      {
        Store (NIT1, NITV)
        Store (NPM1, NPMV)
        Store (NPC1, NPCV)
        Store (NL11, NL1V)
        Store (ND21, ND2V)
        Store (ND11, ND1V)
        Store (NLR1, NLRV)
        Store (NLD1, NLDV)
        Store (NEA1, NEAV)
        Store (NEB1, NEBV)
        Store (NEC1, NECV)
        Store (NRA1, NRAV)
        Store (NMB1, NMBV)
        Store (NMV1, NMVV)
        Store (NPB1, NPBV)
        Store (NPV1, NPVV)
        Store (0, NCRN)
      }
    }

    Device(NVM2)
    {
      Name(_ADR,0x00C2FFFF)

      Include("PchRstPcieStorage.asl")
      Method(_INI)
      {
        Store (NIT2, NITV)
        Store (NPM2, NPMV)
        Store (NPC2, NPCV)
        Store (NL12, NL1V)
        Store (ND22, ND2V)
        Store (ND12, ND1V)
        Store (NLR2, NLRV)
        Store (NLD2, NLDV)
        Store (NEA2, NEAV)
        Store (NEB2, NEBV)
        Store (NEC2, NECV)
        Store (NRA2, NRAV)
        Store (NMB2, NMBV)
        Store (NMV2, NMVV)
        Store (NPB2, NPBV)
        Store (NPV2, NPVV)
        Store (1, NCRN)
      }
    }

    Device(NVM3)
    {
      Name(_ADR,0x00C3FFFF)

      Include("PchRstPcieStorage.asl")
      Method(_INI)
      {
        Store (NIT3, NITV)
        Store (NPM3, NPMV)
        Store (NPC3, NPCV)
        Store (NL13, NL1V)
        Store (ND23, ND2V)
        Store (ND13, ND1V)
        Store (NLR3, NLRV)
        Store (NLD3, NLDV)
        Store (NEA3, NEAV)
        Store (NEB3, NEBV)
        Store (NEC3, NECV)
        Store (NRA3, NRAV)
        Store (NMB3, NMBV)
        Store (NMV3, NMVV)
        Store (NPB3, NPBV)
        Store (NPV3, NPVV)
        Store (2, NCRN)
      }
    }

