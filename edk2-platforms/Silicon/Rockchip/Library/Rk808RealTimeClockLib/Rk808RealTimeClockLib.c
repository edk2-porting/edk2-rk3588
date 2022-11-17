/** @file
  Implement EFI RealTimeClock runtime services via RTC Lib.

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Rockchip Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/TimeBaseLib.h>

#include <Protocol/RealTimeClock.h>

#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>

#include <Library/I2CLib.h>
#include <Rk808.h>

#define BCD2BIN(Val)	(((Val) & 0x0f) + ((Val) >> 4) * 10)
#define BIN2BCD(Val)	((((Val) / 10) << 4) + (Val) % 10)

#define SECONDS_REG_MSK		0x7F
#define MINUTES_REG_MAK		0x7F
#define HOURS_REG_MSK		0x3F
#define DAYS_REG_MSK		0x3F
#define MONTHS_REG_MSK		0x1F
#define YEARS_REG_MSK		0xFF
#define WEEKS_REG_MSK		0x7

#define BIT_RTC_CTRL_REG_STOP_RTC_M		0x01

#define NUM_TIME_REGS	(RK808_WEEKS_REG - RK808_SECONDS_REG + 1)
#define NUM_ALARM_REGS	(RK808_ALARM_YEARS_REG - RK808_ALARM_SECONDS_REG + 1)

I2C_DEVICE gDSRk808RtcDevice = {
    .BusID = I2C_CH0,
    .Speed = 200000,
    .SlaveDeviceAddress = 0x1b
};

STATIC BOOLEAN                mRk808Initialized = FALSE;

EFI_STATUS
Rk808I2CInit (
  VOID
  )
{
  EFI_STATUS	  Status;
  I2C_DEVICE	  Dev;

  (VOID) CopyMem(&Dev, &gDSRk808RtcDevice, sizeof(Dev));

  Status = I2CInit(Dev.BusID, Dev.Speed);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

EXIT:
  return Status;
}

EFI_STATUS
Rk808I2CRead (
  UINT32 Reg,
  UINT32 RLen,
  UINT8 *Buf,
  UINT32 BLen
  )
{
  EFI_STATUS		Status;
  I2C_DEVICE		Dev;

  (VOID) CopyMem(&Dev, &gDSRk808RtcDevice, sizeof(Dev));

  Status = I2CRead(Dev.BusID, Dev.SlaveDeviceAddress, Reg, RLen, Buf, BLen);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

EXIT:
  return EFI_SUCCESS;
}

EFI_STATUS
Rk808I2CWrite (
  UINT32 Reg,
  UINT32 RLen,
  UINT8 *Buf,
  UINT32 BLen
  )
{
  EFI_STATUS		  Status;
  I2C_DEVICE		  Dev;

  (VOID) CopyMem(&Dev, &gDSRk808RtcDevice, sizeof(Dev));

  Status = I2CWrite(Dev.BusID, Dev.SlaveDeviceAddress, Reg, RLen, Buf, BLen);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

EXIT:
  return EFI_SUCCESS;
}

EFI_STATUS
IdentifyRk808 (
  VOID
  )
{
  EFI_STATUS    Status;

  Status = EFI_SUCCESS;

  return Status;
}

EFI_STATUS
InitializeRk808 (
  VOID
  )
{
  EFI_STATUS    Status;

  // Prepare the hardware
  Status = IdentifyRk808();
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  Rk808I2CInit();

  mRk808Initialized = TRUE;

  EXIT:
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
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       RtcData[NUM_TIME_REGS];
  
  // Ensure Time is a valid pointer
  if (NULL == Time) {
    return EFI_INVALID_PARAMETER;
  }

  // Initialize the hardware if not already done
  if (!mRk808Initialized) {
    Status = InitializeRk808 ();
    if (EFI_ERROR (Status)) {
      return EFI_NOT_READY;
    }
  }

  Rk808I2CRead(RK808_SECONDS_REG, 1, &RtcData[0], NUM_TIME_REGS);

  Time->Second = BCD2BIN(RtcData[0] & SECONDS_REG_MSK);
  Time->Minute = BCD2BIN(RtcData[1] & MINUTES_REG_MAK);
  Time->Hour = BCD2BIN(RtcData[2] & HOURS_REG_MSK);
  Time->Day = BCD2BIN(RtcData[3] & DAYS_REG_MSK);
  Time->Month = BCD2BIN(RtcData[4] & MONTHS_REG_MSK) - 1;
  Time->Year = BCD2BIN(RtcData[5] & YEARS_REG_MSK) + 2000;
  Time->Nanosecond  = 0;
  Time->Daylight    = 0;
  Time->TimeZone    = EFI_UNSPECIFIED_TIMEZONE;

  if((EFI_ERROR(Status)) || (!IsTimeValid(Time))) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       RtcData[NUM_TIME_REGS];
  UINT8				Ret, RtcCtl;

  // Check the input parameters are within the range specified by UEFI
  if(!IsTimeValid(Time)){
    return EFI_INVALID_PARAMETER;
  }

  // Initialize the hardware if not already done
  if (!mRk808Initialized) {
    Status = InitializeRk808 ();
    if (EFI_ERROR (Status)) {
      goto EXIT;
    }
  }

  RtcData[0] = BIN2BCD(Time->Second);
	RtcData[1] = BIN2BCD(Time->Minute);
	RtcData[2] = BIN2BCD(Time->Hour);
	RtcData[3] = BIN2BCD(Time->Day);
	RtcData[4] = BIN2BCD(Time->Month + 1);
	RtcData[5] = BIN2BCD(Time->Year - 2000);

	/*Dummy read*/	
	Rk808I2CRead(RK808_RTC_CTRL_REG, 1, &Ret, 1);
	
	/* Stop RTC while updating the TC registers */
	Rk808I2CRead(RK808_RTC_CTRL_REG, 1, &Ret, 1);

	RtcCtl = Ret | (BIT_RTC_CTRL_REG_STOP_RTC_M);
  Rk808I2CWrite(RK808_RTC_CTRL_REG, 1, &RtcCtl, 1);

  Rk808I2CWrite(RK808_SECONDS_REG, 1, &RtcData[0], NUM_TIME_REGS);
  
	/*Dummy read*/	
	Rk808I2CRead(RK808_RTC_CTRL_REG, 1, &Ret, 1);
	
	/* Start RTC again */
	Rk808I2CRead(RK808_RTC_CTRL_REG, 1, &Ret, 1);

	RtcCtl = Ret & (~ BIT_RTC_CTRL_REG_STOP_RTC_M);
  Rk808I2CWrite(RK808_RTC_CTRL_REG, 1, &RtcCtl, 1);

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


  EFI_TIME      EfiTime;

  // Setup the setters and getters
  gRT->GetTime       = LibGetTime;
  gRT->SetTime       = LibSetTime;
  gRT->GetWakeupTime = LibGetWakeupTime;
  gRT->SetWakeupTime = LibSetWakeupTime;

  Status = gRT->GetTime (&EfiTime, NULL);
  if(EFI_ERROR (Status) || (EfiTime.Year < 2010) || (EfiTime.Year > 2099)){
      EfiTime.Year          = 2017;
      EfiTime.Month         = 1;
      EfiTime.Day           = 1;
      EfiTime.Hour          = 0;
      EfiTime.Minute        = 0;
      EfiTime.Second        = 0;
      EfiTime.Nanosecond    = 0;
      EfiTime.Daylight      = 0;
      EfiTime.TimeZone      = EFI_UNSPECIFIED_TIMEZONE;

      Status = gRT->SetTime(&EfiTime);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Status : %r\n", __FUNCTION__, __LINE__, Status));
      }
  }

  DEBUG((EFI_D_INFO, "EfiTime.Year=%d\n", EfiTime.Year));
  DEBUG((EFI_D_INFO, "EfiTime.Month=%d\n", EfiTime.Month));
  DEBUG((EFI_D_INFO, "EfiTime.Day=%d\n", EfiTime.Day));
  DEBUG((EFI_D_INFO, "EfiTime.Hour=%d\n", EfiTime.Hour));
  DEBUG((EFI_D_INFO, "EfiTime.Minute=%d\n", EfiTime.Minute));
  DEBUG((EFI_D_INFO, "EfiTime.Second=%d\n", EfiTime.Second));

  // Install the protocol
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiRealTimeClockArchProtocolGuid,  NULL,
                  NULL
                 );

  return Status;
}
