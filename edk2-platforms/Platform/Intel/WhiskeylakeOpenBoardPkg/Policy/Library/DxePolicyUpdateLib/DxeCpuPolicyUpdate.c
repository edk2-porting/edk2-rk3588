/** @file
  This file is the library for CPU DXE Policy initialization.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DxeCpuPolicyUpdateLib.h>

/**
  This function prints the CPU DXE phase policy.

  @param[in] DxeCpuPolicy - CPU DXE Policy protocol
**/
VOID
CpuDxePrintPolicyProtocol (
  IN  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "\n------------------------ CPU Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : %x\n", DxeCpuPolicy->Revision));
  ASSERT (DxeCpuPolicy->Revision == DXE_CPU_POLICY_PROTOCOL_REVISION);
  DEBUG ((DEBUG_INFO, "\n------------------------ CPU_DXE_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, "EnableDts : %x\n", DxeCpuPolicy->EnableDts));
  DEBUG ((DEBUG_INFO, "\n------------------------ CPU Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();
}

/**
  Get data for CPU policy from setup options.

  @param[in] DxeCpuPolicy              The pointer to get CPU Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeSiCpuPolicy (
  IN OUT  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  CpuInstallPolicyProtocol installs CPU Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeCpuPolicy               The pointer to CPU Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
CpuInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  DXE_CPU_POLICY_PROTOCOL     *DxeCpuPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Print CPU DXE Policy
  ///
  CpuDxePrintPolicyProtocol(DxeCpuPolicy);

  ///
  /// Install the DXE_CPU_POLICY_PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeCpuPolicyProtocolGuid,
                  DxeCpuPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


