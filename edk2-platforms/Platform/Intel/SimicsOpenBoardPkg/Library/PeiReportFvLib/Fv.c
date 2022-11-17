/** @file
  Build FV related hobs for platform.

  Copyright (c) 2006 - 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PiPei.h"
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>

BOOLEAN mS3Supported = TRUE;

/**
  Publish PEI & DXE (Decompressed) Memory based FVs to let PEI
  and DXE know about them.

  @retval EFI_SUCCESS   Platform PEI FVs were initialized successfully.

**/
EFI_STATUS
PeiFvInitialization (
  VOID
  )
{
  BOOLEAN SecureS3Needed;

  DEBUG ((EFI_D_INFO, "Platform PEI Firmware Volume Initialization\n"));

  DEBUG (
    (EFI_D_ERROR, "Firmware Volume HOB: 0x%x 0x%x\n",
      PcdGet32 (PcdSimicsPeiMemFvBase),
      PcdGet32 (PcdSimicsPeiMemFvSize)
      )
    );
  //
  // Create a memory allocation HOB for the PEI FV.
  //
  // Allocate as ACPI NVS is S3 is supported
  //
  BuildMemoryAllocationHob (
    PcdGet32 (PcdSimicsPeiMemFvBase),
    PcdGet32 (PcdSimicsPeiMemFvSize),
    mS3Supported ? EfiACPIMemoryNVS : EfiBootServicesData
    );

  //
  // Let DXE know about the DXE FV
  //
  BuildFvHob (PcdGet32 (PcdSimicsDxeMemFvBase), PcdGet32 (PcdSimicsDxeMemFvSize));

  SecureS3Needed = mS3Supported && FeaturePcdGet (PcdSmmSmramRequire);

  //
  // Create a memory allocation HOB for the DXE FV.
  //
  // If "secure" S3 is needed, then SEC will decompress both PEI and DXE
  // firmware volumes at S3 resume too, hence we need to keep away the OS from
  // DXEFV as well. Otherwise we only need to keep away DXE itself from the
  // DXEFV area.
  //
  BuildMemoryAllocationHob (
    PcdGet32 (PcdSimicsDxeMemFvBase),
    PcdGet32 (PcdSimicsDxeMemFvSize),
    SecureS3Needed ? EfiACPIMemoryNVS : EfiBootServicesData
    );

  //
  // Additionally, said decompression will use temporary memory above the end
  // of DXEFV, so let's keep away the OS from there too.
  //
  if (SecureS3Needed) {
    UINT32 DxeMemFvEnd;

    DxeMemFvEnd = PcdGet32 (PcdSimicsDxeMemFvBase) +
                  PcdGet32 (PcdSimicsDxeMemFvSize);
    BuildMemoryAllocationHob (
      DxeMemFvEnd,
      PcdGet32 (PcdSimicsDecompressionScratchEnd) - DxeMemFvEnd,
      EfiACPIMemoryNVS
      );
  }

  //
  // Let PEI know about the DXE FV so it can find the DXE Core
  //
  PeiServicesInstallFvInfoPpi (
    NULL,
    (VOID *)(UINTN) PcdGet32 (PcdSimicsDxeMemFvBase),
    PcdGet32 (PcdSimicsDxeMemFvSize),
    NULL,
    NULL
    );

  return EFI_SUCCESS;
}

