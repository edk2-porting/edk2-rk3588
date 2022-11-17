/** @file
  Header file for the PolicyInitDxe Driver.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _POLICY_INIT_DXE_H_
#define _POLICY_INIT_DXE_H_


#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>

#include "SaPolicyInitDxe.h"
#include "PchPolicyInitDxe.h"
#include "SiliconPolicyInitDxe.h"
#include "GopPolicyInitDxe.h"
#include "CpuPolicyInitDxe.h"

#include <Library/DxeTbtPolicyLib.h>
/**
  Initialize DXE Platform Policy

  @param[in] ImageHandle - Image handle of this driver.
  @param[in] SystemTable - Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/

EFI_STATUS
EFIAPI
PolicyInitDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

#endif

