/** @file
 *
 *  Differentiated System Definition Table (DSDT)
 *
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2018-2020, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Copyright (c) 2021, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

#define BOARD_I2S0_TPLG "i2s-jack"

#define BOARD_AUDIO_CODEC_HID "ESSX8388"
#define BOARD_CODEC_I2C "\\_SB.I2C7"
#define BOARD_CODEC_I2C_ADDR 0x11
#define BOARD_CODEC_GPIO "\\_SB.GPI1"
#define BOARD_CODEC_GPIO_PIN GPIO_PIN_PC0

//
// WORKAROUND:
// SDMMC may be unreliable at UHS-I speeds.
//
#define SDMMC_CAP_DDR50 0
#define SDMMC_CAP_SDR50 0
#define SDMMC_CAP_SDR104 0

DefinitionBlock ("Dsdt.aml", "DSDT", 2, "RKCP  ", "RK3588  ", 2)
{
  Scope (\_SB_)
  {
    include ("DsdtCommon.asl")

    include ("Cpu.asl")

    include ("Pcie.asl")
    include ("Sata.asl")
    include ("Emmc.asl")
    include ("Sdhc.asl")
    include ("Dma.asl")

    include ("Gpio.asl")
    include ("I2c.asl")
    include ("Uart.asl")

    include ("I2s.asl")

    include ("Usb2Host.asl")
    include ("Usb3Host0.asl")
    include ("Usb3Host2.asl")

    Scope (I2C7) {
      include ("Es8388.asl")
    }
  }
}
