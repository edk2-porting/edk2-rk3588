/** @file
*
*  Copyright (c) 2011-2013, ARM Limited. All rights reserved.<BR>
*  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
/**
  Derived from:
   ArmPlatformPkg/ArmVExpressPkg/Library/ArmVExpressLibRTSM/RTSM.c

**/

//
// The package level header files this module uses
//
#include <PiPei.h>

//
// The protocols, PPI and GUID defintions for this module
//
#include <Ppi/ArmMpCoreInfo.h>
#include <Guid/ArmMpCoreInfo.h>

//
// The Library classes this module consumes
//
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/ArmLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib.h>


extern EFI_GUID gAmdStyxMpCoreInfoGuid;


/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

  @return   Return the current Boot Mode of the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}


/**
  Initialize controllers that must setup in the normal world

  This function is called by the ArmPlatformPkg/Pei or ArmPlatformPkg/Pei/PlatformPeim
  in the PEI phase.

**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  if (!ArmPlatformIsPrimaryCore (MpId)) {
    return RETURN_SUCCESS;
  }

  // XXX Place holder XXX ...

  return RETURN_SUCCESS;
}


//
// Return list of cores in the system
//
EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *ArmCoreCount,
  OUT ARM_CORE_INFO           **ArmCoreInfoTable
  )
{
  EFI_PEI_HOB_POINTERS    Hob;

  if (ArmIsMpCore()) {
    // Iterate through the HOBs and find if there is ARM PROCESSOR ENTRY HOB
    for (Hob.Raw = GetHobList (); !END_OF_HOB_LIST(Hob); Hob.Raw = GET_NEXT_HOB(Hob)) {
      // Check for Correct HOB type
      if ((GET_HOB_TYPE (Hob)) == EFI_HOB_TYPE_GUID_EXTENSION) {
        // Check for correct GUID type
        if (CompareGuid(&(Hob.Guid->Name), &gAmdStyxMpCoreInfoGuid)) {
          *ArmCoreInfoTable = (ARM_CORE_INFO *) GET_GUID_HOB_DATA(Hob);
          *ArmCoreCount = GET_GUID_HOB_DATA_SIZE(Hob)/sizeof(ARM_CORE_INFO);
          return EFI_SUCCESS;
        }
      }
    }
  }

  return EFI_UNSUPPORTED;
}


ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};


VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  if (ArmIsMpCore()) {
    *PpiListSize = sizeof(gPlatformPpiTable);
    *PpiList = gPlatformPpiTable;
  } else {
    *PpiListSize = 0;
    *PpiList = NULL;
  }
}


