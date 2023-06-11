/** @file

  Copyright (c) 2020, Hisilicon Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/CpldD06.h>
#include <Library/CpldIoLib.h>
#include <Library/DebugLib.h>
#include <Library/RtcHelperLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiRuntimeLib.h>

EFI_STATUS
SwitchRtcI2cChannelAndLock (
  VOID
  )
{
  UINT8   Temp;
  UINT8   Count;

  for (Count = 0; Count < 100; Count++) {
    // To get the other side's state is idle first
    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
    if ((Temp & BIT3) != 0) {
      (VOID) MicroSecondDelay (RTC_DELAY_30_MS);
      // Try 100 times, if BMC has not released the bus, return preemption failed
      if (Count == 99) {
        if (!EfiAtRuntime ()) {
          DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Clear cpu_i2c_rtc_state 100 times fail!\n",
            __FUNCTION__, __LINE__));
        }
        return EFI_DEVICE_ERROR;
      }
      continue;
    }

    // if BMC free the bus, can be set 1 preemption
    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
    Temp = Temp | CPU_GET_I2C_CONTROL;
    // CPU occupied RTC I2C State
    WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);
    (VOID)MicroSecondDelay (RTC_DELAY_2_MICROSECOND);
    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
    // Is preempt success
    if (CPU_GET_I2C_CONTROL == (Temp & CPU_GET_I2C_CONTROL)) {
      break;
    }
    if (Count == 99) {
      if (!EfiAtRuntime ()) {
        DEBUG ((DEBUG_ERROR, "[%a]:[%dL]  Clear cpu_i2c_rtc_state fail !!! \n",
          __FUNCTION__, __LINE__));
      }
      return EFI_DEVICE_ERROR;
    }
    (VOID)MicroSecondDelay (RTC_DELAY_30_MS);
  }

  //Polling BMC RTC I2C status
  for (Count = 0; Count < 100; Count++) {
    Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
    if ((Temp & BIT3) == 0) {
      return EFI_SUCCESS;
    }
    (VOID)MicroSecondDelay (RTC_DELAY_30_MS);
  }

  //If the BMC occupies the RTC I2C Channel, write back the CPU side is idle
  // or the subsequent BMC will not preempt
  Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
  Temp = Temp & (~CPU_GET_I2C_CONTROL);
  WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);

  return EFI_NOT_READY;
}

VOID
ReleaseOwnershipOfRtc (
  VOID
  )
{
  UINT8   Temp;

  Temp = ReadCpldReg (CPLD_I2C_SWITCH_FLAG);
  Temp = Temp & ~CPU_GET_I2C_CONTROL;
  WriteCpldReg (CPLD_I2C_SWITCH_FLAG, Temp);
  return ;
}
