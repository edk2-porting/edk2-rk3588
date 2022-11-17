/** @file
  This library identifies the firmware boot media device.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/FirmwareBootMediaInfoLib.h>

/**
  Identifies the firmware boot media type for the current boot.

  @retval BOOT_MEDIA_TYPE   The boot media type. A value of FwBootMediaTypeMax indicates the firmware boot media
                            information is unavailable.
**/
FW_BOOT_MEDIA_TYPE
IdentifyFirmwareBootMediaType (
  VOID
  )
{
  return FwBootMediaSpi;
}
