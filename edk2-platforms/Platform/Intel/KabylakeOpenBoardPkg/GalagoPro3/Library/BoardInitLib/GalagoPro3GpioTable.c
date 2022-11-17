/** @file
  GPIO definition table for GalagoPro3

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _GALAGO_PRO_3_GPIO_TABLE_H_
#define _GALAGO_PRO_3_GPIO_TABLE_H_

#include <PiPei.h>
#include <GpioPinsSklLp.h>
#include <Library/GpioLib.h>
#include <GpioConfig.h>
#include <IoExpander.h>


#define END_OF_GPIO_TABLE 0xFFFFFFFF

GPIO_INIT_CONFIG mGpioTableGalagoPro3[] =
{
  {GPIO_SKL_LP_GPP_A0, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirIn,      GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //RCINB_TIME_SYNC_1
  {GPIO_SKL_LP_GPP_A1, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirInOut,   GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNative}}, //LAD_0_ESPI_IO_0
  {GPIO_SKL_LP_GPP_A2, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirInOut,   GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //LAD_1_ESPI_IO_1
  {GPIO_SKL_LP_GPP_A3, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirInOut,   GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNative}}, //LAD_2_ESPI_IO_2
  {GPIO_SKL_LP_GPP_A4, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirInOut,   GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //LAD_3_ESPI_IO_3
  {GPIO_SKL_LP_GPP_A5, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //LFRAMEB_ESPI_CSB
  {GPIO_SKL_LP_GPP_A6, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirInOut,   GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SERIRQ
  {GPIO_SKL_LP_GPP_A7, {GpioPadModeGpio,     GpioHostOwnGpio,    GpioDirIn,      GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //PIRQAB_GSPI0_CS1B
  {GPIO_SKL_LP_GPP_A8, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CLKRUNB
  {GPIO_SKL_LP_GPP_A9, {GpioPadModeNative1,  GpioHostOwnDefault, GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K}}, //CLKOUT_LPC_0_ESPI_CLK
  {GPIO_SKL_LP_GPP_A10, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K}}, //CLKOUT_LPC_1
  {GPIO_SKL_LP_GPP_A11, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpu20K}}, //PMEB_GSPI1_CS1B
  {GPIO_SKL_LP_GPP_A12, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //BM_BUSYB_ISH_GP_6
  {GPIO_SKL_LP_GPP_A13, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SUSWARNB_SUSPWRDNACK
  {GPIO_SKL_LP_GPP_A14, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SUS_STATB_ESPI_RESETB
  {GPIO_SKL_LP_GPP_A15, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirIn,      GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpu20K}}, //SUSACKB
  {GPIO_SKL_LP_GPP_A16, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SD_1P8_SEL
  {GPIO_SKL_LP_GPP_A17, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SD_VDD1_PWR_EN_B_ISH_GP_7
  {GPIO_SKL_LP_GPP_A18, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_GP_0
  {GPIO_SKL_LP_GPP_A19, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirOut,     GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_GP_1
  {GPIO_SKL_LP_GPP_A20, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_GP_2
  {GPIO_SKL_LP_GPP_A21, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioPlatformReset, GpioTermNone}}, //ISH_GP_3
  {GPIO_SKL_LP_GPP_A22, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioResumeReset,   GpioTermNone}}, //ISH_GP_4
  {GPIO_SKL_LP_GPP_A23, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioResumeReset,   GpioTermNone}}, //ISH_GP_5
  {GPIO_SKL_LP_GPP_B0,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CORE_VID_0
  {GPIO_SKL_LP_GPP_B1,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CORE_VID_1
  {GPIO_SKL_LP_GPP_B2,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //VRALERTB
  {GPIO_SKL_LP_GPP_B3,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CPU_GP_2
  {GPIO_SKL_LP_GPP_B4,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CPU_GP_3
  {GPIO_SKL_LP_GPP_B5,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SRCCLKREQB_0
  {GPIO_SKL_LP_GPP_B6,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SRCCLKREQB_1
  {GPIO_SKL_LP_GPP_B7,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SRCCLKREQB_2
  {GPIO_SKL_LP_GPP_B8,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SRCCLKREQB_3
  {GPIO_SKL_LP_GPP_B9,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutLow,     GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SRCCLKREQB_4
  {GPIO_SKL_LP_GPP_B10, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SRCCLKREQB_5
  {GPIO_SKL_LP_GPP_B11, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EXT_PWR_GATEB
  {GPIO_SKL_LP_GPP_B12, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SLP_S0B
  {GPIO_SKL_LP_GPP_B13, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //PLTRSTB
  {GPIO_SKL_LP_GPP_B14, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K}}, //SPKR
  {GPIO_SKL_LP_GPP_B15, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GSPI0_CS0B
  {GPIO_SKL_LP_GPP_B16, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GSPI0_CLK
  {GPIO_SKL_LP_GPP_B17, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GSPI0_MISO
  {GPIO_SKL_LP_GPP_B18, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpu20K}}, //GSPI0_MOSI
  {GPIO_SKL_LP_GPP_B19, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GSPI1_CS0B
  {GPIO_SKL_LP_GPP_B20, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GSPI1_CLK_NFC_CLK
  {GPIO_SKL_LP_GPP_B21, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GSPI1_MISO_NFC_CLKREQ
  {GPIO_SKL_LP_GPP_B22, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K}}, //GSPI1_MOSI
  {GPIO_SKL_LP_GPP_B23, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SML1ALERTB_PCHHOTB
  {GPIO_SKL_LP_GPP_G0,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_0_SD3_CMD
  {GPIO_SKL_LP_GPP_G1,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirIn,      GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_1_SD3_D0_SD4_RCLK_P
  {GPIO_SKL_LP_GPP_G2,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_2_SD3_D1_SD4_RCLK_N
  {GPIO_SKL_LP_GPP_G3,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_3_SD3_D2
  {GPIO_SKL_LP_GPP_G4,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_4_SD3_D3
  {GPIO_SKL_LP_GPP_G5,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_5_SD3_CDB
  {GPIO_SKL_LP_GPP_G6,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_6_SD3_CLK
  {GPIO_SKL_LP_GPP_G7,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //GPPC_G_7_SD3_WP
  {GPIO_SKL_LP_GPP_D0,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SPI1_CSB_BK_0
  {GPIO_SKL_LP_GPP_D1,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SPI1_CLK_BK_1
  {GPIO_SKL_LP_GPP_D2,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SPI1_MISO_IO_1_BK_2
  {GPIO_SKL_LP_GPP_D3,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SPI1_MOSI_IO_0_BK_3
  {GPIO_SKL_LP_GPP_D4,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //IMGCLKOUT_0_BK_4
  {GPIO_SKL_LP_GPP_D5,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_I2C0_SDA
  {GPIO_SKL_LP_GPP_D6,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_I2C0_SCL
  {GPIO_SKL_LP_GPP_D7,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_I2C1_SDA
  {GPIO_SKL_LP_GPP_D8,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_I2C1_SCL
  {GPIO_SKL_LP_GPP_D9,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_SPI_CSB
  {GPIO_SKL_LP_GPP_D10, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_SPI_CLK
  {GPIO_SKL_LP_GPP_D11, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_SPI_MISO_GP_BSSB_CLK
  {GPIO_SKL_LP_GPP_D12, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_SPI_MOSI_GP_BSSB_DI
  {GPIO_SKL_LP_GPP_D13, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_UART0_RXD_SML0BDATA
  {GPIO_SKL_LP_GPP_D14, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_UART0_TXD_SML0BCLK
  {GPIO_SKL_LP_GPP_D15, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_UART0_RTSB_GSPI2_CS1B
  {GPIO_SKL_LP_GPP_D16, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //ISH_UART0_CTSB_SML0BALERTB
  {GPIO_SKL_LP_GPP_D17, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DMIC_CLK_1_SNDW3_CLK
  {GPIO_SKL_LP_GPP_D18, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DMIC_DATA_1_SNDW3_DATA
  {GPIO_SKL_LP_GPP_D19, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DMIC_CLK_0_SNDW4_CLK
  {GPIO_SKL_LP_GPP_D20, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DMIC_DATA_0_SNDW4_DATA
  {GPIO_SKL_LP_GPP_D21, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirIn,      GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SPI1_IO_2
  {GPIO_SKL_LP_GPP_D22, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SPI1_IO_3
  {GPIO_SKL_LP_GPP_D23, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SSP_MCLK
  {GPIO_SKL_LP_GPP_F0,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_GNSS_PA_BLANKING
  {GPIO_SKL_LP_GPP_F1,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_GNSS_FTA
  {GPIO_SKL_LP_GPP_F2,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_GNSS_SYSCK
  {GPIO_SKL_LP_GPP_F3,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //
  {GPIO_SKL_LP_GPP_F4,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_BRI_DT_UART0_RTSB
  {GPIO_SKL_LP_GPP_F5,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_BRI_RSP_UART0_RXD
  {GPIO_SKL_LP_GPP_F6,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_RGI_DT_UART0_TXD
  {GPIO_SKL_LP_GPP_F7,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_RGI_RSP_UART0_CTSB
  {GPIO_SKL_LP_GPP_F8,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_MFUART2_RXD
  {GPIO_SKL_LP_GPP_F9,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CNV_MFUART2_TXD
  {GPIO_SKL_LP_GPP_F10, {GpioPadModeNative2, GpioHostOwnDefault, GpioDirDefault, GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //
  {GPIO_SKL_LP_GPP_F11, {GpioPadModeNative2, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_CMD
  {GPIO_SKL_LP_GPP_F12, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA0
  {GPIO_SKL_LP_GPP_F13, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA1
  {GPIO_SKL_LP_GPP_F14, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA2
  {GPIO_SKL_LP_GPP_F15, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA3
  {GPIO_SKL_LP_GPP_F16, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA4
  {GPIO_SKL_LP_GPP_F17, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA5
  {GPIO_SKL_LP_GPP_F18, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA6
  {GPIO_SKL_LP_GPP_F19, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_DATA7
  {GPIO_SKL_LP_GPP_F20, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_RCLK
  {GPIO_SKL_LP_GPP_F21, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_CLK
  {GPIO_SKL_LP_GPP_F22, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EMMC_RESETB
  {GPIO_SKL_LP_GPP_F23, {GpioPadModeGpio,    GpioHostOwnAcpi,    GpioDirIn,      GpioOutDefault, GpioIntLevel | GpioIntApic, GpioHostDeepReset, GpioTermNone}}, //A4WP_PRESENT
  {GPIO_SKL_LP_GPD0,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //BATLOWB
  {GPIO_SKL_LP_GPD1,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNative}}, //ACPRESENT
  {GPIO_SKL_LP_GPD2,    {GpioPadModeNative1, GpioHostOwnAcpi,    GpioDirInInv,   GpioOutDefault, GpioIntLevel | GpioIntSci, GpioResetDefault, GpioTermNone}}, //LAN_WAKEB
  {GPIO_SKL_LP_GPD3,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermWpu20K}}, //PWRBTNB
  {GPIO_SKL_LP_GPD4,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //SLP_S3B
  {GPIO_SKL_LP_GPD5,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //SLP_S4B
  {GPIO_SKL_LP_GPD6,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //SLP_AB
  {GPIO_SKL_LP_GPD7,    {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //
  {GPIO_SKL_LP_GPD8,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //SUSCLK
  {GPIO_SKL_LP_GPD9,    {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //SLP_WLANB
  {GPIO_SKL_LP_GPD10,   {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //SLP_S5B
  {GPIO_SKL_LP_GPD11,   {GpioPadModeNative1, GpioHostOwnDefault, GpioDirIn,      GpioOutDefault, GpioIntDis, GpioResetDefault, GpioTermNone}}, //LANPHYPC
  {GPIO_SKL_LP_GPP_C0,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SMBCLK
  {GPIO_SKL_LP_GPP_C1,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SMBDATA
  {GPIO_SKL_LP_GPP_C2,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermWpd20K}}, //SMBALERTB
  {GPIO_SKL_LP_GPP_C3,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SML0CLK
  {GPIO_SKL_LP_GPP_C4,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SML0DATA
  {GPIO_SKL_LP_GPP_C5,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SML0ALERTB
  {GPIO_SKL_LP_GPP_C6,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SML1CLK
  {GPIO_SKL_LP_GPP_C7,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SML1DATA
  {GPIO_SKL_LP_GPP_C8,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART0_RXD
  {GPIO_SKL_LP_GPP_C9,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART0_TXD
  {GPIO_SKL_LP_GPP_C10, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART0_RTSB
  {GPIO_SKL_LP_GPP_C11, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART0_CTSB
  {GPIO_SKL_LP_GPP_C12, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART1_RXD_ISH_UART1_RXD
  {GPIO_SKL_LP_GPP_C13, {GpioPadModeGpio,    GpioHostOwnAcpi,    GpioDirInInv,   GpioOutDefault, GpioIntEdge | GpioIntSci, GpioPlatformReset, GpioTermNone}}, //UART1_TXD_ISH_UART1_TXD
  {GPIO_SKL_LP_GPP_C14, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART1_RTSB_ISH_UART1_RTSB
  {GPIO_SKL_LP_GPP_C15, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART1_CTSB_ISH_UART1_CTSB
  {GPIO_SKL_LP_GPP_C16, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutLow,     GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //I2C0_SDA
  {GPIO_SKL_LP_GPP_C17, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //I2C0_SCL
  {GPIO_SKL_LP_GPP_C18, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //I2C1_SDA
  {GPIO_SKL_LP_GPP_C19, {GpioPadModeGpio,    GpioHostOwnAcpi,    GpioDirInInv,   GpioOutDefault, GpioIntLevel | GpioIntSci, GpioHostDeepReset, GpioTermNone}}, //I2C1_SCL
  {GPIO_SKL_LP_GPP_C20, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutLow,     GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART2_RXD
  {GPIO_SKL_LP_GPP_C21, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART2_TXD
  {GPIO_SKL_LP_GPP_C22, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART2_RTSB
  {GPIO_SKL_LP_GPP_C23, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //UART2_CTSB
  {GPIO_SKL_LP_GPP_E0,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirIn,      GpioOutDefault, GpioIntEdge | GpioIntApic, GpioHostDeepReset, GpioTermWpd20K}}, //SATAXPCIE_0_SATAGP_0
  {GPIO_SKL_LP_GPP_E1,  {GpioPadModeNative1, GpioHostOwnGpio,    GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SATAXPCIE_1_SATAGP_1
  {GPIO_SKL_LP_GPP_E2,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirIn,      GpioOutLow,     GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SATAXPCIE_2_SATAGP_2
  {GPIO_SKL_LP_GPP_E3,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirInOut,   GpioOutLow,     GpioIntLevel | GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CPU_GP_0
  {GPIO_SKL_LP_GPP_E4,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResumeReset, GpioTermNone}}, //SATA_DEVSLP_0
  {GPIO_SKL_LP_GPP_E5,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioResumeReset, GpioTermNone}}, //SATA_DEVSLP_1
  {GPIO_SKL_LP_GPP_E6,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SATA_DEVSLP_2
  {GPIO_SKL_LP_GPP_E7,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirIn,      GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //CPU_GP_1
  {GPIO_SKL_LP_GPP_E8,  {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //SATA_LEDB
  {GPIO_SKL_LP_GPP_E9,  {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //USB2_OCB_0_GP_BSSB_CLk
  {GPIO_SKL_LP_GPP_E10, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //USB2_OCB_1_GP_BSSB_DI
  {GPIO_SKL_LP_GPP_E11, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //USB2_OCB_2
  {GPIO_SKL_LP_GPP_E12, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //USB2_OCB_3
  {GPIO_SKL_LP_GPP_E13, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDSP_HPD_0_DISP_MISC_0
  {GPIO_SKL_LP_GPP_E14, {GpioPadModeNative1, GpioHostOwnGpio,    GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDSP_HPD_1_DISP_MISC_1
  {GPIO_SKL_LP_GPP_E15, {GpioPadModeGpio,    GpioHostOwnAcpi,    GpioDirInInv,   GpioOutDefault, GpioIntEdge | GpioIntSmi, GpioHostDeepReset, GpioTermNone}}, //DDSP_HPD_2_DISP_MISC_2
  {GPIO_SKL_LP_GPP_E16, {GpioPadModeGpio,    GpioHostOwnAcpi,    GpioDirInInv,   GpioOutDefault, GpioIntLevel | GpioIntSci, GpioPlatformReset, GpioTermNone}}, //DDSP_HPD_3_DISP_MISC_3
  {GPIO_SKL_LP_GPP_E17, {GpioPadModeNative1, GpioHostOwnGpio,    GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //EDP_HPD_DISP_MISC_4
  {GPIO_SKL_LP_GPP_E18, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirDefault, GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDPB_CTRLCLK
  {GPIO_SKL_LP_GPP_E19, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDPB_CTRLDATA
  {GPIO_SKL_LP_GPP_E20, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDPC_CTRLCLK
  {GPIO_SKL_LP_GPP_E21, {GpioPadModeNative1, GpioHostOwnDefault, GpioDirNone,    GpioOutDefault, GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDPC_CTRLDATA
  {GPIO_SKL_LP_GPP_E22, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirInOut,   GpioOutDefault, GpioIntLevel | GpioIntApic, GpioHostDeepReset, GpioTermNone}}, //DDPD_CTRLCLK
  {GPIO_SKL_LP_GPP_E23, {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirOut,     GpioOutHigh,    GpioIntDis, GpioHostDeepReset, GpioTermNone}}, //DDPD_CTRLDATA
  {END_OF_GPIO_TABLE,   {GpioPadModeGpio,    GpioHostOwnGpio,    GpioDirNone,    GpioOutDefault, GpioIntDis, GpioDswReset,  GpioTermNone}},//Marking End of Table
};

UINT16 mGpioTableGalagoPro3Size = sizeof (mGpioTableGalagoPro3) / sizeof (GPIO_INIT_CONFIG) - 1;

GPIO_INIT_CONFIG mGpioTableGalagoPro3UcmcDevice[] =
{
    { GPIO_SKL_LP_GPP_B0, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntLevel | GpioIntApic, GpioHostDeepReset, GpioTermNone } }, //GPP_B0
    { GPIO_SKL_LP_GPP_B1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntLevel | GpioIntApic, GpioHostDeepReset, GpioTermNone } }, //GPP_B1
};

UINT16 mGpioTableGalagoPro3UcmcDeviceSize = sizeof (mGpioTableGalagoPro3UcmcDevice) / sizeof (GPIO_INIT_CONFIG);

GPIO_INIT_CONFIG mGpioTableGalagoPro3Touchpanel =
  {GPIO_SKL_LP_GPP_E7, {GpioPadModeGpio,    GpioHostOwnGpio, GpioDirIn,    GpioOutDefault, GpioIntLevel | GpioIntApic, GpioPlatformReset,  GpioTermNone}};

GPIO_INIT_CONFIG mGpioTableGalagoPro3SdhcSidebandCardDetect =
  {GPIO_SKL_LP_GPP_B17, {GpioPadModeGpio,   GpioHostOwnGpio, GpioDirIn,   GpioOutDefault, GpioIntBothEdge, GpioHostDeepReset,  GpioTermNone}}; //SD_CDB D3

//IO Expander Table for SKL RVP7, RVP13 and RVP15
IO_EXPANDER_GPIO_CONFIG mGpioTableIoExpander[] =
{
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_3.3_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SNSR_HUB_DFU_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SATA_PWR_EN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WLAN_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//GFX_CRB_DET_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//MFG_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//FLIP_TO_TABLET_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_8,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_SLOT1_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_9,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB3_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_10, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD_TESTMODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_11, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//BIOS_REC_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_12, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//EINK_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_13, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TBT_FORCE_PWR_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_14, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIFI_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_15, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//DGPU_PRSNT_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_16, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCIE_SLOT1_PWREN_WKCTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_17, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB2_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_18, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//IMAGING_DFU_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_19, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SW_GFX_PWERGD_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_20, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_21, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_22, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TP_IOEXP1_P26
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_23, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TP_IOEXP1_P27
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP4_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_OTG_WP1_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP2_WP3_WP5_PWREN_R_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_AUDIO_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_GNSS_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED}//M.2_WIGIG_PWREN_IOEXP
};

UINT16 mGpioTableIoExpanderSize = sizeof (mGpioTableIoExpander) / sizeof (IO_EXPANDER_GPIO_CONFIG);

//IO Expander Table for KBL -Refresh
IO_EXPANDER_GPIO_CONFIG mGpioTableIoExpanderKabylakeRDdr4[] =
{
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_3.3_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SNSR_HUB_DFU_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SATA_PWR_EN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WLAN_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//GFX_CRB_DET_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//MFG_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//FLIP_TO_TABLET_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_8,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_SLOT1_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_9,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB3_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_10, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD_TESTMODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_11, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//BIOS_REC_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_12, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//Unused pin
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_13, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TBT_FORCE_PWR_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_14, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIFI_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_15, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RTD3_USB_PD1_PWR_EN
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_16, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCIE_SLOT1_PWREN_WKCTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_17, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB2_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_18, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//IMAGING_DFU_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_19, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//HRESET_PD1_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_20, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_21, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_RST_IOEXP_N
  //{IO_EXPANDER_0, IO_EXPANDER_GPIO_22, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_RST_CNTRL_R
  // We want the initial state to be high.
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_22, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_RST_CNTRL_R
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_23, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_WAKE_CTRL_R_N
  // Turn off WWAN power and will turn it on later.
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP2_WP3_WP5_PWREN_R_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_AUDIO_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_GNSS_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_PWREN_IOEXP
};
UINT16 mGpioTableIoExpanderSizeKabylakeRDdr4 = sizeof (mGpioTableIoExpanderKabylakeRDdr4) / sizeof (IO_EXPANDER_GPIO_CONFIG);

//IO Expander Table for KBL -kc
IO_EXPANDER_GPIO_CONFIG mGpioTableIoExpanderKabylakeKcDdr3[] =
{
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_3.3_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SNSR_HUB_DFU_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SATA_PWR_EN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WLAN_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//GFX_CRB_DET_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//MFG_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//FLIP_TO_TABLET_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_8,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_SLOT1_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_9,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB3_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_10, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD_TESTMODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_11, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//BIOS_REC_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_12, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//EINK_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_13, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TBT_FORCE_PWR_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_14, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIFI_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_15, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_16, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCIE_SLOT1_PWREN_WKCTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_17, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB2_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_18, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//IMAGING_DFU_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_19, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_20, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_21, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_22, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TP_IOEXP1_P26
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_23, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TP_IOEXP1_P27
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP4_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_OTG_WP1_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP2_WP3_WP5_PWREN_R_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_AUDIO_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_GNSS_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_8,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//FPS_LOCK_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_9,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_FLEX_PWREN
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_10, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB_UART_SEL
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_11, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_DOCK_PWREN_IOEXP_R
};
UINT16 mGpioTableIoExpanderSizeKabylakeKcDdr3 = sizeof (mGpioTableIoExpanderKabylakeKcDdr3) / sizeof (IO_EXPANDER_GPIO_CONFIG);
//IO Expander Table Full table for N 1XX WU
IO_EXPANDER_GPIO_CONFIG mGpioTableIoExpanderGalagoPro3[] =
{
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_3.3_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SNSR_HUB_DFU_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SATA_PWR_EN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WLAN_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//GFX_CRB_DET_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//MFG_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//FLIP_TO_TABLET_MODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_8,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_SLOT1_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_9,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB3_CAM_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_10, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//RSVD_TESTMODE_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_11, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//BIOS_REC_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_12, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//EINK_PWREN_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_13, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TBT_FORCE_PWR_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_14, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIFI_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_15, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//DGPU_PRSNT_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_16, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCIE_SLOT1_PWREN_WKCTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_17, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED },//SW_GFX_DGPU_SEL (KBL_RVP3_BOARD)
//{IO_EXPANDER_0, IO_EXPANDER_GPIO_17, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB2_CAM_PWREN_IOEXP (SKL_RVP3_BOARD)
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_18, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//IMAGING_DFU_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_19, IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//SW_GFX_PWERGD_IOEXP
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_20, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_WAKE_CTRL_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_21, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_SSD_RST_IOEXP_N
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_22, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TP_IOEXP1_P26
  {IO_EXPANDER_0, IO_EXPANDER_GPIO_23, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//TP_IOEXP1_P27
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_0,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_1,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WWAN_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_2,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP4_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_INPUT,  IO_EXPANDER_GPO_LEVEL_LOW,  IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED },//Not Connected (KBK_RVP3_BOARD)
//{IO_EXPANDER_1, IO_EXPANDER_GPIO_3,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_OTG_WP1_PWREN_IOEXP (SKL_RVP3_BOARD)
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_4,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB32_WP2_WP3_WP5_PWREN_R_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_5,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//PCH_AUDIO_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_6,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_GNSS_DISABLE_IOEXP_N
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_7,  IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//M.2_WIGIG_PWREN_IOEXP
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_17, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//USB2_CAM_PWREN (KBL_RVP3_BOARD)
  {IO_EXPANDER_1, IO_EXPANDER_GPIO_22, IO_EXPANDER_GPIO_OUTPUT, IO_EXPANDER_GPO_LEVEL_HIGH, IO_EXPANDER_GPI_INV_DISABLED, IO_EXPANDER_GPIO_RESERVED},//FPS_LOCK_N  (KBL_RVP3_BOARD)
};

UINT16 mGpioTableIoExpanderGalagoPro3Size = sizeof (mGpioTableIoExpanderGalagoPro3) / sizeof (IO_EXPANDER_GPIO_CONFIG);

#endif // _GALAGO_PRO_3_GPIO_TABLE_H_
