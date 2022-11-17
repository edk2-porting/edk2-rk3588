/** @file
  Header file for the DxePolicyBoardConfig Library.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_POLICY_BOARD_CONFIG_LIB_H_
#define _DXE_POLICY_BOARD_CONFIG_LIB_H_

#include <Protocol/MePolicy.h>
#include <Protocol/SaPolicy.h>

/**
  This function performs DXE SA Policy update by board configuration.

  @param[in, out] DxeSaPolicy     DXE SA Policy

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdateDxeSaPolicyBoardConfig (
  IN OUT  SA_POLICY_PROTOCOL         *DxeSaPolicy
  );

#endif

