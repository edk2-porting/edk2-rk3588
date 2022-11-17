/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiPei.h>

#include <Library/ArmLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Platform/MemoryMap.h>
#include <Platform/Pcie.h>

#include <Ppi/Capsule.h>
#include <Ppi/DramInfo.h>

#define ARM_MEMORY_REGION(Base, Size) \
  { (Base), (Base), (Size), ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK }

#define ARM_UNCACHED_REGION(Base, Size) \
  { (Base), (Base), (Size), ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED }

#define ARM_DEVICE_REGION(Base, Size) \
  { (Base), (Base), (Size), ARM_MEMORY_REGION_ATTRIBUTE_DEVICE }

#define ARM_CACHED_DEVICE_REGION(Base, Size) \
  { (Base), (Base), (Size), ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK_NONSHAREABLE }

VOID
BuildMemoryTypeInformationHob (
  VOID
  );

STATIC CONST EFI_RESOURCE_ATTRIBUTE_TYPE mDramResourceAttributes =
  EFI_RESOURCE_ATTRIBUTE_PRESENT |
  EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
  EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
  EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
  EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
  EFI_RESOURCE_ATTRIBUTE_TESTED;

STATIC CONST ARM_MEMORY_REGION_DESCRIPTOR mVirtualMemoryTable[] = {
  // Memory mapped SPI NOR flash
  ARM_CACHED_DEVICE_REGION (FixedPcdGet64 (PcdFdBaseAddress),
                            FixedPcdGet32 (PcdFdSize)),

  // SynQuacer OnChip peripherals
  ARM_DEVICE_REGION (SYNQUACER_PERIPHERALS_BASE,
                     SYNQUACER_PERIPHERALS_SZ),

  // SynQuacer OnChip non-secure SRAM
  ARM_CACHED_DEVICE_REGION (SYNQUACER_NON_SECURE_SRAM_BASE,
                            SYNQUACER_NON_SECURE_SRAM_SZ),

  // SynQuacer GIC-500
  ARM_DEVICE_REGION (SYNQUACER_GIC500_DIST_BASE, SYNQUACER_GIC500_DIST_SIZE),
  ARM_DEVICE_REGION (SYNQUACER_GIC500_RDIST_BASE, SYNQUACER_GIC500_RDIST_SIZE),

  // SynQuacer eMMC(SDH30)
  ARM_DEVICE_REGION (SYNQUACER_EMMC_BASE, SYNQUACER_EMMC_BASE_SZ),

  // SynQuacer GPIO block
  ARM_DEVICE_REGION (SYNQUACER_GPIO_BASE, SYNQUACER_GPIO_SIZE),

  // SynQuacer EEPROM - could point to NOR flash as well
  ARM_CACHED_DEVICE_REGION (FixedPcdGet32 (PcdNetsecEepromBase),
                            SYNQUACER_EEPROM_BASE_SZ),

  // SynQuacer I2C
  ARM_DEVICE_REGION (SYNQUACER_I2C0_BASE, SYNQUACER_I2C0_SIZE),
  ARM_DEVICE_REGION (SYNQUACER_I2C1_BASE, SYNQUACER_I2C1_SIZE),

  // SynQuacer NETSEC
  ARM_DEVICE_REGION (SYNQUACER_NETSEC1_BASE, SYNQUACER_NETSEC1_BASE_SZ),

  // PCIe control registers
  ARM_DEVICE_REGION (SYNQUACER_PCIE_BASE, SYNQUACER_PCIE_SIZE),

  // PCIe config space
  ARM_DEVICE_REGION (SYNQUACER_PCI_SEG0_CONFIG_BASE,
                     SYNQUACER_PCI_SEG0_CONFIG_SIZE),
  ARM_DEVICE_REGION (SYNQUACER_PCI_SEG1_CONFIG_BASE,
                     SYNQUACER_PCI_SEG1_CONFIG_SIZE),

  // PCIe I/O space
  ARM_DEVICE_REGION (SYNQUACER_PCI_SEG0_PORTIO_MEMBASE,
                     SYNQUACER_PCI_SEG0_PORTIO_MEMSIZE),
  ARM_DEVICE_REGION (SYNQUACER_PCI_SEG1_PORTIO_MEMBASE,
                     SYNQUACER_PCI_SEG1_PORTIO_MEMSIZE),

  // variable store
  ARM_DEVICE_REGION (FixedPcdGet32 (PcdFip006DxeRegBaseAddress),
                     EFI_PAGE_SIZE),
  ARM_DEVICE_REGION (FixedPcdGet32 (PcdFip006DxeMemBaseAddress),
                     EFI_PAGE_SIZE),
  ARM_DEVICE_REGION (FixedPcdGet32 (PcdFlashNvStorageVariableBase),
                     FixedPcdGet32 (PcdFlashNvStorageVariableSize)),
  ARM_DEVICE_REGION (FixedPcdGet32 (PcdFlashNvStorageFtwWorkingBase),
                     FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize)),
  ARM_DEVICE_REGION (FixedPcdGet32 (PcdFlashNvStorageFtwSpareBase),
                     FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize)),

  // NETSEC/eMMC SMMU
  ARM_DEVICE_REGION (SYNQUACER_SCB_SMMU_BASE, SYNQUACER_SCB_SMMU_SIZE),

  // DesignWare FUART
  ARM_DEVICE_REGION (SYNQUACER_UART1_BASE, SYNQUACER_UART1_SIZE),

  // SPI#1 MMIO
  ARM_DEVICE_REGION (SYNQUACER_SPI1_MMIO_BASE, SYNQUACER_SPI1_MMIO_SIZE),
};

STATIC
EFI_STATUS
DeclareDram (
  OUT ARM_MEMORY_REGION_DESCRIPTOR    **VirtualMemoryTable
  )
{
  SYNQUACER_DRAM_INFO_PPI       *DramInfo;
  EFI_STATUS                    Status;
  UINTN                         Idx;
  UINTN                         RegionCount;
  UINT64                        Base;
  UINT64                        Size;
  ARM_MEMORY_REGION_DESCRIPTOR  *DramDescriptor;

  Status = PeiServicesLocatePpi (&gSynQuacerDramInfoPpiGuid, 0, NULL,
             (VOID **)&DramInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = DramInfo->GetRegionCount (&RegionCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *VirtualMemoryTable = AllocatePool (sizeof (mVirtualMemoryTable) +
                                      (RegionCount + 1) *
                                      sizeof (ARM_MEMORY_REGION_DESCRIPTOR));
  if (*VirtualMemoryTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (*VirtualMemoryTable, mVirtualMemoryTable,
    sizeof (mVirtualMemoryTable));

  DramDescriptor = *VirtualMemoryTable + ARRAY_SIZE (mVirtualMemoryTable);

  for (Idx = 0; Idx < RegionCount; Idx++) {
    Status = DramInfo->GetRegion (Idx, &Base, &Size);
    ASSERT_EFI_ERROR (Status);

    BuildResourceDescriptorHob (EFI_RESOURCE_SYSTEM_MEMORY,
      mDramResourceAttributes, Base, Size);

    if (Base > MAX_ALLOC_ADDRESS - Size + 1) {
      continue;
    }

    DramDescriptor->PhysicalBase = Base;
    DramDescriptor->VirtualBase  = Base;
    DramDescriptor->Length       = MIN (Size, MAX_ALLOC_ADDRESS - Base + 1);
    DramDescriptor->Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    DramDescriptor++;
  }

  DramDescriptor->PhysicalBase = 0;
  DramDescriptor->VirtualBase  = 0;
  DramDescriptor->Length       = 0;
  DramDescriptor->Attributes   = 0;

  return EFI_SUCCESS;
}

STATIC
BOOLEAN
CheckCapsule (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_CAPSULE_PPI               *Capsule,
  IN  EFI_PHYSICAL_ADDRESS          UefiMemoryBase,
  OUT VOID                          **CapsuleBuffer,
  OUT UINTN                         *CapsuleBufferLength
  )
{
  EFI_STATUS        Status;

  Status = Capsule->CheckCapsuleUpdate (PeiServices);
  if (!EFI_ERROR (Status)) {

    //
    // Coalesce the capsule into unused memory. CreateState() below will copy
    // it to a properly allocated buffer.
    //
    *CapsuleBuffer = (VOID *)(UINTN)PcdGet64 (PcdSystemMemoryBase);
    *CapsuleBufferLength = UefiMemoryBase - PcdGet64 (PcdSystemMemoryBase);

    PeiServicesSetBootMode (BOOT_ON_FLASH_UPDATE);

    Status = Capsule->Coalesce (PeiServices, CapsuleBuffer,
                        CapsuleBufferLength);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: Coalesced capsule @ %p (0x%lx)\n",
        __FUNCTION__, *CapsuleBuffer, *CapsuleBufferLength));
      return TRUE;
    } else {
      DEBUG ((DEBUG_WARN, "%a: failed to coalesce() capsule (Status == %r)\n",
        __FUNCTION__, Status));
    }
  }
  return FALSE;
}

EFI_STATUS
EFIAPI
MemoryPeim (
  IN EFI_PHYSICAL_ADDRESS       UefiMemoryBase,
  IN UINT64                     UefiMemorySize
  )
{
  EFI_STATUS                    Status;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  EFI_PEI_SERVICES              **PeiServices;
  PEI_CAPSULE_PPI               *Capsule;
  VOID                          *CapsuleBuffer;
  UINTN                         CapsuleBufferLength;
  BOOLEAN                       HaveCapsule;
  EFI_BOOT_MODE                 BootMode;

  Status = DeclareDram (&VirtualMemoryTable);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  ASSERT (PeiServices != NULL);

  Status = PeiServicesLocatePpi (&gPeiCapsulePpiGuid, 0, NULL,
             (VOID **)&Capsule);
  ASSERT_EFI_ERROR (Status);

  //
  // Check for persistent capsules, unless we are booting with default
  // settings.
  //
  Status = PeiServicesGetBootMode (&BootMode);
  if (!EFI_ERROR (Status) && BootMode != BOOT_WITH_DEFAULT_SETTINGS) {
    HaveCapsule = CheckCapsule (PeiServices, Capsule, UefiMemoryBase,
                    &CapsuleBuffer, &CapsuleBufferLength);
  } else {
    HaveCapsule = FALSE;
  }

  Status = ArmConfigureMmu (VirtualMemoryTable, NULL, NULL);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HaveCapsule) {
    Status = Capsule->CreateState (PeiServices, CapsuleBuffer,
                        CapsuleBufferLength);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: Capsule->CreateState failed (Status == %r)\n",
        __FUNCTION__, Status));
    }
  }

  if (FeaturePcdGet (PcdPrePiProduceMemoryTypeInformationHob)) {
    // Optional feature that helps prevent EFI memory map fragmentation.
    BuildMemoryTypeInformationHob ();
  }
  return EFI_SUCCESS;
}
