/** @file

  Copyright (c) 2020, Hisilicon Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/I2CLib.h>
#include <Library/RtcHelperLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/RealTimeClock.h>
#include "RX8900RealTimeClock.h"

extern I2C_DEVICE gRtcDevice;

STATIC BOOLEAN       mRX8900Initialized = FALSE;
STATIC CONST CHAR16  mTimeZoneVariableName[] = L"RX8900RtcTimeZone";
STATIC CONST CHAR16  mDaylightVariableName[] = L"RX8900RtcDaylight";

EFI_STATUS
InitializeRX8900 (
  VOID
  )
{
  EFI_STATUS    Status;
  unsigned char writeTemp;

  Status = SwitchRtcI2cChannelAndLock ();
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  Status = I2CInit (gRtcDevice.Socket, gRtcDevice.Port, Normal);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  writeTemp = RX8900_VDETOFF_SWOFF;
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_BACKUP_FUN, 1, &writeTemp);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }
  writeTemp = TEMPERATURE_COMPENSATION_2S;
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_CONTRLREG, 1, &writeTemp);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }
  writeTemp = OUTPUT_FREQUENCY_32768;
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_EXTENSIONREG, 1, &writeTemp);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  writeTemp = FLAG_REG_DEFAULT;
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_FLAGREG, 1, &writeTemp);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }
  writeTemp = RX8900_RAM_REG_DEFAULT;
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_RAM, 1, &writeTemp);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  mRX8900Initialized = TRUE;

  EXIT:

  ReleaseOwnershipOfRtc ();
  return Status;
}

STATIC
INT16
GetTimeZone (
  VOID
  )
{
  EFI_STATUS  Status;
  INT16       TimeZone;
  UINTN       Size;

  TimeZone = EFI_UNSPECIFIED_TIMEZONE;
  Size = sizeof (TimeZone);
  Status = EfiGetVariable (
             (CHAR16 *)mTimeZoneVariableName,
             &gEfiCallerIdGuid,
             NULL,
             &Size,
             (VOID *)&TimeZone
             );

  if (EFI_ERROR (Status)) {
    TimeZone = EFI_UNSPECIFIED_TIMEZONE;
    // The time zone variable does not exist in non-volatile storage, so create it.
    Status = EfiSetVariable (
               (CHAR16 *)mTimeZoneVariableName,
               &gEfiCallerIdGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               Size,
               (VOID *)&TimeZone
               );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Failed to save %s variable, Status = %r\n",
          mTimeZoneVariableName, Status));
    }
  } else {
    // Check TimeZone bounds:   -1440 to 1440 or 2047
    if ((TimeZone < -EFI_TIMEOFFSET_TIMEZONE) || (TimeZone > EFI_TIMEOFFSET_TIMEZONE)) {
      TimeZone = EFI_UNSPECIFIED_TIMEZONE;
    }
  }

  return TimeZone;
}

STATIC
UINT8
GetDayLight (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT8       DayLight;
  UINTN       Size;

  DayLight = 0;
  // Get the current daylight information from non-volatile storage
  Size = sizeof (DayLight);
  Status = EfiGetVariable (
             (CHAR16 *)mDaylightVariableName,
             &gEfiCallerIdGuid,
             NULL,
             &Size,
             (VOID *)&DayLight
             );

  if (EFI_ERROR (Status)) {
    DayLight = 0;
    // The daylight variable does not exist in non-volatile storage, so create it.
    Status = EfiSetVariable (
               (CHAR16 *)mDaylightVariableName,
               &gEfiCallerIdGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               Size,
               (VOID *)&DayLight
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed to save %s variable, Status = %r\n",
          mDaylightVariableName, Status));
    }
  }

  return DayLight;
}

EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME                *Time,
  OUT EFI_TIME_CAPABILITIES   *Capabilities
  )
{
  EFI_STATUS  Status;
  UINT8       Temp[7] = {0};
  UINT16      BaseYear = 2000;
  UINTN       EpochSeconds;
  UINT8       TryCount = 0;

  // Ensure Time is a valid pointer
  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Initialize the hardware if not already done
  if (!mRX8900Initialized) {
    Status = InitializeRX8900 ();
    if (EFI_ERROR (Status)) {
      return EFI_NOT_READY;
    }
  }

  Status = SwitchRtcI2cChannelAndLock ();
  if (EFI_ERROR (Status)) {
    ReleaseOwnershipOfRtc ();
    return Status;
  }

  do {
    Status = I2CRead (&gRtcDevice, RX8900_REGADDR_SECONDS, 7, Temp);
    if (EFI_ERROR (Status)) {
      Status = EFI_DEVICE_ERROR;
      goto Err;
    }

    Time->Second = BcdToDecimal8 (Temp[0]);
    Time->Minute = BcdToDecimal8 (Temp[1]);
    Time->Hour = BcdToDecimal8 (Temp[2]);
    Time->Day = BcdToDecimal8 (Temp[4]);
    Time->Month = BcdToDecimal8 (Temp[5]);
    Time->Year = BaseYear + BcdToDecimal8 (Temp[6]);
    Time->Nanosecond = 0;

    EpochSeconds = EfiTimeToEpoch (Time);

    Time->TimeZone = GetTimeZone ();
    // Adjust for the correct time zone
    if (Time->TimeZone != EFI_UNSPECIFIED_TIMEZONE) {
      EpochSeconds += Time->TimeZone * SEC_PER_MIN;
    }

    Time->Daylight = GetDayLight ();
    // Adjust for the correct period
    if ((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT) {
      // Convert to adjusted time, i.e. spring forwards one hour
      EpochSeconds += SEC_PER_HOUR;
    }

    // Convert from internal 32-bit time to UEFI time
    EpochToEfiTime (EpochSeconds, Time);
    if ((!IsTimeValid (Time)) || ((Time->Year - BaseYear) > 99) || (Time->Year < 2000)) {
      DEBUG ((DEBUG_INFO, "LibGetTime: %d-%d-%d %d-%d-%d EpochSeconds:%llx is invalid time!\n",
        Time->Second, Time->Minute, Time->Hour, Time->Day, Time->Month,
        Time->Year, EpochSeconds));
      Status = EFI_DEVICE_ERROR;
    }

Err:
    TryCount++;
  } while ((TryCount < 3) && (EFI_ERROR (Status)));

  ReleaseOwnershipOfRtc ();
  return Status;
}

STATIC
EFI_STATUS
SetTimeToRX8900 (
  IN  EFI_TIME                *Time
  )
{
  EFI_STATUS   Status;
  UINT8        Temp;

  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  Temp = DecimalToBcd8 (Time->Second);
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_SECONDS, 1, &Temp);
  if(EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    return Status;
  }

  Temp = DecimalToBcd8 (Time->Minute);
  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_MIUTES, 1, &Temp);
  if(EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    return Status;
  }

  Temp = DecimalToBcd8 (Time->Hour);
  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_HOURS, 1, &Temp);
  if(EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    return Status;
  }

  Temp = DecimalToBcd8 (Time->Day);
  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_DATE, 1, &Temp);
  if(EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    return Status;
  }

  Temp = DecimalToBcd8 (Time->Month);
  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_MONTH, 1, &Temp);
  if(EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    return Status;
  }

  Time->Year= Time->Year % 100;
  Temp = Time->Year;
  Temp = DecimalToBcd8 (Temp);
  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  Status = I2CWrite (&gRtcDevice, RX8900_REGADDR_YEAR, 1, &Temp);
  if(EFI_ERROR (Status)) {
    Status =  EFI_DEVICE_ERROR;
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
LibSetTime (
  IN  EFI_TIME                *Time
  )
{
  EFI_STATUS  Status;
  UINTN EpochSeconds;

  // Initialize the hardware if not already done
  if (!mRX8900Initialized) {
    Status = InitializeRX8900 ();
    if (EFI_ERROR (Status)) {
      goto EXIT;
    }
  }

  Status = SwitchRtcI2cChannelAndLock ();
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  if(!IsTimeValid(Time)){
    return EFI_INVALID_PARAMETER;
  }

  EpochSeconds = EfiTimeToEpoch (Time);

  // Adjust for the correct time zone, i.e. convert to UTC time zone
  if (Time->TimeZone != EFI_UNSPECIFIED_TIMEZONE) {
    EpochSeconds -= Time->TimeZone * SEC_PER_MIN;
  }

  // Adjust for the correct period
  if ((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT) {
    // Convert to un-adjusted time, i.e. fall back one hour
    EpochSeconds -= SEC_PER_HOUR;
  }

  EpochToEfiTime (EpochSeconds, Time);
  Status = SetTimeToRX8900 (Time);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  // Save the current time zone information into non-volatile storage
  Status = EfiSetVariable (
             (CHAR16 *)mTimeZoneVariableName,
             &gEfiCallerIdGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof (Time->TimeZone),
             (VOID *)&(Time->TimeZone)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "LibSetTime: Failed to save %s variable, Status = %r\n",
      mTimeZoneVariableName, Status));
    goto EXIT;
  }

  // Save the current daylight information into non-volatile storage
  Status = EfiSetVariable (
             (CHAR16 *)mDaylightVariableName,
             &gEfiCallerIdGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof(Time->Daylight),
             (VOID *)&(Time->Daylight)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "LibSetTime: Failed to save %s variable, Status = %r\n",
      mDaylightVariableName, Status));
    goto EXIT;
  }

  EXIT:
  ReleaseOwnershipOfRtc ();
  return Status;
}

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


EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    Handle;
  EFI_TIME      EfiTime;

  // Setup the setters and getters
  gRT->GetTime = LibGetTime;
  gRT->SetTime = LibSetTime;
  gRT->GetWakeupTime = LibGetWakeupTime;
  gRT->SetWakeupTime = LibSetWakeupTime;

  Status = gRT->GetTime (&EfiTime, NULL);
  if (EFI_ERROR (Status) || (EfiTime.Year < 2000) || (EfiTime.Year > 2099) ||
      (!IsTimeValid (&EfiTime))) {
       EfiTime.Year = 2000;
       EfiTime.Month = 1;
       EfiTime.Day = 1;
       EfiTime.Hour = 0;
       EfiTime.Minute = 0;
       EfiTime.Second = 0;
       EfiTime.Nanosecond = 0;
       EfiTime.Daylight = 0;
       EfiTime.TimeZone = EFI_UNSPECIFIED_TIMEZONE;

      Status = gRT->SetTime (&EfiTime);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "SetTime Status : %r\n", Status));
      }
  }

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiRealTimeClockArchProtocolGuid,
                  NULL,
                  NULL
                 );

  return Status;
}


VOID
EFIAPI
LibRtcVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  return;
}
