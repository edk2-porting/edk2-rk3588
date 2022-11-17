/** @file
  DXE Variable Write Lib

  This library provides phase agnostic access to the UEFI Variable Services.
  This is done by implementing a wrapper on top of the phase specific mechanism
  for writing to UEFI variables. For example, the DXE implementation accesses
  the UEFI Runtime Services Table, and the SMM implementation uses
  EFI_SMM_VARIABLE_PROTOCOL.

  Using this library allows code to be written in a generic manner that can be
  used in DXE or SMM without modification.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Guid/EventGroup.h>
#include <Protocol/VariableLock.h>

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

STATIC EDKII_VARIABLE_LOCK_PROTOCOL  *mVariableWriteLibVariableLock = NULL;

/**
  Sets the value of a variable.

  @param[in]  VariableName       A Null-terminated string that is the name of the vendor's variable.
                                 Each VariableName is unique for each VendorGuid. VariableName must
                                 contain 1 or more characters. If VariableName is an empty string,
                                 then EFI_INVALID_PARAMETER is returned.
  @param[in]  VendorGuid         A unique identifier for the vendor.
  @param[in]  Attributes         Attributes bitmask to set for the variable.
  @param[in]  DataSize           The size in bytes of the Data buffer. Unless the EFI_VARIABLE_APPEND_WRITE or
                                 EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set, a size of zero
                                 causes the variable to be deleted. When the EFI_VARIABLE_APPEND_WRITE attribute is
                                 set, then a SetVariable() call with a DataSize of zero will not cause any change to
                                 the variable value (the timestamp associated with the variable may be updated however
                                 even if no new data value is provided,see the description of the
                                 EFI_VARIABLE_AUTHENTICATION_2 descriptor below. In this case the DataSize will not
                                 be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated).
  @param[in]  Data               The contents for the variable.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                 but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_UNSUPPORTED        This function is not implemented by this instance of the LibraryClass

  @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found.

**/
EFI_STATUS
EFIAPI
VarLibSetVariable (
  IN  CHAR16                        *VariableName,
  IN  EFI_GUID                      *VendorGuid,
  IN  UINT32                        Attributes,
  IN  UINTN                         DataSize,
  IN  VOID                          *Data
  )
{
  EFI_STATUS    Status = EFI_UNSUPPORTED;

  if (gRT != NULL) {
    Status = gRT->SetVariable (
                    VariableName,
                    VendorGuid,
                    Attributes,
                    DataSize,
                    Data
                    );
  }
  return Status;
}

/**
  Returns information about the EFI variables.

  @param[in]   Attributes                   Attributes bitmask to specify the type of variables on
                                            which to return information.
  @param[out]  MaximumVariableStorageSize   On output the maximum size of the storage space
                                            available for the EFI variables associated with the
                                            attributes specified.
  @param[out]  RemainingVariableStorageSize Returns the remaining size of the storage space
                                            available for the EFI variables associated with the
                                            attributes specified.
  @param[out]  MaximumVariableSize          Returns the maximum size of the individual EFI
                                            variables associated with the attributes specified.

  @retval EFI_SUCCESS                  Valid answer returned.
  @retval EFI_INVALID_PARAMETER        An invalid combination of attribute bits was supplied
  @retval EFI_UNSUPPORTED              The attribute is not supported on this platform, and the
                                       MaximumVariableStorageSize,
                                       RemainingVariableStorageSize, MaximumVariableSize
                                       are undefined.
**/
EFI_STATUS
EFIAPI
VarLibQueryVariableInfo (
  IN  UINT32                        Attributes,
  OUT UINT64                        *MaximumVariableStorageSize,
  OUT UINT64                        *RemainingVariableStorageSize,
  OUT UINT64                        *MaximumVariableSize
  )
{
  EFI_STATUS    Status = EFI_UNSUPPORTED;

  if (gRT != NULL) {
    Status = gRT->QueryVariableInfo (
                    Attributes,
                    MaximumVariableStorageSize,
                    RemainingVariableStorageSize,
                    MaximumVariableSize
                    );
  }
  return Status;
}

/**
  Indicates if the VarLibVariableRequestToLock() API is supported by the current
  VariableWriteLib implementation. At time of writting, this API is not
  available in SMM or after ExitBootServices.

  @retval TRUE                  The VarLibVariableRequestToLock() API is supported
  @retval FALSE                 The VarLibVariableRequestToLock() API is not supported

**/
BOOLEAN
EFIAPI
VarLibIsVariableRequestToLockSupported (
  VOID
  )
{
  if (mVariableWriteLibVariableLock != NULL) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Mark a variable that will become read-only after leaving the DXE phase of execution.
  Write request coming from SMM environment through EFI_SMM_VARIABLE_PROTOCOL is allowed.

  @param[in] This          The EDKII_VARIABLE_LOCK_PROTOCOL instance.
  @param[in] VariableName  A pointer to the variable name that will be made read-only subsequently.
  @param[in] VendorGuid    A pointer to the vendor GUID that will be made read-only subsequently.

  @retval EFI_SUCCESS           The variable specified by the VariableName and the VendorGuid was marked
                                as pending to be read-only.
  @retval EFI_INVALID_PARAMETER VariableName or VendorGuid is NULL.
                                Or VariableName is an empty string.
  @retval EFI_ACCESS_DENIED     EFI_END_OF_DXE_EVENT_GROUP_GUID or EFI_EVENT_GROUP_READY_TO_BOOT has
                                already been signaled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough resource to hold the lock request.
  @retval EFI_UNSUPPORTED       This function is not implemented by this instance of the LibraryClass

**/
EFI_STATUS
EFIAPI
VarLibVariableRequestToLock (
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid
  )
{
  EFI_STATUS    Status = EFI_UNSUPPORTED;

  if (mVariableWriteLibVariableLock != NULL) {
    Status = mVariableWriteLibVariableLock->RequestToLock (
                                              mVariableWriteLibVariableLock,
                                              VariableName,
                                              VendorGuid
                                              );
  }
  return Status;
}

/**
  Exit Boot Services Event notification handler.

  @param[in]  Event     Event whose notification function is being invoked.
  @param[in]  Context   Pointer to the notification function's context.

**/
VOID
EFIAPI
DxeRuntimeVariableWriteLibOnExitBootServices (
  IN  EFI_EVENT                    Event,
  IN  VOID                         *Context
  )
{
  mVariableWriteLibVariableLock = NULL;
}

/**
  The constructor function acquires the Variable Lock Protocol

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The constructor was successful.
  @retval EFI_NOT_FOUND         gEdkiiVariableLockProtocolGuid Protocol
                                interface not found, which technically should
                                not be possible since this protocol is in the
                                LibraryClass DEPEX
  @retval EFI_OUT_OF_RESOURCES  Unable to create new events

**/
EFI_STATUS
EFIAPI
DxeRuntimeVariableWriteLibConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     ExitBootServiceEvent;
  EFI_EVENT     LegacyBootEvent;

  //
  // Locate VariableLockProtocol.
  //
  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&mVariableWriteLibVariableLock);
  ASSERT_EFI_ERROR (Status);

  //
  // Register the event to inform SMM variable that it is at runtime.
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             DxeRuntimeVariableWriteLibOnExitBootServices,
             NULL,
             &gEfiEventExitBootServicesGuid,
             &ExitBootServiceEvent
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Register the event to inform SMM variable that it is at runtime for legacy boot.
  // Reuse OnExitBootServices() here.
  //
  Status = EfiCreateEventLegacyBootEx (
             TPL_NOTIFY,
             DxeRuntimeVariableWriteLibOnExitBootServices,
             NULL,
             &LegacyBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
