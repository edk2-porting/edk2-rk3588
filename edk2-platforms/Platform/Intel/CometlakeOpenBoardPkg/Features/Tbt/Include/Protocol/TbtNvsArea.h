/** @file
  This file defines the TBT NVS Area Protocol.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_NVS_AREA_H_
#define _TBT_NVS_AREA_H_

//
// Platform NVS Area definition
//
#include <TbtNvsAreaDef.h>

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

/**
 Making any TBT_NVS_AREA structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:   - Initial version.\n
  <b>Revision 2</b>:   - Adding TBT NVS AREA Revision, Deprecated DTbtControllerEn0, DTbtControllerEn1.\n
**/
#define TBT_NVS_AREA_REVISION       2

//
// Platform NVS Area Protocol
//
typedef struct _TBT_NVS_AREA_PROTOCOL {
  TBT_NVS_AREA     *Area;
} TBT_NVS_AREA_PROTOCOL;

#endif // _TBT_NVS_AREA_H_

