/** @file
  This file contains SKL specific GPIO information

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioPrivateLib.h>
#include <Pch/Library/PeiDxeSmmGpioLib/GpioLibrary.h>

//
// ISH GP pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshGPGpio[PCH_ISH_MAX_GP_PINS] =
{
  {GPIO_SKL_LP_GPP_A18, GpioPadModeNative1},// ISH_GP_0
  {GPIO_SKL_LP_GPP_A19, GpioPadModeNative1},// ISH_GP_1
  {GPIO_SKL_LP_GPP_A20, GpioPadModeNative1},// ISH_GP_2
  {GPIO_SKL_LP_GPP_A21, GpioPadModeNative1},// ISH_GP_3
  {GPIO_SKL_LP_GPP_A22, GpioPadModeNative1},// ISH_GP_4
  {GPIO_SKL_LP_GPP_A23, GpioPadModeNative1},// ISH_GP_5
  {GPIO_SKL_LP_GPP_A12, GpioPadModeNative2},// ISH_GP_6
  {GPIO_SKL_LP_GPP_A17, GpioPadModeNative2} // ISH_GP_7
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshGPGpio[PCH_ISH_MAX_GP_PINS] =
{
  {GPIO_SKL_H_GPP_A18, GpioPadModeNative1},// ISH_GP_0
  {GPIO_SKL_H_GPP_A19, GpioPadModeNative1},// ISH_GP_1
  {GPIO_SKL_H_GPP_A20, GpioPadModeNative1},// ISH_GP_2
  {GPIO_SKL_H_GPP_A21, GpioPadModeNative1},// ISH_GP_3
  {GPIO_SKL_H_GPP_A22, GpioPadModeNative1},// ISH_GP_4
  {GPIO_SKL_H_GPP_A23, GpioPadModeNative1},// ISH_GP_5
  {GPIO_SKL_H_GPP_A12, GpioPadModeNative2},// ISH_GP_6
  {GPIO_SKL_H_GPP_A17, GpioPadModeNative1} // ISH_GP_7
};
//
// ISH UART controller pins
// ISH UART[controller number][pin: RXD/TXD/RTSB/CTSB]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshUartGpio[PCH_ISH_MAX_UART_CONTROLLERS][PCH_ISH_PINS_PER_UART_CONTROLLER] =
{
  { //UART0
    {GPIO_SKL_LP_GPP_D13, GpioPadModeNative1},// ISH_UART0_RXD
    {GPIO_SKL_LP_GPP_D14, GpioPadModeNative1},// ISH_UART0_TXD
    {GPIO_SKL_LP_GPP_D15, GpioPadModeNative1},// ISH_UART0_RTS
    {GPIO_SKL_LP_GPP_D16, GpioPadModeNative1} // ISH_UART0_CTS
  },
  { //UART1
    {GPIO_SKL_LP_GPP_C12, GpioPadModeNative2},// ISH_UART1_RXD
    {GPIO_SKL_LP_GPP_C13, GpioPadModeNative2},// ISH_UART1_TXD
    {GPIO_SKL_LP_GPP_C14, GpioPadModeNative2},// ISH_UART1_RTSB
    {GPIO_SKL_LP_GPP_C15, GpioPadModeNative2} // ISH_UART1_CTSB
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshUartGpio[PCH_ISH_MAX_UART_CONTROLLERS][PCH_ISH_PINS_PER_UART_CONTROLLER] =
{
  { //UART0
    {GPIO_SKL_H_GPP_D13, GpioPadModeNative1},// ISH_UART0_RXD
    {GPIO_SKL_H_GPP_D14, GpioPadModeNative1},// ISH_UART0_TXD
    {GPIO_SKL_H_GPP_D15, GpioPadModeNative1},// ISH_UART0_RTS
    {GPIO_SKL_H_GPP_D16, GpioPadModeNative1} // ISH_UART0_CTS
  },
  { //UART1
    {GPIO_SKL_H_GPP_C12, GpioPadModeNative2},// ISH_UART1_RXD
    {GPIO_SKL_H_GPP_C13, GpioPadModeNative2},// ISH_UART1_TXD
    {GPIO_SKL_H_GPP_C14, GpioPadModeNative2},// ISH_UART1_RTS
    {GPIO_SKL_H_GPP_C15, GpioPadModeNative2} // ISH_UART1_CTS
  }
};

//
// ISH I2C controller pins
// ISH I2C[controller number][pin: SDA/SCL]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshI2cGpio[PCH_ISH_MAX_I2C_CONTROLLERS][PCH_ISH_PINS_PER_I2C_CONTROLLER] =
{
  { //I2C0
    {GPIO_SKL_LP_GPP_D5,  GpioPadModeNative1},// ISH_I2C0_SDA
    {GPIO_SKL_LP_GPP_D6,  GpioPadModeNative1} // ISH_I2C0_SCL
  },
  { //I2C1
    {GPIO_SKL_LP_GPP_D7,  GpioPadModeNative1},// ISH_I2C1_SDA
    {GPIO_SKL_LP_GPP_D8,  GpioPadModeNative1} // ISH_I2C1_SCL
  },
  { //I2C2
    {GPIO_SKL_LP_GPP_F10, GpioPadModeNative2},// ISH_I2C2_SDA
    {GPIO_SKL_LP_GPP_F11, GpioPadModeNative2} // ISH_I2C2_SCL
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshI2cGpio[PCH_ISH_MAX_I2C_CONTROLLERS][PCH_ISH_PINS_PER_I2C_CONTROLLER] =
{
  { //I2C0
    {GPIO_SKL_H_GPP_H19, GpioPadModeNative1},// ISH_I2C0_SDA
    {GPIO_SKL_H_GPP_H20, GpioPadModeNative1} // ISH_I2C0_SCL
  },
  { //I2C1
    {GPIO_SKL_H_GPP_H21, GpioPadModeNative1},// ISH_I2C1_SDA
    {GPIO_SKL_H_GPP_H22, GpioPadModeNative1} // ISH_I2C1_SCL
  },
  { //I2C2
    {GPIO_SKL_H_GPP_D4,  GpioPadModeNative1},// ISH_I2C2_SDA
    {GPIO_SKL_H_GPP_D23, GpioPadModeNative1} // ISH_I2C2_SCL
  }
};

//
// ISH SPI controller pins
// ISH SPI[pin: CSB/CLK/MISO/MOSI]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshSpiGpio[PCH_ISH_PINS_PER_SPI_CONTROLLER] =
{
  {GPIO_SKL_LP_GPP_D9,  GpioPadModeNative1},// ISH_SPI_CSB
  {GPIO_SKL_LP_GPP_D10, GpioPadModeNative1},// ISH_SPI_CLK
  {GPIO_SKL_LP_GPP_D11, GpioPadModeNative1},// ISH_SPI_MISO
  {GPIO_SKL_LP_GPP_D12, GpioPadModeNative1} // ISH_SPI_MOSI
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshSpiGpio[PCH_ISH_PINS_PER_SPI_CONTROLLER] =
{
  {GPIO_SKL_H_GPP_D9,  GpioPadModeNative1},// ISH_SPI_CSB
  {GPIO_SKL_H_GPP_D10, GpioPadModeNative1},// ISH_SPI_CLK
  {GPIO_SKL_H_GPP_D11, GpioPadModeNative1},// ISH_SPI_MISO
  {GPIO_SKL_H_GPP_D12, GpioPadModeNative1} // ISH_SPI_MOSI
};

//
// GPIO pin for PCIE SCRCLKREQB
// SCRCLKREQB_x -> GPIO pin y
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpPcieSrcClkReqbPinToGpioMap[PCH_LP_PCIE_MAX_CLK_REQ] =
{
  {GPIO_SKL_LP_GPP_B5,  GpioPadModeNative1},
  {GPIO_SKL_LP_GPP_B6,  GpioPadModeNative1},
  {GPIO_SKL_LP_GPP_B7,  GpioPadModeNative1},
  {GPIO_SKL_LP_GPP_B8,  GpioPadModeNative1},
  {GPIO_SKL_LP_GPP_B9,  GpioPadModeNative1},
  {GPIO_SKL_LP_GPP_B10, GpioPadModeNative1}
};


GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHPcieSrcClkReqbPinToGpioMap[PCH_H_PCIE_MAX_CLK_REQ]  =
{
  {GPIO_SKL_H_GPP_B5,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_B6,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_B7,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_B8,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_B9,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_B10, GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H0,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H1,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H2,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H3,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H4,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H5,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H6,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H7,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H8,  GpioPadModeNative1},
  {GPIO_SKL_H_GPP_H9,  GpioPadModeNative1}
};

//
// PCHHOTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpPchHotbPin = {GPIO_SKL_LP_GPP_B23,  GpioPadModeNative2};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHPchHotbPin = {GPIO_SKL_H_GPP_B23,  GpioPadModeNative2};

//
// CPU GP pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCpuGpPinMap[4] =
{
  {GPIO_SKL_LP_GPP_E3, GpioPadModeNative1}, // CPU_GP_0
  {GPIO_SKL_LP_GPP_E7, GpioPadModeNative1}, // CPU_GP_1
  {GPIO_SKL_LP_GPP_B3, GpioPadModeNative1}, // CPU_GP_2
  {GPIO_SKL_LP_GPP_B4, GpioPadModeNative1}, // CPU_GP_3
};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCpuGpPinMap[4] =
{
  {GPIO_SKL_H_GPP_E3, GpioPadModeNative1}, // CPU_GP_0
  {GPIO_SKL_H_GPP_E7, GpioPadModeNative1}, // CPU_GP_1
  {GPIO_SKL_H_GPP_B3, GpioPadModeNative1}, // CPU_GP_2
  {GPIO_SKL_H_GPP_B4, GpioPadModeNative1}, // CPU_GP_3
};
