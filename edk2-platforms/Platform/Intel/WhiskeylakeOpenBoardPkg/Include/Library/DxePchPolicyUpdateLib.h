/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_PCH_POLICY_UPDATE_LIB_H_
#define _DXE_PCH_POLICY_UPDATE_LIB_H_

/**
  Get data for platform policy from setup options.

  @param[in] PchPolicy               The pointer to get PCH Policy protocol instance

  @retval EFI_SUCCESS               Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxePchPolicy (
  IN OUT  PCH_POLICY_PROTOCOL    *PchPolicy
  );

#endif

