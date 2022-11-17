/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MULTI_BOARD_INIT_SUPPORT_LIB_H_
#define _MULTI_BOARD_INIT_SUPPORT_LIB_H_

#include <Library/BoardInitLib.h>

/**
  This board service detects the board type.

  @retval EFI_SUCCESS   The board was detected successfully.
  @retval EFI_NOT_FOUND The board could not be detected.

**/
typedef
EFI_STATUS
(EFIAPI *BOARD_DETECT) (
  VOID
  );

/**
  This board service performs board-specific initialization.

  @retval EFI_SUCCESS   Board-specific initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.

**/
typedef
EFI_STATUS
(EFIAPI *BOARD_INIT) (
  VOID
  );

/**
  This board service detects the boot mode.

  @retval EFI_BOOT_MODE The boot mode.
  @retval EFI_NOT_READY The board has not been detected yet.

**/
typedef
EFI_BOOT_MODE
(EFIAPI *BOARD_BOOT_MODE_DETECT) (
  VOID
  );

typedef struct {
  BOARD_DETECT  BoardDetect;
} BOARD_DETECT_FUNC;

typedef struct {
  BOARD_INIT              BoardDebugInit;
  BOARD_BOOT_MODE_DETECT  BoardBootModeDetect;
  BOARD_INIT              BoardInitBeforeMemoryInit;
  BOARD_INIT              BoardInitAfterMemoryInit;
  BOARD_INIT              BoardInitBeforeTempRamExit;
  BOARD_INIT              BoardInitAfterTempRamExit;
} BOARD_PRE_MEM_INIT_FUNC;

typedef struct {
  BOARD_INIT              BoardInitBeforeSiliconInit;
  BOARD_INIT              BoardInitAfterSiliconInit;
} BOARD_POST_MEM_INIT_FUNC;

typedef struct {
  BOARD_INIT              BoardInitAfterPciEnumeration;
  BOARD_INIT              BoardInitReadyToBoot;
  BOARD_INIT              BoardInitEndOfFirmware;
} BOARD_NOTIFICATION_INIT_FUNC;

/**
  Registers the given function for callback during board detection.

  When this function is called the given function pointer is added to an internal list. When board detection is
  performed within the BoardDetect() API, the function pointers in the list will be invoked until a board
  detection function reports it has successfully detected the board.

  @param[in]    BoardDetect             A pointer to a function of type BOARD_DETECT_FUNC that is called during
                                        board detection.

  @retval       EFI_SUCCESS             The function was successfully registered.
  @retval       EFI_INVALID_PARAMETER   The function pointer given is NULL.
  @retval       EFI_OUT_OF_RESOURCES    Insufficient memory resources exist to add a new board detection callback.

**/
EFI_STATUS
EFIAPI
RegisterBoardDetect (
  IN BOARD_DETECT_FUNC  *BoardDetect
  );

/**
  Registers the given set of board functions for callback to perform pre-memory board initialization tasks.

  When this function is called, the given structure of function pointers are stored for future invocation. When
  board pre-memory initialization tasks are required, the corresponding pre-memory function in this structure
  will be called. Typically, RegisterBoardPreMemInit() is called during board detection with the successfully
  detected board providing its set of board-specific pre-memory initialization functions.

  @param[in]    BoardPreMemInit         A pointer to a structure of function pointers described in the type
                                        BOARD_PRE_MEM_INIT_FUNC.

  @retval       EFI_SUCCESS             The board pre-memory functions were successfully registered.
  @retval       EFI_INVALID_PARAMETER   The pointer given is NULL.
  @retval       EFI_OUT_OF_RESOURCES    Insufficient memory resources exist to register the callback functions.

**/
EFI_STATUS
EFIAPI
RegisterBoardPreMemInit (
  IN BOARD_PRE_MEM_INIT_FUNC  *BoardPreMemInit
  );

/**
  Registers the given set of board functions for callback to perform post-memory board initialization tasks.

  When this function is called, the given structure of function pointers are stored for future invocation. When
  board post-memory initialization tasks are required, the corresponding post-memory function in this structure
  will be called. Typically, RegisterBoardPostMemInit() is called during board detection with the successfuly
  detected board providing its set of board-specific post-memory initialization functions.

  @param[in]    BoardPostMemInit        A pointer to a structure of function pointers described in the type
                                        BOARD_POST_MEM_INIT_FUNC.

  @retval       EFI_SUCCESS             The board post-memory functions were successfully registered.
  @retval       EFI_INVALID_PARAMETER   The pointer given is NULL.
  @retval       EFI_OUT_OF_RESOURCES    Insufficient memory resources exist to register the callback functions.

**/
EFI_STATUS
EFIAPI
RegisterBoardPostMemInit (
  IN BOARD_POST_MEM_INIT_FUNC  *BoardPostMemInit
  );

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
  );

#endif
