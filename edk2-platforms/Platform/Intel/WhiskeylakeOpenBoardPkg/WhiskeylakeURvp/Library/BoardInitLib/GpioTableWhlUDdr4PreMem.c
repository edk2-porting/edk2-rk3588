/** @file
  GPIO definition table for Whiskey Lake U DDR4 RVP Pre-Memory


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <GpioPinsCnlLp.h>
#include <Library/GpioLib.h>
#include <GpioConfig.h>

GPIO_INIT_CONFIG mGpioTableWhlUDdr4PreMem[] =
{
  {GPIO_CNL_LP_GPP_C15,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //SLOT1_RST_N
  {GPIO_CNL_LP_GPP_C14,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //SLOT1_PWREN_N
  {GPIO_CNL_LP_GPP_C12,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //PCIE_NAND_RST_N
  {GPIO_CNL_LP_GPP_C13,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutHigh, GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //PCIE_NAND_PWREN_N
};
UINT16 mGpioTableWhlUDdr4PreMemSize = sizeof (mGpioTableWhlUDdr4PreMem) / sizeof (GPIO_INIT_CONFIG);

GPIO_INIT_CONFIG mGpioTableWhlTbtRvpPreMem[] =
{
  // do not reset SLOT1 due to TR AIC card cannot be reset in S3/S4 resume.
  //{GPIO_CNL_LP_GPP_C15,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //SLOT1_RST_N
  {GPIO_CNL_LP_GPP_C14,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //SLOT1_PWREN_N
  {GPIO_CNL_LP_GPP_C12,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //PCIE_NAND_RST_N
  {GPIO_CNL_LP_GPP_C13,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutHigh, GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //PCIE_NAND_PWREN_N
};
UINT16 mGpioTableWhlTbtRvpPreMemSize = sizeof (mGpioTableWhlTbtRvpPreMem) / sizeof (GPIO_INIT_CONFIG);

GPIO_INIT_CONFIG mGpioTableWhlUDdr4WwanOnEarlyPreMem[] =
{
  // Turn on WWAN power and de-assert reset pins by default
  {GPIO_CNL_LP_GPP_A11,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirInInv, GpioOutDefault, GpioIntLevel|GpioIntSci, GpioHostDeepReset, GpioTermWpu20K, GpioPadConfigUnlock}},   //WWAN_WAKE_N
  {GPIO_CNL_LP_GPP_D13,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_FCP_OFF
  {GPIO_CNL_LP_GPP_D16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //EN_V3.3A_WWAN_LS
  {GPIO_CNL_LP_GPP_E15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,     GpioIntDis,              GpioPlatformReset, GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_PERST
  {GPIO_CNL_LP_GPP_F1,   { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_RST_N
  {GPIO_CNL_LP_GPP_H16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_WAKE_CTRL
  {GPIO_CNL_LP_GPP_H17,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_DISABLE_N
};
UINT16 mGpioTableWhlUDdr4WwanOnEarlyPreMemSize = sizeof (mGpioTableWhlUDdr4WwanOnEarlyPreMem) / sizeof (GPIO_INIT_CONFIG);

GPIO_INIT_CONFIG mGpioTableWhlUDdr4WwanOffEarlyPreMem[] =
{
  // Assert reset pins and then turn off WWAN power
  {GPIO_CNL_LP_GPP_A11,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirInInv, GpioOutDefault, GpioIntLevel|GpioIntSci, GpioHostDeepReset, GpioTermWpu20K, GpioPadConfigUnlock}},   //WWAN_WAKE_N
  {GPIO_CNL_LP_GPP_D13,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_FCP_OFF
  {GPIO_CNL_LP_GPP_F1,   { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,     GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_RST_N
  {GPIO_CNL_LP_GPP_E15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioPlatformReset, GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_PERST
  {GPIO_CNL_LP_GPP_D16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,     GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //EN_V3.3A_WWAN_LS
  {GPIO_CNL_LP_GPP_H16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_WAKE_CTRL
  {GPIO_CNL_LP_GPP_H17,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_DISABLE_N
};
UINT16 mGpioTableWhlUDdr4WwanOffEarlyPreMemSize = sizeof (mGpioTableWhlUDdr4WwanOffEarlyPreMem) / sizeof (GPIO_INIT_CONFIG);
