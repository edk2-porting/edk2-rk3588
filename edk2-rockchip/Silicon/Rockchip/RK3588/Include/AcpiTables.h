/** @file
 *
 *  RK3588 defines for constructing ACPI tables
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2019, ARM Ltd. All rights reserved.
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __ACPITABLES_H__
#define __ACPITABLES_H__

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <Library/GpioLib.h>
#include <Library/Rk3588Pcie.h>

#define EFI_ACPI_OEM_ID  {'R','K','C','P',' ',' '}

#define EFI_ACPI_OEM_TABLE_ID  SIGNATURE_64 ('R','K','3','5','8','8',' ',' ')

#define EFI_ACPI_OEM_REVISION      0x00000000
#define EFI_ACPI_CREATOR_ID        SIGNATURE_32 ('E','D','K','2')
#define EFI_ACPI_CREATOR_REVISION  0x00000000
#define EFI_ACPI_VENDOR_ID         SIGNATURE_32 ('R','K','C','P')

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define ACPI_HEADER(Signature, Type, Revision)  {                 \
    Signature,                      /* UINT32  Signature */       \
    sizeof (Type),                  /* UINT32  Length */          \
    Revision,                       /* UINT8   Revision */        \
    0,                              /* UINT8   Checksum */        \
    EFI_ACPI_OEM_ID,                /* UINT8   OemId[6] */        \
    EFI_ACPI_OEM_TABLE_ID,          /* UINT64  OemTableId */      \
    EFI_ACPI_OEM_REVISION,          /* UINT32  OemRevision */     \
    EFI_ACPI_CREATOR_ID,            /* UINT32  CreatorId */       \
    EFI_ACPI_CREATOR_REVISION       /* UINT32  CreatorRevision */ \
  }

#define EFI_ACPI_6_0_GIC_REDISTRIBUTOR_INIT(RedisRegionAddr, RedisDiscLength) \
  {                                                                           \
    EFI_ACPI_6_0_GICR,                                                        \
    sizeof (EFI_ACPI_6_0_GICR_STRUCTURE),                                     \
    0,                                                                        \
    RedisRegionAddr,                                                          \
    RedisDiscLength                                                           \
  }

#define EFI_ACPI_6_0_GIC_ITS_FRAME_INIT(Id, PhysAddress)                      \
  {                                                                           \
    EFI_ACPI_6_0_GIC_ITS,                                                     \
    sizeof (EFI_ACPI_6_0_GIC_ITS_STRUCTURE),                                  \
    0,                                                                        \
    Id,                                                                       \
    PhysAddress,                                                              \
    0                                                                         \
  }

//
// Device resource helpers
//
#define QWORDMEMORY_BUF(Index, ResourceType)                    \
  QWordMemory (ResourceType,,                                   \
    MinFixed, MaxFixed, NonCacheable, ReadWrite,                \
    0x0, 0x0, 0x0, 0x0, 0x1,,, RB ## Index)

#define QWORDIO_BUF(Index, ResourceType)                        \
  QWordIO (ResourceType,                                        \
    MinFixed, MaxFixed, PosDecode, EntireRange,                 \
    0x0, 0x0, 0x0, 0x0, 0x1,,, RB ## Index)

#define DWORDMEMORY_BUF(Index, ResourceType)                    \
  DWordMemory (ResourceType,,                                   \
    MinFixed, MaxFixed, NonCacheable, ReadWrite,                \
    0x0, 0x0, 0x0, 0x0, 0x1,,, RB ## Index)

#define WORDBUSNUMBER_BUF(Index, ResourceType)                  \
  WordBusNumber (ResourceType,                                  \
    MinFixed, MaxFixed, PosDecode,                              \
    0x0, 0x0, 0x0, 0x0, 0x1,,, RB ## Index)

#define QWORD_SET(Index, Minimum, Length, Translation)          \
  CreateQWordField (RBUF, RB ## Index._MIN, MI ## Index)        \
  CreateQWordField (RBUF, RB ## Index._MAX, MA ## Index)        \
  CreateQWordField (RBUF, RB ## Index._TRA, TR ## Index)        \
  CreateQWordField (RBUF, RB ## Index._LEN, LE ## Index)        \
  LE ## Index = Length                                          \
  MI ## Index = Minimum                                         \
  TR ## Index = Translation                                     \
  MA ## Index = MI ## Index + LE ## Index - 1

#define DWORD_SET(Index, Minimum, Length, Translation)          \
  CreateDWordField (RBUF, RB ## Index._MIN, MI ## Index)        \
  CreateDWordField (RBUF, RB ## Index._MAX, MA ## Index)        \
  CreateDWordField (RBUF, RB ## Index._TRA, TR ## Index)        \
  CreateDWordField (RBUF, RB ## Index._LEN, LE ## Index)        \
  LE ## Index = Length                                          \
  MI ## Index = Minimum                                         \
  TR ## Index = Translation                                     \
  MA ## Index = MI ## Index + LE ## Index - 1

#define WORD_SET(Index, Minimum, Length, Translation)           \
  CreateWordField (RBUF, RB ## Index._MIN, MI ## Index)         \
  CreateWordField (RBUF, RB ## Index._MAX, MA ## Index)         \
  CreateWordField (RBUF, RB ## Index._TRA, TR ## Index)         \
  CreateWordField (RBUF, RB ## Index._LEN, LE ## Index)         \
  LE ## Index = Length                                          \
  MI ## Index = Minimum                                         \
  TR ## Index = Translation                                     \
  MA ## Index = MI ## Index + LE ## Index - 1

#pragma pack(push, 1)
typedef struct {
  EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER                           Header;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE    ConfigSpaces[2][NUM_PCIE_CONTROLLER];
} RK3588_MCFG_TABLE;
#pragma pack(pop)

#endif // __ACPITABLES_H__
