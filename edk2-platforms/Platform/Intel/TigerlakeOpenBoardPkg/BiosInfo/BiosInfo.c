/** @file
  Driver for BIOS Info support.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
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
#include <Ppi/FirmwareVolumeInfoMeasurementExcluded.h>
#include <Library/MemoryAllocationLib.h>

#define BASE_FV_SIZE 10

#define FSP_WRAPPER_FV_SIZE 3

#define TSN_MAC_ADDRESS_FV_SIZE 0

#define BIOS_INFO_STRUCT_SIZE (BASE_FV_SIZE + FSP_WRAPPER_FV_SIZE + TSN_MAC_ADDRESS_FV_SIZE)


/*
  BIOS_INFO structure is the base of the firmware volume layout for Intel platform BIOS implementation
  so security checker module can run based on the structure and throw warnings, error or deadloop
  when any unexpected firmware volumes are detected.

  BIOS_INFO is recommended to support full entries of firmware volumes present in a flash
  with right type, attribute, version, flash map base address and size,
  all associated information which is defined by BIOS_INFO_STRUCT structure.
  - IBB firmware volumes, which are expected to be measured or/and verified
    by hardware base security solution to meet SecureBoot chain of trust
    (Intel BootGuard for example), have attribute 0x0.
  - Post IBB firmware volumes, which are expected to be measured or/and verified
    by BIOS (TCG code for measurement, RSA2048SHA256Sign algorithm for verification for example),
    have attribute BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB.
  - Else, follows Firmware Interface Table specification.
*/
#pragma pack (1)
typedef struct {
  BIOS_INFO_HEADER  Header;
  BIOS_INFO_STRUCT  Entry[BIOS_INFO_STRUCT_SIZE];
} BIOS_INFO;
#pragma pack ()

GLOBAL_REMOVE_IF_UNREFERENCED BIOS_INFO  mBiosInfo = {
  {
    BIOS_INFO_SIGNATURE,
    BIOS_INFO_STRUCT_SIZE,
    0,
  },
  {
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,
      0x0100,
      FixedPcdGet32 (PcdFlashNvStorageVariableSize) + FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) + FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize),
      FixedPcdGet32 (PcdFlashNvStorageVariableBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvAdvancedSize),
      FixedPcdGet32 (PcdFlashFvAdvancedBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvOptionalSize),
      FixedPcdGet32 (PcdFlashFvOptionalBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvOsBootSize),
      FixedPcdGet32 (PcdFlashFvOsBootBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvUefiBootSize),
      FixedPcdGet32 (PcdFlashFvUefiBootBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvPostMemorySize),
      FixedPcdGet32 (PcdFlashFvPostMemoryBase)
    },
    {
      /*
        Note :
        Startup ACM is one of the binaries in FvFirmwareBinaries,
        so put type 07 but not type 02.
        FIT table will contain a type 02 entry with actual address
        of ACM binary (it is passed as an input to FitGen tool).
      */
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,
      0x0100,
      FixedPcdGet32 (PcdFlashFvFirmwareBinariesSize),
      FixedPcdGet32 (PcdFlashFvFirmwareBinariesBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspSSize),
      FixedPcdGet32 (PcdFlashFvFspSBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspMSize),
      FixedPcdGet32 (PcdFlashFvFspMBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspTSize),
      FixedPcdGet32 (PcdFlashFvFspTBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvSecuritySize),
      FixedPcdGet32 (PcdFlashFvSecurityBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvPreMemorySize),
      FixedPcdGet32 (PcdFlashFvPreMemoryBase)
    },
    {
      FIT_TYPE_01_MICROCODE,
      BIOS_INFO_STRUCT_ATTRIBUTE_MICROCODE_WHOLE_REGION,
      0x0100,
      FixedPcdGet32 (PcdFlashMicrocodeFvSize),
      FixedPcdGet32 (PcdFlashMicrocodeFvBase)
    },
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR  mBiosInfoPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gBiosInfoGuid,
  &mBiosInfo
};

/**
  Installs BiosInfo Ppi.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   Install the BiosInfo Ppi successfully.

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
