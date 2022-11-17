/** @file
  This library identifies the firmware boot media device.

  The firmware boot media device is used to make system initialization decisions in the boot flow dependent
  upon firmware boot media. Note that the firmware boot media is the storage media that the boot firmware is stored on.
  It is not the OS storage media which may be stored upon a different non-volatile storage device.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _FIRMWARE_BOOT_MEDIA_LIB_H_
#define _FIRMWARE_BOOT_MEDIA_LIB_H_

#include <Uefi.h>

typedef enum {
  FwBootMediaSpi,
  FwBootMediaUfs,
  FwBootMediaEmmc,
  FwBootMediaNvme,
  FwBootMediaMax
} FW_BOOT_MEDIA_TYPE;

typedef struct {
  FW_BOOT_MEDIA_TYPE BootMedia;
} FW_BOOT_MEDIA_HOB_DATA;

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
  );

/**
  Indicates whether the boot media is presently known.

  Note: Very early in boot this data may not be available.

  @retval TRUE        Boot media is known
  @retval FALSE       Boot media is not known
**/
BOOLEAN
EFIAPI
FirmwareBootMediaIsKnown (
  VOID
  );

#endif
