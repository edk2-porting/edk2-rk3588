/** @file
 Intel DXE SA Policy update by board configuration


  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "DxePolicyBoardConfig.h"
#include <Library/ConfigBlockLib.h>

/**
  This function performs DXE SA Policy update by board configuration.

  @param[in, out] DxeSaPolicy    DXE SA Policy

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdateDxeSaPolicyBoardConfig (
  IN OUT  SA_POLICY_PROTOCOL         *DxeSaPolicy
  )
{
  EFI_STATUS                         Status;
  MEMORY_DXE_CONFIG                  *MemoryDxeConfig;

  DEBUG((DEBUG_INFO, "Updating SA Policy by board config in DXE\n"));

  Status = GetConfigBlock ((VOID *)DxeSaPolicy, &gMemoryDxeConfigGuid, (VOID *)&MemoryDxeConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

