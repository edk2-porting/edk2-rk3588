/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>

#include <Library/GpioLib.h>
#include <GpioPinsSklH.h>
#include <Library/PcdLib.h>

#include <Register/PchRegsPcr.h>
#include <Library/PchPcrLib.h>
#include <Register/PchRegsSpi.h>

GPIO_INIT_CONFIG mGpioTableMicrosoftWcs[] =
{
// Group A
    {GPIO_SKL_H_GPP_A0, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInInv, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone }},//GPP_A_0_LPC_RCIN_N_ESPI_ALERT1_N
    { GPIO_SKL_H_GPP_A1, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_1_LAD_0_ESPI_IO_0
    { GPIO_SKL_H_GPP_A2, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_2_LAD_1_ESPI_IO_1
    { GPIO_SKL_H_GPP_A3, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_3_LAD_2_ESPI_IO_2
    { GPIO_SKL_H_GPP_A4, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_4_LAD_3_ESPI_IO_3
    { GPIO_SKL_H_GPP_A5, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_5_LPC_LFRAME_N_ESPI_CS0_N
    { GPIO_SKL_H_GPP_A6, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_6_IRQ_LPC_SERIRQ_ESPI_CS1_N
    { GPIO_SKL_H_GPP_A7, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_7_IRQ_LPC_PIRQA_N_ESPI_ALERT0_N
    { GPIO_SKL_H_GPP_A8, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_8_FM_LPC_CLKRUN_N
    { GPIO_SKL_H_GPP_A9, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_9_CLK_24M_66M_LPC0_ESPI
    { GPIO_SKL_H_GPP_A10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_10_TP_PCH_GPP_A_10
    { GPIO_SKL_H_GPP_A11, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_11_FM_LPC_PME_N
    { GPIO_SKL_H_GPP_A12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirInInv, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_A_12_IRQ_PCH_SCI_WHEA_N
    { GPIO_SKL_H_GPP_A13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_A_13_FM_EUP_LOT6_N MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_A14, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_14_ESPI_RESET_N
    { GPIO_SKL_H_GPP_A15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_15_SUSACK_N
    { GPIO_SKL_H_GPP_A16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_16_TP_PCH_GPP_A_16
    { GPIO_SKL_H_GPP_A17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_17_FM_KTI_SLOW_MODE_N
    { GPIO_SKL_H_GPP_A18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_A_18_FM_BIOS_ADV_FUNCTIONS
    //{GPIO_SKL_H_GPP_A19, { GpioPadModeGpio,    GpioHostOwnGpio, GpioDirIn,    GpioOutDefault, GpioIntDis,GpioResetNormal,GpioTermNone}},//GPP_A_19_FM_ME_RCVR_N
    { GPIO_SKL_H_GPP_A20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_A_20_TP_PCH_GPP_A_20 MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_A21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_21_TP_PCH_GPP_A_21
    { GPIO_SKL_H_GPP_A22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_22_TP_PCH_GPP_A_22
    { GPIO_SKL_H_GPP_A23, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_A_23_TP_PCH_GPP_A_23
    // Group B
    { GPIO_SKL_H_GPP_B0, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_0_CORE_VID_0
    { GPIO_SKL_H_GPP_B1, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_1_CORE_VID_1
    { GPIO_SKL_H_GPP_B2, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_2_VRALERT_N
    { GPIO_SKL_H_GPP_B3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_3_FM_QAT_ENABLE_N
    { GPIO_SKL_H_GPP_B4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_4_TP_PCH_GPP_B_4
    { GPIO_SKL_H_GPP_B5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_5_PU_PCH_GPP_B_5
    { GPIO_SKL_H_GPP_B6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_B_6_PU_PCH_GPP_B_6
    { GPIO_SKL_H_GPP_B7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_B_7_PU_PCH_GPP_B_7
    { GPIO_SKL_H_GPP_B8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_8_PU_PCH_GPP_B_8
    { GPIO_SKL_H_GPP_B9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_9_PU_PCH_GPP_B_9
    { GPIO_SKL_H_GPP_B10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_10_PU_PCH_GPP_B_10
    { GPIO_SKL_H_GPP_B11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_11_FM_PMBUS_ALERT_B_EN
    { GPIO_SKL_H_GPP_B12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_12_TP_SLP_S0_N
    { GPIO_SKL_H_GPP_B13, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_13_RST_PLTRST_N
    { GPIO_SKL_H_GPP_B14, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_B_14_FM_PCH_BIOS_RCVR_SPKR
    { GPIO_SKL_H_GPP_B15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_15_FM_CPU_ERR0_LVT3_N
    { GPIO_SKL_H_GPP_B16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_16_FM_CPU_ERR1_LVT3_N
    { GPIO_SKL_H_GPP_B17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_17_TP_PCH_GPP_B_17
    { GPIO_SKL_H_GPP_B18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_B_18_PU_NO_REBOOT MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_B19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_19_TP_PCH_GPP_B_19
    { GPIO_SKL_H_GPP_B20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_B_20_FM_BIOS_POST_CMPLT_N
    { GPIO_SKL_H_GPP_B21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_21_TP_LINK_WIDTH_ID5
    { GPIO_SKL_H_GPP_B22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_B_22_FM_PCH_BOOT_BIOS_DEVICE
    { GPIO_SKL_H_GPP_B23, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_B_23_FM_PCH_BMC_THERMTRIP_EXI_STRAP_N
    // Group C
    { GPIO_SKL_H_GPP_C0, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_0_SMB_HOST_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_C1, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_1_SMB_HOST_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_C2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_2_PU_PCH_TLS_ENABLE_STRAP
    { GPIO_SKL_H_GPP_C3, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_3_SMB_SMLINK0_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_C4, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_4_SMB_SMLINK0_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_C5, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_C_5_IRQ_SML0_ALERT_N MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_C6, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_6_SMB_PMBUS_SML1_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_C7, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_7_SMB_PMBUS_SML1_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_C8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_8_FM_PASSWORD_CLEAR_N
    { GPIO_SKL_H_GPP_C9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_9_FM_MFG_MODE
    { GPIO_SKL_H_GPP_C10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_10_FM_PCH_SATA_RAID_KEY
    { GPIO_SKL_H_GPP_C11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_11_TP_FP_AUD_DETECT_N
    { GPIO_SKL_H_GPP_C12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_12_FM_BOARD_REV_ID0
    { GPIO_SKL_H_GPP_C13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_C_13_FM_BOARD_REV_ID1
    { GPIO_SKL_H_GPP_C14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntLevel | GpioIntSci, GpioResetNormal, GpioTermNone } },//GPP_C_14_FM_BMC_PCH_SCI_LPC_N
    { GPIO_SKL_H_GPP_C15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_15_TP_LINK_WIDTH_ID0
    { GPIO_SKL_H_GPP_C16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_16_TP_LINK_WIDTH_ID1
    { GPIO_SKL_H_GPP_C17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_17_TP_LINK_WIDTH_ID2
    { GPIO_SKL_H_GPP_C18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_18_TP_LINK_WIDTH_ID3
    { GPIO_SKL_H_GPP_C19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_19_TP_LINK_WIDTH_ID4 , MSFT_WCS_override for AVA Slot3_PRSNT_N_2_4
    { GPIO_SKL_H_GPP_C20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_20_FM_THROTTLE_N
    { GPIO_SKL_H_GPP_C21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_C_21_RST_PCH_MIC_MUX_N
    { GPIO_SKL_H_GPP_C22, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntLevel | GpioIntSmi, GpioResetNormal, GpioTermNone } },//GPP_C_22_IRQ_BMC_PCH_SMI_LPC_N
    { GPIO_SKL_H_GPP_C23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntLevel | GpioIntSmi, GpioResetNormal, GpioTermNone } },//GPP_C_23_FM_CPU_CATERR_DLY_LVT3_N
    // Group D
    { GPIO_SKL_H_GPP_D0, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutDefault, GpioIntLevel | GpioIntNmi, GpioResetNormal, GpioTermNone } },//GPP_D_0_IRQ_BMC_PCH_NMI
    { GPIO_SKL_H_GPP_D1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_1_FP_PWR_LED_N
    { GPIO_SKL_H_GPP_D2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_D_2_FM_TBT_FORCE_PWR
    { GPIO_SKL_H_GPP_D3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_D_3 [PVDDQ_KLM_PINALERT_N]
    { GPIO_SKL_H_GPP_D4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_D_4_FM_PLD_PCH_DATA
    { GPIO_SKL_H_GPP_D5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_5_TP_PCH_GPP_D_5
    { GPIO_SKL_H_GPP_D6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_6_TP_PCH_GPP_D_6
    { GPIO_SKL_H_GPP_D7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_7_TP_PCH_GPP_D_7
    { GPIO_SKL_H_GPP_D8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_8_TP_PCH_GPP_D_8
    { GPIO_SKL_H_GPP_D9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_9_TP_PCH_GPP_D_9
    { GPIO_SKL_H_GPP_D10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_10_FM_M2_SSD_DEVSLP , MSFT_WCS_override for AVA Slot3_PRSNT_N_2_6
    { GPIO_SKL_H_GPP_D11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_11_FM_LA_TRIGGER_N
    { GPIO_SKL_H_GPP_D12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_12_SGPIO_SSATA_DATA1
    { GPIO_SKL_H_GPP_D13, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_13_SMB_SMLINK5_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_D14, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_14_SMB_SMLINK5_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_D15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_15_SSATA_SDATAOUT0
    { GPIO_SKL_H_GPP_D16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_16_TP_PCH_GPP_D_16
    { GPIO_SKL_H_GPP_D17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_17_TP_PCH_GPP_D_17
    { GPIO_SKL_H_GPP_D18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_D_18_TP_PCH_GPP_D_18
    { GPIO_SKL_H_GPP_D19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_D_19_FM_PS_PWROK_DLY_SEL
    { GPIO_SKL_H_GPP_D20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_20_TP_PCH_GPP_D_20
    { GPIO_SKL_H_GPP_D21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_21_SPC_IE_LVC3_RX
    { GPIO_SKL_H_GPP_D22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_22_SPC_IE_LVC3_TX
    { GPIO_SKL_H_GPP_D23, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_D_23_TP_PCH_GPP_D_23
    // Group E
    { GPIO_SKL_H_GPP_E0, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_0_TP_PCH_GPP_E_0
    { GPIO_SKL_H_GPP_E1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_1_TP_PCH_GPP_E_1
    { GPIO_SKL_H_GPP_E2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_2_TP_PCH_GPP_E_2
    { GPIO_SKL_H_GPP_E3, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_3_FM_ADR_TRIGGER_N
    { GPIO_SKL_H_GPP_E4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_4_TP_PCH_GPP_E_4
    { GPIO_SKL_H_GPP_E5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_5_TP_PCH_GPP_E_5
    { GPIO_SKL_H_GPP_E6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_6_TP_PCH_GPP_E_6
    { GPIO_SKL_H_GPP_E7, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutLow, GpioIntLevel | GpioIntSmi, GpioResetNormal, GpioTermNone } },//GPP_E_7_FM_ADR_SMI_GPIO_N MSFT_WCS_override: INT config and reset type
    { GPIO_SKL_H_GPP_E8, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_8_LED_PCH_SATA_HDD_N
    { GPIO_SKL_H_GPP_E9, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_9_FM_OC0_USB_N
    { GPIO_SKL_H_GPP_E10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_10_FM_OC1_USB_N
    { GPIO_SKL_H_GPP_E11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_11_FM_OC2_USB_N
    { GPIO_SKL_H_GPP_E12, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_E_12_FM_OC3_USB_N
    // Group F
    { GPIO_SKL_H_GPP_F0, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_0_TP_PCH_GPP_F_0
    { GPIO_SKL_H_GPP_F1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_1_TP_PCH_GPP_F_1
    { GPIO_SKL_H_GPP_F2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_2_TP_PCH_GPP_F_2
    { GPIO_SKL_H_GPP_F3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_3_TP_PCH_GPP_F_3
    { GPIO_SKL_H_GPP_F4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_4_TP_PCH_GPP_F_4
    { GPIO_SKL_H_GPP_F5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_F_5_IRQ_TPM_SPI_N
    { GPIO_SKL_H_GPP_F6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_F_6_JTAG_PCH_PLD_TCK
    { GPIO_SKL_H_GPP_F7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_F_7_JTAG_PCH_PLD_TDI
    { GPIO_SKL_H_GPP_F8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_F_8_JTAG_PCH_PLD_TMS
    { GPIO_SKL_H_GPP_F9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_F_9_JTAG_PCH_PLD_TDO
    { GPIO_SKL_H_GPP_F10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_10_SGPIO_SATA_CLOCK
    { GPIO_SKL_H_GPP_F11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_11_SGPIO_SATA_LOAD, MSFT_WCS_override for AVA Slot5_PRSNT_N_2_1
    { GPIO_SKL_H_GPP_F12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_12_SGPIO_SATA_DATA1, MSFT_WCS_override for AVA Slot4_PRSNT_N_2_1
    { GPIO_SKL_H_GPP_F13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_13_SGPIO_SATA_DATA0, MSFT_WCS_override for AVA Slot3_PRSNT_N_2_1
    { GPIO_SKL_H_GPP_F14, { GpioPadModeNative3, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_14_LED_PCH_SSATA_HDD_N
    { GPIO_SKL_H_GPP_F15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_15_FM_OC4_USB_N
    { GPIO_SKL_H_GPP_F16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_16_FM_OC5_USB_N
    { GPIO_SKL_H_GPP_F17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_17_FM_OC6_USB_N
    { GPIO_SKL_H_GPP_F18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_18_FM_OC7_USB_N
    { GPIO_SKL_H_GPP_F19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_19_SMB_GBE_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_F20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_20_SMB_GBE_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_F21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_21_TP_PCH_GPP_F_21
    { GPIO_SKL_H_GPP_F22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_22_SGPIO_SSATA_CLOCK
    { GPIO_SKL_H_GPP_F23, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_F_23_SGPIO_SSATA_LOAD
    // Group G
    { GPIO_SKL_H_GPP_G0, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_0_FAN_TACH_0_FAN_TACH0IE, MSFT_WCS_override for AVA Slot5_PRSNT_N_2_3
    { GPIO_SKL_H_GPP_G1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_1_FAN_TACH_1_FAN_TACH1IE, MSFT_WCS_override for AVA Slot5_PRSNT_N_2_4
    { GPIO_SKL_H_GPP_G2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_2_FAN_TACH_2_FAN_TACH2IE, MSFT_WCS_override for AVA Slot5_PRSNT_N_2_6
    { GPIO_SKL_H_GPP_G3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_3_FAN_TACH_3_FAN_TACH3IE
    { GPIO_SKL_H_GPP_G4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_4_FAN_TACH_4_FAN_TACH4IE
    { GPIO_SKL_H_GPP_G5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_5_FAN_TACH_5_FAN_TACH5IE
    { GPIO_SKL_H_GPP_G6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_6_FAN_TACH_6_FAN_TACH6IE
    { GPIO_SKL_H_GPP_G7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_7_FAN_TACH_7_FAN_TACH7IE
    { GPIO_SKL_H_GPP_G8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_G_8_FAN_PWM_0_FAN_PWM0IE MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_G9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_9_FAN_PWM_1_FAN_PWM1IE
    { GPIO_SKL_H_GPP_G10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_G_10_FAN_PWM_2_FAN_PWM2IE MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_G11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_G_11_FAN_PWM_3_FAN_PWM3IE MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_G12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_G_12_FM_BOARD_SKU_ID0
    { GPIO_SKL_H_GPP_G13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_G_13_FM_BOARD_SKU_ID1
    { GPIO_SKL_H_GPP_G14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_G_14_FM_BOARD_SKU_ID2
    { GPIO_SKL_H_GPP_G15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_G_15_FM_BOARD_SKU_ID3
    { GPIO_SKL_H_GPP_G16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_G_16_FM_BOARD_SKU_ID4
    { GPIO_SKL_H_GPP_G17, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_17_ADR_COMPLETE
    { GPIO_SKL_H_GPP_G18, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_18_FM_NMI_EVENT_N
    { GPIO_SKL_H_GPP_G19, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_19_FM_SMI_ACTIVE_N
    { GPIO_SKL_H_GPP_G20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_20_IRQ_SML1_PMBUS_ALERT_N
    { GPIO_SKL_H_GPP_G21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_21_FM_SATAEXPRESS_DEVSLP
    { GPIO_SKL_H_GPP_G22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_22_FM_BIOS_IMAGE_SWAP_N
    { GPIO_SKL_H_GPP_G23, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_G_23_FM_SSATA_PCIE_SEL
    // Group H
    { GPIO_SKL_H_GPP_H0, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_0_PU_PCH_GPP_H_0
    { GPIO_SKL_H_GPP_H1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_1_FM_SWAP_OVERRIDE_N
    { GPIO_SKL_H_GPP_H2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_2_FM_PCH_MGPIO_TEST0
    { GPIO_SKL_H_GPP_H3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_3_FM_PCH_MGPIO_TEST1
    { GPIO_SKL_H_GPP_H4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_4_FM_PCH_MGPIO_TEST4
    { GPIO_SKL_H_GPP_H5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_5_FM_CLKREQ_M2_SSD_N
    { GPIO_SKL_H_GPP_H6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_6_FM_OCULINK1_PCIE_SSD0_PRSNT_N
    { GPIO_SKL_H_GPP_H7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_7_FM_OCULINK1_PCIE_SSD1_PRSNT_N
    { GPIO_SKL_H_GPP_H8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_8_FM_CLKREQ_NIC1_N
    { GPIO_SKL_H_GPP_H9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_9_FM_PCH_MGPIO_TEST5
    { GPIO_SKL_H_GPP_H10, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_10_SMB_SMLINK2_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_H11, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_11_SMB_SMLINK2_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_H12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_12_FM_ESPI_FLASH_MODE
    { GPIO_SKL_H_GPP_H13, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_13_SMB_SMLINK3_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_H14, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_14_SMB_SMLINK3_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_H15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_H_15_PU_ADR_TIMER_HOLD_OFF_N
    { GPIO_SKL_H_GPP_H16, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_16_SMB_SMLINK4_STBY_LVC3_SCL
    { GPIO_SKL_H_GPP_H17, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_H_17_SMB_SMLINK4_STBY_LVC3_SDA
    { GPIO_SKL_H_GPP_H18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_H_18_FM_LT_KEY_DOWNGRADE_N
    { GPIO_SKL_H_GPP_H19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_H_19_TP_PCH_GPP_H_19
    { GPIO_SKL_H_GPP_H20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_H_20_FM_PCH_MGPIO_TEST2
    { GPIO_SKL_H_GPP_H21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_H_21_FM_PCH_MGPIO_TEST3 MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_H22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_H_22_TP_PCH_GPP_H_22
    { GPIO_SKL_H_GPP_H23, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetNormal, GpioTermNone } },//GPP_H_23_FM_SSATA_PCIE_M2_SEL , MSFT_WCS_override for AVA Slot4_PRSNT_N_2_3
    // Group I
    { GPIO_SKL_H_GPP_I0, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_0_GBE_TDO
    { GPIO_SKL_H_GPP_I1, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_1_GBE_TCK
    { GPIO_SKL_H_GPP_I2, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_2_GBE_TMS
    { GPIO_SKL_H_GPP_I3, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_3_GBE_TDI
    { GPIO_SKL_H_GPP_I4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_4_FP_LED_STATUS_GREEN_PCH_N
    { GPIO_SKL_H_GPP_I5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_5_FP_LED_STATUS_AMBER_PCH_N
    { GPIO_SKL_H_GPP_I6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_6_FP_ID_LED_PCH_N
    { GPIO_SKL_H_GPP_I7, { GpioPadModeNative2, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_7_JTAG_GBE_TRST_N
    { GPIO_SKL_H_GPP_I8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_8_FP_ID_BTN_PCH_N
    { GPIO_SKL_H_GPP_I9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_9_FM_MEM_THERM_EVENT_PCH_N
    { GPIO_SKL_H_GPP_I10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_I_10_TP_PCH_GPP_I_10
    // Group GPD
    { GPIO_SKL_H_GPD0, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_0_FM_FIVRBREAK_N
    { GPIO_SKL_H_GPD1, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_1_ACPRESENT
    { GPIO_SKL_H_GPD2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_2_LAN_WAKEB
    { GPIO_SKL_H_GPD3, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_3_PWRBTNB
    { GPIO_SKL_H_GPD4, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_4_SLP_S3B
    { GPIO_SKL_H_GPD5, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_5_SLP_S4B
    { GPIO_SKL_H_GPD6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_6_SLP_AB
    { GPIO_SKL_H_GPD7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_7_TP_GPD_7
    { GPIO_SKL_H_GPD8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_8_CLK_33K_PCH_SUSCLK_PLD
    { GPIO_SKL_H_GPD9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_9_TP_SLP_WLAN
    { GPIO_SKL_H_GPD10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_10_SLP_S5B_N
    { GPIO_SKL_H_GPD11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPD_11_FM_PHY_DISABLE_N
    // Group J
    { GPIO_SKL_H_GPP_J0, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_0_LED_GBE_0_ACTIVITY
    { GPIO_SKL_H_GPP_J1, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_1_LED_GBE_0_SPEED
    { GPIO_SKL_H_GPP_J2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_2_LED_GBE_1_ACTIVITY
    { GPIO_SKL_H_GPP_J3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_3_LED_GBE_1_SPEED
    { GPIO_SKL_H_GPP_J4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_4_LED_GBE_2_ACTIVITY, MSFT_WCS_override for AVA Slot3_PRSNT_N_2_3
    { GPIO_SKL_H_GPP_J5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_5_LED_GBE_2_SPEED, MSFT_WCS_override for AVA Slot4_PRSNT_N_2_6
    { GPIO_SKL_H_GPP_J6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_J_6_LED_GBE_3_ACTIVITY MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_J7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_J_7_LED_GBE_3_SPEED    MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_J8, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_8_SMB_GBE0_LVC3_R_SCL
    { GPIO_SKL_H_GPP_J9, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_9_SMB_GBE0_LVC3_R_SDA
    { GPIO_SKL_H_GPP_J10, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_10_SMB_GBE1_LVC3_R_SCL
    { GPIO_SKL_H_GPP_J11, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirInOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_11_SMB_GBE1_LVC3_R_SDA
    { GPIO_SKL_H_GPP_J12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_12_SMB_GBE2_LVC3_R_SCL
    { GPIO_SKL_H_GPP_J13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_13_SMB_GBE2_LVC3_R_SDA
    { GPIO_SKL_H_GPP_J14, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_14_SMB_GBE3_LVC3_R_SCL
    { GPIO_SKL_H_GPP_J15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_15_SMB_GBE3_LVC3_R_SDA, MSFT_WCS_override for AVA Slot4_PRSNT_N_2_4
    { GPIO_SKL_H_GPP_J16, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_16_FM_T1_LVC3_MOD_ABS0
    { GPIO_SKL_H_GPP_J17, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_17_LED_GBE_0_LOW_SPEED
    { GPIO_SKL_H_GPP_J18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_J_18_FM_L1_LVC3_MOD_ABS0  MSFT_WCS_override: Reset Type
    { GPIO_SKL_H_GPP_J19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_19_LED_GBE_1_LOW_SPEED
    { GPIO_SKL_H_GPP_J20, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_20_FM_T2_LVC3_MOD_ABS0
    { GPIO_SKL_H_GPP_J21, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_21_LED_GBE_2_LOW_SPEED
    { GPIO_SKL_H_GPP_J22, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutLow, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_22_FM_L2_LVC3_MOD_ABS0
    { GPIO_SKL_H_GPP_J23, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirOut, GpioOutHigh, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_J_23_LED_GBE_3_LOW_SPEED
    // Group K
    { GPIO_SKL_H_GPP_K0, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_0_CLK_50M_CKMNG_PCH
    { GPIO_SKL_H_GPP_K1, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_1_RMII_PCH_BMC_RXD<0>
    { GPIO_SKL_H_GPP_K2, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_2_RMII_PCH_BMC_RXD<1>
    { GPIO_SKL_H_GPP_K3, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_3_GBE_NCSI_CRS_DV_R
    { GPIO_SKL_H_GPP_K4, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_4_RMII_BMC_PCH_TX_EN
    { GPIO_SKL_H_GPP_K5, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_5_RMII_BMC_PCH_TXD<0>
    { GPIO_SKL_H_GPP_K6, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_6_RMII_BMC_PCH_TXD<1>
    { GPIO_SKL_H_GPP_K7, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_7_RMII_PCH_BMC_RX_ER
    { GPIO_SKL_H_GPP_K8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_8_PD_RMII_PCH_ARB_IN
    { GPIO_SKL_H_GPP_K9, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirOut, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_9_PU_RMII_PCH_ARB_OUT
    { GPIO_SKL_H_GPP_K10, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetDeep, GpioTermNone } },//GPP_K_10_RST_PCIE_PCH_PERST_N
    //{GPIO_SKL_H_GPP_K11, { GpioPadModeNative1, GpioHostOwnGpio, GpioDirIn,    GpioOutDefault, GpioIntDis,GpioResetDeep,  GpioTermNone}},//GPP_K_11_PD_1P8_3P3_RCOMP
    // Group L
    //{GPIO_SKL_H_GPP_L0,  { GpioPadModeGpio,    GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis,GpioResetDeep,  GpioTermNone}},//GPP_L_0
    //{GPIO_SKL_H_GPP_L1,  { GpioPadModeGpio,    GpioHostOwnGpio, GpioDirOut,   GpioOutHigh,    GpioIntDis,GpioResetDeep,  GpioTermNone}},//GPP_L_1
    { GPIO_SKL_H_GPP_L2, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_2_VISA2CH0_D0
    { GPIO_SKL_H_GPP_L3, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_3_VISA2CH0_D1
    { GPIO_SKL_H_GPP_L4, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_4_VISA2CH0_D2
    { GPIO_SKL_H_GPP_L5, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_5_VISA2CH0_D3
    { GPIO_SKL_H_GPP_L6, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_6_VISA2CH0_D4
    { GPIO_SKL_H_GPP_L7, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_7_VISA2CH0_D5
    { GPIO_SKL_H_GPP_L8, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_8_VISA2CH0_D6
    { GPIO_SKL_H_GPP_L9, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_9_VISA2CH0_D7
    { GPIO_SKL_H_GPP_L10, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutLow, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_10_VISA2CH0_CLK
    { GPIO_SKL_H_GPP_L11, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutLow, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_11_VISA2CH1_D0
    { GPIO_SKL_H_GPP_L12, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_12_VISA2CH1_D1
    { GPIO_SKL_H_GPP_L13, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_13_VISA2CH1_D2
    { GPIO_SKL_H_GPP_L14, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_14_VISA2CH1_D3
    { GPIO_SKL_H_GPP_L15, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_15_VISA2CH1_D4
    { GPIO_SKL_H_GPP_L16, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_16_VISA2CH1_D5
    { GPIO_SKL_H_GPP_L17, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_17_VISA2CH1_D6
    { GPIO_SKL_H_GPP_L18, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_18_VISA2CH1_D7
    { GPIO_SKL_H_GPP_L19, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioResetPwrGood, GpioTermNone } },//GPP_L_19_VISA2CH1_CLK
};

UINTN mGpioTableSizeMicrosoftWcs = sizeof(mGpioTableMicrosoftWcs);
