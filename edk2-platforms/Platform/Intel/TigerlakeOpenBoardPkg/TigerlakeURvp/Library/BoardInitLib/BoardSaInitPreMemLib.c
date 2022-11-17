/** @file
 Source code for the board SA configuration Pcd init functions in Pre-Memory init phase.


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "TigerlakeURvpInit.h"
#include <Pins/GpioPinsVer2Lp.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>

/**
  MRC configuration init function for PEI pre-memory phase.

  @param[in]  BoardId           An unsigned integer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
SaMiscConfigInit (
  IN UINT16         BoardId
  )
{
  //
  // UserBd
  //
  switch (BoardId) {
    case BoardIdTglUDdr4:
      //
      // Assign UserBd to 5 which is assigned to MrcInputs->BoardType btUser4 for ULT platforms.
      // This is required to skip Memory voltage programming based on GPIO's in MRC
      //
      PcdSet8S (PcdSaMiscUserBd, 5); // MrcBoardType btUser4 for ULT platform
      break;

    default:
      // MiscPeiPreMemConfig.UserBd = 0 by default.
      break;
  }

  return EFI_SUCCESS;
}

/**
  Board Memory Init related configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
MrcConfigInit (
  IN UINT16 BoardId
  )
{

  PcdSet8S (PcdMrcSpdAddressTable0, 0xA0);
  PcdSet8S (PcdMrcSpdAddressTable1, 0xA2);
  PcdSet8S (PcdMrcSpdAddressTable2, 0xA4);
  PcdSet8S (PcdMrcSpdAddressTable3, 0xA6);

  return EFI_SUCCESS;
}

/**
  Board SA related GPIO configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
SaGpioConfigInit (
  IN UINT16 BoardId
  )
{
  return EFI_SUCCESS;
}

/**
  SA Display DDI configuration init function for PEI pre-memory phase.

  @param[in]  BoardId       An unsigned integer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
SaDisplayConfigInit (
  IN UINT16 BoardId
  )
{
  return EFI_SUCCESS;
}

