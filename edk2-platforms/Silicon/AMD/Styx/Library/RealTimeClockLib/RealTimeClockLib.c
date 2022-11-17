/** @file
  Implement EFI RealTimeClock runtime services via RTC Lib.

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
/**
  Derived from:
   ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.c

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Guid/EventGroup.h>

#include <Protocol/AmdIscpDxeProtocol.h>
#include <Iscp.h>

extern  EFI_BOOT_SERVICES       *gBS;

AMD_ISCP_DXE_PROTOCOL *mRtcIscpDxeProtocol = NULL;
STATIC EFI_EVENT     mRtcVirtualAddrChangeEvent;


/**
  Returns the current time and date information, and the time-keeping capabilities
  of the hardware platform.

  @param  Time                  A pointer to storage to receive a snapshot of the current time.
  @param  Capabilities          An optional pointer to a buffer to receive the real time clock
                                device's capabilities.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER Time is NULL.
  @retval EFI_DEVICE_ERROR      The time could not be retrieved due to hardware error.

**/
EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME                *Time,
  OUT  EFI_TIME_CAPABILITIES  *Capabilities
  )
{
  ISCP_RTC_INFO  RtcInfo;
  EFI_STATUS     Status;

  if (mRtcIscpDxeProtocol == NULL) {
      DEBUG((EFI_D_ERROR, "RTC: ISCP DXE Protocol is NULL!\n"));
      return EFI_DEVICE_ERROR;
  }

  //
  // Fill in Time and Capabilities via data from you RTC
  //
  Status = mRtcIscpDxeProtocol->AmdExecuteGetRtc (
                                  mRtcIscpDxeProtocol,
                                  &RtcInfo
                                  );
  if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "RTC: Failed GetRtc() via ISCP - Status = %r \n", Status));
      return Status;
  }

  Time->Year = RtcInfo.Year;
  Time->Month = RtcInfo.Month;
  Time->Day = RtcInfo.Day;
  Time->Hour = RtcInfo.Hour;
  Time->Minute = RtcInfo.Minute;
  Time->Second = RtcInfo.Second;

  return Status;
}


/**
  Sets the current local time and date information.

  @param  Time                  A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due due to hardware error.

**/
EFI_STATUS
EFIAPI
LibSetTime (
  IN EFI_TIME                *Time
  )
{
  EFI_STATUS     Status;
  ISCP_RTC_INFO  RtcInfo;

  //
  // Use Time, to set the time in your RTC hardware
  //
  RtcInfo.Year = Time->Year;
  RtcInfo.Month = Time->Month;
  RtcInfo.Day = Time->Day;
  RtcInfo.Hour = Time->Hour;
  RtcInfo.Minute = Time->Minute;
  RtcInfo.Second = Time->Second;

  if (mRtcIscpDxeProtocol == NULL) {
      DEBUG((EFI_D_ERROR, "RTC: ISCP DXE Protocol is NULL!\n"));
      return EFI_DEVICE_ERROR;
  }

  Status = mRtcIscpDxeProtocol->AmdExecuteSetRtc (
                                  mRtcIscpDxeProtocol,
                                  &RtcInfo
                                  );
  if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "RTC: Failed SetRtc() via ISCP - Status = %r \n", Status));
      return Status;
  }

  return Status;
}


/**
  Returns the current wakeup alarm clock setting.

  @param  Enabled               Indicates if the alarm is currently enabled or disabled.
  @param  Pending               Indicates if the alarm signal is pending and requires acknowledgement.
  @param  Time                  The current alarm setting.

  @retval EFI_SUCCESS           The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER Any parameter is NULL.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error.

**/
EFI_STATUS
EFIAPI
LibGetWakeupTime (
  OUT BOOLEAN     *Enabled,
  OUT BOOLEAN     *Pending,
  OUT EFI_TIME    *Time
  )
{
  // Not a required feature
  return EFI_UNSUPPORTED;
}


/**
  Sets the system wakeup alarm clock time.

  @param  Enabled               Enable or disable the wakeup alarm.
  @param  Time                  If Enable is TRUE, the time to set the wakeup alarm for.

  @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If
                                Enable is FALSE, then the wakeup alarm was disabled.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error.
  @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform.

**/
EFI_STATUS
EFIAPI
LibSetWakeupTime (
  IN BOOLEAN      Enabled,
  OUT EFI_TIME    *Time
  )
{
  // Not a required feature
  return EFI_UNSUPPORTED;
}



/**
  This is the declaration of an EFI image entry point. This can be the entry point to an application
  written to this specification, an EFI boot service driver, or an EFI runtime driver.

  @param  ImageHandle           Handle that identifies the loaded image.
  @param  SystemTable           System Table for this image.

  @retval EFI_SUCCESS           The operation completed successfully.

**/
EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS    Status;

  //
  // Do some initialization if required to turn on the RTC
  //
  Status = gBS->LocateProtocol (
               &gAmdIscpDxeProtocolGuid,
               NULL,
               (VOID **)&mRtcIscpDxeProtocol
               );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "RTC: Failed to Locate ISCP DXE Protocol - Status = %r \n", Status));
    return Status;
  }

  //
  // Register for the virtual address change event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  LibRtcVirtualNotifyEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mRtcVirtualAddrChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
VOID
EFIAPI
LibRtcVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  //
  // Only needed if you are going to support the OS calling RTC functions in virtual mode.
  // You will need to call EfiConvertPointer (). To convert any stored physical addresses
  // to virtual address. After the OS transistions to calling in virtual mode, all future
  // runtime calls will be made in virtual mode.
  //
  EfiConvertPointer (0x0, (VOID**)&mRtcIscpDxeProtocol);
}



