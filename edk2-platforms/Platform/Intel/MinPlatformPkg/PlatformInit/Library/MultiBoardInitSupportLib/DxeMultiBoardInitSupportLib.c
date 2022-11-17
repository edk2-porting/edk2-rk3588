/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Registers the given set of board functions for callback to perform board initialization tasks.

  When this function is called, the given structure of function pointers are stored for future invocation. When
  board initialization tasks are required, the corresponding functions in this structure will be called.

  @param[in]    BoardNotificationInit   A pointer to a structure of function pointers described in the type
                                        BOARD_NOTIFICATION_INIT_FUNC.

  @retval       EFI_SUCCESS             The board notification functions were successfully registered.
  @retval       EFI_INVALID_PARAMETER   The pointer given is NULL.
  @retval       EFI_OUT_OF_RESOURCES    Insufficient memory resources exist to register the callback functions.

**/
EFI_STATUS
EFIAPI
RegisterBoardNotificationInit (
  IN BOARD_NOTIFICATION_INIT_FUNC  *BoardNotificationInit
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  if (BoardNotificationInit == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gBoardNotificationInitGuid,
                  EFI_NATIVE_INTERFACE,
                  BoardNotificationInit
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
