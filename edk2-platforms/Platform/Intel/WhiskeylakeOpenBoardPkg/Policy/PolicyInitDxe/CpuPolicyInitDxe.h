/** @file
  Header file for the SiliconPolicyInitDxe Driver.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_POLICY_INIT_DXE_H_
#define _CPU_POLICY_INIT_DXE_H_

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

#include <Protocol/CpuPolicyProtocol.h>
#include <Library/DxeCpuPolicyUpdateLib.h>


/**
  Initialize Intel CPU DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
CpuPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

#endif

