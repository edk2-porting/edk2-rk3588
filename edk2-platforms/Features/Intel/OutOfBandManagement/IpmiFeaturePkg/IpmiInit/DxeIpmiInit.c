/** @file
    IPMI stack initialization.

Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IpmiCommandLib.h>

#define BMC_TIMEOUT          30  // [s] How long shall BIOS wait for BMC
#define BMC_KCS_TIMEOUT      5   // [s] Single KSC request timeout

EFI_STATUS
GetSelfTest (
  VOID
  )
/*++

Routine Description:

  Execute the Get Self Test results command to determine whether or not the BMC self tests
  have passed

Arguments:

  mIpmiInstance   - Data structure describing BMC variables and used for sending commands
  StatusCodeValue - An array used to accumulate error codes for later reporting.
  ErrorCount      - Counter used to keep track of error codes in StatusCodeValue

Returns:

  EFI_SUCCESS       - BMC Self test results are retrieved and saved into BmcStatus
  EFI_DEVICE_ERROR  - BMC failed to return self test results.

--*/
{
  EFI_STATUS                       Status;
  IPMI_SELF_TEST_RESULT_RESPONSE   TestResult;

  //
  // Get the SELF TEST Results.
  //
  Status = IpmiGetSelfTestResult (&TestResult);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "\n[IPMI] BMC does not respond (status: %r)!\n\n", Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "[IPMI] BMC self-test result: %02X-%02X\n", TestResult.Result, TestResult.Param));

  return EFI_SUCCESS;
}

EFI_STATUS
GetDeviceId (
  OUT BOOLEAN *UpdateMode
  )
/*++

Routine Description:
  Execute the Get Device ID command to determine whether or not the BMC is in Force Update
  Mode.  If it is, then report it to the error manager.

Arguments:

Returns:
  Status

--*/
{
  EFI_STATUS                   Status;
  IPMI_GET_DEVICE_ID_RESPONSE  BmcInfo;
  UINT32                       Retries;

  //
  // Set up a loop to retry for up to 30 seconds. Calculate retries not timeout
  // so that in case KCS is not enabled and IpmiSendCommand() returns
  // immediately we will not wait all the 30 seconds.
  //
  Retries = BMC_TIMEOUT / BMC_KCS_TIMEOUT + 1;
  //
  // Get the device ID information for the BMC.
  //
  do {
    Status = IpmiGetDeviceId (&BmcInfo);
    if (!EFI_ERROR(Status)) {
      break;
    }
    DEBUG ((DEBUG_ERROR, "[IPMI] BMC does not respond (status: %r), %d retries left\n", Status, Retries));
    MicroSecondDelay(50 * 1000);
    if (Retries-- == 0) {
      return Status;
    }
  } while (TRUE);

  DEBUG((
    DEBUG_INFO,
    "[IPMI] BMC Device ID: 0x%02X, firmware version: %d.%02X\n",
    BmcInfo.DeviceId,
    BmcInfo.FirmwareRev1.Bits.MajorFirmwareRev,
    BmcInfo.MinorFirmwareRev
    ));
  *UpdateMode = (BOOLEAN)BmcInfo.FirmwareRev1.Bits.UpdateMode;
  return Status;
}

/**
  The entry point of the Ipmi DXE.

@param[in] ImageHandle - Handle of this driver image
@param[in] SystemTable - Table containing standard EFI services

@retval EFI_SUCCESS - Always success is returned even if KCS does not function

 **/
EFI_STATUS
EFIAPI
IpmiInterfaceInit (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  BOOLEAN      UpdateMode;
  EFI_STATUS   Status;

  DEBUG((DEBUG_ERROR,"IPMI Dxe:Get BMC Device Id\n"));

  //
  // Get the Device ID and check if the system is in Force Update mode.
  //
  Status = GetDeviceId (&UpdateMode);
  //
  // Do not continue initialization if the BMC is in Force Update Mode.
  //
  if (!EFI_ERROR(Status) && !UpdateMode) {
    //
    // Get the SELF TEST Results.
    //
    GetSelfTest ();
  }

  return EFI_SUCCESS;
}


