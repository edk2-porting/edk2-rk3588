/** @file
  ACPI DSDT table

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(\_SB) {
  Name(PR00, Package(){
// PCI Bridge
// D31: cAVS, SMBus, GbE, Nothpeak
    Package(){0x001FFFFF, 0, LNKA, 0 },
    Package(){0x001FFFFF, 1, LNKB, 0 },
    Package(){0x001FFFFF, 2, LNKC, 0 },
    Package(){0x001FFFFF, 3, LNKD, 0 },
// D30: SerialIo and SCS - can't use PIC
// D29: PCI Express Port 9-16
    Package(){0x001DFFFF, 0, LNKA, 0 },
    Package(){0x001DFFFF, 1, LNKB, 0 },
    Package(){0x001DFFFF, 2, LNKC, 0 },
    Package(){0x001DFFFF, 3, LNKD, 0 },
// D28: PCI Express Port 1-8
    Package(){0x001CFFFF, 0, LNKA, 0 },
    Package(){0x001CFFFF, 1, LNKB, 0 },
    Package(){0x001CFFFF, 2, LNKC, 0 },
    Package(){0x001CFFFF, 3, LNKD, 0 },
// D27: PCI Express Port 17-20
    Package(){0x001BFFFF, 0, LNKA, 0 },
    Package(){0x001BFFFF, 1, LNKB, 0 },
    Package(){0x001BFFFF, 2, LNKC, 0 },
    Package(){0x001BFFFF, 3, LNKD, 0 },
// D25: SerialIo - can't use PIC
// D23: SATA controller
    Package(){0x0017FFFF, 0, LNKA, 0 },
// D22: CSME (HECI, IDE-R, Keyboard and Text redirection
    Package(){0x0016FFFF, 0, LNKA, 0 },
    Package(){0x0016FFFF, 1, LNKB, 0 },
    Package(){0x0016FFFF, 2, LNKC, 0 },
    Package(){0x0016FFFF, 3, LNKD, 0 },
// D21: SerialIo - can't use PIC
// D20: xHCI, Thermal Subsystem, Camera IO Host Controller
    Package(){0x0014FFFF, 0, LNKA, 0 },
    Package(){0x0014FFFF, 1, LNKB, 0 },
    Package(){0x0014FFFF, 2, LNKC, 0 },
    Package(){0x0014FFFF, 3, LNKD, 0 },
// D19: Integrated Sensor Hub - can't use PIC

// Host Bridge
// P.E.G. Root Port D1F0
    Package(){0x0001FFFF, 0, LNKA, 0 },
    Package(){0x0001FFFF, 1, LNKB, 0 },
    Package(){0x0001FFFF, 2, LNKC, 0 },
    Package(){0x0001FFFF, 3, LNKD, 0 },
// P.E.G. Root Port D1F1
// P.E.G. Root Port D1F2
// SA IGFX Device
    Package(){0x0002FFFF, 0, LNKA, 0 },
// SA Thermal Device
    Package(){0x0004FFFF, 0, LNKA, 0 },
// SA SkyCam Device
    Package(){0x0005FFFF, 0, LNKA, 0 },
// SA GMM Device
    Package(){0x0008FFFF, 0, LNKA, 0 },
  })
  Name(AR00, Package(){
// PCI Bridge
// D31: cAVS, SMBus, GbE, Nothpeak
    Package(){0x001FFFFF, 0, 0, 16 },
    Package(){0x001FFFFF, 1, 0, 17 },
    Package(){0x001FFFFF, 2, 0, 18 },
    Package(){0x001FFFFF, 3, 0, 19 },
// D30: SerialIo and SCS
    Package(){0x001EFFFF, 0, 0, 20 },
    Package(){0x001EFFFF, 1, 0, 21 },
    Package(){0x001EFFFF, 2, 0, 22 },
    Package(){0x001EFFFF, 3, 0, 23 },
// D29: PCI Express Port 9-16
    Package(){0x001DFFFF, 0, 0, 16 },
    Package(){0x001DFFFF, 1, 0, 17 },
    Package(){0x001DFFFF, 2, 0, 18 },
    Package(){0x001DFFFF, 3, 0, 19 },
// D28: PCI Express Port 1-8
    Package(){0x001CFFFF, 0, 0, 16 },
    Package(){0x001CFFFF, 1, 0, 17 },
    Package(){0x001CFFFF, 2, 0, 18 },
    Package(){0x001CFFFF, 3, 0, 19 },
// D27: PCI Express Port 17-20
    Package(){0x001BFFFF, 0, 0, 16 },
    Package(){0x001BFFFF, 1, 0, 17 },
    Package(){0x001BFFFF, 2, 0, 18 },
    Package(){0x001BFFFF, 3, 0, 19 },
// D25: SerialIo
    Package(){0x0019FFFF, 0, 0, 32 },
    Package(){0x0019FFFF, 1, 0, 33 },
    Package(){0x0019FFFF, 2, 0, 34 },
// D23: SATA controller
    Package(){0x0017FFFF, 0, 0, 16 },
// D22: CSME (HECI, IDE-R, Keyboard and Text redirection
    Package(){0x0016FFFF, 0, 0, 16 },
    Package(){0x0016FFFF, 1, 0, 17 },
    Package(){0x0016FFFF, 2, 0, 18 },
    Package(){0x0016FFFF, 3, 0, 19 },
// D21: SerialIo
    Package(){0x0015FFFF, 0, 0, 16 },
    Package(){0x0015FFFF, 1, 0, 17 },
    Package(){0x0015FFFF, 2, 0, 18 },
    Package(){0x0015FFFF, 3, 0, 19 },
// D20: xHCI, OTG, Thermal Subsystem, Camera IO Host Controller
    Package(){0x0014FFFF, 0, 0, 16 },
    Package(){0x0014FFFF, 1, 0, 17 },
    Package(){0x0014FFFF, 2, 0, 18 },
    Package(){0x0014FFFF, 3, 0, 19 },
// D19: Integrated Sensor Hub
    Package(){0x0013FFFF, 0, 0, 20 },

// Host Bridge
// P.E.G. Root Port D1F0
    Package(){0x0001FFFF, 0, 0, 16 },
    Package(){0x0001FFFF, 1, 0, 17 },
    Package(){0x0001FFFF, 2, 0, 18 },
    Package(){0x0001FFFF, 3, 0, 19 },
// P.E.G. Root Port D1F1
// P.E.G. Root Port D1F2
// SA IGFX Device
    Package(){0x0002FFFF, 0, 0, 16 },
// SA Thermal Device
    Package(){0x0004FFFF, 0, 0, 16 },
// SA SkyCam Device
    Package(){0x0005FFFF, 0, 0, 16 },
// SA GMM Device
    Package(){0x0008FFFF, 0, 0, 16 },
  })
  Name(PR04, Package(){
    Package(){0x0000FFFF, 0, LNKA, 0 },
    Package(){0x0000FFFF, 1, LNKB, 0 },
    Package(){0x0000FFFF, 2, LNKC, 0 },
    Package(){0x0000FFFF, 3, LNKD, 0 },
  })
  Name(AR04, Package(){
    Package(){0x0000FFFF, 0, 0, 16 },
    Package(){0x0000FFFF, 1, 0, 17 },
    Package(){0x0000FFFF, 2, 0, 18 },
    Package(){0x0000FFFF, 3, 0, 19 },
  })
  Name(PR05, Package(){
    Package(){0x0000FFFF, 0, LNKB, 0 },
    Package(){0x0000FFFF, 1, LNKC, 0 },
    Package(){0x0000FFFF, 2, LNKD, 0 },
    Package(){0x0000FFFF, 3, LNKA, 0 },
  })
  Name(AR05, Package(){
    Package(){0x0000FFFF, 0, 0, 17 },
    Package(){0x0000FFFF, 1, 0, 18 },
    Package(){0x0000FFFF, 2, 0, 19 },
    Package(){0x0000FFFF, 3, 0, 16 },
  })
  Name(PR06, Package(){
    Package(){0x0000FFFF, 0, LNKC, 0 },
    Package(){0x0000FFFF, 1, LNKD, 0 },
    Package(){0x0000FFFF, 2, LNKA, 0 },
    Package(){0x0000FFFF, 3, LNKB, 0 },
  })
  Name(AR06, Package(){
    Package(){0x0000FFFF, 0, 0, 18 },
    Package(){0x0000FFFF, 1, 0, 19 },
    Package(){0x0000FFFF, 2, 0, 16 },
    Package(){0x0000FFFF, 3, 0, 17 },
  })
  Name(PR07, Package(){
    Package(){0x0000FFFF, 0, LNKD, 0 },
    Package(){0x0000FFFF, 1, LNKA, 0 },
    Package(){0x0000FFFF, 2, LNKB, 0 },
    Package(){0x0000FFFF, 3, LNKC, 0 },
  })
  Name(AR07, Package(){
    Package(){0x0000FFFF, 0, 0, 19 },
    Package(){0x0000FFFF, 1, 0, 16 },
    Package(){0x0000FFFF, 2, 0, 17 },
    Package(){0x0000FFFF, 3, 0, 18 },
  })
  Name(PR08, Package(){
    Package(){0x0000FFFF, 0, LNKA, 0 },
    Package(){0x0000FFFF, 1, LNKB, 0 },
    Package(){0x0000FFFF, 2, LNKC, 0 },
    Package(){0x0000FFFF, 3, LNKD, 0 },
  })
  Name(AR08, Package(){
    Package(){0x0000FFFF, 0, 0, 16 },
    Package(){0x0000FFFF, 1, 0, 17 },
    Package(){0x0000FFFF, 2, 0, 18 },
    Package(){0x0000FFFF, 3, 0, 19 },
  })
  Name(PR09, Package(){
    Package(){0x0000FFFF, 0, LNKB, 0 },
    Package(){0x0000FFFF, 1, LNKC, 0 },
    Package(){0x0000FFFF, 2, LNKD, 0 },
    Package(){0x0000FFFF, 3, LNKA, 0 },
  })
  Name(AR09, Package(){
    Package(){0x0000FFFF, 0, 0, 17 },
    Package(){0x0000FFFF, 1, 0, 18 },
    Package(){0x0000FFFF, 2, 0, 19 },
    Package(){0x0000FFFF, 3, 0, 16 },
  })
  Name(PR0E, Package(){
    Package(){0x0000FFFF, 0, LNKC, 0 },
    Package(){0x0000FFFF, 1, LNKD, 0 },
    Package(){0x0000FFFF, 2, LNKA, 0 },
    Package(){0x0000FFFF, 3, LNKB, 0 },
  })
  Name(AR0E, Package(){
    Package(){0x0000FFFF, 0, 0, 18 },
    Package(){0x0000FFFF, 1, 0, 19 },
    Package(){0x0000FFFF, 2, 0, 16 },
    Package(){0x0000FFFF, 3, 0, 17 },
  })
  Name(PR0F, Package(){
    Package(){0x0000FFFF, 0, LNKD, 0 },
    Package(){0x0000FFFF, 1, LNKA, 0 },
    Package(){0x0000FFFF, 2, LNKB, 0 },
    Package(){0x0000FFFF, 3, LNKC, 0 },
  })
  Name(AR0F, Package(){
    Package(){0x0000FFFF, 0, 0, 19 },
    Package(){0x0000FFFF, 1, 0, 16 },
    Package(){0x0000FFFF, 2, 0, 17 },
    Package(){0x0000FFFF, 3, 0, 18 },
  })
  Name(PR02, Package(){
    Package(){0x0000FFFF, 0, LNKA, 0 },
    Package(){0x0000FFFF, 1, LNKB, 0 },
    Package(){0x0000FFFF, 2, LNKC, 0 },
    Package(){0x0000FFFF, 3, LNKD, 0 },
  })
  Name(AR02, Package(){
// P.E.G. Port Slot x16
    Package(){0x0000FFFF, 0, 0, 16 },
    Package(){0x0000FFFF, 1, 0, 17 },
    Package(){0x0000FFFF, 2, 0, 18 },
    Package(){0x0000FFFF, 3, 0, 19 },
  })
  Name(PR0A, Package(){
// P.E.G. Port Slot x8
    Package(){0x0000FFFF, 0, LNKB, 0 },
    Package(){0x0000FFFF, 1, LNKC, 0 },
    Package(){0x0000FFFF, 2, LNKD, 0 },
    Package(){0x0000FFFF, 3, LNKA, 0 },
  })
  Name(AR0A, Package(){
// P.E.G. Port Slot x8
    Package(){0x0000FFFF, 0, 0, 17 },
    Package(){0x0000FFFF, 1, 0, 18 },
    Package(){0x0000FFFF, 2, 0, 19 },
    Package(){0x0000FFFF, 3, 0, 16 },
  })
  Name(PR0B, Package(){
// P.E.G. Port Slot x4
    Package(){0x0000FFFF, 0, LNKC, 0 },
    Package(){0x0000FFFF, 1, LNKD, 0 },
    Package(){0x0000FFFF, 2, LNKA, 0 },
    Package(){0x0000FFFF, 3, LNKB, 0 },
  })
  Name(AR0B, Package(){
// P.E.G. Port Slot x4
    Package(){0x0000FFFF, 0, 0, 18 },
    Package(){0x0000FFFF, 1, 0, 19 },
    Package(){0x0000FFFF, 2, 0, 16 },
    Package(){0x0000FFFF, 3, 0, 17 },
  })
//---------------------------------------------------------------------------
// List of IRQ resource buffers compatible with _PRS return format.
//---------------------------------------------------------------------------
// Naming legend:
// RSxy, PRSy - name of the IRQ resource buffer to be returned by _PRS, "xy" - last two characters of IRQ Link name.
// Note. PRSy name is generated if IRQ Link name starts from "LNK".
// HLxy , LLxy - reference names, can be used to access bit mask of available IRQs. HL and LL stand for active High(Low) Level triggered Irq model.
//---------------------------------------------------------------------------
  Name(PRSA, ResourceTemplate(){  // Link name: LNKA
    IRQ(Level, ActiveLow, Shared, LLKA) {3,4,5,6,10,11,12,14,15}
  })
  Alias(PRSA,PRSB)  // Link name: LNKB
  Alias(PRSA,PRSC)  // Link name: LNKC
  Alias(PRSA,PRSD)  // Link name: LNKD
  Alias(PRSA,PRSE)  // Link name: LNKE
  Alias(PRSA,PRSF)  // Link name: LNKF
  Alias(PRSA,PRSG)  // Link name: LNKG
  Alias(PRSA,PRSH)  // Link name: LNKH
//---------------------------------------------------------------------------
// Begin PCI tree object scope
//---------------------------------------------------------------------------
  Device(PCI0) { // PCI Bridge "Host Bridge"
    Name(_HID, EISAID("PNP0A08")) // Indicates PCI Express/PCI-X Mode2 host hierarchy
    Name(_CID, EISAID("PNP0A03")) // To support legacy OS that doesn't understand the new HID
    Name(_ADR, 0x00000000)
    Method(^BN00, 0){ return(0x0000) }  // Returns default Bus number for Peer PCI busses. Name can be overriden with control method placed directly under Device scope
    Method(_BBN, 0){ return(BN00()) } // Bus number, optional for the Root PCI Bus
    Name(_UID, 0x0000)  // Unique Bus ID, optional
    Method(_PRT,0) {
      If(PICM) {Return(AR00)} // APIC mode
      Return (PR00) // PIC Mode
    } // end _PRT

  Include("HostBus.asl")
  } // end PCI0 Bridge "Host Bridge"
} // end _SB scope
