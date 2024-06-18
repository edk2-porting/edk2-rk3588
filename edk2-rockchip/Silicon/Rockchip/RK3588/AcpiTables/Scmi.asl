/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

#define SCMI_SHARED_MEM_BASE  FixedPcdGet32(PcdRkMtlMailBoxBase)
#define SCMI_SHARED_MEM_SIZE  FixedPcdGet32(PcdRkMtlMailBoxSize)

// Mailbox 0, channel 0
#define SCMI_DOORBELL_BASE    0xfec60030
#define SCMI_DOORBELL_SIZE    0x8

#define SMT_MESSAGE_TYPE_COMMAND   0

#define SMT_CHANNEL_STATUS_ERROR   (0x1 << 1)
#define SMT_CHANNEL_STATUS_FREE    (0x1 << 0)
#define SMT_CHANNEL_STATUS_BUSY    0

#define SMT_CHANNEL_FLAGS_INTERRUPT   (0x1 << 0)
#define SMT_CHANNEL_FLAGS_POLL        0

#define SMT_POLL_TIMEOUT_US   20000

#define SCMI_STATUS_SUCCESS   0

Device (SCMI) {
  Name (_HID, "PNP0C02")
  Name (_UID, 0)

  Method (_CRS, 0, Serialized) {
    Name (RBUF, ResourceTemplate () {
      Memory32Fixed (ReadWrite, SCMI_SHARED_MEM_BASE, SCMI_SHARED_MEM_SIZE)
    })
    Return (RBUF)
  }

  //
  // CLRG - Clock Rate Get
  //
  // Arguments:
  //   Arg0: Clock ID
  //
  // Return:
  //   Package of two integers: Status, Clock rate
  //   Values following Status are only valid on success.
  //
  Method (CLRG, 1, Serialized) {
    Name(PBUF, Buffer (4) { })          // Parameters
    CreateDWordField (PBUF, 0, PCID)    // Clock ID
    PCID = Arg0

    Local0 = SMT (0x14, 0x6, PBUF)      // Response
    CreateDWordField (Local0, 0, RSTA)  // Status
    CreateDWordField (Local0, 4, RRTL)  // Rate low part
    CreateDWordField (Local0, 8, RRTH)  // Rate high part

    // Prepare return package
    Local1 = Package (2) { 0, 0 }
    Local1[0] = RSTA

    If (RSTA == SCMI_STATUS_SUCCESS) {
      Local1[1] = (RRTH << 32) | RRTL
    }

    Return (Local1)
  }

  //
  // CLRS - Clock Rate Set
  //
  // Arguments:
  //   Arg0: Clock ID
  //   Arg1: Clock rate
  //
  // Return:
  //   Package of one integer: Status
  //
  Method (CLRS, 2, Serialized) {
    Name(PBUF, Buffer (16) { })         // Parameters
    CreateDWordField (PBUF, 0, PFLG)    // Flags
    CreateDWordField (PBUF, 4, PCID)    // Clock ID
    CreateDWordField (PBUF, 8, PRTL)    // Rate low part
    CreateDWordField (PBUF, 12, PRTH)   // Rate high part
    PFLG = 0
    PCID = Arg0
    PRTL = Arg1
    PRTH = Arg1 >> 32

    Local0 = SMT (0x14, 0x5, PBUF)      // Response
    CreateDWordField (Local0, 0, RSTA)  // Status

    // Prepare return package
    Local1 = Package (1) { 0 }
    Local1[0] = RSTA

    Return (Local1)
  }

  //
  // CLCS - Clock Config Set
  //
  // Arguments:
  //   Arg0: Clock ID
  //   Arg1: Attributes (1 -> enable, 0 -> disable)
  //
  // Return:
  //   Package of one integer: Status
  //
  Method (CLCS, 2, Serialized) {
    Name(PBUF, Buffer (8) { })          // Parameters
    CreateDWordField (PBUF, 0, PCID)    // Clock ID
    CreateDWordField (PBUF, 4, PATR)    // Attributes
    PCID = Arg0
    PATR = Arg1

    Local0 = SMT (0x14, 0x7, PBUF)      // Response
    CreateDWordField (Local0, 0, RSTA)  // Status

    // Prepare return package
    Local1 = Package (1) { 0 }
    Local1[0] = RSTA

    Return (Local1)
  }

  //
  // VDLG - Voltage Domain Level Get
  //
  // Arguments:
  //   Arg0: Domain ID
  //
  // Return:
  //   Package of two integers: Status, Voltage level
  //   Values following Status are only valid on success.
  //
  Method (VDLG, 1, Serialized) {
    Name(PBUF, Buffer (4) { })          // Parameters
    CreateDWordField (PBUF, 0, PDID)    // Domain ID
    PDID = Arg0

    Local0 = SMT (0x17, 0x8, PBUF)      // Response
    CreateDWordField (Local0, 0, RSTA)  // Status
    CreateDWordField (Local0, 4, RVOL)  // Voltage level

    // Prepare return package
    Local1 = Package (2) { 0, 0 }
    Local1[0] = RSTA

    If (RSTA == SCMI_STATUS_SUCCESS) {
      Local1[1] = RVOL
    }

    Return (Local1)
  }

  //
  // VDLS - Voltage Domain Level Set
  //
  // Arguments:
  //   Arg0: Domain ID
  //   Arg1: Voltage level
  //
  // Return:
  //   Package of one integer: Status
  //
  Method (VDLS, 2, Serialized) {
    Name(PBUF, Buffer (12) { })         // Parameters
    CreateDWordField (PBUF, 0, PDID)    // Domain ID
    CreateDWordField (PBUF, 4, PFLG)    // Flags
    CreateDWordField (PBUF, 8, PVOL)    // Voltage level
    PDID = Arg0
    PFLG = 0
    PVOL = Arg1

    Local0 = SMT (0x17, 0x7, PBUF)      // Response
    CreateDWordField (Local0, 0, RSTA)  // Status

    // Prepare return package
    Local1 = Package (1) { 0 }
    Local1[0] = RSTA

    Return (Local1)
  }

  OperationRegion (SHM, SystemMemory, SCMI_SHARED_MEM_BASE, SCMI_SHARED_MEM_SIZE)
  Field (SHM, ByteAcc, NoLock, Preserve) {
        , 32,   // Reserved
    SCHS, 32,   // Channel status
        , 64,   // Reserved
    SCHF, 32,   // Channel flags
    SLEN, 32,   // Length
    SMSH, 32,   // Message header
    SPLD, 736   // Payload
  }

  OperationRegion (DBEL, SystemMemory, SCMI_DOORBELL_BASE, SCMI_DOORBELL_SIZE)
  Field (DBEL, DWordAcc, NoLock, Preserve) {
    DCMD, 32,   // Command
    DDAT, 32    // Data
  }

  Method (FREE, 0, Serialized) {
    For (Local0 = SMT_POLL_TIMEOUT_US, Local0 > 0, Local0--) {
      If ((SCHS & SMT_CHANNEL_STATUS_FREE) != 0) {
        Return (1)
      }
      Sleep (1)
    }
    Return (0)
  }

  //
  // SMT - Shared Memory Transport
  //
  // Arguments:
  //   Arg0: SCMI Protocol ID
  //   Arg1: SCMI Message ID
  //   Arg2: SCMI Input Buffer
  //
  // Return:
  //   Buffer: SCMI Payload
  //
  Method (SMT, 3, Serialized) {
    // Initial return value (transport error)
    Local0 = Buffer (4) { 1 }

    // Wait for any previous message
    If (FREE () == 0) {
      Return (Local0)
    }

    // Fill in the message header
    Name (MSGH, Buffer (4) { })
    CreateField (MSGH, 10, 8, PRTI) // Protocol identifier
    CreateField (MSGH, 8, 2, MSGT)  // Message type
    CreateField (MSGH, 0, 8, MSGI)  // Message identifier
    PRTI = Arg0
    MSGT = SMT_MESSAGE_TYPE_COMMAND
    MSGI = Arg1
    SMSH = MSGH

    SCHS = SMT_CHANNEL_STATUS_BUSY
    SCHF = SMT_CHANNEL_FLAGS_POLL
    SLEN = 4 + SizeOf (Arg2)        // Message header + Payload
    SPLD = Arg2

    // Ring the doorbell
    DCMD = 0
    DDAT = 0

    // Wait for completion
    If ((FREE () == 0) || ((SCHS & SMT_CHANNEL_STATUS_ERROR) != 0)) {
      Return (Local0)
    }

    Local0 = SPLD
    Return (Local0)
  }
}
