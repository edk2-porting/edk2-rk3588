/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_CPU_POLICY_UPDATE_LIB_H_
#define _DXE_CPU_POLICY_UPDATE_LIB_H_

#include <PiDxe.h>
#include <PchAccess.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/CpuPolicyProtocol.h>

/**

  This function prints the CPU DXE phase policy.

  @param[in] DxeCpuPolicy - CPU DXE Policy protocol

**/
VOID
CpuDxePrintPolicyProtocol (
  IN  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  );

/**

Routine Description:

  This function updates Dxe Cpu Policy Protocol

Arguments:

  @param[in] DxeCpuPolicy                 The Cpu Policy protocol instance

Returns:

  @retval EFI_SUCCESS                     Initialization complete.
  @retval EFI_UNSUPPORTED                 The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES            Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR                Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UpdateDxeSiCpuPolicy (
  IN OUT  DXE_CPU_POLICY_PROTOCOL  *DxeCpuPolicy
  );

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
  );

#endif

