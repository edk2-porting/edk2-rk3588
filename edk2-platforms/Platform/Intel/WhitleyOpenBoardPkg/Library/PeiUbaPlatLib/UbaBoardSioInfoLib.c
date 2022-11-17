/** @file

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

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
#include <Library/UbaBoardSioInfoLib.h>

EFI_STATUS
PlatformGetBoardSioInfo (
  OUT   PEI_BOARD_SIO_INFO   *BoardSioInfoData
)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
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

  TableSize = sizeof(PEI_BOARD_SIO_INFO);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformBoardSioInfoDataGuid,
                                BoardSioInfoData,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (BoardSioInfoData -> Signature == BOARD_SIO_INFO_DATA_SIGNATURE);
  ASSERT (BoardSioInfoData -> Version == BOARD_SIO_INFO_DATA_VERSION);

  return Status;
}
