/** @file
  IPMI library - KCS.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/IpmiPlatformHookLib.h>

#include <IndustryStandard/Ipmi.h>

#include "KcsBmc.h"

#define MAX_TEMP_DATA     160

//
// Structure of IPMI Command buffer
//
#define EFI_IPMI_COMMAND_HEADER_SIZE  2

typedef struct {
  UINT8 Lun : 2;
  UINT8 NetFunction : 6;
  UINT8 Command;
  UINT8 CommandData[MAX_TEMP_DATA - EFI_IPMI_COMMAND_HEADER_SIZE];
} EFI_IPMI_COMMAND;

//
// Structure of IPMI Command response buffer
//
#define EFI_IPMI_RESPONSE_HEADER_SIZE 2

typedef struct {
  UINT8 Lun : 2;
  UINT8 NetFunction : 6;
  UINT8 Command;
  UINT8 ResponseData[MAX_TEMP_DATA - EFI_IPMI_RESPONSE_HEADER_SIZE];
} EFI_IPMI_RESPONSE;


#define IPMI_INSTANCE_INFO_HOB_GUID { \
  0x38ee71f, 0x1c78, 0x4874, { 0xba, 0xe3, 0xf8, 0xa2, 0x57, 0x75, 0x28, 0x52 } \
  }

EFI_GUID mIpmiInstanceGuid = IPMI_INSTANCE_INFO_HOB_GUID;

#define SM_IPMI_BMC_SIGNATURE SIGNATURE_32 ('i', 'p', 'm', 'i')
typedef UINT32  EFI_BMC_STATUS;
typedef struct {
  UINTN               Signature;
  UINT64              KcsTimeoutPeriod;
  UINT16              IpmiIoBase;
  UINT8               SlaveAddress;
  EFI_BMC_STATUS      BmcStatus;
  UINT64              ErrorStatus;
  UINT8               SoftErrorCount;
  UINT8               TempData[MAX_TEMP_DATA];
} IPMI_INSTANCE;

#define EFI_BMC_OK        0
#define EFI_BMC_SOFTFAIL  1
#define EFI_BMC_HARDFAIL  2
#define EFI_BMC_UPDATE_IN_PROGRESS  3
#define EFI_BMC_NOTREADY  4

EFI_STATUS
UpdateErrorStatus (
  IN UINT8                      BmcError,
  IPMI_INSTANCE                 *IpmiInstance
  )
/*++

Routine Description:

  Check if the completion code is a Soft Error and increment the count.  The count
  is not updated if the BMC is in Force Update Mode.

Arguments:

  BmcError      - Completion code to check
  IpmiInstance  - BMC instance data

Returns:

  EFI_SUCCESS   - Status

--*/
{
  UINT8   Errors[] = {
    IPMI_COMP_CODE_NODE_BUSY, IPMI_COMP_CODE_TIMEOUT, IPMI_COMP_CODE_OUT_OF_SPACE, IPMI_COMP_CODE_OUT_OF_RANGE,
    IPMI_COMP_CODE_CMD_RESP_NOT_PROVIDED, IPMI_COMP_CODE_FAIL_DUP_REQUEST, IPMI_COMP_CODE_SDR_REP_IN_UPDATE_MODE,
    IPMI_COMP_CODE_DEV_IN_FW_UPDATE_MODE, IPMI_COMP_CODE_BMC_INIT_IN_PROGRESS, IPMI_COMP_CODE_UNSPECIFIED
  };
  UINT16  CodeCount;
  UINT8   i;

  CodeCount = sizeof (Errors) / sizeof (Errors[0]);
  for (i = 0; i < CodeCount; i++) {
    if (BmcError == Errors[i]) {
      //
      // Don't change Bmc Status flag if the BMC is in Force Update Mode.
      //
      if (IpmiInstance->BmcStatus != EFI_BMC_UPDATE_IN_PROGRESS) {
        IpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
      }

      IpmiInstance->SoftErrorCount++;
      break;
    }
  }

  return EFI_SUCCESS;
}

VOID
UpdateBmcStatusOnResponse (
  IN IPMI_INSTANCE               *IpmiInstance,
  IN EFI_IPMI_COMMAND            *IpmiCommand,
  IN EFI_STATUS                  EfiStatus,
  IN EFI_IPMI_RESPONSE           *IpmiResponse
  )
{
  IPMI_GET_DEVICE_ID_RESPONSE      *BmcInfo;
  IPMI_SELF_TEST_RESULT_RESPONSE   *TestResult;

  if ((IpmiCommand->NetFunction == IPMI_NETFN_APP) && (IpmiCommand->Command == IPMI_APP_GET_DEVICE_ID)) {
    if (EFI_ERROR(EfiStatus)) {
      IpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
    } else {
      BmcInfo = (VOID *)IpmiResponse->ResponseData;
      if (BmcInfo->FirmwareRev1.Bits.UpdateMode) {
        IpmiInstance->BmcStatus = EFI_BMC_UPDATE_IN_PROGRESS;
      }
    }
  } else if ((IpmiCommand->NetFunction == IPMI_NETFN_APP) && (IpmiCommand->Command == IPMI_APP_GET_DEVICE_ID)) {
    if (EFI_ERROR(EfiStatus)) {
      IpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
    } else {
      TestResult = (VOID *)IpmiResponse->ResponseData;
      switch (TestResult->Result) {
      case IPMI_APP_SELFTEST_NO_ERROR:
      case IPMI_APP_SELFTEST_NOT_IMPLEMENTED:
        IpmiInstance->BmcStatus = EFI_BMC_OK;
        break;
      case IPMI_APP_SELFTEST_ERROR:
        //
        // Three of the possible errors result in BMC hard failure; FRU Corruption,
        // BootBlock Firmware corruption, and Operational Firmware Corruption.  All
        // other errors are BMC soft failures.
        //
        if ((TestResult->Param & (IPMI_APP_SELFTEST_FRU_CORRUPT | IPMI_APP_SELFTEST_FW_BOOTBLOCK_CORRUPT | IPMI_APP_SELFTEST_FW_CORRUPT)) != 0) {
          IpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
        } else {
          IpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
        }
        break;

      case IPMI_APP_SELFTEST_FATAL_HW_ERROR:
        IpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
        break;

      default:
        break;
      }
    }
  }
}

/**
  This service enables submitting commands via Ipmi.

  @param[in]         NetFunction       Net function of the command.
  @param[in]         Command           IPMI Command.
  @param[in]         RequestData       Command Request Data.
  @param[in]         RequestDataSize   Size of Command Request Data.
  @param[out]        ResponseData      Command Response Data. The completion code is the first byte of response data.
  @param[in, out]    ResponseDataSize  Size of Command Response Data.

  @retval EFI_SUCCESS            The command byte stream was successfully submit to the device and a response was successfully received.
  @retval EFI_NOT_FOUND          The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_NOT_READY          Ipmi Device is not ready for Ipmi command access.
  @retval EFI_DEVICE_ERROR       Ipmi Device hardware error.
  @retval EFI_TIMEOUT            The command time out.
  @retval EFI_UNSUPPORTED        The command was not successfully sent to the device.
  @retval EFI_OUT_OF_RESOURCES   The resource allcation is out of resource or data size error.
**/
EFI_STATUS
EFIAPI
IpmiSubmitCommand (
  IN     UINT8     NetFunction,
  IN     UINT8     Command,
  IN     UINT8     *RequestData,
  IN     UINT32    RequestDataSize,
     OUT UINT8     *ResponseData,
  IN OUT UINT32    *ResponseDataSize
  )
{
  UINT8                       DataSize;
  EFI_STATUS                  Status;
  EFI_IPMI_COMMAND            *IpmiCommand;
  EFI_IPMI_RESPONSE           *IpmiResponse;
  VOID                        *Hob;
  IPMI_INSTANCE               *IpmiInstance;

  DEBUG ((DEBUG_INFO, "IpmiSubmitCommand\n"));

  Hob = GetFirstGuidHob (&mIpmiInstanceGuid);
  if (Hob != NULL) {
    IpmiInstance = GET_GUID_HOB_DATA(Hob);
  } else {
    IpmiInstance = BuildGuidHob (&mIpmiInstanceGuid, sizeof(IPMI_INSTANCE));
    ASSERT(IpmiInstance != NULL);
    if (IpmiInstance == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    IpmiInstance->Signature                      = SM_IPMI_BMC_SIGNATURE;
    IpmiInstance->KcsTimeoutPeriod               = PcdGet64(PcdIpmiKcsTimeoutPeriod); 
    IpmiInstance->SlaveAddress                   = PcdGet8(PcdIpmiBmcSlaveAddress);
    IpmiInstance->IpmiIoBase                     = PcdGet16(PcdIpmiIoBaseAddress);
    DEBUG((DEBUG_INFO,"IPMI KcsTimeoutPeriod=0x%x\n", IpmiInstance->KcsTimeoutPeriod));
    DEBUG((DEBUG_INFO,"IPMI SlaveAddress=0x%x\n", IpmiInstance->SlaveAddress));
    DEBUG((DEBUG_INFO,"IPMI IpmiIoBase=0x%x\n", IpmiInstance->IpmiIoBase));

    IpmiInstance->BmcStatus                      = EFI_BMC_NOTREADY;
    IpmiInstance->ErrorStatus                    = 0x00;
    IpmiInstance->SoftErrorCount                 = 0x00;

    MicroSecondDelay(10*1000);

    Status = PlatformIpmiIoRangeSet (IpmiInstance->IpmiIoBase);
    DEBUG ((DEBUG_INFO, "IPMI PlatformIpmiIoRangeSet - %r!\n", Status));
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  IpmiCommand  = (VOID *)IpmiInstance->TempData;
  IpmiResponse = (VOID *)IpmiInstance->TempData;

  //
  // Send IPMI command to BMC
  //
  IpmiCommand->Lun          = 0;
  IpmiCommand->NetFunction  = NetFunction;
  IpmiCommand->Command      = Command;

  //
  // Ensure that the buffer is valid before attempting to copy the command data
  // buffer into the IpmiCommand structure.
  //
  if (RequestDataSize > 0) {
    if (RequestData == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    CopyMem (
      IpmiCommand->CommandData,
      RequestData,
      RequestDataSize
      );
  }

  Status = SendDataToBmcPort (
  	IpmiInstance->KcsTimeoutPeriod,
    IpmiInstance->IpmiIoBase,
    (UINT8 *)IpmiCommand,
    (UINT8)(RequestDataSize + EFI_IPMI_COMMAND_HEADER_SIZE)
    );

  if (Status != EFI_SUCCESS) {
    IpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
    IpmiInstance->SoftErrorCount++;
    UpdateBmcStatusOnResponse (IpmiInstance, IpmiCommand, Status, NULL);
    return Status;
  }

  //
  // Get Response to IPMI Command from BMC.
  //
  DataSize = MAX_TEMP_DATA;
  Status = ReceiveBmcDataFromPort (
  	IpmiInstance->KcsTimeoutPeriod,
    IpmiInstance->IpmiIoBase,
    (UINT8 *)IpmiResponse,
    &DataSize
    );

  if (Status != EFI_SUCCESS) {
    IpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
    IpmiInstance->SoftErrorCount++;
    UpdateBmcStatusOnResponse (IpmiInstance, IpmiCommand, Status, NULL);
    return Status;
  }

  //
  // If we got this far without any error codes, but the DataSize is 0 then the 
  // command response failed, so do not continue.  
  //
  if (DataSize < 3) {
    Status = EFI_DEVICE_ERROR;
    IpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
    IpmiInstance->SoftErrorCount++;
    UpdateBmcStatusOnResponse (IpmiInstance, IpmiCommand, Status, NULL);
    return Status;
  }

  if ((IpmiResponse->ResponseData[0] != IPMI_COMP_CODE_NORMAL) &&
      (IpmiInstance->BmcStatus == EFI_BMC_UPDATE_IN_PROGRESS)) {
    //
    // If the completion code is not normal and the BMC is in Force Update
    // mode, then update the error status and return EFI_UNSUPPORTED.
    //
    UpdateErrorStatus (
      IpmiResponse->ResponseData[0],
      IpmiInstance
      );
    UpdateBmcStatusOnResponse (IpmiInstance, IpmiCommand, Status, IpmiResponse);
    return EFI_UNSUPPORTED;
  } else if (IpmiResponse->ResponseData[0] != IPMI_COMP_CODE_NORMAL) {
    //
    // Otherwise if the BMC is in normal mode, but the completion code
    // is not normal, then update the error status and return device error.
    //
    UpdateErrorStatus (
      IpmiResponse->ResponseData[0],
      IpmiInstance
      );
    UpdateBmcStatusOnResponse (IpmiInstance, IpmiCommand, Status, IpmiResponse);
    return EFI_DEVICE_ERROR;
  }

  //
  // Verify the response data buffer passed in is big enough.
  //
  if ((UINTN)(DataSize - EFI_IPMI_RESPONSE_HEADER_SIZE) > *ResponseDataSize) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Copy data over to the response data buffer.
  //
  if ((ResponseData != NULL) && (ResponseDataSize != NULL) && (*ResponseDataSize != 0)) {
    *ResponseDataSize = DataSize - EFI_IPMI_RESPONSE_HEADER_SIZE;
    CopyMem (
      ResponseData,
      IpmiResponse->ResponseData,
      *ResponseDataSize
      );
  }
  IpmiInstance->BmcStatus = EFI_BMC_OK;

  return EFI_SUCCESS;
}

