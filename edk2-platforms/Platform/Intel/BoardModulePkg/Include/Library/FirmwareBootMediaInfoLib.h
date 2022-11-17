/** @file
  This library identifies the firmware boot media device.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _FIRMWARE_BOOT_MEDIA_INFO_LIB_H_
#define _FIRMWARE_BOOT_MEDIA_INFO_LIB_H_

#include <Uefi.h>
#include <Library/FirmwareBootMediaLib.h>

/**
  Identifies the firmware boot media type for the current boot.

  @retval BOOT_MEDIA_TYPE   The boot media type. A value of FwBootMediaTypeMax indicates the firmware boot media
                            information is unavailable.
**/
FW_BOOT_MEDIA_TYPE
IdentifyFirmwareBootMediaType (
  VOID
  );

#endif
