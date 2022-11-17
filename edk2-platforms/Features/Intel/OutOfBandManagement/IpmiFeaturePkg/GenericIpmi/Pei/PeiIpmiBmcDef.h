/** @file
  Generic IPMI transport layer common head file during PEI phase

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_IPMI_COMMON_BMC_H_
#define _PEI_IPMI_COMMON_BMC_H_

#include <Ppi/IpmiTransportPpi.h>

#define MAX_TEMP_DATA     160
#define BMC_SLAVE_ADDRESS 0x20
#define MAX_SOFT_COUNT    10
#define COMP_CODE_NORMAL  0x00

//
// IPMI command completion codes to check for in the UpdateErrorStatus routine.
// These completion codes indicate a soft error and a running total of the occurrences
// of these errors is maintained.
//
#define COMP_CODE_NODE_BUSY               0xC0
#define COMP_CODE_TIMEOUT                 0xC3
#define COMP_CODE_OUT_OF_SPACE            0xC4
#define COMP_CODE_OUT_OF_RANGE            0xC9
#define COMP_CODE_CMD_RESP_NOT_PROVIDED   0xCE
#define COMP_CODE_FAIL_DUP_REQUEST        0xCF
#define COMP_CODE_SDR_REP_IN_UPDATE_MODE  0xD0
#define COMP_CODE_DEV_IN_FW_UPDATE_MODE   0xD1
#define COMP_CODE_BMC_INIT_IN_PROGRESS    0xD2
#define COMP_CODE_UNSPECIFIED             0xFF

#define COMPLETION_CODES \
  { \
    COMP_CODE_NODE_BUSY, COMP_CODE_TIMEOUT, COMP_CODE_OUT_OF_SPACE, COMP_CODE_OUT_OF_RANGE, \
    COMP_CODE_CMD_RESP_NOT_PROVIDED, COMP_CODE_FAIL_DUP_REQUEST, COMP_CODE_SDR_REP_IN_UPDATE_MODE, \
    COMP_CODE_DEV_IN_FW_UPDATE_MODE, COMP_CODE_BMC_INIT_IN_PROGRESS, COMP_CODE_UNSPECIFIED \
  }
//
// Ensure proper structure formats
//
#pragma pack(1)
//
// Pei Ipmi instance data
//
typedef struct {
  UINTN                  Signature;
  UINT64                 KcsTimeoutPeriod;
  UINT8                  SlaveAddress;
  UINT8                  TempData[MAX_TEMP_DATA];
  BMC_STATUS             BmcStatus;
  UINT64                 ErrorStatus;
  UINT8                  SoftErrorCount;
  UINT16                 IpmiIoBase;
  PEI_IPMI_TRANSPORT_PPI IpmiTransportPpi;
  EFI_PEI_PPI_DESCRIPTOR PeiIpmiBmcDataDesc;
} PEI_IPMI_BMC_INSTANCE_DATA;

//
// Structure of IPMI Command buffer
//
#define IPMI_COMMAND_HEADER_SIZE  2

typedef struct {
  UINT8 Lun : 2;
  UINT8 NetFunction : 6;
  UINT8 Command;
  UINT8 CommandData[MAX_TEMP_DATA - IPMI_COMMAND_HEADER_SIZE];
} IPMI_COMMAND;

//
// Structure of IPMI Command response buffer
//
#define IPMI_RESPONSE_HEADER_SIZE 3

typedef struct {
  UINT8 Lun : 2;
  UINT8 NetFunction : 6;
  UINT8 Command;
  UINT8 CompletionCode;
  UINT8 ResponseData[MAX_TEMP_DATA - IPMI_RESPONSE_HEADER_SIZE];
} IPMI_RESPONSE;
#pragma pack()

EFI_STATUS
PeiIpmiSendCommandToBmc (
  IN      PEI_IPMI_TRANSPORT_PPI        *This,
  IN      UINT8                         NetFunction,
  IN      UINT8                         Lun,
  IN      UINT8                         Command,
  IN      UINT8                         *CommandData,
  IN      UINT8                         CommandDataSize,
  IN OUT  UINT8                         *ResponseData,
  IN OUT  UINT8                         *ResponseDataSize,
  IN      VOID                          *Context
  )
/*++

Routine Description:

  Send IPMI command to BMC

Arguments:

  This              - Pointer to IPMI protocol instance
  NetFunction       - Net Function of command to send
  Lun               - LUN of command to send
  Command           - IPMI command to send
  CommandData       - Pointer to command data buffer, if needed
  CommandDataSize   - Size of command data buffer
  ResponseData      - Pointer to response data buffer
  ResponseDataSize  - Pointer to response data buffer size
  Context           - Context

Returns:

  EFI_INVALID_PARAMETER - One of the input values is bad
  EFI_DEVICE_ERROR      - IPMI command failed
  EFI_BUFFER_TOO_SMALL  - Response buffer is too small
  EFI_SUCCESS           - Command completed successfully

--*/
;


EFI_STATUS
PeiIpmiBmcStatus (
  IN      PEI_IPMI_TRANSPORT_PPI       *This,
  OUT BMC_STATUS                       *BmcStatus,
  OUT SM_COM_ADDRESS                   *ComAddress,
  IN  VOID                             *Context
  )
/*++

Routine Description:

  Updates the BMC status and returns the Com Address

Arguments:

  This        - Pointer to IPMI protocol instance
  BmcStatus   - BMC status
  ComAddress  - Com Address
  Context     - Context

Returns:

  EFI_SUCCESS - Success

--*/
;


#endif //_PEI_IPMI_COMMON_BMC_H_
