/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
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
  if (!PcdGetBool(PcdFspWrapperBootMode)) {
    DEBUG ((DEBUG_INFO, "Install FlashFvFspM - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspMBase), PcdGet32 (PcdFlashFvFspMSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFspMBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspMBase),
      PcdGet32 (PcdFlashFvFspMSize),
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
    if (!PcdGetBool(PcdFspWrapperBootMode)) {
      DEBUG ((DEBUG_INFO, "Install FlashFvFspS - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspSBase), PcdGet32 (PcdFlashFvFspSSize)));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFspSBase))->FileSystemGuid),
        (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspSBase),
        PcdGet32 (PcdFlashFvFspSSize),
        NULL,
        NULL,
        0
        );
      DEBUG ((DEBUG_INFO, "Install FlashFvFspU - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspUBase), PcdGet32 (PcdFlashFvFspUSize)));
      PeiServicesInstallFvInfo2Ppi (
        &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFspUBase))->FileSystemGuid),
        (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspUBase),
        PcdGet32 (PcdFlashFvFspUSize),
        NULL,
        NULL,
        0
        );
    }
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
