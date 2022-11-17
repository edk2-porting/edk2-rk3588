/** @file
  Library to initialize SPI services for future SPI accesses.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SPI_LIB_H_
#define _SPI_LIB_H_

/**
  Initializes SPI for access from future services.

  @retval EFI_SUCCESS         The SPI service was initialized successfully.
  @retval EFI_OUT_OF_RESOUCES Insufficient memory available to allocate structures required for initialization.
  @retval Others              An error occurred initializing SPI services.

**/
EFI_STATUS
EFIAPI
SpiServiceInit (
  VOID
  );

#endif
