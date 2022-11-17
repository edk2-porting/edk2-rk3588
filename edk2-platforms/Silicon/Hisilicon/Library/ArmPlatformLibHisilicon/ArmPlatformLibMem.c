/** @file
*
*  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under ArmPlatformPkg/ArmVExpressPkg/Library/ArmVExpressLibRTSM/
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/OemSetVirtualMapDesc.h>

#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS          32

// DDR attributes
#define DDR_ATTRIBUTES_CACHED           ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED         ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR describing a Physical-to-
                                    Virtual Memory mapping. This array must be ended by a zero-filled
                                    entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes;
  UINTN                         Index;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  EFI_PEI_HOB_POINTERS          NextHob;

  ASSERT (VirtualMemoryMap != NULL);

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)AllocatePages(EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR) * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
      return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;

  Index = OemSetVirtualMapDesc(VirtualMemoryTable, CacheAttributes);

  // Search for System Memory Hob that contains the EFI resource system memory
  NextHob.Raw = GetHobList ();
  while ((NextHob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, NextHob.Raw)) != NULL)
  {
    if (NextHob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
    {
        if (NextHob.ResourceDescriptor->PhysicalStart > BASE_4GB)
        {
            VirtualMemoryTable[++Index].PhysicalBase = NextHob.ResourceDescriptor->PhysicalStart;
            VirtualMemoryTable[Index].VirtualBase  = NextHob.ResourceDescriptor->PhysicalStart;
            VirtualMemoryTable[Index].Length       =NextHob.ResourceDescriptor->ResourceLength;
            VirtualMemoryTable[Index].Attributes   =  CacheAttributes;
        }
    }

    NextHob.Raw = GET_NEXT_HOB (NextHob);
  }

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase = 0;
  VirtualMemoryTable[Index].VirtualBase  = 0;
  VirtualMemoryTable[Index].Length       = 0;
  VirtualMemoryTable[Index].Attributes   = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT((Index + 1) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  DEBUG((EFI_D_INFO, "[%a]:[%dL] discriptor count=%d\n", __FUNCTION__, __LINE__, Index+1));

  *VirtualMemoryMap = VirtualMemoryTable;
}
