/** @file

  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2018, Linaro Limited. All rights reserved.<BR>

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
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "M41T83RealTimeClock.h"

extern I2C_DEVICE gRtcDevice;

STATIC EFI_LOCK  mRtcLock;

/**
  Read RTC content through its registers.

  @param  Address   Address offset of RTC data.
  @param  Size      Size of RTC data to read.
  @param  Data      The data of UINT8 type read from RTC.

  @return EFI_STATUS
**/
EFI_STATUS
RtcRead (
  IN  UINT8   Address,
  IN  UINT8   Size,
  OUT UINT8   *Data
  )
{
  EFI_STATUS  Status;

  Status = I2CRead (&gRtcDevice, Address, Size, Data);
  MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  return Status;
}

/**
  Write RTC through its registers.

  @param  Address   Address offset of RTC data.
  @param  Size      Size of RTC data to write.
  @param  Data      The data of UINT8 type write from RTC.

  @return EFI_STATUS
**/
EFI_STATUS
RtcWrite (
  IN  UINT8   Address,
  IN  UINT8   Size,
  UINT8       *Data
  )
{
  EFI_STATUS  Status;

  Status = I2CWrite (&gRtcDevice, Address, Size, Data);
  MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  return Status;
}

EFI_STATUS
InitializeM41T83 (
  VOID
  )
{
  EFI_STATUS                Status;
  RTC_M41T83_ALARM1HOUR     Alarm1Hour;
  RTC_M41T83_SECOND         Second;

  // Acquire RTC Lock to make access to RTC atomic
  if (!EfiAtRuntime ()) {
    EfiAcquireLock (&mRtcLock);
  }

  Status = I2CInit (gRtcDevice.Socket, gRtcDevice.Port, Normal);
  MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);
  if (EFI_ERROR (Status)) {
    if (!EfiAtRuntime ()) {
      EfiReleaseLock (&mRtcLock);
    }
    return Status;
  }

  Status = SwitchRtcI2cChannelAndLock ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get i2c preemption failed: %r\n", Status));
    if (!EfiAtRuntime ()) {
      EfiReleaseLock (&mRtcLock);
    }
    return Status;
  }

  MicroSecondDelay(RTC_DELAY_1000_MICROSECOND);

  // Set ST at Power up to clear Oscillator fail detection(OF)
  Status = RtcRead (M41T83_REGADDR_SECONDS, 1, &Second.Uint8);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
      __FUNCTION__, __LINE__, Status));
  }
  Second.Bits.ST= 1;
  Status = RtcWrite (M41T83_REGADDR_SECONDS, 1, &Second.Uint8);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
      __FUNCTION__, __LINE__, Status));
    goto Exit;
  }
  Status = RtcRead (M41T83_REGADDR_SECONDS, 1, &Second.Uint8);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
      __FUNCTION__, __LINE__, Status));
  }
  Second.Bits.ST= 0;
  Status = RtcWrite (M41T83_REGADDR_SECONDS, 1, &Second.Uint8);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
      __FUNCTION__, __LINE__, Status));
    goto Exit;
  }

  // Clear HT bit to enanle write to the RTC registers (addresses 0-7)
  Status = RtcRead (M41T83_REGADDR_ALARM1HOUR, 1, &Alarm1Hour.Uint8);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
      __FUNCTION__, __LINE__, Status));
  }
  Alarm1Hour.Bits.HT = 0;
  Status = RtcWrite (M41T83_REGADDR_ALARM1HOUR, 1, &Alarm1Hour.Uint8);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
      __FUNCTION__, __LINE__, Status));
    goto Exit;
  }

Exit:
  // Release RTC Lock.
  ReleaseOwnershipOfRtc ();
  if (!EfiAtRuntime ()) {
    EfiReleaseLock (&mRtcLock);
  }
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
  EFI_STATUS                  Status = EFI_SUCCESS;
  RTC_M41T83_TIME             BcdTime;
  UINT16                      CenturyBase = 2000;
  UINTN                       LineNum = 0;

  if (NULL == Time) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsTimeValid (Time)) {
    if (!EfiAtRuntime ()) {
      DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
        __FUNCTION__, __LINE__, Status));
      DEBUG ((DEBUG_ERROR, "Now RTC Time is : %04d-%02d-%02d %02d:%02d:%02d\n",
        Time->Year, Time->Month, Time->Day, Time->Hour, Time->Minute, Time->Second
      ));
    }
    return EFI_INVALID_PARAMETER;
  }

  Status = SwitchRtcI2cChannelAndLock ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  (VOID)MicroSecondDelay (RTC_DELAY_1000_MICROSECOND);

  SetMem (&BcdTime, sizeof (RTC_M41T83_TIME), 0);

  // Acquire RTC Lock to make access to RTC atomic
  if (!EfiAtRuntime ()) {
    EfiAcquireLock (&mRtcLock);
  }

  BcdTime.Second.Bits.Seconds = DecimalToBcd8 (Time->Second);
  BcdTime.Minute.Bits.Minutes = DecimalToBcd8 (Time->Minute);
  BcdTime.Hour.Bits.Hours = DecimalToBcd8 (Time->Hour);
  BcdTime.Day.Bits.Days = DecimalToBcd8 (Time->Day);
  BcdTime.Month.Bits.Months = DecimalToBcd8 (Time->Month);
  BcdTime.Year.Bits.Years = DecimalToBcd8 (Time->Year % 100);
  BcdTime.Hour.Bits.CB = (Time->Year - CenturyBase) / 100 % 10;

  Status = RtcWrite (M41T83_REGADDR_DOTSECONDS, 1, &BcdTime.DotSecond.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }
  Status = RtcWrite (M41T83_REGADDR_SECONDS, 1, &BcdTime.Second.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }
  Status = RtcWrite (M41T83_REGADDR_MINUTES, 1, &BcdTime.Minute.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }
  Status = RtcWrite (M41T83_REGADDR_HOURS, 1, &BcdTime.Hour.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }
  Status = RtcWrite (M41T83_REGADDR_DAY, 1, &BcdTime.Day.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }
  Status = RtcWrite (M41T83_REGADDR_MONTH, 1, &BcdTime.Month.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }
  Status = RtcWrite (M41T83_REGADDR_YEAR, 1, &BcdTime.Year.Uint8);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }

Exit:
  ReleaseOwnershipOfRtc ();
  // Release RTC Lock.
  if (!EfiAtRuntime ()) {
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
        __FUNCTION__, LineNum, Status));
    }
    EfiReleaseLock (&mRtcLock);
  }
  return Status;
}


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
  EFI_STATUS                  Status = EFI_SUCCESS;
  RTC_M41T83_TIME             BcdTime;
  UINT16                      CenturyBase = 2000;
  UINTN                       LineNum = 0;
  BOOLEAN                     IsTimeInvalid = FALSE;
  UINT8                       TimeTemp[7] = {0};

  // Ensure Time is a valid pointer
  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SwitchRtcI2cChannelAndLock ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay(RTC_DELAY_1000_MICROSECOND);

  SetMem (&BcdTime, sizeof (RTC_M41T83_TIME), 0);
  SetMem (Time , sizeof (EFI_TIME), 0);

  // Acquire RTC Lock to make access to RTC atomic
  if (!EfiAtRuntime ()) {
    EfiAcquireLock (&mRtcLock);
  }

  Status = RtcRead (M41T83_REGADDR_SECONDS, 7, TimeTemp);
  if (EFI_ERROR (Status)) {
    LineNum = __LINE__;
    goto Exit;
  }

  BcdTime.Second.Uint8 = TimeTemp[0];  //SECONDS
  BcdTime.Minute.Uint8 = TimeTemp[1];  //MINUTES
  BcdTime.Hour.Uint8 = TimeTemp[2];    //HOURS
  BcdTime.Day.Uint8 = TimeTemp[4];     //DAY
  BcdTime.Month.Uint8 = TimeTemp[5];   //MONTH
  BcdTime.Year.Uint8 = TimeTemp[6];    //Year

  Time->Year = BcdToDecimal8 (BcdTime.Year.Bits.Years);
  Time->Year += CenturyBase + BcdTime.Hour.Bits.CB * 100;
  Time->Month = BcdToDecimal8 (BcdTime.Month.Bits.Months);
  Time->Day = BcdToDecimal8 (BcdTime.Day.Bits.Days);
  Time->Hour = BcdToDecimal8 (BcdTime.Hour.Bits.Hours);
  Time->Minute = BcdToDecimal8 (BcdTime.Minute.Bits.Minutes);
  Time->Second = BcdToDecimal8 (BcdTime.Second.Bits.Seconds);
  Time->TimeZone = EFI_UNSPECIFIED_TIMEZONE;

  if (!IsTimeValid (Time)) {
      Status = EFI_DEVICE_ERROR;
      LineNum = __LINE__;
      IsTimeInvalid = TRUE;
      goto Exit;
  }

Exit:
  ReleaseOwnershipOfRtc ();
  // Release RTC Lock.
  if (!EfiAtRuntime ()) {
    if (EFI_ERROR (Status)) {
      if (IsTimeInvalid == TRUE) {
        DEBUG((DEBUG_ERROR, "%a(%d) Time invalid.\r\n",__FUNCTION__, LineNum));
      } else {
        DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n",
          __FUNCTION__, LineNum, Status));
      }
    }
    EfiReleaseLock (&mRtcLock);
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
  IN  BOOLEAN      Enabled,
  OUT EFI_TIME     *Time
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
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_TIME      EfiTime;

  EfiInitializeLock (&mRtcLock, TPL_CALLBACK);

  // Setup the setters and getters
  gRT->GetTime       = LibGetTime;
  gRT->SetTime       = LibSetTime;
  gRT->GetWakeupTime = LibGetWakeupTime;
  gRT->SetWakeupTime = LibSetWakeupTime;

  Status = InitializeM41T83 ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\nRTC M41T83 Init Failed !!!\n",
            __FUNCTION__, __LINE__, Status));
    /*
     * Returning ERROR on failure of RTC initilization will cause the system to hang up.
     * So we add some debug message to indecate the RTC initilization failed,
     * and continue without returning with error to avoid system hanging up.
     *
     *return Status;
     */
  }

  LibGetTime (&EfiTime, NULL);
  if (!IsTimeValid (&EfiTime)) {
    EfiTime.Year    = 2015;
    EfiTime.Month   = 1;
    EfiTime.Day     = 1;
    EfiTime.Hour    = 0;
    EfiTime.Minute  = 0;
    EfiTime.Second  = 0;
    Status = LibSetTime (&EfiTime);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a]:[%dL] RTC settime Status : %r\n",
        __FUNCTION__, __LINE__, Status));
    }
  }

  DEBUG ((
    DEBUG_ERROR, "Now RTC Time is : %04d-%02d-%02d %02d:%02d:%02d\n",
    EfiTime.Year, EfiTime.Month, EfiTime.Day, EfiTime.Hour, EfiTime.Minute,
    EfiTime.Second
    ));
    /*
     * Returning ERROR on failure of RTC initilization will cause the system to hang up.
     * So we add some debug message to indecate the RTC initilization failed,
     * and return success to avoid system hanging up.
     */
  return EFI_SUCCESS;
}
