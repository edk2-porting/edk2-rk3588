/** @file
  GPIO definition table for Tiger Lake U RVP

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_TABLE_TIGER_LAKE_U_DDR4_RVP_H_
#define _GPIO_TABLE_TIGER_LAKE_U_DDR4_RVP_H_

#include <Pins/GpioPinsVer2Lp.h>
#include <Library/GpioLib.h>
#include <Library/GpioConfig.h>

GPIO_INIT_CONFIG mGpioTableTglUDdr4[] =
{
  // M.2 Key-E - WLAN/BT
  {GPIO_VER2_LP_GPP_A13, {GpioPadModeGpio, GpioHostOwnDefault, GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }}, // BT_RF_KILL_N
  {GPIO_VER2_LP_GPP_B15, {GpioPadModeGpio, GpioHostOwnDefault, GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }}, // WIFI_RF_KILL_N
  {GPIO_VER2_LP_GPP_C22, {GpioPadModeGpio, GpioHostOwnDefault, GpioDirOut,   GpioOutHigh,    GpioIntDis,              GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }}, // WLAN_RST_N
  {GPIO_VER2_LP_GPP_C23, {GpioPadModeGpio, GpioHostOwnAcpi,    GpioDirInInv, GpioOutDefault, GpioIntLevel|GpioIntSci, GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }}, // WIFI_WAKE_N
  {GPIO_VER2_LP_GPP_H19, {GpioPadModeGpio, GpioHostOwnAcpi,    GpioDirInInv, GpioOutDefault, GpioIntLevel|GpioIntSci, GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }}, // UART_BT_WAKE_N : Not default POR
  {GPIO_VER2_LP_GPP_A10, {GpioPadModeGpio, GpioHostOwnAcpi,    GpioDirIn,    GpioOutDefault, GpioIntDis,              GpioPlatformReset,  GpioTermNone}},                       // M.2 BT

  // X4 Pcie Slot for Gen3 and Gen 4
  {GPIO_VER2_LP_GPP_A14, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //ONBOARD_X4_PCIE_SLOT1_PWREN_N
  {GPIO_VER2_LP_GPP_C13, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //ONBOARD_X4_PCIE_SLOT1_RESET_N
  {GPIO_VER2_LP_GPP_F5,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv,  GpioOutDefault,GpioIntLevel|GpioIntSci,GpioHostDeepReset,  GpioTermNone,  GpioPadConfigUnlock  }},  //ONBOARD_X4_PCIE_SLOT1_WAKE_N
  {GPIO_VER2_LP_GPP_F20, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //ONBOARD_X4_PCIE_SLOT1_DGPU_SEL
  {GPIO_VER2_LP_GPP_F21, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn,  GpioOutDefault,GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //ONBOARD_X4_PCIE_SLOT1_DGPU_PWROK

  // TBT Re-Timers
  {GPIO_VER2_LP_GPD7,    {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,  GpioIntDis,GpioDswReset,  GpioTermNone}},  //TCP_RETIMER_PERST_N

  // Battery Charger Vmin to PCH PROCHOT, derived from ICL
  {GPIO_VER2_LP_GPP_B2,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv,  GpioOutDefault,GpioIntEdge|GpioIntSci,GpioPlatformReset,  GpioTermNone,  GpioPadConfigUnlock  }},  //BC_PROCHOT_N

  // SATA Direct Connect
  {GPIO_VER2_LP_GPP_B4,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //SATA_DIRECT_PWREN

  // FPS
  {GPIO_VER2_LP_GPP_B14, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //FPS_RST_N
  {GPIO_VER2_LP_GPP_E3,  {GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn,  GpioOutDefault,GpioIntLevel|GpioIntApic,GpioPlatformReset,  GpioTermNone,  GpioPadConfigUnlock  }},  //FPS_INT

  // PCH M.2 SSD
  {GPIO_VER2_LP_GPP_B16, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //M2_PCH_SSD_PWREN
  {GPIO_VER2_LP_GPP_H0,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //M2_SSD_RST_N


  // Camera
  {GPIO_VER2_LP_GPP_B23, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CRD_CAM_PWREN - CAM1
  {GPIO_VER2_LP_GPP_C15, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //WF_CAM_RST_N  - CAM1

  {GPIO_VER2_LP_GPP_H12, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CAM2_RST_N

  {GPIO_VER2_LP_GPP_H15, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CAM3_PWREN
  {GPIO_VER2_LP_GPP_H13, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CAM3_RST_N

  // Camera Common GPIO's for all Camera, Rework Options
  {GPIO_VER2_LP_GPP_B18, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CRD_CAM_STROBE_1
  {GPIO_VER2_LP_GPP_C2,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //WF_CAM_CLK_EN

  // Audio
  {GPIO_VER2_LP_GPP_C5,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //SPKR_PD_N
  {GPIO_VER2_LP_GPP_C12, {GpioPadModeGpio, GpioHostOwnGpio, GpioDirInInv,  GpioOutDefault,GpioIntLevel|GpioIntApic,GpioPlatformReset,  GpioTermNone,  GpioPadConfigUnlock  }},  // CODEC_INT_N

  // Touch Pad
  // Touch Pad and Touch Panel 2 share the same Power Enable, default is Touch pad
  {GPIO_VER2_LP_GPP_H1,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //TCH_PAD_LS_EN - PWR_En
  {GPIO_VER2_LP_GPP_C8,  {GpioPadModeGpio, GpioHostOwnGpio, GpioDirInInv,  GpioOutDefault,GpioIntEdge|GpioIntApic,GpioPlatformReset,  GpioTermNone,  GpioPadConfigUnlock  }},  //TCH_PAD_INT_N

  // EC
  {GPIO_VER2_LP_GPP_E7,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv,  GpioOutDefault,GpioIntLevel|GpioIntSmi,GpioPlatformReset,  GpioTermNone,  GpioPadConfigUnlock  }},  //EC_SMI_N
  {GPIO_VER2_LP_GPP_E8,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //EC_SLP_S0_CS_N

  // SPI TPM, derived from ICL
  {GPIO_VER2_LP_GPP_C14, {GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn,  GpioOutDefault,GpioIntLevel | GpioIntApic, GpioHostDeepReset,  GpioTermWpu20K,  GpioPadConfigUnlock  }},  //SPI_TPM_INT_N

  // TypeC BIAS : Not used by default in RVP, derived from ICL
  {GPIO_VER2_LP_GPP_E22, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutLow,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //DISP_AUX_P_BIAS_GPIO
  {GPIO_VER2_LP_GPP_E23, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //DISP_AUX_N_BIAS_GPIO

  // LAN : Not used by Default in RVP

  // X1 Pcie Slot
  {GPIO_VER2_LP_GPP_F4,  {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv,  GpioOutDefault,GpioIntLevel|GpioIntSci,GpioHostDeepReset,  GpioTermNone,  GpioPadConfigUnlock  }},  //X1 Slot WAKE
  {GPIO_VER2_LP_GPP_F10, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //X1 Slot RESET
};


UINT16 mGpioTableTglUDdr4Size = sizeof (mGpioTableTglUDdr4) / sizeof (GPIO_INIT_CONFIG);

#endif // _GPIO_TABLE_TIGER_LAKE_U_DDR4_RVP_H_
