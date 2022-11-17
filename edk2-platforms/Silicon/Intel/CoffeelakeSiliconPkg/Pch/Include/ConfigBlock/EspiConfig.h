/** @file
  Espi policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ESPI_CONFIG_H_
#define _ESPI_CONFIG_H_

#define ESPI_CONFIG_REVISION 1
extern EFI_GUID gEspiConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to ESPI.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    LPC (eSPI) Memory Range Decode Enable. When TRUE, then the range
    specified in PCLGMR[31:16] is enabled for decoding to LPC (eSPI).
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT32    LgmrEnable            :  1;
  /**
    eSPI Master and Slave BME settings.
    When TRUE, then the BME bit enabled in eSPI Master and Slave.
    0: FALSE, <b>1: TRUE </b>
  **/
  UINT32    BmeMasterSlaveEnabled :  1;
  UINT32    RsvdBits              : 30;     ///< Reserved bits
} PCH_ESPI_CONFIG;

#pragma pack (pop)

#endif // _ESPI_CONFIG_H_

