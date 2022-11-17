/** @file
  This library identifies the firmware boot media device.

  The firmware boot media device is used to make system initialization decisions in the boot flow dependent
  upon firmware boot media. Note that the firmware boot media is the storage media that the boot firmware is stored on.
  It is not the OS storage media which may be stored upon a different non-volatile storage device.

  This file contains implementation specific to the DXE and SMM boot phases.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/FirmwareBootMediaLib.h>
#include <Library/HobLib.h>

STATIC FW_BOOT_MEDIA_TYPE mFwBootMedia = FwBootMediaMax;

/**
  Determines the current platform firmware boot media device.

  @param[out] FwBootMediaType The current platform Boot Media type.

  @retval     EFI_SUCCESS     The operation completed successfully.
  @retval     EFI_NOT_FOUND   The boot media type could not be found.
**/
EFI_STATUS
EFIAPI
GetFirmwareBootMediaType (
  OUT FW_BOOT_MEDIA_TYPE   *FwBootMediaType
  )
{
  if (mFwBootMedia == FwBootMediaMax) {
    return EFI_NOT_FOUND;
  }

  *FwBootMediaType = mFwBootMedia;
  return EFI_SUCCESS;
}

/**
  Determines if the boot media is presently known.

  Note: Very early in boot this data may not be available.

  @retval TRUE        Boot media is known
  @retval FALSE       Boot media is not known
**/
BOOLEAN
EFIAPI
FirmwareBootMediaIsKnown (
  VOID
  )
{
  return mFwBootMedia != FwBootMediaMax;
}

/**
  The library constructor.

  This constructor depends upon gFirmwareBootMediaHobGuid.
  It should be produced by the platform in the HOB producer phase.

  @param[in]  ImageHandle           The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable           A pointer to the EFI system table.

  @retval     EFI_SUCCESS           The constructor executed successfully.
  @retval     EFI_NOT_FOUND         The gFirmwareBootMediaHobGuid HOB could not be located
                                    or the boot media type is invalid.
**/
EFI_STATUS
EFIAPI
DxeSmmFirmwareBootMediaLibInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  FW_BOOT_MEDIA_HOB_DATA    *BootMediaHobData;
  FW_BOOT_MEDIA_TYPE        BootMediaType;
  EFI_HOB_GUID_TYPE         *GuidHobPtr;

  GuidHobPtr  = GetFirstGuidHob (&gFirmwareBootMediaHobGuid);
  if (GuidHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "The firmware boot media HOB does not exist!\n"));
    ASSERT (GuidHobPtr != NULL);
    return EFI_NOT_FOUND;
  }

  BootMediaHobData = (FW_BOOT_MEDIA_HOB_DATA *) GET_GUID_HOB_DATA (GuidHobPtr);
  if (BootMediaHobData == NULL) {
    return EFI_NOT_FOUND;
  }

  BootMediaType = BootMediaHobData->BootMedia;

  if (BootMediaType >= FwBootMediaMax) {
    DEBUG ((DEBUG_ERROR, "The firmware boot media HOB specifies an invalid media type.\n"));
    return EFI_NOT_FOUND;
  }

  mFwBootMedia = BootMediaType;

  return EFI_SUCCESS;
}
