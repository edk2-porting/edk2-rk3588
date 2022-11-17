/** @file
  Library of memory map for Phytium platform.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ArmSmcLib.h>
#include <SystemServiceInterface.h>

// Number of Virtual Memory Map Descriptors
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 32

// DDR attributes
#define DDR_ATTRIBUTES_CACHED      ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED    ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU on your platform.

  @param[out]  VirtualMemoryMap  Array of ARM_MEMORY_REGION_DESCRIPTOR describing a Physical-to-
                                 Virtual Memory mapping. This array must be ended by a zero-filled
                                 entry
**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  PHYTIUM_MEMORY_BLOCK          *MemBlock;
  PHYTIUM_MEMORY_INFO           *MemInfo;
  ARM_SMC_ARGS                  ArmSmcArgs;
  UINT32                        MemBlockCnt;
  UINT32                        Index1;
  UINT32                        Index2;

  MemBlock = NULL;
  MemInfo  = NULL;
  MemBlockCnt = 0;
  Index1      = 0;
  Index2      = 0;
  CacheAttributes = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  ASSERT (VirtualMemoryMap != NULL);
  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)AllocatePages \
                         (EFI_SIZE_TO_PAGES (sizeof (ARM_MEMORY_REGION_DESCRIPTOR) * \
                         MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
    return;
  }

  ResourceAttributes =
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED;

  MemInfo = AllocatePages (1);
  ASSERT (MemInfo != NULL);

  ArmSmcArgs.Arg0 = PHYTIUM_OEM_SVC_MEM_REGIONS;
  ArmSmcArgs.Arg1 = (UINTN) MemInfo;
  ArmSmcArgs.Arg2 = EFI_PAGE_SIZE;
  ArmCallSmc (&ArmSmcArgs);
  if (ArmSmcArgs.Arg0 == 0) {
    MemBlockCnt = MemInfo->MemBlockCount;
    MemBlock = MemInfo->MemBlock;
  } else {
    ASSERT (FALSE);
  }

  //Soc Io Space
  VirtualMemoryTable[Index1].PhysicalBase   = PcdGet64 (PcdSystemIoBase);
  VirtualMemoryTable[Index1].VirtualBase    = PcdGet64 (PcdSystemIoBase);
  VirtualMemoryTable[Index1].Length         = PcdGet64 (PcdSystemIoSize);
  VirtualMemoryTable[Index1].Attributes     = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  //
  // PCI Configuration Space
  //
  VirtualMemoryTable[++Index1].PhysicalBase  = PcdGet64 (PcdPciConfigBase);
  VirtualMemoryTable[Index1].VirtualBase     = PcdGet64 (PcdPciConfigBase);
  VirtualMemoryTable[Index1].Length          = PcdGet64 (PcdPciConfigSize);
  VirtualMemoryTable[Index1].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  //
  // PCI Memory Space
  //
  VirtualMemoryTable[++Index1].PhysicalBase  = PcdGet64 (PcdPciIoBase) + PcdGet64 (PcdPciIoTranslation);
  VirtualMemoryTable[Index1].VirtualBase     = PcdGet64 (PcdPciIoBase) + PcdGet64 (PcdPciIoTranslation);
  VirtualMemoryTable[Index1].Length          = PcdGet64 (PcdPciIoSize);
  VirtualMemoryTable[Index1].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  //
  // PCI Memory Space
  //
  VirtualMemoryTable[++Index1].PhysicalBase  = PcdGet32 (PcdPciMmio32Base);
  VirtualMemoryTable[Index1].VirtualBase     = PcdGet32 (PcdPciMmio32Base);
  VirtualMemoryTable[Index1].Length          = PcdGet32 (PcdPciMmio32Size);
  VirtualMemoryTable[Index1].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  //
  // 64-bit PCI Memory Space
  //
  VirtualMemoryTable[++Index1].PhysicalBase  = PcdGet64 (PcdPciMmio64Base);
  VirtualMemoryTable[Index1].VirtualBase     = PcdGet64 (PcdPciMmio64Base);
  VirtualMemoryTable[Index1].Length          = PcdGet64 (PcdPciMmio64Size);
  VirtualMemoryTable[Index1].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  //DDR
  for (Index2 = 0; Index2 < MemBlockCnt; Index2++) {
    VirtualMemoryTable[++Index1].PhysicalBase = MemBlock->MemStart;
    VirtualMemoryTable[Index1].VirtualBase    = MemBlock->MemStart;
    VirtualMemoryTable[Index1].Length         = MemBlock->MemSize;
    VirtualMemoryTable[Index1].Attributes     = CacheAttributes;

    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      ResourceAttributes,
      MemBlock->MemStart,
      MemBlock->MemSize
      );

    MemBlock++;
  }

  // End of Table
  VirtualMemoryTable[++Index1].PhysicalBase = 0;
  VirtualMemoryTable[Index1].VirtualBase    = 0;
  VirtualMemoryTable[Index1].Length         = 0;
  VirtualMemoryTable[Index1].Attributes     = (ARM_MEMORY_REGION_ATTRIBUTES)0;


  for (Index2 = 0; Index2 < Index1; Index2++) {
    DEBUG ((DEBUG_ERROR, "PhysicalBase %12lx VirtualBase %12lx Length %12lx Attributes %12lx\n",\
      VirtualMemoryTable[Index2].PhysicalBase, VirtualMemoryTable[Index2].VirtualBase, \
      VirtualMemoryTable[Index2].Length, VirtualMemoryTable[Index2].Attributes));
  }

  *VirtualMemoryMap = VirtualMemoryTable;
}
