/** @file
*
*  Copyright (c) 2011-2021, Arm Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <ArmPlatform.h>

#define DP_BASE_DESCRIPTOR      ((FixedPcdGet64 (PcdArmMaliDpBase) != 0) ? 1 : 0)

// Number of Virtual Memory Map Descriptors
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS (14 + DP_BASE_DESCRIPTOR)

// DDR attributes
#define DDR_ATTRIBUTES_CACHED   ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize
  the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR
                                    describing a Physical-to-Virtual Memory
                                    mapping. This array must be ended by a
                                    zero-filled entry.

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  UINTN                         Index = 0;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  UINT32                        SysId;
  BOOLEAN                       HasSparseMemory;
  EFI_VIRTUAL_ADDRESS           SparseMemoryBase;
  UINT64                        SparseMemorySize;

  ASSERT (VirtualMemoryMap != NULL);

  // The FVP model has Sparse memory
  SysId = MmioRead32 (ARM_VE_SYS_ID_REG);
  if (SysId != ARM_RTSM_SYS_ID) {
    HasSparseMemory = TRUE;

    ResourceAttributes =
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED;

    // Declared the additional DRAM from 2GB to 4GB
    SparseMemoryBase = 0x0880000000;
    SparseMemorySize = SIZE_2GB;

    BuildResourceDescriptorHob (
        EFI_RESOURCE_SYSTEM_MEMORY,
        ResourceAttributes,
        SparseMemoryBase,
        SparseMemorySize);
  } else {
    HasSparseMemory = FALSE;
    SparseMemoryBase = 0x0;
    SparseMemorySize = 0x0;
  }

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)
    AllocatePages (EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR)
                                      * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
    return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;

  // ReMap (Either NOR Flash or DRAM)
  VirtualMemoryTable[Index].PhysicalBase = ARM_VE_REMAP_BASE;
  VirtualMemoryTable[Index].VirtualBase = ARM_VE_REMAP_BASE;
  VirtualMemoryTable[Index].Length = ARM_VE_REMAP_SZ;
  VirtualMemoryTable[Index].Attributes = CacheAttributes;

  // DDR
  VirtualMemoryTable[++Index].PhysicalBase = ARM_VE_DRAM_BASE;
  VirtualMemoryTable[Index].VirtualBase = ARM_VE_DRAM_BASE;
  VirtualMemoryTable[Index].Length = ARM_VE_DRAM_SZ;
  VirtualMemoryTable[Index].Attributes = CacheAttributes;

  // CPU peripherals. TRM. Manual says not all of them are implemented.
  VirtualMemoryTable[++Index].PhysicalBase = ARM_VE_ON_CHIP_PERIPH_BASE;
  VirtualMemoryTable[Index].VirtualBase = ARM_VE_ON_CHIP_PERIPH_BASE;
  VirtualMemoryTable[Index].Length = ARM_VE_ON_CHIP_PERIPH_SZ;
  VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SMB CS0-CS1 - NOR Flash 1 & 2
  VirtualMemoryTable[++Index].PhysicalBase = ARM_VE_SMB_NOR0_BASE;
  VirtualMemoryTable[Index].VirtualBase = ARM_VE_SMB_NOR0_BASE;
  VirtualMemoryTable[Index].Length = ARM_VE_SMB_NOR0_SZ + ARM_VE_SMB_NOR1_SZ;
  VirtualMemoryTable[Index].Attributes = CacheAttributes;

  // SMB CS2 - SRAM
  VirtualMemoryTable[++Index].PhysicalBase = ARM_VE_SMB_SRAM_BASE;
  VirtualMemoryTable[Index].VirtualBase = ARM_VE_SMB_SRAM_BASE;
  VirtualMemoryTable[Index].Length = ARM_VE_SMB_SRAM_SZ;
  VirtualMemoryTable[Index].Attributes = CacheAttributes;

  // Peripheral CS2 and CS3
  VirtualMemoryTable[++Index].PhysicalBase = ARM_VE_SMB_PERIPH_BASE;
  VirtualMemoryTable[Index].VirtualBase = ARM_VE_SMB_PERIPH_BASE;
  VirtualMemoryTable[Index].Length = 2 * ARM_VE_SMB_PERIPH_SZ;
  VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // Map region for the framebuffer in the system RAM if no VRAM present
  if (FixedPcdGet32 (PcdArmLcdDdrFrameBufferBase) == 0) {
    // VRAM
    VirtualMemoryTable[++Index].PhysicalBase = PL111_CLCD_VRAM_MOTHERBOARD_BASE;
    VirtualMemoryTable[Index].VirtualBase = PL111_CLCD_VRAM_MOTHERBOARD_BASE;
    VirtualMemoryTable[Index].Length = PL111_CLCD_VRAM_MOTHERBOARD_SIZE;
    //
    // Map the VRAM region as Normal Non-Cacheable memory and not device memory,
    // so that we can use the accelerated string routines that may use unaligned
    // accesses or DC ZVA instructions. The enum identifier is slightly awkward
    // here, but it maps to a memory type that allows buffering and reordering.
    //
    VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;

  } else {
    ASSERT ((ARM_VE_DRAM_BASE + ARM_VE_DRAM_SZ - 1) <
            FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase));
    VirtualMemoryTable[++Index].PhysicalBase = FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase);
    VirtualMemoryTable[Index].VirtualBase = FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase);
    VirtualMemoryTable[Index].Length = FixedPcdGet32 (PcdArmLcdDdrFrameBufferSize);
    // Map as Normal Non-Cacheable memory, so that we can use the accelerated
    // SetMem/CopyMem routines that may use unaligned accesses or
    // DC ZVA instructions. If mapped as device memory, these routine may cause
    // alignment faults.
    // NOTE: The attribute value is misleading, it indicates memory map type as
    // an un-cached, un-buffered but allows buffering and reordering.
    VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
  }

  if (FixedPcdGet64 (PcdArmMaliDpBase) != 0) {
    // DP500/DP550/DP650 peripheral memory region
    VirtualMemoryTable[++Index].PhysicalBase = FixedPcdGet64 (PcdArmMaliDpBase);
    VirtualMemoryTable[Index].VirtualBase = FixedPcdGet64 (PcdArmMaliDpBase);
    VirtualMemoryTable[Index].Length = FixedPcdGet32 (PcdArmMaliDpMemoryRegionLength);
    VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  }

  // Map the PCI configration and memory space if the platform is FVP RevC.
  if ((SysId & ARM_FVP_SYS_ID_REV_MASK) == ARM_FVP_BASE_REVC_REV) {
    // SMMUv3
    VirtualMemoryTable[++Index].PhysicalBase  = FVP_REVC_SMMUV3_BASE;
    VirtualMemoryTable[Index].VirtualBase     = FVP_REVC_SMMUV3_BASE;
    VirtualMemoryTable[Index].Length          = FVP_REVC_SMMUV3_SIZE;
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

    // PCI Configuration Space
    VirtualMemoryTable[++Index].PhysicalBase  = FixedPcdGet64 (PcdPciExpressBaseAddress);
    VirtualMemoryTable[Index].VirtualBase     = FixedPcdGet64 (PcdPciExpressBaseAddress);
    VirtualMemoryTable[Index].Length          = FixedPcdGet64 (PcdPciExpressBaseSize);
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

    // PCI Memory Space
    VirtualMemoryTable[++Index].PhysicalBase  = FixedPcdGet32 (PcdPciMmio32Base);
    VirtualMemoryTable[Index].VirtualBase     = FixedPcdGet32 (PcdPciMmio32Base);
    VirtualMemoryTable[Index].Length          = FixedPcdGet32 (PcdPciMmio32Size);
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

    // 64-bit PCI Memory Space
    VirtualMemoryTable[++Index].PhysicalBase  = FixedPcdGet64 (PcdPciMmio64Base);
    VirtualMemoryTable[Index].VirtualBase     = FixedPcdGet64 (PcdPciMmio64Base);
    VirtualMemoryTable[Index].Length          = FixedPcdGet64 (PcdPciMmio64Size);
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  }

  // Map sparse memory region if present
  if (HasSparseMemory) {
    VirtualMemoryTable[++Index].PhysicalBase = SparseMemoryBase;
    VirtualMemoryTable[Index].VirtualBase = SparseMemoryBase;
    VirtualMemoryTable[Index].Length = SparseMemorySize;
    VirtualMemoryTable[Index].Attributes = CacheAttributes;
  }

  VirtualMemoryTable[++Index].PhysicalBase = SBSA_WATCHDOG_BASE;
  VirtualMemoryTable[Index].VirtualBase = SBSA_WATCHDOG_BASE;
  VirtualMemoryTable[Index].Length = SBSA_WATCHDOG_SIZE;
  VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase = 0;
  VirtualMemoryTable[Index].VirtualBase = 0;
  VirtualMemoryTable[Index].Length = 0;
  VirtualMemoryTable[Index].Attributes = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT (Index < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  *VirtualMemoryMap = VirtualMemoryTable;
}
