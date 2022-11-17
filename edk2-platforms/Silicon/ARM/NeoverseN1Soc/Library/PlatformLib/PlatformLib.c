/** @file
*
*  Copyright (c) 2018-2020, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/BaseLib.h>
#include <Ppi/ArmMpCoreInfo.h>

STATIC ARM_CORE_INFO mCoreInfoTable[] = {
  { 0x0, 0x0 }, // Cluster 0, Core 0
  { 0x0, 0x1 }, // Cluster 0, Core 1
  { 0x1, 0x0 }, // Cluster 1, Core 0
  { 0x1, 0x1 }  // Cluster 1, Core 1
};

EFI_BOOT_MODE
ArmPlatformGetBootMode (
         VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

RETURN_STATUS
ArmPlatformInitialize (
  IN     UINTN                  MpId
  )
{
  return RETURN_SUCCESS;
}

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
     OUT UINTN                  *CoreCount,
     OUT ARM_CORE_INFO          **ArmCoreTable
  )
{
  *CoreCount = sizeof (mCoreInfoTable) / sizeof (ARM_CORE_INFO);
  *ArmCoreTable = mCoreInfoTable;
  return EFI_SUCCESS;
}

STATIC ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = {
  PrePeiCoreGetMpCoreInfo
};

EFI_PEI_PPI_DESCRIPTOR gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
     OUT UINTN                  *PpiListSize,
     OUT EFI_PEI_PPI_DESCRIPTOR **PpiList
  )
{
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}
