/** @file PeiReportFvLib.c
  Source code file for Report Firmware Volume (FV) library

  Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ReportFvLib.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Ppi/FirmwareVolumeInfo2.h>

VOID
ReportPreMemFv (
  VOID
  )
{
  UINTN                                 Index = 0;
  EFI_PEI_PPI_DESCRIPTOR                *Descriptor = NULL;
  EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI     *Ppi = NULL;
  EFI_STATUS                            Status = EFI_SUCCESS;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader = NULL;
  EFI_BOOT_MODE                         BootMode = BOOT_WITH_FULL_CONFIGURATION;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  DEBUG_CODE (
    for (Index = 0; Status == EFI_SUCCESS; Index++) {
      Status = PeiServicesLocatePpi (&gEfiPeiFirmwareVolumeInfo2PpiGuid, Index, &Descriptor, &Ppi);
      if (!EFI_ERROR (Status)) {
        FvHeader = (EFI_FIRMWARE_VOLUME_HEADER*) Ppi->FvInfo;
        DEBUG ((DEBUG_INFO, "Found FV at 0x%x, size 0x%x\n", FvHeader, FvHeader->FvLength));
      }
    }
  );

  //
  // FvBspPreMemory and FvPreMemory are required for all stages.
  //

  DEBUG ((DEBUG_INFO, "Install FlashFvBspPreMemory - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvBspPreMemoryBase), PcdGet32 (PcdFlashFvBspPreMemorySize)));
  PeiServicesInstallFvInfo2Ppi (
    &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvBspPreMemoryBase))->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvBspPreMemoryBase),
    PcdGet32 (PcdFlashFvBspPreMemorySize),
    NULL,
    NULL,
    0
    );

  DEBUG ((DEBUG_INFO, "Install FlashFvPreMemory - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvPreMemoryBase), PcdGet32 (PcdFlashFvPreMemorySize)));
  PeiServicesInstallFvInfo2Ppi (
    &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvPreMemoryBase))->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvPreMemoryBase),
    PcdGet32 (PcdFlashFvPreMemorySize),
    NULL,
    NULL,
    0
    );

  //
  // In API mode, do not publish FSP FV.
  //
  if (!PcdGetBool (PcdFspWrapperBootMode)) {
    //
    // FvFspT may be required for all stages
    //
    DEBUG ((DEBUG_INFO, "Install FlashFvFspT - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspTBase), PcdGet32 (PcdFlashFvFspTSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvFspTBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspTBase),
      PcdGet32 (PcdFlashFvFspTSize),
      NULL,
      NULL,
      0
      );

    //
    // FvFspM required for stage 2 and above
    //
    if (PcdGet8 (PcdBootStage) >= 2) {
      DEBUG ((DEBUG_INFO, "Install FlashFvFspM - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspMBase), PcdGet32 (PcdFlashFvFspMSize)));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvFspMBase))->FileSystemGuid),
        (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspMBase),
        PcdGet32 (PcdFlashFvFspMSize),
        NULL,
        NULL,
        0
        );
    }
  }

  //
  // FvOprom may be required for most stages
  //
  if (PcdGet8 (PcdBootStage) >= 2) {
    DEBUG ((DEBUG_INFO, "Install PcdFlashFvOprom - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvOpromBase), PcdGet32 (PcdFlashFvOpromSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvOpromBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvOpromBase),
      PcdGet32 (PcdFlashFvOpromSize),
      NULL,
      NULL,
      0
      );
  }

  //
  // FvAdvanced not needed until stage 6
  //
  if (PcdGet8 (PcdBootStage) >= 6) {
    DEBUG ((DEBUG_INFO, "Install FlashFvAdvancedPreMemory - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvAdvancedPreMemoryBase), PcdGet32 (PcdFlashFvAdvancedPreMemorySize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvAdvancedPreMemoryBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvAdvancedPreMemoryBase),
      PcdGet32 (PcdFlashFvAdvancedPreMemorySize),
      NULL,
      NULL,
      0
      );
  }
}

VOID
ReportPostMemFv (
  VOID
  )
{
  UINTN                                 Index = 0;
  EFI_PEI_PPI_DESCRIPTOR                *Descriptor = NULL;
  EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI     *Ppi = NULL;
  EFI_STATUS                            Status = EFI_SUCCESS;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader = NULL;

  DEBUG_CODE (
    for (Index = 0; Status == EFI_SUCCESS; Index++) {
      Status = PeiServicesLocatePpi (&gEfiPeiFirmwareVolumeInfo2PpiGuid, Index, &Descriptor, &Ppi);
      if (!EFI_ERROR (Status)) {
        FvHeader = (EFI_FIRMWARE_VOLUME_HEADER*) Ppi->FvInfo;
        DEBUG ((DEBUG_INFO, "Found FV at 0x%x, size 0x%x\n", FvHeader, FvHeader->FvLength));
      }
    }
  );

  //
  // FvFspS, FvPostMemory, and FvBsp may be required for completing stage 2
  //
  if (PcdGet8 (PcdBootStage) >= 2) {
    //
    // In API mode, do not publish FSP FV.
    //
    if (!PcdGetBool (PcdFspWrapperBootMode)) {
      DEBUG ((DEBUG_INFO, "Install FlashFvFspS - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspSBase), PcdGet32 (PcdFlashFvFspSSize)));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvFspSBase))->FileSystemGuid),
        (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspSBase),
        PcdGet32 (PcdFlashFvFspSSize),
        NULL,
        NULL,
        0
        );
    }

    DEBUG ((DEBUG_INFO, "Install FlashFvPostMemory - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvPostMemoryBase), PcdGet32 (PcdFlashFvPostMemorySize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvPostMemoryBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvPostMemoryBase),
      PcdGet32 (PcdFlashFvPostMemorySize),
      NULL,
      NULL,
      0
      );

    DEBUG ((DEBUG_INFO, "Install FlashFvBsp - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvBspBase), PcdGet32 (PcdFlashFvBspSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvBspBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvBspBase),
      PcdGet32 (PcdFlashFvBspSize),
      NULL,
      NULL,
      0
      );
  }

  //
  // FvUefiBoot required for completing stage 3
  //
  if (PcdGet8 (PcdBootStage) >= 3) {
    DEBUG ((DEBUG_INFO, "Install FlashFvUefiBoot - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvUefiBootBase), PcdGet32 (PcdFlashFvUefiBootSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvUefiBootBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvUefiBootBase),
      PcdGet32 (PcdFlashFvUefiBootSize),
      NULL,
      NULL,
      0
      );
  }

  //
  // FvOsBoot required for completing stage 4
  //
  if (PcdGet8 (PcdBootStage) >= 4) {
    DEBUG ((DEBUG_INFO, "Install FlashFvOsBoot - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvOsBootBase), PcdGet32 (PcdFlashFvOsBootSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvOsBootBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvOsBootBase),
      PcdGet32 (PcdFlashFvOsBootSize),
      NULL,
      NULL,
      0
      );
  }

  //
  // FvSecurity required for completing stage 5
  //
  if (PcdGet8 (PcdBootStage) >= 5) {
    DEBUG ((DEBUG_INFO, "Install FlashFvSecurity - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvSecurityBase), PcdGet32 (PcdFlashFvSecuritySize)));
      PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvSecurityBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvSecurityBase),
      PcdGet32 (PcdFlashFvSecuritySize),
        NULL,
        NULL,
        0
        );
  }

  //
  // FvAdvanced required for completing stage 6
  //
  if (PcdGet8 (PcdBootStage) >= 6) {
    DEBUG ((DEBUG_INFO, "Install FlashFvAdvanced - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvAdvancedBase), PcdGet32 (PcdFlashFvAdvancedSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER*) (UINTN) PcdGet32 (PcdFlashFvAdvancedBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvAdvancedBase),
      PcdGet32 (PcdFlashFvAdvancedSize),
      NULL,
      NULL,
      0
      );
  }

  //
  // Report resource related HOB for flash FV to reserve space in GCD and memory map
  //

  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    (UINTN) PcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN) PcdGet32 (PcdFlashAreaSize)
    );

  BuildMemoryAllocationHob (
    (UINTN) PcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN) PcdGet32 (PcdFlashAreaSize),
    EfiMemoryMappedIO
    );
}
