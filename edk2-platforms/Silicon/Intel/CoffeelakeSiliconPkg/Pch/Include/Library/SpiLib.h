/** @file
  Header file for Spi Library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SPI_LIB_H_
#define _SPI_LIB_H_

/**
  This function Initial SPI services

  @retval EFI_STATUS  Results of the installation of the SPI services
**/
EFI_STATUS
EFIAPI
SpiServiceInit (
  VOID
  );

#endif
