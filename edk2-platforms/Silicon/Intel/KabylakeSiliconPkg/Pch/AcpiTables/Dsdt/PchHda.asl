/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External(\_SB.PCI0.HDAS.PS0X, MethodObj)
External(\_SB.PCI0.HDAS.PS3X, MethodObj)
External(\_SB.PCI0.HDAS.PPMS, MethodObj)
External(HIDW, MethodObj)
External(HIWC, MethodObj)

//
// High Definition Audio Controller - Device 31, Function 3
//
Device(HDAS)
{
  Name(_ADR, 0x001F0003)

  //
  // Define a Memory Region that will allow access to the HDA PCI Configuration Space
  //
  OperationRegion(HDAR, PCI_Config, 0x00, 0x100)
  Field(HDAR,WordAcc,NoLock,Preserve) {
    VDID,32       // 0x00, VID DID
  }
  //
  // Byte access for PMCS field to avoid race condition on device D-state
  //
  Field(HDAR,ByteAcc,NoLock,Preserve) {
    Offset(R_PCH_HDA_PCS),   // 0x54, Power Management Control and Status Register
        ,8,
    PMEE,1,         // PME Enable
        ,6,
    PMES,1          // PME Status
  }

  Name(_S0W, 3) // Device can wake itself from D3 in S0

  Method(_DSW, 3) { Store(Arg0, PMEE) } // Device wake enable

  Method(_PRW, 0) { Return(GPRW(0x6D, 4)) }  // Can wakeup from S4 state

  // GPE handler for HDA, this is part of _Lxx handler for bus 0 PME
  Method(GPEH) {
    If(LEqual(VDID, 0xFFFFFFFF)) {
      Return()
    }

    If(LAnd(PMEE, PMES)) {
      ADBG("HDAS GPEH")
      Store(1, PMES) // clear PME Status
      Notify(HDAS, 0x02)
    }
  }

  // D0 Method for HD-A Controller
  Method(_PS0, 0, Serialized)
  {


    //
    // Call platform HDAS PS0 method if present
    //
    If(CondRefOf(\_SB.PCI0.HDAS.PS0X)) {
      \_SB.PCI0.HDAS.PS0X()
    }
  }

  // D3 Method for HD-A Controller
  Method(_PS3, 0, Serialized)
  {


    //
    // Call platform HDAS PS3 method if present
    //
    If(CondRefOf(\_SB.PCI0.HDAS.PS3X)) {
      \_SB.PCI0.HDAS.PS3X()
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

  }

  Method(_DSM, 0x4, NotSerialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
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

          If(CondRefOf(\_SB.PCI0.HDAS.PPMS)) {
            // Call platform method PPMS to check if 3rd Party IP module with given GUID (Arg3) is supported
            Return(\_SB.PCI0.HDAS.PPMS (Arg3))
          }

          Return(0) // Is not supported
        }

        Default {
          // Function not supported (Arg2)
          ADBG("_DSM Fun NOK")
          Return(Buffer(One) { 0x00 })
        }
      } // Switch(Arg2) End
    } // If(Arg0, UUID) End

    //-------------------------------------------
    //  HID Wake up Event solution
    //-------------------------------------------
    If(CondRefOf(HIWC)) {
      If(HIWC(Arg0)) {
        If(CondRefOf(HIDW)) {
          Return (HIDW(Arg0, Arg1, Arg2, Arg3))
        }
      }
    }

    // UUID not supported (Arg0)
    ADBG("_DSM UUID NOK")
    Return(Buffer() {0})
  } // _DSM End
} // end "High Definition Audio Controller"
