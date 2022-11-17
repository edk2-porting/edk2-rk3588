/** @file
  Header file for a library to install StallPpi.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _STALL_PPI_LIB_H_
#define _STALL_PPI_LIB_H_

/**
  This function is to install StallPpi

  @retval  EFI_SUCCESS if Ppi is installed successfully.
**/
EFI_STATUS
EFIAPI
InstallStallPpi(
  VOID
  );
#endif //_STALL_PPI_LIB_H_
