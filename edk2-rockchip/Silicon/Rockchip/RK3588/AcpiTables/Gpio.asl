/** @file
 *
 *  Copyright (c) 2022, Rockchip Electronics Co. Ltd.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

#define GPIO_PIN_PA0    0
#define GPIO_PIN_PA1    1
#define GPIO_PIN_PA2    2
#define GPIO_PIN_PA3    3
#define GPIO_PIN_PA4    4
#define GPIO_PIN_PA5    5
#define GPIO_PIN_PA6    6
#define GPIO_PIN_PA7    7
#define GPIO_PIN_PB0    8
#define GPIO_PIN_PB1    9
#define GPIO_PIN_PB2    10
#define GPIO_PIN_PB3    11
#define GPIO_PIN_PB4    12
#define GPIO_PIN_PB5    13
#define GPIO_PIN_PB6    14
#define GPIO_PIN_PB7    15
#define GPIO_PIN_PC0    16
#define GPIO_PIN_PC1    17
#define GPIO_PIN_PC2    18
#define GPIO_PIN_PC3    19
#define GPIO_PIN_PC4    20
#define GPIO_PIN_PC5    21
#define GPIO_PIN_PC6    22
#define GPIO_PIN_PC7    23
#define GPIO_PIN_PD0    24
#define GPIO_PIN_PD1    25
#define GPIO_PIN_PD2    26
#define GPIO_PIN_PD3    27
#define GPIO_PIN_PD4    28
#define GPIO_PIN_PD5    29
#define GPIO_PIN_PD6    30
#define GPIO_PIN_PD7    31

//
// Description: GPIO
//
Device (GPI0)
{
  Name (_HID, "RKCP3002")
  Name (_CID, "PRP0001")
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
  Name (_HID, "RKCP3002")
  Name (_CID, "PRP0001")
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
  Name (_HID, "RKCP3002")
  Name (_CID, "PRP0001")
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
  Name (_HID, "RKCP3002")
  Name (_CID, "PRP0001")
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
  Name (_HID, "RKCP3002")
  Name (_CID, "PRP0001")
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
