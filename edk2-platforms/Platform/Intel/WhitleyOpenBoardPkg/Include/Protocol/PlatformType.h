/** @file
  This file defines platform policies for Platform Type.

  @copyright
  Copyright 1996 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_TYPE_H_
#define _PLATFORM_TYPE_H_

#include <Guid/PlatformInfo.h>

typedef struct _EFI_PLATFORM_TYPE_PROTOCOL {
  UINT8                       SystemUuid[16];     // 16 bytes
  UINT32                      Signature;          // "$PIT" 0x54495024
  UINT32                      Size;               // Size of the table
  UINT16                      Revision;           // Revision of the table
  UINT16                      Type;               // Platform Type
  UINT32                      CpuType;            // Cpu Type
  UINT8                       CpuStepping;        // Cpu Stepping
  UINT32                      TypeRevisionId;     // Board Revision ID
  UINT16                      IioSku;
  UINT16                      IioRevision;
  UINT16                      PchSku;
  UINT16                      PchRevision;
  UINT16                      PchType;            // Retrive PCH SKU type installed
  BOOLEAN                     ExtendedInfoValid;  // If TRUE then below fields are Valid
  UINT8                       Checksum;           // Checksum minus SystemUuid is valid in DXE only.
  UINT64                      TypeStringPtr;
  UINT64                      IioStringPtr;
  UINT64                      PchStringPtr;
  EFI_PLATFORM_PCI_DATA       PciData;
  EFI_PLATFORM_CPU_DATA       CpuData;
  EFI_PLATFORM_MEM_DATA       MemData;
  EFI_PLATFORM_SYS_DATA       SysData;
  EFI_PLATFORM_PCH_DATA       PchData;
  UINT8                       IioRiserId;
  UINT8                       BoardId;
  UINT8                       PcieRiser1Type;
  UINT8                       PcieRiser2Type;
  UINT8                       Emulation;         // 100b = Simics
} EFI_PLATFORM_TYPE_PROTOCOL;

extern EFI_GUID gEfiPlatformTypeProtocolGuid;

#endif
