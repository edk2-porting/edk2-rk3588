/** @file
*
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>

#include <Library/ArmGicLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include "ArmPlatform.h"

typedef enum {
  ARM_FVP_BASE_AEMv8x4_AEMv8x4_GICV2,
  ARM_FVP_BASE_AEMv8x4_AEMv8x4_GICV2_LEGACY,
  ARM_FVP_BASE_AEMv8x4_AEMv8x4_GICV3,
  ARM_FVP_FOUNDATION_GICV2,
  ARM_FVP_FOUNDATION_GICV2_LEGACY,
  ARM_FVP_FOUNDATION_GICV3,
  ARM_FVP_UNKNOWN,
} ARM_VEXPRESS_PLATFORM_ID;

ARM_VEXPRESS_PLATFORM_ID
GetPlatformId (
  VOID
  )
{
  UINT32                SysId;
  UINT32                FvpSysId;
  UINT32                VariantSysId;
  ARM_GIC_ARCH_REVISION GicRevision;

  SysId = MmioRead32 (ARM_VE_SYS_ID_REG);

  // Remove the GIC variant to identify if we are running on the FVP Base or
  // Foundation models
  FvpSysId     = SysId & (ARM_FVP_SYS_ID_HBI_MASK | ARM_FVP_SYS_ID_PLAT_MASK );
  // Extract the variant from the SysId
  VariantSysId = SysId & ARM_FVP_SYS_ID_VARIANT_MASK;

  if (FvpSysId == ARM_FVP_BASE_BOARD_SYS_ID) {
    if (VariantSysId == ARM_FVP_GIC_VE_MMAP) {
      // FVP Base Model with legacy GIC memory map -- no longer supported
      return ARM_FVP_BASE_AEMv8x4_AEMv8x4_GICV2_LEGACY;
    } else {
      GicRevision = ArmGicGetSupportedArchRevision ();

      if (GicRevision == ARM_GIC_ARCH_REVISION_2) {
        // FVP Base Model with GICv2 support
        return ARM_FVP_BASE_AEMv8x4_AEMv8x4_GICV2;
      } else {
        // FVP Base Model with GICv3 support
        return ARM_FVP_BASE_AEMv8x4_AEMv8x4_GICV3;
      }
    }
  } else if (FvpSysId == ARM_FVP_FOUNDATION_BOARD_SYS_ID) {
    if (VariantSysId == ARM_FVP_GIC_VE_MMAP) {
      // FVP Foundation Model with legacy GIC memory map -- no longer supported
      return ARM_FVP_FOUNDATION_GICV2_LEGACY;
    } else {
      GicRevision = ArmGicGetSupportedArchRevision ();

      if (GicRevision == ARM_GIC_ARCH_REVISION_2) {
        // FVP Foundation Model with GICv2
        return ARM_FVP_FOUNDATION_GICV2;
      } else {
        // FVP Foundation Model with GICv3
        return ARM_FVP_FOUNDATION_GICV3;
      }
    }
  }
  return ARM_FVP_UNKNOWN;
}

/**
  Return a pool allocated copy of the DTB image that is appropriate for
  booting the current platform via DT.

  @param[out]   Dtb                   Pointer to the DTB copy
  @param[out]   DtbSize               Size of the DTB copy

  @retval       EFI_SUCCESS           Operation completed successfully
  @retval       EFI_NOT_FOUND         No suitable DTB image could be located
  @retval       EFI_OUT_OF_RESOURCES  No pool memory available

**/
EFI_STATUS
EFIAPI
DtPlatformLoadDtb (
  OUT   VOID        **Dtb,
  OUT   UINTN       *DtbSize
  )
{
  EFI_STATUS                Status;
  VOID                      *OrigDtb;
  VOID                      *CopyDtb;
  UINTN                     OrigDtbSize;
  ARM_VEXPRESS_PLATFORM_ID  PlatformId;

  PlatformId = GetPlatformId ();
  ASSERT (PlatformId < ARM_FVP_UNKNOWN);
  if (PlatformId >= ARM_FVP_UNKNOWN) {
    return EFI_NOT_FOUND;
  }

  Status = GetSectionFromAnyFv (&gDtPlatformDefaultDtbFileGuid,
             EFI_SECTION_RAW, (UINTN)PlatformId, &OrigDtb, &OrigDtbSize);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  CopyDtb = AllocateCopyPool (OrigDtbSize, OrigDtb);
  if (CopyDtb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *Dtb = CopyDtb;
  *DtbSize = OrigDtbSize;

  return EFI_SUCCESS;
}
