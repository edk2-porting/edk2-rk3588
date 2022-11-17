/** @file
  Driver for BIOS Info support.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Guid/BiosInfo.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>

#define INDEXPORT_TO_ADDRESS(x)             (x)
#define DATAPORT_TO_ADDRESS(x)              ((x) << 16)
#define PORTWIDTH_TO_ADDRESS(x)             ((x) << 32)
#define PORTBITNUMBER_TO_ADDRESS(x)         ((x) << 40)
#define PORTINDEXNUMBER_TO_ADDRESS(x)       ((x) << 48)

//
// Internal
//
#pragma pack (1)

typedef struct {
  BIOS_INFO_HEADER  Header;
  BIOS_INFO_STRUCT  Entry[1];
} BIOS_INFO;
#pragma pack ()

GLOBAL_REMOVE_IF_UNREFERENCED BIOS_INFO  mBiosInfo = {
  {
    BIOS_INFO_SIGNATURE,
    1,
    0,
  },
  {
    {
      FIT_TYPE_01_MICROCODE,
      BIOS_INFO_STRUCT_ATTRIBUTE_MICROCODE_WHOLE_REGION,
      0x0100,
      FixedPcdGet32 (PcdFlashMicrocodeFvSize),
      FixedPcdGet32 (PcdFlashMicrocodeFvBase)
    }
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR  mBiosInfoPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gBiosInfoGuid,
  &mBiosInfo
};

/**
  Installs BiosInfo Ppi and builds BiosInfo HOB .

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   Install the BiosInfo Ppi and HOB successfully.

**/
EFI_STATUS
EFIAPI
BiosInfoEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;
  VOID        *HobData;

  //
  // Install PPI, so that other PEI module can add dependency.
  //
  Status = PeiServicesInstallPpi (&mBiosInfoPpiList);
  ASSERT_EFI_ERROR (Status);

  //
  // Build hob, so that DXE module can also get the data.
  //
  HobData = BuildGuidHob (&gBiosInfoGuid, sizeof (mBiosInfo));
  ASSERT (HobData != NULL);
  if (HobData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (HobData, &mBiosInfo, sizeof (mBiosInfo));

  return EFI_SUCCESS;
}
