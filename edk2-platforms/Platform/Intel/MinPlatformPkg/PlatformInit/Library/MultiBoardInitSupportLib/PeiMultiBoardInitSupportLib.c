/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>

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
  )
{
  EFI_STATUS                 Status;
  EFI_PEI_PPI_DESCRIPTOR     *PpiListBoardDetect;

  if (BoardDetect == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PpiListBoardDetect = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiListBoardDetect == NULL) {
    ASSERT (PpiListBoardDetect != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiListBoardDetect->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListBoardDetect->Guid  = &gBoardDetectGuid;
  PpiListBoardDetect->Ppi   = BoardDetect;

  Status = PeiServicesInstallPpi (PpiListBoardDetect);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

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
  )
{
  EFI_STATUS                 Status;
  EFI_PEI_PPI_DESCRIPTOR     *PpiListBoardInitPreMem;

  if (BoardPreMemInit == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PpiListBoardInitPreMem = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiListBoardInitPreMem == NULL) {
    ASSERT (PpiListBoardInitPreMem != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiListBoardInitPreMem->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListBoardInitPreMem->Guid  = &gBoardPreMemInitGuid;
  PpiListBoardInitPreMem->Ppi   = BoardPreMemInit;

  Status = PeiServicesInstallPpi (PpiListBoardInitPreMem);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

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
  )
{
  EFI_STATUS                 Status;
  EFI_PEI_PPI_DESCRIPTOR     *PpiListBoardInitPostMem;

  if (BoardPostMemInit == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PpiListBoardInitPostMem = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiListBoardInitPostMem == NULL) {
    ASSERT (PpiListBoardInitPostMem != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiListBoardInitPostMem->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListBoardInitPostMem->Guid  = &gBoardPostMemInitGuid;
  PpiListBoardInitPostMem->Ppi   = BoardPostMemInit;

  Status = PeiServicesInstallPpi (PpiListBoardInitPostMem);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
