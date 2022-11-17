/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/UbaIioPortBifurcationInitLib.h>

EFI_STATUS
IioPortBifurcationInit (
  IN IIO_GLOBALS *IioGlobalData
)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  IIO_PORT_BIFURCATION_INIT_TABLE       IioPortBifurcationInitTable;
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

  TableSize = sizeof(IioPortBifurcationInitTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gIioPortBifurcationInitDataGuid,
                                &IioPortBifurcationInitTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioPortBifurcationInitTable.Signature == IIO_PORT_BIFURCATION_INIT_SIGNATURE);
  ASSERT (IioPortBifurcationInitTable.Version == IIO_PORT_BIFURCATION_INIT_VERSION);

  IioPortBifurcationInitTable.CallUpdate(IioGlobalData);

  return Status;
}
