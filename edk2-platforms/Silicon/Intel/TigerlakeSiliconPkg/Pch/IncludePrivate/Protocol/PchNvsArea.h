/** @file
  This file defines the PCH NVS Area Protocol.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_NVS_AREA_H_
#define _PCH_NVS_AREA_H_

//
// PCH NVS Area definition
//
#include <Pch/IncludePrivate/PchNvsAreaDef.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchNvsAreaProtocolGuid;

/**
  This protocol is used to sync PCH information from POST to runtime ASL.
  This protocol exposes the pointer of PCH NVS Area only. Please refer to
  ASL definition for PCH NVS AREA.
**/
typedef struct {
  PCH_NVS_AREA                          *Area;
} PCH_NVS_AREA_PROTOCOL;

#endif // _PCH_NVS_AREA_H_
