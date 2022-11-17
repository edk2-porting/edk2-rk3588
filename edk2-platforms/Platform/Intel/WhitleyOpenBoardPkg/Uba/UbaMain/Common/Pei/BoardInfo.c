/** @file
  Install Board Info Data.

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCommonBoardInitLib.h"
#include <Library/UbaBoardSioInfoLib.h>
#include <SioRegs.h>
#include <Platform.h>
#include <Ppi/DynamicSiLibraryPpi.h>

STATIC SIO_INDEX_DATA mSioInitTable[] = {
  //
  // Init GPIO
  //
  {
    PILOTIV_LOGICAL_DEVICE,
    PILOTIV_SIO_GPIO
  },
  {
    PILOTIV_ACTIVATE,
    0x01
  },
  {
    PILOTIV_BASE_ADDRESS_HIGH0,
    (UINT8) ((UINT16) SIO_GPIO_BASE_ADDRESS >> 8)
  },
  {
    PILOTIV_BASE_ADDRESS_LOW0,
    (UINT8) (SIO_GPIO_BASE_ADDRESS & 0x00ff)
  }
};

//
// Platform board sio information structure
//
static PEI_BOARD_SIO_INFO BoardSioInfoData = {
  BOARD_SIO_INFO_DATA_SIGNATURE,
  BOARD_SIO_INFO_DATA_VERSION,
  //
  // SIO Initialization table
  //
  PILOTIV_SIO_INDEX_PORT,                  // SIO Index port
  PILOTIV_SIO_DATA_PORT,                   // SIO Data port
  mSioInitTable,                            // mSioInitTable - contains the settings for initializing the SIO.
  sizeof(mSioInitTable)/sizeof(SIO_INDEX_DATA) // NumSioItems - Number of items in the SIO init table.
};


EFI_STATUS
InstallBoardInfoData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

   Status = UbaConfigPpi->AddData (
                                  UbaConfigPpi,
                                  &gPlatformBoardSioInfoDataGuid,
                                  &BoardSioInfoData,
                                  sizeof(PEI_BOARD_SIO_INFO)
                                  );

  return Status;
}
