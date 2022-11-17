/*++

  Copyright (c) 2004  - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  EfiRegTableLib.h

Abstract:

  Definitions and macros for building register tables for chipset
  initialization..

  Components linking this lib must include PciRootBridgeIo and
  BootScriptSave protocols in their DPX.



--*/

#ifndef EFI_REG_TABLE_H
#define EFI_REG_TABLE_H


#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Protocol/PciRootBridgeIo.h>


#define OPCODE_BASE(OpCode)       ((UINT8)((OpCode) & 0xFF))
#define OPCODE_FLAGS(OpCode)      ((UINT8)(((OpCode) >> 8) & 0xFF))
#define OPCODE_EXTRA_DATA(OpCode) ((UINT16)((OpCode) >> 16))

//
// RegTable Base OpCodes
//
#define OP_TERMINATE_TABLE                0
#define OP_MEM_WRITE                      1
#define OP_MEM_READ_MODIFY_WRITE          2
#define OP_IO_WRITE                       3
#define OP_IO_READ_MODIFY_WRITE           4
#define OP_PCI_WRITE                      5
#define OP_PCI_READ_MODIFY_WRITE          6
#define OP_STALL                          7

//
// RegTable OpCode Flags
//
#define OPCODE_FLAG_S3SAVE                1


#define TERMINATE_TABLE { (UINT32) OP_TERMINATE_TABLE, (UINT32) 0, (UINT32) 0 }


//
// REG_TABLE_ENTRY_PCI_WRITE encodes the width in the upper bits of the OpCode
// as one of the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH values
//
typedef struct {
  UINT32                                OpCode;
  UINT32                                PciAddress;
  UINT32                                Data;
} EFI_REG_TABLE_PCI_WRITE;

#define PCI_WRITE(Bus, Dev, Fnc, Reg, Width, Data, S3Flag)                    \
  {                                                                           \
    (UINT32) (OP_PCI_WRITE | ((S3Flag) << 8) | ((Width) << 16)),              \
    (UINT32) (EFI_PCI_ADDRESS ((Bus), (Dev), (Fnc), (Reg))),                  \
    (UINT32) (Data),                                                          \
    (UINT32) (0)                                                              \
  }

typedef struct {
  UINT32                                OpCode;
  UINT32                                MemAddress;
  UINT32                                Data;
} EFI_REG_TABLE_MEM_WRITE;

typedef struct {
  UINT32                                OpCode;
  UINT32                                PciAddress;
  UINT32                                OrMask;
  UINT32                                AndMask;
} EFI_REG_TABLE_PCI_READ_MODIFY_WRITE;

#define PCI_READ_MODIFY_WRITE(Bus, Dev, Fnc, Reg, Width, OrMask, AndMask, S3Flag)  \
  {                                                                           \
    (UINT32) (OP_PCI_READ_MODIFY_WRITE | ((S3Flag) << 8) | ((Width) << 16)),  \
    (UINT32) (EFI_PCI_ADDRESS ((Bus), (Dev), (Fnc), (Reg))),                  \
    (UINT32) (OrMask),                                                        \
    (UINT32) (AndMask)                                                        \
  }

typedef struct {
  UINT32                                OpCode;
  UINT32                                MemAddress;
  UINT32                                OrMask;
  UINT32                                AndMask;
} EFI_REG_TABLE_MEM_READ_MODIFY_WRITE;

#define MEM_READ_MODIFY_WRITE(Address, Width, OrMask, AndMask, S3Flag)  \
  {                                                                           \
    (UINT32) (OP_MEM_READ_MODIFY_WRITE | ((S3Flag) << 8) | ((Width) << 16)),  \
    (UINT32) (Address),                  \
    (UINT32) (OrMask),                                                        \
    (UINT32) (AndMask)                                                        \
  }

typedef struct {
  UINT32                                OpCode;
  UINT32                                Field2;
  UINT32                                Field3;
  UINT32                                Field4;
} EFI_REG_TABLE_GENERIC;

typedef union {
  EFI_REG_TABLE_GENERIC                 Generic;
  EFI_REG_TABLE_PCI_WRITE               PciWrite;
  EFI_REG_TABLE_PCI_READ_MODIFY_WRITE   PciReadModifyWrite;
  EFI_REG_TABLE_MEM_READ_MODIFY_WRITE   MemReadModifyWrite;
} EFI_REG_TABLE;

/**
  Processes register table assuming which may contain PCI, IO, MEM, and STALL
  entries.

  No parameter checking is done so the caller must be careful about omitting
  values for PciRootBridgeIo parameters.  If the regtable does
  not contain any PCI accesses, it is safe to omit the PciRootBridgeIo (supply
  NULL).

  The RegTableEntry parameter is not checked, but is required.

  gBS is assumed to have been defined and is used when processing stalls.

  The function processes each entry sequentially until an OP_TERMINATE_TABLE
  entry is encountered.

  @param[in] RegTableEntry    A pointer to the register table to process

  @param[in] PciRootBridgeIo  A pointer to the instance of PciRootBridgeIo that is used
                              when processing PCI table entries

  @retval Nothing.

**/
VOID
ProcessRegTablePci (
  EFI_REG_TABLE                   * RegTableEntry,
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL * PciRootBridgeIo
  );

#endif
