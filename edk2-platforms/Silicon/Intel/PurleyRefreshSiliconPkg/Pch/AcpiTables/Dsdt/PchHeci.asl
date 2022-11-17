/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(\_SB.PCI0) {
  //
  // Management Engine Interface 1 - Device 22, Function 0
  //
  Device(HECI) {
    Name(_ADR, 0x00160000)

    Method(_DSM, 0x4, NotSerialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }
      //Fix warning: not all control paths return a value
      Return(0)
    } // End _DSM
  } // Device(HECI)
}

