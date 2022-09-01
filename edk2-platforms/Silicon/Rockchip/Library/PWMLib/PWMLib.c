/** @file
  Rockchip PWM Driver.

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PWMLib.h>
#include <stdlib.h>

#define PWM_INT_EN(ch)                      (1 << (ch))
#define PWM_PWR_INT_EN(ch)                  (1 << ((ch) + 4 ))

#define PWM_DISABLE                         (0 << PWM_PWM0_CTRL_PWM_EN_SHIFT)
#define PWM_ENABLE                          (1 << PWM_PWM0_CTRL_PWM_EN_SHIFT)

#define PWM_MODE_SHIFT                      (1)
#define PWM_MODE_MASK                       (0x3U << PWM_MODE_SHIFT)

#define PWM_DUTY_POSTIVE                    (1 << PWM_PWM0_CTRL_DUTY_POL_SHIFT)
#define PWM_DUTY_NEGATIVE                   (0 << PWM_PWM0_CTRL_DUTY_POL_SHIFT)
#define PWM_DUTY_MASK                       (1 << 3)

#define PWM_INACTIVE_POSTIVE                (1 << PWM_PWM0_CTRL_INACTIVE_POL_SHIFT)
#define PWM_INACTIVE_NEGATIVE               (0 << PWM_PWM0_CTRL_INACTIVE_POL_SHIFT)
#define PWM_INACTIVE_MASK                   (1 << 4)

#define PWM_OUTPUT_LEFT                     (0 << PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT)
#define PWM_OUTPUT_CENTER                   (1 << PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT)

#define PWM_UNLOCK                          (0 << PWM_PWM0_CTRL_CONLOCK_SHIFT)
#define PWM_LOCK                            (1 << PWM_PWM0_CTRL_CONLOCK_SHIFT)

#define PWM_LP_DISABLE                      (0 << PWM_PWM0_CTRL_FORCE_CLK_EN_SHIFT)
#define PWM_LP_ENABLE                       (1 << PWM_PWM0_CTRL_FORCE_CLK_EN_SHIFT)

#define PWM_SEL_SRC_CLK                     (0 << PWM_PWM0_CTRL_CLK_SEL_SHIFT)
#define PWM_SEL_SCALE_CLK                   (1 << PWM_PWM0_CTRL_CLK_SEL_SHIFT)

#define PWM_CTRL_SCALE_SHIFT                (PWM_PWM0_CTRL_SCALE_SHIFT)
#define PWM_CTRL_SCALE_MASK                 (PWM_PWM0_CTRL_SCALE_MASK)

#define PWM_PWRMATCH_MAX_SHIFT              (PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT)

#define PWM_CTRL_OFFSET                     (PWM_PWM0_CTRL_OFFSET)
#define PWM_PERIOD_HPR_OFFSET               (PWM_PWM0_PERIOD_HPR_OFFSET)
#define PWM_DUTY_LPR_OFFSET                 (PWM_PWM0_DUTY_LPR_OFFSET)

#define PWM_ONE_SHOT_MODE                   (0)
#define PWM_CONTINUOUS_MODE                 (1)
#define PWM_CAPTURE_MODE                    (2)

#define FREQUENCY_24M                       (24000000)

#define PWM_MAX_CONTROLLER_NUM              (4)

UINTN mPwmControllerBase[PWM_MAX_CONTROLLER_NUM] = {
  RK3588_PWM0_BASE,
  RK3588_PWM1_BASE,
  RK3588_PWM2_BASE,
  RK3588_PWM3_BASE
};

RETURN_STATUS
EFIAPI
RkPwmSetConfig (
  IN PWM_DATA                   *Data
  )
{
  ldiv_t PeriodDiv, DutyDiv;
  UINTN Base = mPwmControllerBase[Data->ControllerID];
  UINT32 Ctrl;
  UINT64 PeriodCycle, DutyCycle;
  UINT64 ChannelOffset = Data->ChannelID * 0x10;

  PeriodDiv = ldiv((UINT64)FREQUENCY_24M * Data->PeriodNs, 1000000000);
  DutyDiv = ldiv((UINT64)FREQUENCY_24M * Data->DutyNs, 1000000000);

  PeriodCycle = PeriodDiv.quot;
  DutyCycle = DutyDiv.quot;

  Ctrl = MmioRead32(Base + ChannelOffset + PWM_CTRL_OFFSET);
  Ctrl |= PWM_LOCK;
  MmioWrite32(Base + ChannelOffset + PWM_CTRL_OFFSET, Ctrl);

  MmioWrite32(Base + ChannelOffset + PWM_PERIOD_HPR_OFFSET, Ctrl);
  MmioWrite32(Base + ChannelOffset + PWM_DUTY_LPR_OFFSET, Ctrl);

  Ctrl &= ~(PWM_DUTY_MASK | PWM_INACTIVE_MASK);

  if (Data->Polarity) {
    Ctrl |= PWM_DUTY_NEGATIVE | PWM_INACTIVE_POSTIVE;
  } else {
    Ctrl |= PWM_DUTY_POSTIVE | PWM_INACTIVE_NEGATIVE;
  }

  Ctrl &= ~PWM_LOCK;
  MmioWrite32(Base + ChannelOffset + PWM_CTRL_OFFSET, Ctrl);

  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
RkPwmEnable (
  IN PWM_DATA                   *Data
  )
{
  UINTN Base = mPwmControllerBase[Data->ControllerID];
  UINT32 Ctrl;
  UINT64 ChannelOffset = Data->ChannelID * 0x10;

  Ctrl = MmioRead32(Base + ChannelOffset + PWM_CTRL_OFFSET);
  Ctrl &= ~PWM_MODE_MASK;
	Ctrl |= (PWM_CONTINUOUS_MODE << PWM_MODE_SHIFT) | PWM_OUTPUT_LEFT | PWM_LP_DISABLE | PWM_ENABLE;

  MmioWrite32(Base + ChannelOffset + PWM_CTRL_OFFSET, Ctrl);

  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
RkPwmDisable (
  IN PWM_DATA                   *Data
  )
{
  UINTN Base = mPwmControllerBase[Data->ControllerID];
  UINT32 Ctrl;
  UINT64 ChannelOffset = Data->ChannelID * 0x10;

  Ctrl = MmioRead32(Base + ChannelOffset + PWM_CTRL_OFFSET);
  Ctrl &= ~PWM_ENABLE;

  MmioWrite32(Base + ChannelOffset + PWM_CTRL_OFFSET, Ctrl);

  return RETURN_SUCCESS;
}
