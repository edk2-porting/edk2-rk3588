/** @file
*
* Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
* Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
* Copyright (c) 2016, Linaro Limited. All rights reserved.<BR>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

//
// Ged
//

//Define a control method power button
Scope(_SB)
{
  OperationRegion(IOM1, SystemMemory, 0x941900C8, 0x4)
  Field(IOM1, DWordAcc, NoLock, Preserve) {
    IMX0, 32,
  }

  Method (_INI) {
    Store(IMX0, Local0)
    And(Local0, 0xFFFFFFFC, Local0)
    Or(Local0, 0x4, Local0)
    Store(Local0, IMX0)
  }

  Device(GED1) {
    Name(_HID, "ACPI0013")
    Name(_UID, 0)

    Name (_CRS, ResourceTemplate ()  {
      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive) {
        121
      }
    })

    Method (_STA, 0x0, NotSerialized) {
      return (0xF);
    }

    Method (_EVT, 0x1) {
      Switch(ToInteger(Arg0)) {
        Case(121) {
          Notify (\_SB.PWRB, 0x80)
        }
      }
    }
  }
}
