/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_SA_POLICY_UPDATE_LIB_H_
#define _DXE_SA_POLICY_UPDATE_LIB_H_

/**
  Get data for platform policy from setup options.

  @param[in] SaPolicy               The pointer to get SA Policy protocol instance

  @retval EFI_SUCCESS               Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeSaPolicy (
  IN OUT  SA_POLICY_PROTOCOL    *SaPolicy
  );

#endif

