/** @file
 *
 *  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2017-2021, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2019, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2014, Linaro Limited. All rights reserved.
 *  Copyright (c) 2013-2018, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/Rk3588Mem.h>
#include <Library/RkAtagsLib.h>

// Max number of DRAM banks in the DDR_MEM ATAG.
#define MAX_DRAM_BANKS  10

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS  (8 + MAX_DRAM_BANKS)

STATIC BOOLEAN                    VirtualMemoryInfoInitialized = FALSE;
STATIC RK3588_MEMORY_REGION_INFO  VirtualMemoryInfo[MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS];

#define VariablesBase  FixedPcdGet64(PcdFlashNvStorageVariableBase64)

#define VariablesSize  (FixedPcdGet32(PcdFlashNvStorageVariableSize)   +\
                       FixedPcdGet32(PcdFlashNvStorageFtwWorkingSize) + \
                       FixedPcdGet32(PcdFlashNvStorageFtwSpareSize))

#define OpteeBase  0x08400000
#define OpteeSize  0x1000000

/**
  Add the System RAM reported by the DDR init blob in the DDR_MEM ATAG.

  The DRAM banks can have holes, e.g. at 0x180000000 on 12GB (4 x 3GB
  channels), or at 0x3FC000000 and 0x3FFF00000 on 16GB.

  @param[in,out] VirtualMemoryTable  The table to append the regions to.
  @param[in,out] Index               Next free entry in the table.

  @retval TRUE   The regions were added.
  @retval FALSE  The ATAG is missing or invalid and nothing was added.
**/
STATIC
BOOLEAN
AddSystemRamFromAtags (
  IN OUT ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable,
  IN OUT UINTN                         *Index
  )
{
  RKATAG_DDR_MEM  *DdrMem;
  UINT32          Bank;
  UINT64          Start;
  UINT64          End;
  UINT64          PrevEnd;

  DdrMem = RkAtagsGetDdrMem ();
  if ((DdrMem == NULL) || (DdrMem->Count == 0) || (DdrMem->Count > MAX_DRAM_BANKS)) {
    return FALSE;
  }

  //
  // Bank[] holds all the base addresses first, followed by all the sizes.
  // The banks must be in order and stay clear of the MMIO windows.
  //
  PrevEnd = 0;
  for (Bank = 0; Bank < DdrMem->Count; Bank++) {
    Start = DdrMem->Bank[Bank];
    End   = Start + DdrMem->Bank[Bank + DdrMem->Count];
    DEBUG ((DEBUG_INFO, "DDR bank %u: 0x%lx - 0x%lx\n", Bank, Start, End));

    if ((End <= Start) || (Start < PrevEnd) || (((Start | End) & EFI_PAGE_MASK) != 0) ||
        ((Start < 0x100000000UL) && (End > 0xF0000000)) || (End > 0x0000000900000000UL))
    {
      return FALSE;
    }

    PrevEnd = End;
  }

  //
  // The first bank must also hold the firmware regions below OP-TEE's end,
  // which are mapped separately.
  //
  if ((DdrMem->Bank[0] != 0) || (DdrMem->Bank[DdrMem->Count] <= OpteeBase + OpteeSize)) {
    return FALSE;
  }

  for (Bank = 0; Bank < DdrMem->Count; Bank++) {
    Start = MAX (DdrMem->Bank[Bank], OpteeBase + OpteeSize);
    End   = DdrMem->Bank[Bank] + DdrMem->Bank[Bank + DdrMem->Count];

    VirtualMemoryTable[*Index].PhysicalBase = Start;
    VirtualMemoryTable[*Index].VirtualBase  = Start;
    VirtualMemoryTable[*Index].Length       = End - Start;
    VirtualMemoryTable[*Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    VirtualMemoryInfo[*Index].Type          = RK3588_MEM_BASIC_REGION;
    VirtualMemoryInfo[(*Index)++].Name      = (Start < 0x100000000UL) ? L"System RAM (< 4GB)" : L"System RAM >= 4GB";
  }

  return TRUE;
}

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR
                                    describing a Physical-to-Virtual Memory
                                    mapping. This array must be ended by a
                                    zero-filled entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR  **VirtualMemoryMap
  )
{
  UINTN                         Index = 0;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR *)AllocatePages (
                                                         EFI_SIZE_TO_PAGES (
                                                           sizeof (ARM_MEMORY_REGION_DESCRIPTOR) *
                                                           MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS
                                                           )
                                                         );
  if (VirtualMemoryTable == NULL) {
    return;
  }

  //
  // TF-A Region
  // Must be unmapped for the shared memory to retain its attributes.
  //
  VirtualMemoryTable[Index].PhysicalBase = 0x00000000;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = 0x200000;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_UNMAPPED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"TF-A + Shared Memory";

  // Firmware Volume
  VirtualMemoryTable[Index].PhysicalBase = FixedPcdGet64 (PcdFvBaseAddress);
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = FixedPcdGet32 (PcdFvSize);
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_RESERVED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"UEFI FV";

  // Variable Volume
  VirtualMemoryTable[Index].PhysicalBase = VariablesBase;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = VariablesSize;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_RUNTIME_REGION;
  VirtualMemoryInfo[Index++].Name        = L"Variable Store";

  // Base System RAM (< OP-TEE)
  VirtualMemoryTable[Index].PhysicalBase = VariablesBase + VariablesSize;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = OpteeBase - VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_BASIC_REGION;
  VirtualMemoryInfo[Index++].Name        = L"System RAM (< OP-TEE)";

  // OP-TEE Region
  VirtualMemoryTable[Index].PhysicalBase = OpteeBase;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = OpteeSize;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_RESERVED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"OP-TEE";

  if (!AddSystemRamFromAtags (VirtualMemoryTable, &Index)) {
    DEBUG ((
      DEBUG_ERROR,
      "No valid DDR_MEM ATAG, mapping only the first 0x%lx bytes of RAM!\n",
      FixedPcdGet64 (PcdSystemMemorySize)
      ));

    // Base System RAM (< PcdSystemMemorySize)
    VirtualMemoryTable[Index].PhysicalBase = OpteeBase + OpteeSize;
    VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
    VirtualMemoryTable[Index].Length       = FixedPcdGet64 (PcdSystemMemorySize) - (OpteeBase + OpteeSize);
    VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    VirtualMemoryInfo[Index].Type          = RK3588_MEM_BASIC_REGION;
    VirtualMemoryInfo[Index++].Name        = L"System RAM (< 4GB)";
  }

  // MMIO
  VirtualMemoryTable[Index].PhysicalBase = 0xF0000000;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = 0x10000000;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_UNMAPPED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"MMIO";

  // MMIO > 32GB
  VirtualMemoryTable[Index].PhysicalBase = 0x0000000900000000UL;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = 0x0000000141400000UL;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_UNMAPPED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"MMIO > 32GB";

  // End of Table
  VirtualMemoryTable[Index].PhysicalBase = 0;
  VirtualMemoryTable[Index].VirtualBase  = 0;
  VirtualMemoryTable[Index].Length       = 0;
  VirtualMemoryTable[Index++].Attributes = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT (Index <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap            = VirtualMemoryTable;
  VirtualMemoryInfoInitialized = TRUE;
}

/**
  Return additional memory info not populated by the above call.

  This call should follow the one to ArmPlatformGetVirtualMemoryMap ().

**/
VOID
Rk3588PlatformGetVirtualMemoryInfo (
  IN RK3588_MEMORY_REGION_INFO  **MemoryInfo
  )
{
  ASSERT (VirtualMemoryInfo != NULL);

  if (!VirtualMemoryInfoInitialized) {
    DEBUG ((
      DEBUG_ERROR,
      "ArmPlatformGetVirtualMemoryMap must be called before Rk3588PlatformGetVirtualMemoryInfo.\n"
      ));
    return;
  }

  *MemoryInfo = VirtualMemoryInfo;
}
