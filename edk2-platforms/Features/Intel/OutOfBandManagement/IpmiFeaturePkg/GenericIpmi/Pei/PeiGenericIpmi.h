/** @file
  Generic IPMI stack head file during PEI phase

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_IPMI_INIT_H_
#define _PEI_IPMI_INIT_H_

#include <PiPei.h>
#include <Uefi.h>

#include <Ppi/IpmiTransportPpi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/TimerLib.h>
#include <Library/PciLib.h>

#include "PeiIpmiBmcDef.h"
#include "PeiIpmiBmc.h"

//
// Prototypes
//
#define MBXDAT_B                          0x0B
#define BMC_KCS_TIMEOUT_PEI               5     // [s] Single KSC request timeout
#define KCS_DELAY_UNIT_PEI                1000  // [s] Each KSC IO delay
#define IPMI_DEFAULT_IO_BASE              0xCA2

//
// Internal(hook) function list
//
EFI_STATUS
SendPreBootSignaltoBmc (
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
  /*++

Routine Description:
  Send Pre-Boot signal to BMC

Arguments:
  PeiServices           - General purpose services available to every PEIM.

Returns:
  EFI_SUCCESS           - Success
--*/
;

EFI_STATUS
PeiIpmiSendCommand (
  IN      PEI_IPMI_TRANSPORT_PPI       *This,
  IN      UINT8                        NetFunction,
  IN      UINT8                        Lun,
  IN      UINT8                        Command,
  IN      UINT8                        *CommandData,
  IN      UINT32                       CommandDataSize,
  IN OUT  UINT8                        *ResponseData,
  IN OUT  UINT32                       *ResponseDataSize
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

Returns:
  EFI_INVALID_PARAMETER - One of the input values is bad
  EFI_DEVICE_ERROR      - IPMI command failed
  EFI_BUFFER_TOO_SMALL  - Response buffer is too small
  EFI_UNSUPPORTED       - Command is not supported by BMC
  EFI_SUCCESS           - Command completed successfully
--*/
;

EFI_STATUS
PeiGetIpmiBmcStatus (
  IN  PEI_IPMI_TRANSPORT_PPI                           *This,
  OUT BMC_STATUS                                       *BmcStatus,
  OUT SM_COM_ADDRESS                                   *ComAddress
  )
/*++

Routine Description:
  Updates the BMC status and returns the Com Address

Arguments:
  This        - Pointer to IPMI protocol instance
  BmcStatus   - BMC status
  ComAddress  - Com Address

Returns:
  EFI_SUCCESS - Success
--*/
;

//
// internal function list
//
EFI_STATUS
GetDeviceId (
  IN      PEI_IPMI_BMC_INSTANCE_DATA   *mIpmiInstance
  )
/*++

Routine Description:
  Execute the Get Device ID command to determine whether or not the BMC is in Force Update
  Mode.  If it is, then report it to the error manager.

Arguments:
  mIpmiInstance   - Data structure describing BMC variables and used for sending commands
  StatusCodeValue - An array used to accumulate error codes for later reporting.
  ErrorCount      - Counter used to keep track of error codes in StatusCodeValue

Returns:
  Status

--*/
;
#endif //_PEI_IPMI_INIT_H_

