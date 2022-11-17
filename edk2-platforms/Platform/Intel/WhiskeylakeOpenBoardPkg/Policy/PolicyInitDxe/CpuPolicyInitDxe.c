/** @file
  This file is SampleCode for Intel Silicon DXE Platform Policy initialzation.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <CpuPolicyInitDxe.h>

DXE_CPU_POLICY_PROTOCOL mCpuPolicyData;

/**
  Initialize Intel CPU DXE Platform Policy

  @param[in] ImageHandle        Image handle of this driver.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
CpuPolicyInitDxe (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS                Status;

  ZeroMem(&mCpuPolicyData, sizeof (DXE_CPU_POLICY_PROTOCOL));
  mCpuPolicyData.Revision                         = DXE_CPU_POLICY_PROTOCOL_REVISION;

  UpdateDxeSiCpuPolicy(&mCpuPolicyData);

  //
  // Install CpuInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = CpuInstallPolicyProtocol(ImageHandle, &mCpuPolicyData);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

