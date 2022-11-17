/** @file
  Provides services to do password authentication.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __USER_PASSWORD_UI_LIB_H__
#define __USER_PASSWORD_UI_LIB_H__

/**
  Do password authentication.

  @retval EFI_SUCCESS               Password authentication pass.
**/
EFI_STATUS
EFIAPI
UiDoPasswordAuthentication (
  VOID
  );

/**
  Set password verification policy.

  @param[in] NeedReVerify           Need re-verify or not.

  @retval EFI_SUCCESS               Set verification policy successfully.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to set verification policy.
**/
EFI_STATUS
EFIAPI
UiSetPasswordVerificationPolicy (
  IN BOOLEAN    NeedReVerify
  );

#endif
