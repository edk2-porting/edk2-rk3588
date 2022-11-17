/** @file
  KCS Transport Hook head file.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _KCS_BMC_H
#define _KCS_BMC_H

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#define KCS_WRITE_START       0x61
#define KCS_WRITE_END         0x62
#define KCS_READ              0x68
#define KCS_GET_STATUS        0x60
#define KCS_ABORT             0x60
#define KCS_DELAY_UNIT        50  // [s] Each KSC IO delay

//
// In OpenBMC, UpdateMode: the bit 7 of byte 4 in get device id command is used for the BMC status:
// 0 means BMC is ready, 1 means BMC is not ready.
// At the very beginning of BMC power on, the status is 1 means BMC is in booting process and not ready. It is not the flag for force update mode.
//
#define BMC_READY             0


#define KCS_ABORT_RETRY_COUNT 1

//#define TIMEOUT64(a,b)  ((INT64)((b) - (a)) < 0)

typedef enum {
  KcsIdleState,
  KcsReadState,
  KcsWriteState,
  KcsErrorState
} KCS_STATE;

typedef union {
  UINT8     RawData;
  struct {
    UINT8 Obf   : 1;
    UINT8 Ibf   : 1;
    UINT8 SmAtn : 1;
    UINT8 CD    : 1;
    UINT8 Oem1  : 1;
    UINT8 Oem2  : 1;
    UINT8 State : 2;
  } Status;
} KCS_STATUS;


//
//External Fucntion List
//
EFI_STATUS
SendDataToBmcPort (
  UINT64                                    KcsTimeoutPeriod,
  UINT16                                    KcsPort,
  VOID                                      *Context,
  UINT8                                     *Data,
  UINT8                                     DataSize
  )
/*++

Routine Description:

  Send data to BMC

Arguments:

  IpmiInstance  - The pointer of IPMI_BMC_INSTANCE_DATA
  Context       - The context of this operation
  Data          - The data pointer to be sent
  DataSize      - The data size

Returns:

  EFI_SUCCESS   - Send out the data successfully

--*/
;

EFI_STATUS
ReceiveBmcDataFromPort (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  VOID                            *Context,
  UINT8                           *Data,
  UINT8                           *DataSize
  )
/*++

Routine Description:

  Routine Description:

  Receive data from BMC

Arguments:

  IpmiInstance  - The pointer of IPMI_BMC_INSTANCE_DATA
  Context       - The context of this operation
  Data          - The buffer pointer
  DataSize      - The buffer size

Returns:

  EFI_SUCCESS   - Received data successfully

--*/
;

//
//Internal Fucntion List
//
EFI_STATUS
KcsErrorExit (
  UINT64                            KcsTimeoutPeriod,
  UINT16                            KcsPort,
  VOID                              *Context
  )
/*++

Routine Description:

  Check the KCS error status

Arguments:

  IpmiInstance     - The pointer of IPMI_BMC_INSTANCE_DATA
  KcsPort          - The base port of KCS
  Context          - The Context for this operation

Returns:

  EFI_DEVICE_ERROR - The device error happened
  EFI_SUCCESS      - Successfully check the KCS error status

--*/
;

EFI_STATUS
KcsCheckStatus (
  UINT64                            KcsTimeoutPeriod,
  UINT16                            KcsPort,
  KCS_STATE                         KcsState,
  BOOLEAN                           *Idle,
  VOID                              *Context
  )
/*++

Routine Description:

  Ckeck KCS status

Arguments:

  IpmiInstance  - The pointer of IPMI_BMC_INSTANCE_DATA
  KcsPort       - The base port of KCS
  KcsState      - The state of KCS to be checked
  Idle          - If the KCS is idle
  Context       - The context for this operation

Returns:

  EFI_SUCCESS   - Checked the KCS status successfully

--*/
;


EFI_STATUS
SendDataToBmc (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  VOID                            *Context,
  UINT8                           *Data,
  UINT8                           DataSize
  )
/*++

Routine Description:

  Send data to BMC

Arguments:

  IpmiInstance  - The pointer of IPMI_BMC_INSTANCE_DATA
  Context       - The context of this operation
  Data          - The data pointer to be sent
  DataSize      - The data size

Returns:

  EFI_SUCCESS   - Send out the data successfully

--*/
;


EFI_STATUS
ReceiveBmcData (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  VOID                            *Context,
  UINT8                           *Data,
  UINT8                           *DataSize
  )
/*++

Routine Description:

  Routine Description:

  Receive data from BMC

Arguments:

  IpmiInstance  - The pointer of IPMI_BMC_INSTANCE_DATA
  Context       - The context of this operation
  Data          - The buffer pointer
  DataSize      - The buffer size

Returns:

  EFI_SUCCESS   - Received data successfully

--*/
;

#endif
