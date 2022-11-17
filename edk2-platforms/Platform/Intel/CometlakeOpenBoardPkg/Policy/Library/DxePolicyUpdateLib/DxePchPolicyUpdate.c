/** @file
  This file is the library for PCH DXE Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <PchAccess.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/PchPolicy.h>
#include <ConfigBlock/HdAudioConfig.h>
#include <Library/ConfigBlockLib.h>

/**
  Get data for PCH policy from setup options.

  @param[in] PchPolicy                 The pointer to get PCH Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxePchPolicy (
  IN OUT  PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS              Status;
  PCH_HDAUDIO_DXE_CONFIG  *HdAudioDxeConfig;

  Status = GetConfigBlock ((VOID *)PchPolicy, &gHdAudioDxeConfigGuid, (VOID *)&HdAudioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

