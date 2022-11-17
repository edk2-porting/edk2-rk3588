/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UbaHsioPtssTableConfigLib.h>

EFI_STATUS
InstallPlatformHsioPtssTable (
  IN          PCH_SETUP                    *PchSetup,
  IN OUT      PCH_POLICY_PPI               *PchPolicy

)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_HSIO_PTSS_CONFIG_TABLE      PlatformHsioPtssConfigTable;
  UINTN                                 TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(PlatformHsioPtssConfigTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformHsioPtssTableGuid,
                                &PlatformHsioPtssConfigTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (PlatformHsioPtssConfigTable.Signature == PLATFORM_HSIO_PTSS_TABLE_SIGNATURE);
  ASSERT (PlatformHsioPtssConfigTable.Version == PLATFORM_HSIO_PTSS_TABLE_VERSION);

  PlatformHsioPtssConfigTable.CallUpdate( PchSetup, PchPolicy );
  return Status;
}
