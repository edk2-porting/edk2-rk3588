/** @file
  GPIO definition table

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <GpioPinsCnlLp.h>
#include <Library/GpioLib.h>
#include <GpioConfig.h>

#define END_OF_GPIO_TABLE 0xFFFFFFFF

//
// CNL U DRR4 Board GPIO table configuration is used as default
//
GPIO_INIT_CONFIG mGpioTableDefault[] =
{
//                      Pmode,  GPI_IS,  GpioDir,  GPIOTxState,  RxEvCfg,  GPIRoutConfig,  PadRstCfg,  Term,
  //{GPIO_CNL_LP_GPP_A0,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},
  //{GPIO_CNL_LP_GPP_A1,  { GpioPadModeNative2,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_IO_0
  //{GPIO_CNL_LP_GPP_A2,  { GpioPadModeNative2,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_IO_1
  //{GPIO_CNL_LP_GPP_A3,  { GpioPadModeNative2,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_IO_2
  //{GPIO_CNL_LP_GPP_A4,  { GpioPadModeNative2,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_IO_2
  //{GPIO_CNL_LP_GPP_A5,  { GpioPadModeNative2,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_CSB
  //{GPIO_CNL_LP_GPP_A6,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //GPPC_A6_SERIRQ
  {GPIO_CNL_LP_GPP_A7,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirIn,  GpioOutDefault,  GpioIntEdge | GpioIntSci,  GpioHostDeepReset,  GpioTermWpu20K, GpioPadConfigUnlock }},  //SPI_TPM_INT_N
  //{GPIO_CNL_LP_GPP_A8,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},
  //(Default HW)  {GPIO_CNL_LP_GPP_A9,  { GpioPadModeNative2,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_CLK
  //{GPIO_CNL_LP_GPP_A10,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},
  {GPIO_CNL_LP_GPP_A11,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirInInv,  GpioOutDefault,  GpioIntLevel|GpioIntSci,  GpioHostDeepReset,  GpioTermWpu20K, GpioPadConfigUnlock }},  //WWAN_WAKE_N
  // (RC control) {GPIO_CNL_LP_GPP_A12, { GpioPadModeNative2, GpioHostOwnDefault, GpioDirInOut, GpioOutDefault, GpioIntDefault, GpioPlatformReset, GpioTermNone }},  //SLATEMODE_HALLOUT
  {GPIO_CNL_LP_GPP_A13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDefault, GpioPlatformReset, GpioTermNone } },  //DGPU_SEL_SLOT1
  //(Default HW)  {GPIO_CNL_LP_GPP_A14,  { GpioPadModeNative2,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //eSPI_Reset
  {GPIO_CNL_LP_GPP_A15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPKR_PD_N
  {GPIO_CNL_LP_GPP_A16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //WFCAM_PWREN
  //(RC control) {GPIO_CNL_LP_GPP_A17,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //SD_PWREN
  //(RC control) {GPIO_CNL_LP_GPP_A18,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermWpu20K }},  //ACCEL_INT
  //(RC control) {GPIO_CNL_LP_GPP_A19,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermWpu20K }},  //ALS_INT
  //(RC control) {GPIO_CNL_LP_GPP_A20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermWpu20K }},  //HUMAN_PRESENCE_INT
  //(RC control) {GPIO_CNL_LP_GPP_A21,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermWpu20K }},  //HALL_SENSOR_INT
  //(RC control) {GPIO_CNL_LP_GPP_A22,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IVCAM_WAKE
  //(RC control) {GPIO_CNL_LP_GPP_A23,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermWpu20K }},  //SHARED_INT
  //(Not used) {GPIO_CNL_LP_GPP_B0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirOut,  GpioOutDefault,  GpioIntDis,  GpioResetDefault,  GpioTermNone }},  //CORE_VID0
  //(Not used) {GPIO_CNL_LP_GPP_B1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirOut,  GpioOutDefault,  GpioIntDis,  GpioResetDefault,  GpioTermNone }},  //CORE_VID0
  {GPIO_CNL_LP_GPP_B2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntLevel | GpioIntApic, GpioHostDeepReset, GpioTermNone, GpioPadConfigUnlock | GpioOutputStateUnlock } },  //BT_UART_WAKE
  {GPIO_CNL_LP_GPP_B3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntLevel | GpioIntApic, GpioPlatformReset, GpioTermNone, GpioPadConfigUnlock | GpioOutputStateUnlock }},  //FORCE_PAD_INT
  {GPIO_CNL_LP_GPP_B4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermNone , GpioPadConfigUnlock} },  //BT_DISABLE_N
  //(RC control) {GPIO_CNL_LP_GPP_B5,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //WWAN_CLK_REQ
  //(RC control) {GPIO_CNL_LP_GPP_B6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //PCIE_NAND_CLK_REQ
  //(RC control) {GPIO_CNL_LP_GPP_B7,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //LAN_CLK_REQ
  //(RC control) {GPIO_CNL_LP_GPP_B8,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //WLAN_CLK_REQ
  //(RC control) {GPIO_CNL_LP_GPP_B9,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //PCIE_SLOT1_CLK_REQ
  //(RC control) {GPIO_CNL_LP_GPP_B10,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //PCIE_SLOT2_CLK_REQ
  {GPIO_CNL_LP_GPP_B11, { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioPlatformReset,  GpioTermNone }},
  //(Default HW)  {GPIO_CNL_LP_GPP_B12,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S0_N
  //(Default HW)  {GPIO_CNL_LP_GPP_B13,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PLT_RST_N
  {GPIO_CNL_LP_GPP_B14, { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},  //TCH_PNL_PWR_EN
  //(CSME Pad) {GPIO_CNL_LP_GPP_B15,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone }},  //NFC_DFU
  { GPIO_CNL_LP_GPP_B16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntLevel | GpioIntApic, GpioPlatformReset, GpioTermNone, GpioPadConfigUnlock } },  //FPS_INT_N
  { GPIO_CNL_LP_GPP_B17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone, GpioPadConfigUnlock} },  //FPS_RESET_N
  {GPIO_CNL_LP_GPP_B18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone }},  //TBT_CIO_PWR_EN
  //(RC control) {GPIO_CNL_LP_GPP_B19,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GSPI1_CS_FPS
  //(RC control) {GPIO_CNL_LP_GPP_B20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GSPI1_CLK_FPS
  //(RC control) {GPIO_CNL_LP_GPP_B21,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GSPI1_MISO_FPS
  //(RC control) {GPIO_CNL_LP_GPP_B22,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GSPI1_MOSI_FPS
  {GPIO_CNL_LP_GPP_B23, { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone}},  //EC_SLP_S0_CS_N
  //(RC control) {GPIO_CNL_LP_GPP_C0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //SMB_CLK
  //(RC control) {GPIO_CNL_LP_GPP_C1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //SMB_DATA
  {GPIO_CNL_LP_GPP_C2, { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioHostDeepReset,  GpioTermNone }},  //WIFI_RF_KILL_N
  //(CSME Pad) {GPIO_CNL_LP_GPP_C3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //SML0_CLK
  //(CSME Pad) {GPIO_CNL_LP_GPP_C4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //SML0_DATA
  {GPIO_CNL_LP_GPP_C5, { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirInInv,  GpioOutDefault,  GpioIntLevel | GpioIntSci,  GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }},  //WIFI_WAKE_N
  //(Not used) {GPIO_CNL_LP_GPP_C6,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},
  //(Not used) {GPIO_CNL_LP_GPP_C7,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},
  { GPIO_CNL_LP_GPP_C8, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntLevel | GpioIntApic, GpioHostDeepReset, GpioTermWpu20K } },  //CODEC_INT_N
  { GPIO_CNL_LP_GPP_C9, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntEdge | GpioIntSci, GpioPlatformReset, GpioTermWpu20K, GpioPadConfigUnlock }},  //TBT_CIO_PLUG_EVENT_N
  {GPIO_CNL_LP_GPP_C10,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone}},  //TBT_FORCE_PWR
  {GPIO_CNL_LP_GPP_C11,  { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntLevel | GpioIntSci, GpioHostDeepReset, GpioTermWpu20K, GpioPadConfigUnlock } },  //IVCAM_WAKE_N
  {GPIO_CNL_LP_GPP_C12,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},  //PCIE_NAND_RST_N
  {GPIO_CNL_LP_GPP_C13,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},  //PCIE_NAND_PWREN_N
  {GPIO_CNL_LP_GPP_C14,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},  //SLOT1_PWREN_N
  {GPIO_CNL_LP_GPP_C15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioPlatformReset,  GpioTermNone}},  //SLOT1_RST_N
  //(RC control) {GPIO_CNL_LP_GPP_C16,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //I2C0_SDA
  //(RC control) {GPIO_CNL_LP_GPP_C17,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //I2C0_SCL
  //(RC control) {GPIO_CNL_LP_GPP_C18,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //I2C1_SDA
  //(RC control) {GPIO_CNL_LP_GPP_C19,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //I2C1_SCL
  //(RC control) {GPIO_CNL_LP_GPP_C20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //UART2_RXD
  //(RC control) {GPIO_CNL_LP_GPP_C21,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //UART2_TXD
  //(RC control) {GPIO_CNL_LP_GPP_C22,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //UART2_RTS
  //(RC control) {GPIO_CNL_LP_GPP_C23,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //UART2_CTS
  //(CSME Pad) {GPIO_CNL_LP_GPP_D0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPI1_TCH_PNL_CS0_N
  //(CSME Pad) {GPIO_CNL_LP_GPP_D1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPI1_TCH_PNL_CLK_N
  //(CSME Pad) {GPIO_CNL_LP_GPP_D2,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPI1_TCH_PNL_MISO
  //(CSME Pad) {GPIO_CNL_LP_GPP_D3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPI1_TCH_PNL_MOSI
  //(RC control) {GPIO_CNL_LP_GPP_D4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IMGCLKOUT
  //(RC control) {GPIO_CNL_LP_GPP_D5,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //ISH_I2C0_SDA
  //(RC control) {GPIO_CNL_LP_GPP_D6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //ISH_I2C0_SCL
  //(RC control) {GPIO_CNL_LP_GPP_D7,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //ISH_I2C1_SDA
  //(RC control) {GPIO_CNL_LP_GPP_D8,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //ISH_I2C1_SCL
  {GPIO_CNL_LP_GPP_D9,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioPlatformReset,  GpioTermNone }},  //TCH_PNL2_RST_N
  {GPIO_CNL_LP_GPP_D10,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntEdge | GpioIntApic,  GpioPlatformReset,  GpioTermNone, GpioPadConfigUnlock }},  //TCH_PNL2_INT_N
  {GPIO_CNL_LP_GPP_D11,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirInInv ,  GpioOutDefault,  GpioIntLevel| GpioIntSci,  GpioPlatformReset,  GpioTermWpu20K, GpioPadConfigUnlock }},  //SLOT1_WAKE_N
  //(CSME Pad) {GPIO_CNL_LP_GPP_D12,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //NFC_RST_N
  {GPIO_CNL_LP_GPP_D13,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDis,  GpioResumeReset,  GpioTermNone }},  //WWAN_PWREN
  {GPIO_CNL_LP_GPP_D14,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDis,  GpioPlatformReset,  GpioTermNone }},  //TCH_PNL_RST_N
  //(CSME Pad) {GPIO_CNL_LP_GPP_D15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //NFC_INT_N
  {GPIO_CNL_LP_GPP_D16,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntSci,  GpioHostDeepReset,  GpioTermNone, GpioPadConfigUnlock }},  //WIGIG_WAKE_N
  //(RC control) {GPIO_CNL_LP_GPP_D17,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //DMIC_CLK_1
  //(RC control) {GPIO_CNL_LP_GPP_D18,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //DMIC_DATA_1
  //(RC control) {GPIO_CNL_LP_GPP_D19,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //DMIC_CLK_0
  //(RC control) {GPIO_CNL_LP_GPP_D20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //DMIC_DATA_0
  {GPIO_CNL_LP_GPP_D21,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPI1_TCH_PNL_IO2
  {GPIO_CNL_LP_GPP_D22,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirInOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SPI1_TCH_PNL_IO3
  //(RC control) {GPIO_CNL_LP_GPP_D23,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirOut,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //SSP_MCLK
  //(Not used) {GPIO_CNL_LP_GPP_E0,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntSci,  GpioPlatformReset,  GpioTermWpu20K }},  //Reserved for SATA/PCIE detect
  //(RC control) {GPIO_CNL_LP_GPP_E1,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntSci,  GpioPlatformReset,  GpioTermNone }},  //M.2_SSD_DET
  {GPIO_CNL_LP_GPP_E2,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntDis,  GpioPlatformReset,  GpioTermWpu20K}},  //Reserved for SATA HP val
  {GPIO_CNL_LP_GPP_E3,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirIn,  GpioOutDefault,  GpioIntEdge|GpioIntSmi,  GpioPlatformReset,  GpioTermNone}},  //EC_SMI_N
  {GPIO_CNL_LP_GPP_E4,  { GpioPadModeGpio,  GpioHostOwnAcpi,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntSci,  GpioPlatformReset,  GpioTermNone, GpioPadConfigUnlock }},  //DGPU_PWROK
  //(RC control) {GPIO_CNL_LP_GPP_E5,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SSD_DEVSLP
  //(RC control) {GPIO_CNL_LP_GPP_E6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntLevel|GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //HDD_DEVSLP
  {GPIO_CNL_LP_GPP_E7,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntEdge|GpioIntDefault,  GpioPlatformReset,  GpioTermNone, GpioPadConfigUnlock }},  //TCH_PNL_INT_N
  //(RC control) {GPIO_CNL_LP_GPP_E8,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SATA_LED_N
  //(RC control) {GPIO_CNL_LP_GPP_E9,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //BSSB_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E10,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //BSSB_DI
  //(RC control) {GPIO_CNL_LP_GPP_E11,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //USB_OC_2
  //(RC control) {GPIO_CNL_LP_GPP_E12,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //USB_OC_3
  //(RC control) {GPIO_CNL_LP_GPP_E13,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI1_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E14,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI2_HPD_EC
  //(RC control) {GPIO_CNL_LP_GPP_E15,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI3_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E16,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI4_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E17,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //EDP_HPD
  //(RC control) {GPIO_CNL_LP_GPP_E18,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI1_CTRL_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E19,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI1_CTRL_DATA
  //(RC control) {GPIO_CNL_LP_GPP_E20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI2_CTRL_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E21,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI2_CTRL_DATA
  //(RC control) {GPIO_CNL_LP_GPP_E22,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI3_CTRL_CLK
  //(RC control) {GPIO_CNL_LP_GPP_E23,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI3_CTRL_DATA
  //(Not used){GPIO_CNL_LP_GPP_F0,  { GpioPadModeGpio,  GpioHostOwnDefault,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F0_COEX3
  {GPIO_CNL_LP_GPP_F1,  { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResumeReset, GpioTermWpu20K }},  //WWAN_RST_N
  {GPIO_CNL_LP_GPP_F2,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //SATA_HDD_PWREN
  {GPIO_CNL_LP_GPP_F3,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutLow,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //WF_CLK_EN
  //(RC control) {GPIO_CNL_LP_GPP_F4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //CNV_BRI_DT_UART0_RTSB
  //(RC control) {GPIO_CNL_LP_GPP_F5,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //CNV_BRI_RSP_UART0_RXD
  //(RC control) {GPIO_CNL_LP_GPP_F6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //CNV_RGI_DT_UART0_TXD
  //(RC control) {GPIO_CNL_LP_GPP_F7,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //CNV_RGI_RSP_UART0_CTSB
  {GPIO_CNL_LP_GPP_F8,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //CNV_MFUART2_RXD
  {GPIO_CNL_LP_GPP_F9,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //CNV_MFUART2_TXD

  //Also need to assign same GPIO pin to PcdRecoveryModeGpio which will be used at IsRecoveryMode()
  {GPIO_CNL_LP_GPP_F10,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirIn,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone}},  //BIOS_REC

  //(RC control)  {GPIO_CNL_LP_GPP_F11,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F11_EMMC_CMD
  //(RC control)  {GPIO_CNL_LP_GPP_F12,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F12_EMMC_DATA0
  //(RC control)  {GPIO_CNL_LP_GPP_F13,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F13_EMMC_DATA1
  //(RC control)  {GPIO_CNL_LP_GPP_F14,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F14_EMMC_DATA2
  //(RC control)  {GPIO_CNL_LP_GPP_F15,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F15_EMMC_DATA3
  //(RC control)  {GPIO_CNL_LP_GPP_F16,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F16_EMMC_DATA4
  //(RC control)  {GPIO_CNL_LP_GPP_F17,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F17_EMMC_DATA5
  //(RC control)  {GPIO_CNL_LP_GPP_F18,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F18_EMMC_DATA6
  //(RC control)  {GPIO_CNL_LP_GPP_F19,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F19_EMMC_DATA7
  //(RC control)  {GPIO_CNL_LP_GPP_F20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F20_EMMC_RCLK
  //(RC control)  {GPIO_CNL_LP_GPP_F21,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F21_EMMC_CLK
  //(RC control)  {GPIO_CNL_LP_GPP_F22,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_F22_EMMC_RESETB
  //{GPIO_CNL_LP_GPP_F23,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //GPP_F_23
  //(RC control)  {GPIO_CNL_LP_GPP_G0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNative }},  //GPP_G_0_SD3_CMD
  //(RC control)  {GPIO_CNL_LP_GPP_G1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNative }},  //GPP_G_1_SD3_D0_SD4_RCLK_P
  //(RC control)  {GPIO_CNL_LP_GPP_G2,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNative }},  //GPP_G_2_SD3_D1_SD4_RCLK_N
  //(RC control)  {GPIO_CNL_LP_GPP_G3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNative }},  //GPP_G_3_SD3_D2
  //(RC control)  {GPIO_CNL_LP_GPP_G4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNative }},  //GPP_G_4_SD3_D3
  {GPIO_CNL_LP_GPP_G5,  { GpioHardwareDefault,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermWpu20K }},  //GPP_G_5_SD3_CDB
  //(Default HW)  {GPIO_CNL_LP_GPP_G6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //GPP_G_6_SD3_CLK
  {GPIO_CNL_LP_GPP_G7,  { GpioHardwareDefault,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermWpd20K }},  //GPP_G_7_SD3_WP
  //{GPIO_CNL_LP_GPP_H0,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //GPP_H_0_SSP2_SCLK
  //{GPIO_CNL_LP_GPP_H1,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //GPP_H_1_SSP2_SFRM
  //{GPIO_CNL_LP_GPP_H2,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //GPP_H_2_SSP2_TXD
  //{GPIO_CNL_LP_GPP_H3,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermNone }},  //GPP_H_3_SSP2_RXD
  //(RC control)  {GPIO_CNL_LP_GPP_H4,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H_4_I2C2_SDA
  //(RC control)  {GPIO_CNL_LP_GPP_H5,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H_5_I2C2_SCL
  //(RC control)  {GPIO_CNL_LP_GPP_H6,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H_6_I2C3_SDA
  //(RC control)  {GPIO_CNL_LP_GPP_H7,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H_7_I2C3_SCL
  //(RC control)  {GPIO_CNL_LP_GPP_H8,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H_8_I2C4_SDA
  //(RC control)  {GPIO_CNL_LP_GPP_H9,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H_9_I2C4_SCL
  {GPIO_CNL_LP_GPP_H10,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IVCAM_PWREN
  {GPIO_CNL_LP_GPP_H11,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IVCAM_RECOVERY
  {GPIO_CNL_LP_GPP_H12,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IRIS_STROBE
  {GPIO_CNL_LP_GPP_H13,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IVCAM_MUX_SEL0
  {GPIO_CNL_LP_GPP_H14,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutLow,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone, GpioPadUnlock }},  //UF_CAM_PRIVACY_LED
  {GPIO_CNL_LP_GPP_H15,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IVCAM_KEY
  //(Not used) {GPIO_CNL_LP_GPP_H16,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI4_CTRL_CLK
  //(Not used) {GPIO_CNL_LP_GPP_H17,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //DDI4_CTRL_DATA
  //(Default HW)  {GPIO_CNL_LP_GPP_H18,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirOut,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //VCCIO_LPM
  {GPIO_CNL_LP_GPP_H19,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,   GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IVCAM_MUX_SEL1
  //(RC control) {GPIO_CNL_LP_GPP_H20,  { GpioPadModeNative1,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //IMGCLKOUT_WF_CAM
  //(Not used) {GPIO_CNL_LP_GPP_H21,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H21
  {GPIO_CNL_LP_GPP_H22,  { GpioPadModeGpio,  GpioHostOwnGpio,  GpioDirOut,  GpioOutHigh,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //WF_CAM_RST
  //(Not used) {GPIO_CNL_LP_GPP_H23,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioPlatformReset,  GpioTermNone }},  //GPP_H23
  //(Default HW)  {GPIO_CNL_LP_GPD0,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_BATLOW_N
  //(Default HW)  {GPIO_CNL_LP_GPD1,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //BC_ACOK
  //(Default HW)  {GPIO_CNL_LP_GPD2,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //LAN_WAKE
  //(Default HW)  {GPIO_CNL_LP_GPD3,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_PWRBTN_N
  //(Default HW)  {GPIO_CNL_LP_GPD4,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S3_N
  //(Default HW)  {GPIO_CNL_LP_GPD5,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S4_N
  //(Default HW)  {GPIO_CNL_LP_GPD6,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //SLP_A_N
  //{GPIO_CNL_LP_GPD7,  { GpioPadModeNotUsed,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //GPD_7
  //(Default HW)  {GPIO_CNL_LP_GPD8,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //SUS_CLK
  //(Default HW)  {GPIO_CNL_LP_GPD9,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_WLAN_N
  //(Default HW)  {GPIO_CNL_LP_GPD10,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //PM_SLP_S5_N
  //(Default HW)  {GPIO_CNL_LP_GPD11,  { GpioPadModeNative1,  GpioHostOwnGpio,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioResetDefault,  GpioTermNone }},  //LANPHY_EN
  {GPIO_CNL_LP_PECI,  { GpioHardwareDefault,  GpioHostOwnDefault,  GpioDirDefault,  GpioOutDefault,  GpioIntDefault,  GpioHostDeepReset,  GpioTermWpd20K }}, // 20K PD for PECI
};
UINT16 mGpioTableDefaultSize = sizeof (mGpioTableDefault) / sizeof (GPIO_INIT_CONFIG);
