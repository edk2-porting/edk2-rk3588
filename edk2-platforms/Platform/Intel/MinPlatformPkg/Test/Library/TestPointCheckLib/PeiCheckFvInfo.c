/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/FirmwareVolumeInfo2.h>

EFI_STATUS
TestPointCheckFv (
  IN EFI_FIRMWARE_VOLUME_HEADER   *FvHeader,
  IN UINT64                       Size,
  IN GUID                         *FvFormat
  )
{
  if (!CompareGuid (FvFormat, &FvHeader->FileSystemGuid)) {
    DEBUG ((DEBUG_ERROR, "FvFormat error - 0x%lx\n", FvHeader));
    return EFI_INVALID_PARAMETER;
  }
  if (Size != FvHeader->FvLength) {
    DEBUG ((DEBUG_ERROR, "FvLength error - 0x%lx\n", FvHeader));
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckFvInfo (
  VOID
  )
{
  EFI_STATUS                         Status;
  EFI_PEI_PPI_DESCRIPTOR             *Descriptor;
  EFI_PEI_FIRMWARE_VOLUME_INFO_PPI   *FvInfo;
  EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI  *FvInfo2;
  UINTN                              Index;
  UINTN                              Index2;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckFvInfo - Enter\n"));
  DEBUG ((DEBUG_INFO, "FV Info PPI\n"));
  for (Index = 0; ; Index++) {
    Status = PeiServicesLocatePpi (
               &gEfiPeiFirmwareVolumeInfoPpiGuid,
               Index,
               &Descriptor,
               (VOID **)&FvInfo
               );
    if (EFI_ERROR (Status)) {
      break;
    }
    DEBUG ((DEBUG_INFO,
      "  BA=%08x  L=%08x  Format={%g}",
      (UINT32)(UINTN)FvInfo->FvInfo,
      FvInfo->FvInfoSize,
      &FvInfo->FvFormat,
      FvInfo->ParentFvName,
      FvInfo->ParentFileName
      ));
    if (FvInfo->ParentFvName != NULL) {
      DEBUG ((DEBUG_INFO,
        "  ParentFv={%g}",
        FvInfo->ParentFvName
        ));
    }
    if (FvInfo->ParentFileName != NULL) {
      DEBUG ((DEBUG_INFO,
        "  ParentFileName={%g}",
        FvInfo->ParentFileName
        ));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }
  
  DEBUG ((DEBUG_INFO, "FV Info2 PPI\n"));
  for (Index2 = 0; ; Index2++) {
    Status = PeiServicesLocatePpi (
               &gEfiPeiFirmwareVolumeInfo2PpiGuid,
               Index2,
               &Descriptor,
               (VOID **)&FvInfo2
               );
    if (EFI_ERROR (Status)) {
      break;
    }
    DEBUG ((DEBUG_INFO,
      "  BA=%08x  L=%08x  Format={%g}",
      (UINT32)(UINTN)FvInfo2->FvInfo,
      FvInfo2->FvInfoSize,
      &FvInfo2->FvFormat,
      FvInfo2->ParentFvName,
      FvInfo2->ParentFileName,
      FvInfo2->AuthenticationStatus
      ));
    if (FvInfo2->ParentFvName != NULL) {
      DEBUG ((DEBUG_INFO,
        "  ParentFv={%g}",
        FvInfo2->ParentFvName
        ));
    }
    if (FvInfo2->ParentFileName != NULL) {
      DEBUG ((DEBUG_INFO,
        "  ParentFileName={%g}",
        FvInfo2->ParentFileName
        ));
    }
    DEBUG ((DEBUG_INFO,
      "  Auth=%08x\n",
      FvInfo2->AuthenticationStatus
      ));
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckFvInfo - Exit\n"));

  for (Index = 0; ; Index++) {
    Status = PeiServicesLocatePpi (
               &gEfiPeiFirmwareVolumeInfoPpiGuid,
               Index,
               &Descriptor,
               (VOID **)&FvInfo
               );
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = TestPointCheckFv (FvInfo->FvInfo, FvInfo->FvInfoSize, &FvInfo->FvFormat);
    if (EFI_ERROR(Status)) {
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL_ERROR_CODE \
          TEST_POINT_MEMORY_DISCOVERED \
          TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL_ERROR_STRING
        );
      return EFI_INVALID_PARAMETER;
    }
  }
  for (Index2 = 0; ; Index2++) {
    Status = PeiServicesLocatePpi (
               &gEfiPeiFirmwareVolumeInfo2PpiGuid,
               Index2,
               &Descriptor,
               (VOID **)&FvInfo2
               );
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = TestPointCheckFv (FvInfo2->FvInfo, FvInfo2->FvInfoSize, &FvInfo2->FvFormat);
    if (EFI_ERROR(Status)) {
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL_ERROR_CODE \
          TEST_POINT_MEMORY_DISCOVERED \
          TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL_ERROR_STRING
        );
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}
