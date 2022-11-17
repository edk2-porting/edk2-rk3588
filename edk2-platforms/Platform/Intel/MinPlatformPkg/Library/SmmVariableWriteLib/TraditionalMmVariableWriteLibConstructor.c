/** @file
  Traditional MM Variable Write Lib

  This library provides phase agnostic access to the UEFI Variable Services.
  This is done by implementing a wrapper on top of the phase specific mechanism
  for reading from UEFI variables.

  This is the traditional SMM specific LibraryClass constructor.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Protocol/SmmVariable.h>

#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>

extern EFI_SMM_VARIABLE_PROTOCOL  *mVariableWriteLibSmmVariable;

/**
  The constructor function acquires the EFI SMM Variable Services

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The constructor always returns RETURN_SUCCESS.
  @retval EFI_NOT_FOUND   gEfiSmmVariableProtocolGuid Protocol interface not
                          found, which technically should not be possible since
                          this protocol is in the LibraryClass DEPEX

**/
EFI_STATUS
EFIAPI
TraditionalMmVariableWriteLibConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;

  //
  // Locate SmmVariableProtocol.
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID **) &mVariableWriteLibSmmVariable);
  ASSERT_EFI_ERROR (Status);
  return Status;
}
