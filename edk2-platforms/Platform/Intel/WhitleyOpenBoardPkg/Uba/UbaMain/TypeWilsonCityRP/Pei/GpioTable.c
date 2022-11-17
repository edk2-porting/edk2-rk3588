/** @file

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <Library/UbaGpioUpdateLib.h>

#include <Library/GpioLib.h>
#include <Library/UbaGpioInitLib.h>
#include <GpioPinsSklH.h>
#include <Library/PcdLib.h>

//
// Board     : Wilson City RP
//
static GPIO_INIT_CONFIG mGpioTableWilsonCityRP [] =
  {
    {GPIO_SKL_H_GPP_A0,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_0_LPC_RCIN_N_ESPI_ALERT1_N
    {GPIO_SKL_H_GPP_A1,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_1_LPC_LAD0_ESPI_IO0
    {GPIO_SKL_H_GPP_A2,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_2_LPC_LAD1_ESPI_IO1
    {GPIO_SKL_H_GPP_A3,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_3_LPC_LAD2_ESPI_IO2
    {GPIO_SKL_H_GPP_A4,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_4_LPC_LAD3_ESPI_IO3
    {GPIO_SKL_H_GPP_A5,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_5_LPC_LFRAME_N_ESPI_CS0_N
    {GPIO_SKL_H_GPP_A6,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_6_IRQ_LPC_SERIRQ_ESPI_CS1_N
    {GPIO_SKL_H_GPP_A7,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_7_IRQ_LPC_PIRQA_N_ESPI_ALERT0_N
    {GPIO_SKL_H_GPP_A8,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_8_FM_LPC_CLKRUN_N
    {GPIO_SKL_H_GPP_A9,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_9_CLK_24M_66M_LPC0_ESPI
    {GPIO_SKL_H_GPP_A10, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_10_TP_PCH_GPP_A_10
    {GPIO_SKL_H_GPP_A11, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_11_FM_LPC_PME_N
    {GPIO_SKL_H_GPP_A12, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_12_IRQ_PCH_SCI_WHEA_N
    {GPIO_SKL_H_GPP_A13, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_13_FM_EUP_LOT6_N
    {GPIO_SKL_H_GPP_A14, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_14_RST_ESPI_RESET_N
    {GPIO_SKL_H_GPP_A15, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_15_FM_SUSACK_N
    {GPIO_SKL_H_GPP_A16, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_16_TP_PCH_GPP_A_16
    {GPIO_SKL_H_GPP_A17, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_17_TP_PCH_GPP_A_16
    {GPIO_SKL_H_GPP_A18, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_18_FM_BIOS_ADV_FUNCTIONS
    {GPIO_SKL_H_GPP_A20, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_20_TP_PCH_GPP_A_20
    {GPIO_SKL_H_GPP_A21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_21_TP_PCH_GPP_A_21
    {GPIO_SKL_H_GPP_A22, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_22_TP_PCH_GPP_A_22
    {GPIO_SKL_H_GPP_A23, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_A_23_TP_PCH_GPP_A_23
    {GPIO_SKL_H_GPP_B0,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_0_FM_PCH_CORE_VID_0
    {GPIO_SKL_H_GPP_B1,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_1_FM_PCH_CORE_VID_1
    {GPIO_SKL_H_GPP_B2,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_2_PU_PCH_VRALERT_N
//   GPIO_SKL_H_GPP_B3 - Not Owned by BIOS
//   GPIO_SKL_H_GPP_B4 - Not Owned by BIOS
    {GPIO_SKL_H_GPP_B5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_5_FM_PCH_INTERPOSER_SEL1
    {GPIO_SKL_H_GPP_B6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_6_FM_PCH_INTERPOSER_SEL2
    {GPIO_SKL_H_GPP_B7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_7_TP_PCH_GPP_B_7
    {GPIO_SKL_H_GPP_B8,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_8_TP_PCH_GPP_B_8
    {GPIO_SKL_H_GPP_B9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_9_FM_BOARD_REV_ID2
    {GPIO_SKL_H_GPP_B10, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_10_FM_TPM_MOD_PRES_N
//   GPIO_SKL_H_GPP_B11 - Not Owned by BIOS
    {GPIO_SKL_H_GPP_B12, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_12_TP_SLP_S0_N
    {GPIO_SKL_H_GPP_B13, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_13_RST_PLTRST_N
    {GPIO_SKL_H_GPP_B14, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_14_FM_PCH_BIOS_RCVR_SPKR
    {GPIO_SKL_H_GPP_B15, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_15_FM_CPU_ERR0_PCH_N
    {GPIO_SKL_H_GPP_B16, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_16_FM_CPU_ERR1_PCH_N
    {GPIO_SKL_H_GPP_B17, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_17_FM_CPU_ERR2_PCH_N
    {GPIO_SKL_H_GPP_B18, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_18_FM_NO_REBOOT
    {GPIO_SKL_H_GPP_B19, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_19_FM_BOARD_SKU_ID5
    {GPIO_SKL_H_GPP_B20, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis, GpioPlatformReset, GpioTermNone, GpioPadConfigLock}},//GPP_B_20_FM_BIOS_POST_CMPLT_N
    {GPIO_SKL_H_GPP_B21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_21_TP_PCH_GPP_B_21
    {GPIO_SKL_H_GPP_B22, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_22_FM_PCH_BOOT_BIOS_DEVICE
    {GPIO_SKL_H_GPP_B23, { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_B_23_FM_PCH_BMC_THERMTRIP_EXI_STRAP_N
    {GPIO_SKL_H_GPP_C2,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_2_PU_PCH_TLS_ENABLE_STRAP
    {GPIO_SKL_H_GPP_C5,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_5_IRQ_SML0_ALERT_N
    {GPIO_SKL_H_GPP_C8,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_8_FM_PASSWORD_CLEAR_N
    {GPIO_SKL_H_GPP_C9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_9_FM_MFG_MODE
    {GPIO_SKL_H_GPP_C10, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone}},//GPP_C_10_FM_PCH_SATA_RAID_KEY
    {GPIO_SKL_H_GPP_C11, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_11_TP_FP_AUD_DETECT_N
    {GPIO_SKL_H_GPP_C12, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_12_FM_BOARD_REV_ID0
    {GPIO_SKL_H_GPP_C13, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_13_FM_BOARD_REV_ID1
    {GPIO_SKL_H_GPP_C14, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_14_FM_BMC_PCH_SCI_LPC_N
    {GPIO_SKL_H_GPP_C15, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_15_FM_RISER1_ID_0
    {GPIO_SKL_H_GPP_C16, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_16_FM_RISER1_ID_1
    {GPIO_SKL_H_GPP_C17, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_17_FM_RISER2_ID_0
    {GPIO_SKL_H_GPP_C18, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_18_FM_RISER2_ID_1
    {GPIO_SKL_H_GPP_C19, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_19_RST_SMB_HOST_PCH_MUX_N
//   GPIO_SKL_H_GPP_C20 - Not Owned by BIOS
    {GPIO_SKL_H_GPP_C21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_21_RST_PCH_MIC_MUX_N
    {GPIO_SKL_H_GPP_C22, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_22_IRQ_BMC_PCH_SMI_LPC_N
    {GPIO_SKL_H_GPP_C23, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_C_23_FM_CPU_CATERR_DLY_LVT3_N
    {GPIO_SKL_H_GPP_D0,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_0_IRQ_BMC_PCH_NMI
    {GPIO_SKL_H_GPP_D1,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_1_FP_PWR_LED_N
    {GPIO_SKL_H_GPP_D2,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_2_FM_TBT_FORCE_PWR
    {GPIO_SKL_H_GPP_D3,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_3_FM_TBT_SCI_EVENT
    {GPIO_SKL_H_GPP_D4,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_4_FM_PLD_PCH_DATA
    {GPIO_SKL_H_GPP_D5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_5_TP_PCH_GPP_D_5
    {GPIO_SKL_H_GPP_D6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_6_TP_PCH_GPP_D_6
    {GPIO_SKL_H_GPP_D7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_7_TP_PCH_GPP_D_7
    {GPIO_SKL_H_GPP_D8,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_8_FM_UPLINK_SEL
    {GPIO_SKL_H_GPP_D9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_9_TP_PCH_GPP_D_9
    {GPIO_SKL_H_GPP_D10, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_10_FM_M2_2_SSD_DEVSLP
    {GPIO_SKL_H_GPP_D11, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_11_TP_PCH_GPP_D_11
    {GPIO_SKL_H_GPP_D12, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_12_SGPIO_SSATA_DATA1
    {GPIO_SKL_H_GPP_D13, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_13_SMB_SMLINK5_STBY_LVC3_R_SCL
    {GPIO_SKL_H_GPP_D14, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_14_SMB_SMLINK5_STBY_LVC3_R_SDA
    {GPIO_SKL_H_GPP_D15, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_15_SGPIO_SSATA_DATA0
    {GPIO_SKL_H_GPP_D16, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_16_FM_ME_PFR_1
    {GPIO_SKL_H_GPP_D17, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_17_FM_ME_PFR_2
    {GPIO_SKL_H_GPP_D18, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_18_MCP_RESET_CTRL_N
    {GPIO_SKL_H_GPP_D19, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_19_FM_PS_PWROK_DLY_SEL_R
    {GPIO_SKL_H_GPP_D20, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_20_TP_PCH_GPP_D_20
    {GPIO_SKL_H_GPP_D21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_21_TP_PCH_GPP_D_21
    {GPIO_SKL_H_GPP_D22, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_22_TP_PCH_GPP_D_22
    {GPIO_SKL_H_GPP_D23, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_D_23_TP_PCH_GPP_D_23
    {GPIO_SKL_H_GPP_E0,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_0_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_E1,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_1_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_E2,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_2_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_E3,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,    GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_3_FM_ADR_TRIGGER_N
    {GPIO_SKL_H_GPP_E4,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_4_TP_PCH_GPP_E_4
    {GPIO_SKL_H_GPP_E5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_5_TP_PCH_GPP_E_5
    {GPIO_SKL_H_GPP_E6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_6_TP_PCH_GPP_E_6
    {GPIO_SKL_H_GPP_E7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_7_FM_ADR_SMI_GPIO_R_N
    {GPIO_SKL_H_GPP_E8,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_8_LED_PCH_SATA_HDD_N
    {GPIO_SKL_H_GPP_E9,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_9_FM_OC0_USB_N
    {GPIO_SKL_H_GPP_E10, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_10_FM_OC1_USB_N
    {GPIO_SKL_H_GPP_E11, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_11_PU_OC2_USB_N
    {GPIO_SKL_H_GPP_E12, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_E_12_PU_OC3_USB_N
    {GPIO_SKL_H_GPP_F0,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_0_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_F1,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_1_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_F2,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_2_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_F3,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_3_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_F4,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_4_FM_QAT_ENABLE_N
    {GPIO_SKL_H_GPP_F5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_5_IRQ_TPM_SPI_N
    {GPIO_SKL_H_GPP_F6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_6_JTAG_PCH_PLD_TCK
    {GPIO_SKL_H_GPP_F7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_7_JTAG_PCH_PLD_TDI
    {GPIO_SKL_H_GPP_F8,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_8_JTAG_PCH_PLD_TMS
    {GPIO_SKL_H_GPP_F9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_9_JTAG_PCH_PLD_TDO
    {GPIO_SKL_H_GPP_F10, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_10_SGPIO_SATA_CLOCK
    {GPIO_SKL_H_GPP_F11, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_11_SGPIO_SATA_LOAD
    {GPIO_SKL_H_GPP_F12, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_12_SGPIO_SATA_DATA1
    {GPIO_SKL_H_GPP_F13, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_13_SGPIO_SATA_DATA0
    {GPIO_SKL_H_GPP_F14, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_14_LED_PCH_SSATA_HDD_N
    {GPIO_SKL_H_GPP_F15, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_15_FM_OC4_USB_N
    {GPIO_SKL_H_GPP_F16, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_16_PU_OC5_USB_N
    {GPIO_SKL_H_GPP_F17, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_17_FM_OC6_USB_N
    {GPIO_SKL_H_GPP_F18, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_18_PU_OC7_USB_N
    {GPIO_SKL_H_GPP_F19, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_19_SMB_GBE_STBY_LVC3_SCL
    {GPIO_SKL_H_GPP_F20, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_20_SMB_GBE_STBY_LVC3_SDA
    {GPIO_SKL_H_GPP_F21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_21_TP_PCH_GPP_F_21
    {GPIO_SKL_H_GPP_F22, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_22_SGPIO_SSATA_CLOCK
    {GPIO_SKL_H_GPP_F23, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_F_23_SGPIO_SSATA_LOAD
    {GPIO_SKL_H_GPP_G0,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_0_TP_FAN_PCH_TACH0
    {GPIO_SKL_H_GPP_G1,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_1_TP_FAN_PCH_TACH1
    {GPIO_SKL_H_GPP_G2,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_2_TP_FAN_PCH_TACH2
    {GPIO_SKL_H_GPP_G3,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_3_TP_FAN_PCH_TACH3
    {GPIO_SKL_H_GPP_G4,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_4_TP_FAN_PCH_TACH4
    {GPIO_SKL_H_GPP_G5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_5_TP_FAN_PCH_TACH5
    {GPIO_SKL_H_GPP_G6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_6_TP_FAN_PCH_TACH6
    {GPIO_SKL_H_GPP_G7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_7_TP_FAN_PCH_TACH7
    {GPIO_SKL_H_GPP_G8,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_8_TP_FAN_PCH_PWM0
    {GPIO_SKL_H_GPP_G9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_9_TP_FAN_PCH_PWM1
    {GPIO_SKL_H_GPP_G10, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_10_TP_FAN_PCH_PWM2
    {GPIO_SKL_H_GPP_G11, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_11_TP_FAN_PCH_PWM3
    {GPIO_SKL_H_GPP_G12, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_12_FM_BOARD_SKU_ID0
    {GPIO_SKL_H_GPP_G13, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_13_FM_BOARD_SKU_ID1
    {GPIO_SKL_H_GPP_G14, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_14_FM_BOARD_SKU_ID2
    {GPIO_SKL_H_GPP_G15, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_15_FM_BOARD_SKU_ID3
    {GPIO_SKL_H_GPP_G16, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_16_FM_BOARD_SKU_ID4
    {GPIO_SKL_H_GPP_G17, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_17_FM_ADR_COMPLETE
    {GPIO_SKL_H_GPP_G18, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_18_IRQ_NMI_EVENT_N
    {GPIO_SKL_H_GPP_G19, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_19_IRQ_SMI_ACTIVE_N
//   GPIO_SKL_H_GPP_G20 - Not Owned by BIOS
    {GPIO_SKL_H_GPP_G21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_21_FM_BIOS_IMAGE_SWAP_N
    {GPIO_SKL_H_GPP_G22, { GpioPadModeNative3, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_22_FM_M2_2_SSD_DEVSLP
    {GPIO_SKL_H_GPP_G23, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_G_23_TP_PCH_GPP_G_23
    {GPIO_SKL_H_GPP_H0,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_0_FM_PCH_MGPIO_TEST2
    {GPIO_SKL_H_GPP_H1,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_1_FM_SWAP_OVERRIDE_N
    {GPIO_SKL_H_GPP_H2,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_2_FM_PCH_MGPIO_TEST0
    {GPIO_SKL_H_GPP_H3,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_3_FM_PCH_MGPIO_TEST1
    {GPIO_SKL_H_GPP_H4,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_4_FM_PCH_MGPIO_TEST4
    {GPIO_SKL_H_GPP_H6,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_6_FM_CLKREQ_M2_2_N
    {GPIO_SKL_H_GPP_H7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_7_FM_PCH_MGPIO_TEST3
    {GPIO_SKL_H_GPP_H8,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_8_FM_CLKREQ_NIC1_N
    {GPIO_SKL_H_GPP_H9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_9_FM_PCH_MGPIO_TEST5
//   GPIO_SKL_H_GPP_H10 - Not Owned by BIOS
//   GPIO_SKL_H_GPP_H11 - Not Owned by BIOS
    {GPIO_SKL_H_GPP_H12, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_12_FM_ESPI_FLASH_MODE
    {GPIO_SKL_H_GPP_H15, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_15_PU_ADR_TIMER_HOLD_OFF_N
    {GPIO_SKL_H_GPP_H18, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_18_FM_LT_KEY_DOWNGRADE_N
    {GPIO_SKL_H_GPP_H19, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_19_FM_PCH_10GBE_PCI_DISABLE_N
    {GPIO_SKL_H_GPP_H20, { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_20_FM_SSATA_PCIE_M2_1_SEL
    {GPIO_SKL_H_GPP_H21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_21_FM_PCH_10GBE_LAN_DISABLE_N
    {GPIO_SKL_H_GPP_H22, { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_22_FM_SSATA_PCIE_M2_2_SEL
    {GPIO_SKL_H_GPP_H23, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_H_23_TP_PCH_GPP_H_23
    {GPIO_SKL_H_GPP_I0,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_0_TP_PCH_GPP_I_0
    {GPIO_SKL_H_GPP_I1,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_1_TP_PCH_GPP_I_1
    {GPIO_SKL_H_GPP_I2,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_2_TP_PCH_GPP_I_2
    {GPIO_SKL_H_GPP_I3,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_3_TP_PCH_GPP_I_3
    {GPIO_SKL_H_GPP_I4,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_4_TP_PCH_GPP_I_4
    {GPIO_SKL_H_GPP_I5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_5_TP_PCH_GPP_I_5
    {GPIO_SKL_H_GPP_I6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_6_TP_PCH_GPP_I_6
    {GPIO_SKL_H_GPP_I7,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_7_TP_PCH_GPP_I_7
    {GPIO_SKL_H_GPP_I8,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_8_FM_PCH_10GBE_PCI_DISABLE_N
    {GPIO_SKL_H_GPP_I9,  { GpioPadModeNative2, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_9_FM_PCH_10GBE_LAN_DISABLE_N
    {GPIO_SKL_H_GPP_I10, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_I_10_TP_PCH_GPP_I_10
//   GPIO_SKL_H_GPP_I11  - Not Owned by BIOS
//   GPIO_SKL_H_GPD0 - Not Owned by BIOS
    {GPIO_SKL_H_GPD1,    { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_1_PU_ACPRESENT
    {GPIO_SKL_H_GPD2,    { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_2_FM_LAN_WAKE_N
    {GPIO_SKL_H_GPD3,    { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_3_FM_PCH_PWRBTN_N
    {GPIO_SKL_H_GPD4,    { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_4_FM_SLPS3_N
    {GPIO_SKL_H_GPD5,    { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_5_FM_SLPS4_N
    {GPIO_SKL_H_GPD6,    { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_6_FM_SLPA_N
    {GPIO_SKL_H_GPD7,    { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_7_TP_GPD_7
    {GPIO_SKL_H_GPD8,    { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_8_TP_GPD_8_SUSCLK
    {GPIO_SKL_H_GPD9,    { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_9_TP_GPD_9_SLP
    {GPIO_SKL_H_GPD10,   { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_10_FM_SLPS5_N
    {GPIO_SKL_H_GPD11,   { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPD_11_FM_PHY_DISABLE_N
    {GPIO_SKL_H_GPP_J0,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_0_TP_PCH_GPP_J_0
    {GPIO_SKL_H_GPP_J1,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_1_TP_PCH_GPP_J_1
    {GPIO_SKL_H_GPP_J2,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_2_TP_PCH_GPP_J_2
    {GPIO_SKL_H_GPP_J3,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_3_TP_PCH_GPP_J_3
    {GPIO_SKL_H_GPP_J4,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_4_TP_PCH_GPP_J_4
    {GPIO_SKL_H_GPP_J5,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_5_TP_PCH_GPP_J_5
    {GPIO_SKL_H_GPP_J6,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_6_TP_PCH_GPP_J_6
    {GPIO_SKL_H_GPP_J7,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_7_TP_PCH_GPP_J_7
    {GPIO_SKL_H_GPP_J8,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_8_TP_PCH_GPP_J_8
    {GPIO_SKL_H_GPP_J9,  { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_9_TP_PCH_GPP_J_9
    {GPIO_SKL_H_GPP_J10, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_10_TP_PCH_GPP_J_10
    {GPIO_SKL_H_GPP_J11, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_11_TP_PCH_GPP_J_11
    {GPIO_SKL_H_GPP_J12, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_12_TP_PCH_GPP_J_12
    {GPIO_SKL_H_GPP_J13, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_13_TP_PCH_GPP_J_13
    {GPIO_SKL_H_GPP_J14, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_14_TP_PCH_GPP_J_14
    {GPIO_SKL_H_GPP_J15, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_15_TP_PCH_GPP_J_15
    {GPIO_SKL_H_GPP_J16, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_16_TP_PCH_GPP_J_16
    {GPIO_SKL_H_GPP_J17, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_17_TP_PCH_GPP_J_17
    {GPIO_SKL_H_GPP_J18, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_18_TP_PCH_GPP_J_18
    {GPIO_SKL_H_GPP_J19, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_19_TP_PCH_GPP_J_19
    {GPIO_SKL_H_GPP_J20, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_20_TP_PCH_GPP_J_20
    {GPIO_SKL_H_GPP_J21, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_21_TP_PCH_GPP_J_21
    {GPIO_SKL_H_GPP_J22, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_22_TP_PCH_GPP_J_22
    {GPIO_SKL_H_GPP_J23, { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_J_23_TP_PCH_GPP_J_23
    {GPIO_SKL_H_GPP_K0,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_0_CLK_50M_CKMNG_PCH
    {GPIO_SKL_H_GPP_K1,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_1_RMII_BMC_PCH_TXD0
    {GPIO_SKL_H_GPP_K2,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_2_RMII_BMC_PCH_TXD1
    {GPIO_SKL_H_GPP_K3,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_3_RMII_BMC_PCH_TX_EN
    {GPIO_SKL_H_GPP_K4,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_4_RMII_PCH_BMC_CRS_DV
    {GPIO_SKL_H_GPP_K5,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_5_RMII_PCH_BMC_RXD0
    {GPIO_SKL_H_GPP_K6,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_6_RMII_PCH_BMC_RXD1
    {GPIO_SKL_H_GPP_K7,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_7_RMII_PCH_BMC_RX_ER
    {GPIO_SKL_H_GPP_K8,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_8_RMII_PCH_CONN_ARB_IN
    {GPIO_SKL_H_GPP_K9,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_9_RMII_PCH_CONN_ARB_OUT
    {GPIO_SKL_H_GPP_K10, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_K_10_RST_PCIE_PCH_PERST_N
//   GPIO_SKL_H_GPP_K11 - Not Owned by BIOS
    {GPIO_SKL_H_GPP_L2,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_2_TRC_2CH0_D0
    {GPIO_SKL_H_GPP_L3,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_3_TRC_2CH0_D1
    {GPIO_SKL_H_GPP_L4,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_4_TRC_2CH0_D2
    {GPIO_SKL_H_GPP_L5,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_5_TRC_2CH0_D3
    {GPIO_SKL_H_GPP_L6,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_6_TRC_2CH0_D4
    {GPIO_SKL_H_GPP_L7,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_7_TRC_2CH0_D5
    {GPIO_SKL_H_GPP_L8,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_8_TRC_2CH0_D6
    {GPIO_SKL_H_GPP_L9,  { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_9_TRC_2CH0_D7
    {GPIO_SKL_H_GPP_L10, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_10_TRC_2CH0_CLK
    {GPIO_SKL_H_GPP_L11, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_11_TRC_2CH1_D0
    {GPIO_SKL_H_GPP_L12, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_12_TRC_2CH1_D1
    {GPIO_SKL_H_GPP_L13, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_13_TRC_2CH1_D2
    {GPIO_SKL_H_GPP_L14, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_14_TRC_2CH1_D3
    {GPIO_SKL_H_GPP_L15, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_15_TRC_2CH1_D4
    {GPIO_SKL_H_GPP_L16, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_16_TRC_2CH1_D5
    {GPIO_SKL_H_GPP_L17, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_17_TRC_2CH1_D6
    {GPIO_SKL_H_GPP_L18, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_18_TRC_2CH1_D7
    {GPIO_SKL_H_GPP_L19, { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis, GpioResetPwrGood, GpioTermNone, GpioPadConfigLock}},//GPP_L_19_TRC_2CH1_CLK
};

EFI_STATUS
TypeWilsonCityRPInstallGpioData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformGpioInitDataGuid,
                                 &mGpioTableWilsonCityRP,
                                 sizeof(mGpioTableWilsonCityRP)
                                 );
  Status = PcdSet32S (PcdOemSku_GPIO_TABLE_SIZE, sizeof (mGpioTableWilsonCityRP));
  ASSERT_EFI_ERROR (Status);
  return Status;
}
