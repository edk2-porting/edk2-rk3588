/** @file
  UBA System Board Info Library Header File.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_SYSTEM_BOARD_INFO_LIB_H
#define _UBA_SYSTEM_BOARD_INFO_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <SystemBoard.h>

#define SYSTEM_SYSTEM_BOARD_INFO_SIGNATURE  SIGNATURE_32 ('I', 'N', 'F', 'O')
#define SYSTEM_SYSTEM_BOARD_INFO_VERSION    01

typedef DXE_SYSTEM_BOARD_INFO *(*SYSTEM_BOARD_INFO_CALLBACK) ();

typedef struct
{
  UINT32                                    Signature;
  UINT32                                    Version;
  SYSTEM_BOARD_INFO_CALLBACK                CallUpdate;
} SYSTEM_BOARD_INFO_DATA;

EFI_STATUS
GetSystemBoardInfo (
  IN OUT   DXE_SYSTEM_BOARD_INFO       **SystemboardInfoTableBuffer
  );

extern EFI_GUID gSystemBoardInfoConfigDataGuid;

#endif  //_UBA_SYSTEM_BOARD_INFO_LIB_H
