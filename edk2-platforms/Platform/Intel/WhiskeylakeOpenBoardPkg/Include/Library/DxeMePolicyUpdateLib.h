/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_ME_POLICY_UPDATE_LIB_H_
#define _DXE_ME_POLICY_UPDATE_LIB_H_

/**
  Update the ME Policy Library

  @param[in] DxeMePolicy                The pointer to get ME Policy protocol instance

  @retval EFI_SUCCESS                   Initialization complete.
  @retval EFI_UNSUPPORTED               The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES          Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR              Device error, driver exits abnormally.

**/
EFI_STATUS
UpdateDxeMePolicy (
  IN OUT  ME_POLICY_PROTOCOL      *DxeMePolicy
  );

#endif // _DXE_ME_POLICY_UPDATE_LIB_H_

