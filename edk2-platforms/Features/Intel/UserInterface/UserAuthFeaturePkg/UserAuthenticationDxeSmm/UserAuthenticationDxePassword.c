/** @file
  UserAuthentication DXE password wrapper.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "UserAuthenticationDxe.h"

/**
  Initialize the communicate buffer using DataSize and Function.

  @param[out]      DataPtr          Points to the data in the communicate buffer.
  @param[in]       DataSize         The data size to send to SMM.
  @param[in]       Function         The function number to initialize the communicate header.

  @return Communicate buffer.
**/
VOID*
InitCommunicateBuffer (
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
SendCommunicateBuffer (
  IN      VOID                              *Buffer,
  IN      UINTN                             DataSize
  )
{
  EFI_STATUS                                Status;
  UINTN                                     CommSize;
  EFI_SMM_COMMUNICATE_HEADER                *SmmCommunicateHeader;
  SMM_PASSWORD_COMMUNICATE_HEADER           *SmmPasswordFunctionHeader;

  CommSize = DataSize + OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data) + sizeof (SMM_PASSWORD_COMMUNICATE_HEADER);

  Status = mSmmCommunication->Communicate (mSmmCommunication, Buffer, &CommSize);
  ASSERT_EFI_ERROR (Status);

  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) Buffer;
  SmmPasswordFunctionHeader = (SMM_PASSWORD_COMMUNICATE_HEADER *)SmmCommunicateHeader->Data;
  return  SmmPasswordFunctionHeader->ReturnStatus;
}

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
  )
{
  EFI_STATUS                                Status;
  VOID                                      *Buffer;
  SMM_PASSWORD_COMMUNICATE_VERIFY_PASSWORD  *VerifyPassword;

  ASSERT (Password != NULL);

  if (PasswordSize > sizeof(VerifyPassword->Password) * sizeof(CHAR16)) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer = InitCommunicateBuffer (
             (VOID**)&VerifyPassword,
             sizeof(*VerifyPassword),
             SMM_PASSWORD_FUNCTION_VERIFY_PASSWORD
             );
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = UnicodeStrToAsciiStrS (Password, VerifyPassword->Password, sizeof(VerifyPassword->Password));
  if (EFI_ERROR(Status)) {
    goto EXIT;
  }

  Status = SendCommunicateBuffer (Buffer, sizeof(*VerifyPassword));

EXIT:
  ZeroMem (VerifyPassword, sizeof(*VerifyPassword));
  return Status;
}

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
  )
{
  EFI_STATUS                                Status;
  VOID                                      *Buffer;
  SMM_PASSWORD_COMMUNICATE_SET_PASSWORD     *SetPassword;

  if (NewPasswordSize > sizeof(SetPassword->NewPassword) * sizeof(CHAR16)) {
    return EFI_INVALID_PARAMETER;
  }
  if (OldPasswordSize > sizeof(SetPassword->OldPassword) * sizeof(CHAR16)) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer = InitCommunicateBuffer (
             (VOID**)&SetPassword,
             sizeof(*SetPassword),
             SMM_PASSWORD_FUNCTION_SET_PASSWORD
             );
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (NewPassword != NULL) {
    Status = UnicodeStrToAsciiStrS (NewPassword, SetPassword->NewPassword, sizeof(SetPassword->NewPassword));
    if (EFI_ERROR(Status)) {
      goto EXIT;
    }
  } else {
    SetPassword->NewPassword[0] = 0;
  }

  if (OldPassword != NULL) {
    Status = UnicodeStrToAsciiStrS (OldPassword, SetPassword->OldPassword, sizeof(SetPassword->OldPassword));
    if (EFI_ERROR(Status)) {
      goto EXIT;
    }
  } else {
    SetPassword->OldPassword[0] = 0;
  }

  Status = SendCommunicateBuffer (Buffer, sizeof(*SetPassword));

EXIT:
  ZeroMem (SetPassword, sizeof(*SetPassword));
  return Status;
}

/**
  Return if the password is set.

  @retval TRUE      The password is set.
  @retval FALSE     The password is not set.
**/
BOOLEAN
IsPasswordInstalled (
  VOID
  )
{
  EFI_STATUS                                Status;
  VOID                                      *Buffer;

  Buffer = InitCommunicateBuffer (
             NULL,
             0,
             SMM_PASSWORD_FUNCTION_IS_PASSWORD_SET
             );
  if (Buffer == NULL) {
    return FALSE;
  }

  Status = SendCommunicateBuffer (Buffer, 0);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
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
  SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY        *GetVerifyPolicy;

  Buffer = InitCommunicateBuffer (
             (VOID**)&GetVerifyPolicy,
             sizeof(*GetVerifyPolicy),
             SMM_PASSWORD_FUNCTION_GET_VERIFY_POLICY
             );
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = SendCommunicateBuffer (Buffer, sizeof(*GetVerifyPolicy));
  if (!EFI_ERROR (Status)) {
    CopyMem (VerifyPolicy, GetVerifyPolicy, sizeof (SMM_PASSWORD_COMMUNICATE_VERIFY_POLICY));
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

  Buffer = InitCommunicateBuffer (
             NULL,
             0,
             SMM_PASSWORD_FUNCTION_WAS_PASSWORD_VERIFIED
             );
  if (Buffer == NULL) {
    return FALSE;
  }

  Status = SendCommunicateBuffer (Buffer, 0);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}
