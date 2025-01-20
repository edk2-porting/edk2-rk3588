/** @file
 *
 *  Copyright (c) 2020, Jeremy Linton
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "ConfigTable.h"

typedef struct {
  UINT32    Id;
  CHAR16    *SupportedVarString;
} FDT_COMPAT_MODE_VAR;

STATIC FDT_COMPAT_MODE_VAR  mFdtCompatModeVarTable[] = {
  { FDT_COMPAT_MODE_VENDOR,   L"FdtCompatModeVendorSupported"   },
  { FDT_COMPAT_MODE_MAINLINE, L"FdtCompatModeMainlineSupported" },
};

STATIC
BOOLEAN
IsFdtCompatModeSupported (
  IN UINT32  CompatMode
  )
{
  return PlatformGetDtbFileGuid (CompatMode) != NULL;
}

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
  UINTN       Size;
  UINT32      Var32;
  UINT8       Var8;
  EFI_STATUS  Status;
  UINTN       Index;
  UINT32      FirstFdtCompatModeSupported;

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"ConfigTableMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdConfigTableMode, FixedPcdGet32 (PcdConfigTableModeDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"AcpiPcieEcamCompatMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdAcpiPcieEcamCompatMode, FixedPcdGet32 (PcdAcpiPcieEcamCompatModeDefault));
    ASSERT_EFI_ERROR (Status);
  }

  FirstFdtCompatModeSupported = FDT_COMPAT_MODE_UNSUPPORTED;

  for (Index = 0; Index < ARRAY_SIZE (mFdtCompatModeVarTable); Index++) {
    FDT_COMPAT_MODE_VAR  CompatModeVar       = mFdtCompatModeVarTable[Index];
    UINT8                CompatModeSupported = IsFdtCompatModeSupported (CompatModeVar.Id);

    Status = gRT->SetVariable (
                    CompatModeVar.SupportedVarString,
                    &gRK3588DxeFormSetGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (CompatModeSupported),
                    &CompatModeSupported
                    );
    ASSERT_EFI_ERROR (Status);

    if (CompatModeSupported &&
        (FirstFdtCompatModeSupported == FDT_COMPAT_MODE_UNSUPPORTED))
    {
      FirstFdtCompatModeSupported = CompatModeVar.Id;
    }
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"FdtCompatMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status) || !IsFdtCompatModeSupported (Var32)) {
    UINT32  FdtCompatModeDefault = FixedPcdGet32 (PcdFdtCompatModeDefault);
    if (!IsFdtCompatModeSupported (FdtCompatModeDefault)) {
      FdtCompatModeDefault = FirstFdtCompatModeSupported;
    }

    Status = PcdSet32S (PcdFdtCompatMode, FdtCompatModeDefault);
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT8);
  Status = gRT->GetVariable (
                  L"FdtForceGop",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var8
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet8S (PcdFdtForceGop, FixedPcdGet8 (PcdFdtForceGopDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT8);
  Status = gRT->GetVariable (
                  L"FdtSupportOverrides",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var8
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet8S (PcdFdtSupportOverrides, FixedPcdGet8 (PcdFdtSupportOverridesDefault));
    ASSERT_EFI_ERROR (Status);
  }
}
