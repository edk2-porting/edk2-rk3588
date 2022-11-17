/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CPUCSRACCESS_PROTOCOL_H_
#define _CPUCSRACCESS_PROTOCOL_H_

//
// CPU CSR Access Protocol GUID
//
// {0067835F-9A50-433a-8CBB-852078197814}
#define EFI_CPU_CSR_ACCESS_GUID \
  { \
    0x67835f, 0x9a50, 0x433a, 0x8c, 0xbb, 0x85, 0x20, 0x78, 0x19, 0x78, 0x14 \
  }

//#define REG_ADDR( bus, dev, func, reg, size ) ((size << 28) + ((bus+2) << 20) + (dev << 15) + (func << 12) + reg)

typedef
UINT64
(EFIAPI *GET_CPU_CSR_ADDRESS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   Offset,
  IN OUT UINT8 *Size
  );

typedef
UINT32
(EFIAPI *READ_CPU_CSR) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   Offset
  );

typedef
VOID
(EFIAPI *WRITE_CPU_CSR) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   RegOffset,
  IN UINT32   Data
  );

typedef
UINT32
(EFIAPI *READ_MC_CPU_CSR) (
  IN UINT8    SocId,
  IN UINT8    McId,
  IN UINT32   Offset
  );

typedef
VOID
(EFIAPI *WRITE_MC_CPU_CSR) (
  IN UINT8    SocId,
  IN UINT8    McId,
  IN UINT32   RegOffset,
  IN UINT32   Data
  );

typedef
UINTN
(EFIAPI *GET_MC_CPU_ADDR) (
  IN UINT8    SocId,
  IN UINT8    McId,
  IN UINT32   RegOffset
  );

typedef
UINT32
(EFIAPI *READ_PCI_CSR) (
  IN UINT8    socket,
  IN UINT32   reg
  );

typedef
VOID
(EFIAPI *WRITE_PCI_CSR) (
  IN UINT8    socket,
  IN UINT32   reg,
  IN UINT32   data
  );

typedef
UINT32
(EFIAPI *GET_PCI_CSR_ADDR) (
  IN UINT8    socket,
  IN UINT32   reg
  );

typedef
VOID
(EFIAPI *UPDATE_CPU_CSR_ACCESS_VAR) (
  VOID
  );

typedef
UINT32
(EFIAPI *BIOS_2_PCODE_MAILBOX_WRITE) (
  IN UINT8  socket,
  IN UINT32 command,
  IN UINT32 data
  );

typedef
UINT64
(EFIAPI *BIOS_2_VCODE_MAILBOX_WRITE) (
  IN UINT8  socket,
  IN UINT32 command,
  IN UINT32 data
  );

typedef
VOID
(EFIAPI *BREAK_AT_CHECK_POINT) (
  IN UINT8    majorCode,
  IN UINT8    minorCode,
  IN UINT16   data
  );

typedef struct _EFI_CPU_CSR_ACCESS_PROTOCOL {
  GET_CPU_CSR_ADDRESS         GetCpuCsrAddress;
  READ_CPU_CSR                ReadCpuCsr;
  WRITE_CPU_CSR               WriteCpuCsr;
  BIOS_2_PCODE_MAILBOX_WRITE  Bios2PcodeMailBoxWrite;
  BIOS_2_VCODE_MAILBOX_WRITE  Bios2VcodeMailBoxWrite;
  READ_MC_CPU_CSR             ReadMcCpuCsr;
  WRITE_MC_CPU_CSR            WriteMcCpuCsr;
  GET_MC_CPU_ADDR             GetMcCpuCsrAddress;
  UPDATE_CPU_CSR_ACCESS_VAR   UpdateCpuCsrAccessVar;
  READ_PCI_CSR                ReadPciCsr;
  WRITE_PCI_CSR               WritePciCsr;
  GET_PCI_CSR_ADDR            GetPciCsrAddress;
  BREAK_AT_CHECK_POINT        BreakAtCheckpoint;
} EFI_CPU_CSR_ACCESS_PROTOCOL;

extern EFI_GUID         gEfiCpuCsrAccessGuid;

#endif
