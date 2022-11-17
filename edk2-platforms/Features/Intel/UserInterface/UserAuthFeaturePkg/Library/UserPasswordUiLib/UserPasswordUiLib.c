/** @file
  UserPasswordUiLib instance provides services to do password authentication.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Protocol/SmmCommunication.h>

#include <Guid/PiSmmCommunicationRegionTable.h>
#include <Guid/UserAuthentication.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/PlatformPasswordLib.h>
#include <Library/UserPasswordLib.h>

/**
  Initialize the communicate buffer using DataSize and Function.

  @param[out]      DataPtr          Points to the data in the communicate buffer.
  @param[in]       DataSize         The data size to send to SMM.
  @param[in]       Function         The function number to initialize the communicate header.

  @return Communicate buffer.
**/
VOID*
UserPasswordUiLibInitCommunicateBuffer (
  OUT     VOID                              **DataPtr OPTIONAL,
  IN      UINTN                             DataSize,
  IN      UINTN                             Function
  )
{
  EFI_SMM_COMMUNICATE_HEADER                *SmmCommunicateHeader;
  SMM_PASSWORD_COMMUNICATE_HEADER           *SmmPasswordFunctionHeader;
  VOID                                      *Buffer;
  EDKII_PI_SMM_COMMUNICATION_REGION_TABLE   *SmmCommRegionTable;
  EFI_MEMORY_DESCRIPTOR                     *SmmCommMemRegion;
  UINTN                                     Index;
  UINTN                                     Size;
  EFI_STATUS                                Status;

  Buffer = NULL;
  Status = EfiGetSystemConfigurationTable (
             &gEdkiiPiSmmCommunicationRegionTableGuid,
             (VOID **) &SmmCommRegionTable
             );
  if (EFI_ERROR (Status)) {
    return NULL;
  }
  ASSERT (SmmCommRegionTable != NULL);
  SmmCommMemRegion = (EFI_MEMORY_DESCRIPTOR *) (SmmCommRegionTable + 1);
  Size = 0;
  for (Index = 0; Index < SmmCommRegionTable->NumberOfEntries; Index++) {
    if (SmmCommMemRegion->Type == EfiConventionalMemory) {
      Size = EFI_PAGES_TO_SIZE ((UINTN) SmmCommMemRegion->NumberOfPages);
      if (Size >= (DataSize + OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data) + sizeof (SMM_PASSWORD_COMMUNICATE_HEADER))) {
        break;
      }
    }
    SmmCommMemRegion = (EFI_MEMORY_DESCRIPTOR *) ((UINT8 *) SmmCommMemRegion + SmmCommRegionTable->DescriptorSize);
  }
  ASSERT (Index < SmmCommRegionTable->NumberOfEntries);

  Buffer = (VOID*)(UINTN)SmmCommMemRegion->PhysicalStart;
  ASSERT (Buffer != NULL);
  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) Buffer;
  CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gUserAuthenticationGuid);
  SmmCommunicateHeader->MessageLength = DataSize + sizeof (SMM_PASSWORD_COMMUNICATE_HEADER);

  SmmPasswordFunctionHeader = (SMM_PASSWORD_COMMUNICATE_HEADER *) SmmCommunicateHeader->Data;
  ZeroMem (SmmPasswordFunctionHeader, DataSize + sizeof (SMM_PASSWORD_COMMUNICATE_HEADER));
  SmmPasswordFunctionHeader->Function = Function;
  if (DataPtr != NULL) {
    *DataPtr = SmmPasswordFunctionHeader + 1;
  }

  return Buffer;
}

/**
  Send the data in communicate buffer to SMM.

  @param[in]   Buffer                 Points to the data in the communicate buffer.
  @param[in]   DataSize               The data size to send to SMM.

  @retval      EFI_SUCCESS            Success is returned from the function in SMM.
  @retval      Others                 Failure is returned from the function in SMM.

**/
EFI_STATUS
UserPasswordUiLibSendCommunicateBuffer (
  IN      VOID                              *Buffer,
  IN      UINTN                             DataSize
  )
{
  EFI_STATUS                                Status;
  UINTN                                     CommSize;
  EFI_SMM_COMMUNICATE_HEADER                *SmmCommunicateHeader;
  SMM_PASSWORD_COMMUNICATE_HEADER           *SmmPasswordFunctionHeader;
  EFI_SMM_COMMUNICATION_PROTOCOL            *SmmCommunication;

  //
  // Locates SMM Communication protocol.
  //
  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
  ASSERT_EFI_ERROR (Status);

  CommSize = DataSize + OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data) + sizeof (SMM_PASSWORD_COMMUNICATE_HEADER);

  Status = SmmCommunication->Communicate (SmmCommunication, Buffer, &CommSize);
  ASSERT_EFI_ERROR (Status);

  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) Buffer;
  SmmPasswordFunctionHeader = (SMM_PASSWORD_COMMUNICATE_HEADER *)SmmCommunicateHeader->Data;
  return SmmPasswordFunctionHeader->ReturnStatus;
}

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
  )
{
  VOID                                      *Buffer;
  SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY    *SetVerifyPolicy;

  Buffer = UserPasswordUiLibInitCommunicateBuffer (
             (VOID**)&SetVerifyPolicy,
             sizeof(*SetVerifyPolicy),
             SMM_PASSWORD_FUNCTION_SET_VERIFY_POLICY
             );
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SetVerifyPolicy->NeedReVerify = NeedReVerify;

  return UserPasswordUiLibSendCommunicateBuffer (Buffer, sizeof(*SetVerifyPolicy));
}

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
  Get password verification policy.

  @param[out] VerifyPolicy          Verification policy.

  @retval EFI_SUCCESS               Get verification policy successfully.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to get verification policy.
**/
EFI_STATUS
GetPasswordVerificationPolicy (
  OUT SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY    *VerifyPolicy
  )
{
  EFI_STATUS                                    Status;
  VOID                                          *Buffer;
  SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY        *TempVerifyPolicy;

  Buffer = UserPasswordUiLibInitCommunicateBuffer (
             (VOID**)&TempVerifyPolicy,
             sizeof(*TempVerifyPolicy),
             SMM_PASSWORD_FUNCTION_GET_VERIFY_POLICY
             );
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = UserPasswordUiLibSendCommunicateBuffer (Buffer, sizeof(*TempVerifyPolicy));
  if (!EFI_ERROR (Status)) {
    CopyMem (VerifyPolicy, TempVerifyPolicy, sizeof (SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY));
  }

  return Status;
}

/**
  Return if the password was verified.

  @retval TRUE      The password was verified.
  @retval FALSE     The password was not verified.
**/
BOOLEAN
WasPasswordVerified (
  VOID
  )
{
  EFI_STATUS                                Status;
  VOID                                      *Buffer;

  Buffer = UserPasswordUiLibInitCommunicateBuffer (
             NULL,
             0,
             SMM_PASSWORD_FUNCTION_WAS_PASSWORD_VERIFIED
             );
  if (Buffer == NULL) {
    return FALSE;
  }

  Status = UserPasswordUiLibSendCommunicateBuffer (Buffer, 0);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
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
  EFI_STATUS                                Status;
  CHAR16                                    UserInputPw[PASSWORD_MAX_SIZE];
  CHAR16                                    *PopUpString;
  SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY    VerifyPolicy;

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
  Do password authentication.

  @retval EFI_SUCCESS               Password authentication pass.
**/
EFI_STATUS
EFIAPI
UiDoPasswordAuthentication (
  VOID
  )
{
  BOOLEAN   PasswordSet;

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
}

