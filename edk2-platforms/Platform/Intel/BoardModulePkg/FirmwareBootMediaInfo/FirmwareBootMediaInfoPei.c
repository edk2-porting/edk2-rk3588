/** @file
  Firmware Boot Media Info Module

  This module publishes firmware boot media device information used in the boot flow for system initialization
  decisions dependent upon the firmware boot media.

  This module depends upon a library instance to actually perform firmware boot media device detection since the
  detection mechanism will vary across systems. In many cases, the media type may simply be set to a single firmware
  boot media device with no run-time logic required. In any case, this module should dispatch as early as possible in
  the system boot flow so the firmware boot media information is available for other modules. If any dependencies are
  required to dynamically determine the firmware boot media device, those should be in the DEPEX section of the active
  FirmwareBootMediaInfoLib such that this module will dispatch once those dependencies are satisfied.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/FirmwareBootMediaLib.h>
#include <Library/FirmwareBootMediaInfoLib.h>
#include <Library/HobLib.h>

/**
  Produces a gFirmwareBootMediaHobGuid HOB instance.

  @retval     EFI_SUCCESS     The firmware boot media HOB was produced successfully.
  @retval     EFI_NOT_FOUND   The firmware boot media device information could not be found to produce the
                              the firmware boot media HOB.
**/
EFI_STATUS
EFIAPI
ProduceFirmwareBootMediaHob (
  VOID
  )
{
  FW_BOOT_MEDIA_HOB_DATA FwBootMediaHobData;

  FwBootMediaHobData.BootMedia = IdentifyFirmwareBootMediaType ();
  if (FwBootMediaHobData.BootMedia == FwBootMediaMax) {
    return EFI_NOT_FOUND;
  }

  BuildGuidDataHob (
    &gFirmwareBootMediaHobGuid,
    &FwBootMediaHobData,
    sizeof (FW_BOOT_MEDIA_HOB_DATA)
    );

  return EFI_SUCCESS;
}

/**
  Attempts to publish the firmware boot media HOB.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   Produce the BootMedia Hob successfully.

**/
EFI_STATUS
EFIAPI
FirmwareBootMediaInfoPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  Status = ProduceFirmwareBootMediaHob ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}
