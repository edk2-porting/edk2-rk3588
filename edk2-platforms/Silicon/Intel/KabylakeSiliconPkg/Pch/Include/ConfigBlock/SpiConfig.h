/** @file
  Spi policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SPI_CONFIG_H_
#define _SPI_CONFIG_H_

#define SPI_CONFIG_REVISION 1
extern EFI_GUID gSpiConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to SPI.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Force to show SPI controller.
    <b>0: FALSE</b>, 1: TRUE
    NOTE: For Windows OS, it MUST BE false. It's optional for other OSs.
  **/
  UINT32    ShowSpiController           :  1;
  UINT32    RsvdBits                    : 31;     ///< Reserved bits
} PCH_SPI_CONFIG;

#pragma pack (pop)

#endif // _SPI_CONFIG_H_
