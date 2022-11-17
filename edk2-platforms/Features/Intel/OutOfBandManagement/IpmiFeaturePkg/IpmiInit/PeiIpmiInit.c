/** @file
  IPMI stack initialization in PEI.

Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IpmiCommandLib.h>

#define BMC_TIMEOUT_PEI      50  // [s] How long shall BIOS wait for BMC
#define BMC_KCS_TIMEOUT      5   // [s] Single KSC request timeout

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
  Retries = BMC_TIMEOUT_PEI/ BMC_KCS_TIMEOUT + 1;
  //
  // Get the device ID information for the BMC.
  //
  do {
    Status = IpmiGetDeviceId (&BmcInfo);
    if (!EFI_ERROR(Status)) {
      break;
    }
    DEBUG ((DEBUG_ERROR, "[IPMI] BMC does not respond (status: %r), %d retries left\n", Status, Retries));
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
  The entry point of the Ipmi PEIM.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   Indicates that Ipmi initialization completed successfully.
  @retval Others        Indicates that Ipmi initialization could not complete successfully.
**/
EFI_STATUS
EFIAPI
PeimIpmiInterfaceInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  BOOLEAN      UpdateMode;
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "IPMI Peim:Get BMC Device Id\n"));

  //
  // Get the Device ID and check if the system is in Force Update mode.
  //
  Status = GetDeviceId (&UpdateMode);
  return Status;
}
