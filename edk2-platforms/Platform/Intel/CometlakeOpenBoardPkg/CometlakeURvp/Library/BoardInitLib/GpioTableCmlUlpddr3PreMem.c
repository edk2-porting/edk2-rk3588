/** @file
  GPIO definition table for Comet Lake U LP3 RVP Pre-Memory


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <GpioPinsCnlLp.h>
#include <Library/GpioLib.h>
#include <GpioConfig.h>

GPIO_INIT_CONFIG mGpioTableCmlULpddr3PreMem[] =
{
  {GPIO_CNL_LP_GPP_C15,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //SLOT1_RST_N
  {GPIO_CNL_LP_GPP_C14,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //SLOT1_PWREN_N
  {GPIO_CNL_LP_GPP_C12,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //PCIE_NAND_RST_N
  {GPIO_CNL_LP_GPP_C13,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirOut,  GpioOutHigh, GpioIntDis,  GpioPlatformReset,  GpioTermNone}},   //PCIE_NAND_PWREN_N
};
UINT16 mGpioTableCmlULpddr3PreMemSize = sizeof (mGpioTableCmlULpddr3PreMem) / sizeof (GPIO_INIT_CONFIG);

GPIO_INIT_CONFIG mGpioTableCmlULpddr3WwanOnEarlyPreMem[] =
{
  // Turn on WWAN power and de-assert reset pins by default
  {GPIO_CNL_LP_GPP_A11,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirInInv, GpioOutDefault, GpioIntLevel|GpioIntSci, GpioHostDeepReset, GpioTermWpu20K, GpioPadConfigUnlock}},   //WWAN_WAKE_N
  {GPIO_CNL_LP_GPP_D13,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_FCP_OFF
  {GPIO_CNL_LP_GPP_D16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //EN_V3.3A_WWAN_LS
  {GPIO_CNL_LP_GPP_E15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,     GpioIntDis,              GpioPlatformReset, GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_PERST
  {GPIO_CNL_LP_GPP_F1,   { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_RST_N
  {GPIO_CNL_LP_GPP_H16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_WAKE_CTRL
  {GPIO_CNL_LP_GPP_H17,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_DISABLE_N
};
UINT16 mGpioTableCmlULpddr3WwanOnEarlyPreMemSize = sizeof(mGpioTableCmlULpddr3WwanOnEarlyPreMem) / sizeof(GPIO_INIT_CONFIG);


GPIO_INIT_CONFIG mGpioTableCmlULpddr3WwanOffEarlyPreMem[] =
{
  // Assert reset pins and then turn off WWAN power
  {GPIO_CNL_LP_GPP_F1,   { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,     GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_RST_N
  {GPIO_CNL_LP_GPP_E15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioPlatformReset, GpioTermNone,   GpioOutputStateUnlock}}, //WWAN_PERST
  {GPIO_CNL_LP_GPP_D16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,     GpioIntDis,              GpioResumeReset,   GpioTermNone,   GpioOutputStateUnlock}}, //EN_V3.3A_WWAN_LS
};
UINT16 mGpioTableCmlULpddr3WwanOffEarlyPreMemSize = sizeof(mGpioTableCmlULpddr3WwanOffEarlyPreMem) / sizeof(GPIO_INIT_CONFIG);
