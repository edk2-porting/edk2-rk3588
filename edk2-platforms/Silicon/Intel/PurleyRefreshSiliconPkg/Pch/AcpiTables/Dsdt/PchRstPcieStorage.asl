/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define PCI_CARD_BASE_ADDR0     0x10
#define PCI_CARD_BASE_ADDR1     0x14
#define PCI_CARD_BASE_ADDR2     0x18
#define PCI_CARD_BASE_ADDR3     0x1C
#define PCI_CARD_BASE_ADDR4     0x20
#define PCI_CARD_BASE_ADDR5     0x24

  //RST Pcie Storage Remapped Base Address Index Value
  Name(PRBI, 0)

  //RST Pcie Storage Remapped Base Address Data Value
  Name(PRBD, 0)

  //RST Pcie Storage Endpoint Command Data
  Name(PCMD, 0)

  //RST Pcie Storage Cycle Router
  Name(NCRN, 0)

  //
  // Variables list to store corresponding value for different NVM device
  //
  Name(NITV, 0)  // Interface Type
  Name(NPMV, 0)  // Power Management Capability Pointer
  Name(NPCV, 0)  // PCIe Capabilities Pointer
  Name(NL1V, 0)  // L1SS Capability Pointer
  Name(ND2V, 0)  // Endpoint L1SS Control Data2
  Name(ND1V, 0)  // Endpoint L1SS Control Data1
  Name(NLRV, 0)  // LTR Capability Pointer
  Name(NLDV, 0)  // Endpoint LTR Data
  Name(NEAV, 0)  // Endpoint LCTL Data
  Name(NEBV, 0)  // Endpoint DCTL Data
  Name(NECV, 0)  // Endpoint DCTL2 Data
  Name(NRAV, 0)  // RootPort DCTL2 Data
  Name(NMBV, 0)  // Endpoint unique MSI-X Table BAR
  Name(NMVV, 0)  // Endpoint unique MSI-X Table BAR value
  Name(NPBV, 0)  // Endpoint unique MSI-X PBA BAR
  Name(NPVV, 0)  // Endpoint unique MSI-X PBA BAR value

  Method(EPD0, 0, Serialized) // Put Remapped Device into D0 state
  {
    RDCA(NCRN,Add(NPMV,0x04),0xFFFFFFFC,0x0,ENDPOINT_WRITE)
  }

  Method(EPD3, 0, Serialized) // Put Remapped Device into D3 state
  {
    RDCA(NCRN,Add(NPMV,0x04),0xFFFFFFFC,0x3,ENDPOINT_WRITE)
  }

  //
  // Restore of Remapped Device and Hidden Root Port
  // This method is called after the endpoint is to be power ungated (D3-cold to D0 unitialized)
  //
  Method(CNRS, 0, Serialized)
  {
    //
    // Return if RST Pcie Storage Remapping is disabled
    //
    If(LEqual(NITV,0))
    {
      Return(0)
    }

    //
    // Clear all BARs in Remapped Device
    //
    RDCA(NCRN,PCI_CARD_BASE_ADDR0,0x0,0x0,ENDPOINT_WRITE)
    RDCA(NCRN,PCI_CARD_BASE_ADDR1,0x0,0x0,ENDPOINT_WRITE)
    RDCA(NCRN,PCI_CARD_BASE_ADDR2,0x0,0x0,ENDPOINT_WRITE)
    RDCA(NCRN,PCI_CARD_BASE_ADDR3,0x0,0x0,ENDPOINT_WRITE)
    RDCA(NCRN,PCI_CARD_BASE_ADDR4,0x0,0x0,ENDPOINT_WRITE)
    RDCA(NCRN,PCI_CARD_BASE_ADDR5,0x0,0x0,ENDPOINT_WRITE)

    //
    // Restore Endpoint CMD and remapped BAR
    //
    RDCA(NCRN,0x4,0xFFFFFFF8,PCMD,ENDPOINT_WRITE)
    RDCA(NCRN,PRBI,0x0,PRBD,ENDPOINT_WRITE)

    //
    // Restore of Remapped Device L1 Substate if this Capability is supported
    //
    If(LNotEqual(NL1V,0))
    {
      RDCA(NCRN,Add(NL1V,0x0C),0xFFFFFF00,ND2V,ENDPOINT_WRITE)
      RDCA(NCRN,Add(NL1V,0x08),0x0000000F,And(ND1V,0xFFFFFFF0),ENDPOINT_WRITE)
      RDCA(NCRN,Add(NL1V,0x08),0xFFFFFFFF,ND1V,ENDPOINT_WRITE)
    }

    //
    // Restore of Remapped Device LTR if this Capability is supported
    //
    If(LNotEqual(NLRV,0))
    {
      RDCA(NCRN,Add(NLRV,0x04),0xFFFFFFFF,NLDV,ENDPOINT_WRITE)
    }

    //
    // Restore of Remapped Device Link Control's "Enable Clock Power Management" field and "Common Clock Configuration" field
    //
    RDCA(NCRN,Add(NPCV,0x10),0xFFFFFEBF,And(NEAV,0xFFFC),ENDPOINT_WRITE)

    //
    // Restore of Remapped Device Device Control 2 field
    //
    RDCA(NCRN,Add(NPCV,0x28),0xFFFFFBFF,NECV,ENDPOINT_WRITE)

    //
    // Restore of Remapped Device Device Control field
    //
    RDCA(NCRN,Add(NPCV,0x8),0xFFFFFF1F,NEBV,ENDPOINT_WRITE)

    //
    // Restore of Hidden Root Port field
    //
    RDCA(NCRN,0x68,0xFFFFFBFF,NRAV,ROOTPORT_WRITE)

    //
    // Check CCC bit
    // If this bit is 1, perform link retrain by setting the "Retrain Link" bit
    //
    If(LEqual(And(NEAV,0x40),0x40))
    {
      RDCA(NCRN,0x50,0xFFFFFFDF,0x20,ROOTPORT_WRITE)
      //
      // Poll PCIe Link Active status till it is active
      //
      while(LEqual(And(RDCA(NCRN,0x52,0x0,0x0,ROOTPORT_READ),0x2000),0))
      {
        Stall(10)
      }
    }

    //
    // Restore of Remapped Device Link Control's "Active State Link PM Control" field
    //
    RDCA(NCRN,Add(NPCV,0x10),0xFFFFFFFC,And(NEAV,0x0003),ENDPOINT_WRITE)

    //
    // Restore of Remapped Device related device BAR for the MSI-X Table BAR if the device supports unique MSI-X Table BAR
    //
    If(LNotEqual(NMVV,0))
    {
      RDCA(NCRN,NMBV,0x0,NMVV,ENDPOINT_WRITE)
    }

    //
    // Restore of Remapped Device related device BAR for the MSI-X PBA BAR if the device supports unique MSI-X PBA BAR
    //
    If(LNotEqual(NPVV,0))
    {
      RDCA(NCRN,NPBV,0x0,NPVV,ENDPOINT_WRITE)
    }
    //Fix warning: not all control paths return a value
    Return(0)
  }

  Method(_PS3,0,Serialized)
  {
    //
    // Return if RST Pcie Storage Remapping is disabled
    //
    If(LEqual(NITV,0))
    {
      //Fix warning: restricted method should not return a value

    }

    //
    // Store Endpoint CMD and remapped BAR for CNRS() restoration
    //
    Store(RDCA(NCRN,0x4,0x0,0x0,ENDPOINT_READ),PCMD)
    If(LEqual(NITV,1)) // Store BAR5 if Endpoint is AHCI Interface
    {
      Store(0x24,PRBI)
      Store(RDCA(NCRN,0x24,0x0,0x0,ENDPOINT_READ),PRBD)
    }
    ElseIf(LEqual(NITV,2)) // Store BAR0 if Endpoint is NVMe Interface
    {
      Store(0x10,PRBI)
      Store(RDCA(NCRN,0x10,0x0,0x0,ENDPOINT_READ),PRBD)
    }

    EPD3()
    RPD3(NCRN)
  }

  Method(_PS0,0,Serialized)
  {
    //
    // Return if RST Pcie Storage Remapping is disabled
    //
    If(LEqual(NITV,0))
    {
      //Fix warning: restricted method should not return a value
    }
    RPD0(NCRN)
    EPD0()

    //
    // Check NSR bit in PMCS
    // If this bit is 0, invoke CNRS() to perform restoration on the remapped device and hidden root port
    //
    Store(RDCA(NCRN,Add(NPMV,0x04),0x0,0x0,ENDPOINT_READ),Local0)
    If(LEqual(And(Local0,0x8),0))
    {
      CNRS()
    }
  }
