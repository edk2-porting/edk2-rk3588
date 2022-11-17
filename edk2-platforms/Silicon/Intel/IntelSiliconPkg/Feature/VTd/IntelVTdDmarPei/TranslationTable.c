/** @file

  Copyright (c) 2020 - 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <IndustryStandard/Vtd.h>
#include <Ppi/IoMmu.h>
#include <Ppi/VtdInfo.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Guid/VtdPmrInfoHob.h>
#include <Library/CacheMaintenanceLib.h>
#include "IntelVTdDmarPei.h"

#define ALIGN_VALUE_UP(Value, Alignment)  (((Value) + (Alignment) - 1) & (~((Alignment) - 1)))
#define ALIGN_VALUE_LOW(Value, Alignment) ((Value) & (~((Alignment) - 1)))

/**
  Allocate zero pages.

  @param[in]  Pages             the number of pages.

  @return the page address.
  @retval NULL No resource to allocate pages.
**/
VOID *
EFIAPI
AllocateZeroPages (
  IN UINTN                      Pages
  )
{
  VOID *Addr;

  Addr = AllocatePages (Pages);
  if (Addr == NULL) {
    return NULL;
  }
  ZeroMem (Addr, EFI_PAGES_TO_SIZE (Pages));
  return Addr;
}

/**
  Set second level paging entry attribute based upon IoMmuAccess.

  @param[in]  PtEntry           The paging entry.
  @param[in]  IoMmuAccess       The IOMMU access.
**/
VOID
SetSecondLevelPagingEntryAttribute (
  IN VTD_SECOND_LEVEL_PAGING_ENTRY      *PtEntry,
  IN UINT64                             IoMmuAccess
  )
{
  PtEntry->Bits.Read  = ((IoMmuAccess & EDKII_IOMMU_ACCESS_READ) != 0);
  PtEntry->Bits.Write = ((IoMmuAccess & EDKII_IOMMU_ACCESS_WRITE) != 0);
  DEBUG ((DEBUG_VERBOSE, "SetSecondLevelPagingEntryAttribute - 0x%x - 0x%x\n", PtEntry, IoMmuAccess));
}

/**
  Create second level paging entry table.

  @param[in]  VTdUnitInfo               The VTd engine unit information.
  @param[in]  SecondLevelPagingEntry    The second level paging entry.
  @param[in]  MemoryBase                The base of the memory.
  @param[in]  MemoryLimit               The limit of the memory.
  @param[in]  IoMmuAccess               The IOMMU access.

  @return The second level paging entry.
**/
VTD_SECOND_LEVEL_PAGING_ENTRY *
CreateSecondLevelPagingEntryTable (
  IN VTD_UNIT_INFO                      *VTdUnitInfo,
  IN VTD_SECOND_LEVEL_PAGING_ENTRY      *SecondLevelPagingEntry,
  IN UINT64                             MemoryBase,
  IN UINT64                             MemoryLimit,
  IN UINT64                             IoMmuAccess
  )
{
  UINTN                                 Index5;
  UINTN                                 Index4;
  UINTN                                 Index3;
  UINTN                                 Index2;
  UINTN                                 Lvl5Start;
  UINTN                                 Lvl5End;
  UINTN                                 Lvl4PagesStart;
  UINTN                                 Lvl4PagesEnd;
  UINTN                                 Lvl4Start;
  UINTN                                 Lvl4End;
  UINTN                                 Lvl3Start;
  UINTN                                 Lvl3End;
  VTD_SECOND_LEVEL_PAGING_ENTRY         *Lvl5PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY         *Lvl4PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY         *Lvl3PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY         *Lvl2PtEntry;
  UINT64                                BaseAddress;
  UINT64                                EndAddress;
  BOOLEAN                               Is5LevelPaging;

  if (MemoryLimit == 0) {
    return EFI_SUCCESS;
  }

  Lvl4PagesStart = 0;
  Lvl4PagesEnd   = 0;
  Lvl4PtEntry    = NULL;
  Lvl5PtEntry    = NULL;

  BaseAddress = ALIGN_VALUE_LOW (MemoryBase, SIZE_2MB);
  EndAddress = ALIGN_VALUE_UP (MemoryLimit, SIZE_2MB);
  DEBUG ((DEBUG_INFO, "CreateSecondLevelPagingEntryTable: BaseAddress - 0x%016lx, EndAddress - 0x%016lx\n", BaseAddress, EndAddress));

  if (SecondLevelPagingEntry == NULL) {
    SecondLevelPagingEntry = AllocateZeroPages (1);
    if (SecondLevelPagingEntry == NULL) {
      DEBUG ((DEBUG_ERROR, "Could not Alloc LVL4 or LVL5 PT. \n"));
      return NULL;
    }
    FlushPageTableMemory (VTdUnitInfo, (UINTN) SecondLevelPagingEntry, EFI_PAGES_TO_SIZE (1));
  }

  DEBUG ((DEBUG_INFO, " SecondLevelPagingEntry:0x%016lx\n", SecondLevelPagingEntry));
  //
  // If no access is needed, just create not present entry.
  //
  if (IoMmuAccess == 0) {
    DEBUG ((DEBUG_INFO, " SecondLevelPagingEntry:0x%016lx\n", (UINTN) SecondLevelPagingEntry));
    return SecondLevelPagingEntry;
  }

  Is5LevelPaging = VTdUnitInfo->Is5LevelPaging;

  if (Is5LevelPaging) {
    Lvl5Start = RShiftU64 (BaseAddress, 48) & 0x1FF;
    Lvl5End = RShiftU64 (EndAddress - 1, 48) & 0x1FF;
    DEBUG ((DEBUG_INFO, "  Lvl5Start - 0x%x, Lvl5End - 0x%x\n", Lvl5Start, Lvl5End));

    Lvl4Start = RShiftU64 (BaseAddress, 39) & 0x1FF;
    Lvl4End = RShiftU64 (EndAddress - 1, 39) & 0x1FF;

    Lvl4PagesStart = (Lvl5Start<<9) | Lvl4Start;
    Lvl4PagesEnd = (Lvl5End<<9) | Lvl4End;
    DEBUG ((DEBUG_INFO, "  Lvl4PagesStart - 0x%x, Lvl4PagesEnd - 0x%x\n", Lvl4PagesStart, Lvl4PagesEnd));

    Lvl5PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) SecondLevelPagingEntry;
  } else {
    Lvl5Start = RShiftU64 (BaseAddress, 48) & 0x1FF;
    Lvl5End = Lvl5Start;

    Lvl4Start = RShiftU64 (BaseAddress, 39) & 0x1FF;
    Lvl4End = RShiftU64 (EndAddress - 1, 39) & 0x1FF;
    DEBUG ((DEBUG_INFO, "  Lvl4Start - 0x%x, Lvl4End - 0x%x\n", Lvl4Start, Lvl4End));

    Lvl4PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) SecondLevelPagingEntry;
  }

  for (Index5 = Lvl5Start; Index5 <= Lvl5End; Index5++) {
    if (Is5LevelPaging) {
      if (Lvl5PtEntry[Index5].Uint64 == 0) {
        Lvl5PtEntry[Index5].Uint64 = (UINT64) (UINTN) AllocateZeroPages (1);
        if (Lvl5PtEntry[Index5].Uint64 == 0) {
          DEBUG ((DEBUG_ERROR, "!!!!!! ALLOCATE LVL4 PAGE FAIL (0x%x)!!!!!!\n", Index5));
          ASSERT (FALSE);
          return NULL;
        }
        FlushPageTableMemory (VTdUnitInfo, (UINTN) Lvl5PtEntry[Index5].Uint64, SIZE_4KB);
        SetSecondLevelPagingEntryAttribute (&Lvl5PtEntry[Index5], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      }
      Lvl4Start = Lvl4PagesStart & 0x1FF;
      if (((Index5+1)<<9) > Lvl4PagesEnd) {
        Lvl4End = SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY) - 1;;
        Lvl4PagesStart = (Index5+1)<<9;
      } else {
        Lvl4End = Lvl4PagesEnd & 0x1FF;
      }
      DEBUG ((DEBUG_INFO, "  Lvl5(0x%x): Lvl4Start - 0x%x, Lvl4End - 0x%x\n", Index5, Lvl4Start, Lvl4End));
      Lvl4PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) (UINTN) VTD_64BITS_ADDRESS(Lvl5PtEntry[Index5].Bits.AddressLo, Lvl5PtEntry[Index5].Bits.AddressHi);
    }

    for (Index4 = Lvl4Start; Index4 <= Lvl4End; Index4++) {
      if (Lvl4PtEntry[Index4].Uint64 == 0) {
        Lvl4PtEntry[Index4].Uint64 = (UINT64) (UINTN) AllocateZeroPages (1);
        if (Lvl4PtEntry[Index4].Uint64 == 0) {
          DEBUG ((DEBUG_ERROR, "!!!!!! ALLOCATE LVL4 PAGE FAIL (0x%x)!!!!!!\n", Index4));
          ASSERT(FALSE);
          return NULL;
        }
        FlushPageTableMemory (VTdUnitInfo, (UINTN) Lvl4PtEntry[Index4].Uint64, SIZE_4KB);
        SetSecondLevelPagingEntryAttribute (&Lvl4PtEntry[Index4], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      }

      Lvl3Start = RShiftU64 (BaseAddress, 30) & 0x1FF;
      if (ALIGN_VALUE_LOW(BaseAddress + SIZE_1GB, SIZE_1GB) <= EndAddress) {
        Lvl3End = SIZE_4KB / sizeof (VTD_SECOND_LEVEL_PAGING_ENTRY) - 1;
      } else {
        Lvl3End = RShiftU64 (EndAddress - 1, 30) & 0x1FF;
      }
      DEBUG ((DEBUG_INFO, "  Lvl4(0x%x): Lvl3Start - 0x%x, Lvl3End - 0x%x\n", Index4, Lvl3Start, Lvl3End));

      Lvl3PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) (UINTN) VTD_64BITS_ADDRESS(Lvl4PtEntry[Index4].Bits.AddressLo, Lvl4PtEntry[Index4].Bits.AddressHi);
      for (Index3 = Lvl3Start; Index3 <= Lvl3End; Index3++) {
        if (Lvl3PtEntry[Index3].Uint64 == 0) {
          Lvl3PtEntry[Index3].Uint64 = (UINT64) (UINTN) AllocateZeroPages (1);
          if (Lvl3PtEntry[Index3].Uint64 == 0) {
            DEBUG ((DEBUG_ERROR, "!!!!!! ALLOCATE LVL3 PAGE FAIL (0x%x, 0x%x)!!!!!!\n", Index4, Index3));
            ASSERT(FALSE);
            return NULL;
          }
          FlushPageTableMemory (VTdUnitInfo, (UINTN) Lvl3PtEntry[Index3].Uint64, SIZE_4KB);
          SetSecondLevelPagingEntryAttribute (&Lvl3PtEntry[Index3], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
        }

        Lvl2PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) (UINTN) VTD_64BITS_ADDRESS(Lvl3PtEntry[Index3].Bits.AddressLo, Lvl3PtEntry[Index3].Bits.AddressHi);
        for (Index2 = 0; Index2 < SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY); Index2++) {
          Lvl2PtEntry[Index2].Uint64 = BaseAddress;
          SetSecondLevelPagingEntryAttribute (&Lvl2PtEntry[Index2], IoMmuAccess);
          Lvl2PtEntry[Index2].Bits.PageSize = 1;
          BaseAddress += SIZE_2MB;
          if (BaseAddress >= MemoryLimit) {
            break;
          }
        }
        FlushPageTableMemory (VTdUnitInfo, (UINTN) Lvl2PtEntry, SIZE_4KB);
        if (BaseAddress >= MemoryLimit) {
          break;
        }
      }
      FlushPageTableMemory (VTdUnitInfo, (UINTN) &Lvl3PtEntry[Lvl3Start], (UINTN) &Lvl3PtEntry[Lvl3End + 1] - (UINTN) &Lvl3PtEntry[Lvl3Start]);
      if (BaseAddress >= MemoryLimit) {
        break;
      }
    }
    FlushPageTableMemory (VTdUnitInfo, (UINTN) &Lvl4PtEntry[Lvl4Start], (UINTN) &Lvl4PtEntry[Lvl4End + 1] - (UINTN) &Lvl4PtEntry[Lvl4Start]);
  }
  FlushPageTableMemory (VTdUnitInfo, (UINTN) &Lvl5PtEntry[Lvl5Start], (UINTN) &Lvl5PtEntry[Lvl5End + 1] - (UINTN) &Lvl5PtEntry[Lvl5Start]);

  DEBUG ((DEBUG_INFO, " SecondLevelPagingEntry:0x%016lx\n", (UINTN)SecondLevelPagingEntry));
  return SecondLevelPagingEntry;
}

/**
  Create context entry.

  @param[in]  VTdUnitInfo       The VTd engine unit information.

  @retval EFI_SUCCESS           The context entry is created.
  @retval EFI_OUT_OF_RESOURCE   No enough resource to create context entry.

**/
EFI_STATUS
CreateContextEntry (
  IN VTD_UNIT_INFO              *VTdUnitInfo
  )
{
  UINTN                         RootPages;
  UINTN                         ContextPages;
  UINTN                         EntryTablePages;
  VOID                          *Buffer;
  UINTN                         RootIndex;
  UINTN                         ContextIndex;
  VTD_ROOT_ENTRY                *RootEntryBase;
  VTD_ROOT_ENTRY                *RootEntry;
  VTD_CONTEXT_ENTRY             *ContextEntryTable;
  VTD_CONTEXT_ENTRY             *ContextEntry;
  VTD_SOURCE_ID                 SourceId;
  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry;
  UINT64                        Pt;

  RootPages = EFI_SIZE_TO_PAGES (sizeof (VTD_ROOT_ENTRY) * VTD_ROOT_ENTRY_NUMBER);
  ContextPages = EFI_SIZE_TO_PAGES (sizeof (VTD_CONTEXT_ENTRY) * VTD_CONTEXT_ENTRY_NUMBER);
  EntryTablePages = RootPages + ContextPages * (VTD_ROOT_ENTRY_NUMBER);
  Buffer = AllocateZeroPages (EntryTablePages);
  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Could not Alloc Root Entry Table.. \n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_ERROR, "RootEntryTable address - 0x%x\n", Buffer));
  VTdUnitInfo->RootEntryTable = (UINT32) (UINTN) Buffer;
  VTdUnitInfo->RootEntryTablePageSize = (UINT16) EntryTablePages;
  RootEntryBase = (VTD_ROOT_ENTRY *) Buffer;
  Buffer = (UINT8 *) Buffer + EFI_PAGES_TO_SIZE (RootPages);

  if (VTdUnitInfo->FixedSecondLevelPagingEntry == 0) {
    DEBUG ((DEBUG_ERROR, "FixedSecondLevelPagingEntry is empty\n"));
    ASSERT(FALSE);
  }

  for (RootIndex = 0; RootIndex < VTD_ROOT_ENTRY_NUMBER; RootIndex++) {
    SourceId.Index.RootIndex = (UINT8) RootIndex;

    RootEntry = &RootEntryBase[SourceId.Index.RootIndex];
    RootEntry->Bits.ContextTablePointerLo  = (UINT32) RShiftU64 ((UINT64) (UINTN) Buffer, 12);
    RootEntry->Bits.ContextTablePointerHi  = (UINT32) RShiftU64 ((UINT64) (UINTN) Buffer, 32);
    RootEntry->Bits.Present = 1;
    Buffer = (UINT8 *)Buffer + EFI_PAGES_TO_SIZE (ContextPages);
    ContextEntryTable = (VTD_CONTEXT_ENTRY *) (UINTN) VTD_64BITS_ADDRESS(RootEntry->Bits.ContextTablePointerLo, RootEntry->Bits.ContextTablePointerHi) ;

    for (ContextIndex = 0; ContextIndex < VTD_CONTEXT_ENTRY_NUMBER; ContextIndex++) {
      SourceId.Index.ContextIndex = (UINT8) ContextIndex;
      ContextEntry = &ContextEntryTable[SourceId.Index.ContextIndex];

      ContextEntry->Bits.TranslationType = 0;
      ContextEntry->Bits.FaultProcessingDisable = 0;
      ContextEntry->Bits.Present = 0;

      ContextEntry->Bits.AddressWidth = VTdUnitInfo->Is5LevelPaging ? 0x3 : 0x2;

      if (VTdUnitInfo->FixedSecondLevelPagingEntry != 0) {
        SecondLevelPagingEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) (UINTN) VTdUnitInfo->FixedSecondLevelPagingEntry;
        Pt = (UINT64)RShiftU64 ((UINT64) (UINTN) SecondLevelPagingEntry, 12);
        ContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
        ContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
        ContextEntry->Bits.DomainIdentifier = ((1 << (UINT8)((UINTN)VTdUnitInfo->CapReg.Bits.ND * 2 + 4)) - 1);
        ContextEntry->Bits.Present = 1;
      }
    }
  }

  FlushPageTableMemory (VTdUnitInfo, VTdUnitInfo->RootEntryTable, EFI_PAGES_TO_SIZE(EntryTablePages));

  return EFI_SUCCESS;
}

/**
  Create extended context entry.

  @param[in]  VTdUnitInfo       The VTd engine unit information.

  @retval EFI_SUCCESS           The extended context entry is created.
  @retval EFI_OUT_OF_RESOURCE   No enough resource to create extended context entry.
**/
EFI_STATUS
CreateExtContextEntry (
  IN VTD_UNIT_INFO              *VTdUnitInfo
  )
{
  UINTN                         RootPages;
  UINTN                         ContextPages;
  UINTN                         EntryTablePages;
  VOID                          *Buffer;
  UINTN                         RootIndex;
  UINTN                         ContextIndex;
  VTD_EXT_ROOT_ENTRY            *ExtRootEntryBase;
  VTD_EXT_ROOT_ENTRY            *ExtRootEntry;
  VTD_EXT_CONTEXT_ENTRY         *ExtContextEntryTable;
  VTD_EXT_CONTEXT_ENTRY         *ExtContextEntry;
  VTD_SOURCE_ID                 SourceId;
  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry;
  UINT64                        Pt;

  RootPages = EFI_SIZE_TO_PAGES (sizeof (VTD_EXT_ROOT_ENTRY) * VTD_ROOT_ENTRY_NUMBER);
  ContextPages = EFI_SIZE_TO_PAGES (sizeof (VTD_EXT_CONTEXT_ENTRY) * VTD_CONTEXT_ENTRY_NUMBER);
  EntryTablePages = RootPages + ContextPages * (VTD_ROOT_ENTRY_NUMBER);
  Buffer = AllocateZeroPages (EntryTablePages);
  if (Buffer == NULL) {
    DEBUG ((DEBUG_INFO, "Could not Alloc Root Entry Table !\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_ERROR, "ExtRootEntryTable address - 0x%x\n", Buffer));
  VTdUnitInfo->ExtRootEntryTable = (UINT32) (UINTN) Buffer;
  VTdUnitInfo->ExtRootEntryTablePageSize = (UINT16) EntryTablePages;
  ExtRootEntryBase = (VTD_EXT_ROOT_ENTRY *) Buffer;
  Buffer = (UINT8 *) Buffer + EFI_PAGES_TO_SIZE (RootPages);

  if (VTdUnitInfo->FixedSecondLevelPagingEntry == 0) {
    DEBUG ((DEBUG_ERROR, "FixedSecondLevelPagingEntry is empty\n"));
    ASSERT(FALSE);
  }

  for (RootIndex = 0; RootIndex < VTD_ROOT_ENTRY_NUMBER; RootIndex++) {
    SourceId.Index.RootIndex = (UINT8)RootIndex;

    ExtRootEntry = &ExtRootEntryBase[SourceId.Index.RootIndex];
    ExtRootEntry->Bits.LowerContextTablePointerLo  = (UINT32) RShiftU64 ((UINT64) (UINTN) Buffer, 12);
    ExtRootEntry->Bits.LowerContextTablePointerHi  = (UINT32) RShiftU64 ((UINT64) (UINTN) Buffer, 32);
    ExtRootEntry->Bits.LowerPresent = 1;
    ExtRootEntry->Bits.UpperContextTablePointerLo  = (UINT32) RShiftU64 ((UINT64) (UINTN) Buffer, 12) + 1;
    ExtRootEntry->Bits.UpperContextTablePointerHi  = (UINT32) RShiftU64 (RShiftU64 ((UINT64) (UINTN) Buffer, 12) + 1, 20);
    ExtRootEntry->Bits.UpperPresent = 1;
    Buffer = (UINT8 *) Buffer + EFI_PAGES_TO_SIZE (ContextPages);
    ExtContextEntryTable = (VTD_EXT_CONTEXT_ENTRY *) (UINTN) VTD_64BITS_ADDRESS (ExtRootEntry->Bits.LowerContextTablePointerLo, ExtRootEntry->Bits.LowerContextTablePointerHi) ;

    for (ContextIndex = 0; ContextIndex < VTD_CONTEXT_ENTRY_NUMBER; ContextIndex++) {
      SourceId.Index.ContextIndex = (UINT8) ContextIndex;
      ExtContextEntry = &ExtContextEntryTable[SourceId.Index.ContextIndex];

      ExtContextEntry->Bits.TranslationType = 0;
      ExtContextEntry->Bits.FaultProcessingDisable = 0;
      ExtContextEntry->Bits.Present = 0;

      ExtContextEntry->Bits.AddressWidth = VTdUnitInfo->Is5LevelPaging ? 0x3 : 0x2;

      if (VTdUnitInfo->FixedSecondLevelPagingEntry != 0) {
        SecondLevelPagingEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) (UINTN) VTdUnitInfo->FixedSecondLevelPagingEntry;
        Pt = (UINT64)RShiftU64 ((UINT64) (UINTN) SecondLevelPagingEntry, 12);

        ExtContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
        ExtContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
        ExtContextEntry->Bits.DomainIdentifier = ((1 << (UINT8) ((UINTN) VTdUnitInfo->CapReg.Bits.ND * 2 + 4)) - 1);
        ExtContextEntry->Bits.Present = 1;
      }
    }
  }

  FlushPageTableMemory (VTdUnitInfo, VTdUnitInfo->ExtRootEntryTable, EFI_PAGES_TO_SIZE(EntryTablePages));

  return EFI_SUCCESS;
}

#define VTD_PG_R                   BIT0
#define VTD_PG_W                   BIT1
#define VTD_PG_X                   BIT2
#define VTD_PG_EMT                 (BIT3 | BIT4 | BIT5)
#define VTD_PG_TM                  (BIT62)

#define VTD_PG_PS                  BIT7

#define PAGE_PROGATE_BITS          (VTD_PG_TM | VTD_PG_EMT | VTD_PG_W | VTD_PG_R)

#define PAGING_4K_MASK  0xFFF
#define PAGING_2M_MASK  0x1FFFFF
#define PAGING_1G_MASK  0x3FFFFFFF

#define PAGING_VTD_INDEX_MASK     0x1FF

#define PAGING_4K_ADDRESS_MASK_64 0x000FFFFFFFFFF000ull
#define PAGING_2M_ADDRESS_MASK_64 0x000FFFFFFFE00000ull
#define PAGING_1G_ADDRESS_MASK_64 0x000FFFFFC0000000ull

typedef enum {
  PageNone,
  Page4K,
  Page2M,
  Page1G,
} PAGE_ATTRIBUTE;

typedef struct {
  PAGE_ATTRIBUTE   Attribute;
  UINT64           Length;
  UINT64           AddressMask;
} PAGE_ATTRIBUTE_TABLE;

PAGE_ATTRIBUTE_TABLE mPageAttributeTable[] = {
  {Page4K,  SIZE_4KB, PAGING_4K_ADDRESS_MASK_64},
  {Page2M,  SIZE_2MB, PAGING_2M_ADDRESS_MASK_64},
  {Page1G,  SIZE_1GB, PAGING_1G_ADDRESS_MASK_64},
};

/**
  Return length according to page attributes.

  @param[in]  PageAttributes   The page attribute of the page entry.

  @return The length of page entry.
**/
UINTN
PageAttributeToLength (
  IN PAGE_ATTRIBUTE  PageAttribute
  )
{
  UINTN  Index;
  for (Index = 0; Index < sizeof (mPageAttributeTable) / sizeof (mPageAttributeTable[0]); Index++) {
    if (PageAttribute == mPageAttributeTable[Index].Attribute) {
      return (UINTN) mPageAttributeTable[Index].Length;
    }
  }
  return 0;
}

/**
  Return page table entry to match the address.

  @param[in]   VTdUnitInfo              The VTd engine unit information.
  @param[in]   SecondLevelPagingEntry   The second level paging entry in VTd table for the device.
  @param[in]   Address                  The address to be checked.
  @param[out]  PageAttributes           The page attribute of the page entry.

  @return The page entry.
**/
VOID *
GetSecondLevelPageTableEntry (
  IN  VTD_UNIT_INFO                 *VTdUnitInfo,
  IN  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry,
  IN  PHYSICAL_ADDRESS              Address,
  OUT PAGE_ATTRIBUTE                *PageAttribute
  )
{
  UINTN                 Index1;
  UINTN                 Index2;
  UINTN                 Index3;
  UINTN                 Index4;
  UINTN                 Index5;
  UINT64                *L1PageTable;
  UINT64                *L2PageTable;
  UINT64                *L3PageTable;
  UINT64                *L4PageTable;
  UINT64                *L5PageTable;
  BOOLEAN               Is5LevelPaging;

  Index5 = ((UINTN) RShiftU64 (Address, 48)) & PAGING_VTD_INDEX_MASK;
  Index4 = ((UINTN) RShiftU64 (Address, 39)) & PAGING_VTD_INDEX_MASK;
  Index3 = ((UINTN) Address >> 30) & PAGING_VTD_INDEX_MASK;
  Index2 = ((UINTN) Address >> 21) & PAGING_VTD_INDEX_MASK;
  Index1 = ((UINTN) Address >> 12) & PAGING_VTD_INDEX_MASK;

  Is5LevelPaging = VTdUnitInfo->Is5LevelPaging;

  if (Is5LevelPaging) {
    L5PageTable = (UINT64 *) SecondLevelPagingEntry;
    if (L5PageTable[Index5] == 0) {
      L5PageTable[Index5] = (UINT64) (UINTN) AllocateZeroPages (1);
      if (L5PageTable[Index5] == 0) {
        DEBUG ((DEBUG_ERROR, "!!!!!! ALLOCATE LVL5 PAGE FAIL (0x%x)!!!!!!\n", Index4));
        ASSERT(FALSE);
        *PageAttribute = PageNone;
        return NULL;
      }
      FlushPageTableMemory (VTdUnitInfo, (UINTN) L5PageTable[Index5], SIZE_4KB);
      SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *) &L5PageTable[Index5], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      FlushPageTableMemory (VTdUnitInfo, (UINTN) &L5PageTable[Index5], sizeof(L5PageTable[Index5]));
    }
    L4PageTable = (UINT64 *) (UINTN) (L5PageTable[Index5] & PAGING_4K_ADDRESS_MASK_64);
  } else {
    L4PageTable = (UINT64 *)SecondLevelPagingEntry;
  }

  if (L4PageTable[Index4] == 0) {
    L4PageTable[Index4] = (UINT64) (UINTN) AllocateZeroPages (1);
    if (L4PageTable[Index4] == 0) {
      DEBUG ((DEBUG_ERROR, "!!!!!! ALLOCATE LVL4 PAGE FAIL (0x%x)!!!!!!\n", Index4));
      ASSERT(FALSE);
      *PageAttribute = PageNone;
      return NULL;
    }
    FlushPageTableMemory (VTdUnitInfo, (UINTN) L4PageTable[Index4], SIZE_4KB);
    SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *) &L4PageTable[Index4], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
    FlushPageTableMemory (VTdUnitInfo, (UINTN) &L4PageTable[Index4], sizeof(L4PageTable[Index4]));
  }

  L3PageTable = (UINT64 *) (UINTN) (L4PageTable[Index4] & PAGING_4K_ADDRESS_MASK_64);
  if (L3PageTable[Index3] == 0) {
    L3PageTable[Index3] = (UINT64) (UINTN) AllocateZeroPages (1);
    if (L3PageTable[Index3] == 0) {
      DEBUG ((DEBUG_ERROR, "!!!!!! ALLOCATE LVL3 PAGE FAIL (0x%x, 0x%x)!!!!!!\n", Index4, Index3));
      ASSERT(FALSE);
      *PageAttribute = PageNone;
      return NULL;
    }
    FlushPageTableMemory (VTdUnitInfo, (UINTN) L3PageTable[Index3], SIZE_4KB);
    SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *) &L3PageTable[Index3], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
    FlushPageTableMemory (VTdUnitInfo, (UINTN) &L3PageTable[Index3], sizeof (L3PageTable[Index3]));
  }
  if ((L3PageTable[Index3] & VTD_PG_PS) != 0) {
    // 1G
    *PageAttribute = Page1G;
    return &L3PageTable[Index3];
  }

  L2PageTable = (UINT64 *) (UINTN) (L3PageTable[Index3] & PAGING_4K_ADDRESS_MASK_64);
  if (L2PageTable[Index2] == 0) {
    L2PageTable[Index2] = Address & PAGING_2M_ADDRESS_MASK_64;
    SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *) &L2PageTable[Index2], 0);
    L2PageTable[Index2] |= VTD_PG_PS;
    FlushPageTableMemory (VTdUnitInfo, (UINTN) &L2PageTable[Index2], sizeof (L2PageTable[Index2]));
  }
  if ((L2PageTable[Index2] & VTD_PG_PS) != 0) {
    // 2M
    *PageAttribute = Page2M;
    return &L2PageTable[Index2];
  }

  // 4k
  L1PageTable = (UINT64 *) (UINTN) (L2PageTable[Index2] & PAGING_4K_ADDRESS_MASK_64);
  if ((L1PageTable[Index1] == 0) && (Address != 0)) {
    *PageAttribute = PageNone;
    return NULL;
  }
  *PageAttribute = Page4K;
  return &L1PageTable[Index1];
}

/**
  Modify memory attributes of page entry.

  @param[in]   VTdUnitInfo      The VTd engine unit information.
  @param[in]   PageEntry        The page entry.
  @param[in]   IoMmuAccess      The IOMMU access.
  @param[out]  IsModified       TRUE means page table modified. FALSE means page table not modified.
**/
VOID
ConvertSecondLevelPageEntryAttribute (
  IN VTD_UNIT_INFO                      *VTdUnitInfo,
  IN  VTD_SECOND_LEVEL_PAGING_ENTRY     *PageEntry,
  IN  UINT64                            IoMmuAccess,
  OUT BOOLEAN                           *IsModified
  )
{
  UINT64  CurrentPageEntry;
  UINT64  NewPageEntry;

  CurrentPageEntry = PageEntry->Uint64;
  SetSecondLevelPagingEntryAttribute (PageEntry, IoMmuAccess);
  FlushPageTableMemory (VTdUnitInfo, (UINTN) PageEntry, sizeof(*PageEntry));
  NewPageEntry = PageEntry->Uint64;
  if (CurrentPageEntry != NewPageEntry) {
    *IsModified = TRUE;
    DEBUG ((DEBUG_VERBOSE, "ConvertSecondLevelPageEntryAttribute 0x%lx", CurrentPageEntry));
    DEBUG ((DEBUG_VERBOSE, "->0x%lx\n", NewPageEntry));
  } else {
    *IsModified = FALSE;
  }
}

/**
  This function returns if there is need to split page entry.

  @param[in]  BaseAddress      The base address to be checked.
  @param[in]  Length           The length to be checked.
  @param[in]  PageAttribute    The page attribute of the page entry.

  @retval SplitAttributes on if there is need to split page entry.
**/
PAGE_ATTRIBUTE
NeedSplitPage (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length,
  IN  PAGE_ATTRIBUTE                    PageAttribute
  )
{
  UINT64                PageEntryLength;

  PageEntryLength = PageAttributeToLength (PageAttribute);

  if (((BaseAddress & (PageEntryLength - 1)) == 0) && (Length >= PageEntryLength)) {
    return PageNone;
  }

  if (((BaseAddress & PAGING_2M_MASK) != 0) || (Length < SIZE_2MB)) {
    return Page4K;
  }

  return Page2M;
}

/**
  This function splits one page entry to small page entries.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  PageEntry        The page entry to be splitted.
  @param[in]  PageAttribute    The page attribute of the page entry.
  @param[in]  SplitAttribute   How to split the page entry.

  @retval RETURN_SUCCESS            The page entry is splitted.
  @retval RETURN_UNSUPPORTED        The page entry does not support to be splitted.
  @retval RETURN_OUT_OF_RESOURCES   No resource to split page entry.
**/
RETURN_STATUS
SplitSecondLevelPage (
  IN VTD_UNIT_INFO                      *VTdUnitInfo,
  IN  VTD_SECOND_LEVEL_PAGING_ENTRY     *PageEntry,
  IN  PAGE_ATTRIBUTE                    PageAttribute,
  IN  PAGE_ATTRIBUTE                    SplitAttribute
  )
{
  UINT64   BaseAddress;
  UINT64   *NewPageEntry;
  UINTN    Index;

  ASSERT (PageAttribute == Page2M || PageAttribute == Page1G);

  if (PageAttribute == Page2M) {
    //
    // Split 2M to 4K
    //
    ASSERT (SplitAttribute == Page4K);
    if (SplitAttribute == Page4K) {
      NewPageEntry = AllocateZeroPages (1);
      DEBUG ((DEBUG_INFO, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = PageEntry->Uint64 & PAGING_2M_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_4KB * Index) | (PageEntry->Uint64 & PAGE_PROGATE_BITS);
      }
      FlushPageTableMemory (VTdUnitInfo, (UINTN)NewPageEntry, SIZE_4KB);

      PageEntry->Uint64 = (UINT64)(UINTN)NewPageEntry;
      SetSecondLevelPagingEntryAttribute (PageEntry, EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      FlushPageTableMemory (VTdUnitInfo, (UINTN)PageEntry, sizeof(*PageEntry));
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else if (PageAttribute == Page1G) {
    //
    // Split 1G to 2M
    // No need support 1G->4K directly, we should use 1G->2M, then 2M->4K to get more compact page table.
    //
    ASSERT (SplitAttribute == Page2M || SplitAttribute == Page4K);
    if ((SplitAttribute == Page2M || SplitAttribute == Page4K)) {
      NewPageEntry = AllocateZeroPages (1);
      DEBUG ((DEBUG_INFO, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = PageEntry->Uint64 & PAGING_1G_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_2MB * Index) | VTD_PG_PS | (PageEntry->Uint64 & PAGE_PROGATE_BITS);
      }
      FlushPageTableMemory (VTdUnitInfo, (UINTN)NewPageEntry, SIZE_4KB);

      PageEntry->Uint64 = (UINT64)(UINTN)NewPageEntry;
      SetSecondLevelPagingEntryAttribute (PageEntry, EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      FlushPageTableMemory (VTdUnitInfo, (UINTN)PageEntry, sizeof(*PageEntry));
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else {
    return RETURN_UNSUPPORTED;
  }
}

/**
  Set VTd attribute for a system memory on second level page entry

  @param[in]  VTdUnitInfo             The VTd engine unit information.
  @param[in]  SecondLevelPagingEntry  The second level paging entry in VTd table for the device.
  @param[in]  BaseAddress             The base of device memory address to be used as the DMA memory.
  @param[in]  Length                  The length of device memory address to be used as the DMA memory.
  @param[in]  IoMmuAccess             The IOMMU access.

  @retval EFI_SUCCESS            The IoMmuAccess is set for the memory range specified by BaseAddress and Length.
  @retval EFI_INVALID_PARAMETER  BaseAddress is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is 0.
  @retval EFI_INVALID_PARAMETER  IoMmuAccess specified an illegal combination of access.
  @retval EFI_UNSUPPORTED        The bit mask of IoMmuAccess is not supported by the IOMMU.
  @retval EFI_UNSUPPORTED        The IOMMU does not support the memory range specified by BaseAddress and Length.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to modify the IOMMU access.
  @retval EFI_DEVICE_ERROR       The IOMMU device reported an error while attempting the operation.
**/
EFI_STATUS
SetSecondLevelPagingAttribute (
  IN VTD_UNIT_INFO                 *VTdUnitInfo,
  IN VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry,
  IN UINT64                        BaseAddress,
  IN UINT64                        Length,
  IN UINT64                        IoMmuAccess
  )
{
  VTD_SECOND_LEVEL_PAGING_ENTRY  *PageEntry;
  PAGE_ATTRIBUTE                 PageAttribute;
  UINTN                          PageEntryLength;
  PAGE_ATTRIBUTE                 SplitAttribute;
  EFI_STATUS                     Status;
  BOOLEAN                        IsEntryModified;

  DEBUG ((DEBUG_INFO, "SetSecondLevelPagingAttribute (0x%016lx - 0x%016lx : %x) \n", BaseAddress, Length, IoMmuAccess));
  DEBUG ((DEBUG_INFO, "  SecondLevelPagingEntry Base - 0x%x\n", SecondLevelPagingEntry));

  if (BaseAddress != ALIGN_VALUE(BaseAddress, SIZE_4KB)) {
    DEBUG ((DEBUG_ERROR, "SetSecondLevelPagingAttribute - Invalid Alignment\n"));
    return EFI_UNSUPPORTED;
  }
  if (Length != ALIGN_VALUE(Length, SIZE_4KB)) {
    DEBUG ((DEBUG_ERROR, "SetSecondLevelPagingAttribute - Invalid Alignment\n"));
    return EFI_UNSUPPORTED;
  }

  while (Length != 0) {
    PageEntry = GetSecondLevelPageTableEntry (VTdUnitInfo, SecondLevelPagingEntry, BaseAddress, &PageAttribute);
    if (PageEntry == NULL) {
      DEBUG ((DEBUG_ERROR, "PageEntry - NULL\n"));
      return RETURN_UNSUPPORTED;
    }
    PageEntryLength = PageAttributeToLength (PageAttribute);
    SplitAttribute = NeedSplitPage (BaseAddress, Length, PageAttribute);
    if (SplitAttribute == PageNone) {
      ConvertSecondLevelPageEntryAttribute (VTdUnitInfo, PageEntry, IoMmuAccess, &IsEntryModified);
      if (IsEntryModified) {
        //mVtdUnitInformation[VtdIndex].HasDirtyPages = TRUE;
      }
      //
      // Convert success, move to next
      //
      BaseAddress += PageEntryLength;
      Length -= PageEntryLength;
    } else {
      Status = SplitSecondLevelPage (VTdUnitInfo, PageEntry, PageAttribute, SplitAttribute);
      if (RETURN_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "SplitSecondLevelPage - %r\n", Status));
        return RETURN_UNSUPPORTED;
      }
      //mVtdUnitInformation[VtdIndex].HasDirtyPages = TRUE;
      //
      // Just split current page
      // Convert success in next around
      //
    }
  }

  return EFI_SUCCESS;
}

/**
  Create Fixed Second Level Paging Entry.

  @param[in]  VTdUnitInfo       The VTd engine unit information.

  @retval EFI_SUCCESS           Setup translation table successfully.
  @retval EFI_OUT_OF_RESOURCES  Setup translation table fail.

**/
EFI_STATUS
CreateFixedSecondLevelPagingEntry (
  IN VTD_UNIT_INFO              *VTdUnitInfo
  )
{
  EFI_STATUS Status;
  UINT64                        IoMmuAccess;
  UINT64                        BaseAddress;
  UINT64                        Length;
  VOID                          *Hob;
  DMA_BUFFER_INFO               *DmaBufferInfo;

  VTdUnitInfo->FixedSecondLevelPagingEntry = (UINT32) (UINTN) CreateSecondLevelPagingEntryTable (VTdUnitInfo, NULL, 0, SIZE_4GB, 0);
  if (VTdUnitInfo->FixedSecondLevelPagingEntry == 0) {
    DEBUG ((DEBUG_ERROR, "FixedSecondLevelPagingEntry is empty\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Hob = GetFirstGuidHob (&mDmaBufferInfoGuid);
  DmaBufferInfo = GET_GUID_HOB_DATA (Hob);
  BaseAddress = DmaBufferInfo->DmaBufferBase;
  Length = DmaBufferInfo->DmaBufferLimit - DmaBufferInfo->DmaBufferBase;
  IoMmuAccess = EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE;

  DEBUG ((DEBUG_INFO, "  BaseAddress = 0x%lx\n", BaseAddress));
  DEBUG ((DEBUG_INFO, "  Length = 0x%lx\n", Length));
  DEBUG ((DEBUG_INFO, "  IoMmuAccess = 0x%lx\n", IoMmuAccess));

  Status = SetSecondLevelPagingAttribute (VTdUnitInfo, (VTD_SECOND_LEVEL_PAGING_ENTRY*) (UINTN) VTdUnitInfo->FixedSecondLevelPagingEntry, BaseAddress, Length, IoMmuAccess);

  return Status;
}
/**
  Setup VTd translation table.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           Setup translation table successfully.
  @retval EFI_OUT_OF_RESOURCES  Setup translation table fail.

**/
EFI_STATUS
SetupTranslationTable (
  IN VTD_INFO                   *VTdInfo
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  VTD_UNIT_INFO                 *VtdUnitInfo;

  for (Index = 0; Index < VTdInfo->VTdEngineCount; Index++) {
    VtdUnitInfo = &VTdInfo->VtdUnitInfo[Index];

    Status = CreateFixedSecondLevelPagingEntry (VtdUnitInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "CreateFixedSecondLevelPagingEntry failed - %r\n", Status));
      return Status;
    }

    if (VtdUnitInfo->ECapReg.Bits.ECS) {
      DEBUG ((DEBUG_INFO, "CreateExtContextEntry - %d\n", Index));
      Status = CreateExtContextEntry (VtdUnitInfo);
    } else {
      DEBUG ((DEBUG_INFO, "CreateContextEntry - %d\n", Index));
      Status = CreateContextEntry (VtdUnitInfo);
    }
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}

/**
  Find the VTd index by the Segment and SourceId.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  Segment           The segment of the source.
  @param[in]  SourceId          The SourceId of the source.
  @param[out] ExtContextEntry   The ExtContextEntry of the source.
  @param[out] ContextEntry      The ContextEntry of the source.

  @return The index of the VTd engine.
  @retval (UINTN)-1  The VTd engine is not found.
**/
UINTN
FindVtdIndexBySegmentSourceId (
  IN  VTD_INFO                  *VTdInfo,
  IN  UINT16                    Segment,
  IN  VTD_SOURCE_ID             SourceId,
  OUT VTD_EXT_CONTEXT_ENTRY     **ExtContextEntry,
  OUT VTD_CONTEXT_ENTRY         **ContextEntry
  )
{
  UINTN                         VtdIndex;
  VTD_ROOT_ENTRY                *RootEntryBase;
  VTD_ROOT_ENTRY                *RootEntry;
  VTD_CONTEXT_ENTRY             *ContextEntryTable;
  VTD_CONTEXT_ENTRY             *ThisContextEntry;
  VTD_EXT_ROOT_ENTRY            *ExtRootEntryBase;
  VTD_EXT_ROOT_ENTRY            *ExtRootEntry;
  VTD_EXT_CONTEXT_ENTRY         *ExtContextEntryTable;
  VTD_EXT_CONTEXT_ENTRY         *ThisExtContextEntry;

  for (VtdIndex = 0; VtdIndex < VTdInfo->VTdEngineCount; VtdIndex++) {
    if (GetPciDataIndex (&VTdInfo->VtdUnitInfo[VtdIndex], Segment, SourceId) != (UINTN)-1) {
      DEBUG ((DEBUG_INFO, "Find VtdIndex(0x%x) for S%04x B%02x D%02x F%02x\n", VtdIndex, Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
      break;
    }
  }
  if (VtdIndex >= VTdInfo->VTdEngineCount) {
    for (VtdIndex = 0; VtdIndex < VTdInfo->VTdEngineCount; VtdIndex++) {
      if (Segment != VTdInfo->VtdUnitInfo[VtdIndex].Segment) {
        continue;
      }
      if (VTdInfo->VtdUnitInfo[VtdIndex].PciDeviceInfo.IncludeAllFlag) {
        DEBUG ((DEBUG_INFO, "Find IncludeAllFlag VtdIndex(0x%x) for S%04x B%02x D%02x F%02x\n", VtdIndex, Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
        break;
      }
    }
  }

  if (VtdIndex < VTdInfo->VTdEngineCount) {
    ExtRootEntryBase = (VTD_EXT_ROOT_ENTRY *) (UINTN) VTdInfo->VtdUnitInfo[VtdIndex].ExtRootEntryTable;
    if (ExtRootEntryBase != 0) {
      ExtRootEntry = &ExtRootEntryBase[SourceId.Index.RootIndex];
      ExtContextEntryTable = (VTD_EXT_CONTEXT_ENTRY *) (UINTN) VTD_64BITS_ADDRESS(ExtRootEntry->Bits.LowerContextTablePointerLo, ExtRootEntry->Bits.LowerContextTablePointerHi) ;
      ThisExtContextEntry  = &ExtContextEntryTable[SourceId.Index.ContextIndex];
      if (ThisExtContextEntry->Bits.AddressWidth == 0) {
        DEBUG ((DEBUG_INFO, "ExtContextEntry AddressWidth : 0x%x\n", ThisExtContextEntry->Bits.AddressWidth));
        return (UINTN)-1;
      }
      *ExtContextEntry = ThisExtContextEntry;
      *ContextEntry    = NULL;
    } else {
      RootEntryBase = (VTD_ROOT_ENTRY*) (UINTN) VTdInfo->VtdUnitInfo[VtdIndex].RootEntryTable;
      RootEntry = &RootEntryBase[SourceId.Index.RootIndex];
      ContextEntryTable = (VTD_CONTEXT_ENTRY *) (UINTN) VTD_64BITS_ADDRESS(RootEntry->Bits.ContextTablePointerLo, RootEntry->Bits.ContextTablePointerHi) ;
      ThisContextEntry  = &ContextEntryTable[SourceId.Index.ContextIndex];
      if (ThisContextEntry->Bits.AddressWidth == 0) {
        DEBUG ((DEBUG_INFO, "ContextEntry AddressWidth : 0x%x\n", ThisContextEntry->Bits.AddressWidth));
        return (UINTN)-1;
      }
      *ExtContextEntry = NULL;
      *ContextEntry    = ThisContextEntry;
    }

    return VtdIndex;
  }

  return (UINTN)-1;
}

/**
  Always enable the VTd page attribute for the device.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  Segment           The Segment used to identify a VTd engine.
  @param[in]  SourceId          The SourceId used to identify a VTd engine and table entry.
  @param[in]  MemoryBase        The base of the memory.
  @param[in]  MemoryLimit       The limit of the memory.
  @param[in]  IoMmuAccess       The IOMMU access.

  @retval EFI_SUCCESS           The VTd entry is updated to always enable all DMA access for the specific device.
**/
EFI_STATUS
EnableRmrrPageAttribute (
  IN VTD_INFO                   *VTdInfo,
  IN UINT16                     Segment,
  IN VTD_SOURCE_ID              SourceId,
  IN UINT64                     MemoryBase,
  IN UINT64                     MemoryLimit,
  IN UINT64                     IoMmuAccess
  )
{
  EFI_STATUS Status;
  UINTN                         VtdIndex;
  VTD_EXT_CONTEXT_ENTRY         *ExtContextEntry;
  VTD_CONTEXT_ENTRY             *ContextEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry;
  UINT64                        Pt;

  DEBUG ((DEBUG_INFO, "EnableRmrrPageAttribute (S%04x B%02x D%02x F%02x)\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));

  VtdIndex = FindVtdIndexBySegmentSourceId (VTdInfo, Segment, SourceId, &ExtContextEntry, &ContextEntry);
  if (VtdIndex == (UINTN)-1) {
    DEBUG ((DEBUG_ERROR, "EnableRmrrPageAttribute - Can not locate Pci device (S%04x B%02x D%02x F%02x) !\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
    return EFI_DEVICE_ERROR;
  }

  if (VTdInfo->VtdUnitInfo[VtdIndex].RmrrSecondLevelPagingEntry == 0) {
    DEBUG ((DEBUG_INFO, "CreateSecondLevelPagingEntry - %d\n", VtdIndex));
    VTdInfo->VtdUnitInfo[VtdIndex].RmrrSecondLevelPagingEntry = (UINT32)(UINTN)CreateSecondLevelPagingEntryTable (&VTdInfo->VtdUnitInfo[VtdIndex], NULL, 0, SIZE_4GB, 0);
    if (VTdInfo->VtdUnitInfo[VtdIndex].RmrrSecondLevelPagingEntry == 0) {
      return EFI_OUT_OF_RESOURCES;
    }

    Status =SetSecondLevelPagingAttribute (&VTdInfo->VtdUnitInfo[VtdIndex], (VTD_SECOND_LEVEL_PAGING_ENTRY*)(UINTN)VTdInfo->VtdUnitInfo[VtdIndex].RmrrSecondLevelPagingEntry, MemoryBase, MemoryLimit + 1 - MemoryBase, IoMmuAccess);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  SecondLevelPagingEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *) (UINTN) VTdInfo->VtdUnitInfo[VtdIndex].RmrrSecondLevelPagingEntry;
  Pt = (UINT64) RShiftU64 ((UINT64) (UINTN) SecondLevelPagingEntry, 12);
  if (ExtContextEntry != NULL) {
    ExtContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
    ExtContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
    ExtContextEntry->Bits.DomainIdentifier = ((1 << (UINT8) ((UINTN) VTdInfo->VtdUnitInfo[VtdIndex].CapReg.Bits.ND * 2 + 4)) - 1);
    ExtContextEntry->Bits.Present = 1;
    FlushPageTableMemory (&VTdInfo->VtdUnitInfo[VtdIndex], (UINTN) ExtContextEntry, sizeof(*ExtContextEntry));
  } else if (ContextEntry != NULL) {
    ContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
    ContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64 (Pt, 20);
    ContextEntry->Bits.DomainIdentifier = ((1 << (UINT8) ((UINTN) VTdInfo->VtdUnitInfo[VtdIndex].CapReg.Bits.ND * 2 + 4)) - 1);
    ContextEntry->Bits.Present = 1;
    FlushPageTableMemory (&VTdInfo->VtdUnitInfo[VtdIndex], (UINTN) ContextEntry, sizeof (*ContextEntry));
  }

  return EFI_SUCCESS;
}
