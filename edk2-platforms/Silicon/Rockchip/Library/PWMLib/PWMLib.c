/** @file

  Copyright (c) 2017, Rockchip Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PWMLib.h>

/* PWM CLK MHz*/
#define PWM_CLK       24

/* PWM Registers offset */
#define PWM_PERIOD    0x04
#define PWM_DUTY      0x08
#define PWM_CTRL      0x0c

/* PWM_CTRL */
#define PWM_ENABLE                         (1)
#define PWM_DISABLE                        (0)
#define PWM_MODE_CONTINUOUS                (1<<1)
#define PWM_DUTY_POL_POSITIVE              (1<<3)
#define PWM_INACTIVE_POL_NEGATIVE          (0<<4)
#define PWM_INACTIVE_POL_POSITIVE          (1<<4)

/* PWM Apply */
RETURN_STATUS
EFIAPI
PWMApply (
  IN UINTN   PWMBase,
  IN UINT32  PeriodNs,
  IN UINT32  DutyNs,
  IN BOOLEAN Polarity
  )
{
  UINT32 Period, Duty;

  Period = PWM_CLK * PeriodNs;
  Duty   = PWM_CLK * DutyNs;

  // Set Period
  MmioWrite32(PWMBase + PWM_PERIOD, Period & 0xFF);

  // Set Duty
  MmioWrite32(PWMBase + PWM_DUTY, Duty & 0xFF);

  // Set Control
  if (Polarity)
      MmioWrite32(PWMBase + PWM_CTRL, PWM_INACTIVE_POL_POSITIVE | PWM_DUTY_POL_POSITIVE | PWM_MODE_CONTINUOUS);
  else
      MmioWrite32(PWMBase + PWM_CTRL, PWM_INACTIVE_POL_NEGATIVE | PWM_DUTY_POL_POSITIVE | PWM_MODE_CONTINUOUS);

  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
PWMEnable (
  IN UINTN   PWMBase
  )
{
  UINT32 Ctrl;

  Ctrl = MmioRead32(PWMBase + PWM_CTRL);
  MmioWrite32(PWMBase + PWM_CTRL, Ctrl | PWM_ENABLE);

  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
PWMDisable (
  IN UINTN   PWMBase
  )
{
  UINT32 Ctrl;

  Ctrl = MmioRead32(PWMBase + PWM_CTRL);
  MmioWrite32(PWMBase + PWM_CTRL, Ctrl | PWM_DISABLE);

  return RETURN_SUCCESS;
}
