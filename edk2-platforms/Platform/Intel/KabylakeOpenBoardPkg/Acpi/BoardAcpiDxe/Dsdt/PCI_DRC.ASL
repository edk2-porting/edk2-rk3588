/** @file
  ACPI DSDT table

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


Scope (\_SB.PCI0){

  Device(PDRC)
  {
    //
    // PCI Device Resource Consumption
    //

    Name(_HID,EISAID("PNP0C02"))        

    Name(_UID,1)

    Name(BUF0,ResourceTemplate()
    {
      //
      // MCH BAR _BAS will be updated in _CRS below according to B0:D0:F0:Reg.48h
      //
      Memory32Fixed(ReadWrite,0,0x08000,MCHB) 
      //
      // DMI BAR _BAS will be updated in _CRS below according to B0:D0:F0:Reg.68h
      //
      Memory32Fixed(ReadWrite,0,0x01000,DMIB) 
      //
      // EP BAR _BAS will be updated in _CRS below according to B0:D0:F0:Reg.40h
      //
      Memory32Fixed(ReadWrite,0,0x01000,EGPB)
      //
      // PCI Express BAR _BAS and _LEN will be updated in _CRS below according to B0:D0:F0:Reg.60h
      //
      Memory32Fixed(ReadWrite,0,0,PCIX)  

      //
      // MISC ICH TTT base address reserved for the TxT module use. Check if the hard code meets the real configuration.
      // If not, dynamically update it like the _CRS method below.
      //
      Memory32Fixed(ReadWrite,0xFED20000,0x20000)   
      
      //
      // VTD engine memory range. Check if the hard code meets the real configuration.
      // If not, dynamically update it like the _CRS method below.
      //
      Memory32Fixed(ReadOnly, 0xFED90000, 0x00004000)
      
      //
      // MISC ICH. Check if the hard code meets the real configuration.
      // If not, dynamically update it like the _CRS method below.
      //
      Memory32Fixed(ReadWrite,0xFED45000,0x4B000,TPMM)
      
      //
      // FLASH range
      //
      Memory32Fixed (ReadOnly, 0xFF000000, 0x1000000, FIOH) //16MB as per IOH spec
      
      //
      // Local APIC range(0xFEE0_0000 to 0xFEEF_FFFF)
      //
      Memory32Fixed (ReadOnly, 0xFEE00000, 0x100000, LIOH)
      
      //
      // Sx handler reserved MMIO
      //
      Memory32Fixed (ReadWrite, 0, 0, SXRE)
      
      //
      // Reserve HPET address decode range
      //
      Memory32Fixed (ReadWrite, 0, 0, HPET)

    })
 

    Method(_CRS,0,Serialized)
    {

      CreateDwordField(BUF0,^MCHB._BAS,MBR0)
      Store(\_SB.PCI0.GMHB(), MBR0)

      CreateDwordField(BUF0,^DMIB._BAS,DBR0)
      Store(\_SB.PCI0.GDMB(), DBR0)

      CreateDwordField(BUF0,^EGPB._BAS,EBR0)
      Store(\_SB.PCI0.GEPB(), EBR0)

      CreateDwordField(BUF0,^PCIX._BAS,XBR0)
      Store(\_SB.PCI0.GPCB(), XBR0)

      CreateDwordField(BUF0,^PCIX._LEN,XSZ0)
      Store(\_SB.PCI0.GPCL(), XSZ0)

      CreateDwordField(BUF0,^SXRE._BAS,SXRA)
      Store(SXRB, SXRA)
      CreateDwordField(BUF0,^SXRE._LEN,SXRL)
      Store(SXRS, SXRL)

      // HPET device claims the resource in LPC_DEV.ASL.
      If(LNOT(HPTE)){
        CreateDwordField(BUF0,^HPET._BAS,HBAS)
        CreateDwordField(BUF0,^HPET._LEN,HLEN)
        Store(HPTB, HBAS)
        Store(0x400, HLEN)
      }

      Return(BUF0)
    }
  } //end of PDRC
} // end of SB
