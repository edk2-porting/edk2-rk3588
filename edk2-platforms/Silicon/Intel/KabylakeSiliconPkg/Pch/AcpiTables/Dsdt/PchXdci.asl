/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External(\_SB.PCI0.LPCB.H_EC.XDAT, MethodObj)

Scope(\_SB.PCI0)
{
  Device(XDCI)
  {
    Name (_ADR, 0x00140001)  // _ADR: Address

    OperationRegion (OTGD, PCI_Config, 0x0, 0x100)
    Field (OTGD, DWordAcc, NoLock, Preserve)
    {
      Offset(0x0),
      DVID, 16,
      Offset(0x10),
      XDCB, 64
    }
    //
    // Byte access for PMCS field to avoid race condition on device D-state
    //
    Field (OTGD, ByteAcc, NoLock, Preserve)
    {
      Offset(R_PCH_XDCI_PMCSR), // 0x84, PM_CS - Power Management Control/Status
      D0I3, 2,      // PM_CS[1:0] PowerState
          , 6,
      PMEE, 1,      // bit 8, PME Enable
          , 6,
      PMES, 1       // bit 15, PME Status
    }

    Method(XDBA, 0)
    {
      Return(And(^XDCB, 0xFFFFFFFFFFFFFF00))
    }

    //
    // Arg0: UUID = {732b85d5-b7a7-4a1b-9ba0-4bbd00ffd511}
    // Arg1: Revision ID = 1
    // Arg2: Function Index
    // Arg3: Argument
    //
    Method(_DSM,4,Serialized)
    {
      If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }

      If(LEqual(Arg0, ToUUID("732b85d5-b7a7-4a1b-9ba0-4bbd00ffd511"))){
        If(Lequal(Arg1, 1)){
          //
          // Set PMU Power State
          // Arg0: 0x00 PMU should enter the D0 power state.
          //       0x03 PMU should enter the D3 power state.
          // Arg1: 0x03 Enable PMU PME
          //       0x00 Clear PMU PME
          //
          Method (SPPS,2,Serialized) {
            OperationRegion(XDBW, SystemMemory, XDBA(), 0x110000)
            Field(XDBW, WordAcc, NoLock, Preserve)
            {
              Offset(R_PCH_XDCI_APBFC_U3PMU_CFG2), // 0x10F810
                  ,  8,
              U2CP,  2,     // USB2 core power state
              U3CP,  2,     // USB3 core power state
              Offset(R_PCH_XDCI_APBFC_U3PMU_CFG4), // 0x10F818
              PUPS,  2,     // PMU power state
                  ,  1,
              PURC,  1,     // Reset PMU core
                  , 12,
              Offset(R_PCH_XDCI_APBFC_U3PMU_CFG5), // 0x10F81C
                  ,  3,
              UXPE,  2,     // U2 PME EN / U3 PME EN
                  , 11,
            }

            // Local 1 is power state D0 or D3
            Store(Arg0, Local1)
            // Local 2 is Enable/Clear PMU PME
            Store(Arg1, Local2)

            If(LEqual(Local1, 0)){
              // Clear PMU PME
              // 0x10F81C BIT3: USB3 PME
              // 0x10F81C BIT4: USB2 PME
              Store(0, UXPE)
              // Wait for at least 100us, currently configured to 1000us
              Store(0, Local0)
              While(LLess(Local0, 10)){
                Stall(100)
                Increment(Local0)
              }
              // Set PMU to D0 by writing 0 to 0x10f818 Bit 1:0
              Store(0, PUPS)
              // Wait 200ms for PMU to enter D0
              // Confirm PMU being in D0 by checking 0x10f810 Bit 11:8 to be clear
              // 0x10f810 Bit 11:10 - Current power state of USB3 core
              // 0x10f810 Bit 9:8 - Current power state of USB2 core
              // both should be clear
              Store(0,Local0)
              While(LLess(Local0,2000)){
                Stall(100)
                If(LAnd(LEqual(U2CP,0),LEqual(U3CP,0))){
                  break
                }
                Increment(Local0)
              }
              If(LNotEqual(U2CP, 0)){
                // Show warning message
              }
              If(LNotEqual(U3CP, 0)){
                // Show warning message
              }
              Return(0)
            }

            If(LEqual(Local1, 3)){
              // PMU should be in D0 at this point
              // 0x10f810 Bit 11:10 - current power state of USB3 core
              // 0x10f810 Bit 9:8   - current power state of USB2 core
              // both should be clear
              If(LNotEqual(U2CP, 0)){
                // Show warning message
              }
              If(LNotEqual(U3CP, 0)){
                // Show warning message
              }
              // Set PMU to D3 by writing 3 to 0x10f818 bit 1:0
              Store(3, PUPS)
              // Wait 200ms for PMU to enter D3
              // Confirm PMU being in D3 by checking 0x10f810 Bit 11:8 to be set
              // 0x10f810 Bit 11:10 - Current power state of USB3 core
              // 0x10f810 Bit 9:8 - Current power state of USB2 core
              // both should be set
              Store(0,Local0)
              While(LLess(Local0,2000)){
                Stall(100)
                If(LAnd(LEqual(U2CP,3),LEqual(U3CP,3))){
                  break
                }
                Increment(Local0)
              }
              If(LNotEqual(U2CP, 3)){
                // Show warning message
              }
              If(LNotEqual(U3CP, 3)){
                // Show warning message
              }
              // Set/Clear PMU PME
              // 0x10F81C BIT3: USB3 PME
              // 0x10F81C BIT4: USB2 PME
              Store(Local2, UXPE)
              Return(0)
            }
            Return(0)
          }

          Switch(ToInteger(Arg2)) {
            Case(0){
              // Function 0: return Bit map to indicate Function 0,1,4,5,6,7 supported
              Return(Buffer(){0xF3})
            }
            Case(1){
              // Function 1: Attach/Detach and Port Detection Properties Method
              // This method is called by the USB function stack to set the power state of the PMU.
              //   Bit 0 as 1: to indicate Platform support for Attach/detach notify
              //   Bit 1 as 0:HW based charging indication
              Return(0x1)
            }
            Case(4){
              // Function 4: Set PMU Power State Method, clear PMU PME
              // Arg3: A package consisting of 1 ULONG value
              //   0x00 PMU should enter the D0 power state.
              //   0x03 PMU should enter the D3 power state.

              // Local 1 is power state D0 or D3
              Store(DerefOf(Index(Arg3,0)), Local1)

              // Set PMU to Dx state and clear PMU PME
              SPPS(Local1, 0)
            }
            Case(5){
              // Function 5: Attach Notification Enabled Method
              // This method is called by the USB function stack to indicate that it has enabled ACPI attach detach notifications.
              // In response the platform may issue an notification indicating the current attach/detach state of the controller.

              // If device is attached, notify XDCI with 0x80
              // If device is detached, notify XDCI with 0x81


              If(CondRefOf(\_SB.PCI0.LPCB.H_EC.XDAT)){
                If(LEqual(\_SB.PCI0.LPCB.H_EC.XDAT(), 1)){
                  Notify(\_SB.PCI0.XDCI,0x80)
                }Else{
                  Notify(\_SB.PCI0.XDCI,0x81)
                }
              }

              Return(0)
            }
            Case(6){
              // Function 6: XDCI Soft Reset Workaround
              // Return value indicate to OS softreset should be done by OS or not
              // Return value == 1: OS does softreset
              // Return value == 0: platform does softreset


              OperationRegion(XDBD, SystemMemory, XDBA(), 0x110000)
              Field(XDBD, DwordAcc, NoLock, Preserve)
              {
                Offset(0xC704),
                    , 30,
                CSFR,  1,
                    ,  1,
              }
              OperationRegion(XDW2, SystemMemory, XDBA(), 0x110000)
              Field(XDW2, WordAcc, NoLock, Preserve)
              {
                Offset(R_PCH_XDCI_APBFC_U3PMU_CFG6), // 0x10F820
                    , 13,
                OTHC,  1,     // OTG switched to host complete
              }

              If(LEqual(OTHC, 0)){
                Store(1, CSFR)
                // timeout is 100ms
                Store(0, Local0)
                While(LLess(Local0, 100)){
                  If(LEqual(CSFR, 0)){
                    Break
                  }
                  Sleep(1)
                }
              }
              Return(0)
            }
            Case(7){
              // Function 7: Get PMU Power State Method
              // Return:
              //   0: PMU is in D0 state
              //   3: PMU is in D3 state


              OperationRegion(XD22, SystemMemory, XDBA(), 0x110000)
              Field(XD22, WordAcc, NoLock, Preserve)
              {
                Offset(R_PCH_XDCI_APBFC_U3PMU_CFG4), // 0x10F818
                P2PS,  2,     // PMU power state
                    , 14,
              }
              Store(P2PS, Local0)
              Return(Local0)
            }
          }
        }
      }
      Return(Buffer() {0})
    }

    Name (_DDN, "SPT XHCI controller")  // _DDN: DOS Device Name
    Name (_STR, Unicode ("SPT XHCI controller"))  // _STR: Description String

    Method(_S0W, 0)
    { // PMEs can be generated from D3(hot)
      Return(3)
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If(LNotEqual(DVID, 0xFFFFFFFF)) {    // device not disabled
        Return (0xF)
      } Else{
        Return (0x0)
      }
    }

    // XDCI Wake Support
    Method(_PRW, 0)
    {
      Return(GPRW(0x6D,4)) // can wakeup from S4 state
    }

    Method(_DSW, 3)
    {
      Store(Arg0, PMEE)
    }

    Method(GPEH, 0, NotSerialized)
    {
      If(LEqual(DVID,0xFFFF)){
        Return()
      }
      Store(PMES, Local0)
      Store(1, PMES) // Clear PME Status
      If(LAnd(PMEE, Local0)){
        Notify(XDCI, 0x02)
      }
    }

  }
}
