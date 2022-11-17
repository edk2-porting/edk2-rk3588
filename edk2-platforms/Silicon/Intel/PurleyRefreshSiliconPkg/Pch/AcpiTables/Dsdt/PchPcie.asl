/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  OperationRegion(PXCS,PCI_Config,0x00,0x480)
  Field(PXCS,AnyAcc, NoLock, Preserve)
  {
    Offset(0),
    VDID, 32,
    Offset(0x50), // LCTL - Link Control Register
    L0SE, 1,      // 0, L0s Entry Enabled
    , 3,
    LDIS, 1,
    , 3,
    Offset(0x52), // LSTS - Link Status Register
    , 13,
    LASX, 1,      // 0, Link Active Status
    Offset(0x5A), // SLSTS[7:0] - Slot Status Register
    ABPX, 1,      // 0, Attention Button Pressed
    , 2,
    PDCX, 1,      // 3, Presence Detect Changed
    , 2,
    PDSX, 1,      // 6, Presence Detect State
    , 1,
    Offset(0x60), // RSTS - Root Status Register
    , 16,
    PSPX, 1,      // 16,  PME Status
    Offset(0xA4),
    D3HT, 2,      // Power State
    Offset(0xD8), // MPC - Miscellaneous Port Configuration Register
    , 30,
    HPEX, 1,      // 30,  Hot Plug SCI Enable
    PMEX, 1,      // 31,  Power Management SCI Enable
    Offset(0xE2), // RPPGEN - Root Port Power Gating Enable
    , 2,
    L23E, 1,      // 2,   L23_Rdy Entry Request (L23ER)
    L23R, 1,       // 3,   L23_Rdy to Detect Transition (L23R2DT)
    Offset(0x324),
    , 3,
    LEDM, 1,       // PCIEDBG.DMIL1EDM
    Offset(0x420), // Offset 420h: PCIEPMECTL - PCIe PM Extension Control
    , 30,
    DPGE, 1,       // PCIEPMECTL[30]: Disabled, Detect and L23_Rdy State PHY Lane Power Gating Enable (DLSULPPGE):
  }
  Field(PXCS,AnyAcc, NoLock, WriteAsZeros)
  {
    Offset(0xDC), // SMSCS - SMI/SCI Status Register
    , 30,
    HPSX, 1,      // 30,  Hot Plug SCI Status
    PMSX, 1       // 31,  Power Management SCI Status
  }


  Name(LTRV, Package(){0,0,0,0})

  //
  // _DSM Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index (0 = Return Supported Functions)
  // Arg3: Package Parameters
  Method(_DSM, 4, Serialized) {
    //
    // Switch based on which unique function identifier was passed in
    //
    If (LEqual(Arg0, ToUUID ("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))) {
      //
      // _DSM Definitions for Latency Tolerance Reporting
      //
      // Arguments:
      // Arg0: UUID: E5C937D0-3553-4d7a-9117-EA4D19C3434D
      // Arg1: Revision ID: 2
      // Arg2: Function Index: 1, 4 or 6
      // Arg3: Empty Package
      //
      // Return:
      // A Package of four integers corresponding with the LTR encoding defined
      // in the PCI Express Base Specification, as follows:
      // Integer 0: Maximum Snoop Latency Scale
      // Integer 1: Maximum Snoop Latency Value
      // Integer 2: Maximum No-Snoop Latency Scale
      // Integer 3: Maximum No-Snoop Latency Value
      // These values correspond directly to the LTR Extended Capability Structure
      // fields described in the PCI Express Base Specification.
      //
      //
      // Switch by function index
      //
      Switch(ToInteger(Arg2)) {
        //
        // Function Index:0
        // Standard query - A bitmask of functions supported
        //
        Case (0) {
          Name(OPTS,Buffer(2){0,0})
          CreateBitField(OPTS,0,FUN0)
          CreateBitField(OPTS,4,FUN4)
          CreateBitField(OPTS,6,FUN6)
          CreateBitField(OPTS,8,FUN8)
          CreateBitField(OPTS,9,FUN9)

          if (LGreaterEqual(Arg1, 2)){ // test Arg1 for Revision ID: 2
            Store(1,FUN0)
            if (LTRE){
              Store(1,Fun6)
            }
            if (OBFF){
              Store(1,Fun4)
            }
            if(LEqual(ECR1,1)){
              if (LGreaterEqual(Arg1, 3)){ // test Arg1 for Revision ID: 3
                Store(1,Fun8)
                Store(1,Fun9)
              }
            }
          }
          Return (OPTS)
        }
        //
        // Function Index: 4
        //
        Case(4) {
          if (LGreaterEqual(Arg1, 2)){ // test Arg1 for Revision ID: 2
            if (OBFN){
              Return (Buffer () {0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0}) // OBFF capable, offset 4[08h]
            } else {
              Return (Buffer () {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0})
            }
          }
        }
        //
        // Function Index: 6
        // LTR Extended Capability Structure
        //
        Case(6) {
          if (LGreaterEqual(Arg1, 2)){ // test Arg1 for Revision ID: 2
            if (LTRN){
              if (LOr(LEqual(LMSL, 0),LEqual(LNSL, 0)))
              {
                if (LEqual (PCHS, SPTH)) {
                  Store (0x0846, LMSL)
                  Store (0x0846, LNSL)
                } elseif (LEqual (PCHS, SPTL)) {
                  Store (0x1003, LMSL)
                  Store (0x1003, LNSL)
                }
              }
              Store(And(ShiftRight(LMSL,10),7), Index(LTRV, 0))
              Store(And(LMSL,0x3FF), Index(LTRV, 1))
              Store(And(ShiftRight(LNSL,10),7), Index(LTRV, 2))
              Store(And(LNSL,0x3FF), Index(LTRV, 3))

              Return (LTRV)
            } else {
              Return (0)
            }
          }
        }
        Case(8) { //ECR ACPI additions for FW latency optimizations, DSM for Avoiding Power-On Reset Delay Duplication on Sx Resume
          if(LEqual(ECR1,1)){
            if (LGreaterEqual(Arg1, 3)) { // test Arg1 for Revision ID: 3
              return (1)
            }
          }
        }
        Case(9) { //ECR ACPI additions for FW latency optimizations, DSM for Specifying Device Readiness Durations
          if(LEqual(ECR1,1)){
            if (LGreaterEqual(Arg1, 3)) { // test Arg1 for Revision ID: 3
              return(Package(5){50000,Ones,Ones,50000,Ones})
            }
          }
        }
      } // End of switch(Arg2)
  } // End of if
    return (Buffer() {0x00})
  } // End of _DSM

  Device(PXSX)
  {
    Name(_ADR, 0x00000000)

    // NOTE:  Any PCIE Hot-Plug dependency for this port is
    // specific to the CRB.  Please modify the code based on
    // your platform requirements.
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
  }

  //
  // PCI_EXP_STS Handler for PCIE Root Port
  //
  Method(HPME,0,Serialized) {
    If(LAnd(LNotEqual(VDID,0xFFFFFFFF), LEqual(PMSX,1))) { //if port exists and has PME SCI Status set...
      Notify (PXSX, 0x2) //notify child device; this will cause its driver to clear PME_Status from device
      Store(1,PMSX) // clear rootport's PME SCI status
      Store(1,PSPX) // consume one pending PME notification to prevent it from blocking the queue
    }
  }

