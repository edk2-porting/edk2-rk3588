/** @file
  Header file for UserAuthenticationDxe.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _USER_AUTHENTICATION_DXE_H_
#define _USER_AUTHENTICATION_DXE_H_


#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/SmmCommunication.h>

#include <Guid/MdeModuleHii.h>
#include <Guid/HiiPlatformSetupFormset.h>
#include <Guid/PiSmmCommunicationRegionTable.h>
#include <Guid/UserAuthentication.h>

#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformPasswordLib.h>

#include "UserAuthenticationDxeFormset.h"

extern UINT8  UserAuthenticationDxeVfrBin[];
extern UINT8  UserAuthenticationDxeStrings[];
extern EFI_SMM_COMMUNICATION_PROTOCOL *mSmmCommunication;

typedef struct {
  EFI_HII_CONFIG_ACCESS_PROTOCOL       ConfigAccess;
  EFI_HANDLE                           DriverHandle;
  EFI_HII_HANDLE                       HiiHandle;
  UINT8                                PasswordState;
  CHAR16                               OldPassword[PASSWORD_MAX_SIZE];
} USER_AUTHENTICATION_PRIVATE_DATA;

#pragma pack(1)
///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

/**
  Validate if the password is correct.

  @param[in] Password               The user input password.
  @param[in] PasswordSize           The size of Password in byte.

  @retval EFI_SUCCESS               The password is correct.
  @retval EFI_SECURITY_VIOLATION    The password is incorrect.
  @retval EFI_INVALID_PARAMETER     The password or size is invalid.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to verify the password.
  @retval EFI_ACCESS_DENIED         Password retry count reach.
**/
EFI_STATUS
VerifyPassword (
  IN   CHAR16       *Password,
  IN   UINTN        PasswordSize
  );

/**
  Set a new password.

  @param[in] NewPassword            The user input new password.
                                    NULL means clear password.
  @param[in] NewPasswordSize        The size of NewPassword in byte.
  @param[in] OldPassword            The user input old password.
                                    NULL means no old password.
  @param[in] OldPasswordSize        The size of OldPassword in byte.

  @retval EFI_SUCCESS               The NewPassword is set successfully.
  @retval EFI_SECURITY_VIOLATION    The OldPassword is incorrect.
  @retval EFI_INVALID_PARAMETER     The password or size is invalid.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to set the password.
  @retval EFI_ACCESS_DENIED         Password retry count reach.
  @retval EFI_UNSUPPORTED           NewPassword is not strong enough.
  @retval EFI_ALREADY_STARTED       NewPassword is in history.
**/
EFI_STATUS
SetPassword (
  IN   CHAR16       *NewPassword,     OPTIONAL
  IN   UINTN        NewPasswordSize,
  IN   CHAR16       *OldPassword,     OPTIONAL
  IN   UINTN        OldPasswordSize
  );

/**
  Return if the password is set.

  @retval TRUE      The password is set.
  @retval FALSE     The password is not set.
**/
BOOLEAN
IsPasswordInstalled (
  VOID
  );

/**
  Get password verification policy.

  @param[out] VerifyPolicy          Verification policy.

  @retval EFI_SUCCESS               Get verification policy successfully.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to get verification policy.
**/
EFI_STATUS
GetPasswordVerificationPolicy (
  OUT SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY    *VerifyPolicy
  );

/**
  Return if the password was verified.

  @retval TRUE      The password was verified.
  @retval FALSE     The password was not verified.
**/
BOOLEAN
WasPasswordVerified (
  VOID
  );

#endif
