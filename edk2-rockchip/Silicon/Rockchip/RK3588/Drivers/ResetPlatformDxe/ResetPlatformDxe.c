/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ResetUtilityLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PlatformSpecificResetHandler.h>

#define ROCKCHIP_BOOT_MODE_REG  0xFD588080 // PMU0_GRF_OS_REG8

#define BOOT_BROM_DOWNLOAD  0xEF08A53C

#define PMU1GRF_SOC_CON1  0xfd58a004

STATIC
VOID
EFIAPI
ResetNotifyHandler (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  GUID     *ResetSubtype;
  BOOLEAN  SoftReset;

  if ((ResetType != EfiResetPlatformSpecific) ||
      (DataSize == 0) ||
      (ResetData == NULL))
  {
    return;
  }

  ResetSubtype = GetResetPlatformSpecificGuid (DataSize, ResetData);

  SoftReset = TRUE;

  if (CompareGuid (ResetSubtype, &gRockchipResetTypeMaskromGuid)) {
    MmioWrite32 (ROCKCHIP_BOOT_MODE_REG, BOOT_BROM_DOWNLOAD);
  } else {
    SoftReset = FALSE;
  }

  //
  // It has been observed that the boot mode register does not persist
  // across reset after BL31 sets reset_width to 0xFFFF.
  // Setting it back to 0 causes a softer reset, which solves the issue.
  //
  if (SoftReset) {
    MmioWrite32 (PMU1GRF_SOC_CON1, 0xFFFF0000);
  }
}

EFI_STATUS
EFIAPI
ResetPlatformDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL  *ResetNotify;

  Status = gBS->LocateProtocol (
                  &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
                  NULL,
                  (VOID **)&ResetNotify
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = ResetNotify->RegisterResetNotify (
                            ResetNotify,
                            ResetNotifyHandler
                            );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}
