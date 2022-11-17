/** @file
  This file is a wrapper for Platform Policy driver. Get Setup
  Value to initialize Intel DXE Platform Policy.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PolicyInitDxe.h"
#include <CpuSmm.h>
#include "BoardInitLib.h"

GLOBAL_REMOVE_IF_UNREFERENCED UINT8            mFirmwareConfiguration = 0;

/**
  Initialize  DXE Platform Policy

  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PolicyInitDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS           Status;

  Status = BoardConfigInit();

  mFirmwareConfiguration = FwConfigProduction;
  //
  // SystemAgent Dxe Platform Policy Initialization
  //
  Status = SaPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "SystemAgent Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // PCH Dxe Platform Policy Initialization
  //
  Status = PchPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "PCH Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // Silicon Dxe Platform Policy Initialization
  //
  Status = SiliconPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "Silicon Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // CPU DXE Platform Policy Initialization
  //
  Status = CpuPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "Cpu Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);


  if (PcdGetBool(PcdIntelGopEnable)) {
    //
    // GOP Dxe Policy Initialization
    //
    Status = GopPolicyInitDxe(ImageHandle);
    DEBUG((DEBUG_INFO, "GOP Dxe Policy Initialization done\n"));
    ASSERT_EFI_ERROR(Status);
  }
  if (PcdGetBool(PcdTbtEnable)) {
    //
    // Update TBT Policy
    //
    Status = InstallTbtPolicy (ImageHandle);
    DEBUG ((DEBUG_INFO, "Install Tbt Policy done\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;

}

