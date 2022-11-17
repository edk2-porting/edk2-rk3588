/** @file
  This Driver mainly provides Setup Form to change password and
  does user authentication before entering Setup.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "UserAuthenticationDxe.h"

EFI_EVENT                           mExitBootServicesEvent  = NULL;
EFI_RSC_HANDLER_PROTOCOL           *mRscHandlerProtocol     = NULL;
USER_AUTHENTICATION_PRIVATE_DATA   *mUserAuthenticationData = NULL;
EFI_SMM_COMMUNICATION_PROTOCOL     *mSmmCommunication       = NULL;

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
  Get a user input string.

  @param[in]       PopUpString      A popup string to inform user.
  @param[in, out]  UserInput        The user input string
  @param[in]       UserInputMaxLen  The max unicode count of the UserInput without NULL terminator.
**/
EFI_STATUS
GetUserInput (
  IN     CHAR16      *PopUpString,
  IN OUT CHAR16      *UserInput,
  IN     UINTN       UserInputMaxLen
  )
{
  EFI_INPUT_KEY                InputKey;
  UINTN                        InputLength;
  CHAR16                       *Mask;

  UserInput[0] = 0;
  Mask = AllocateZeroPool ((UserInputMaxLen + 1) * sizeof(CHAR16));
  if (Mask == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  InputLength = 0;

  while (TRUE) {
    if (InputLength < UserInputMaxLen) {
      Mask[InputLength] = L'_';
    }
    CreatePopUp (
      EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
      &InputKey,
      PopUpString,
      L"--------------------------------",
      Mask,
      NULL
      );
    if (InputKey.ScanCode == SCAN_NULL) {
      //
      // Check whether finish inputing password.
      //
      if (InputKey.UnicodeChar == CHAR_CARRIAGE_RETURN && InputLength > 0) {
        //
        // Add the null terminator.
        //
        UserInput[InputLength] = 0;
        break;
      } else if ((InputKey.UnicodeChar == CHAR_NULL) ||
                 (InputKey.UnicodeChar == CHAR_LINEFEED) ||
                 (InputKey.UnicodeChar == CHAR_CARRIAGE_RETURN)
                ) {
        continue;
      } else {
        //
        // delete last key entered
        //
        if (InputKey.UnicodeChar == CHAR_BACKSPACE) {
          if (InputLength > 0) {
            UserInput[InputLength] = 0;
            Mask[InputLength] = 0;
            InputLength--;
          }
        } else {
          if (InputLength == UserInputMaxLen) {
            Mask[InputLength] = 0;
            continue;
          }
          //
          // add Next key entry
          //
          UserInput[InputLength] = InputKey.UnicodeChar;
          Mask[InputLength] = L'*';
          InputLength++;
        }
      }
    }
  }
  FreePool (Mask);
  return EFI_SUCCESS;
}

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
  Require user input password.

  @retval TRUE   User input correct password successfully.
  @retval FALSE  The password is not set.
**/
BOOLEAN
RequireUserPassword (
  VOID
  )
{
  EFI_STATUS                                    Status;
  CHAR16                                        UserInputPw[PASSWORD_MAX_SIZE];
  CHAR16                                        *PopUpString;
  SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY        VerifyPolicy;

  Status = EFI_SUCCESS;
  ZeroMem(UserInputPw, sizeof(UserInputPw));

  if (!IsPasswordInstalled ()) {
    return FALSE;
  }

  Status = GetPasswordVerificationPolicy (&VerifyPolicy);
  if (!EFI_ERROR (Status)) {
    if (WasPasswordVerified() && (!VerifyPolicy.NeedReVerify)) {
      DEBUG ((DEBUG_INFO, "Password was verified and Re-verify is not needed\n"));
      return TRUE;
    }
  }

  PopUpString = L"Please input admin password";

  while (TRUE) {
    gST->ConOut->ClearScreen(gST->ConOut);
    GetUserInput (PopUpString, UserInputPw, PASSWORD_MAX_SIZE - 1);

    Status = VerifyPassword (UserInputPw, StrSize(UserInputPw));
    if (!EFI_ERROR(Status)) {
      break;
    }
    if (Status == EFI_ACCESS_DENIED) {
      //
      // Password retry count reach.
      //
      ForceSystemReset ();
    }
    MessageBox (L"Incorrect password!");
  }

  ZeroMem(UserInputPw, sizeof(UserInputPw));

  gST->ConOut->ClearScreen(gST->ConOut);

  return TRUE;
}

/**
  Set user password.

**/
VOID
SetUserPassword (
  VOID
  )
{
  EFI_STATUS                   Status;
  CHAR16                       UserInputPw[PASSWORD_MAX_SIZE];
  CHAR16                       TmpPassword[PASSWORD_MAX_SIZE];
  CHAR16                       *PopUpString;
  CHAR16                       *PopUpString2;

  ZeroMem(UserInputPw, sizeof(UserInputPw));
  ZeroMem(TmpPassword, sizeof(TmpPassword));

  PopUpString = L"Please set admin password";

  while (TRUE) {
    gST->ConOut->ClearScreen(gST->ConOut);
    GetUserInput (PopUpString, UserInputPw, PASSWORD_MAX_SIZE - 1);

    PopUpString2 = L"Please confirm your new password";
    gST->ConOut->ClearScreen(gST->ConOut);
    GetUserInput (PopUpString2, TmpPassword, PASSWORD_MAX_SIZE - 1);
    if (StrCmp (TmpPassword, UserInputPw) != 0) {
      MessageBox (L"Password are not the same!");
      continue;
    }

    Status = SetPassword (UserInputPw, StrSize(UserInputPw), NULL, 0);
    PrintSetPasswordStatus (Status);
    if (!EFI_ERROR(Status)) {
      break;
    }
  }

  ZeroMem(UserInputPw, sizeof(UserInputPw));
  ZeroMem(TmpPassword, sizeof(TmpPassword));

  gST->ConOut->ClearScreen(gST->ConOut);
}

/**
  Check password before entering into setup.

  @param  CodeType      Indicates the type of status code being reported.  Type EFI_STATUS_CODE_TYPE is defined in "Related Definitions" below.

  @param  Value         Describes the current status of a hardware or software entity.
                        This included information about the class and subclass that is used to classify the entity
                        as well as an operation.  For progress codes, the operation is the current activity.
                        For error codes, it is the exception.  For debug codes, it is not defined at this time.
                        Type EFI_STATUS_CODE_VALUE is defined in "Related Definitions" below.
                        Specific values are discussed in the Intel? Platform Innovation Framework for EFI Status Code Specification.

  @param  Instance      The enumeration of a hardware or software entity within the system.
                        A system may contain multiple entities that match a class/subclass pairing.
                        The instance differentiates between them.  An instance of 0 indicates that instance information is unavailable,
                        not meaningful, or not relevant.  Valid instance numbers start with 1.


  @param  CallerId      This optional parameter may be used to identify the caller.
                        This parameter allows the status code driver to apply different rules to different callers.
                        Type EFI_GUID is defined in InstallProtocolInterface() in the UEFI 2.0 Specification.


  @param  Data          This optional parameter may be used to pass additional data

  @retval EFI_SUCCESS             Status code is what we expected.
  @retval EFI_UNSUPPORTED         Status code not supported.

**/
EFI_STATUS
EFIAPI
CheckForPassword (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId, OPTIONAL
  IN EFI_STATUS_CODE_DATA     *Data      OPTIONAL
  )
{
  BOOLEAN   PasswordSet;

  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) &&
      (Value == (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_PC_USER_SETUP))) {
    //
    // Check whether enter setup page.
    //
    PasswordSet = RequireUserPassword ();
    if (PasswordSet) {
      DEBUG ((DEBUG_INFO, "Welcome Admin!\n"));
    } else {
      DEBUG ((DEBUG_INFO, "Admin password is not set!\n"));
      if (NeedEnrollPassword()) {
        SetUserPassword ();
      }
    }

    return EFI_SUCCESS;
  } else{
    return EFI_UNSUPPORTED;
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
          if ((StrLen (UserInputPassword) >= PASSWORD_MAX_SIZE)) {
            Status = EFI_NOT_READY;
            break;
          }
          if (UserInputPassword[0] == 0) {
            //
            // Setup will use a NULL password to check whether the old password is set,
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
  Unregister status code callback functions.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
UnregisterBootTimeHandlers (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  mRscHandlerProtocol->Unregister (CheckForPassword);
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
UserAuthenticationEntry (
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
                   UserAuthenticationDxeStrings,
                   UserAuthenticationDxeVfrBin,
                   NULL
                   );
  if (HiiHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mUserAuthenticationData->HiiHandle = HiiHandle;

  //
  // Locate report status code protocol.
  //
  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mRscHandlerProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  //
  //Register the callback function for ReportStatusCode() notification.
  //
  mRscHandlerProtocol->Register (CheckForPassword, TPL_HIGH_LEVEL);

  //
  // Unregister boot time report status code listener at ExitBootService Event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UnregisterBootTimeHandlers,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Locates SMM Communication protocol.
  //
  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &mSmmCommunication);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
UserAuthenticationUnload (
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

