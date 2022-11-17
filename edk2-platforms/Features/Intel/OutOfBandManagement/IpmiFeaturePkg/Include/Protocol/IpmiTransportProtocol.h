/** @file
  IPMITtransport Protocol Header File.

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_TRANSPORT_PROTO_H_
#define _IPMI_TRANSPORT_PROTO_H_

#include <ServerManagement.h>

typedef struct _IPMI_TRANSPORT IPMI_TRANSPORT;

#define IPMI_TRANSPORT_PROTOCOL_GUID \
  { \
    0x6bb945e8, 0x3743, 0x433e, 0xb9, 0xe, 0x29, 0xb3, 0xd, 0x5d, 0xc6, 0x30 \
  }

#define SMM_IPMI_TRANSPORT_PROTOCOL_GUID \
{ \
  0x8bb070f1, 0xa8f3, 0x471d, 0x86, 0x16, 0x77, 0x4b, 0xa3, 0xf4, 0x30, 0xa0 \
}

//
// Common Defines
//
typedef UINT32  BMC_STATUS;

#define BMC_OK                  0
#define BMC_SOFTFAIL            1
#define BMC_HARDFAIL            2
#define BMC_UPDATE_IN_PROGRESS  3
#define BMC_NOTREADY            4

//
//  IPMI Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *IPMI_SEND_COMMAND) (
  IN IPMI_TRANSPORT                    *This,
  IN UINT8                             NetFunction,
  IN UINT8                             Lun,
  IN UINT8                             Command,
  IN UINT8                             *CommandData,
  IN UINT32                            CommandDataSize,
  OUT UINT8                            *ResponseData,
  OUT UINT32                           *ResponseDataSize
  );

typedef
EFI_STATUS
(EFIAPI *IPMI_GET_CHANNEL_STATUS) (
  IN IPMI_TRANSPORT                   *This,
  OUT BMC_STATUS                      *BmcStatus,
  OUT SM_COM_ADDRESS                  *ComAddress
  );

//
// IPMI TRANSPORT PROTOCOL
//
struct _IPMI_TRANSPORT {
  UINT64                      Revision;
  IPMI_SEND_COMMAND           IpmiSubmitCommand;
  IPMI_GET_CHANNEL_STATUS     GetBmcStatus;
  EFI_HANDLE                  IpmiHandle;
  UINT8                       CompletionCode;
};

extern EFI_GUID gIpmiTransportProtocolGuid;
extern EFI_GUID gSmmIpmiTransportProtocolGuid;

#endif
