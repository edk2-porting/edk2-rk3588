/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define TRACE_HUB_MASTER_NUM_ASL        32
#define TRACE_HUB_CHANNEL_NUM_ASL       22

//
// @note Only include register definition macros in ASL.
//       GCC will leak C function definitions in to ASL
//       code, causing compilation errors in some cases.
//
#include <PchReservedResources.h>
#include <Register/PchRegsTraceHub.h>

Scope(\){
  // Check if Trace Hub is enabled
  Method (THEN, 0, Serialized) {
    OperationRegion (THBA, SystemMemory, PCH_TRACE_HUB_SW_BASE_ADDRESS, 0x10)
    Field (THBA, DWordAcc, NoLock, Preserve) {
      Offset (0x00),
      DO00, 32,
    }
    Return (LNotEqual(DO00, 0xFFFFFFFF))
  }

  // Trace Hub debug address
  // This is internal helper runtine of THDS
  // Arg0 : Master number
  // Arg1 : Channel number
  Method (THDA, 2, Serialized) {
    // Local0 = PCH_TRACE_HUB_SW_BASE_ADDRESS + 0x40 * (V_PCH_TRACE_HUB_MTB_CHLCNT * (Master - V_PCH_TRACE_HUB_MTB_STHMSTR)) + 0x40 * Channel;
    Store (PCH_TRACE_HUB_SW_BASE_ADDRESS, Local0)
    Add (Local0, Multiply (0x40, Multiply (V_PCH_TRACE_HUB_MTB_CHLCNT, Subtract (Arg0, V_PCH_TRACE_HUB_MTB_STHMSTR))), Local0)
    Add (Local0, Multiply (0x40, Arg1), Local0)
    Return (Local0)
  }

  // String to raw data
  // This is internal helper runtine of THDS
  // Arg0 : string
  // Arg1 : Index
  // Arg2 : size
  Method (STRD, 3, Serialized) {
    If (LGreater (Add (Arg1, Arg2), SizeOf (Arg0))) {
      Return (0)
    }
    // Local0 is return value
    // Lccal1 is loop index
    // Local2 is char of string
    // Local3 is buffer of string
    ToBuffer (Arg0, Local3)
    Store (0, Local0)
    Store (0, Local1)
    While (LLess (Local1, Arg2)) {
      Store (DeRefOf (Index (Local3, Add (Arg1, Local1))), Local2)
      Add (Local0, ShiftLeft (Local2, Multiply (8, Local1)), Local0)
      Increment (Local1)
    }
    Return (Local0)
  }

  // Trace Hub debug string
  // Arg0 : debug string
  Method (THDS, 1, Serialized) {
    // TH check if enabled.
    If (LNot (THEN())) {
      Return
    }

    // Local0 is the length of string
    // Local1 is the debug base address
    Store (Sizeof (Arg0), Local0)
    Store (THDA (TRACE_HUB_MASTER_NUM_ASL, TRACE_HUB_CHANNEL_NUM_ASL), Local1)
    OperationRegion (THBA, SystemMemory, local1, 0x40)
    Field (THBA, QWordAcc, NoLock, Preserve) {
      Offset (0x00),
      QO00, 64,
    }
    Field (THBA, DWordAcc, NoLock, Preserve) {
      Offset (0x00),
      DO00, 32,
      Offset (0x10),
      DO10, 32,
      offset (0x30),
      DO30, 32,
    }
    Field (THBA, WordAcc, NoLock, Preserve) {
      Offset (0x00),
      WO00, 16,
    }
    Field (THBA, ByteAcc, NoLock, Preserve) {
      Offset (0x00),
      BO00, 8,
    }

    // time stamp
    Store (0x01000242, DO10)
    // length of string
    Store (Local0, WO00)
    // string
    Store (0, Local6)
    Store (Local0, Local7)
    while (LGreaterEqual(Local7, 8)) {
      Store (STRD (Arg0, Local6, 8), QO00)
      Add (Local6, 8, Local6)
      Subtract (Local7, 8, Local7)
    }
    If (LGreaterEqual(Local7, 4)) {
      Store (STRD (Arg0, Local6, 4), DO00)
      Add (Local6, 4, Local6)
      Subtract (Local7, 4, Local7)
    }
    If (LGreaterEqual(Local7, 2)) {
      Store (STRD (Arg0, Local6, 2), WO00)
      Add (Local6, 2, Local6)
      Subtract (Local7, 2, Local7)
    }
    If (LGreaterEqual(Local7, 1)) {
      Store (STRD (Arg0, Local6, 1), BO00)
      Add (Local6, 1, Local6)
      Subtract (Local7, 1, Local7)
    }
    // flag
    Store (0, DO30)
  }

  // Trace Hub debug Hex string
  // Arg0 : Integer, buffer
  Method (THDH, 1, Serialized) {
    THDS (ToHexString (Arg0))
  }

  // Trace Hub debug decimal string
  // Arg0 : Integer, buffer
  Method (THDD, 1, Serialized) {
    THDS (ToDecimalString (Arg0))
  }
}
