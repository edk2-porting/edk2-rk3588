/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


// ITSS
// Define the needed ITSS registers used by ASL on Interrupt
// mapping.

scope(\_SB){
      OperationRegion(ITSS, SystemMemory, 0xfdc43100, 0x208)
      Field(ITSS, ByteAcc, NoLock, Preserve)
      {
        PARC, 8,
        PBRC, 8,
        PCRC, 8,
        PDRC, 8,
        PERC, 8,
        PFRC, 8,
        PGRC, 8,
        PHRC, 8,
       Offset(0x200),   // Offset 3300h ITSSPRC - ITSS Power Reduction Control
            , 1,
            , 1,
        SCGE, 1,        // ITSSPRC[2]: 8254 Static Clock Gating Enable (8254CGE)

      }
}


