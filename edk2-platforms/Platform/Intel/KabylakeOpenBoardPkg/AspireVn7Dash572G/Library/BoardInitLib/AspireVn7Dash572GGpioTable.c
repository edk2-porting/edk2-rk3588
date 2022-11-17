/** @file
  GPIO definition table for Acer Aspire VN7-572G

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ASPIRE_VN7_572G_GPIO_TABLE_H_
#define _ASPIRE_VN7_572G_GPIO_TABLE_H_

#include <PiPei.h>
#include <GpioConfig.h>
#include <GpioPinsSklLp.h>
#include <Library/GpioLib.h>

#define END_OF_GPIO_TABLE 0xFFFFFFFF

/* TODO: Vendor configures many NC pads as _TERM_GPO. Why? */
/* TODO: Clean-up
 * - On direction: Are some of these comments illusory? At least some pads
 *   are bidirectional on the other side of the GPIO.
 * - Then, finalise whitespace */
/* NB: Do not reconfigure pads used by Optimus, their assertion state may be lost */

GPIO_INIT_CONFIG mGpioTableAspireVn7Dash572G[] =
{

  /* ------- GPIO Community 0 ------- */

  /* ------- GPIO Group GPP_A ------- */
  // RCIN# <= H_RCIN#
  { GPIO_SKL_LP_GPP_A0, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // LAD0 (ESPI_IO0) <=> LPC_AD_CPU_P0
  { GPIO_SKL_LP_GPP_A1, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNative } },
  // LAD1 (ESPI_IO1) <=> LPC_AD_CPU_P1
  { GPIO_SKL_LP_GPP_A2, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNative } },
  // LAD2 (ESPI_IO2) <=> LPC_AD_CPU_P2
  { GPIO_SKL_LP_GPP_A3, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNative } },
  // LAD3 (ESPI_IO3) <=> LPC_AD_CPU_P3
  { GPIO_SKL_LP_GPP_A4, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNative } },
  // LFRAME# (ESPI_CS#) => LPC_FRAME#_CPU
  { GPIO_SKL_LP_GPP_A5, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SERIRQ <=> INT_SERIRQ
  { GPIO_SKL_LP_GPP_A6, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // PIRQA# = PIRQA#
  { GPIO_SKL_LP_GPP_A7, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // CLKRUN# <= PM_CLKRUN#_EC
  { GPIO_SKL_LP_GPP_A8, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // CLKOUT_LPC0 (ESPI_CLK) <= LPC_CLK_CPU_P0
  { GPIO_SKL_LP_GPP_A9, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // CLKOUT_LPC1 <= LPC_CLK_CPU_P1
  { GPIO_SKL_LP_GPP_A10, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (PME#) // NC
  { GPIO_SKL_LP_GPP_A11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SX_EXIT_HOLDOFF#/BM_BUSY#/ISH_GP6) <= GC6_FB_EN
  { GPIO_SKL_LP_GPP_A12, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SUSWARN#/SUSPWRDNACK = PM_SUSACK#
  { GPIO_SKL_LP_GPP_A13, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SUS_STAT# (ESPI_RESET#) => PM_SUS_STAT#
  { GPIO_SKL_LP_GPP_A14, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SUS_ACK# = PM_SUSACK#
  { GPIO_SKL_LP_GPP_A15, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_1P8_SEL) // NC
  { GPIO_SKL_LP_GPP_A16, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_PWR_EN#/ISH_GP7) // NC
  { GPIO_SKL_LP_GPP_A17, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_GP0) => GSENSOR_INT#
  { GPIO_SKL_LP_GPP_A18, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (ISH_GP1) // NC
  { GPIO_SKL_LP_GPP_A19, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_GP3) // NC
  { GPIO_SKL_LP_GPP_A21, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_GP4) <= GPU_EVENT#
  { GPIO_SKL_LP_GPP_A22, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (ISH_GP5) // NC
  { GPIO_SKL_LP_GPP_A23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },

  /* ------- GPIO Group GPP_B ------- */
  // CORE_VID0 // V0.85A_VID0
  { GPIO_SKL_LP_GPP_B0, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // CORE_VID1 // V0.85A_VID1
  { GPIO_SKL_LP_GPP_B1, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (CPU_GP2) <= TP_IN#
  // TODO: APIC-routed pads don't have host owners?
  { GPIO_SKL_LP_GPP_B3, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntApic | GpioIntLevel, GpioHostDeepReset, GpioTermNone } },
  // SRCCLKREQ0# <= PEG_CLKREQ_CPU#
  { GPIO_SKL_LP_GPP_B5, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SRCCLKREQ1# <= LAN_CLKREQ_CPU#
  { GPIO_SKL_LP_GPP_B6, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SRCCLKREQ2# <= WLAN_CLKREQ_CPU#
  { GPIO_SKL_LP_GPP_B7, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SRCCLKREQ3# <= MSATA_CLKREQ_CPU#
  { GPIO_SKL_LP_GPP_B8, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SRCCLKREQ4# // SRCCLKREQ4# ("Remove TBT")
  { GPIO_SKL_LP_GPP_B9, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SRCCLKREQ5# // SRCCLKREQ5#
  { GPIO_SKL_LP_GPP_B10, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (EXT_PWR_GATE#) = EXT_PWR_GATE#
  { GPIO_SKL_LP_GPP_B11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SLP_S0#) // NC
  { GPIO_SKL_LP_GPP_B12, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // PLTRST# => PLT_RST#
  { GPIO_SKL_LP_GPP_B13, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (SPKR) => HDA_SPKR (Strap - Top Swap Override)
  { GPIO_SKL_LP_GPP_B14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (GSPI0_CS#) = TOUCH_DET#
  { GPIO_SKL_LP_GPP_B15, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (GSPI0_CLK) // NC
  { GPIO_SKL_LP_GPP_B16, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (GSPI0_MISO) // NC ("Remove TBT")
  { GPIO_SKL_LP_GPP_B17, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutLow, GpioIntSci | GpioIntEdge, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (GSPI0_MOSI) => GPP_B18/GSPI0_MOSI (Strap - No reboot)
  { GPIO_SKL_LP_GPP_B18, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (GSPI1_CS#) => RTC_DET#
  { GPIO_SKL_LP_GPP_B19, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (GSPI1_CLK) <= PSW_CLR#
  { GPIO_SKL_LP_GPP_B20, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (GSPI1_MOSI) => GPP_B22/GSPI1_MOSI (Strap - Boot BIOS strap)
  { GPIO_SKL_LP_GPP_B22, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SML1ALERT#/PCHHOT#) => GPP_B23 (Strap)
  { GPIO_SKL_LP_GPP_B23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },

  /* ------- GPIO Community 1 ------- */

  /* ------- GPIO Group GPP_C ------- */
  // SMBCLK <= SMB_CLK
  { GPIO_SKL_LP_GPP_C0, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SMBDATA = SMB_DATA
  { GPIO_SKL_LP_GPP_C1, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SMBALERT#) => GPP_C2 (Strap - TLS Confidentiality)
  { GPIO_SKL_LP_GPP_C2, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SML0CLK) // NC
  { GPIO_SKL_LP_GPP_C3, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SML0DATA) // NC
  { GPIO_SKL_LP_GPP_C4, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SML0ALERT#) // NC (Strap - eSPI or LPC)
  { GPIO_SKL_LP_GPP_C5, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // RESERVED (SML1CLK) <=> SML1_CLK (KBC)
  // RESERVED (SML1DATA) <=> SML1_DATA (KBC)
  // GPIO (UART0_RXD) // NC
  { GPIO_SKL_LP_GPP_C8, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART0_TXD) // NC
  { GPIO_SKL_LP_GPP_C9, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART0_RTS#) // NC
  { GPIO_SKL_LP_GPP_C10, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART0_CTS#) // NC
  { GPIO_SKL_LP_GPP_C11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART1_RXD/ISH_UART1_RXD) // NC
  { GPIO_SKL_LP_GPP_C12, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART1_TXD/ISH_UART1_TXD) // NC
  { GPIO_SKL_LP_GPP_C13, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART1_RTS#/ISH_UART1_RTS#) // NC
  { GPIO_SKL_LP_GPP_C14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (UART1_CTS#/ISH_UART1_CTS#) // NC
  { GPIO_SKL_LP_GPP_C15, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // I2C0_SDA <=> I2C0_DATA_CPU (Touch Panel)
  { GPIO_SKL_LP_GPP_C16, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // I2C0_SCL <=> I2C0_CLK_CPU (Touch Panel)
  { GPIO_SKL_LP_GPP_C17, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // I2C1_SDA <=> I2C1_DATA_CPU (Touch Pad)
  { GPIO_SKL_LP_GPP_C18, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // I2C1_SCL <=> I2C1_CLK_CPU (Touch Pad)
  { GPIO_SKL_LP_GPP_C19, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // UART2_RXD = LPSS_UART2_RXD
  { GPIO_SKL_LP_GPP_C20, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // UART2_TXD = LPSS_UART2_TXD
  { GPIO_SKL_LP_GPP_C21, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // UART2_RTS# = LPSS_UART2_RTS#
  { GPIO_SKL_LP_GPP_C22, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // UART2_CTS# = LPSS_UART2_CTS#
  { GPIO_SKL_LP_GPP_C23, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },

  /* ------- GPIO Group GPP_D ------- */
  // GPIO (SPI1_CS#) // NC
  { GPIO_SKL_LP_GPP_D0, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SPI1_CLK) // NC
  { GPIO_SKL_LP_GPP_D1, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // SPI1_MISO // NC
  { GPIO_SKL_LP_GPP_D2, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SPI1_MOSI // NC
  { GPIO_SKL_LP_GPP_D3, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (FLASHTRIG) // NC
  { GPIO_SKL_LP_GPP_D4, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_I2C0_SDA) // NC
  { GPIO_SKL_LP_GPP_D5, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_I2C0_SCL) // NC
  { GPIO_SKL_LP_GPP_D6, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_I2C1_SDA) // NC
  { GPIO_SKL_LP_GPP_D7, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_I2C1_SCL) // NC
  { GPIO_SKL_LP_GPP_D8, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO // NC
  { GPIO_SKL_LP_GPP_D9, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO => TOUCH_S_RST#
  { GPIO_SKL_LP_GPP_D10, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO // NC
  { GPIO_SKL_LP_GPP_D11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO // NC ("Remove TBT")
  { GPIO_SKL_LP_GPP_D12, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (ISH_UART0_RXD/SML0BDATA/I2C4B_SDA) // NC
  { GPIO_SKL_LP_GPP_D13, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_UART0_TXD/SML0BCLK/I2C4B_SCL) // NC
  { GPIO_SKL_LP_GPP_D14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_UART0_RTS#) // NC
  { GPIO_SKL_LP_GPP_D15, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (ISH_UART0_CTS#/SML0BALERT#) // NC
  { GPIO_SKL_LP_GPP_D16, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (DMIC_CLK1) // NC
  { GPIO_SKL_LP_GPP_D17, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (DMIC_DATA1) // NC
  { GPIO_SKL_LP_GPP_D18, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // DMIC_CLK0 => DMIC_CLK_CON_R
  { GPIO_SKL_LP_GPP_D19, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // DMIC_DATA0 => DMIC_PCH_DATA
  { GPIO_SKL_LP_GPP_D20, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SPI1_IO2 // NC
  { GPIO_SKL_LP_GPP_D21, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SPI1_IO3 // NC
  { GPIO_SKL_LP_GPP_D22, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (I2S_MCLK) // NC
  { GPIO_SKL_LP_GPP_D23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },

  /* ------- GPIO Group GPP_E ------- */
  // SATAXPCIE0 (SATAGP0) = SATAGP0
  { GPIO_SKL_LP_GPP_E0, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SATAXPCIE1 (SATAGP1) // NC
  { GPIO_SKL_LP_GPP_E1, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // SATAXPCIE2 (SATAGP2) = SATAGP2
  { GPIO_SKL_LP_GPP_E2, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (CPU_GP0) // NC
  { GPIO_SKL_LP_GPP_E3, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (DEVSLP0) // NC ("Remove DEVSLP_PCH")
  { GPIO_SKL_LP_GPP_E4, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (DEVSLP1) // NC
  { GPIO_SKL_LP_GPP_E5, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (DEVSLP2) // NC
  { GPIO_SKL_LP_GPP_E6, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (CPU_GP1) <= TOUCH_INT#
  { GPIO_SKL_LP_GPP_E7, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutLow, GpioIntApic | GpioIntLevel, GpioHostDeepReset, GpioTermNone } },
  // SATALED# = SATA_LED#
  { GPIO_SKL_LP_GPP_E8, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // USB2_OC0# = USB_OC#
  { GPIO_SKL_LP_GPP_E9, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // USB2_OC1# // USB_OC#
  { GPIO_SKL_LP_GPP_E10, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // USB2_OC2# // USB_OC#
  { GPIO_SKL_LP_GPP_E11, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // USB2_OC3# // USB_OC#
  { GPIO_SKL_LP_GPP_E12, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // DDPB_HPD0 <= DDI1_HDMI_HPD_CPU
  { GPIO_SKL_LP_GPP_E13, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // DDPC_HPD1 // NC ("Remove HPD")
  { GPIO_SKL_LP_GPP_E14, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (DDPD_HPD2) <= EC_SMI#
  // FIXME: Vendor configures as _TERM_GPO. Why?
  { GPIO_SKL_LP_GPP_E15, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutLow, GpioIntSmi | GpioIntLevel, GpioHostDeepReset, GpioTermNone } },
  // GPIO (DDPE_HPD3) <= EC_SCI#
  { GPIO_SKL_LP_GPP_E16, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirInInv, GpioOutLow, GpioIntSci | GpioIntLevel, GpioPlatformReset, GpioTermNone } },
  // EDP_HPD <= eDP_HPD_CPU
  { GPIO_SKL_LP_GPP_E17, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // DDPB_CTRLCLK <=> DDI1_HDMI_CLK_CPU
  { GPIO_SKL_LP_GPP_E18, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // DDPB_CTRLDATA <=> DDI1_HDMI_DATA_CPU (Strap - Display Port B Detected)
  { GPIO_SKL_LP_GPP_E19, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // DDPC_CTRLCLK // NC
  { GPIO_SKL_LP_GPP_E20, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // DDPC_CTRLDATA => DDPC_CDA (Strap - Display Port C Detected)
  { GPIO_SKL_LP_GPP_E21, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO // NC
  // TODO: Vendor configures as _GPIO_BIDIRECT. Why?
  { GPIO_SKL_LP_GPP_E22, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO => DDPD_CDA (Strap - Display Port D Detected)
  { GPIO_SKL_LP_GPP_E23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },

  /* ------- GPIO Community 2 ------- */

  /* -------- GPIO Group GPD -------- */
  // GPIO (BATLOW#) = BATLOW
  { GPIO_SKL_LP_GPD0, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioDswReset, GpioTermWpd20K } },
  // ACPRESENT <= AC_PRESENT
  { GPIO_SKL_LP_GPD1, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermNone } },
  // GPIO (LAN_WAKE#) = GPD2/LAN_WAKE#
  { GPIO_SKL_LP_GPD2, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioDswReset, GpioTermWpd20K } },
  // PWRBTN# <= PM_PWRBTN#
  { GPIO_SKL_LP_GPD3, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermWpu20K } },
  // SLP_S3# => PM_SLP_S3#
  { GPIO_SKL_LP_GPD4, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermNone } },
  // SLP_S4# => PM_SLP_S4#
  { GPIO_SKL_LP_GPD5, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermNone } },
  // SLP_A# // NC
  { GPIO_SKL_LP_GPD6, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermWpd20K } },
  // GPIO (RSVD#AT15) // NC
  { GPIO_SKL_LP_GPD7, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioDswReset, GpioTermWpd20K } },
  // SUSCLK => SUS_CLK_CPU
  { GPIO_SKL_LP_GPD8, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermNone } },
  // SLP_WLAN# // NC
  { GPIO_SKL_LP_GPD9, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermWpd20K } },
  // SLP_S5# // NC
  { GPIO_SKL_LP_GPD10, { GpioPadModeNative1, GpioHostOwnAcpi, GpioDirInOut, GpioOutLow, GpioIntDis, GpioDswReset, GpioTermWpd20K } },
  // GPIO (LANPHYPC) // NC
  { GPIO_SKL_LP_GPD11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioDswReset, GpioTermWpd20K } },

  /* ------- GPIO Community 3 ------- */

  /* ------- GPIO Group GPP_F ------- */
  // GPIO (I2S2_SCLK) // NC
  { GPIO_SKL_LP_GPP_F0, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2S2_SFRM) // NC
  { GPIO_SKL_LP_GPP_F1, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2S2_TXD) // NC
  { GPIO_SKL_LP_GPP_F2, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2S2_RXD) // NC
  { GPIO_SKL_LP_GPP_F3, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C2_SDA) // NC
  { GPIO_SKL_LP_GPP_F4, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C2_SCL) // NC
  { GPIO_SKL_LP_GPP_F5, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C3_SDA) // NC
  { GPIO_SKL_LP_GPP_F6, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C3_SCL) // NC
  { GPIO_SKL_LP_GPP_F7, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C4_SDA) // NC
  { GPIO_SKL_LP_GPP_F8, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C4_SCL) // NC
  { GPIO_SKL_LP_GPP_F9, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C5_SDA/ISH_I2C2_SDA) // NC
  { GPIO_SKL_LP_GPP_F10, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (I2C5_SCL/ISH_I2C2_SCL) // NC
  { GPIO_SKL_LP_GPP_F11, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_CMD) // NC
  { GPIO_SKL_LP_GPP_F12, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA0) // NC
  { GPIO_SKL_LP_GPP_F13, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA1) // NC
  { GPIO_SKL_LP_GPP_F14, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA2) // NC
  { GPIO_SKL_LP_GPP_F15, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA3) // NC
  { GPIO_SKL_LP_GPP_F16, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA4) // NC
  { GPIO_SKL_LP_GPP_F17, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA5) // NC
  { GPIO_SKL_LP_GPP_F18, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA6) // NC
  { GPIO_SKL_LP_GPP_F19, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_DATA7) // NC
  { GPIO_SKL_LP_GPP_F20, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_RCLK) // NC
  { GPIO_SKL_LP_GPP_F21, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (EMMC_CLK) // NC
  { GPIO_SKL_LP_GPP_F22, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO // NC
  { GPIO_SKL_LP_GPP_F23, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntApic | GpioIntLevel, GpioHostDeepReset, GpioTermNone } },

  /* ------- GPIO Group GPP_G ------- */
  // GPIO (SD_CMD) // NC
  { GPIO_SKL_LP_GPP_G0, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_DATA0) // NC
  { GPIO_SKL_LP_GPP_G1, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_DATA1) // NC
  { GPIO_SKL_LP_GPP_G2, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_DATA2) // NC
  { GPIO_SKL_LP_GPP_G3, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_DATA3) // NC
  // TODO: Vendor configures as _GPO. Why?
  { GPIO_SKL_LP_GPP_G4, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (SD_CD#) // NC
  { GPIO_SKL_LP_GPP_G5, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_CLK) // NC
  { GPIO_SKL_LP_GPP_G6, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  // GPIO (SD_WP) // NC
  { GPIO_SKL_LP_GPP_G7, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirNone, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  //Marking End of Table
  { END_OF_GPIO_TABLE, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutDefault, GpioIntDis, GpioDswReset, GpioTermNone} },
};

UINT16 mGpioTableAspireVn7Dash572GSize = sizeof (mGpioTableAspireVn7Dash572G) / sizeof (GPIO_INIT_CONFIG) - 1;

GPIO_INIT_CONFIG mGpioTableAspireVn7Dash572G_early[] =
{
  // GPIO (ISH_GP2) = DGPU_PRESENT
  { GPIO_SKL_LP_GPP_A20, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (VRALERT#) <= DGPU_PWROK
  { GPIO_SKL_LP_GPP_B2, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirIn, GpioOutLow, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (CPU_GP3) => DGPU_HOLD_RST#
  { GPIO_SKL_LP_GPP_B4, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermNone } },
  // GPIO (GSPI1_MISO) => DGPU_PWR_EN#
  { GPIO_SKL_LP_GPP_B21, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioHostDeepReset, GpioTermWpd20K } },
  //Marking End of Table
  { END_OF_GPIO_TABLE, { GpioPadModeGpio, GpioHostOwnGpio, GpioDirNone, GpioOutDefault, GpioIntDis, GpioDswReset, GpioTermNone} },
};

UINT16 mGpioTableAspireVn7Dash572G_earlySize = sizeof (mGpioTableAspireVn7Dash572G_early) / sizeof (GPIO_INIT_CONFIG) - 1;

#endif // _ASPIRE_VN7_572G_GPIO_TABLE_H_
