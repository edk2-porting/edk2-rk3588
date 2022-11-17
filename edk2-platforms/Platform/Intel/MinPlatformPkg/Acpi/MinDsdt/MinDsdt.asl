/** @file
  ACPI minimum DSDT table

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock (
  "DSDT.aml",
  "DSDT",
  0x01, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "INTEL ",   // OEM ID (6 byte string)
  "MIN     ",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)

// BEGIN OF ASL SCOPE
{
  Scope(\_SB) {
  //---------------------------------------------------------------------------
  // Begin PCI tree object scope
  //---------------------------------------------------------------------------
    Device(PCI0) { // PCI Bridge "Host Bridge"
      Name(_HID, EISAID("PNP0A08")) // Indicates PCI Express/PCI-X Mode2 host hierarchy
      Name(_CID, EISAID("PNP0A03")) // To support legacy OS that doesn't understand the new HID
      Name(_SEG, 0)
      Name(_ADR, 0x00000000)
      Method(^BN00, 0){ return(0x0000) }  // Returns default Bus number for Peer PCI busses. Name can be overriden with control method placed directly under Device scope
      Method(_BBN, 0){ return(BN00()) } // Bus number, optional for the Root PCI Bus
      Name(_UID, 0x0000)  // Unique Bus ID, optional
      Name(BUF0,ResourceTemplate()
      {
        //
        // I/O Region Allocation 0 ( 0x0000 - 0x0CF7 )
        //
        DWordIo(ResourceProducer,MinFixed,MaxFixed,PosDecode,EntireRange,
          0x00,0x0000,0x0CF7,0x00,0x0CF8,,,PI00)
        //
        // PCI Configuration Registers ( 0x0CF8 - 0x0CFF )
        //
        Io(Decode16,0x0CF8,0x0CF8,1,0x08)
        //
        // PCI MMIO space
        //
        DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,NonCacheable,
          ReadWrite,0x00,0x00,0x00,0x00,0x00,,,PM01)
      })
      Method(_CRS,0,Serialized)
      {
        //
        // Create pointers to Memory Sizing values.
        //
        CreateDwordField(BUF0, ^PM01._MIN,M1MN)
        CreateDwordField(BUF0, ^PM01._MAX,M1MX)
        CreateDwordField(BUF0, ^PM01._LEN,M1LN)

        //
        // Set Memory Size Values. TLUD represents bits 31:20 of phyical
        // TOM, so shift these bits into the correct position and fix up
        // the Memory Region available to PCI.
        //
        Subtract (FixedPcdGet32(PcdPciReservedMemLimit),FixedPcdGet32(PcdPciReservedMemBase),M1LN)
        Store (FixedPcdGet32(PcdPciReservedMemBase), M1MN)
        Subtract (FixedPcdGet32(PcdPciReservedMemLimit), 1, M1MX)

        Return(BUF0)
      }
    }
  }
}// End of ASL File

