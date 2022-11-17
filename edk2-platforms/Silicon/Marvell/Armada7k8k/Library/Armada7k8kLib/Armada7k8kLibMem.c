/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Uefi.h>

#include <IndustryStandard/ArmStdSmc.h>
#include <IndustryStandard/MvSmc.h>

#include <Library/ArmadaBoardDescLib.h>
#include <Library/ArmadaSoCDescLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include "Armada7k8kLibMem.h"

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 16

// DDR attributes
#define DDR_ATTRIBUTES_CACHED           ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED         ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

STATIC ARM_MEMORY_REGION_DESCRIPTOR mVirtualMemoryTable[MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS];

// Obtain DRAM size basing on register values filled by early firmware.
STATIC
UINT64
GetDramSize (
  IN OUT UINT64 *MemSize
  )
{
#if defined(MDE_CPU_AARCH64)
  ARM_SMC_ARGS SmcRegs = {0};
  EFI_STATUS Status;

  SmcRegs.Arg0 = MV_SMC_ID_DRAM_SIZE;
  Status = ArmadaSoCAp8xxBaseGet (&SmcRegs.Arg1, 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ArmCallSmc (&SmcRegs);

  *MemSize = SmcRegs.Arg0;
#else
  //
  // Use fixed value, as currently there is no support
  // in Armada early firmware for 32-bit SMC
  //
  *MemSize = FixedPcdGet64 (PcdSystemMemorySize);
#endif

  return EFI_SUCCESS;
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
  UINTN                         Index = 0;
  UINT64                        MemSize;
  UINT64                        MemLowSize;
  UINT64                        MemHighStart;
  UINT64                        MemHighSize;
  UINT64                        ConfigSpaceBaseAddr;
  UINTN                         PcieControllerCount;
  UINTN                         PcieIndex;
  MV_PCIE_CONTROLLER CONST      *PcieControllers;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  EFI_STATUS                    Status;

  ASSERT (VirtualMemoryMap != NULL);

  ConfigSpaceBaseAddr = FixedPcdGet64 (PcdConfigSpaceBaseAddress);

  // Obtain total memory size from the hardware.
  Status = GetDramSize (&MemSize);
  if (EFI_ERROR (Status)) {
    MemSize = FixedPcdGet64 (PcdSystemMemorySize);
    DEBUG ((DEBUG_ERROR, "Limit total memory size to %d MB\n", MemSize / 1024 / 1024));
  }

  if (DRAM_REMAP_ENABLED) {
    MemLowSize = MIN (DRAM_REMAP_TARGET, MemSize);
    MemHighStart = (UINT64)DRAM_REMAP_TARGET + DRAM_REMAP_SIZE;
    MemHighSize = MemSize - MemLowSize;
  } else {
    MemLowSize = MIN (ConfigSpaceBaseAddr, MemSize);
  }

  ResourceAttributes = (
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED
  );

  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    ResourceAttributes,
    FixedPcdGet64 (PcdSystemMemoryBase),
    MemLowSize
    );

  // DDR
  mVirtualMemoryTable[Index].PhysicalBase    = FixedPcdGet64 (PcdSystemMemoryBase);
  mVirtualMemoryTable[Index].VirtualBase     = FixedPcdGet64 (PcdSystemMemoryBase);
  mVirtualMemoryTable[Index].Length          = MemLowSize;
  mVirtualMemoryTable[Index].Attributes      = DDR_ATTRIBUTES_CACHED;

  // SoC MMIO configuration space
  mVirtualMemoryTable[++Index].PhysicalBase  = ConfigSpaceBaseAddr;
  mVirtualMemoryTable[Index].VirtualBase     = ConfigSpaceBaseAddr;
  mVirtualMemoryTable[Index].Length          = SIZE_4GB - ConfigSpaceBaseAddr;
  mVirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIE ECAM
  Status = ArmadaBoardPcieControllerGet (&PcieControllers, &PcieControllerCount);
  ASSERT_EFI_ERROR (Status);
  for (PcieIndex = 0; PcieIndex < PcieControllerCount; PcieIndex++) {
    mVirtualMemoryTable[++Index].PhysicalBase  = PcieControllers[PcieIndex].ConfigSpaceAddress;
    mVirtualMemoryTable[Index].VirtualBase     = PcieControllers[PcieIndex].ConfigSpaceAddress;
    mVirtualMemoryTable[Index].Length          = (PcieControllers[PcieIndex].ConfigSpaceSize == 0) ?
                                                 SIZE_256MB :
                                                 PcieControllers[PcieIndex].ConfigSpaceSize;
    mVirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  }

  if (MemSize > MemLowSize) {
    //
    // If we have more than MemLowSize worth of DRAM, the remainder will be
    // mapped at the top of the remapped window.
    //
    mVirtualMemoryTable[++Index].PhysicalBase  = MemHighStart;
    mVirtualMemoryTable[Index].VirtualBase     = MemHighStart;
    mVirtualMemoryTable[Index].Length          = MemHighSize;
    mVirtualMemoryTable[Index].Attributes      = DDR_ATTRIBUTES_CACHED;

    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      ResourceAttributes,
      MemHighStart,
      MemHighSize
      );
  }

  // End of Table
  mVirtualMemoryTable[++Index].PhysicalBase  = 0;
  mVirtualMemoryTable[Index].VirtualBase     = 0;
  mVirtualMemoryTable[Index].Length          = 0;
  mVirtualMemoryTable[Index].Attributes      = 0;

  ASSERT((Index + 1) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap = mVirtualMemoryTable;
}
