/** @file
  GPIO definition table for Tiger Lake U DDR4 RVP Pre-Memory


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_TABLE_TIGER_LAKE_U_DDR4_RVP_PREMEM_H_
#define _GPIO_TABLE_TIGER_LAKE_U_DDR4_RVP_PREMEM_H_

#include <Pins/GpioPinsVer2Lp.h>
#include <Library/GpioLib.h>
#include <Library/GpioConfig.h>

GPIO_INIT_CONFIG mGpioTablePreMemTglUDdr4[] =
{
  { GPIO_VER2_LP_GPP_A14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutLow,  GpioIntDis, GpioPlatformReset, GpioTermNone } },  //ONBOARD_X4_PCIE_SLOT1_PWREN_N
  { GPIO_VER2_LP_GPP_C13, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone } },  //ONBOARD_X4_PCIE_SLOT1_RESET_N
  // CPU M.2 SSD
  { GPIO_VER2_LP_GPP_D16, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone } },  //CPU SSD PWREN
  { GPIO_VER2_LP_GPP_A11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone } },  //CPU SSD RESET
  // X1 Pcie Slot
  { GPIO_VER2_LP_GPP_F9,  { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone } },  //X1 Slot PWREN
  { GPIO_VER2_LP_GPP_A23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutLow,  GpioIntDis, GpioPlatformReset, GpioTermNone } },  //TC_RETIMER_FORCE_PWR
  // Camera
  { GPIO_VER2_LP_GPP_R6,  { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone } },  //CAM2_PWREN/BIOS_REC
  { GPIO_VER2_LP_GPP_R5,  { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutLow,  GpioIntDis, GpioPlatformReset, GpioTermNone } },  //CRD_CAM_PRIVACY_LED_1
};

UINT16 mGpioTablePreMemTglUDdr4Size = sizeof (mGpioTablePreMemTglUDdr4) / sizeof (GPIO_INIT_CONFIG);

#endif  //_GPIO_TABLE_TIGER_LAKE_U_DDR4_RVP_PREMEM_H_
