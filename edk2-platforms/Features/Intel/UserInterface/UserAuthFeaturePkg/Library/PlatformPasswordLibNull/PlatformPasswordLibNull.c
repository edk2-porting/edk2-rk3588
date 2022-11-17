/** @file
  NULL PlatformPasswordLib instance does NOT really detect whether the password is cleared
  but returns the PCD value directly. This instance can be used to verify security
  related features during platform enabling and development. It should be replaced
  by a platform-specific method(e.g. Button pressed) in a real platform for product.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

BOOLEAN       mPasswordCleared      = FALSE;

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
  )
{
  return mPasswordCleared;
}

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
  )
{
  return FALSE;
}


/**
  Save password clear state from a PCD to mPasswordCleared.

  @param  ImageHandle   ImageHandle of the loaded driver.
  @param  SystemTable   Pointer to the EFI System Table.

  @retval  EFI_SUCCESS          PcdPasswordCleared is got successfully.

**/
EFI_STATUS
EFIAPI
PlatformPasswordLibNullConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  mPasswordCleared = PcdGetBool(PcdPasswordCleared);

  return EFI_SUCCESS;
}

