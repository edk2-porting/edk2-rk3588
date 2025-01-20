/** @file
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "PciExpress30.h"

VOID
EFIAPI
ApplyPcie30Variables (
  VOID
  )
{
  /* nothing to do yet */
}

VOID
EFIAPI
SetupPcie30Variables (
  VOID
  )
{
  UINTN   Size;
  UINT32  Var32;
  UINT8   Var8;

  EFI_STATUS  Status;

  Size = sizeof (UINT32);

  Status = gRT->GetVariable (
                  L"Pcie30State",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status) || !FixedPcdGetBool (PcdPcie30Supported)) {
    Status = PcdSet32S (PcdPcie30State, FixedPcdGet32 (PcdPcie30Supported));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT8);

  Status = gRT->GetVariable (
                  L"Pcie30PhyMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var8
                  );
  if (EFI_ERROR (Status) || !FixedPcdGetBool (PcdPcie30PhyModeSwitchable)) {
    Status = PcdSet8S (PcdPcie30PhyMode, FixedPcdGet8 (PcdPcie30PhyModeDefault));
    ASSERT_EFI_ERROR (Status);
  }
}
