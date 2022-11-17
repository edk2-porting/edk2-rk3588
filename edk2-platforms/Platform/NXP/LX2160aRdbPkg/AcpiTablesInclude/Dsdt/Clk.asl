/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  Device(PCLK) {
    Name(_HID, "NXP0017")
    Name(CLK, 0)
    Name(AVBL, 0)
    OperationRegion(RCWS, SystemMemory, DCFG_BASE, DCFG_LEN)
    Method(_REG,2) {
      if (Arg0 == "RCWS") {
        Store(Arg1, AVBL)
      }
    }
    Field (RCWS, ByteAcc, NoLock, Preserve) {
      offset(0x100),
      PCFG, 2,
      PRAT, 6,
      offset(0x124),
      RESV, 4,
      SFRQ, 10
    }

    Method(_INI, 0, NotSerialized) {
      Store(SFRQ, Local0)
      Multiply(Local0, 500000, Local0)
      Multiply(Local0, PRAT, Local0)
      Divide(Local0, 6, , Local0)
      Store(Local0, CLK)
    }
  }
} // end of device PCLK
