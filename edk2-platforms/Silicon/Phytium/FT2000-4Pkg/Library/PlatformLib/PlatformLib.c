/** @file
  Library for Phytium platform.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Ppi/ArmMpCoreInfo.h>

ARM_CORE_INFO mPhytiumMpCoreInfoTable[] = {
  {
    0x0, 0x0,              // Cluster 0, Core 0

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  }
};

/*
  This function geted the current Boot Mode.

  This function returns the boot reason on the platform.

  @return   Return the current Boot Mode of the platform.

*/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}


/**
  Initialize controllers that must setup in the normal world.

  This function is called by the ArmPlatformPkg/Pei or ArmPlatformPkg/Pei/PlatformPeim
  in the PEI phase.

  @retval      EFI_SUCCESS    ArmPlatformInitialize() is executed successfully.

**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  return RETURN_SUCCESS;
}


/**
  This function Inited the system (or sometimes called permanent) memory.

  This memory is generally represented by the DRAM.

  @param[in]   None.

  @retval      None.

**/
VOID
ArmPlatformInitializeSystemMemory (
  VOID
  )
{
  // Nothing to do here
}


/**
  This function geted the information of core.

  @param[out]  CoreCount      The count of CoreInfoTable.
  @param[out]  ArmCoreTable   The pointer of CoreInfoTable.

  @retval      EFI_SUCCESS    PrePeiCoreGetMpCoreInfo() is executed successfully.

**/
EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  *CoreCount    = PcdGet32 (PcdCoreCount);
  *ArmCoreTable = mPhytiumMpCoreInfoTable;

  return EFI_SUCCESS;
}

//
// Needs to be declared in the file. Otherwise gArmMpCoreInfoPpiGuid is
// undefined in the contect of PrePeiCore
//
EFI_GUID mArmMpCoreInfoPpiGuid = ARM_MP_CORE_INFO_PPI_GUID;
ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR gPlatformPpiTable[] =
{
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &mArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};


/**
  This function geted the information of Ppitable.

  @param[out]  PpiListSize      The size of Ppitable.
  @param[out]  PpiList          The pointer of Ppitable.

  @retval      None.

**/
VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}
