/** @file
    KCS Transport Hook.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "KcsBmc.h"
#include <Library/DebugLib.h>

EFI_STATUS
KcsErrorExit (
  UINT64                            KcsTimeoutPeriod,
  UINT16                            KcsPort
  )
/*++

Routine Description:

  Check the KCS error status

Arguments:
     
  KcsPort          - The base port of KCS

Returns:

  EFI_DEVICE_ERROR - The device error happened
  EFI_SUCCESS      - Successfully check the KCS error status

--*/
{
  EFI_STATUS      Status;
  UINT8           KcsData;
  EFI_KCS_STATUS  KcsStatus;
  UINT8           BmcStatus;
  UINT8           RetryCount;
  UINT64          TimeOut;

  TimeOut     = 0;
  RetryCount  = 0;
  while (RetryCount < KCS_ABORT_RETRY_COUNT) {

    do {
      MicroSecondDelay(KCS_DELAY_UNIT);
      KcsStatus.RawData = IoRead8 (KcsPort + 1);
      if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
        RetryCount = KCS_ABORT_RETRY_COUNT;
        break;
      }
      TimeOut++;
    } while (KcsStatus.Status.Ibf);

    if (RetryCount >= KCS_ABORT_RETRY_COUNT) {
      break;
    }

    KcsData = KCS_ABORT;
    IoWrite8 ((KcsPort + 1), KcsData);

    TimeOut     = 0;
    do {
      MicroSecondDelay(KCS_DELAY_UNIT);
      KcsStatus.RawData = IoRead8 (KcsPort + 1);
      if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
        Status = EFI_DEVICE_ERROR;
        goto LabelError;
      }
      TimeOut++;
    } while (KcsStatus.Status.Ibf);

    KcsData = IoRead8 (KcsPort);

    KcsData = 0x0;
    IoWrite8 (KcsPort, KcsData);

    TimeOut     = 0;	
    do {
      MicroSecondDelay(KCS_DELAY_UNIT);
      KcsStatus.RawData = IoRead8 (KcsPort + 1);
      if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
        Status = EFI_DEVICE_ERROR;
        goto LabelError;
      }
      TimeOut++;
    } while (KcsStatus.Status.Ibf);

    if (KcsStatus.Status.State == KcsReadState) {
      TimeOut     = 0;	
      do {
        MicroSecondDelay(KCS_DELAY_UNIT);
        KcsStatus.RawData = IoRead8 (KcsPort + 1);
        if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
          Status = EFI_DEVICE_ERROR;
          goto LabelError;
        }
        TimeOut++;
      } while (!KcsStatus.Status.Obf);

      BmcStatus = IoRead8 (KcsPort);

      KcsData = KCS_READ;
      IoWrite8 (KcsPort, KcsData);

      TimeOut     = 0;
      do {
        MicroSecondDelay(KCS_DELAY_UNIT);
        KcsStatus.RawData = IoRead8 (KcsPort + 1);
        if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
          Status = EFI_DEVICE_ERROR;
          goto LabelError;
        }
        TimeOut++;
      } while (KcsStatus.Status.Ibf);

      if (KcsStatus.Status.State == KcsIdleState) {
        TimeOut     = 0;
        do {
          MicroSecondDelay(KCS_DELAY_UNIT);
          KcsStatus.RawData = IoRead8 (KcsPort + 1);
          if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
            Status = EFI_DEVICE_ERROR;
            goto LabelError;
          }
          TimeOut++;
        } while (!KcsStatus.Status.Obf);

        KcsData = IoRead8 (KcsPort);
        break;

      } else {
        RetryCount++;
        continue;
      }

    } else {
      RetryCount++;
      continue;
    }
  }

  if (RetryCount >= KCS_ABORT_RETRY_COUNT) {
    Status = EFI_DEVICE_ERROR;
    goto LabelError;
  }

  return EFI_SUCCESS;

LabelError:

  return Status;
}

EFI_STATUS
KcsCheckStatus (
  UINT64                            KcsTimeoutPeriod,
  UINT16                            KcsPort,
  KCS_STATE                         KcsState,
  BOOLEAN                           *Idle
  )
/*++

Routine Description:

  Ckeck KCS status

Arguments:

  KcsPort       - The base port of KCS
  KcsState      - The state of KCS to be checked
  Idle          - If the KCS is idle

Returns:

  EFI_SUCCESS   - Checked the KCS status successfully

--*/
{
  EFI_STATUS      Status = 0;
  EFI_KCS_STATUS  KcsStatus = { 0 };
  UINT8           KcsData = 0;
  UINT64          TimeOut = 0;
  
  if(Idle == NULL ){ 
       return EFI_INVALID_PARAMETER;
  }
	
  *Idle   = FALSE;
   
  do {
    MicroSecondDelay(KCS_DELAY_UNIT);
    KcsStatus.RawData = IoRead8 (KcsPort + 1);
    if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
      Status = EFI_DEVICE_ERROR;
      goto LabelError;
    }
    TimeOut++;
  } while (KcsStatus.Status.Ibf);

  if (KcsState == KcsWriteState) {
    KcsData = IoRead8 (KcsPort);
  }

  if (KcsStatus.Status.State != KcsState) {
    if ((KcsStatus.Status.State == KcsIdleState) && (KcsState == KcsReadState)) {
      *Idle = TRUE;
    } else {
      Status = KcsErrorExit (KcsTimeoutPeriod, KcsPort);
      goto LabelError;
    }
  }

  if (KcsState == KcsReadState) {
    TimeOut     = 0;
    do {
      MicroSecondDelay(KCS_DELAY_UNIT);
      KcsStatus.RawData = IoRead8 (KcsPort + 1);
      if (KcsStatus.RawData == 0xFF || (TimeOut >= KcsTimeoutPeriod)) {
        Status = EFI_DEVICE_ERROR;
        goto LabelError;
      }
      TimeOut++;
    } while (!KcsStatus.Status.Obf);
  }

  if (KcsState == KcsWriteState || Idle) {
    KcsData = IoRead8 (KcsPort);
  }

  return EFI_SUCCESS;

LabelError:

  return Status;
}

EFI_STATUS
SendDataToBmc (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  UINT8                           *Data,
  UINT8                           DataSize
  )
/*++

Routine Description:

  Send data to BMC

Arguments:

  Data          - The data pointer to be sent
  DataSize      - The data size

Returns:

  EFI_SUCCESS   - Send out the data successfully

--*/
{
  EFI_KCS_STATUS  KcsStatus;
  UINT8           KcsData;
  UINT16          KcsIoBase;
  EFI_STATUS      Status;
  UINT8           i;
  BOOLEAN         Idle;
  UINT64          TimeOut = 0;

  DEBUG ((DEBUG_INFO, "SendDataToBmc (%ld, 0x%x) - ", KcsTimeoutPeriod, KcsPort));
  for (i = 0; i < DataSize; i++) {
    DEBUG ((DEBUG_INFO, "%02x ", Data[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  KcsIoBase = KcsPort;

  do {
    MicroSecondDelay(KCS_DELAY_UNIT);
    KcsStatus.RawData = IoRead8 (KcsIoBase + 1);
    if ((KcsStatus.RawData == 0xFF) || (TimeOut >= KcsTimeoutPeriod))
    {
      if ((Status = KcsErrorExit (KcsTimeoutPeriod, KcsIoBase)) != EFI_SUCCESS)
      {
        DEBUG ((DEBUG_INFO, "KcsErrorExit - %r\n", Status));
        return Status;
      }
    }
    TimeOut++;
  } while (KcsStatus.Status.Ibf);

  KcsData = KCS_WRITE_START;
  IoWrite8 ((KcsIoBase + 1), KcsData);
  if ((Status = KcsCheckStatus (KcsTimeoutPeriod, KcsIoBase, KcsWriteState, &Idle)) != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "KcsCheckStatus 1 - %r\n", Status));
    return Status;
  }

  for (i = 0; i < DataSize; i++) {
    if (i == (DataSize - 1)) {
      if ((Status = KcsCheckStatus (KcsTimeoutPeriod, KcsIoBase, KcsWriteState, &Idle)) != EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "KcsCheckStatus 2 - %r\n", Status));
        return Status;
      }

      KcsData = KCS_WRITE_END;
      IoWrite8 ((KcsIoBase + 1), KcsData);
    }

    Status = KcsCheckStatus (KcsTimeoutPeriod, KcsIoBase, KcsWriteState, &Idle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "KcsCheckStatus 3 - %r\n", Status));
      return Status;
    }

    IoWrite8 (KcsIoBase, Data[i]);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ReceiveBmcData (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  UINT8                           *Data,
  UINT8                           *DataSize
  )
/*++

Routine Description:

  Routine Description:

  Receive data from BMC

Arguments:

  Data          - The buffer pointer
  DataSize      - The buffer size

Returns:

  EFI_SUCCESS   - Received data successfully

--*/
{
  UINT8       KcsData;
  UINT16      KcsIoBase;
  EFI_STATUS  Status;
  BOOLEAN     Idle;
  UINT8       Count;

  Count     = 0;
  KcsIoBase = KcsPort;

  DEBUG ((DEBUG_INFO, "ReceiveBmcData (%ld, 0x%x)...\n", KcsTimeoutPeriod, KcsPort));

  while (TRUE) {

    if ((Status = KcsCheckStatus (KcsTimeoutPeriod, KcsIoBase, KcsReadState, &Idle)) != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "KcsCheckStatus - %r\n", Status));
      return Status;
    }

    if (Idle) {
      DEBUG ((DEBUG_INFO, "DataSize - 0x%x\n", Count));
      *DataSize = Count;
      break;
    }

    if (Count > *DataSize) {
      DEBUG ((DEBUG_INFO, "ERROR: Count(0x%x) > *DataSize(0x%x)\n", Count, *DataSize));
      return EFI_DEVICE_ERROR;
    }

    Data[Count] = IoRead8 (KcsIoBase);

    Count++;

    KcsData = KCS_READ;
    IoWrite8 (KcsIoBase, KcsData);
  }

  DEBUG ((DEBUG_INFO, "ReceiveBmcData (%ld, 0x%x) - ", KcsTimeoutPeriod, KcsPort));
  for (Count = 0; Count < *DataSize; Count++) {
    DEBUG ((DEBUG_INFO, "%02x ", Data[Count]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  return EFI_SUCCESS;
}

EFI_STATUS
ReceiveBmcDataFromPort (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  UINT8                           *Data,
  UINT8                           *DataSize
  )
/*++

Routine Description:

  Receive data from BMC

Arguments:

  Data          - The buffer pointer to receive data
  DataSize      - The buffer size

Returns:

  EFI_SUCCESS   - Received the data successfully

--*/
{
  EFI_STATUS  Status;
  UINT16      KcsIoBase;
  UINT8       i;
  UINT8       MyDataSize;

  MyDataSize  = *DataSize;

  KcsIoBase   = KcsPort;

  for (i = 0; i < KCS_ABORT_RETRY_COUNT; i++) {
    Status = ReceiveBmcData (KcsTimeoutPeriod, KcsIoBase, Data, DataSize);
    if (EFI_ERROR (Status)) {
      if ((Status = KcsErrorExit (KcsTimeoutPeriod, KcsIoBase)) != EFI_SUCCESS) {
        return Status;
      }

      *DataSize = MyDataSize;
    } else {
      return Status;
    }
  }

  return EFI_DEVICE_ERROR;
}

EFI_STATUS
SendDataToBmcPort (
  UINT64                          KcsTimeoutPeriod,
  UINT16                          KcsPort,
  UINT8                           *Data,
  UINT8                           DataSize
  )
/*++

Routine Description:

  Send data to BMC

Arguments:

  Data          - The data pointer to be sent
  DataSize      - The data size

Returns:

  EFI_SUCCESS   - Send out the data successfully

--*/
{
  EFI_STATUS  Status;
  UINT16      KcsIoBase;
  UINT8       i;

  KcsIoBase = KcsPort;

  for (i = 0; i < KCS_ABORT_RETRY_COUNT; i++) {
    Status = SendDataToBmc (KcsTimeoutPeriod, KcsIoBase, Data, DataSize);
    if (EFI_ERROR (Status)) {
      if ((Status = KcsErrorExit (KcsTimeoutPeriod, KcsIoBase)) != EFI_SUCCESS) {
        return Status;
      }
    } else {
      return Status;
    }
  }

  return EFI_DEVICE_ERROR;
}
