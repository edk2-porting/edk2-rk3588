/** @file
 *
 *  Copyright (c) 2022, Rockchip Electronics Co. Ltd.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

//
// Description: DMA
//
  Device (DMA0)
  {
    Name (_HID, "ARMH0330")
    Name (_UID, 0)
    Method (_CRS, 0, Serialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite,
          0xFEA10000,         // Address Base
          0x00004000,         // Address Length
        )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
        {
          0x00000076,
        }
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
        {
          0x00000077,
        }
      })
      Return (RBUF) /* \_SB_.DMA0._CRS.RBUF */
    }
    Name (_DSD, Package() {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"ctlrName", "DMA0"}
      }
    })
  }

  Device (DMA1)
  {
    Name (_HID, "ARMH0330")
    Name (_UID, 1)
    Method (_CRS, 0, Serialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite,
          0xFEA30000,         // Address Base
          0x00004000,         // Address Length
        )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
        {
          0x00000078,
        }
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
        {
          0x00000079,
        }
      })
      Return (RBUF) /* \_SB_.DMA0._CRS.RBUF */
    }
    Name (_DSD, Package() {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"ctlrName", "DMA1"}
      }
    })
  }

  Device (DMA2)
  {
    Name (_HID, "ARMH0330")
    Name (_UID, 2)
    Method (_CRS, 0, Serialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite,
          0xFED10000,         // Address Base
          0x00004000,         // Address Length
        )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
        {
          0x0000007A,
        }
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
        {
          0x0000007B,
        }
      })
      Return (RBUF) /* \_SB_.DMA0._CRS.RBUF */
    }
    Name (_DSD, Package() {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"ctlrName", "DMA2"}
      }
    })
  }