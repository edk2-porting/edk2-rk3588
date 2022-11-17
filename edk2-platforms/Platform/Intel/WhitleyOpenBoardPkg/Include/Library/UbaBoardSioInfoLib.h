/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_BOARD_SIO_INFO_H_
#define _UBA_BOARD_SIO_INFO_H_

#include <SioRegs.h>

#define BOARD_SIO_INFO_DATA_SIGNATURE  SIGNATURE_32 ('P', 'S', 'I', 'O')
#define BOARD_SIO_INFO_DATA_VERSION    01

// {32C1F731-C2CD-4325-888B-60A0C3DEBB30}
#define   PLATFORM_BOARD_SIO_INFO_DATA_GUID \
{ 0x32c1f731, 0xc2cd, 0x4325, { 0x88, 0x8b, 0x60, 0xa0, 0xc3, 0xde, 0xbb, 0x30 } }

//
// board sio information table
//
typedef struct _PEI_BOARD_SIO_INFO{
  //
  // Header information
  //
  INT32                    Signature;
  INT32                    Version;
  //
  // SIO initialization table
  //
  UINT8                   SioIndexPort;               // SIO Index Port value
  UINT8                   SioDataPort;                // SIO Data Port value
  SIO_INDEX_DATA          *mSioInitTable;             // SIO init table
  UINT8                   NumSioItems;                // Number of items in the SIO init table.
} PEI_BOARD_SIO_INFO;

EFI_STATUS
PlatformGetBoardSioInfo (
  OUT   PEI_BOARD_SIO_INFO   *BoardSioInfoData
);

STATIC  EFI_GUID gPlatformBoardSioInfoDataGuid = PLATFORM_BOARD_SIO_INFO_DATA_GUID;

#endif

