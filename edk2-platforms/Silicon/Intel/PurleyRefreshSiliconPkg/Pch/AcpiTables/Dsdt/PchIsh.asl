/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(\_SB.PCI0) {
  //
  // Integrated Sensor Hub (PCI Mode) - Device 19, Function 0
  //
  Device(ISHD) {
    Name(_ADR, 0x00130000)

    Method(_DSM, 0x4, NotSerialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }
      //Fix warning: not all control paths return a value
      Return(0)
    } // End _DSM
  } // Device(ISHD)
}