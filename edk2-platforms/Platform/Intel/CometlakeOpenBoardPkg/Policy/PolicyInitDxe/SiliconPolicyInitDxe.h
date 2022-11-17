/** @file
  Header file for the SiliconPolicyInitDxe Driver.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SILICON_POLICY_INIT_DXE_H_
#define _SILICON_POLICY_INIT_DXE_H_

#include <Protocol/FirmwareVolume2.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Protocol/SiPolicyProtocol.h>

/**
  Initilize Intel CPU DXE Policy

  @param[in] ImageHandle             Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
SiliconPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

#endif

