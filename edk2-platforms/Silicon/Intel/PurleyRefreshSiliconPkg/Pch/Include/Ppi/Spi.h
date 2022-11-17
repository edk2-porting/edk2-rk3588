/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_SPI_PPI_H_
#define _PCH_SPI_PPI_H_

#include <Protocol/Spi.h>

//
// Extern the GUID for PPI users.
//
extern EFI_GUID           gPeiSpiPpiGuid;

/**
  Reuse the PCH_SPI_PROTOCOL definitions
  This is possible becaues the PPI implementation does not rely on a PeiService pointer,
  as it uses EDKII Glue Lib to do IO accesses
**/
typedef EFI_SPI_PROTOCOL PCH_SPI_PPI;

#endif
