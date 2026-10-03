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
#include <Library/SdramLib.h>

UINT64         mSystemMemoryBase = FixedPcdGet64 (PcdSystemMemoryBase);
STATIC UINT64  mSystemMemorySize = FixedPcdGet64 (PcdSystemMemorySize);

// Max number of System RAM regions above 4GB taken from the DDR_MEM ATAG.
#define MAX_HIGH_MEMORY_BANKS  8

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS  (12 + MAX_HIGH_MEMORY_BANKS - 1)

STATIC BOOLEAN                    VirtualMemoryInfoInitialized = FALSE;
STATIC RK3588_MEMORY_REGION_INFO  VirtualMemoryInfo[MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS];

#define VariablesBase  FixedPcdGet64(PcdFlashNvStorageVariableBase64)

#define VariablesSize  (FixedPcdGet32(PcdFlashNvStorageVariableSize)   +\
                       FixedPcdGet32(PcdFlashNvStorageFtwWorkingSize) + \
                       FixedPcdGet32(PcdFlashNvStorageFtwSpareSize))

/**
  Add the System RAM above 4GB as reported by the DDR_MEM ATAG.

  Some DDR configurations (e.g. 12GB made of 4 x 3GB channels) leave holes
  above 4GB, so that RAM cannot be assumed to be contiguous.

  @param[in,out] VirtualMemoryTable  The table to append the regions to.
  @param[in,out] Index               Next free entry in the table.

  @retval TRUE   The regions were added.
  @retval FALSE  No usable ATAG was found and nothing was added.
**/
STATIC
BOOLEAN
AddHighMemoryFromAtags (
  IN OUT ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable,
  IN OUT UINTN                         *Index
  )
{
  RKATAG_DDR_MEM  *DdrMem;
  UINT32          Bank;
  UINT32          HighBanks;
  UINT64          Start;
  UINT64          End;
  UINT64          HighSize;

  DdrMem = RkAtagsGetDdrMem ();
  if ((DdrMem == NULL) || (DdrMem->Count == 0) || (DdrMem->Count > ARRAY_SIZE (DdrMem->Bank) / 2)) {
    return FALSE;
  }

  //
  // Bank[] holds all the base addresses first, followed by all the sizes.
  //
  HighBanks = 0;
  HighSize  = 0;
  for (Bank = 0; Bank < DdrMem->Count; Bank++) {
    Start = DdrMem->Bank[Bank];
    End   = Start + DdrMem->Bank[Bank + DdrMem->Count];
    DEBUG ((DEBUG_INFO, "DDR bank %u: 0x%lx - 0x%lx\n", Bank, Start, End));

    if (End == Start) {
      continue;
    }

    if ((End < Start) || (((Start | End) & EFI_PAGE_MASK) != 0) || (End > 0x0000000900000000UL)) {
      DEBUG ((DEBUG_WARN, "DDR_MEM ATAG looks invalid, ignoring it.\n"));
      return FALSE;
    }

    if (End > 0x100000000UL) {
      HighBanks++;
      HighSize += End - MAX (Start, 0x100000000UL);
    }
  }

  if ((HighBanks == 0) || (HighBanks > MAX_HIGH_MEMORY_BANKS) || (HighSize > mSystemMemorySize)) {
    return FALSE;
  }

  for (Bank = 0; Bank < DdrMem->Count; Bank++) {
    Start = MAX (DdrMem->Bank[Bank], 0x100000000UL);
    End   = DdrMem->Bank[Bank] + DdrMem->Bank[Bank + DdrMem->Count];
    if (End <= Start) {
      continue;
    }

    VirtualMemoryTable[*Index].PhysicalBase = Start;
    VirtualMemoryTable[*Index].VirtualBase  = Start;
    VirtualMemoryTable[*Index].Length       = End - Start;
    VirtualMemoryTable[*Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    VirtualMemoryInfo[*Index].Type          = RK3588_MEM_BASIC_REGION;
    VirtualMemoryInfo[(*Index)++].Name      = L"System RAM >= 4GB";
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

  mSystemMemorySize = SdramGetMemorySize ();
  DEBUG ((DEBUG_INFO, "RAM: 0x%ll08X (Size 0x%ll08X)\n", mSystemMemoryBase, mSystemMemorySize));

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
  VirtualMemoryTable[Index].Length       = MIN (mSystemMemorySize, 0x08400000 - VirtualMemoryTable[Index].PhysicalBase);
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_BASIC_REGION;
  VirtualMemoryInfo[Index++].Name        = L"System RAM (< OP-TEE)";

  // OP-TEE Region
  VirtualMemoryTable[Index].PhysicalBase = 0x08400000;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = 0x1000000;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_RESERVED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"OP-TEE";

  // Base System RAM (< 4GB)
  VirtualMemoryTable[Index].PhysicalBase = 0x08400000 + 0x1000000;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = MIN (mSystemMemorySize, 0xF0000000 - VirtualMemoryTable[Index].PhysicalBase);
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_BASIC_REGION;
  VirtualMemoryInfo[Index++].Name        = L"System RAM (< 4GB)";

  // MMIO
  VirtualMemoryTable[Index].PhysicalBase = 0xF0000000;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = 0x10000000;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_UNMAPPED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"MMIO";

  if (!AddHighMemoryFromAtags (VirtualMemoryTable, &Index) &&
      (mSystemMemorySize > 0x100000000UL))
  {
    // Base System RAM >= 4GB
    VirtualMemoryTable[Index].PhysicalBase = 0x100000000;
    VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
    VirtualMemoryTable[Index].Length       = mSystemMemorySize - 0x100000000;
    VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    VirtualMemoryInfo[Index].Type          = RK3588_MEM_BASIC_REGION;
    VirtualMemoryInfo[Index++].Name        = L"System RAM >= 4GB";
  }

  // MMIO > 32GB
  VirtualMemoryTable[Index].PhysicalBase = 0x0000000900000000UL;
  VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
  VirtualMemoryTable[Index].Length       = 0x0000000141400000UL;
  VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  VirtualMemoryInfo[Index].Type          = RK3588_MEM_UNMAPPED_REGION;
  VirtualMemoryInfo[Index++].Name        = L"MMIO > 32GB";

  if (mSystemMemoryBase + mSystemMemorySize > 0x3fc000000UL) {
    // Bad memory range 1
    VirtualMemoryTable[Index].PhysicalBase = 0x3fc000000;
    VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
    VirtualMemoryTable[Index].Length       = 0x500000;
    VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
    VirtualMemoryInfo[Index].Type          = RK3588_MEM_RESERVED_REGION;
    VirtualMemoryInfo[Index++].Name        = L"BAD1";

    // Bad memory range 2
    VirtualMemoryTable[Index].PhysicalBase = 0x3fff00000;
    VirtualMemoryTable[Index].VirtualBase  = VirtualMemoryTable[Index].PhysicalBase;
    VirtualMemoryTable[Index].Length       = 0x100000;
    VirtualMemoryTable[Index].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
    VirtualMemoryInfo[Index].Type          = RK3588_MEM_RESERVED_REGION;
    VirtualMemoryInfo[Index++].Name        = L"BAD2";
  }

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
