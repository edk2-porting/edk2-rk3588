/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _IPMI_INTERFACE_PROTOCOL_H_
#define _IPMI_INTERFACE_PROTOCOL_H_

#define IPMI_INTERFACE_PROTOCOL_GUID \
  {0xa37e200e, 0xda90, 0x473b, {0x8b, 0xb5, 0x1d, 0x7b, 0x11, 0xba, 0x32, 0x33}}

typedef struct _IPMI_INTERFACE_PROTOCOL IPMI_INTERFACE_PROTOCOL;

//
// Structure to store IPMI Network Function, LUN and command
//
typedef struct {
  UINT8 Lun   : 2;
  UINT8 NetFn : 6;
  UINT8 Cmd;
} IPMI_CMD_HEADER;

//
// System Interface Type
//
typedef enum {
  IPMI_SYSTEM_INTERFACE_UNKNOWN,
  IPMI_SYSTEM_INTERFACE_KCS,
  IPMI_SYSTEM_INTERFACE_SMIC,
  IPMI_SYSTEM_INTERFACE_BT,
  IPMI_SYSTEM_INTERFACE_SSIF,
  IPMI_SYSTEM_INTERFACE_MAX_TYPE
} IPMI_SYSTEM_INTERFACE_TYPE;

//
// System Interface Address Type
//
typedef enum {
  IPMI_MEMORY,
  IPMI_IO,
  IPMI_MAX_INTERFACE_ADDRESS_TYPE
} IPMI_INTERFACE_ADDRESS_TYPE;

typedef
EFI_STATUS
(EFIAPI *IPMI_INTERFACE_PROTOCOL_EXECUTE_IPMI_CMD) (
  IN  IPMI_INTERFACE_PROTOCOL       *This,
  IN  IPMI_CMD_HEADER               Request,
  IN  VOID                          *SendData OPTIONAL,
  IN  UINT8                         SendLength,
  OUT VOID                          *RecvData,
  OUT UINT8                         *RecvLength,
  OUT UINT16                        *StatusCodes OPTIONAL
);
typedef
IPMI_SYSTEM_INTERFACE_TYPE
(EFIAPI *IPMI_INTERFACE_PROTOCOL_GET_IPMI_INTERFACE_TYPE) (
  IN  IPMI_INTERFACE_PROTOCOL   *This
);
typedef
UINT16
(EFIAPI *IPMI_INTERFACE_PROTOCOL_GET_IPMI_BASE_ADDRESS) (
  IN  IPMI_INTERFACE_PROTOCOL   *This
);
typedef
IPMI_INTERFACE_ADDRESS_TYPE
(EFIAPI *IPMI_INTERFACE_PROTOCOL_GET_IPMI_BASE_ADDRESS_TYPE) (
  IN  IPMI_INTERFACE_PROTOCOL   *This
);
typedef
UINT8
(EFIAPI *IPMI_INTERFACE_PROTOCOL_GET_IPMI_VERSION) (
  IN  IPMI_INTERFACE_PROTOCOL   *This
);

//
// Structure of IPMI_INTERFACE_PROTOCOL
//
struct _IPMI_INTERFACE_PROTOCOL{
  IPMI_INTERFACE_PROTOCOL_EXECUTE_IPMI_CMD             ExecuteIpmiCmd;
  IPMI_INTERFACE_PROTOCOL_GET_IPMI_INTERFACE_TYPE      GetIpmiInterfaceType;
  IPMI_INTERFACE_PROTOCOL_GET_IPMI_BASE_ADDRESS        GetIpmiBaseAddress;
  IPMI_INTERFACE_PROTOCOL_GET_IPMI_BASE_ADDRESS_TYPE   GetIpmiBaseAddressType;
  IPMI_INTERFACE_PROTOCOL_GET_IPMI_VERSION             GetIpmiVersion;
} ;

extern EFI_GUID gIpmiInterfaceProtocolGuid;

#endif
