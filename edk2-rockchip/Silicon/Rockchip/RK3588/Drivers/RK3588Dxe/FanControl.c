/** @file
 *
 *  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include <Library/RockchipPlatformLib.h>
#include "RK3588DxeFormSetGuid.h"
#include "FanControl.h"

VOID
EFIAPI
ApplyCoolingFanVariables (
  VOID
  )
{
  UINT32  Var32 = PcdGet32 (PcdCoolingFanState);

  if (Var32 == COOLING_FAN_STATE_ENABLED) {
    Var32 = PcdGet32 (PcdCoolingFanSpeed);
    PwmFanIoSetup ();
    PwmFanSetSpeed (Var32);
  }
}

VOID
EFIAPI
SetupCoolingFanVariables (
  VOID
  )
{
  UINTN       Size;
  UINT32      Var32;
  EFI_STATUS  Status;

  Size = sizeof (UINT32);

  Status = gRT->GetVariable (
                  L"CoolingFanState",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCoolingFanState, 1);
    ASSERT_EFI_ERROR (Status);
  }

  Status = gRT->GetVariable (
                  L"CoolingFanSpeed",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCoolingFanSpeed, FAN_PERCENTAGE_DEFAULT);
    ASSERT_EFI_ERROR (Status);
  }
}
