/** @file
  This code supports a private implementation of the Legacy Region protocol.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _LEGACY_REGION_H_
#define _LEGACY_REGION_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/LegacyRegion2.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/Cpu.h>
#include <IndustryStandard/Pci22.h>
#include <SaAccess.h>

/**
  Install Driver to produce Legacy Region protocol.

  @param[in] ImageHandle             Handle for the image of this driver

  @retval EFI_SUCCESS - Legacy Region protocol installed
  @retval Other       - No protocol installed, unload driver.
**/
EFI_STATUS
LegacyRegionInstall (
  IN EFI_HANDLE           ImageHandle
  );
#endif
