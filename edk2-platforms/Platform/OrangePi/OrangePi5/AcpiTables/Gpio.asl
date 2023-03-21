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
  Name (_HID, "PRP0001")
  Name (_UID, 0x0)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package () { "compatible", "rockchip,gpio-bank" },
    }
  })
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
}//GPIO0


Device (GPI1)
{
  Name (_HID, "PRP0001")
  Name (_UID, 0x1)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
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
  Name (_HID, "PRP0001")
  Name (_UID, 0x2)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package () { "compatible", "rockchip,gpio-bank" },
    }
  })
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
  Name (_HID, "PRP0001")
  Name (_UID, 0x3)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
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
  Name (_HID, "PRP0001")
  Name (_UID, 0x4)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
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
  Method (_STA)
  {
    Return(0xf)
  }
}//GPIO4

Device (PINC)
{
  Name (_HID, "PRP0001")
  Name (_UID, 0x4)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package () { "compatible", "rockchip,rk3588-pinctrl" },
    }
  })
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