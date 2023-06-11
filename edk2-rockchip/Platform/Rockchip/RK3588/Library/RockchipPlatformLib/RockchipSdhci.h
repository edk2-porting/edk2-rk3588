/** @file
 *
 *  Copyright (c) Rockchip Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef _ROCKCHIP_SDHCI_H_
#define _ROCKCHIP_SDHCI_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DmaLib.h>
#include <Protocol/EmbeddedExternalDevice.h>

EFI_STATUS
SdhciGetClockRate (
  IN UINTN TargetFreq,
  OUT UINTN *BaseFreq
  );
#endif
