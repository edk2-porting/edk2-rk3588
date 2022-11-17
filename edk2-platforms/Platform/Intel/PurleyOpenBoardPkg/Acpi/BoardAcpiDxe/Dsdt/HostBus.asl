/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// Define various System Agent (SA) PCI Configuration Space
// registers which will be used to dynamically produce all
// resources in the Host Bus _CRS.
//
OperationRegion (HBUS, PCI_Config, 0x00, 0x100)
Field (HBUS, DWordAcc, NoLock, Preserve)
{
  Offset(0x40),   // EPBAR (0:0:0:40)
  EPEN, 1,        // Enable
      , 11,
  EPBR, 20,       // EPBAR [31:12]

  Offset(0x48),   // MCHBAR (0:0:0:48)
  MHEN, 1,        // Enable
      , 14,
  MHBR, 17,       // MCHBAR [31:15]

  Offset(0x50),   // GGC (0:0:0:50)
  GCLK, 1,        // GGCLCK

  Offset(0x54),   // DEVEN (0:0:0:54)
  D0EN, 1,        // DEV0 Enable
  D1F2, 1,        // DEV1 FUN2 Enable
  D1F1, 1,        // DEV1 FUN1 Enable
  D1F0, 1,        // DEV1 FUN0 Enable

  Offset(0x60),   // PCIEXBAR (0:0:0:60)
  PXEN, 1,        // Enable
  PXSZ, 2,        // PCI Express Size
      , 23,
  PXBR, 6,        // PCI Express BAR [31:26]

  Offset(0x68),   // DMIBAR (0:0:0:68)
  DIEN, 1,        // Enable
      , 11,
  DIBR, 20,       // DMIBAR [31:12]

  Offset(0x70),   // MESEG_BASE (0:0:0:70)
      , 20,
  MEBR, 12,       // MESEG_BASE [31:20]

  Offset(0x80),   // PAM0 Register (0:0:0:80)
      , 4,
  PM0H, 2,        // PAM 0, High Nibble
      , 2,

  Offset(0x81),   // PAM1 Register (0:0:0:81)
  PM1L, 2,        // PAM1, Low  Nibble
      , 2,
  PM1H, 2,        // PAM1, High Nibble
      , 2,

  Offset(0x82),   // PAM2 Register (0:0:0:82)
  PM2L, 2,        // PAM2, Low  Nibble
      , 2,
  PM2H, 2,        // PAM2, High Nibble
      , 2,

  Offset(0x83),   // PAM3 Register (0:0:0:83)
  PM3L, 2,        // PAM3, Low  Nibble
      , 2,
  PM3H, 2,        // PAM3, High Nibble
      , 2,

  Offset(0x84),   // PAM4 Register (0:0:0:84)
  PM4L, 2,        // PAM4, Low  Nibble
      , 2,
  PM4H, 2,        // PAM4, High Nibble
      , 2,

  Offset(0x85),   // PAM5 Register (0:0:0:85)
  PM5L, 2,        // PAM5, Low  Nibble
      , 2,
  PM5H, 2,        // PAM5, High Nibble
      , 2,

  Offset(0x86),   // PAM6 Register (0:0:0:86)
  PM6L, 2,        // PAM6, Low  Nibble
      , 2,
  PM6H, 2,        // PAM6, High Nibble
      , 2,

  Offset(0xA8),   // Top of Upper Usable DRAM Register (0:0:0:A8)
      , 20,
  TUUD, 19,       // TOUUD [38:20]

  Offset(0xBC),   // Top of Lower Usable DRAM Register (0:0:0:BC)
      , 20,
  TLUD, 12,       // TOLUD [31:20]

  Offset(0xC8),   // ERRSTS register (0:0:0:C8)
      , 7,
  HTSE, 1         // Host Thermal Sensor Event for SMI/SCI/SERR
}
//
// Define a buffer that will store all the bus, memory, and IO information
// relating to the Host Bus.  This buffer will be dynamically altered in
// the _CRS and passed back to the OS.
//
Name(BUF0,ResourceTemplate()
{
  //
  // Bus Number Allocation: Bus 0 to 0xFF
  //
  WORDBusNumber(ResourceProducer,MinFixed,MaxFixed,PosDecode,0x00,
    0x0000,0x00FF,0x00,0x0100,,,PB00)

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
  // I/O Region Allocation 1 ( 0x0D00 - 0xFFFF )
  //
  DWordIo(ResourceProducer,MinFixed,MaxFixed,PosDecode,EntireRange,
    0x00,0x0D00,0xFFFF,0x00,0xF300,,,PI01)

  //
  // Video Buffer Area ( 0xA0000 - 0xBFFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xA0000,0xBFFFF,0x00,0x20000,,,A000)

  //
  // ISA Add-on BIOS Area ( 0xC0000 - 0xC3FFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xC0000,0xC3FFF,0x00,0x4000,,,C000)

  //
  // ISA Add-on BIOS Area ( 0xC4000 - 0xC7FFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xC4000,0xC7FFF,0x00,0x4000,,,C400)

  //
  // ISA Add-on BIOS Area ( 0xC8000 - 0xCBFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xC8000,0xCBFFF,0x00,0x4000,,,C800)

  //
  // ISA Add-on BIOS Area ( 0xCC000 - 0xCFFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xCC000,0xCFFFF,0x00,0x4000,,,CC00)

  //
  // ISA Add-on BIOS Area ( 0xD0000 - 0xD3FFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xD0000,0xD3FFF,0x00,0x4000,,,D000)

  //
  // ISA Add-on BIOS Area ( 0xD4000 - 0xD7FFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xD4000,0xD7FFF,0x00,0x4000,,,D400)

  //
  // ISA Add-on BIOS Area ( 0xD8000 - 0xDBFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xD8000,0xDBFFF,0x00,0x4000,,,D800)

  //
  // ISA Add-on BIOS Area ( 0xDC000 - 0xDFFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xDC000,0xDFFFF,0x00,0x4000,,,DC00)

  //
  // BIOS Extension Area ( 0xE0000 - 0xE3FFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xE0000,0xE3FFF,0x00,0x4000,,,E000)

  //
  // BIOS Extension Area ( 0xE4000 - 0xE7FFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xE4000,0xE7FFF,0x00,0x4000,,,E400)

  //
  // BIOS Extension Area ( 0xE8000 - 0xEBFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xE8000,0xEBFFF,0x00,0x4000,,,E800)

  //
  // BIOS Extension Area ( 0xEC000 - 0xEFFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xEC000,0xEFFFF,0x00,0x4000,,,EC00)

  //
  // BIOS Area ( 0xF0000 - 0xFFFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0xF0000,0xFFFFF,0x00,0x10000,,,F000)

//  //
//  // Memory Hole Region ( 0xF00000 - 0xFFFFFF )
//  //
//  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
//    ReadWrite,0x00,0xF00000,0xFFFFFF,0x00,0x100000,,,HOLE)

  //
  // PCI Memory Region ( TOLUD - 0xFEAFFFFF )
  //
  DWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0x00000000,0xFEAFFFFF,0x00,0xFEB00000,,,PM01)

  //
  // PCI Memory Region ( TOUUD - (TOUUD + ABOVE_4G_MMIO_SIZE) )
  // (This is dummy range for OS compatibility, will patch it in _CRS)
  //
  QWordMemory(ResourceProducer,PosDecode,MinFixed,MaxFixed,Cacheable,
    ReadWrite,0x00,0x10000,0x1FFFF,0x00,0x10000,,,PM02)
})

Name(EP_B, 0) // to store EP BAR
Name(MH_B, 0) // to store MCH BAR
Name(PC_B, 0) // to store PCIe BAR
Name(PC_L, 0) // to store PCIe BAR Length
Name(DM_B, 0) // to store DMI BAR


//
// Get PCIe BAR
//
Method(GPCB,0,Serialized)
{
  if(LEqual(PC_B,0))
  {
    //ShiftLeft(\_SB.PC00.PXBR,26,PC_B)
    Store(MCFG,PC_B)
  }
  Return(PC_B)
}

