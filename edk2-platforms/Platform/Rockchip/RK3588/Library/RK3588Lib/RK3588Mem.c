/** @file
*
*  Copyright (c) 2014-2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

#include <Soc.h>

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 12

// DDR attributes
#define DDR_ATTRIBUTES_CACHED           ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED         ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

#define RK3588_EXTRA_SYSTEM_MEMORY_BASE  (FixedPcdGet64(PcdLcdDdrFrameBufferBase) + FixedPcdGet64(PcdLcdDdrFrameBufferSize))
#define RK3588_EXTRA_SYSTEM_MEMORY_SIZE  0xA0000000

#define RK3588_DISPLAY_FB_BASE           (FixedPcdGet64(PcdLcdDdrFrameBufferBase))
#define RK3588_DISPLAY_FB_SIZE           (FixedPcdGet64(PcdLcdDdrFrameBufferSize))

STATIC struct RK3588ReservedMemory {
  EFI_PHYSICAL_ADDRESS         Offset;
  EFI_PHYSICAL_ADDRESS         Size;
} RK3588ReservedMemoryBuffer [] = {
  { 0x00000000, 0x00200000 },    // ATF
  { 0x08400000, 0x01000000 },    // TEE OS
  { 0xF0000000, 0x10000000 },    // REG
  { 0x180000000, 0x00001000 },	 // for grub test
};

STATIC
UINT64
EFIAPI
RK3588InitMemorySize (
  IN VOID
  )
{
  return SIZE_4GB;
}

/*++

Routine Description:

  Remove the reserved region from a System Memory Hob that covers it.

Arguments:

  FileHandle  - Handle of the file being invoked.
  PeiServices - Describes the list of possible PEI Services.

--*/
STATIC
VOID
ReserveMemoryRegion (
  IN EFI_PHYSICAL_ADDRESS      ReservedRegionBase,
  IN UINT32                    ReservedRegionSize
  )
{
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributes;
  EFI_PHYSICAL_ADDRESS         ReservedRegionTop;
  EFI_PHYSICAL_ADDRESS         ResourceTop;
  EFI_PEI_HOB_POINTERS         NextHob;
  UINT64                       ResourceLength;

  ReservedRegionTop = ReservedRegionBase + ReservedRegionSize;

  //
  // Search for System Memory Hob that covers the reserved region,
  // and punch a hole in it
  //
  for (NextHob.Raw = GetHobList ();
       NextHob.Raw != NULL;
       NextHob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,
                                 NextHob.Raw)) {

    if ((NextHob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) &&
        (ReservedRegionBase >= NextHob.ResourceDescriptor->PhysicalStart) &&
        (ReservedRegionTop <= NextHob.ResourceDescriptor->PhysicalStart +
                      NextHob.ResourceDescriptor->ResourceLength))
    {
      ResourceAttributes = NextHob.ResourceDescriptor->ResourceAttribute;
      ResourceLength = NextHob.ResourceDescriptor->ResourceLength;
      ResourceTop = NextHob.ResourceDescriptor->PhysicalStart + ResourceLength;

      if (ReservedRegionBase == NextHob.ResourceDescriptor->PhysicalStart) {
        //
        // This region starts right at the start of the reserved region, so we
        // can simply move its start pointer and reduce its length by the same
        // value
        //
        NextHob.ResourceDescriptor->PhysicalStart += ReservedRegionSize;
        NextHob.ResourceDescriptor->ResourceLength -= ReservedRegionSize;

      } else if ((NextHob.ResourceDescriptor->PhysicalStart +
                  NextHob.ResourceDescriptor->ResourceLength) ==
                  ReservedRegionTop) {

        //
        // This region ends right at the end of the reserved region, so we
        // can simply reduce its length by the size of the region.
        //
        NextHob.ResourceDescriptor->ResourceLength -= ReservedRegionSize;

      } else {
        //
        // This region covers the reserved region. So split it into two regions,
        // each one touching the reserved region at either end, but not covering
        // it.
        //
        NextHob.ResourceDescriptor->ResourceLength =
                 ReservedRegionBase - NextHob.ResourceDescriptor->PhysicalStart;

        // Create the System Memory HOB for the remaining region (top of the FD)
        BuildResourceDescriptorHob (EFI_RESOURCE_SYSTEM_MEMORY,
                                    ResourceAttributes,
                                    ReservedRegionTop,
                                    ResourceTop - ReservedRegionTop);
      }

      //
      // Reserve the memory space.
      //
      BuildResourceDescriptorHob (EFI_RESOURCE_MEMORY_RESERVED,
        0,
        ReservedRegionBase,
        ReservedRegionSize);

      break;
    }
    NextHob.Raw = GET_NEXT_HOB (NextHob);
  }
}


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
  UINTN                         Index = 0, Count;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  UINT64                        MemorySize, AdditionalMemorySize;

  MemorySize = PcdGet64 (PcdTotalMemorySize);
  if (MemorySize == 0)
    MemorySize = RK3588InitMemorySize ();

  ResourceAttributes = (
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED
  );

  // Create initial Base Hob for system memory.
  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    ResourceAttributes,
    PcdGet64 (PcdSystemMemoryBase),
    PcdGet64 (PcdSystemMemorySize)
  );

  if (MemorySize >= PcdGet64 (PcdSystemMemorySize)) {
    ResourceAttributes =
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED;
    AdditionalMemorySize = MemorySize - PcdGet64 (PcdSystemMemorySize);
    if (MemorySize > RK3588_PERIPH_BASE)
      AdditionalMemorySize = RK3588_PERIPH_BASE - PcdGet64 (PcdSystemMemorySize);

    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      ResourceAttributes,
      PcdGet64 (PcdSystemMemorySize),
      AdditionalMemorySize);

    if (MemorySize > SIZE_4GB) {
      BuildResourceDescriptorHob (
        EFI_RESOURCE_SYSTEM_MEMORY,
        ResourceAttributes,
        SIZE_4GB,
        MemorySize - SIZE_4GB);
    }
  }

  Count = sizeof (RK3588ReservedMemoryBuffer) / sizeof (struct RK3588ReservedMemory);
  for (Index = 0; Index < Count; Index++)
  	ReserveMemoryRegion(RK3588ReservedMemoryBuffer[Index].Offset, RK3588ReservedMemoryBuffer[Index].Size);

  ASSERT (VirtualMemoryMap != NULL);

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)AllocatePages(EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR) * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
    return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;

  Index = 0;

  // RK3588 SOC peripherals
  VirtualMemoryTable[Index].PhysicalBase    = RK3588_PERIPH_BASE;
  VirtualMemoryTable[Index].VirtualBase     = RK3588_PERIPH_BASE;
  VirtualMemoryTable[Index].Length          = RK3588_PERIPH_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_DEVICE;

  //PCIe 64 BAR space
  VirtualMemoryTable[++Index].PhysicalBase    = 0x940000000;
  VirtualMemoryTable[Index].VirtualBase     = 0x940000000;
  VirtualMemoryTable[Index].Length          = 0x100000000 + 0x1400000;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_DEVICE;

  // DDR - predefined 1GB size
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes      = CacheAttributes;

  // If DDR capacity is 2GB size, append a new entry to fill the gap.
  if (MemorySize >= PcdGet64 (PcdSystemMemorySize)) {
    AdditionalMemorySize = MemorySize - PcdGet64 (PcdSystemMemorySize);
    if (MemorySize > RK3588_PERIPH_BASE)
      AdditionalMemorySize = RK3588_PERIPH_BASE - PcdGet64 (PcdSystemMemorySize);
      
    if (AdditionalMemorySize >= SIZE_1GB) {
      VirtualMemoryTable[++Index].PhysicalBase = PcdGet64 (PcdSystemMemorySize);
      VirtualMemoryTable[Index].VirtualBase    = PcdGet64 (PcdSystemMemorySize);
      VirtualMemoryTable[Index].Length         = AdditionalMemorySize;
      VirtualMemoryTable[Index].Attributes     = CacheAttributes;
    }
  }
  if (MemorySize > SIZE_4GB) {
    VirtualMemoryTable[++Index].PhysicalBase = SIZE_4GB;
    VirtualMemoryTable[Index].VirtualBase    = SIZE_4GB;
    VirtualMemoryTable[Index].Length         = MemorySize - SIZE_4GB;
    VirtualMemoryTable[Index].Attributes     = CacheAttributes;
  }

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase  = 0;
  VirtualMemoryTable[Index].VirtualBase     = 0;
  VirtualMemoryTable[Index].Length          = 0;
  VirtualMemoryTable[Index].Attributes      = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT((Index + 1) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap = VirtualMemoryTable;
}
