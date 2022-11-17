/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// Bit Difinitions
//
#ifndef BIT0
#define BIT0                     0x0001
#define BIT1                     0x0002
#define BIT2                     0x0004
#define BIT3                     0x0008
#define BIT4                     0x0010
#define BIT5                     0x0020
#define BIT6                     0x0040
#define BIT7                     0x0080
#define BIT8                     0x0100
#define BIT9                     0x0200
#endif  //BIT0

//
// High Definition Audio Controller - Device 31, Function 3
//
Scope(\_SB.PC00.CAVS) {

#ifndef  BIT0
#define  BIT0     0x00000001
#endif
#ifndef  BIT1
#define  BIT1     0x00000002
#endif
#ifndef  BIT2
#define  BIT2     0x00000004
#endif
#ifndef  BIT3
#define  BIT3     0x00000008
#endif
#ifndef  BIT4
#define  BIT4     0x00000010
#endif
#ifndef  BIT5
#define  BIT5     0x00000020
#endif
#ifndef  BIT6
#define  BIT6     0x00000040
#endif
#ifndef  BIT7
#define  BIT7     0x00000080
#endif
#ifndef  BIT8
#define  BIT8     0x00000100
#endif
#ifndef  BIT9
#define  BIT9     0x00000200
#endif
#ifndef  BIT10
#define  BIT10    0x00000400
#endif
#ifndef  BIT11
#define  BIT11    0x00000800
#endif
#ifndef  BIT12
#define  BIT12    0x00001000
#endif
#ifndef  BIT13
#define  BIT13    0x00002000
#endif
#ifndef  BIT14
#define  BIT14    0x00004000
#endif
#ifndef  BIT15
#define  BIT15    0x00008000
#endif
#ifndef  BIT16
#define  BIT16    0x00010000
#endif
#ifndef  BIT17
#define  BIT17    0x00020000
#endif
#ifndef  BIT18
#define  BIT18    0x00040000
#endif
#ifndef  BIT19
#define  BIT19    0x00080000
#endif
#ifndef  BIT20
#define  BIT20    0x00100000
#endif
#ifndef  BIT21
#define  BIT21    0x00200000
#endif
#ifndef  BIT22
#define  BIT22    0x00400000
#endif
#ifndef  BIT23
#define  BIT23    0x00800000
#endif
#ifndef  BIT24
#define  BIT24    0x01000000
#endif
#ifndef  BIT25
#define  BIT25    0x02000000
#endif
#ifndef  BIT26
#define  BIT26    0x04000000
#endif
#ifndef  BIT27
#define  BIT27    0x08000000
#endif
#ifndef  BIT28
#define  BIT28    0x10000000
#endif
#ifndef  BIT29
#define  BIT29    0x20000000
#endif
#ifndef  BIT30
#define  BIT30    0x40000000
#endif
#ifndef  BIT31
#define  BIT31    0x80000000
#endif

  //
  // Define a Memory Region that will allow access to the HDA PCI Configuration Space
  //
  OperationRegion(HDAR, PCI_Config, 0x00, 0x100)
  Field(HDAR,WordAcc,NoLock,Preserve) {
    VDID,32,        // 0x00, VID DID
    Offset(0x48),   // 0x48, CGCTL - Clock Gating Control
        ,6,
    MBCG,1,         // MISCBDCGE [BIT6]
    Offset(0x54),   // 0x54, Power Management Control and Status Register
        ,8,
    PMEE,1,
        ,6,
    PMES,1          // PME Status
  }

  Name(_S0W, 3) // Device can wake itself from D3 in S0

  Method(_DSW, 3) { Store(Arg0, PMEE) } // Device wake enable


  Method(_PRW, 0) { Return(GPRW(0x0D, 4)) }  // Can wakeup from S4 state

  // GPE handler for HDA, this is part of _Lxx handler for bus 0 PME
  Method(GPEH) {
    If(LEqual(VDID, 0xFFFFFFFF)) {
      Return()
    }

    If(LAnd(PMEE, PMES)) {
      ADBG("HDAS GPEH")
      Store(1, PMES) // clear PME Status
      Notify(CAVS, 0x02)
    }
  }

  // NHLT Table memory descriptor, returned from _DSM
  Name(NBUF, ResourceTemplate () {
    // NHLT table address (_MIN = NHLT 64bit pointer, _MAX = _MIN + _LEN - 1) and length (_LEN)
    QWordMemory (ResourceConsumer, , MinNotFixed, MaxNotFixed, NonCacheable, ReadOnly,
                0x1,                // AddressGranularity
                0x0000000000000000, // AddressMinimum _MIN
                0x0000000000000000, // AddressMaximum _MAX
                0x0,
                0x0,                // RangeLength _LEN
                , , NHLT, AddressRangeACPI,)
  })

  Method(AUWA,0,Serialized)
  {
    If(LEqual(PCHS, 1)) {
      If(LEqual(\_SB.PC00.LPC0.CRID, 0x0)) { Return (1) } // Apply to SPT-H A0 stepping (RevID = 0x0)
    } else {
    If(LEqual(\_SB.PC00.LPC0.CRID, 0x0)) { Return (1) } // Apply to SPT-LP A0 stepping (RevID = 0x0)
    If(LEqual(\_SB.PC00.LPC0.CRID, 0x1)) { Return (1) } // Apply to SPT-LP A1 stepping (RevID = 0x1)
    If(LEqual(\_SB.PC00.LPC0.CRID, 0x9)) { Return (1) } // Apply to SPT-LP A2 stepping (RevID = 0x9)
    }
    Return (0)
  }

  Method(_INI) {
    // Update resource according to NVS
    ADBG("HDAS _INI")

    // Set NHLT base address and length
    CreateQWordField(NBUF, ^NHLT._MIN, NBAS)
    CreateQWordField(NBUF, ^NHLT._MAX, NMAS)
    CreateQWordField(NBUF, ^NHLT._LEN, NLEN)
    Store(NHLA, NBAS)
    Add(NHLA, Subtract(NHLL, 1), NMAS)
    Store(NHLL, NLEN)

    If(LEqual(AUWA(), 1)) {
      Store(0, \_SB.PC00.CAVS.MBCG)
    }
  }

  Method(_DSM, 0x4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
    // Arg0 - UUID: A69F886E-6CEB-4594-A41F-7B5DCE24C553 (Buffer)
    // Arg1 - Revision ID: 0x01 (Integer)
    // Arg2 - Function Index: 0x0 - 0x3 (Integer) - See below for details.
    // Arg3 - Depends on Function Index - See below for details.
    // Return - Depends on Function Index - See below for details.

    ADBG("HDAS _DSM")

    if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }

    // Verify UUID
    If (LEqual(Arg0, ToUUID ("A69F886E-6CEB-4594-A41F-7B5DCE24C553"))) {

          Switch(ToInteger(Arg2)) {

            // Function 0: Function Support Query
            // Arg2 - Function Index: 0x00 (Integer)
            // Arg3: Unused
            // Return: Bitmask of functions supported. (Buffer)
            Case(0) {
              // Supports function 0 - 3
              Return(Buffer(One) { 0x0F })
            }

            // Function 1: Query Non HD Audio Descriptor Table
            //                    Used by the Intel Offload Engine Driver to discover the
            //                    non HD Audio devices supported by the Audio DSP.
            // Arg2 - Function Index: 0x01 (Integer)
            // Arg3 - Unused
            // Return - ACPI Table describing the non HD Audio links and devices supported by the ADSP (ResourceBuffer)
            Case(1) {
              ADBG("_DSM Fun 1 NHLT")
              // NBUF - Memory Resource Descriptor buffer with address and length of NHLT
              Return(NBUF)
            }

            // Function 2: Query Feature Mask
            //                    Used by the Intel Offload Engine Driver to retrieve a bitmask
            //                    of features allowable on this platform.
            // Arg2 - Function Index: 0x02 (Integer)
            // Arg3: Unused
            // Return: Bitmask of supported features.
            Case (2) {
              ADBG("_DSM Fun 2 FMSK")
              // Bit 0 == '1', WoV is supported, Bit 0 == '0', WoV not supported
              // Bit 1 == '1', BT Sideband is supported, Bit 1 == '0', BT not supported
              // Bit 2 == '1', codec based VAD support allowable
              // Bit 3 - 4 Reserved
              // Bit 5 == '1', BT Intel HFP SCO is supported
              // Bit 6 == '1', BT Intel A2DP is supported
              // Bit 7 == '1', DSP based speech pre-processing disabled
              // Bit 8 == '1', Windows Voice Activation, Bit 8 == '0', Intel Wake on Voice
              // Bit 9 - 31 Reserved, shall be set to '0'
              // ADFM - NVS AudioDSP Feature Bit Mask updated from PchPolicy
              Return(ADFM)
            }

            // Function 3: Query Pre/Post Processing Module Support
            //                    Used by the Intel Offload Engine Driver to determine if a
            //                    specified PP Module is allowed to be supported on this platform
            // Arg2 - Function Index: 0x03 (Integer)
            // Arg3 - UUID: Specifies the UUID of the PP module to check (Buffer)
            // Return - TRUE if PP Module supported, else FALSE.
            Case (3) {
              ADBG("_DSM Fun 3 PPMS")
              // ADPM - NVS AudioDSP Post-Processing Module Bit Mask updated from PchPolicy: HdaConfig->DspPpModuleMask

              //
              // Example (to be updated with real GUIDs of supported 3rd party IP):
              //
              // 3rd Party DSP Processing Module 1 placeholder (enabled by policy HdaConfig->DspPpModuleMask |= BIT0)
              // Check PP module with GUID AABBCCDD-EEFF-1122-3344-556677889900
              // If (LEqual(Arg3, ToUUID ("AABBCCDD-EEFF-1122-3344-556677889900"))){
              //   Return(And(ADPM, 0x1)) // DspPpModuleMask[BIT0] / ADPM[BIT0] set - supported 3rd Party Processing Module 1(return true)
              // }
              //
              // 3rd Party DSP Processing Module 5 placeholder (enabled by policy HdaConfig->DspPpModuleMask |= BIT5)
              // Check PP module with GUID 11111111-2222-3333-4444-AABBCCDDEEFF
              // If (LEqual(Arg3, ToUUID ("11111111-2222-3333-4444-AABBCCDDEEFF"))){
              //   Return(And(ADPM, 0x20)) // DspPpModuleMask[BIT5] / ADPM[BIT5] set - supported 3rd Party Processing Module 5(return true)
              // }
              //
              // Implement for all supported PP modules
              //
              Return(0) // Is not supported
            }

            Default {
              // Function not supported (Arg2)
              ADBG("_DSM Fun NOK")
              Return(Buffer(One) { 0x00 })
            }
          } // Switch(Arg2) End
    } // If(Arg0, UUID) End


    // UUID not supported (Arg0)
    ADBG("_DSM UUID NOK")
    //Fix warning: not all control paths return a value
    Return(0)
  } // _DSM End

} // end "High Definition Audio Controller"
