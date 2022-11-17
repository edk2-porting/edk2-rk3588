/** @file

  Copyright (c) 2020, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef BCM_GENET_PLATFORM_DEVICE_H
#define BCM_GENET_PLATFORM_DEVICE_H

#include <Uefi/UefiBaseType.h>

#define BCM_GENET_PLATFORM_DEVICE_PROTOCOL_GUID \
  {0x5e485a22, 0x1bb0, 0x4e22, {0x85, 0x49, 0x41, 0xfc, 0xec, 0x85, 0xdf, 0xd3}}

typedef struct {
  EFI_PHYSICAL_ADDRESS      BaseAddress;
  EFI_MAC_ADDRESS           MacAddress;
} BCM_GENET_PLATFORM_DEVICE_PROTOCOL;

extern EFI_GUID gBcmGenetPlatformDeviceProtocolGuid;

#endif
