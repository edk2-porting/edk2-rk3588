/**
*
*  Copyright (C) 2021, Semihalf.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef BOARD_DESCRIPTION_LIB_H__
#define BOARD_DESCRIPTION_LIB_H__

#define IO_WIN_ALR_OFFSET(WinId)        (0xF06F0000 + 0x0 + (0x10 * (WinId)))
#define IO_WIN_AHR_OFFSET(WinId)        (0xF06F0000 + 0x8 + (0x10 * (WinId)))
#define IO_WIN_CR_OFFSET(WinId)         (0xF06F0000 + 0xC + (0x10 * (WinId)))
#define IO_WIN_ENABLE_BIT               0x1
#define IO_WIN_ADDRESS_SHIFT            16
#define IO_WIN_ADDRESS_MASK             0xFFFFFFF0

#define MCI0_TARGET_ID                  0x0
#define MCI1_TARGET_ID                  0x1
#define CP1_PCIE_WIN64_BASE             0x890000000
#define CP1_PCIE_WIN64_SIZE             0x30000000
#define CP1_PCIE_WIN64_ID               0x5
#define CP2_PCIE_WIN64_BASE             0x8c0000000
#define CP2_PCIE_WIN64_SIZE             0x30000000
#define CP2_PCIE_WIN64_ID               0x6

#define CP0_GPIO1_DATA_OUT_REG          0xF2440140
#define CP0_GPIO1_OUT_EN_REG            0xF2440144
#define CP0_GPIO1_PIN_MASK              (1 << 7)

#endif
