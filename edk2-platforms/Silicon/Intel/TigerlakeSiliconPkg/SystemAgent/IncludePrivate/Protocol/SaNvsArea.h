/** @file
  Definition of the System Agent global NVS area protocol.
  This protocol publishes the address and format of a global ACPI NVS buffer
  used as a communications buffer between SMM/DXE/PEI code and ASL code.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SYSTEM_AGENT_NVS_AREA_H_
#define _SYSTEM_AGENT_NVS_AREA_H_

//
// SA NVS Area definition
//
#include <SaNvsAreaDef.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gSaNvsAreaProtocolGuid;

///
/// System Agent Global NVS Area Protocol
///
typedef struct {
  SYSTEM_AGENT_NVS_AREA *Area;        ///< System Agent Global NVS Area Structure
} SYSTEM_AGENT_NVS_AREA_PROTOCOL;

#endif // _SYSTEM_AGENT_NVS_AREA_H_
