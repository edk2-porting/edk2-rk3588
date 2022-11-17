/** @file
 *
 *  Secondary System Description Table (SSDT) for active (fan) cooling
 *
 *  Copyright (c) 2020, Arm Ltd. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Bcm2711.h>
#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/Bcm2836Gpio.h>

#include <IndustryStandard/Acpi.h>

DefinitionBlock (__FILE__, "SSDT", 2, "RPIFDN", "RPITHFAN", 2)
{
  External (\_SB_.EC00, DeviceObj)
  External (\_SB_.EC00.TZ00, DeviceObj)

  Scope (\_SB_.EC00)
  {
    // Define a NameOp we will modify during InstallTable
    Name (GIOP, 0x2) //08 47 49 4f 50 0a 02 (value must be >1)
    Name (FTMP, 0x2)
    // Describe a fan
    PowerResource (PFAN, 0, 0) {
      OperationRegion (GPIO, SystemMemory, GPIO_BASE_ADDRESS, 0x1000)
      Field (GPIO, DWordAcc, NoLock, Preserve) {
        Offset (0x1C),
        GPS0, 32,
        GPS1, 32,
        RES1, 32,
        GPC0, 32,
        GPC1, 32,
        RES2, 32,
        GPL1, 32,
        GPL2, 32
      }
      // We are hitting a GPIO pin to on/off a fan.
      // This assumes that UEFI has programmed the
      // direction as OUT. Given the current limitations
      // on the GPIO pins, its recommended to use
      // the GPIO to switch a larger voltage/current
      // for the fan rather than driving it directly.
      Method (_STA) {
        if (GPL1 & (1 << GIOP)) {
          Return (1)                 // present and enabled
        }
        Return (0)
      }
      Method (_ON)  {                // turn fan on
        Store (1 << GIOP, GPS0)
      }
      Method (_OFF) {                // turn fan off
        Store (1 << GIOP, GPC0)
      }
    }
    Device (FAN0) {
      // Note, not currently an ACPIv4 fan
      // the latter adds speed control/detection
      // but in the case of linux needs FIF, FPS, FSL, and FST
      Name (_HID, EISAID ("PNP0C0B"))
      Name (_PR0, Package () { PFAN })
    }
  }

  // merge in an active cooling point.
  Scope (\_SB_.EC00.TZ00)
  {
    Method (_AC0) { Return ( (FTMP * 10) + 2732) } // (60C) active cooling trip point,
                                                   // if this is lower than PSV then we
                                                   // prefer active cooling
    Name (_AL0, Package () { \_SB_.EC00.FAN0 })    // the fan used for AC0 above
  }
}
