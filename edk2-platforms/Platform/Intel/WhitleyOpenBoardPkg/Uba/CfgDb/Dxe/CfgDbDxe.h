/** @file
  UbaConfigDatabase Dxe Driver head file.

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_CONFIG_DATABASE_DXE_H_
#define _UBA_CONFIG_DATABASE_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <Protocol/UbaCfgDb.h>
#include <Guid/UbaCfgHob.h>

typedef struct _UBA_DXE_PRIVATE_DATA {
  UINT32                          Signature;
  UINT32                          Version;

  UINTN                           ConfigDataCount;              //for AllConfigDataSize
  UINTN                           HandleCount;
  UBA_BOARD_NODE                  *CurrentSku;

  UBA_CONFIG_DATABASE_PROTOCOL   UbaCfgDbProtocol;
} UBA_DXE_PRIVATE_DATA;

#define PRIVATE_DATA_FROM_PROTOCOL(p)    CR(p, UBA_DXE_PRIVATE_DATA, UbaCfgDbProtocol, UBA_BOARD_SIGNATURE)

#endif // _UBA_CONFIG_DATABASE_DXE_H_

