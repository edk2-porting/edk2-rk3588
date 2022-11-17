/** @file
  This is header file for SA PCIE Root Complex initialization.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

EFI_STATUS
PegInitBeforeEndOfDxe (
  VOID
  );

/**
  This function performs SA registers Saving/Restoring in EndOfDxe callback

  @retval EFI_SUCCESS     - Save/restore has done
  @retval EFI_UNSUPPORTED - Save/restore not done successfully
**/
EFI_STATUS
SaSaveRestore (
  VOID
  );
