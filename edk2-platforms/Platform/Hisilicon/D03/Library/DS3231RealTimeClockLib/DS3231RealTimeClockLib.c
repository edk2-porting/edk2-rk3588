/** @file
  Implement EFI RealTimeClock runtime services via RTC Lib.

  Currently this driver does not support runtime virtual calling.

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011-2013, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.inf

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
// Use EfiAtRuntime to check stage
#include <Library/UefiRuntimeLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/TimeBaseLib.h>
#include <Protocol/RealTimeClock.h>
#include <Library/I2CLib.h>
#include "DS3231RealTimeClock.h"
#include <Library/CpldD03.h>
#include <Library/CpldIoLib.h>

extern I2C_DEVICE gRtcDevice;

STATIC BOOLEAN       mDS3231Initialized = FALSE;

EFI_STATUS
IdentifyDS3231 (
  VOID
  )
{
  EFI_STATUS    Status;

  Status = EFI_SUCCESS;
  return Status;
}

EFI_STATUS
SwitchRtcI2cChannelAndLock (
  VOID
  )
{
  UINT8   Temp;
  UINT8   Count;

  for (Count = 0; Count < 20; Count++) {
    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);

    if ((Temp & BMC_I2C_STATUS) != 0) {
      //The I2C channel is shared with BMC,
      //Check if BMC has taken ownership of I2C.
      //If so, wait 30ms, then try again.
      //If not, start using I2C.
      //And the CPLD_I2C_SWITCH_FLAG will be set to CPU_GET_I2C_CONTROL
      //BMC will check this flag to decide to use I2C or not.
      MicroSecondDelay (30000);
      continue;
    }

    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
    Temp = Temp | CPU_GET_I2C_CONTROL;
    WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);

    //This is empirical value,give cpld some time to make sure the
    //value is wrote in
    MicroSecondDelay (2);
    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);

    if ((Temp & CPU_GET_I2C_CONTROL) == CPU_GET_I2C_CONTROL) {
      return EFI_SUCCESS;
    }

    //There need 30ms to keep consistent with the previous loops if the CPU failed
    //to get control of I2C
    MicroSecondDelay (30000);
  }

  Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
  Temp = Temp & ~CPU_GET_I2C_CONTROL;
  WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);

  return EFI_NOT_READY;
}


EFI_STATUS
InitializeDS3231 (
  VOID
  )
{
  EFI_STATUS    Status;
  I2C_DEVICE    Dev;
  RTC_DS3231_CONTROL Temp;
  RTC_DS3231_HOURS   Hours;

  // Prepare the hardware
  (VOID)IdentifyDS3231();

  (VOID) CopyMem (&Dev, &gRtcDevice, sizeof(Dev));

  Status = I2CInit(Dev.Socket,Dev.Port,Normal);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }
  // Ensure interrupts are masked. We do not want RTC interrupts in UEFI
  Status = I2CRead(&Dev,DS3231_REGADDR_CONTROL,1,&Temp.u8);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }
  Temp.bits.INTCN = 0;
  Status = I2CWrite(&Dev,DS3231_REGADDR_CONTROL,1,&Temp.u8);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  MicroSecondDelay(2000);
  Status = I2CRead(&Dev,DS3231_REGADDR_HOURS,1,&Hours.u8);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }
  Hours.bits.Hour24_n = 0;
  Status = I2CWrite(&Dev,DS3231_REGADDR_HOURS,1,&Hours.u8);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }


  mDS3231Initialized = TRUE;

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
  UINT8       Temp;
  UINT8       BaseHour = 0;

  UINT16      BaseYear = 1900;

  I2C_DEVICE    Dev;

  // Ensure Time is a valid pointer
  if (NULL == Time) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SwitchRtcI2cChannelAndLock();
  if(EFI_ERROR (Status)) {
    return Status;
  }

  // Initialize the hardware if not already done
  if (!mDS3231Initialized) {
    Status = InitializeDS3231 ();
    if (EFI_ERROR (Status)) {
      Status = EFI_NOT_READY;
      goto GExit;
    }
  }

  (VOID) CopyMem (&Dev, &gRtcDevice, sizeof(Dev));

  Status |= I2CRead(&Dev,DS3231_REGADDR_MONTH,1,&Temp);

  Time->Month = ((Temp>>4)&1)*10+(Temp&0x0F);


  if(Temp&0x80){
    BaseYear = 2000;
  }

  Status |= I2CRead(&Dev,DS3231_REGADDR_YEAR,1,&Temp);

  Time->Year  = BaseYear+(Temp>>4) *10 + (Temp&0x0F);

  Status |= I2CRead(&Dev,DS3231_REGADDR_DATE,1,&Temp);

  Time->Day   = ((Temp>>4)&3) *10 + (Temp&0x0F);

  Status |= I2CRead(&Dev,DS3231_REGADDR_HOURS,1,&Temp);

  BaseHour = 0;
  if((Temp&0x30) == 0x30){
    Status = EFI_DEVICE_ERROR;
    goto GExit;
  }else if(Temp&0x20){
    BaseHour = 20;
  }else if(Temp&0x10){
    BaseHour = 10;
  }
  Time->Hour        = BaseHour + (Temp&0x0F);

  Status |= I2CRead(&Dev,DS3231_REGADDR_MIUTES,1,&Temp);

  Time->Minute      = ((Temp>>4)&7) * 10 + (Temp&0x0F);

  Status |= I2CRead(&Dev,DS3231_REGADDR_SECONDS,1,&Temp);

  Time->Second      = (Temp>>4) * 10 + (Temp&0x0F);

  Time->Nanosecond  = 0;
  Time->Daylight    = 0;
  Time->TimeZone    = EFI_UNSPECIFIED_TIMEZONE;

  if((EFI_ERROR(Status)) || (!IsTimeValid(Time)) || ((Time->Year - BaseYear) > 99)) {
    Status = EFI_UNSUPPORTED;
  }

GExit:
  Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
  Temp = Temp & ~CPU_GET_I2C_CONTROL;
  WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);

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
  EFI_STATUS  Status = EFI_SUCCESS;
  I2C_DEVICE    Dev;
  UINT8 Temp;

  UINT16 BaseYear = 1900;



  // Check the input parameters are within the range specified by UEFI
  if(!IsTimeValid(Time)){
    return EFI_INVALID_PARAMETER;
  }

  Status = SwitchRtcI2cChannelAndLock();
  if(EFI_ERROR (Status)) {
    return Status;
  }

  // Initialize the hardware if not already done
  if (!mDS3231Initialized) {
    Status = InitializeDS3231 ();
    if (EFI_ERROR (Status)) {
      goto EXIT;
    }
  }

  (VOID) CopyMem (&Dev, &gRtcDevice, sizeof(Dev));

  Temp = ((Time->Second/10)<<4) | (Time->Second%10);
  MicroSecondDelay(1000);
  Status = I2CWrite(&Dev,DS3231_REGADDR_SECONDS,1,&Temp);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  Temp = ((Time->Minute/10)<<4) | (Time->Minute%10);
  MicroSecondDelay(1000);
  Status = I2CWrite(&Dev,DS3231_REGADDR_MIUTES,1,&Temp);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  Temp = 0;
  if(Time->Hour > 19){
    Temp = 2;
  } else if(Time->Hour > 9){
    Temp = 1;
  }

  Temp = (Temp << 4) | (Time->Hour%10);
  MicroSecondDelay(1000);
  Status = I2CWrite(&Dev,DS3231_REGADDR_HOURS,1,&Temp);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  Temp = ((Time->Day/10)<<4) | (Time->Day%10);
  MicroSecondDelay(1000);
  Status = I2CWrite(&Dev,DS3231_REGADDR_DATE,1,&Temp);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }


  Temp = 0;
  if(Time->Year >= 2000){
    Temp = 0x8;
    BaseYear = 2000;
  }

  if(Time->Month > 9){
    Temp |= 0x1;
  }
  Temp = (Temp<<4) | (Time->Month%10);
  MicroSecondDelay(1000);
  Status = I2CWrite(&Dev,DS3231_REGADDR_MONTH,1,&Temp);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  Temp = (((Time->Year-BaseYear)/10)<<4) | (Time->Year%10);
  MicroSecondDelay(1000);
  Status = I2CWrite(&Dev,DS3231_REGADDR_YEAR,1,&Temp);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  EXIT:

  Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
  Temp = Temp & ~CPU_GET_I2C_CONTROL;
  WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);

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
  EFI_HANDLE    Handle;


  EFI_TIME      EfiTime;

  // Setup the setters and getters
  gRT->GetTime       = LibGetTime;
  gRT->SetTime       = LibSetTime;
  gRT->GetWakeupTime = LibGetWakeupTime;
  gRT->SetWakeupTime = LibSetWakeupTime;


  (VOID)gRT->GetTime (&EfiTime, NULL);
  if((EfiTime.Year < 2015) || (EfiTime.Year > 2099)){
      EfiTime.Year          = 2015;
      EfiTime.Month         = 1;
      EfiTime.Day           = 1;
      EfiTime.Hour          = 0;
      EfiTime.Minute        = 0;
      EfiTime.Second        = 0;
      EfiTime.Nanosecond    = 0;
      Status = gRT->SetTime(&EfiTime);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Status : %r\n", __FUNCTION__, __LINE__, Status));
      }
  }

  // Install the protocol
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiRealTimeClockArchProtocolGuid,  NULL,
                  NULL
                 );

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
  // to virtual address. After the OS transitions to calling in virtual mode, all future
  // runtime calls will be made in virtual mode.
  //
  return;
}
