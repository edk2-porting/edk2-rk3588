/** @file

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/BoardInitLib.h>
#include <PeiPlatformHookLib.h>
#include "TigerlakeURvpInit.h"
#include "GpioTableTigerlakeUDdr4Rvp.h"
#include <Library/ConfigBlockLib.h>

/**
  GPIO init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardGpioInit(
  IN UINT16 BoardId
  )
{
  //
  // GPIO Table Init.
  //
  switch (BoardId) {

    case BoardIdTglUDdr4:
      PcdSet32S (PcdBoardGpioTable, (UINTN) mGpioTableTglUDdr4);
      PcdSet16S (PcdBoardGpioTableSize, mGpioTableTglUDdr4Size);
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

/**
  Touch panel GPIO init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
TouchPanelGpioInit (
  IN UINT16 BoardId
  )
{

  return EFI_SUCCESS;
}

/**
  Misc. init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardMiscInit (
  IN UINT16 BoardId
  )
{

  return EFI_SUCCESS;
}

/**
  Security GPIO init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardSecurityInit (
  IN UINT16 BoardId
  )
{
  return EFI_SUCCESS;
}

/**
  Board configuration initialization in the post-memory boot phase.
**/
VOID
BoardConfigInit (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT16      BoardId;

  BoardId = BoardIdTglUDdr4;

  Status = BoardGpioInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = TouchPanelGpioInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = BoardMiscInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = BoardSecurityInit (BoardId);
  ASSERT_EFI_ERROR (Status);
}


/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
TigerlakeURvpBoardInitBeforeSiliconInit(
  VOID
  )
{
  EFI_STATUS      Status;

  DEBUG ((DEBUG_INFO, "Board Init before Silicon Init\n"));

  BoardConfigInit ();
  //
  // Configure GPIO and SIO
  //
  Status = BoardInit ();
  ASSERT_EFI_ERROR (Status);

  //
  // Initializing Platform Specific Programming
  //
  Status = PlatformSpecificInit ();
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}
