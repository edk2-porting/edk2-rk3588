/** @file
 *
 *  Copyright (c) 2013-2014, ARM Limited. All rights reserved.
 *  Copyright (c) 2017, Linaro Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 */

#include <Library/ArmPlatformLib.h>
#include <Library/BaseLib.h>

#include <Ppi/ArmMpCoreInfo.h>

STATIC ARM_CORE_INFO mSynQuacerInfoTable[] = {
  { 0x0, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 0, Core 0
  { 0x0, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 0, Core 1
  { 0x1, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 1, Core 0
  { 0x1, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 1, Core 1
  { 0x2, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 2, Core 0
  { 0x2, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 2, Core 1
  { 0x3, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 3, Core 0
  { 0x3, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 3, Core 1
  { 0x4, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 4, Core 0
  { 0x4, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 4, Core 1
  { 0x5, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 5, Core 0
  { 0x5, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 5, Core 1
  { 0x6, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 6, Core 0
  { 0x6, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 6, Core 1
  { 0x7, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 7, Core 0
  { 0x7, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 7, Core 1
  { 0x8, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 8, Core 0
  { 0x8, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 8, Core 1
  { 0x9, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 9, Core 0
  { 0x9, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 9, Core 1
  { 0xa, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 10, Core 0
  { 0xa, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 10, Core 1
  { 0xb, 0x0, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 11, Core 0
  { 0xb, 0x1, 0x0, 0x0, 0x0, (UINT64)0xFFFFFFFF }, // Cluster 11, Core 1
};

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
  return RETURN_SUCCESS;
}

STATIC
EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  *CoreCount    = ARRAY_SIZE (mSynQuacerInfoTable);
  *ArmCoreTable = mSynQuacerInfoTable;

  return EFI_SUCCESS;
}

STATIC ARM_MP_CORE_INFO_PPI       mMpCoreInfoPpi = {
  PrePeiCoreGetMpCoreInfo
};

STATIC EFI_PEI_PPI_DESCRIPTOR     mPlatformPpiTable[] = {
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
  *PpiListSize = sizeof mPlatformPpiTable;
  *PpiList = mPlatformPpiTable;
}
