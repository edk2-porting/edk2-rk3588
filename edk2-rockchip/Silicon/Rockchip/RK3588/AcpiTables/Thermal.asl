/** @file
 *
 *  SoC thermal zone, read from the TSADC (started by RK3588Dxe),
 *  with optional active cooling through a PWM fan.
 *
 *  Copyright (c) 2026, Meari <275577454+Meari-Prototype@users.noreply.github.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

//
// Temperatures are in tenths of a Kelvin.
//
#define DECI_KELVIN(C)  ((C) * 10 + 2732)

OperationRegion (TSAD, SystemMemory, 0xfec00000, 0x100)
Field (TSAD, DWordAcc, NoLock, Preserve) {
  Offset (0x2c),
  TSD0, 32,       // DATA0: package (top) sensor
}

//
// TSADC code to temperature, interpolated from the RK3588 code table:
// 215 = -40 C, 285 = 25 C, 350 = 85 C, 395 = 125 C.
//
Method (TCVT, 1, Serialized) {
  Local0 = Arg0 & 0x1ff
  If (Local0 <= 215) {
    Return (2332)                 // -40 C
  }
  If (Local0 <= 285) {
    Return (2332 + (((Local0 - 215) * 650) / 70))
  }
  If (Local0 <= 350) {
    Return (DECI_KELVIN (25) + (((Local0 - 285) * 600) / 65))
  }
  If (Local0 <= 395) {
    Return (DECI_KELVIN (85) + (((Local0 - 350) * 400) / 45))
  }
  Return (DECI_KELVIN (125))
}

#ifdef BOARD_FAN_PWM_BASE

//
// Four fan levels between 60 and 85 C, 2 C of hysteresis, the same as the
// vendor device tree (cooling-levels 0 64 128 192 255). The duty cycle is set
// in the PWM period that the platform library programmed.
//
Name (FANE, 1)                    // cleared by AcpiPlatformDxe when the fan is disabled in setup
Name (FLVL, 0xf)                  // levels on, bit n = FANn; the firmware leaves the fan running

OperationRegion (FPWM, SystemMemory, BOARD_FAN_PWM_BASE, 0x10)
Field (FPWM, DWordAcc, NoLock, Preserve) {
  FCNT, 32,
  FPER, 32,
  FDUT, 32,
  FCTL, 32,
}

Method (FSET, 0, Serialized) {
  If (FANE == 0) {
    Return (Zero)
  }
  If (FLVL & 8) {
    Local0 = 255
  } ElseIf (FLVL & 4) {
    Local0 = 192
  } ElseIf (FLVL & 2) {
    Local0 = 128
  } ElseIf (FLVL & 1) {
    Local0 = 64
  } Else {
    Local0 = 0
  }
  FCTL |= 0x40                    // conlock
  FDUT = (FPER * Local0) / 255
  FCTL &= ~0x40
}

#define FAN_LEVEL(Index, Bit)                       \
  PowerResource (PFN##Index, 0, 0) {                \
    Method (_STA) {                                 \
      If (FLVL & Bit) {                             \
        Return (1)                                  \
      }                                             \
      Return (0)                                    \
    }                                               \
    Method (_ON) {                                  \
      FLVL |= Bit                                   \
      FSET ()                                       \
      Notify (\_SB.TZ00, 0x81)                      \
    }                                               \
    Method (_OFF) {                                 \
      FLVL &= ~Bit                                  \
      FSET ()                                       \
      Notify (\_SB.TZ00, 0x81)                      \
    }                                               \
  }                                                 \
  Device (FAN##Index) {                             \
    Name (_HID, EISAID ("PNP0C0B"))                 \
    Name (_UID, Index)                              \
    Name (_PR0, Package () { PFN##Index })          \
  }

FAN_LEVEL (0, 1)
FAN_LEVEL (1, 2)
FAN_LEVEL (2, 4)
FAN_LEVEL (3, 8)

//
// A level that is on stays on until the temperature is 2 C below its trip point.
//
#define FAN_TRIP(Bit, C)                            \
  If (FLVL & Bit) {                                 \
    Return (DECI_KELVIN (C - 2))                    \
  }                                                 \
  Return (DECI_KELVIN (C))

#endif

//
// The TSADC for a temperature sensor driver. Windows does not poll _TMP: it reads
// the temperature from the device named by the thermal zone's _DSM (function 2).
//
Device (TADC) {
  Name (_HID, "RKCP3004")
  Name (_UID, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0xfec00000, 0x400)
  })
}

ThermalZone (TZ00) {
  Name (_DEP, Package () { \_SB.TADC })

  Method (_DSM, 4, Serialized) {
    If (Arg0 == ToUUID ("14d399cd-7a27-4b18-8fb4-7cb7b9f4e500")) {   // Microsoft thermal extensions
      If (Arg2 == 0) {
        Return (Buffer () { 0x05 })   // functions 0 and 2
      }
      If (Arg2 == 2) {
        Return ("\\_SB.TADC")         // temperature sensor device
      }
    }
    Return (Buffer () { 0 })
  }

  Method (_TMP) {
    Return (TCVT (TSD0))
  }
  Name (_TZP, 10)                 // poll every second
  Method (_CRT) {
    Return (DECI_KELVIN (115))
  }
#ifdef BOARD_FAN_PWM_BASE
  Method (_AC0) { FAN_TRIP (8, 85) }
  Method (_AC1) { FAN_TRIP (4, 77) }
  Method (_AC2) { FAN_TRIP (2, 68) }
  Method (_AC3) { FAN_TRIP (1, 60) }
  Name (_AL0, Package () { FAN3 })
  Name (_AL1, Package () { FAN2 })
  Name (_AL2, Package () { FAN1 })
  Name (_AL3, Package () { FAN0 })
#endif
}
