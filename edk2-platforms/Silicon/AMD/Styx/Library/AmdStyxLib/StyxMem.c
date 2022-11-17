/** @file
*
*  Copyright (c) 2011-2014, ARM Limited. All rights reserved.<BR>
*  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
/**
  Derived from:
   ArmPlatformPkg/ArmVExpressPkg/Library/ArmVExpressLibRTSM/RTSMMem.c

**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>

#if !defined(MDEPKG_NDEBUG)

static const char *tblAttrDesc[] =
{
  "UNCACHED_UNBUFFERED          ",
  "NONSECURE_UNCACHED_UNBUFFERED",
  "WRITE_BACK                   ",
  "NONSECURE_WRITE_BACK         ",
  "WRITE_THROUGH                ",
  "NONSECURE_WRITE_THROUGH      ",
  "DEVICE                       ",
  "NONSECURE_DEVICE             "
};
#endif

#define LOG_MEM(desc) DEBUG ((EFI_D_ERROR, desc, VirtualMemoryTable[Index].PhysicalBase, \
                            ( VirtualMemoryTable[Index].PhysicalBase+VirtualMemoryTable[Index].Length - 1), \
                             VirtualMemoryTable[Index].Length, tblAttrDesc[VirtualMemoryTable[Index].Attributes]));


// Number of Virtual Memory Map Descriptors
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS          16

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
  UINTN                         Index = 0;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;

  ASSERT(VirtualMemoryMap != NULL);

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)AllocatePages(EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR) * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
      return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;

  DEBUG ((EFI_D_ERROR, " Memory Map\n------------------------------------------------------------------------\n"));
  DEBUG ((EFI_D_ERROR, "Description                    :        START       -        END         [        SIZE        ]    {              ATTR             }\n"));

  // 0xE000_0000 - 0xEFFF_FFFF: Mapped I/O space
  VirtualMemoryTable[Index].PhysicalBase   = 0xE0000000UL;
  VirtualMemoryTable[Index].VirtualBase    = 0xE0000000UL;
  VirtualMemoryTable[Index].Length         = SIZE_256MB;
  VirtualMemoryTable[Index].Attributes     = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("I/O Space [Platform MMIO]      : 0x%016lx - 0x%016lx [ 0x%016lx ]    { %a }\n");

  // 0xF000_0000 - 0xFFFF_FFFF: PCI config space
  VirtualMemoryTable[++Index].PhysicalBase = 0xF0000000UL;
  VirtualMemoryTable[Index].VirtualBase    = 0xF0000000UL;
  VirtualMemoryTable[Index].Length         = SIZE_256MB;
  VirtualMemoryTable[Index].Attributes     = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("I/O Space [PCI config space]   : 0x%016lx - 0x%016lx [ 0x%016lx ]    { %a }\n");

  // DRAM
  VirtualMemoryTable[++Index].PhysicalBase = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase    = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].Length         = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes     = CacheAttributes;
  LOG_MEM ("DRAM                           : 0x%016lx - 0x%016lx [ 0x%016lx ]    { %a }\n");

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase = 0;
  VirtualMemoryTable[Index].VirtualBase    = 0;
  VirtualMemoryTable[Index].Length         = 0;
  VirtualMemoryTable[Index].Attributes     = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  *VirtualMemoryMap = VirtualMemoryTable;
}
