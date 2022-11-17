/** @file
  This file defines the SA Iotrap SMI Protocol to provide the
  I/O address for registered Iotrap SMI.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_IOTRAP_SMI_PROTOCOL_H_
#define _SA_IOTRAP_SMI_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                       gSaIotrapSmiProtocolGuid;

#define SA_IOTRAP_SMI_PROTOCOL_REVISION_1 1

//
// SA IO Trap SMI Protocol definition (Private protocol for RC internal use only)
//
typedef struct {
/*
 Protocol revision number
 Any backwards compatible changes to this protocol will result in an update in the revision number
 Major changes will require publication of a new protocol

  <b>Revision 1</b>:
    - First version
*/
  UINT8   Revision;
  UINT16  SaIotrapSmiAddress;
} SA_IOTRAP_SMI_PROTOCOL;

#endif
