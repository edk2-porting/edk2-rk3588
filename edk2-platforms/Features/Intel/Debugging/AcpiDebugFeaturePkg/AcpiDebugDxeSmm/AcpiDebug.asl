/** @file
  SSDT for the ACPI Debug feature.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock (
  "AcpiDebug.aml",
  "SSDT",
  2,
  "Intel ",
  "ADebTabl",
  0x1000
  )
{
  Scope(\)
  {
    //
    // These pointers are patched during POST.
    //
    Name (DPTR, 0x80000000) // Address of Acpi debug memory buffer, fixed up during POST
    Name (EPTR, 0x80000000) // End of Acpi debug memory buffer, fixed up during POST
    Name (CPTR, 0x80000000) // Current pointer used as an index into the buffer(starts after the Acpi Debug head), fixed up during POST

    //
    // Use a Mutex to prevent multiple calls from simutaneously writing to the same memory.
    //
    Mutex (MMUT, 0)

    //
    // Operational region for SMI port access
    //
    OperationRegion (ADBP, SystemIO, 0xB2, 2)
    Field (ADBP, ByteAcc, NoLock, Preserve)
    {
        B2PT, 8,
        B3PT, 8,
    }

    //
    // Write a string to a memory buffer
    //
    Method (MDBG, 1, Serialized)
    {
      OperationRegion (ADHD, SystemMemory, DPTR, 32) // Operation region for Acpi Debug buffer first 0x20 bytes
      Field (ADHD, ByteAcc, NoLock, Preserve)
      {
        Offset (0x0),
        ASIG, 128,      // 16 bytes is Signature
        Offset (0x10),
        ASIZ, 32,       // 4 bytes is buffer size
        ACHP, 32,       // 4 bytes is current head pointer, normally is DPTR + 0x20,
                        //   if there's SMM handler to print, then it's the starting of the info hasn't been printed yet.
        ACTP, 32,       // 4 bytes is current tail pointer, is the same as CPTR
        SMIN, 8,        // 1 byte of SMI Number for trigger callback
        WRAP, 8,        // 1 byte of wrap status
        SMMV, 8,        // 1 byte of SMM version status
        TRUN, 8         // 1 byte of truncate status
      }

      Store (Acquire (MMUT, 1000), Local0) // save Acquire result so we can check for Mutex acquired
      If (LEqual (Local0, Zero)) // check for Mutex acquired
      {
        OperationRegion (ABLK, SystemMemory, CPTR, 32) // Operation region to allow writes to ACPI debug buffer
        Field (ABLK, ByteAcc, NoLock, Preserve)
        {
          Offset (0x0),
          AAAA, 256 // 32 bytes is max size for string or data
        }
        ToHexString (Arg0, Local1) // convert argument to Hexadecimal String
        Store (0, TRUN)
        If (LGreaterEqual (SizeOf (Local1), 32))
        {
          Store (1, TRUN) // the input from ASL >= 32
        }
        Mid (Local1, 0, 31, AAAA) // extract the input to current buffer

        Add (CPTR, 32, CPTR) // advance current pointer to next string location in memory buffer
        If (LGreaterEqual (CPTR, EPTR) ) // check for end of 64kb Acpi debug buffer
        {
          Add (DPTR, 32, CPTR) // wrap around to beginning of buffer if the end has been reached
          Store (1, WRAP)
        }
        Store (CPTR, ACTP)

        If (SMMV)
        {
          //
          // Trigger the SMI to print
          //
          Store (SMIN, B2PT)
        }
        Release (MMUT)
      }

      Return (Local0) // return error code indicating whether Mutex was acquired
    }

  } // End Scope
} // End SSDT
