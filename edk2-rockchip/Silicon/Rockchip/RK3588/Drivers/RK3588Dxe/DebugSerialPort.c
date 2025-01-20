/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "DebugSerialPort.h"

VOID
EFIAPI
ApplyDebugSerialPortVariables (
  VOID
  )
{
  /* nothing to do here */
}

VOID
EFIAPI
SetupDebugSerialPortVariables (
  VOID
  )
{
  UINTN       Size;
  UINT64      Var64;
  EFI_STATUS  Status;

  Size = sizeof (UINT64);

  Status = gRT->GetVariable (
                  L"DebugSerialPortBaudRate",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var64
                  );
  if (EFI_ERROR (Status)) {
    Var64  = DEBUG_SERIAL_PORT_BAUD_RATE_DEFAULT;
    Status = gRT->SetVariable (
                    L"DebugSerialPortBaudRate",
                    &gRK3588DxeFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    Size,
                    &Var64
                    );
    ASSERT_EFI_ERROR (Status);
  }
}
