/** @file
  Source code file for Report Firmware Volume (FV) library

Copyright (c) 2018 - 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ReportFvLib.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Ppi/FirmwareVolumeInfo.h>

VOID
ReportPreMemFv (
  VOID
  )
{
  ///
  /// Note : FSP FVs except FSP-T FV are installed in IntelFsp2WrapperPkg in Dispatch mode.
  ///
  if (PcdGetBool(PcdFspWrapperBootMode)) {
    DEBUG ((DEBUG_INFO, "Install FlashFvFspT - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspTBase), PcdGet32 (PcdFlashFvFspTSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFspTBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspTBase),
      PcdGet32 (PcdFlashFvFspTSize),
      NULL,
      NULL,
      0
      );
  }
  DEBUG ((DEBUG_INFO, "Install FlashFvSecurity - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvSecurityBase), PcdGet32 (PcdFlashFvSecuritySize)));
  PeiServicesInstallFvInfo2Ppi (
    &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvSecurityBase))->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvSecurityBase),
    PcdGet32 (PcdFlashFvSecuritySize),
    NULL,
    NULL,
    0
    );
  if (PcdGet8 (PcdBootStage) >= 6) {
    DEBUG ((
      DEBUG_INFO,
      "Install FlashFvAdvancedPreMemory - 0x%x, 0x%x\n",
      PcdGet32 (PcdFlashFvAdvancedPreMemoryBase),
      PcdGet32 (PcdFlashFvAdvancedPreMemorySize)
      ));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvAdvancedPreMemoryBase))->FileSystemGuid),
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
  EFI_STATUS                    Status;
  EFI_BOOT_MODE                 BootMode;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Note : FSP FVs except FSP-T FV are installed in IntelFsp2WrapperPkg in Dispatch mode.
  ///

  ///
  /// Build HOB for DXE
  ///
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    ///
    /// Prepare the recovery service
    ///
  } else {
    DEBUG ((DEBUG_INFO, "Install FlashFvPostMemory - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvPostMemoryBase), PcdGet32 (PcdFlashFvPostMemorySize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvPostMemoryBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvPostMemoryBase),
      PcdGet32 (PcdFlashFvPostMemorySize),
      NULL,
      NULL,
      0
      );
    DEBUG ((DEBUG_INFO, "Install FlashFvUefiBoot - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvUefiBootBase), PcdGet32 (PcdFlashFvUefiBootSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvUefiBootBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvUefiBootBase),
      PcdGet32 (PcdFlashFvUefiBootSize),
      NULL,
      NULL,
      0
      );
    DEBUG ((DEBUG_INFO, "Install FlashFvOsBoot - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvOsBootBase), PcdGet32 (PcdFlashFvOsBootSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvOsBootBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvOsBootBase),
      PcdGet32 (PcdFlashFvOsBootSize),
      NULL,
      NULL,
      0
      );
    if (PcdGet8 (PcdBootStage) >= 6) {
      DEBUG ((DEBUG_INFO, "Install FlashFvAdvanced - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvAdvancedBase), PcdGet32 (PcdFlashFvAdvancedSize)));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvAdvancedBase))->FileSystemGuid),
        (VOID *) (UINTN) PcdGet32 (PcdFlashFvAdvancedBase),
        PcdGet32 (PcdFlashFvAdvancedSize),
        NULL,
        NULL,
        0
        );
    }
  }

  //
  // Report resource HOB for flash FV
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
