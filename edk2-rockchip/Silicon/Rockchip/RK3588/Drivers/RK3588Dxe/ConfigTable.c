/** @file
 *
 *  Copyright (c) 2020, Jeremy Linton
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "ConfigTable.h"

VOID
EFIAPI
ApplyConfigTableVariables (
  VOID
  )
{
  /* nothing to do here, the PCDs are read by AcpiPlatformDxe and FdtPlatformDxe */
}

VOID
EFIAPI
SetupConfigTableVariables (
  VOID
  )
{
  UINTN      Size;
  UINT32     Var32;
  UINT8      Var8;
  EFI_STATUS Status;

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"ConfigTableMode",
                            &gRK3588DxeFormSetGuid,
                            NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdConfigTableMode, PcdGet32 (PcdConfigTableMode));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"AcpiUsb2State",
                            &gRK3588DxeFormSetGuid,
                            NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdAcpiUsb2State, FixedPcdGet32 (PcdAcpiUsb2StateDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT8);
  Status = gRT->GetVariable (L"FdtSupportOverrides",
                            &gRK3588DxeFormSetGuid,
                            NULL, &Size, &Var8);
  if (EFI_ERROR (Status)) {
    Status = PcdSet8S (PcdFdtSupportOverrides, PcdGet8 (PcdFdtSupportOverrides));
    ASSERT_EFI_ERROR (Status);
  }
}
