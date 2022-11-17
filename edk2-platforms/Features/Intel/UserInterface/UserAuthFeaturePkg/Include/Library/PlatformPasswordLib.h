/** @file
  Provides a platform-specific method to return password policy.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __PLATFORM_PASSWORD_LIB_H__
#define __PLATFORM_PASSWORD_LIB_H__

/**
  This function is called at password driver entrypoint.
  This function should be called only once, to clear the password.

  This function provides a way to reset the password, just in case
  the platform owner forgets the password.
  The platform should provide a secure way to make sure
  only the platform owner is allowed to clear password.

  Once the password is cleared, the platform should provide a way
  to set a new password.

  @retval TRUE  There is a platform request to clear the password.
  @retval FALSE There is no platform request to clear the password.
**/
BOOLEAN
EFIAPI
IsPasswordCleared (
  VOID
  );

/**
  This function is called if the password driver finds that the password is not enrolled,
  when the password is required to input.

  This function should return the action according to platform policy.

  @retval TRUE  The caller should force the user to enroll the password.
  @retval FALSE The caller may skip the password enroll.
**/
BOOLEAN
EFIAPI
NeedEnrollPassword (
  VOID
  );

#endif
