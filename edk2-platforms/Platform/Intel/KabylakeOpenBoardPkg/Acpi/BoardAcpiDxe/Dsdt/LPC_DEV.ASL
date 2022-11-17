/** @file
  ACPI DSDT table

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device(FWHD)  // Firmware Hub Device
{
  Name(_HID,EISAID("INT0800"))        

  Name(_CRS,ResourceTemplate()
  {
    Memory32Fixed(ReadOnly,0xFF000000,0x1000000)
  })
}


Device(HPET)  // High Performance Event Timer
{
  Name(_HID,EISAID("PNP0103"))
  Name(_UID, 0)

  Name(BUF0,ResourceTemplate()
  {
    Memory32Fixed(ReadWrite,0xFED00000,0x400,FED0)
  })

  Method(_STA,0)
  {
    // Show this Device only if the OS is WINXP or beyond.
    If(HPTE)
    {
      Return(0x000F)  // Enabled, do Display.
    }

    Return(0x0000)      // Return Nothing.
  }

  Method(_CRS,0,Serialized)
  {
    If(HPTE)
    {
      // Check if HPETimer Base should be modified.
      CreateDwordField(BUF0,^FED0._BAS,HPT0)
      Store(HPTB,HPT0)
    }

    Return(BUF0)
  }
}

Device(IPIC)  // 8259 PIC
{
  Name(_HID,EISAID("PNP0000"))

  Name(_CRS,ResourceTemplate()
  {
    IO(Decode16,0x20,0x20,0x01,0x02)
    IO(Decode16,0x24,0x24,0x01,0x02)
    IO(Decode16,0x28,0x28,0x01,0x02)
    IO(Decode16,0x2C,0x2C,0x01,0x02)
    IO(Decode16,0x30,0x30,0x01,0x02)
    IO(Decode16,0x34,0x34,0x01,0x02)
    IO(Decode16,0x38,0x38,0x01,0x02)
    IO(Decode16,0x3C,0x3C,0x01,0x02)
    IO(Decode16,0xA0,0xA0,0x01,0x02)
    IO(Decode16,0xA4,0xA4,0x01,0x02)
    IO(Decode16,0xA8,0xA8,0x01,0x02)
    IO(Decode16,0xAC,0xAC,0x01,0x02)
    IO(Decode16,0xB0,0xB0,0x01,0x02)
    IO(Decode16,0xB4,0xB4,0x01,0x02)
    IO(Decode16,0xB8,0xB8,0x01,0x02)
    IO(Decode16,0xBC,0xBC,0x01,0x02)
    IO(Decode16,0x4D0,0x4D0,0x01,0x02)
    IRQNoFlags() {2}
  })
}


Device(MATH)  // Math Co-Processor
{
  Name(_HID,EISAID("PNP0C04"))

  Name(_CRS,ResourceTemplate()
  {
    IO(Decode16,0xF0,0xF0,0x01,0x01)
    IRQNoFlags() {13}
  })

  //
  // Report device present for LPT-H.
  //
  Method (_STA, 0x0, NotSerialized) 
  {
    If(LEqual(PCHV(), SPTH)) {
      Return(0x1F)
    } else {
      Return(0x0)
    }
  }
}


Device(LDRC)  // LPC Device Resource Consumption
{
  Name(_HID,EISAID("PNP0C02"))        

  Name(_UID,2)

  Name(_CRS,ResourceTemplate()                      // This is for Cougar Point
  {
    IO(Decode16,0x2E,0x2E,0x1,0x02)             // SIO Access.
    IO(Decode16,0x4E,0x4E,0x1,0x02)             // LPC Slot Access.
    IO(Decode16,0x61,0x61,0x1,0x1)               // NMI Status.           
    IO(Decode16,0x63,0x63,0x1,0x1)               // Processor I/F.
    IO(Decode16,0x65,0x65,0x1,0x1)               // Processor I/F.
    IO(Decode16,0x67,0x67,0x1,0x1)               // Processor I/F.
    IO(Decode16,0x70,0x70,0x1,0x1)               // NMI Enable.          
    IO(Decode16,0x80,0x80,0x1,0x1)               // Port 80h.
    IO(Decode16,0x92,0x92,0x1,0x1)               // Processor I/F.
    IO(Decode16,0xB2,0xB2,0x01,0x02)           // Software SMI.
    IO(Decode16,0x680,0x680,0x1,0x20)        // 32 Byte I/O.
    IO(Decode16,0xFFFF,0xFFFF,0x1,0x1)        // ACPI IO Trap.
    IO(Decode16,0xFFFF,0xFFFF,0x1,0x1)        // DTS IO Trap. 
    IO(Decode16,0xFFFF,0xFFFF,0x1,0x1)        // HotKey IO Trap.                      

    IO(Decode16, 0x1800,0x1800,0x1,0xFF)     // PCH ACPI Base

    IO(Decode16,0x164e,0x164e,0x1,0x02)    // 16 Byte I/O.
  })
}

Device(LDR2)  // LPC Device Resource Consumption for PCH GPIO
{
  Name(_HID,EISAID("PNP0C02"))        

  Name(_UID, "LPC_DEV")
  
  // LPT-H GPIO resource map
  Name(_CRS,ResourceTemplate()
  {
    IO(Decode16, 0x800,0x800,0x1,0x80)       // PCH GPIO Base.
  })

  Method(_STA, 0, NotSerialized)
  {
    If(LEqual(PCHV(), SPTH)) {
      Return(0xF)
    } else {
      Return(0)
    }
  }
}

Device(RTC) // RTC
{
  Name(_HID,EISAID("PNP0B00"))

  Name(_CRS,ResourceTemplate()
  {
    IO(Decode16,0x70,0x70,0x01,0x08)
    IRQNoFlags() {8}
  })
}

Device(TIMR)  // 8254 Timer
{
  Name(_HID,EISAID("PNP0100"))

  Name(_CRS,ResourceTemplate()
  {
    IO(Decode16,0x40,0x40,0x01,0x04)
    IO(Decode16,0x50,0x50,0x10,0x04)
    IRQNoFlags() {0}
  })
}
    
Device(CWDT)
{
  Name(_HID,EISAID("INT3F0D"))
  Name(_CID,EISAID("PNP0C02"))
  Name(BUF0,ResourceTemplate()
    {
      IO(Decode16, 0x1854, 0x1854, 0x4, 0x4)  // PMBS 0x1800 + Offset 0x54
    }
  )
  
  Method(_STA,0,Serialized)
  {
    Return(0x0F)
  }
  
  Method(_CRS,0,Serialized)
  {
    Return(BUF0)
  }
}
