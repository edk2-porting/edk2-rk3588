/** @file
  Implement EFI RealTimeClock runtime services via RTC Lib.

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.inf

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/TimeBaseLib.h>

#include <Protocol/RealTimeClock.h>

#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>

#include <Library/ArmArchTimer.h>

STATIC CONST CHAR16           mTimeZoneVariableName[] = L"PV660VirtualRtcTimeZone";
STATIC CONST CHAR16           mDaylightVariableName[] = L"PV660VirtualRtcDaylight";
STATIC EFI_EVENT              mRtcVirtualAddrChangeEvent;
STATIC EFI_RUNTIME_SERVICES   *mRT;


STATIC INTN mEpochDiff = 0;

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
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64      Temp;
  UINT32      EpochSeconds;
  INT16       TimeZone = 0;
  UINT8       Daylight = 0;
  UINTN       Size;

  // Ensure Time is a valid pointer
  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ArmArchTimerReadReg(CntPct,&Temp);

  // UINT32 force convertion for PC-LINT
  EpochSeconds = mEpochDiff + Temp / (UINT32) PcdGet32(PcdArmArchTimerFreqInHz);

  // Get the current time zone information from non-volatile storage
  Size = sizeof (TimeZone);
  Status = mRT->GetVariable (
                  (CHAR16 *)mTimeZoneVariableName,
                  &gEfiCallerIdGuid,
                  NULL,
                  &Size,
                  (VOID *)&TimeZone
                  );

  if (EFI_ERROR (Status)) {
    ASSERT(Status != EFI_INVALID_PARAMETER);
    ASSERT(Status != EFI_BUFFER_TOO_SMALL);

    if (Status != EFI_NOT_FOUND)
      goto EXIT;

    // The time zone variable does not exist in non-volatile storage, so create it.
    Time->TimeZone = EFI_UNSPECIFIED_TIMEZONE;
    // Store it
    Status = mRT->SetVariable (
                    (CHAR16 *)mTimeZoneVariableName,
                    &gEfiCallerIdGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    Size,
                    (VOID *)&(Time->TimeZone)
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "LibGetTime: Failed to save %s variable to non-volatile storage, Status = %r\n",
        mTimeZoneVariableName,
        Status
        ));
      goto EXIT;
    }
  } else {
    // Got the time zone
    Time->TimeZone = TimeZone;

    // Check TimeZone bounds:   -1440 to 1440 or 2047
    if (((Time->TimeZone < -1440) || (Time->TimeZone > 1440))
        && (Time->TimeZone != EFI_UNSPECIFIED_TIMEZONE)) {
      Time->TimeZone = EFI_UNSPECIFIED_TIMEZONE;
    }

    // Adjust for the correct time zone
    if (Time->TimeZone != EFI_UNSPECIFIED_TIMEZONE) {
      EpochSeconds += Time->TimeZone * SEC_PER_MIN;
    }
  }

  // Get the current daylight information from non-volatile storage
  Size = sizeof (Daylight);
  Status = mRT->GetVariable (
                  (CHAR16 *)mDaylightVariableName,
                  &gEfiCallerIdGuid,
                  NULL,
                  &Size,
                  (VOID *)&Daylight
                  );

  if (EFI_ERROR (Status)) {
    ASSERT(Status != EFI_INVALID_PARAMETER);
    ASSERT(Status != EFI_BUFFER_TOO_SMALL);

    if (Status != EFI_NOT_FOUND)
      goto EXIT;

    // The daylight variable does not exist in non-volatile storage, so create it.
    Time->Daylight = 0;
    // Store it
    Status = mRT->SetVariable (
                    (CHAR16 *)mDaylightVariableName,
                    &gEfiCallerIdGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    Size,
                    (VOID *)&(Time->Daylight)
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "LibGetTime: Failed to save %s variable to non-volatile storage, Status = %r\n",
        mDaylightVariableName,
        Status
        ));
      goto EXIT;
    }
  } else {
    // Got the daylight information
    Time->Daylight = Daylight;

    // Adjust for the correct period
    if ((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT) {
      // Convert to adjusted time, i.e. spring forwards one hour
      EpochSeconds += SEC_PER_HOUR;
    }
  }

  // Convert from internal 32-bit time to UEFI time
  EpochToEfiTime (EpochSeconds, Time);

  // Update the Capabilities info
  if (Capabilities != NULL) {
    Capabilities->Resolution  = 1;
    // Accuracy in ppm multiplied by 1,000,000, e.g. for 50ppm set 50,000,000

    Capabilities->Accuracy    = PcdGet32 (PcdArmArchTimerFreqInHz);
    // FALSE: Setting the time does not clear the values below the resolution level
    Capabilities->SetsToZero  = FALSE;
  }

  EXIT:
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
  IN  EFI_TIME                *Time
  )
{
  EFI_STATUS  Status;
  UINTN       EpochSeconds;
  UINTN       Temp;

  // Check the input parameters are within the range specified by UEFI
  if (!IsTimeValid (Time)) {
    Status = EFI_INVALID_PARAMETER;
    goto EXIT;
  }

  // Because the PL031 is a 32-bit counter counting seconds,
  // the maximum time span is just over 136 years.
  // Time is stored in Unix Epoch format, so it starts in 1970,
  // Therefore it can not exceed the year 2106.
  if ((Time->Year < 1970) || (Time->Year >= 2106)) {
    Status = EFI_UNSUPPORTED;
    goto EXIT;
  }

  EpochSeconds = EfiTimeToEpoch (Time);

  // Adjust for the correct time zone, i.e. convert to UTC time zone
  if (Time->TimeZone != EFI_UNSPECIFIED_TIMEZONE) {
    EpochSeconds -= Time->TimeZone * SEC_PER_MIN;
  }

  // TODO: Automatic Daylight activation

  // Adjust for the correct period
  if ((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT) {
    // Convert to un-adjusted time, i.e. fall back one hour
    EpochSeconds -= SEC_PER_HOUR;
  }

  ArmArchTimerReadReg(CntPct,&Temp);

  // UINT32 force convertion for PC-LINT
  mEpochDiff = EpochSeconds - Temp / (UINT32) PcdGet32(PcdArmArchTimerFreqInHz);

  // The accesses to Variable Services can be very slow, because we may be writing to Flash.
  // Do this after having set the RTC.

  // Save the current time zone information into non-volatile storage
  Status = mRT->SetVariable (
                  (CHAR16 *)mTimeZoneVariableName,
                  &gEfiCallerIdGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof (Time->TimeZone),
                  (VOID *)&(Time->TimeZone)
                  );
  if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "LibSetTime: Failed to save %s variable to non-volatile storage, Status = %r\n",
        mTimeZoneVariableName,
        Status
        ));
    goto EXIT;
  }

  // Save the current daylight information into non-volatile storage
  Status = mRT->SetVariable (
                  (CHAR16 *)mDaylightVariableName,
                  &gEfiCallerIdGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof(Time->Daylight),
                  (VOID *)&(Time->Daylight)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "LibSetTime: Failed to save %s variable to non-volatile storage, Status = %r\n",
      mDaylightVariableName,
      Status
      ));
    goto EXIT;
  }

  EXIT:
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
  EfiConvertPointer (0x0, (VOID**)&mRT);
  return;
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
  EFI_HANDLE    Handle;

  // Setup the setters and getters
  gRT->GetTime       = LibGetTime;
  gRT->SetTime       = LibSetTime;
  gRT->GetWakeupTime = LibGetWakeupTime;
  gRT->SetWakeupTime = LibSetWakeupTime;

  mRT = gRT;

  // Install the protocol
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiRealTimeClockArchProtocolGuid,  NULL,
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
