/** @file
  File to update SVID values from Interposer mapping.

@copyright
  Copyright 2018 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/HobLib.h>
#include <Library/MemVrSvidMapLib.h>
#include <Ppi/DynamicSiLibraryPpi.h>

/**
  After Memory policy installation need to map SVID in case of an Interposer is present.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
MapInterposerToSvid (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  INTERPOSER_TYPE         InterposerType;
  EFI_PLATFORM_INFO       *PlatformInfo;
  EFI_HOB_GUID_TYPE       *GuidHob;
  UINT8                   OriginalMcId = 0;
  UINT8                   CurrentMcId = 0;
  UINT8                   Socket = 0;
  UINT8                   SvidValue = 0;
  MEM_SVID_MAP            *MemSvidMap;
  UINTN                   Size;
  EFI_STATUS              Status;
  INTERPOSER_MAP          *MemInterposerMap = NULL;
  BOOLEAN                 InterposerPresent = FALSE;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

  DEBUG ((EFI_D_INFO, "MapInterposerToSvid   Entry\n"));

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_INFO, "No Platform Info HOB Detected Exiting...\n"));
    return EFI_SUCCESS;
  } else {
    MemInterposerMap = (INTERPOSER_MAP *) PcdGetPtr (PcdMemInterposerMap);
    if (MemInterposerMap == NULL) {
      return EFI_SUCCESS;
    }
    Size = sizeof (MEM_SVID_MAP);
    DEBUG ((EFI_D_INFO, "Allocate memory for MemSvidMap PCD\n"));
    Status = (*PeiServices)->AllocatePool (PeiServices, Size, &MemSvidMap);
    ASSERT_EFI_ERROR (Status);
    ZeroMem (MemSvidMap, Size);

    PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      InterposerType = PlatformInfo->InterposerType[Socket];
      for (CurrentMcId =0; CurrentMcId < MAX_IMC; CurrentMcId++) {
        if (InterposerType != InterposerUnknown) {
          OriginalMcId = MemInterposerMap->Interposer[InterposerType].MappedMcId[CurrentMcId];
          if (OriginalMcId < MAX_IMC) {
            Status = DynamicSiLibraryPpi->GetSvidMap (Socket, OriginalMcId, &SvidValue);

            if(Status == EFI_NOT_FOUND) {
              DEBUG ((DEBUG_ERROR, "PcdMemSrVidMap = NULL\n"));
            } else {
              DEBUG ((DEBUG_ERROR, "SocketId = %d, McId = %d, SvidValue = %d\n", Socket, OriginalMcId, SvidValue));
            }

            MemSvidMap->Socket[Socket].Mc[CurrentMcId] = SvidValue;
            InterposerPresent = TRUE;
          }
          DEBUG ((EFI_D_INFO, "Current MC id = %d, Original MC id = %d, SVID = %d\n", CurrentMcId, OriginalMcId, SvidValue));
        }
      }
    }
  }
  if (InterposerPresent) {
    PcdSetPtrS (PcdMemSrvidMap, &Size, (VOID *) MemSvidMap);
  } else {
    DEBUG ((EFI_D_INFO, "No Interposer Present....\n"));
  }

  DEBUG ((EFI_D_INFO, "MapInterposerToSvid   Exit\n"));
  return EFI_SUCCESS;
}

EFI_PEI_NOTIFY_DESCRIPTOR mMapInterposerToSvidNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gMemoryPolicyPpiGuid,
  MapInterposerToSvid
};

/**
  Initialize SVID PCD with Interposer mapping

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
**/
EFI_STATUS
EFIAPI
InterposerToSvidMapEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  DEBUG ((EFI_D_INFO, "InterposerToSvidMap   Entry\n"));

  Status = PeiServicesNotifyPpi (&mMapInterposerToSvidNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "InterposerToSvidMap   Exit\n"));
  return EFI_SUCCESS;;
}