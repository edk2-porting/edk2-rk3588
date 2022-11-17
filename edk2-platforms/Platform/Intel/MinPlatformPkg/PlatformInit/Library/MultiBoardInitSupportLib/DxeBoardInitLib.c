/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  A hook for board-specific initialization after PCI enumeration.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitAfterPciEnumeration (
  VOID
  )
{
  BOARD_NOTIFICATION_INIT_FUNC    *BoardNotificationInit;
  EFI_STATUS                      Status;

  Status = gBS->LocateProtocol (
                  &gBoardNotificationInitGuid,
                  NULL,
                  (VOID **)&BoardNotificationInit
                  );
  if (!EFI_ERROR(Status)) {
    if (BoardNotificationInit->BoardInitAfterPciEnumeration != NULL) {
      return BoardNotificationInit->BoardInitAfterPciEnumeration ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ReadyToBoot event.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitReadyToBoot (
  VOID
  )
{
  BOARD_NOTIFICATION_INIT_FUNC    *BoardNotificationInit;
  EFI_STATUS                      Status;

  Status = gBS->LocateProtocol (
                  &gBoardNotificationInitGuid,
                  NULL,
                  (VOID **)&BoardNotificationInit
                  );
  if (!EFI_ERROR(Status)) {
    if (BoardNotificationInit->BoardInitReadyToBoot != NULL) {
      return BoardNotificationInit->BoardInitReadyToBoot ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ExitBootServices event.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitEndOfFirmware (
  VOID
  )
{
  BOARD_NOTIFICATION_INIT_FUNC    *BoardNotificationInit;
  EFI_STATUS                      Status;

  Status = gBS->LocateProtocol (
                  &gBoardNotificationInitGuid,
                  NULL,
                  (VOID **)&BoardNotificationInit
                  );
  if (!EFI_ERROR(Status)) {
    if (BoardNotificationInit->BoardInitEndOfFirmware != NULL) {
      return BoardNotificationInit->BoardInitEndOfFirmware ();
    }
  }
  return EFI_SUCCESS;
}
