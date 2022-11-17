/** @file
  UbaConfigDatabase Peim head file.

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_CONFIG_DATABASE_PEIM_H_
#define _UBA_CONFIG_DATABASE_PEIM_H_

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Guid/UbaCfgHob.h>

typedef struct _UBA_PEIM_PRIVATE_DATA {
  UINT32                          Signature;
  UINT32                          Version;

  UINTN                           ConfigDataCount;      //for AllConfigDataSize
  UINTN                           HandleCount;
  UBA_BOARD_NODE                  *CurrentSku;
  UINTN                           ThisAddress;

  UBA_CONFIG_DATABASE_PPI         UbaCfgDbPpi;
  EFI_PEI_PPI_DESCRIPTOR          UbaPeimPpiList;
} UBA_PEIM_PRIVATE_DATA;

#define PRIVATE_DATA_FROM_PPI(p)    CR(p, UBA_PEIM_PRIVATE_DATA, UbaCfgDbPpi, UBA_BOARD_SIGNATURE)

#endif // _UBA_CONFIG_DATABASE_PEIM_H_
