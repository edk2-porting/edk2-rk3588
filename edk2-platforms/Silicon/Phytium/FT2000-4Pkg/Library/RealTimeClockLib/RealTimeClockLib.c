/** @file
  Implement EFI RealTimeClock runtime services via RTC Lib.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/RealTimeClock.h>
#include "RealTimeClockLib.h"

STATIC EFI_EVENT    mRtcVirtualAddrChangeEvent;
STATIC UINTN        mRtcBase;
STATIC CONST CHAR16   mTimeZoneVariableName[] = L"RtcTimeZone";
STATIC CONST CHAR16   mDaylightVariableName[] = L"RtcDaylight";

/**
  Returns the current time and date information, and the time-keeping capabilities
  of the hardware platform.

  @param  Time                   A pointer to storage to receive a snapshot of the current time.
  @param  Capabilities           An optional pointer to a buffer to receive the real time clock
                                 device's capabilities.

  @retval EFI_SUCCESS            The operation completed successfully.
  @retval EFI_INVALID_PARAMETER  Time is NULL.
  @retval EFI_DEVICE_ERROR       The time could not be retrieved due to hardware error.
  @retval EFI_SECURITY_VIOLATION The time could not be retrieved due to an authentication failure.

**/
EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME                *Time,
  OUT EFI_TIME_CAPABILITIES   *Capabilities
  )
{
  UINT32      EpochSeconds;
  INT16       TimeZone;
  UINT8       Daylight;
  UINTN       Size;
  EFI_STATUS  Status;

  // Ensure Time is a valid pointer
  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 (mRtcBase + RTC_AES_SEL, 0x100);
  //
  //read cdr high 32bit
  //
  EpochSeconds = MmioRead32 (mRtcBase + RTC_CCVR);
  MmioRead32 (mRtcBase + RTC_CDR_LOW);
  //
  // Get the current time zone information from non-volatile storage
  //
  Size = sizeof (TimeZone);
  Status = EfiGetVariable (
    (CHAR16 *)mTimeZoneVariableName,
    &gEfiCallerIdGuid,
    NULL,
    &Size,
    (VOID *)&TimeZone
    );

  if (EFI_ERROR (Status)) {
    ASSERT (Status != EFI_INVALID_PARAMETER);
    ASSERT (Status != EFI_BUFFER_TOO_SMALL);
    //
    // The time zone variable does not exist in non-volatile storage, so create it.
    //UTC+8:00
    //
    Time->TimeZone = -480;
    //
    // Store it
    //
    Status = EfiSetVariable (
      (CHAR16 *)mTimeZoneVariableName,
      &gEfiCallerIdGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      Size,
      (VOID *)&(Time->TimeZone)
      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    //
    // Got the time zone
    //
    Time->TimeZone = TimeZone;
    //
    // Check TimeZone bounds: -1440 to 1440 or 2047
    //
    if(!IsValidTimeZone(Time->TimeZone)) {
      Time->TimeZone = EFI_UNSPECIFIED_TIMEZONE;
    }
    //
    // Adjust for the correct time zone
    //
    if (IsValidTimeZone(Time->TimeZone)) {
      EpochSeconds -= Time->TimeZone * SEC_PER_MIN;
    }
  }
  //
  // Get the current daylight information from non-volatile storage
  //
  Size = sizeof (Daylight);
  Status = EfiGetVariable (
    (CHAR16 *)mDaylightVariableName,
    &gEfiCallerIdGuid,
    NULL,
    &Size,
    (VOID *)&Daylight
    );

  if (EFI_ERROR (Status)) {
    ASSERT (Status != EFI_INVALID_PARAMETER);
    ASSERT (Status != EFI_BUFFER_TOO_SMALL);
    //
    // The daylight variable does not exist in non-volatile storage, so create it.
    //
    Time->Daylight = 0;
    //
    // Store it
    //
    Status = EfiSetVariable (
      (CHAR16 *)mDaylightVariableName,
      &gEfiCallerIdGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      Size,
      (VOID *)&(Time->Daylight)
      );

    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    //
    // Got the daylight information
    //
    Time->Daylight = Daylight;
    //
    // Adjust for the correct period
    //
    if ((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT) {
      //
      // Convert to adjusted time, i.e. spring forwards one hour
      //
      EpochSeconds += SEC_PER_HOUR;
    }
  }

  //
  // Convert from internal 32-bit time to UEFI time
  //
  EpochToEfiTime (EpochSeconds, Time);

  return EFI_SUCCESS;
}


/**
  Sets the current local time and date information.

  @param[in]  Time                  A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due due to hardware error.

**/
EFI_STATUS
EFIAPI
LibSetTime (
  IN  EFI_TIME                *Time
  )
{
  UINTN    EpochSeconds;
  EFI_STATUS  Status;
  //
  // the maximum time span is just over 136 years.
  // Time is stored in Unix Epoch format, so it starts in 1970,
  // Therefore it can not exceed the year 2106.
  //
  if ((Time->Year < 1970) || (Time->Year >= 2106)) {
    return EFI_UNSUPPORTED;
  }
  EpochSeconds = EfiTimeToEpoch (Time);
  //
  // Adjust for the correct time zone, i.e. convert to UTC time zone
  //
  if (IsValidTimeZone(Time->TimeZone)) {
    EpochSeconds += Time->TimeZone * SEC_PER_MIN;
  }
  //
  // Adjust for the correct period
  //
  if (((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT)
    && (EpochSeconds > SEC_PER_HOUR)) {
    //
    // Convert to un-adjusted time, i.e. fall back one hour
    //
    EpochSeconds -= SEC_PER_HOUR;
  }
  //
  // Set the Rtc
  //
  MmioWrite32 (mRtcBase + RTC_AES_SEL, 0x100);
  MmioWrite32 (mRtcBase + RTC_CLR_LOW, 0x0);
  MmioWrite32 (mRtcBase + RTC_CLR, (UINT32)EpochSeconds);
  //
  // Save the current time zone information into non-volatile storage
  //
  Status = EfiSetVariable (
    (CHAR16 *)mTimeZoneVariableName,
    &gEfiCallerIdGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof (Time->TimeZone),
    (VOID *)&(Time->TimeZone)
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Save the current daylight information into non-volatile storage
  //
  Status = EfiSetVariable (
    (CHAR16 *)mDaylightVariableName,
    &gEfiCallerIdGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof (Time->Daylight),
    (VOID *)&(Time->Daylight)
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}


/**
  Returns the current wakeup alarm clock setting.

  @param[out]  Enabled               Indicates if the alarm is currently enabled or disabled.
  @param[out]  Pending               Indicates if the alarm signal is pending and requires acknowledgement.
  @param[out]  Time                  The current alarm setting.

  @retval      EFI_SUCCESS           The alarm settings were returned.
  @retval      EFI_INVALID_PARAMETER Any parameter is NULL.
  @retval      EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error.

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

  @param[in]   Enabled               Enable or disable the wakeup alarm.
  @param[out]  Time                  If Enable is TRUE, the time to set the wakeup alarm for.

  @retval      EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If
                                     Enable is FALSE, then the wakeup alarm was disabled.
  @retval      EFI_INVALID_PARAMETER A time field is out of range.
  @retval      EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error.
  @retval      EFI_UNSUPPORTED       A wakeup timer is not supported on this platform.

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
  // to virtual address. After the OS transitions to calling in virtual mode, all future
  // runtime calls will be made in virtual mode.
  //
  EfiConvertPointer (0x0, (VOID **)&mRtcBase);

  return;
}

/**
  This is the declaration of an EFI image entry point. This can be the entry point to an application
  written to this specification, an EFI boot service driver, or an EFI runtime driver.

  @param[in]  ImageHandle           Handle that identifies the loaded image.
  @param[in]  SystemTable           System Table for this image.

  @retval     EFI_SUCCESS           The operation completed successfully.

**/
EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE      Handle;
  INT16         TimeZone;
  UINTN         Size;
  EFI_TIME    Time;
  UINT8     Daylight;
  //
  // Initialize RTC Base Address
  //
  mRtcBase = PcdGet32 (PcdRtcBaseAddress);
  //
  // Declare the controller as EFI_MEMORY_RUNTIME
  //
  Status = gDS->AddMemorySpace (
    EfiGcdMemoryTypeMemoryMappedIo,
    mRtcBase,
    SIZE_4KB,
    EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
    );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  //init timezone
  //
  Size = sizeof (TimeZone);
  Status = EfiGetVariable (
    (CHAR16 *)mTimeZoneVariableName,
    &gEfiCallerIdGuid,
    NULL,
    &Size,
    (VOID *)&TimeZone
    );
  if (EFI_ERROR (Status)) {
    ASSERT (Status != EFI_INVALID_PARAMETER);
    ASSERT (Status != EFI_BUFFER_TOO_SMALL);
    //
    // The time zone variable does not exist in non-volatile storage, so create it.
    //UTC 8:00
    //
    Time.TimeZone = -480;
    //
    // Store it
    //
    Status = EfiSetVariable (
      (CHAR16 *)mTimeZoneVariableName,
      &gEfiCallerIdGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      Size,
      (VOID *)&(Time.TimeZone)
      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  //
  //daylight init
  //
  Size = sizeof (Daylight);
  Status = EfiGetVariable (
    (CHAR16 *)mDaylightVariableName,
    &gEfiCallerIdGuid,
    NULL,
    &Size,
    (VOID *)&Daylight
    );
  if (EFI_ERROR (Status)) {
    ASSERT (Status != EFI_INVALID_PARAMETER);
    ASSERT (Status != EFI_BUFFER_TOO_SMALL);
    //
    // The daylight variable does not exist in non-volatile storage, so create it.
    //
    Time.Daylight = 0;
    //
    // Store it
    //
    Status = EfiSetVariable (
      (CHAR16 *)mDaylightVariableName,
      &gEfiCallerIdGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      Size,
      (VOID *)&(Time.Daylight)
      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = gDS->SetMemorySpaceAttributes (mRtcBase, SIZE_4KB, EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Install the protocol
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
    &Handle,
    &gEfiRealTimeClockArchProtocolGuid,
    NULL,
    NULL
    );
  ASSERT_EFI_ERROR (Status);
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
