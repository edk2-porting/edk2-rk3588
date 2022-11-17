/** @file
  Provide FSP wrapper hob process related function.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/FspWrapperPlatformLib.h>
#include <Guid/GuidHobFspEas.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/GraphicsInfoHob.h>
#include <Guid/PcdDataBaseHobGuid.h>
#include <Guid/ZeroGuid.h>
#include <Ppi/Capsule.h>

#include <FspEas.h>

//
// Additional pages are used by DXE memory manager.
// It should be consistent between RetrieveRequiredMemorySize() and GetPeiMemSize()
//
#define PEI_ADDITIONAL_MEMORY_SIZE    (16 * EFI_PAGE_SIZE)

/**
  Get the mem size in memory type infromation table.

  @param[in] PeiServices  PEI Services table.

  @return the mem size in memory type infromation table.
**/
UINT64
GetMemorySizeInMemoryTypeInformation (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_HOB_POINTERS        Hob;
  EFI_MEMORY_TYPE_INFORMATION *MemoryData;
  UINT8                       Index;
  UINTN                       TempPageNum;

  MemoryData = NULL;
  Status     = (*PeiServices)->GetHobList ((CONST EFI_PEI_SERVICES**)PeiServices, (VOID **) &Hob.Raw);
  ASSERT_EFI_ERROR (Status);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
      CompareGuid (&Hob.Guid->Name, &gEfiMemoryTypeInformationGuid)) {
      MemoryData = (EFI_MEMORY_TYPE_INFORMATION *) (Hob.Raw + sizeof (EFI_HOB_GENERIC_HEADER) + sizeof (EFI_GUID));
      break;
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  if (MemoryData == NULL) {
    return 0;
  }

  TempPageNum = 0;
  for (Index = 0; MemoryData[Index].Type != EfiMaxMemoryType; Index++) {
    //
    // Accumulate default memory size requirements
    //
    TempPageNum += MemoryData[Index].NumberOfPages;
  }

  return TempPageNum * EFI_PAGE_SIZE;
}

/**
  Get the mem size need to be reserved in PEI phase.

  @param[in] PeiServices  PEI Services table.

  @return the mem size need to be reserved in PEI phase.
**/
UINT64
RetrieveRequiredMemorySize (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  UINT64                      Size;

  Size = GetMemorySizeInMemoryTypeInformation (PeiServices);
  return Size + PEI_ADDITIONAL_MEMORY_SIZE;
}

/**
  Get the mem size need to be consumed and reserved in PEI phase.

  @param[in] PeiServices  PEI Services table.
  @param[in] BootMode     Current boot mode.

  @return the mem size need to be consumed and reserved in PEI phase.
**/
UINT64
GetPeiMemSize (
  IN EFI_PEI_SERVICES **PeiServices,
  IN UINT32           BootMode
  )
{
  UINT64                      Size;
  UINT64                      MinSize;

  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    return PcdGet32 (PcdPeiRecoveryMinMemSize);
  }

  Size = GetMemorySizeInMemoryTypeInformation (PeiServices);

  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    //
    // Maybe more size when in CapsuleUpdate phase ?
    //
    MinSize = PcdGet32 (PcdPeiMinMemSize);
  } else {
    MinSize = PcdGet32 (PcdPeiMinMemSize);
  }

  return MinSize + Size + PEI_ADDITIONAL_MEMORY_SIZE;
}

/**
  Post FSP-M HOB process for Memory Resource Descriptor.

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
PostFspmHobProcess (
  IN VOID                 *FspHobList
  )
{
  EFI_PEI_HOB_POINTERS Hob;
  UINT64               PeiMemSize;
  EFI_PHYSICAL_ADDRESS PeiMemBase;
  EFI_STATUS           Status;
  EFI_BOOT_MODE        BootMode;
  EFI_PEI_CAPSULE_PPI  *Capsule;
  VOID                 *CapsuleBuffer;
  UINTN                CapsuleBufferLength;
  UINT64               RequiredMemSize;
  UINT64               ResourceLength;
  EFI_PEI_SERVICES     **PeiServices;

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  PeiServicesGetBootMode (&BootMode);

  PeiMemBase = 0;
  PeiMemSize = 0;
  RequiredMemSize = 0;
  ResourceLength = 0;
  //
  // Parse the hob list from fsp
  // Report all the resource hob except MMIO and IO resource Hob's
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    PeiMemSize = GetPeiMemSize (PeiServices, BootMode);
    RequiredMemSize = RetrieveRequiredMemorySize (PeiServices);
    Hob.Raw = (UINT8 *)(UINTN)FspHobList;
    DEBUG((DEBUG_INFO, "FspHobList - 0x%x\n", FspHobList));
    //
    // Find the largest available system Memory and use it for PeiMemory
    //
    while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
        && (Hob.ResourceDescriptor->PhysicalStart >= PeiMemBase)
        && (Hob.ResourceDescriptor->ResourceLength >= PeiMemSize)) {
           PeiMemBase = Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength - PeiMemSize;
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }
  }

  Hob.Raw = (UINT8 *)(UINTN)FspHobList;

  //
  // Skip the MMIO and IO reource map from the FSP Hob list
  //
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_IO) || (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_MAPPED_IO)) {
      Hob.Raw = GET_NEXT_HOB (Hob);
      continue;
    }
    ResourceLength = Hob.ResourceDescriptor->ResourceLength;
    DEBUG((DEBUG_INFO, "Resource start %lx resource length %lx resource type %d\n",Hob.ResourceDescriptor->PhysicalStart,Hob.ResourceDescriptor->ResourceLength,Hob.ResourceDescriptor->ResourceType));
    if (BootMode != BOOT_ON_S3_RESUME) {
      //
      // If the system memory found in FSP Hob is determined for PeiMemory. Split the Resource descriptor Hob
      //
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
        && (Hob.ResourceDescriptor->PhysicalStart <= PeiMemBase)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength >= PeiMemBase + PeiMemSize)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)) {
        if ((CompareGuid (&Hob.ResourceDescriptor->Owner, &gZeroGuid))) {
          BuildResourceDescriptorHob (
          Hob.ResourceDescriptor->ResourceType,
            Hob.ResourceDescriptor->ResourceAttribute,
            PeiMemBase,
            PeiMemSize
            );
        } else {
          BuildResourceDescriptorWithOwnerHob (
            Hob.ResourceDescriptor->ResourceType,
            Hob.ResourceDescriptor->ResourceAttribute,
            PeiMemBase,
            PeiMemSize,
            &Hob.ResourceDescriptor->Owner
            );
        }
        ResourceLength = (Hob.ResourceDescriptor->ResourceLength) -(PeiMemSize);
      }
    }

    //
    // Report the resource hob
    //
    if ((CompareGuid (&Hob.ResourceDescriptor->Owner, &gZeroGuid))) {
      BuildResourceDescriptorHob (
        Hob.ResourceDescriptor->ResourceType,
        Hob.ResourceDescriptor->ResourceAttribute,
        Hob.ResourceDescriptor->PhysicalStart,
        ResourceLength
        );
    } else {
      BuildResourceDescriptorWithOwnerHob (
        Hob.ResourceDescriptor->ResourceType,
        Hob.ResourceDescriptor->ResourceAttribute,
        Hob.ResourceDescriptor->PhysicalStart,
        ResourceLength,
        &Hob.ResourceDescriptor->Owner
        );
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  //
  // @todo: It is a W/A for SetMemorySpaceAttribute issue in PchSpi and PchReset drivers.
  //        We need to modify it instead of hard code here. Due to InstallEfiMemory is using hard code to
  //        describe memory resource, we have to hard code in here. Once InstallEfiMemory is merged, we should
  //        be able to remove this.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
    0xFFA00000,
    0x80000
    );
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
    0xFE000000,
    0x10000
    );
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
    PcdGet64 (PcdPciExpressBaseAddress),
    PcdGet32 (PcdPciExpressRegionLength)
    );
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
    0xFE010000,
    0x1000
    );


    //
    // Capsule mode
    //
    Capsule = NULL;
    CapsuleBuffer = NULL;
    CapsuleBufferLength = 0;
    if (BootMode == BOOT_ON_FLASH_UPDATE) {
      Status = PeiServicesLocatePpi (
                 &gEfiPeiCapsulePpiGuid,
                 0,
                 NULL,
                 (VOID **) &Capsule
                 );
      ASSERT_EFI_ERROR (Status);

      if (Status == EFI_SUCCESS) {
        Status = PeiServicesGetHobList ((void**)&Hob.Raw);
        ASSERT_EFI_ERROR (Status);
        while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
          if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
               && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
               && (Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB)
               && (Hob.ResourceDescriptor->PhysicalStart != PeiMemBase)
               && (Hob.ResourceDescriptor->ResourceLength >= CapsuleBufferLength)) {
             CapsuleBufferLength = (UINTN)Hob.ResourceDescriptor->ResourceLength;
             CapsuleBuffer = (VOID*)(UINTN)Hob.ResourceDescriptor->PhysicalStart;

          }
          Hob.Raw = GET_NEXT_HOB (Hob);
        }

        //
        // Call the Capsule PPI Coalesce function to coalesce the capsule data.
        //
        Status = Capsule->Coalesce (PeiServices, &CapsuleBuffer, &CapsuleBufferLength);
      }
    }


    DEBUG((DEBUG_INFO, "FSP wrapper PeiMemBase      : 0x%08x\n", PeiMemBase));
    DEBUG((DEBUG_INFO, "FSP wrapper PeiMemSize      : 0x%08x\n", PeiMemSize));
    DEBUG((DEBUG_INFO, "FSP wrapper RequiredMemSize : 0x%08x\n", RequiredMemSize));


    //
    // Install efi memory
    //
    Status = PeiServicesInstallPeiMemory (
               PeiMemBase,
               PeiMemSize - RequiredMemSize
               );
    ASSERT_EFI_ERROR (Status);

    if (Capsule != NULL) {
      Status = Capsule->CreateState ((EFI_PEI_SERVICES **)PeiServices, CapsuleBuffer, CapsuleBufferLength);
    }


  //
  // Create a memory allocation HOB at fixed location for MP Services PPI AP wait loop.
  // Report memory region used by FSP.
  //
  BuildMemoryAllocationHob (
    PcdGet32 (PcdFspCpuPeiApWakeupBufferAddr),
    EFI_PAGE_SIZE,
    EfiBootServicesData
    );

  return EFI_SUCCESS;
}

/**
  Process FSP HOB list

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

**/
VOID
ProcessFspHobList (
  IN VOID                 *FspHobList
  )
{
  UINT8                 PhysicalAddressBits;
  UINT32                RegEax;
  EFI_PEI_HOB_POINTERS  FspHob;

  FspHob.Raw = FspHobList;

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  } else {
    PhysicalAddressBits = 36;
  }

  ///
  /// Create a CPU hand-off information
  ///
  BuildCpuHob (PhysicalAddressBits, 16);

  //
  // Add all the HOBs from FSP binary to FSP wrapper
  //
  while (!END_OF_HOB_LIST (FspHob)) {
    if (FspHob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      //
      // Skip FSP binary creates PcdDataBaseHobGuid
      //
      if (!CompareGuid(&FspHob.Guid->Name, &gPcdDataBaseHobGuid)) {
        BuildGuidDataHob (
          &FspHob.Guid->Name,
          GET_GUID_HOB_DATA(FspHob),
          GET_GUID_HOB_DATA_SIZE(FspHob)
        );
      }
    }
    FspHob.Raw = GET_NEXT_HOB (FspHob);
  }
}

VOID
CheckFspGraphicsDeviceInfoHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS             Hob;
  EFI_STATUS                       Status;
  EFI_PEI_GRAPHICS_INFO_HOB        *FspGraphicsInfo = NULL;
  EFI_PEI_GRAPHICS_DEVICE_INFO_HOB *FspGraphicsDeviceInfo = NULL;
  EFI_PEI_GRAPHICS_DEVICE_INFO_HOB GraphicsDeviceInfo;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  if (!EFI_ERROR (Status)) {
    if (Hob.Raw != NULL) {
      if ((Hob.Raw = GetNextGuidHob (&gEfiGraphicsInfoHobGuid, Hob.Raw)) != NULL) {
        FspGraphicsInfo = GET_GUID_HOB_DATA (Hob.Guid);
      }
    }
  }
  if (FspGraphicsInfo == NULL) {
    return ;
  }

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  if (!EFI_ERROR (Status)) {
    if (Hob.Raw != NULL) {
      if ((Hob.Raw = GetNextGuidHob (&gEfiGraphicsDeviceInfoHobGuid, Hob.Raw)) != NULL) {
        FspGraphicsDeviceInfo = GET_GUID_HOB_DATA (Hob.Guid);
      }
    }
  }
  if (FspGraphicsDeviceInfo != NULL) {
    return ;
  }

  //
  // FSP only publish FspGraphicsInfo, but no FspGraphicsDeviceInfo.
  //
  // Workaround: Need publish FspGraphicsDeviceInfo, because Intel Graphics BarIndex is 1.
  //
  GraphicsDeviceInfo.VendorId          = MAX_UINT16;
  GraphicsDeviceInfo.DeviceId          = MAX_UINT16;
  GraphicsDeviceInfo.SubsystemVendorId = MAX_UINT16;
  GraphicsDeviceInfo.SubsystemId       = MAX_UINT16;
  GraphicsDeviceInfo.RevisionId        = MAX_UINT8;
  GraphicsDeviceInfo.BarIndex          = 1;
  BuildGuidDataHob (
    &gEfiGraphicsDeviceInfoHobGuid,
    &GraphicsDeviceInfo,
    sizeof(GraphicsDeviceInfo)
    );

  return ;
}

/**
  Dump FSP SMBIOS memory info HOB

**/
VOID
DumpFspSmbiosMemoryInfoHob (
  VOID
  )
{
}


/**
  Dump FSP SMBIOS Processor Info HOB

**/
VOID
DumpFspSmbiosProcessorInfoHob (
  VOID
  )
{
}

/**
  Dump FSP SMBIOS Cache Info Hob

**/
VOID
DumpFspSmbiosCacheInfoHob (
  VOID
  )
{
}

/**
  Dump FSP HOB list

**/
VOID
DumpFspHobList (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS Hob;
  EFI_STATUS           Status;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  ASSERT_EFI_ERROR (Status);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      DEBUG ((DEBUG_INFO, "FSP Extended    GUID HOB: {%g}\n", &(Hob.Guid->Name)));
    }
    if ((Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) &&
       (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)) {
      DEBUG ((DEBUG_INFO, "FSP Reserved Resource HOB: %016lX ~ %016lX\n", \
              Hob.ResourceDescriptor->PhysicalStart, Hob.ResourceDescriptor->PhysicalStart \
              + Hob.ResourceDescriptor->ResourceLength));
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
}

/**
  Dump FSP memory resource

**/
VOID
DumpFspMemoryResource (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS Hob;
  EFI_STATUS           Status;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "\nFSP Memory Resource\n"));
  DEBUG ((DEBUG_INFO, "         Resource Range           Type    Attribute                   Owner\n"));
  DEBUG ((DEBUG_INFO, "================================= ==== ================ ====================================\n"));
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    if (!CompareGuid (&(Hob.ResourceDescriptor->Owner), &gZeroGuid)) {
      DEBUG ((DEBUG_INFO, "%016lx-%016lx %4x %016x %g\n",
              Hob.ResourceDescriptor->PhysicalStart,
              Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength,
              Hob.ResourceDescriptor->ResourceType,
              Hob.ResourceDescriptor->ResourceAttribute,
              &(Hob.ResourceDescriptor->Owner)
              ));
    } else {
      DEBUG ((DEBUG_INFO, "%016lx-%016lx %4x %016x \n",
              Hob.ResourceDescriptor->PhysicalStart,
              Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength,
              Hob.ResourceDescriptor->ResourceType,
              Hob.ResourceDescriptor->ResourceAttribute
              ));
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Dump FSP memory resource

**/
VOID
DumpFspGraphicsInfoHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS      Hob;
  EFI_STATUS                Status;
  EFI_PEI_GRAPHICS_INFO_HOB *FspGraphicsInfo = NULL;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  if (!EFI_ERROR (Status)) {
    if (Hob.Raw != NULL) {
      if ((Hob.Raw = GetNextGuidHob (&gEfiGraphicsInfoHobGuid, Hob.Raw)) != NULL) {
        FspGraphicsInfo = GET_GUID_HOB_DATA (Hob.Guid);
      }
    }
    if (FspGraphicsInfo != NULL) {
      DEBUG((DEBUG_INFO, "\nGraphicsInfo\n"));
      DEBUG((DEBUG_INFO, "  |-> FrameBufferBase : 0x%016lx\n", FspGraphicsInfo->FrameBufferBase));
      DEBUG((DEBUG_INFO, "  |-> FrameBufferSize : 0x%016lx\n", FspGraphicsInfo->FrameBufferSize));
      DEBUG((DEBUG_INFO, "  |-> GraphicsMode\n"));
      DEBUG((DEBUG_INFO, "    |-> Version              : 0x%08x\n", FspGraphicsInfo->GraphicsMode.Version));
      DEBUG((DEBUG_INFO, "    |-> HorizontalResolution : %d\n", FspGraphicsInfo->GraphicsMode.HorizontalResolution));
      DEBUG((DEBUG_INFO, "    |-> VerticalResolution   : %d\n", FspGraphicsInfo->GraphicsMode.VerticalResolution));
      DEBUG((DEBUG_INFO, "    |-> PixelFormat          : %d\n", FspGraphicsInfo->GraphicsMode.PixelFormat));
      DEBUG((DEBUG_INFO, "    |-> PixelInformation     : %d|%d|%d|%d\n",
        FspGraphicsInfo->GraphicsMode.PixelInformation.RedMask,
        FspGraphicsInfo->GraphicsMode.PixelInformation.GreenMask,
        FspGraphicsInfo->GraphicsMode.PixelInformation.BlueMask,
        FspGraphicsInfo->GraphicsMode.PixelInformation.ReservedMask
        ));
      DEBUG((DEBUG_INFO, "    |-> PixelsPerScanLine    : %d\n", FspGraphicsInfo->GraphicsMode.PixelsPerScanLine));
      DEBUG((DEBUG_INFO, "\n"));
    } else {
      DEBUG((DEBUG_INFO, "\nNo GraphicsInfo\n"));
    }
  }
}

VOID
DumpFspGraphicsDeviceInfoHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS             Hob;
  EFI_STATUS                       Status;
  EFI_PEI_GRAPHICS_DEVICE_INFO_HOB *FspGraphicsDeviceInfo = NULL;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  if (!EFI_ERROR (Status)) {
    if (Hob.Raw != NULL) {
      if ((Hob.Raw = GetNextGuidHob (&gEfiGraphicsDeviceInfoHobGuid, Hob.Raw)) != NULL) {
        FspGraphicsDeviceInfo = GET_GUID_HOB_DATA (Hob.Guid);
      }
    }
    if (FspGraphicsDeviceInfo != NULL) {
      DEBUG((DEBUG_INFO, "\nGraphicsDeviceInfo\n"));
      DEBUG((DEBUG_INFO, "  |-> VendorId          : 0x%04x\n", FspGraphicsDeviceInfo->VendorId));
      DEBUG((DEBUG_INFO, "  |-> DeviceId          : 0x%04x\n", FspGraphicsDeviceInfo->DeviceId));
      DEBUG((DEBUG_INFO, "  |-> SubsystemVendorId : 0x%04x\n", FspGraphicsDeviceInfo->SubsystemVendorId));
      DEBUG((DEBUG_INFO, "  |-> SubsystemId       : 0x%04x\n", FspGraphicsDeviceInfo->SubsystemId));
      DEBUG((DEBUG_INFO, "  |-> RevisionId        : 0x%02x\n", FspGraphicsDeviceInfo->RevisionId));
      DEBUG((DEBUG_INFO, "  |-> BarIndex          : 0x%02x\n", FspGraphicsDeviceInfo->BarIndex));
      DEBUG((DEBUG_INFO, "\n"));
    } else {
      DEBUG((DEBUG_INFO, "\nNo GraphicsDeviceInfo\n"));
    }
  }
}

EFI_PEI_PPI_DESCRIPTOR mSiliconInitializedDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiSiliconInitializedPpiGuid,
  NULL
};

/**
  Post FSP-S HOB process (not Memory Resource Descriptor).

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
PostFspsHobProcess (
  IN VOID                 *FspHobList
  )
{
  EFI_STATUS   Status;

  if (PcdGet8 (PcdFspModeSelection) == 1) {
    //
    // Only in FSP API mode the wrapper has to build hobs basing on FSP output data.
    //
    ASSERT (FspHobList != NULL);
    ProcessFspHobList (FspHobList);
  } else {
    //
    // Only in FSP Dispatch mode, FSP-S should be reported to DXE dispatcher.
    //
    BuildFvHob (
      (EFI_PHYSICAL_ADDRESS) (UINTN) PcdGet32 (PcdFlashFvFspSBase),
      PcdGet32 (PcdFlashFvFspSSize)
      );
  }
  CheckFspGraphicsDeviceInfoHob ();
  DEBUG_CODE_BEGIN ();
    DumpFspSmbiosMemoryInfoHob ();
    DumpFspSmbiosProcessorInfoHob();
    DumpFspSmbiosCacheInfoHob();
    DumpFspGraphicsInfoHob ();
    DumpFspGraphicsDeviceInfoHob ();
    DumpFspHobList ();
    DumpFspMemoryResource ();
  DEBUG_CODE_END ();

  Status = PeiServicesInstallPpi (&mSiliconInitializedDesc);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
