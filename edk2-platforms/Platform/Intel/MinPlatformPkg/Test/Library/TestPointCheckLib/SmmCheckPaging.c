/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiSmm.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MemoryAttributesTable.h>
#include <Register/SmramSaveStateMap.h>
#include <Register/StmApi.h>
#include <Register/Msr.h>

#define NEXT_MEMORY_DESCRIPTOR(MemoryDescriptor, Size) \
  ((EFI_MEMORY_DESCRIPTOR *)((UINT8 *)(MemoryDescriptor) + (Size)))

///
/// Page Table Entry
///
#define IA32_PG_P                   BIT0
#define IA32_PG_RW                  BIT1
#define IA32_PG_U                   BIT2
#define IA32_PG_WT                  BIT3
#define IA32_PG_CD                  BIT4
#define IA32_PG_A                   BIT5
#define IA32_PG_D                   BIT6
#define IA32_PG_PS                  BIT7
#define IA32_PG_PAT_2M              BIT12
#define IA32_PG_PAT_4K              IA32_PG_PS
#define IA32_PG_PMNT                BIT62
#define IA32_PG_NX                  BIT63

#define PAGING_4K_MASK  0xFFF
#define PAGING_2M_MASK  0x1FFFFF
#define PAGING_1G_MASK  0x3FFFFFFF

#define PAGING_PAE_INDEX_MASK  0x1FF

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

EFI_STATUS
EFIAPI
SmmGetSystemConfigurationTable (
  IN  EFI_GUID  *TableGuid,
  OUT VOID      **Table
  );

/**
  Return page table base.

  @return page table base.
**/
UINTN
GetPageTableBase (
  VOID
  )
{
  return (AsmReadCr3 () & PAGING_4K_ADDRESS_MASK_64);
}

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
  for (Index = 0; Index < sizeof(mPageAttributeTable)/sizeof(mPageAttributeTable[0]); Index++) {
    if (PageAttribute == mPageAttributeTable[Index].Attribute) {
      return (UINTN)mPageAttributeTable[Index].Length;
    }
  }
  return 0;
}

/**
  Return page table entry to match the address.

  @param[in]   Address          The address to be checked.
  @param[out]  PageAttributes   The page attribute of the page entry.

  @return The page entry.
**/
VOID *
GetPageTableEntry (
  IN  PHYSICAL_ADDRESS                  Address,
  OUT PAGE_ATTRIBUTE                    *PageAttribute
  )
{
  UINTN                 Index1;
  UINTN                 Index2;
  UINTN                 Index3;
  UINTN                 Index4;
  UINT64                *L1PageTable;
  UINT64                *L2PageTable;
  UINT64                *L3PageTable;
  UINT64                *L4PageTable;

  Index4 = ((UINTN)RShiftU64 (Address, 39)) & PAGING_PAE_INDEX_MASK;
  Index3 = ((UINTN)Address >> 30) & PAGING_PAE_INDEX_MASK;
  Index2 = ((UINTN)Address >> 21) & PAGING_PAE_INDEX_MASK;
  Index1 = ((UINTN)Address >> 12) & PAGING_PAE_INDEX_MASK;

  if (sizeof(UINTN) == sizeof(UINT64)) {
    L4PageTable = (UINT64 *)GetPageTableBase ();
    if (L4PageTable[Index4] == 0) {
      *PageAttribute = Page1G;
      return NULL;
    }

    L3PageTable = (UINT64 *)(UINTN)(L4PageTable[Index4] & PAGING_4K_ADDRESS_MASK_64);
  } else {
    L3PageTable = (UINT64 *)GetPageTableBase ();
  }
  if (L3PageTable[Index3] == 0) {
    *PageAttribute = Page1G;
    return NULL;
  }
  if ((L3PageTable[Index3] & IA32_PG_PS) != 0) {
    // 1G
    *PageAttribute = Page1G;
    return &L3PageTable[Index3];
  }

  L2PageTable = (UINT64 *)(UINTN)(L3PageTable[Index3] & PAGING_4K_ADDRESS_MASK_64);
  if (L2PageTable[Index2] == 0) {
    *PageAttribute = Page2M;
    return NULL;
  }
  if ((L2PageTable[Index2] & IA32_PG_PS) != 0) {
    // 2M
    *PageAttribute = Page2M;
    return &L2PageTable[Index2];
  }

  // 4k
  L1PageTable = (UINT64 *)(UINTN)(L2PageTable[Index2] & PAGING_4K_ADDRESS_MASK_64);
  if ((L1PageTable[Index1] == 0) && (Address != 0)) {
    *PageAttribute = Page4K;
    return NULL;
  }
  *PageAttribute = Page4K;
  return &L1PageTable[Index1];
}

typedef struct {
  volatile BOOLEAN  Valid;
  volatile UINT64   SmBase;
} SMBASE_SHARED_BUFFER;

VOID
EFIAPI
GetSmBaseOnCurrentProcessor (
  IN OUT VOID  *Buffer
  )
{
  SMBASE_SHARED_BUFFER  *SmBaseBuffer;

  SmBaseBuffer = Buffer;
  SmBaseBuffer->SmBase = AsmReadMsr64 (MSR_IA32_SMBASE);
  SmBaseBuffer->Valid = TRUE;
}

UINT64  *mSmBaseBuffer;

VOID
SetupSmBaseBuffer (
  VOID
  )
{
  volatile SMBASE_SHARED_BUFFER      SmBaseBuffer;
  EFI_STATUS                         Status;
  UINTN                              Index;
  UINTN                              Base = 0;
  UINTN                              Delta = 0;

  if (mSmBaseBuffer != NULL) {
    return ;
  }

  mSmBaseBuffer = AllocatePool (sizeof(UINT64) * gSmst->NumberOfCpus);
  ASSERT(mSmBaseBuffer != NULL);

  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    ZeroMem ((VOID *)&SmBaseBuffer, sizeof(SmBaseBuffer));
    if (Index == gSmst->CurrentlyExecutingCpu) {
      GetSmBaseOnCurrentProcessor ((VOID *)&SmBaseBuffer);
      DEBUG ((DEBUG_INFO, "SmbaseBsp(%d) - 0x%x\n", Index, SmBaseBuffer.SmBase));
    } else {
      Status = gSmst->SmmStartupThisAp (GetSmBaseOnCurrentProcessor, Index, (VOID *)&SmBaseBuffer);
      if (!FeaturePcdGet (PcdCpuHotPlugSupport)) {
        ASSERT_EFI_ERROR (Status);
      }
      if (EFI_ERROR(Status)) {
        SmBaseBuffer.SmBase = Base + Delta * Index;
        DEBUG ((DEBUG_INFO, "SmbaseAp(%d) - unknown, guess - 0x%x\n", Index, SmBaseBuffer.SmBase));
      } else {
        while (!SmBaseBuffer.Valid) {
          CpuPause ();
        }
        DEBUG ((DEBUG_INFO, "SmbaseAp(%d) - 0x%x\n", Index, SmBaseBuffer.SmBase));
      }
    }
    mSmBaseBuffer[Index] = SmBaseBuffer.SmBase;
    if (Base == 0) {
      Base = (UINTN)SmBaseBuffer.SmBase;
      DEBUG ((DEBUG_INFO, "-- Base - 0x%x\n", Base));
    } else if (Delta == 0) {
      Delta = (UINTN)(SmBaseBuffer.SmBase - Base);
      DEBUG ((DEBUG_INFO, "-- Delta - 0x%x\n", Delta));
    }
  }
}

BOOLEAN
IsSmmSaveState (
  IN EFI_PHYSICAL_ADDRESS   BaseAddress
  )
{
  UINTN                              Index;
  UINTN                              TileCodeSize;
  UINTN                              TileDataSize;
  UINTN                              TileSize;

  SetupSmBaseBuffer ();

  TileCodeSize = SIZE_4KB; // BUGBUG: Assume 4KB
  TileCodeSize = ALIGN_VALUE(TileCodeSize, SIZE_4KB);
  TileDataSize = (SMRAM_SAVE_STATE_MAP_OFFSET - TXT_SMM_PSD_OFFSET) + sizeof (SMRAM_SAVE_STATE_MAP);
  TileDataSize = ALIGN_VALUE(TileDataSize, SIZE_4KB);
  TileSize = TileDataSize + TileCodeSize - 1;
  TileSize = 2 * GetPowerOfTwo32 ((UINT32)TileSize);

  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    if (Index == gSmst->NumberOfCpus - 1) {
      TileSize = SIZE_32KB;
    }
    if ((BaseAddress >= mSmBaseBuffer[Index] + SMM_HANDLER_OFFSET + TileCodeSize) &&
        (BaseAddress <  mSmBaseBuffer[Index] + SMM_HANDLER_OFFSET + TileSize)) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
TestPointCheckPageTable (
  IN EFI_PHYSICAL_ADDRESS   BaseAddress,
  IN UINTN                  Length,
  IN BOOLEAN                IsCode,
  IN BOOLEAN                IsOutsideSmram
  )
{
  UINT64                            *PageEntry;
  PAGE_ATTRIBUTE                    PageAttribute;
  UINTN                             PageEntryLength;
  EFI_PHYSICAL_ADDRESS              BaseAddressEnd;

  ASSERT ((BaseAddress & (SIZE_4KB - 1)) == 0);
  ASSERT ((Length & (SIZE_4KB - 1)) == 0);

  BaseAddressEnd = BaseAddress + Length;

  //
  // Below logic is to check 2M/4K page to make sure we donot waist memory.
  //
  while (BaseAddress < BaseAddressEnd) {
    PageEntry = GetPageTableEntry (BaseAddress, &PageAttribute);
    ASSERT (PageAttribute != PageNone);

    if (IsOutsideSmram) {
      if ((PageEntry != NULL) && ((*PageEntry & IA32_PG_P) != 0)) {
        DEBUG ((DEBUG_ERROR, "OutsideSmram present - 0x%x\n", BaseAddress));
        return EFI_INVALID_PARAMETER;
      }
    }

    if (IsCode) {
      if ((PageEntry == NULL) || ((*PageEntry & IA32_PG_P) == 0)) {
        DEBUG ((DEBUG_ERROR, "Code Page not exist - 0x%x\n", BaseAddress));
        return EFI_INVALID_PARAMETER;
      } else if ((*PageEntry & IA32_PG_RW) != 0) {
        //
        // Check if it is SMM SaveState
        //
        if (!IsSmmSaveState (BaseAddress)) {
          DEBUG ((DEBUG_ERROR, "Code Page read write - 0x%x\n", BaseAddress));
          return EFI_INVALID_PARAMETER;
        }
      }
    } else {
      if ((PageEntry == NULL) || ((*PageEntry & IA32_PG_P) == 0)) {
        // Pass
      } else if ((*PageEntry & IA32_PG_NX) == 0) {
        DEBUG ((DEBUG_ERROR, "Data Page executable - 0x%x\n", BaseAddress));
        return EFI_INVALID_PARAMETER;
      }
    }

    PageEntryLength = PageAttributeToLength (PageAttribute);
    PageEntryLength = (UINTN)((BaseAddress & ~(PageEntryLength - 1)) + PageEntryLength - BaseAddress);

    //
    // move to next
    //
    BaseAddress += PageEntryLength;
    Length -= PageEntryLength;
  }

  return RETURN_SUCCESS;
}

EFI_STATUS
TestPointCheckPagingWithMemoryAttributesTable (
  IN EFI_MEMORY_ATTRIBUTES_TABLE                     *MemoryAttributesTable
  )
{
  UINTN                 Index;
  EFI_MEMORY_DESCRIPTOR *Entry;
  EFI_STATUS            Status;
  EFI_STATUS            ReturnStatus;

  ReturnStatus = EFI_SUCCESS;
  Entry = (EFI_MEMORY_DESCRIPTOR *)(MemoryAttributesTable + 1);
  for (Index = 0; Index < MemoryAttributesTable->NumberOfEntries; Index++) {
    DEBUG ((DEBUG_INFO, "SmmMemoryAttribute Checking 0x%lx - 0x%x\n", Entry->PhysicalStart, EFI_PAGES_TO_SIZE((UINTN)Entry->NumberOfPages)));
    Status = TestPointCheckPageTable (
               Entry->PhysicalStart,
               EFI_PAGES_TO_SIZE((UINTN)Entry->NumberOfPages),
               ((Entry->Attribute & EFI_MEMORY_RO) == 0) ? FALSE : TRUE,
               FALSE
               );
    if (EFI_ERROR(Status)) {
      ReturnStatus = Status;
    }
    Entry = NEXT_MEMORY_DESCRIPTOR (Entry, MemoryAttributesTable->DescriptorSize);
  }

  return ReturnStatus;
}

EFI_STATUS
TestPointCheckSmmPaging (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *MemoryAttributesTable;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmPaging - Enter\n"));

  Status = SmmGetSystemConfigurationTable (&gEdkiiPiSmmMemoryAttributesTableGuid, (VOID **)&MemoryAttributesTable);
  if (!EFI_ERROR (Status)) {
    Status = TestPointCheckPagingWithMemoryAttributesTable(MemoryAttributesTable);
  }

  if (EFI_ERROR (Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION_ERROR_CODE \
        TEST_POINT_SMM_READY_TO_BOOT \
        TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION_ERROR_STRING
      );
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmPaging - Exit\n"));
  return EFI_SUCCESS;
}
