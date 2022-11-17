/** @file
  Base EC library implementation for H/W layer.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/EcLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>

#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND  1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  1000
#endif

#ifndef EC_INIT_TIME_OUT
#define EC_INIT_TIME_OUT       0x200
#endif

typedef struct {
  UINT8   CommandNumber;
  UINT8   NumberOfSendData;
  UINT8   NumberOfReceiveData;
  BOOLEAN CommandImplemented;
} EC_COMMAND_TABLE;

EC_COMMAND_TABLE mEcCommand[] = {
  {EC_C_FAB_ID                   , 0, 2, TRUE},  // Get the board fab ID in the lower 3 bits
  {EC_C_ACPI_READ                , 1, 1, TRUE},  // Read a byte of EC RAM
  {EC_C_ACPI_WRITE               , 2, 0, TRUE}   // Write a byte of EC RAM
};

//
// Function implementations
//
/**
  Sends command to EC.

  @param[in] Command           Command byte to send
  @param[in] Timeout           Timeout in microseonds

  @retval    EFI_SUCCESS       Command success
  @retval    EFI_DEVICE_ERROR  Command error
  @retval    EFI_TIMEOUT       Command timeout
**/
EFI_STATUS
SendEcCommandTimeout (
  IN UINT8                  Command,
  IN UINT32                 Timeout
  )
{
  UINTN         Index;
  UINT8         EcStatus;
  UINT8         DiscardData;

  Index = 0;
  EcStatus = 0;
  DiscardData = 0;
  //
  // Wait for EC to be ready (with a timeout)
  //
  ReceiveEcStatus (&EcStatus);
  //
  // Check if output buffer bit(OBF) is set.
  // Read and discard the output buffer data so that next BIOS-EC cmd is in sync
  // OBF is cleared by h/w after all data in output buffer is read by BIOS.
  //
  while ((EcStatus & EC_S_OBF) != 0) {
    //
    // Read EC data
    //
    DiscardData = IoRead8 (EC_D_PORT);
    DEBUG ((DEBUG_INFO, "OBF is Set!! DiscardData Read 0x%02X\n", DiscardData));
    ReceiveEcStatus (&EcStatus);
  }
  while (((EcStatus & EC_S_IBF) != 0) && (Index < Timeout)) {
    MicroSecondDelay (15 * STALL_ONE_MICRO_SECOND);
    ReceiveEcStatus (&EcStatus);
    Index++;
  }

  if (Index >= Timeout) {
    return EFI_TIMEOUT;
  }

  //Printing EC Command Sent
  DEBUG ((DEBUG_INFO, "Sending EC Command: %02X\n", Command));

  //
  // Send the EC command
  //
  IoWrite8 (EC_C_PORT, Command);

  return EFI_SUCCESS;
}


/**
  Sends command to EC.

  @param[in] Command           Command byte to send

  @retval    EFI_SUCCESS       Command success
  @retval    EFI_DEVICE_ERROR  Command error
  @retval    EFI_TIMEOUT       Command timeout
**/
EFI_STATUS
SendEcCommand (
  IN UINT8                  Command
  )
{
  return SendEcCommandTimeout (Command, EC_TIME_OUT);
}


/**
  Receives status from EC.

  @param[out] EcStatus       Status byte to receive

  @retval     EFI_SUCCESS
  @retval     EFI_DEVICE_ERROR
**/
EFI_STATUS
ReceiveEcStatus (
  OUT UINT8                 *EcStatus
  )
{
  //
  // Read and return the status
  //
  *EcStatus = IoRead8 (EC_C_PORT);

  return EFI_SUCCESS;
}


/**
  Sends data to EC.

  @param[in] Data          Data byte to send

  @retval    EFI_SUCCESS
  @retval    EFI_DEVICE_ERROR
**/
EFI_STATUS
SendEcData (
  IN UINT8                 Data
  )
{
  UINTN         Index;
  UINT8         EcStatus;

  Index = 0;

  //
  // Wait for EC to be ready (with a timeout)
  //
  ReceiveEcStatus (&EcStatus);
  while (((EcStatus & EC_S_IBF) != 0) && (Index < EC_TIME_OUT)) {
    MicroSecondDelay (15);
    ReceiveEcStatus (&EcStatus);
    Index++;
  }
  if (Index >= EC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }

  //
  //Printing EC Data Sent
  //
  DEBUG ((DEBUG_INFO, "Sending EC Data: %02X\n", Data));

  //
  // Send the data and return
  //
  IoWrite8 (EC_D_PORT, Data);

  return EFI_SUCCESS;
}


/**
  Receives data from EC.

  @param[out] Data              Data byte received
  @param[in]  Timeout           Timeout in microseonds

  @retval     EFI_SUCCESS       Read success
  @retval     EFI_DEVICE_ERROR  Read error
  @retval     EFI_TIMEOUT       Command timeout
--*/
EFI_STATUS
ReceiveEcDataTimeout (
  OUT UINT8                  *Data,
  IN  UINT32                 Timeout
 )
{
  UINTN         Index;
  UINT8         EcStatus;

  Index = 0;

  //
  // Wait for EC to be ready (with a timeout)
  //
  ReceiveEcStatus (&EcStatus);
  while (((EcStatus & EC_S_OBF) == 0) && (Index < Timeout)) {
    MicroSecondDelay (15 * STALL_ONE_MICRO_SECOND);
    ReceiveEcStatus (&EcStatus);
    Index++;
  }

  if (Index >= Timeout) {
    return EFI_TIMEOUT;
  }
  //
  // Read EC data and return
  //
  *Data = IoRead8 (EC_D_PORT);

  //Printing EC Data Received
  DEBUG ((DEBUG_INFO, "Receiving EC Data: %02X\n", *Data));

  return EFI_SUCCESS;
}
/**
  Receives data from EC.

  @param[out] Data              Data byte received

  @retval     EFI_SUCCESS       Read success
  @retval     EFI_DEVICE_ERROR  Read error
  @retval     EFI_TIMEOUT       Command timeout
**/
EFI_STATUS
ReceiveEcData (
  OUT UINT8                  *Data
 )
{
  return ReceiveEcDataTimeout (Data, EC_TIME_OUT);
}

/**
  Send data to EC through LPC interface.

  @param[in]      Command     Command value to send to the EC
  @param[in][out] DataSize    Size of data to send to the EC.
                              If the command retuned data - size of buffer returned by the EC.
                              Be aware of the DataSize must euqal to size of DataBuffer and cannot smaller
                              than number of send data or number of receive data, whichever is the grater.
  @param[in][out] DataBuffer  Pointer to the data buffer including data to be sent to the EC.
                              If the command returned data - pointer to the buffer including the data.
                              The buffer size should be the max of receive and transmit data.

  @retval         EFI_SUCCESS Success
  @retval         Other       Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                       EFI_BUFFER_TOO_SMALL, etc.
**/
EFI_STATUS
LpcEcInterface (
  IN UINT8                     Command,
  IN OUT UINT8                 *DataSize,
  IN OUT UINT8                 *DataBuffer
  )
{
  EFI_STATUS Status;
  UINT8      NumberOfEcCommands;
  UINT8      Index;
  UINT8      TxDataIndex;
  UINT8      RxDataIndex;
  UINT8      MaxValue;

  Status = EFI_SUCCESS;

  NumberOfEcCommands = sizeof (mEcCommand) / sizeof (EC_COMMAND_TABLE);

  for (Index = 0; Index < NumberOfEcCommands; Index++) {
    if (Command != mEcCommand[Index].CommandNumber) {
      continue;
    }
    if (mEcCommand[Index].CommandImplemented != TRUE) {
      Status = EFI_UNSUPPORTED;
      break;
    }

    if ((mEcCommand[Index].NumberOfSendData != 0) || (mEcCommand[Index].NumberOfReceiveData != 0)) {
      if (DataSize == NULL || DataBuffer == NULL) {
        Status = EFI_INVALID_PARAMETER;
        break;
      } else {
        MaxValue = MAX (mEcCommand[Index].NumberOfSendData, mEcCommand[Index].NumberOfReceiveData);
        if (*DataSize < MaxValue) {
          Status = EFI_BUFFER_TOO_SMALL;
          *DataSize = MaxValue;
          break;
        }
      }
    }

    Status = SendEcCommand (Command);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (mEcCommand[Index].NumberOfSendData != 0) {
     for (TxDataIndex = 0; TxDataIndex < mEcCommand[Index].NumberOfSendData; TxDataIndex++) {
        Status = SendEcData (DataBuffer[TxDataIndex]);
        if (EFI_ERROR (Status)) {
          break;
        }
      }
    }
    if (mEcCommand[Index].NumberOfReceiveData != 0) {
      for (RxDataIndex = 0; RxDataIndex < mEcCommand[Index].NumberOfReceiveData; RxDataIndex++) {
        Status = ReceiveEcData (&DataBuffer[RxDataIndex]);
        if (EFI_ERROR (Status)) {
          break;
        }
        *DataSize = RxDataIndex + 1;
      }
    }
    break;
  }
  return Status;
}
