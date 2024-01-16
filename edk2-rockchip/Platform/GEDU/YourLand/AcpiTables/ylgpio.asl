/** @file
 *
 *  Copyright (c) 2022, Rockchip Electronics Co. Ltd.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
 #include "AcpiTables.h"

 //
 // Description: GPIO
 //
 Device (GPI0)
 {
   Name (_HID, "GEDU0826")
   Name (_UID, 0x0)
   Name (_DSD, Package () {
     ToUUID("195CA0F9-0F7B-4364-862F-1FAEFF3523AD"),
     Package () {
         Package () { "compatible", "rockchip,gpio-bank" },
     }
   })
   OperationRegion (GPO0, GeneralPurposeIo, Zero, One)
   Field (GPO0, ByteAcc, NoLock, Preserve)
   {
       Connection (
           GpioIo (Shared, PullNone, 0x0000, 0x0000, IoRestrictionNone,
               "\\_SB.GPI0", 0x00, ResourceConsumer, ,
               )
               {   // Pin list
                   0x1A // GPIO0-D2
               }
       ), 
       LIDR,   1
   }   
   Field (GPO0, ByteAcc, NoLock, Preserve)
   {
       Connection (
           GpioIo (Shared, PullNone, 0x0000, 0x0000, IoRestrictionNone,
               "\\_SB.GPI0", 0x00, ResourceConsumer, ,
               )
               {   // Pin list
                   0x19 // GPIO0-D1 DC-DETECT
               }
       ), 
       DCDT,   1
   }      
   Method (_CRS, 0x0, NotSerialized)
   {
     Name (RBUF, ResourceTemplate()
     {
       Memory32Fixed(ReadWrite, 0xFD8A0000, 0x100)
       Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) {309}
     })
     Return(RBUF)
   }
   Method (_STA)
   {
     Return(0xf)
   }
   Name (GABL, Zero)
   Method (_REG, 2, NotSerialized)  // _REG: Region Availability
   {
       If ((Arg0 == 0x08))
       {
           GABL = Arg1
       }
   }   
 }//GPIO0
 
 
 Device (GPI1)
 {
   Name (_HID, "GEDU0826")
   Name (_UID, 0x1)
   Name (_DSD, Package () {
     ToUUID("195CA0F9-0F7B-4364-862F-1FAEFF3523AD"),
     Package () {
         Package () { "compatible", "rockchip,gpio-bank" },
     }
   })
   Method (_CRS, 0x0, NotSerialized)
   {
     Name (RBUF, ResourceTemplate()
     {
       Memory32Fixed(ReadWrite, 0xFEC20000, 0x100)
       Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) {310}
     })
     Return(RBUF)
   }
   Method (_STA)
   {
     Return(0xf)
   }
 }//GPIO1
 
Device (GPI2)
{
  Name (_HID, "GEDU0826")
  Name (_UID, 0x2)
  Name (_DSD, Package () {
    ToUUID("195CA0F9-0F7B-4364-862F-1FAEFF3523AD"),
    Package () {
        Package () { "compatible", "rockchip,gpio-bank" },
    }
  })
  OperationRegion (GPO2, GeneralPurposeIo, Zero, One)
  Field (GPO2, ByteAcc, NoLock, Preserve)
  {
      Connection (GpioIo (Shared, PullNone, 0x0000, 0x0000, IoRestrictionNone,
              "\\_SB.GPI2", 0x00, ResourceConsumer, ,
              )
              {   // Pin list
                  0xF // GPIO2-B7
              }
      ), 
      SPKC,   1
  }   
  Method (_CRS, 0x0, NotSerialized)
  {
    Name (RBUF, ResourceTemplate()
    {
      Memory32Fixed(ReadWrite, 0xFEC30000, 0x100)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) {311}
    })
    Return(RBUF)
  }
  Method (_STA)
  {
    Return(0xf)
  }
}//GPIO2
 
Device (GPI3)
{
  Name (_HID, "GEDU0826")
  Name (_UID, 0x3)
  Name (_DSD, Package () {
    ToUUID("195CA0F9-0F7B-4364-862F-1FAEFF3523AD"),
    Package () {
        Package () { "compatible", "rockchip,gpio-bank" },
    }
  })
  Method (_CRS, 0x0, NotSerialized)
  {
    Name (RBUF, ResourceTemplate()
    {
      Memory32Fixed(ReadWrite, 0xFEC40000, 0x100)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) {312}
    })
    Return(RBUF)
  }
  Method (_STA)
  {
    Return(0xf)
  }
}//GPIO3

Device (GPI4)
{
  Name (_HID, "GEDU0826")
  Name (_UID, 0x4)
  Name (_DSD, Package () {
    ToUUID("195CA0F9-0F7B-4364-862F-1FAEFF3523AD"),
    Package () {
        Package () { "compatible", "rockchip,gpio-bank" },
    }
  })
  Method (_CRS, 0x0, NotSerialized)
  {
    Name (RBUF, ResourceTemplate()
    {
      Memory32Fixed(ReadWrite, 0xFEC50000, 0x100)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) {313}
    })
    Return(RBUF)
  }
  OperationRegion (GPO4, GeneralPurposeIo, Zero, One)
  Field (GPO4, ByteAcc, NoLock, Preserve)
  {
      Connection (GpioIo (Shared, PullNone, 0x0000, 0x0000, IoRestrictionNone,
              "\\_SB.GPI4", 0x00, ResourceConsumer, ,
              )
              {   // Pin list
                  0x1A // GPIO4-B5 LCD_BL_EN
              }
      ), 
      BLEN,   1 
  }   
  Method (_STA)
  {
    Return(0xf)
  }
}//GPIO4

Device (PINC)
{
  Name (_HID, "GEDU0825")
  Name (_UID, 0x5)
  Name (_DSD, Package () {
    ToUUID("195CA0F9-0F7B-4364-862F-1FAEFF3523AD"),
    Package () {
        Package () { "compatible", "rockchip,rk3588-pinctrl" },
    }
  })
  OperationRegion(BIOC, SystemMemory, 0xFD5F8000, 0x1000) // BUS_IOC
  Field(BIOC, DWordAcc, NoLock, WriteAsZeros) { 
    Offset (0xC), 
    GPBH, 32,
    GPCL, 32,
    GPCH, 32,
    GPDL, 32,
    GPDH, 32,
    G1AL, 32,
    G1AH, 32,
    G1BL, 32,
    G1BH, 32,
    G1CL, 32,
    G1CH, 32,
    G1DL, 32,
    G1DH, 32,
    G2AL, 32,
    G2AH, 32,
    G2BL, 32,
    G2BH, 32,
    G2CL, 32,
    G2CH, 32,
    G2DL, 32,
    G2DH, 32
  }
  OperationRegion(PMU2, SystemMemory, 0xFD5F4000, 0x4000) // PMU2_IOC
  Field(PMU2, DWordAcc, NoLock, WriteAsZeros) { 
    MGBH, 32, 
    MGCL, 32,
    MGCH, 32,
    MGDL, 32,
    MGDH, 32
  }
  Method (_CRS, 0x0, NotSerialized)
  {
    Name (RBUF, ResourceTemplate()
    {
      Memory32Fixed(ReadWrite, 0xFD5F0000, 0x10000)
    })
    Return(RBUF)
  }
  Method (_STA)
  {
    Return(0xf)
  }
}