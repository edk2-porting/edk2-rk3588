/** @file
  Espi policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _ESPI_CONFIG_H_
#define _ESPI_CONFIG_H_

#define ESPI_CONFIG_REVISION 2
extern EFI_GUID gEspiConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to ESPI.

  <b>Revision 1</b>:
  - Initial revision
  <b>Revision 2</b>:
  - Added LockLinkConfiguration field to config block
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
  /**
    Master HOST_C10 (Virtual Wire) to Slave Enable (VWHC10OE)
    <b>0b: Disable HOST_C10 reporting (HOST_C10 indication from PMC is ignored)</b>
    1b: Enable HOST_C10 reporting to Slave via eSPI Virtual Wire (upon receiving a HOST_C10 indication from PMC)
  **/
  UINT32    HostC10ReportEnable   :  1;
  /**
    eSPI Link Configuration Lock (SBLCL)
    If set to TRUE then communication through SET_CONFIG/GET_CONFIG
    to eSPI slaves addresses from range 0x0 - 0x7FF
    <b>1: TRUE</b>, 0: FALSE
  **/
  UINT32    LockLinkConfiguration :  1;
  /**
   Hardware Autonomous Enable (HAE)
   If set to TRUE, then the IP may request a PG whenever it is idle
  **/
  UINT32    EspiPmHAE             :  1;
  UINT32    RsvdBits              : 27;     ///< Reserved bits
} PCH_ESPI_CONFIG;

#pragma pack (pop)

#endif // _ESPI_CONFIG_H_

