/** @file
  Generic SMM IPMI stack driver

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include <IndustryStandard/Ipmi.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/SmmLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IpmiBaseLib.h>
#include <SmStatusCodes.h>
#include "IpmiHooks.h"
#include "IpmiBmcCommon.h"
#include "IpmiBmc.h"
#include <Library/TimerLib.h>

IPMI_BMC_INSTANCE_DATA             *mIpmiInstance;
EFI_HANDLE                         mImageHandle;


EFI_STATUS
GetDeviceId (
  IN      IPMI_BMC_INSTANCE_DATA       *IpmiInstance,
  IN      EFI_STATUS_CODE_VALUE        StatusCodeValue[ ],
  IN OUT  UINT8                        *ErrorCount
  )
/*++

Routine Description:
  Execute the Get Device ID command to determine whether or not the BMC is in Force Update
  Mode.  If it is, then report it to the error manager.

Arguments:
  IpmiInstance    - Data structure describing BMC variables and used for sending commands
  StatusCodeValue - An array used to accumulate error codes for later reporting.
  ErrorCount      - Counter used to keep track of error codes in StatusCodeValue

Returns:
  Status

--*/
{
  EFI_STATUS               Status;
  UINT32                   DataSize;
  SM_CTRL_INFO             *ControllerInfo;
  UINT8                    TimeOut;
  UINT8                    Retries;

  TimeOut = 0;
  Retries = PcdGet8 (PcdIpmiBmcReadyDelayTimer);

  do {
    //
    // Get the device ID information for the BMC.
    //
    DataSize = MAX_TEMP_DATA;
    Status = IpmiSendCommand (
               &IpmiInstance->IpmiTransport,
               IPMI_NETFN_APP,
               0,
               IPMI_APP_GET_DEVICE_ID,
               NULL,
               0,
               IpmiInstance->TempData,
               &DataSize
               );
    if (Status == EFI_SUCCESS) {
      DEBUG ((EFI_D_INFO, "IPMI: SendCommand success!\n"));
      break;
    } else {
      //
      // Display message and retry.
      //
      DEBUG (
        (EFI_D_ERROR | EFI_D_INFO,
         "IPMI: Waiting for BMC (KCS 0x%x)...\n",
         IpmiInstance->IpmiIoBase)
        );
      MicroSecondDelay (500 * 1000);
      TimeOut++;
    }
  } while (TimeOut < Retries);

  //
  // If there is no error then proceed to check the data returned by the BMC
  //
  if (!EFI_ERROR (Status)) {
    ControllerInfo = (SM_CTRL_INFO *) IpmiInstance->TempData;
    //
    // If the controller is in Update Mode and the maximum number of errors has not been exceeded, then
    // save the error code to the StatusCode array and increment the counter.  Set the BMC Status to indicate
    // the BMC is in force update mode.
    //
    if (ControllerInfo->UpdateMode != 0) {
      IpmiInstance->BmcStatus = BMC_UPDATE_IN_PROGRESS;
      if (*ErrorCount < MAX_SOFT_COUNT) {
        StatusCodeValue[*ErrorCount] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | CU_FP_EC_FORCE_UPDATE_MODE;
        (*ErrorCount)++;
      }
    }
  }

  return Status;
}

EFI_STATUS
SmmInitializeIpmiKcsPhysicalLayer (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
/*++

Routine Description:
  Setup and initialize the BMC for the DXE phase.  In order to verify the BMC is functioning
  as expected, the BMC Selftest is performed.  The results are then checked and any errors are
  reported to the error manager.  Errors are collected throughout this routine and reported
  just prior to installing the driver.  If there are more errors than MAX_SOFT_COUNT, then they
  will be ignored.

Arguments:
  ImageHandle - Handle of this driver image
  SystemTable - Table containing standard EFI services

Returns:
  EFI_SUCCESS - Successful driver initialization

--*/
{
  EFI_STATUS                       Status;
  UINT8                            ErrorCount;
  EFI_HANDLE                       Handle;
  EFI_STATUS_CODE_VALUE            StatusCodeValue[MAX_SOFT_COUNT];

  DEBUG ((DEBUG_INFO,"SmmInitializeIpmiKcsPhysicalLayer entry \n"));
  ErrorCount = 0;
  mImageHandle = ImageHandle;

  mIpmiInstance = AllocateZeroPool (sizeof (IPMI_BMC_INSTANCE_DATA));
  ASSERT (mIpmiInstance != NULL);
  if (mIpmiInstance == NULL) {
    DEBUG ((EFI_D_ERROR, "ERROR!! Null Pointer returned by AllocateZeroPool ()\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  } else {

    //
    // Initialize the KCS transaction timeout. Assume delay unit is 1000 us.
    //
    mIpmiInstance->KcsTimeoutPeriod = (BMC_KCS_TIMEOUT * 1000*1000) / KCS_DELAY_UNIT;

    //
    // Initialize IPMI IO Base, we still use SMS IO base to get device ID and Seltest result since SMM IF may have different cmds supported
    //
    mIpmiInstance->IpmiIoBase                       = PcdGet16 (PcdIpmiSmmIoBaseAddress);
    mIpmiInstance->Signature                        = SM_IPMI_BMC_SIGNATURE;
    mIpmiInstance->SlaveAddress                     = BMC_SLAVE_ADDRESS;
    mIpmiInstance->BmcStatus                        = BMC_NOTREADY;
    mIpmiInstance->IpmiTransport.IpmiSubmitCommand  = IpmiSendCommand;
    mIpmiInstance->IpmiTransport.GetBmcStatus       = IpmiGetBmcStatus;

    DEBUG ((DEBUG_INFO,"IPMI: Waiting for Getting BMC DID in SMM \n"));
    //
    // Get the Device ID and check if the system is in Force Update mode.
    //
    // Just obey the Spec..
    // If we want to improve performance, we're going to comment it.
    //
    Status = GetDeviceId (
               mIpmiInstance,
               StatusCodeValue,
               &ErrorCount
               );
    ASSERT_EFI_ERROR (Status);
    Handle = NULL;
    Status = gSmst->SmmInstallProtocolInterface (
                      &Handle,
                      &gSmmIpmiTransportProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mIpmiInstance->IpmiTransport
                      );
    ASSERT_EFI_ERROR (Status);

    DEBUG ((DEBUG_INFO,"SmmInitializeIpmiKcsPhysicalLayer exit \n"));

    return EFI_SUCCESS;
  }
}

EFI_STATUS
InitializeSmmGenericIpmi (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  SmmInitializeIpmiKcsPhysicalLayer (ImageHandle, SystemTable);
  return EFI_SUCCESS;
}
