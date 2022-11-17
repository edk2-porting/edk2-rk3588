/** @file
  Implement EFI RealTimeClock runtime services via RTC Lib.

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Marvell International Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
/**
  Derived from:
  ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.c

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/IoLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/RealTimeClock.h>
#include "RealTimeClockLib.h"

STATIC EFI_EVENT              mRtcVirtualAddrChangeEvent;
STATIC UINTN                  mArmadaRtcBase;

/**
  According to errata FE-3124064, write to RTC TIME RTC_ALARMx registers
  may fail. As a workaround, before actual writing to those registers,
  issue a dummy write of 0x0 twice to RTC Status register.
  Also, according to the datasheet, the OS should wait 5us after every
  register write to the RTC hard macro, so that the required update
  can occur without holding off the system bus.
**/
STATIC
VOID
RtcDelayedWrite (
  IN UINT32        Offset,
  IN UINT32        Value
  )
{
  MmioWrite32 (mArmadaRtcBase + RTC_STATUS_REG, 0);
  MmioWrite32 (mArmadaRtcBase + RTC_STATUS_REG, 0);
  MmioWrite32 (mArmadaRtcBase + Offset, Value);
  MicroSecondDelay (5);
}

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
  OUT EFI_TIME_CAPABILITIES   *Capabilities
  )
{
  EFI_STATUS     Status = EFI_SUCCESS;
  UINT32 RegVal;

  RegVal = MmioRead32 (mArmadaRtcBase + RTC_TIME_REG);

  // Convert from internal 32-bit time to UEFI time
  EpochToEfiTime (RegVal, Time);

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
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       EpochSeconds;

  // Check the input parameters are within the range specified by UEFI
  if (!IsTimeValid (Time)) {
    return EFI_INVALID_PARAMETER;
  }

  // Convert time to raw seconds
  EpochSeconds = EfiTimeToEpoch (Time);
  if (EpochSeconds > MAX_UINT32) {
    return EFI_INVALID_PARAMETER;
  }

  // Issue delayed write to time register
  RtcDelayedWrite (RTC_TIME_REG, (UINT32)EpochSeconds);

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
  UINT32 WakeupSeconds;

  if (Time == NULL || Enabled == NULL || Pending == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Enabled = MmioRead32 (mArmadaRtcBase + RTC_IRQ_2_CONFIG_REG) & RTC_IRQ_ALARM_EN;

  *Pending = MmioRead32 (mArmadaRtcBase + RTC_IRQ_STATUS_REG) & RTC_IRQ_ALARM_MASK;
  // Ack pending alarm
  if (*Pending) {
    MmioWrite32 (mArmadaRtcBase + RTC_IRQ_STATUS_REG, RTC_IRQ_ALARM_MASK);
  }

  WakeupSeconds = MmioRead32 (mArmadaRtcBase + RTC_ALARM_2_REG);
  EpochToEfiTime (WakeupSeconds, Time);

  return EFI_SUCCESS;
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
  UINTN       WakeupSeconds;

  // Handle timer disabling case
  if (!Enabled) {
    RtcDelayedWrite (RTC_IRQ_2_CONFIG_REG, 0);
    return EFI_SUCCESS;
  }

  if (Time == NULL || !IsTimeValid (Time)) {
    return EFI_INVALID_PARAMETER;
  }

  // Convert time to raw seconds
  WakeupSeconds = EfiTimeToEpoch (Time);
  if (WakeupSeconds > MAX_UINT32) {
    return EFI_INVALID_PARAMETER;
  }

  // Issue delayed write to alarm register
  RtcDelayedWrite (RTC_ALARM_2_REG, (UINT32)WakeupSeconds);

  // Enable wakeup timer
  RtcDelayedWrite (RTC_IRQ_2_CONFIG_REG, RTC_IRQ_ALARM_EN);

  return EFI_SUCCESS;
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
  EFI_HANDLE    Handle;
  EFI_STATUS    Status;

  // Obtain RTC device base address
  mArmadaRtcBase = PcdGet64 (PcdRtcBaseAddress);

  // Check if the controller can be initialized
  if (mArmadaRtcBase == 0) {
    DEBUG ((DEBUG_ERROR, "RTC: None of controllers enabled\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Declare the controller as EFI_MEMORY_RUNTIME
  Status = gDS->AddMemorySpace (
                  EfiGcdMemoryTypeMemoryMappedIo,
                  mArmadaRtcBase,
                  SIZE_4KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to add memory space\n"));
    return Status;
  }

  Status = gDS->SetMemorySpaceAttributes (
                  mArmadaRtcBase,
                  SIZE_4KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to set memory attributes\n"));
    goto ErrSetMem;
  }

  /* Update RTC-MBUS bridge timing parameters */
  MmioAndThenOr32 (
          mArmadaRtcBase + RTC_BRIDGE_TIMING_CTRL0_REG_OFFS,
          ~(RTC_WRITE_SETUP_DELAY_MASK | RTC_WRITE_PERIOD_DELAY_MASK),
          (RTC_WRITE_SETUP_DELAY_DEFAULT | RTC_WRITE_PERIOD_DELAY_DEFAULT)
          );
  MmioAndThenOr32 (
          mArmadaRtcBase + RTC_BRIDGE_TIMING_CTRL1_REG_OFFS,
          ~RTC_READ_OUTPUT_DELAY_MASK,
          RTC_READ_OUTPUT_DELAY_DEFAULT
          );

  // Install the protocol
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiRealTimeClockArchProtocolGuid,
                  NULL,
                  NULL
                 );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to install the protocol\n"));
    goto ErrSetMem;
  }

  // Register for the virtual address change event
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  LibRtcVirtualNotifyEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mRtcVirtualAddrChangeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to register virtual address change event\n"));
    goto ErrEvent;
  }

  return Status;

ErrEvent:
  gBS->UninstallProtocolInterface (Handle, &gEfiRealTimeClockArchProtocolGuid, NULL);
ErrSetMem:
  gDS->RemoveMemorySpace (mArmadaRtcBase, SIZE_4KB);

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
  EfiConvertPointer (0x0, (VOID**)&mArmadaRtcBase);
}
