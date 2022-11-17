/** @file
  This Driver mainly provides Setup Form to change password.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "UserAuthentication2Dxe.h"

USER_AUTHENTICATION_PRIVATE_DATA   *mUserAuthenticationData = NULL;

EFI_GUID mUserAuthenticationVendorGuid = USER_AUTHENTICATION_FORMSET_GUID;
HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    USER_AUTHENTICATION_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

/**
  Display a message box to end user.

  @param[in] DisplayString   The string in message box.
**/
VOID
MessageBox (
  IN CHAR16  *DisplayString
  )
{
  EFI_INPUT_KEY  Key;

  do {
    CreatePopUp (
      EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
      &Key,
      L"",
      DisplayString,
      L"Press ENTER to continue ...",
      L"",
      NULL
      );
  } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);
}

/**
  Force system reset.
**/
VOID
ForceSystemReset (
  VOID
  )
{
  MessageBox (L"Password retry count reach, reset system!");
  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  CpuDeadLoop();
}

/**
  Display message for set password.

  @param[in]  ReturnStatus   The return status for set password.
**/
VOID
PrintSetPasswordStatus (
  IN EFI_STATUS  ReturnStatus
  )
{
  CHAR16         *DisplayString;
  CHAR16         *DisplayString2;

  EFI_INPUT_KEY  Key;

  if (ReturnStatus == EFI_UNSUPPORTED) {
    DisplayString  = L"New password is not strong enough!";
    DisplayString2 = L"Password must at least 8 chars and include lowercase, uppercase alphabetic, number and symbol";

    do {
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &Key,
        L"",
        DisplayString,
        DisplayString2,
        L"Press ENTER to continue ...",
        L"",
        NULL
        );
    } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);
  } else {
    if (ReturnStatus == EFI_SUCCESS) {
      DisplayString = L"New password is updated successfully!";
    } else if (ReturnStatus == EFI_ALREADY_STARTED) {
      DisplayString = L"New password is found in the history passwords!";
    } else {
      DisplayString = L"New password update fails!";
    }

    do {
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &Key,
        L"",
        DisplayString,
        L"Press ENTER to continue ...",
        L"",
        NULL
        );
    } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);
  }
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Request                A null-terminated Unicode string in
                                 <ConfigRequest> format.
  @param  Progress               On return, points to a character in the Request
                                 string. Points to the string's null terminator if
                                 request was successful. Points to the most recent
                                 '&' before the first failing name/value pair (or
                                 the beginning of the string if the failure is in
                                 the first name/value pair) if the request was not
                                 successful.
  @param  Results                A null-terminated Unicode string in
                                 <ConfigAltResp> format which has all values filled
                                 in for the names in the Request string. String to
                                 be allocated by the called function.

  @retval EFI_SUCCESS            The Results is filled with the requested values.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Progress = Request;
  return EFI_NOT_FOUND;
}


/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Configuration          A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param  Progress               A pointer to a string filled in with the offset of
                                 the most recent '&' before the first failing
                                 name/value pair (or the beginning of the string if
                                 the failure is in the first name/value pair) or
                                 the terminating NULL if all was successful.

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  return EFI_NOT_FOUND;
}

/**
  HII update Admin Password status.

**/
VOID
HiiUpdateAdminPasswordStatus (
  VOID
  )
{
  if (IsPasswordInstalled ()) {
    HiiSetString (
      mUserAuthenticationData->HiiHandle,
      STRING_TOKEN (STR_ADMIN_PASSWORD_STS_CONTENT),
      L"Installed",
      NULL
      );
  } else {
    HiiSetString (
      mUserAuthenticationData->HiiHandle,
      STRING_TOKEN (STR_ADMIN_PASSWORD_STS_CONTENT),
      L"Not Installed",
      NULL
      );
  }
}

/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.

**/
EFI_STATUS
EFIAPI
UserAuthenticationCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS  Status;
  CHAR16      *UserInputPassword;

  Status = EFI_SUCCESS;

  if (((Value == NULL) && (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE)) ||
      (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Action) {
  case EFI_BROWSER_ACTION_FORM_OPEN:
    {
      switch (QuestionId) {
      case ADMIN_PASSWORD_KEY_ID:
        HiiUpdateAdminPasswordStatus ();
      default:
        break;
      }
    }
    break;
  case EFI_BROWSER_ACTION_CHANGING:
    {
      switch (QuestionId) {
      case ADMIN_PASSWORD_KEY_ID:
        if ((Type == EFI_IFR_TYPE_STRING) && (Value->string == 0) &&
            (mUserAuthenticationData->PasswordState == BROWSER_STATE_SET_PASSWORD)) {
          mUserAuthenticationData->PasswordState = BROWSER_STATE_VALIDATE_PASSWORD;
          ZeroMem (mUserAuthenticationData->OldPassword, sizeof(mUserAuthenticationData->OldPassword));
          return EFI_INVALID_PARAMETER;
        }
        //
        // The Callback is responsible for validating old password input by user,
        // If Callback return EFI_SUCCESS, it indicates validation pass.
        //
        switch (mUserAuthenticationData->PasswordState) {
        case BROWSER_STATE_VALIDATE_PASSWORD:
          UserInputPassword = HiiGetString (mUserAuthenticationData->HiiHandle, Value->string, NULL);
          if (UserInputPassword == NULL) {
            return EFI_UNSUPPORTED;
          }
          if ((StrLen (UserInputPassword) >= PASSWORD_MAX_SIZE)) {
            Status = EFI_NOT_READY;
            break;
          }
          if (UserInputPassword[0] == 0) {
            //
            // Setup will use an empty password to check whether the old password is set,
            // If the validation is successful, means there is no old password, return
            // success to set the new password. Or need to return EFI_NOT_READY to
            // let user input the old password.
            //
            Status = VerifyPassword (UserInputPassword, StrSize (UserInputPassword));
            if (Status == EFI_SUCCESS) {
              mUserAuthenticationData->PasswordState = BROWSER_STATE_SET_PASSWORD;
            } else {
              Status = EFI_NOT_READY;
            }
            break;
          }
          Status = VerifyPassword (UserInputPassword, StrSize (UserInputPassword));
          if (Status == EFI_SUCCESS) {
            mUserAuthenticationData->PasswordState = BROWSER_STATE_SET_PASSWORD;
            StrCpyS (
              mUserAuthenticationData->OldPassword,
              sizeof(mUserAuthenticationData->OldPassword)/sizeof(CHAR16),
              UserInputPassword
              );
          } else {
            //
            // Old password mismatch, return EFI_NOT_READY to prompt for error message.
            //
            if (Status == EFI_ACCESS_DENIED) {
              //
              // Password retry count reach.
              //
              ForceSystemReset ();
            }
            Status = EFI_NOT_READY;
          }
          break;

        case BROWSER_STATE_SET_PASSWORD:
          UserInputPassword = HiiGetString (mUserAuthenticationData->HiiHandle, Value->string, NULL);
          if (UserInputPassword == NULL) {
            return EFI_UNSUPPORTED;
          }
          if ((StrLen (UserInputPassword) >= PASSWORD_MAX_SIZE)) {
            Status = EFI_NOT_READY;
            break;
          }
          Status = SetPassword (UserInputPassword, StrSize (UserInputPassword), mUserAuthenticationData->OldPassword, StrSize(mUserAuthenticationData->OldPassword));
          PrintSetPasswordStatus (Status);
          ZeroMem (mUserAuthenticationData->OldPassword, sizeof(mUserAuthenticationData->OldPassword));
          mUserAuthenticationData->PasswordState = BROWSER_STATE_VALIDATE_PASSWORD;
          HiiUpdateAdminPasswordStatus ();
          break;

        default:
          break;
        }
      default:
        break;
      }
    }
    break;
  default:
    break;
  }
  return Status;
}

/**
  User Authentication entry point.

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval EFI_SUCCESS    The entry point is executed successfully.
  @return  other         Contain some other errors.

**/
EFI_STATUS
EFIAPI
UserAuthentication2Entry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE        DriverHandle;
  EFI_HII_HANDLE    HiiHandle;

  DriverHandle  = NULL;

  mUserAuthenticationData = AllocateZeroPool (sizeof (USER_AUTHENTICATION_PRIVATE_DATA));
  if (mUserAuthenticationData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mUserAuthenticationData->ConfigAccess.ExtractConfig = ExtractConfig;
  mUserAuthenticationData->ConfigAccess.RouteConfig = RouteConfig;
  mUserAuthenticationData->ConfigAccess.Callback = UserAuthenticationCallback;
  mUserAuthenticationData->PasswordState = BROWSER_STATE_VALIDATE_PASSWORD;

  //
  // Install Config Access protocol to driver handle.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mUserAuthenticationData->ConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  mUserAuthenticationData->DriverHandle = DriverHandle;

  //
  // Add HII data to database.
  //
  HiiHandle = HiiAddPackages (
                   &mUserAuthenticationVendorGuid,
                   DriverHandle,
                   UserAuthentication2DxeStrings,
                   UserAuthenticationDxeVfrBin,
                   NULL
                   );
  if (HiiHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mUserAuthenticationData->HiiHandle = HiiHandle;

  return EFI_SUCCESS;
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
UserAuthentication2Unload (
  IN EFI_HANDLE  ImageHandle
  )
{
  ASSERT (mUserAuthenticationData != NULL);

  //
  // Uninstall Config Access Protocol.
  //
  if (mUserAuthenticationData->DriverHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mUserAuthenticationData->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiVendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mUserAuthenticationData->ConfigAccess,
           NULL
           );
    mUserAuthenticationData->DriverHandle = NULL;
  }

  //
  // Remove Hii Data.
  //
  if (mUserAuthenticationData->HiiHandle != NULL) {
    HiiRemovePackages (mUserAuthenticationData->HiiHandle);
  }

  FreePool (mUserAuthenticationData);
  mUserAuthenticationData = NULL;

  return EFI_SUCCESS;
}

