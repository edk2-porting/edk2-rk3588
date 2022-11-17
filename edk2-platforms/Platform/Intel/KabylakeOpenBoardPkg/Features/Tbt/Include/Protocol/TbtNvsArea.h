/** @file
  This file defines the TBT NVS Area Protocol.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _TBT_NVS_AREA_H_
#define _TBT_NVS_AREA_H_

//
// Platform NVS Area definition
//
#include <Acpi\TbtNvsAreaDef.h>

//
// Includes
//
#define TBT_NVS_DEVICE_ENABLE 1
#define TBT_NVS_DEVICE_DISABLE 0

//
// Forward reference for pure ANSI compatibility
//
typedef struct _TBT_NVS_AREA_PROTOCOL TBT_NVS_AREA_PROTOCOL;

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gTbtNvsAreaProtocolGuid;

#define TBT_NVS_AREA_REVISION_1       1

//
// Platform NVS Area Protocol
//
typedef struct _TBT_NVS_AREA_PROTOCOL {
  TBT_NVS_AREA     *Area;
} TBT_NVS_AREA_PROTOCOL;

#endif // _TBT_NVS_AREA_H_
