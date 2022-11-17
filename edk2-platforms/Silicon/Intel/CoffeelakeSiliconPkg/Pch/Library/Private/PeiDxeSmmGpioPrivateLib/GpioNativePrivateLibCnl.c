/** @file
  This file contains specific GPIO information

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <SaAccess.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <GpioPinsCnlLp.h>
#include <GpioPinsCnlH.h>
#include <Register/PchRegsGpio.h>
#include <Register/PchRegsGpioCnl.h>
#include <Register/PchRegsSerialIo.h>
#include <Register/PchRegsIsh.h>
#include <Register/PchRegsScs.h>
#include <Register/PchRegsLpcCnl.h>

#include "GpioNativePrivateLibInternal.h"

//
// I2C controller pins
// I2C[controller number][pin: SDA/SCL]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpI2cGpio [][PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER]=
{
  {{GPIO_CNL_LP_GPP_C16, GpioPadModeNative1}, {GPIO_CNL_LP_GPP_C17, GpioPadModeNative1}},
  {{GPIO_CNL_LP_GPP_C18, GpioPadModeNative1}, {GPIO_CNL_LP_GPP_C19, GpioPadModeNative1}},
  {{GPIO_CNL_LP_GPP_H4,  GpioPadModeNative1}, {GPIO_CNL_LP_GPP_H5 , GpioPadModeNative1}},
  {{GPIO_CNL_LP_GPP_H6,  GpioPadModeNative1}, {GPIO_CNL_LP_GPP_H7 , GpioPadModeNative1}},
  {{GPIO_CNL_LP_GPP_H8,  GpioPadModeNative1}, {GPIO_CNL_LP_GPP_H9 , GpioPadModeNative1}},
  {{GPIO_CNL_LP_GPP_H10, GpioPadModeNative1}, {GPIO_CNL_LP_GPP_H11, GpioPadModeNative1}}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION  mPchHI2cGpio [][PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER]=
{
  {{GPIO_CNL_H_GPP_C16, GpioPadModeNative1}, {GPIO_CNL_H_GPP_C17, GpioPadModeNative1}}, // I2C0
  {{GPIO_CNL_H_GPP_C18, GpioPadModeNative1}, {GPIO_CNL_H_GPP_C19, GpioPadModeNative1}}, // I2C1
  {{GPIO_CNL_H_GPP_D13, GpioPadModeNative3}, {GPIO_CNL_H_GPP_D14, GpioPadModeNative3}}, // I2C2
  {{GPIO_CNL_H_GPP_D4,  GpioPadModeNative2}, {GPIO_CNL_H_GPP_D23, GpioPadModeNative2}}  // I2C3
};


/**
  This function provides SerialIo I2C controller pins

  @param[in]  SerialIoI2cControllerNumber    I2C controller

  @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetSerialIoI2cPins (
  IN  UINT32                      SerialIoI2cControllerNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    if (SerialIoI2cControllerNumber < ARRAY_SIZE (mPchLpI2cGpio)) {
      *NativePinsTable = mPchLpI2cGpio[SerialIoI2cControllerNumber];
      return;
    }
  } else {
    if (SerialIoI2cControllerNumber < ARRAY_SIZE (mPchHI2cGpio)) {
      *NativePinsTable = mPchHI2cGpio[SerialIoI2cControllerNumber];
      return;
    }
  }
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}
//
// UART controller pins
// UART[controller number][pin: RXD/TXD/RTSB/CTSB]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpUartGpio [][PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER]=
{
  { // UART0
    {GPIO_CNL_LP_GPP_C8,  GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C9,  GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C10, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C11, GpioPadModeNative1}
  },
  { // UART1
    {GPIO_CNL_LP_GPP_C12, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C13, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C14, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C15, GpioPadModeNative1}
  },
  { // UART2
    {GPIO_CNL_LP_GPP_C20, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C21, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C22, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_C23, GpioPadModeNative1},
  },
  { // UART0 (2nd pin set)
    {GPIO_CNL_LP_GPP_F5, GpioPadModeNative2},
    {GPIO_CNL_LP_GPP_F6, GpioPadModeNative2},
    {GPIO_CNL_LP_GPP_F4, GpioPadModeNative2},
    {GPIO_CNL_LP_GPP_F7, GpioPadModeNative2}
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHUartGpio [][PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER]=
{
  { // UART0
    {GPIO_CNL_H_GPP_C8,  GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C9,  GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C10, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C11, GpioPadModeNative1}
  },
  { // UART1
    {GPIO_CNL_H_GPP_C12, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C13, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C14, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C15, GpioPadModeNative1}
  },
  { // UART2
    {GPIO_CNL_H_GPP_C20, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C21, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C22, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_C23, GpioPadModeNative1}
  },
  { // UART0 (2nd pin set)
    {GPIO_CNL_H_GPP_J5, GpioPadModeNative2},
    {GPIO_CNL_H_GPP_J6, GpioPadModeNative2},
    {GPIO_CNL_H_GPP_J4, GpioPadModeNative2},
    {GPIO_CNL_H_GPP_J7, GpioPadModeNative2}
  }
};

/**
  This function provides SerialIo UART controller pins

  @param[in]  SerialIoUartControllerNumber   UART controller
  @param[in]  HardwareFlowControl            Hardware Flow control
  @param[in]  PinMuxing                      UART controller pin muxing
   @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetSerialIoUartPins (
  IN  UINT32                      SerialIoUartControllerNumber,
  IN  BOOLEAN                     HardwareFlowControl,
  IN  UINT32                      PinMuxing,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  UINTN                    UartGpioIndex;

  UartGpioIndex = SerialIoUartControllerNumber;

  if ((SerialIoUartControllerNumber == 0) && (PinMuxing == 1)) {
    // Last record is for UART0 second pin set
    if (IsPchLp ()) {
      UartGpioIndex = ARRAY_SIZE (mPchLpUartGpio) - 1;
    } else {
      UartGpioIndex = ARRAY_SIZE (mPchHUartGpio) - 1;
    }
  }

  if (HardwareFlowControl) {
    *NoOfNativePins = PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER;
  } else {
    *NoOfNativePins = PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER_NO_FLOW_CTRL;
  }

  if (IsPchLp ()) {
    if (UartGpioIndex < ARRAY_SIZE (mPchLpUartGpio)) {
      *NativePinsTable = mPchLpUartGpio[UartGpioIndex];
      return;
    }
  } else {
    if (UartGpioIndex < ARRAY_SIZE (mPchHUartGpio)) {
      *NativePinsTable = mPchHUartGpio[UartGpioIndex];
      return;
    }
  }

  *NativePinsTable = NULL;
  *NoOfNativePins = 0;
  ASSERT (FALSE);
}

//
// SPI controller pins
// SPI[controller number][pin: CSB/CLK/MISO/MOSI]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpSpiGpio [][PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER]=
{
  { // SPI0
    {GPIO_CNL_LP_GPP_B15, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_B16, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_B17, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_B18, GpioPadModeNative1}
  },
  { // SPI1
    {GPIO_CNL_LP_GPP_B19, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_B20, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_B21, GpioPadModeNative1},
    {GPIO_CNL_LP_GPP_B22, GpioPadModeNative1}
  },
  { // SPI2
    {GPIO_CNL_LP_GPP_D9,  GpioPadModeNative3},
    {GPIO_CNL_LP_GPP_D10, GpioPadModeNative3},
    {GPIO_CNL_LP_GPP_D11, GpioPadModeNative3},
    {GPIO_CNL_LP_GPP_D12, GpioPadModeNative3}
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSpiGpio [][PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER]=
{
  { // SPI0
    {GPIO_CNL_H_GPP_B15, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_B16, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_B17, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_B18, GpioPadModeNative1}
  },
  { // SPI1
    {GPIO_CNL_H_GPP_B19, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_B20, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_B21, GpioPadModeNative1},
    {GPIO_CNL_H_GPP_B22, GpioPadModeNative1}
  },
  { // SPI2
    {GPIO_CNL_H_GPP_D9,  GpioPadModeNative3},
    {GPIO_CNL_H_GPP_D10, GpioPadModeNative3},
    {GPIO_CNL_H_GPP_D11, GpioPadModeNative3},
    {GPIO_CNL_H_GPP_D12, GpioPadModeNative3}
  }
};

/**
  This function provides SerialIo SPI controller pins

  @param[in]  SerialIoSpiControllerNumber   SPI controller

  @param[out] NativePinsTable               Table with pins
  @param[out] NoOfNativePins                Number of pins
**/
VOID
GpioGetSerialIoSpiPins (
  IN  UINT32                      SerialIoSpiControllerNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    if (SerialIoSpiControllerNumber < ARRAY_SIZE (mPchLpSpiGpio)) {
      *NativePinsTable = mPchLpSpiGpio[SerialIoSpiControllerNumber];
      *NoOfNativePins =  ARRAY_SIZE (mPchLpSpiGpio[SerialIoSpiControllerNumber]);
      return;
    }
  } else {
    if (SerialIoSpiControllerNumber < ARRAY_SIZE (mPchHSpiGpio)) {
      *NativePinsTable = mPchHSpiGpio[SerialIoSpiControllerNumber];
      *NoOfNativePins =  ARRAY_SIZE (mPchHSpiGpio[SerialIoSpiControllerNumber]);
      return;
    }
  }
  *NativePinsTable = NULL;
  *NoOfNativePins = 0;
  ASSERT (FALSE);
}

//
// ISH GP pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshGPGpio[] =
{
  {GPIO_CNL_LP_GPP_A18, GpioPadModeNative1},// ISH_GP_0
  {GPIO_CNL_LP_GPP_A19, GpioPadModeNative1},// ISH_GP_1
  {GPIO_CNL_LP_GPP_A20, GpioPadModeNative1},// ISH_GP_2
  {GPIO_CNL_LP_GPP_A21, GpioPadModeNative1},// ISH_GP_3
  {GPIO_CNL_LP_GPP_A22, GpioPadModeNative1},// ISH_GP_4
  {GPIO_CNL_LP_GPP_A23, GpioPadModeNative1},// ISH_GP_5
  {GPIO_CNL_LP_GPP_A12, GpioPadModeNative2},// ISH_GP_6
  {GPIO_CNL_LP_GPP_A17, GpioPadModeNative2} // ISH_GP_7
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshGPGpio[] =
{
  {GPIO_CNL_H_GPP_A18, GpioPadModeNative1},// ISH_GP_0
  {GPIO_CNL_H_GPP_A19, GpioPadModeNative1},// ISH_GP_1
  {GPIO_CNL_H_GPP_A20, GpioPadModeNative1},// ISH_GP_2
  {GPIO_CNL_H_GPP_A21, GpioPadModeNative1},// ISH_GP_3
  {GPIO_CNL_H_GPP_A22, GpioPadModeNative1},// ISH_GP_4
  {GPIO_CNL_H_GPP_A23, GpioPadModeNative1},// ISH_GP_5
  {GPIO_CNL_H_GPP_A12, GpioPadModeNative2},// ISH_GP_6
  {GPIO_CNL_H_GPP_A17, GpioPadModeNative2} // ISH_GP_7
};

/**
  This function provides ISH GP pin data

  @param[in]  IshGpPinNumber        ISH GP pin number
  @param[out] NativePin             ISH GP pin
**/
VOID
GpioGetIshGpPin (
  IN  UINT32                      IshGpPinNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    *NativePin
  )
{
  if (IsPchLp ()) {
    if (IshGpPinNumber < ARRAY_SIZE (mPchLpIshGPGpio)) {
      *NativePin = mPchLpIshGPGpio[IshGpPinNumber];
      return;
    }
  } else {
    if (IshGpPinNumber < ARRAY_SIZE (mPchHIshGPGpio)) {
      *NativePin = mPchHIshGPGpio[IshGpPinNumber];
      return;
    }
  }
  *NativePin = (GPIO_PAD_NATIVE_FUNCTION){0};
  ASSERT (FALSE);
}

//
// ISH UART controller pins
// ISH UART[controller number][pin: RXD/TXD/RTSB/CTSB]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshUartGpio[][PCH_ISH_PINS_PER_UART_CONTROLLER] =
{
  { // UART0
    {GPIO_CNL_LP_GPP_D13, GpioPadModeNative1},// ISH_UART0_RXD
    {GPIO_CNL_LP_GPP_D14, GpioPadModeNative1},// ISH_UART0_TXD
    {GPIO_CNL_LP_GPP_D15, GpioPadModeNative1},// ISH_UART0_RTS
    {GPIO_CNL_LP_GPP_D16, GpioPadModeNative1} // ISH_UART0_CTS
  },
  { // UART1
    {GPIO_CNL_LP_GPP_C12, GpioPadModeNative2},// ISH_UART1_RXD
    {GPIO_CNL_LP_GPP_C13, GpioPadModeNative2},// ISH_UART1_TXD
    {GPIO_CNL_LP_GPP_C14, GpioPadModeNative2},// ISH_UART1_RTSB
    {GPIO_CNL_LP_GPP_C15, GpioPadModeNative2} // ISH_UART1_CTSB
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshUartGpio[][PCH_ISH_PINS_PER_UART_CONTROLLER] =
{
  { // UART0
    {GPIO_CNL_H_GPP_D13, GpioPadModeNative1},// ISH_UART0_RXD
    {GPIO_CNL_H_GPP_D14, GpioPadModeNative1},// ISH_UART0_TXD
    {GPIO_CNL_H_GPP_D15, GpioPadModeNative1},// ISH_UART0_RTS
    {GPIO_CNL_H_GPP_D16, GpioPadModeNative1} // ISH_UART0_CTS
  },
  { // UART1
    {GPIO_CNL_H_GPP_C12, GpioPadModeNative2},// ISH_UART1_RXD
    {GPIO_CNL_H_GPP_C13, GpioPadModeNative2},// ISH_UART1_TXD
    {GPIO_CNL_H_GPP_C14, GpioPadModeNative2},// ISH_UART1_RTS
    {GPIO_CNL_H_GPP_C15, GpioPadModeNative2} // ISH_UART1_CTS
  }
};

/**
  This function provides ISH UART controller pins

  @param[in]  IshUartControllerNumber   ISH UART controller

  @param[out] NativePinsTable           Table with pins
**/
VOID
GpioGetIshUartPins (
  IN  UINT32                      IshUartControllerNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    if (IshUartControllerNumber < ARRAY_SIZE (mPchLpIshUartGpio)) {
      *NativePinsTable = mPchLpIshUartGpio[IshUartControllerNumber];
      return;
    }
  } else {
    if (IshUartControllerNumber < ARRAY_SIZE (mPchHIshUartGpio)) {
      *NativePinsTable = mPchHIshUartGpio[IshUartControllerNumber];
      return;
    }
  }
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}

//
// ISH I2C controller pins
// ISH I2C[controller number][pin: SDA/SCL]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshI2cGpio[][PCH_ISH_PINS_PER_I2C_CONTROLLER] =
{
  { // I2C0
    {GPIO_CNL_LP_GPP_D5,  GpioPadModeNative1},// ISH_I2C0_SDA
    {GPIO_CNL_LP_GPP_D6,  GpioPadModeNative1} // ISH_I2C0_SCL
  },
  { // I2C1
    {GPIO_CNL_LP_GPP_D7,  GpioPadModeNative1},// ISH_I2C1_SDA
    {GPIO_CNL_LP_GPP_D8,  GpioPadModeNative1} // ISH_I2C1_SCL
  },
  { // I2C2
    {GPIO_CNL_LP_GPP_H10, GpioPadModeNative2},// ISH_I2C2_SDA
    {GPIO_CNL_LP_GPP_H11, GpioPadModeNative2} // ISH_I2C2_SCL
  },
  { // I2C3
    {GPIO_CNL_LP_GPP_H4,  GpioPadModeNative2},// ISH_I2C3_SDA
    {GPIO_CNL_LP_GPP_H5,  GpioPadModeNative2} // ISH_I2C3_SCL
  },
  { // I2C4
    {GPIO_CNL_LP_GPP_H6,  GpioPadModeNative2},// ISH_I2C4_SDA
    {GPIO_CNL_LP_GPP_H7,  GpioPadModeNative2} // ISH_I2C4_SCL
  },
  { // I2C5
    {GPIO_CNL_LP_GPP_H8,  GpioPadModeNative2},// ISH_I2C5_SDA
    {GPIO_CNL_LP_GPP_H9,  GpioPadModeNative2} // ISH_I2C5_SCL
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshI2cGpio[][PCH_ISH_PINS_PER_I2C_CONTROLLER] =
{
  { // I2C0
    {GPIO_CNL_H_GPP_H19, GpioPadModeNative1},// ISH_I2C0_SDA
    {GPIO_CNL_H_GPP_H20, GpioPadModeNative1} // ISH_I2C0_SCL
  },
  { // I2C1
    {GPIO_CNL_H_GPP_H21, GpioPadModeNative1},// ISH_I2C1_SDA
    {GPIO_CNL_H_GPP_H22, GpioPadModeNative1} // ISH_I2C1_SCL
  },
  { // I2C2
    {GPIO_CNL_H_GPP_D4,  GpioPadModeNative1},// ISH_I2C2_SDA
    {GPIO_CNL_H_GPP_D23, GpioPadModeNative1} // ISH_I2C2_SCL
  }
};

/**
  This function provides ISH I2C controller pins

  @param[in]  IshI2cControllerNumber   ISH I2C controller

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetIshI2cPins (
  IN  UINT32                      IshI2cControllerNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    if (IshI2cControllerNumber < ARRAY_SIZE (mPchLpIshI2cGpio)) {
      *NativePinsTable = mPchLpIshI2cGpio[IshI2cControllerNumber];
      return;
    }
  } else {
    if (IshI2cControllerNumber < ARRAY_SIZE (mPchHIshI2cGpio)) {
      *NativePinsTable = mPchHIshI2cGpio[IshI2cControllerNumber];
      return;
    }
  }
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}

//
// ISH SPI controller pins
// ISH SPI[pin: CSB/CLK/MISO/MOSI]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpIshSpiGpio[PCH_ISH_PINS_PER_SPI_CONTROLLER] =
{
  {GPIO_CNL_LP_GPP_D9,  GpioPadModeNative1},// ISH_SPI_CSB
  {GPIO_CNL_LP_GPP_D10, GpioPadModeNative1},// ISH_SPI_CLK
  {GPIO_CNL_LP_GPP_D11, GpioPadModeNative1},// ISH_SPI_MISO
  {GPIO_CNL_LP_GPP_D12, GpioPadModeNative1} // ISH_SPI_MOSI
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHIshSpiGpio[PCH_ISH_PINS_PER_SPI_CONTROLLER] =
{
  {GPIO_CNL_H_GPP_D9,  GpioPadModeNative1},// ISH_SPI_CSB
  {GPIO_CNL_H_GPP_D10, GpioPadModeNative1},// ISH_SPI_CLK
  {GPIO_CNL_H_GPP_D11, GpioPadModeNative1},// ISH_SPI_MISO
  {GPIO_CNL_H_GPP_D12, GpioPadModeNative1} // ISH_SPI_MOSI
};

/**
  This function provides ISH SPI controller pins

  @param[in]  IshSpiControllerNumber   SPI controller
  @param[out] NativePinsTable          Table with pins
  @param[out] NoOfNativePins           Number of pins
**/
VOID
GpioGetIshSpiPins (
  IN  UINT32                      IshSpiControllerNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    if (IshSpiControllerNumber < ARRAY_SIZE (mPchLpIshSpiGpio)) {
      *NativePinsTable = mPchLpIshSpiGpio;
      *NoOfNativePins = ARRAY_SIZE (mPchLpIshSpiGpio);
      return;
    }
  } else {
    if (IshSpiControllerNumber < ARRAY_SIZE (mPchHIshSpiGpio)) {
      *NativePinsTable = mPchHIshSpiGpio;
      *NoOfNativePins = ARRAY_SIZE (mPchHIshSpiGpio);
      return;
    }
  }

  *NoOfNativePins = 0;
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}

//
// GPIO pins for SD controller
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpScsSdCardGpio[9] =
{
  {GPIO_CNL_LP_GPP_A17, GpioPadModeNative1},// SD_PWR_EN_B
  {GPIO_CNL_LP_GPP_G0,  GpioPadModeNative1},// SD_CMD
  {GPIO_CNL_LP_GPP_G1,  GpioPadModeNative1},// SD_DATA_0
  {GPIO_CNL_LP_GPP_G2,  GpioPadModeNative1},// SD_DATA_1
  {GPIO_CNL_LP_GPP_G3,  GpioPadModeNative1},// SD_DATA_2
  {GPIO_CNL_LP_GPP_G4,  GpioPadModeNative1},// SD_DATA_3
  {GPIO_CNL_LP_GPP_G5,  GpioPadModeNative1},// SD_CDB
  {GPIO_CNL_LP_GPP_G6,  GpioPadModeNative1},// SD_CLK
  {GPIO_CNL_LP_GPP_G7,  GpioPadModeNative1} // SD_WP
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHScsSdCardGpio[9] =
{
  {GPIO_CNL_H_GPP_A17, GpioPadModeNative1},// SD_PWR_EN_B
  {GPIO_CNL_H_GPP_G0,  GpioPadModeNative1},// SD_CMD
  {GPIO_CNL_H_GPP_G1,  GpioPadModeNative1},// SD_DATA_0
  {GPIO_CNL_H_GPP_G2,  GpioPadModeNative1},// SD_DATA_1
  {GPIO_CNL_H_GPP_G3,  GpioPadModeNative1},// SD_DATA_2
  {GPIO_CNL_H_GPP_G4,  GpioPadModeNative1},// SD_DATA_3
  {GPIO_CNL_H_GPP_G5,  GpioPadModeNative1},// SD_CDB
  {GPIO_CNL_H_GPP_G6,  GpioPadModeNative1},// SD_CLK
  {GPIO_CNL_H_GPP_G7,  GpioPadModeNative1} // SD_WP
};

/**
  This function provides SCS SD CARD controller pins

  @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetScsSdCardPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpScsSdCardGpio;
    *NoOfNativePins = ARRAY_SIZE (mPchLpScsSdCardGpio);
  } else {
    *NativePinsTable = mPchHScsSdCardGpio;
    *NoOfNativePins = ARRAY_SIZE (mPchHScsSdCardGpio);
  }
}

/**
  This function provides SCS SD CARD detect pin

  @retval GpioPin             SD CARD Detect pin
**/
GPIO_PAD
GpioGetScsSdCardDetectPin (
  VOID
  )
{
  if (IsPchLp ()) {
    return GPIO_CNL_LP_VGPIO39;
  } else {
    return GPIO_CNL_H_VGPIO6;
  }
}

//
// GPIO pins for eMMC controller
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpScsEmmcGpio[12] =
{
  {GPIO_CNL_LP_GPP_F11, GpioPadModeNative1},// EMMC_CMD
  {GPIO_CNL_LP_GPP_F12, GpioPadModeNative1},// EMMC_DATA_0
  {GPIO_CNL_LP_GPP_F13, GpioPadModeNative1},// EMMC_DATA_1
  {GPIO_CNL_LP_GPP_F14, GpioPadModeNative1},// EMMC_DATA_2
  {GPIO_CNL_LP_GPP_F15, GpioPadModeNative1},// EMMC_DATA_3
  {GPIO_CNL_LP_GPP_F16, GpioPadModeNative1},// EMMC_DATA_4
  {GPIO_CNL_LP_GPP_F17, GpioPadModeNative1},// EMMC_DATA_5
  {GPIO_CNL_LP_GPP_F18, GpioPadModeNative1},// EMMC_DATA_6
  {GPIO_CNL_LP_GPP_F19, GpioPadModeNative1},// EMMC_DATA_7
  {GPIO_CNL_LP_GPP_F20, GpioPadModeNative1},// EMMC_RCLK
  {GPIO_CNL_LP_GPP_F21, GpioPadModeNative1},// EMMC_CLK
  {GPIO_CNL_LP_GPP_F22, GpioPadModeNative1} // EMMC_RESETB
};

/**
  This function provides SCS eMMC controller pins

  @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetScsEmmcPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpScsEmmcGpio;
    *NoOfNativePins = ARRAY_SIZE (mPchLpScsEmmcGpio);
  } else {
    ASSERT (FALSE);
    return;
  }
}

//
// GPIO pins for HD Audio Link [pin: BCLK/RSTB/SYNC/SDO/SDIx]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpHdaLinkGpio[PCH_GPIO_HDA_LINK_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_HDA_BCLK,  GpioPadModeNative1},// HDA_BCLK
  {GPIO_CNL_LP_HDA_RSTB,  GpioPadModeNative1},// HDA_RSTB
  {GPIO_CNL_LP_HDA_SYNC,  GpioPadModeNative1},// HDA_SYNC
  {GPIO_CNL_LP_HDA_SDO,   GpioPadModeNative1},// HDA_SDO
  {GPIO_CNL_LP_HDA_SDI_0, GpioPadModeNative1},// HDA_SDI_0
  {GPIO_CNL_LP_HDA_SDI_1, GpioPadModeNative1} // HDA_SDI_1
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHHdaLinkGpio[PCH_GPIO_HDA_LINK_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_HDA_BCLK,  GpioPadModeNative1},// HDA_BCLK
  {GPIO_CNL_H_HDA_RSTB,  GpioPadModeNative1},// HDA_RSTB
  {GPIO_CNL_H_HDA_SYNC,  GpioPadModeNative1},// HDA_SYNC
  {GPIO_CNL_H_HDA_SDO,   GpioPadModeNative1},// HDA_SDO
  {GPIO_CNL_H_HDA_SDI_0, GpioPadModeNative1},// HDA_SDI_0
  {GPIO_CNL_H_HDA_SDI_1, GpioPadModeNative1} // HDA_SDI_1
};

/**
  This function provides HD Audio Link pins

  @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetHdAudioLinkPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpHdaLinkGpio;
    *NoOfNativePins = ARRAY_SIZE (mPchLpHdaLinkGpio);
  } else {
    *NativePinsTable = mPchHHdaLinkGpio;
    *NoOfNativePins = ARRAY_SIZE (mPchHHdaLinkGpio);
  }
}

//
// GPIO pins for HD Audio DMIC [DMIC number][pin: CLK/DATA]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpHdaDmicGpio[][PCH_GPIO_HDA_DMIC_NUMBER_OF_PINS] =
{
  { // DMIC0
    {GPIO_CNL_LP_GPP_D19, GpioPadModeNative1},// DMIC_CLK_0
    {GPIO_CNL_LP_GPP_D20, GpioPadModeNative1} // DMIC_DATA_0
  },
  { // DMIC1
    {GPIO_CNL_LP_GPP_D17, GpioPadModeNative1},// DMIC_CLK_1
    {GPIO_CNL_LP_GPP_D18, GpioPadModeNative1} // DMIC_DATA_1
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHHdaDmicGpio[][PCH_GPIO_HDA_DMIC_NUMBER_OF_PINS] =
{
  { // DMIC0
    {GPIO_CNL_H_GPP_D19, GpioPadModeNative1},// DMIC_CLK_0
    {GPIO_CNL_H_GPP_D20, GpioPadModeNative1} // DMIC_DATA_0
  },
  { // DMIC1
    {GPIO_CNL_H_GPP_D17, GpioPadModeNative1},// DMIC_CLK_1
    {GPIO_CNL_H_GPP_D18, GpioPadModeNative1} // DMIC_DATA_1
  }
};

/**
  This function provides DMIC interface pins

  @param[in]  DmicNumber               DMIC interface

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetHdaDmicPins (
  IN  UINT32                      DmicNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    if (DmicNumber < ARRAY_SIZE (mPchLpHdaDmicGpio)) {
      *NativePinsTable = mPchLpHdaDmicGpio[DmicNumber];
      return;
    }
  } else {
    if (DmicNumber < ARRAY_SIZE (mPchHHdaDmicGpio)) {
      *NativePinsTable = mPchHHdaDmicGpio[DmicNumber];
      return;
    }
  }
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}

//
// GPIO pins for HD Audio SSPx/I2Sx interface [SSP number][pin: SCLK/SFRM/TXD/RXD]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpHdaSspInterfaceGpio[][PCH_GPIO_HDA_SSP_NUMBER_OF_PINS] =
{
  { // SSP0/I2S0
    {GPIO_CNL_LP_HDA_BCLK,  GpioPadModeNative2},// SSP0_SCLK
    {GPIO_CNL_LP_HDA_SYNC,  GpioPadModeNative2},// SSP0_SFRM
    {GPIO_CNL_LP_HDA_SDO,   GpioPadModeNative2},// SSP0_TXD
    {GPIO_CNL_LP_HDA_SDI_0, GpioPadModeNative2} // SSP0_RXD
  },
  { // SSP1/I2S1
    {GPIO_CNL_LP_HDA_RSTB,  GpioPadModeNative2},// SSP1_SCLK
    {GPIO_CNL_LP_SSP1_SFRM, GpioPadModeNative1},// SSP1_SFRM
    {GPIO_CNL_LP_SSP1_TXD,  GpioPadModeNative1},// SSP1_TXD
    {GPIO_CNL_LP_HDA_SDI_1, GpioPadModeNative2} // SSP1_RXD
  },
  { // SSP2/I2S2
    {GPIO_CNL_LP_GPP_H0, GpioPadModeNative1},// SSP2_SCLK
    {GPIO_CNL_LP_GPP_H1, GpioPadModeNative1},// SSP2_SFRM
    {GPIO_CNL_LP_GPP_H2, GpioPadModeNative1},// SSP2_TXD
    {GPIO_CNL_LP_GPP_H3, GpioPadModeNative1} // SSP2_RXD
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHHdaSspInterfaceGpio[][PCH_GPIO_HDA_SSP_NUMBER_OF_PINS] =
{
  { // SSP0/I2S0
    {GPIO_CNL_H_HDA_BCLK,  GpioPadModeNative2},// SSP0_SCLK
    {GPIO_CNL_H_HDA_SYNC,  GpioPadModeNative2},// SSP0_SFRM
    {GPIO_CNL_H_HDA_SDO,   GpioPadModeNative2},// SSP0_TXD
    {GPIO_CNL_H_HDA_SDI_0, GpioPadModeNative2} // SSP0_RXD
  },
  { // SSP1/I2S1
    {GPIO_CNL_H_HDA_RSTB,  GpioPadModeNative2},// SSP1_SCLK
    {GPIO_CNL_H_SSP1_SFRM, GpioPadModeNative1},// SSP1_SFRM
    {GPIO_CNL_H_SSP1_TXD,  GpioPadModeNative1},// SSP1_TXD
    {GPIO_CNL_H_HDA_SDI_1, GpioPadModeNative2} // SSP1_RXD
  },
  { // SSP2/I2S2
    {GPIO_CNL_H_GPP_D5, GpioPadModeNative1}, // SSP2_SFRM
    {GPIO_CNL_H_GPP_D6, GpioPadModeNative1}, // SSP2_TXD
    {GPIO_CNL_H_GPP_D7, GpioPadModeNative1}, // SSP2_RXD
    {GPIO_CNL_H_GPP_D8, GpioPadModeNative1}  // SSP2_SCLK
  }
};

/**
  This function provides SSP/I2S interface pins

  @param[in]  SspInterfaceNumber       SSP/I2S interface

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetHdaSspPins (
  IN  UINT32                      SspInterfaceNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    if (SspInterfaceNumber < ARRAY_SIZE (mPchLpHdaSspInterfaceGpio)) {
      *NativePinsTable = mPchLpHdaSspInterfaceGpio[SspInterfaceNumber];
      return;
    }
  } else {
    if (SspInterfaceNumber < ARRAY_SIZE (mPchHHdaSspInterfaceGpio)) {
      *NativePinsTable = mPchHHdaSspInterfaceGpio[SspInterfaceNumber];
      return;
    }
  }
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}

//
// GPIO Pin for HD Audio SSP_MCLK/I2S_MCLK
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpHdaSspMasterClockGpio = {GPIO_CNL_LP_GPP_D23, GpioPadModeNative1};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHHdaSspMasterClockGpio = {GPIO_CNL_H_GPP_B11, GpioPadModeNative1};

/**
  This function sets HDA SSP Master Clock into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableHdaSspMasterClock (
  VOID
  )
{
  if (IsPchLp ()) {
    return GpioSetPadMode (mPchLpHdaSspMasterClockGpio.Pad, mPchLpHdaSspMasterClockGpio.Mode);
  } else {
    return GpioSetPadMode (mPchHHdaSspMasterClockGpio.Pad, mPchHHdaSspMasterClockGpio.Mode);
  }
}

//
// GPIO pins for HD Audio SoundWire interface [SNDW number][pin: CLK/DATA]
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpHdaSndwGpio[][PCH_GPIO_HDA_SNDW_NUMBER_OF_PINS] =
{
  { // SNDW1
    {GPIO_CNL_LP_HDA_RSTB,  GpioPadModeNative3},// SNDW1_CLK
    {GPIO_CNL_LP_HDA_SDI_1, GpioPadModeNative3} // SNDW1_DATA
  },
  { // SNDW2
    {GPIO_CNL_LP_SSP1_SFRM, GpioPadModeNative2},// SNDW2_CLK
    {GPIO_CNL_LP_SSP1_TXD,  GpioPadModeNative2} // SNDW2_DATA
  },
  { // SNDW3
    {GPIO_CNL_LP_GPP_D17,   GpioPadModeNative2},// SNDW3_CLK
    {GPIO_CNL_LP_GPP_D18,   GpioPadModeNative2} // SNDW3_DATA
  },
  { // SNDW4
    {GPIO_CNL_LP_GPP_D19,   GpioPadModeNative2},// SNDW4_CLK
    {GPIO_CNL_LP_GPP_D20,   GpioPadModeNative2} // SNDW4_DATA
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHHdaSndwGpio[][PCH_GPIO_HDA_SNDW_NUMBER_OF_PINS] =
{
  { // SNDW1
    {GPIO_CNL_H_HDA_RSTB,  GpioPadModeNative3},// SNDW1_CLK
    {GPIO_CNL_H_HDA_SDI_1, GpioPadModeNative3} // SNDW1_DATA
  },
  { // SNDW2
    {GPIO_CNL_H_SSP1_SFRM, GpioPadModeNative2},// SNDW2_CLK
    {GPIO_CNL_H_SSP1_TXD,  GpioPadModeNative2} // SNDW2_DATA
  },
  { // SNDW3
    {GPIO_CNL_H_GPP_D17,   GpioPadModeNative2},// SNDW3_CLK
    {GPIO_CNL_H_GPP_D18,   GpioPadModeNative2} // SNDW3_DATA
  },
  { // SNDW4
    {GPIO_CNL_H_GPP_D19,   GpioPadModeNative2},// SNDW4_CLK
    {GPIO_CNL_H_GPP_D20,   GpioPadModeNative2} // SNDW4_DATA
  }
};

/**
  This function provides SNDW interface pins

  @param[in]  SndwInterfaceNumber      SNDWx interface number

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetHdaSndwPins (
  IN  UINT32                      SndwInterfaceNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    if (SndwInterfaceNumber < ARRAY_SIZE (mPchLpHdaSndwGpio)) {
      *NativePinsTable = mPchLpHdaSndwGpio[SndwInterfaceNumber];
      return;
    }
  } else {
    if (SndwInterfaceNumber < ARRAY_SIZE (mPchHHdaSndwGpio)) {
      *NativePinsTable = mPchHHdaSndwGpio[SndwInterfaceNumber];
      return;
    }
  }
  *NativePinsTable = NULL;
  ASSERT (FALSE);
}

//
// GPIO pins for SMBUS
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpSmbusGpio[PCH_GPIO_SMBUS_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_C0, GpioPadModeNative1},// SMB_CLK
  {GPIO_CNL_LP_GPP_C1, GpioPadModeNative1} // SMB_DATA
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSmbusGpio[PCH_GPIO_SMBUS_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_C0, GpioPadModeNative1}, // SMB_CLK
  {GPIO_CNL_H_GPP_C1, GpioPadModeNative1}  // SMB_DATA
};

/**
  This function provides SMBUS interface pins

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetSmbusPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpSmbusGpio;
  } else {
    *NativePinsTable = mPchHSmbusGpio;
  }
}

//
// SMBUS Alert pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpSmbusAlertGpio = {GPIO_CNL_LP_GPP_C2,  GpioPadModeNative1};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSmbusAlertGpio = {GPIO_CNL_H_GPP_C2,  GpioPadModeNative1};

/**
  This function sets SMBUS ALERT pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSmbusAlert (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION SmbusAlertGpio;

  if (IsPchLp ()) {
    SmbusAlertGpio = mPchLpSmbusAlertGpio;
  } else {
    SmbusAlertGpio = mPchHSmbusAlertGpio;
  }

  return GpioSetPadMode (SmbusAlertGpio.Pad, SmbusAlertGpio.Mode);
}

//
// SATADevSlpPin to GPIO pin mapping
// SATA_DEVSLP_x -> GPIO pin y
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpSataDevSlpPinToGpioMap[] =
{
  {GPIO_CNL_LP_GPP_E4, GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_E5, GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_E6, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSataDevSlpPinToGpioMap[] =
{
  {GPIO_CNL_H_GPP_E4, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F7, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F8, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F9, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata1DevSlpPinToGpioMap[] =
{
/// @todo SERVER- update for SATA 1
  {GPIO_CNL_H_GPP_E4, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F7, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata2DevSlpPinToGpioMap[] =
{
/// @todo SERVER- update for SATA 2
  {GPIO_CNL_H_GPP_E4, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F7, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata3DevSlpPinToGpioMap[] =
{
/// @todo SERVER- update for SATA 3
  {GPIO_CNL_H_GPP_E4, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_E6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F5, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F6, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_F7, GpioPadModeNative1}
};

/**
  This function provides SATA DevSlp pin data

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number
  @param[out] NativePin           SATA DevSlp pin
**/
VOID
GpioGetSataDevSlpPin (
  IN  UINT32                    SataCtrlIndex,
  IN  UINTN                     SataPort,
  OUT GPIO_PAD_NATIVE_FUNCTION  *NativePin
  )
{
  if (IsCdfPch ()) {
    if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
      if (SataPort < ARRAY_SIZE (mCdfPchSata1DevSlpPinToGpioMap)) {
        *NativePin = mCdfPchSata1DevSlpPinToGpioMap[SataPort];
        return;
      }
    } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
      if (SataPort < ARRAY_SIZE (mCdfPchSata2DevSlpPinToGpioMap)) {
        *NativePin = mCdfPchSata2DevSlpPinToGpioMap[SataPort];
        return;
      }
    } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
      if (SataPort < ARRAY_SIZE (mCdfPchSata3DevSlpPinToGpioMap)) {
        *NativePin = mCdfPchSata3DevSlpPinToGpioMap[SataPort];
        return;
      }
    }
  } else {
    if (IsPchLp ()) {
      if (SataPort < ARRAY_SIZE (mPchLpSataDevSlpPinToGpioMap)) {
        *NativePin = mPchLpSataDevSlpPinToGpioMap[SataPort];
        return;
      }
    } else {
      if (SataPort < ARRAY_SIZE (mPchHSataDevSlpPinToGpioMap)) {
        *NativePin = mPchHSataDevSlpPinToGpioMap[SataPort];
        return;
      }
    }
  }
  *NativePin = (GPIO_PAD_NATIVE_FUNCTION){0};
  ASSERT (FALSE);
}

//
// SATA reset port to GPIO pin mapping
// SATAGP_x -> GPIO pin y
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpSataGpToGpioMap[] =
{
  {GPIO_CNL_LP_GPP_E0, GpioPadModeNative2},
  {GPIO_CNL_LP_GPP_E1, GpioPadModeNative2},
  {GPIO_CNL_LP_GPP_E2, GpioPadModeNative2}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSataGpToGpioMap[]  =
{
  {GPIO_CNL_H_GPP_E0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E2, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F2, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F3, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F4, GpioPadModeNative2}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata1GpToGpioMap[]  =
{
/// @todo SERVER- update for SATA 1
  {GPIO_CNL_H_GPP_E0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E2, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F2, GpioPadModeNative2}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata2GpToGpioMap[]  =
{
/// @todo SERVER- update for SATA 2
  {GPIO_CNL_H_GPP_E0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E2, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F2, GpioPadModeNative2}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata3GpToGpioMap[]  =
{
/// @todo SERVER- update for SATA 3
  {GPIO_CNL_H_GPP_E0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_E2, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F0, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F1, GpioPadModeNative2},
  {GPIO_CNL_H_GPP_F2, GpioPadModeNative2}
};

/**
  This function provides SATA GP pin data

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number
  @param[out] NativePin           SATA GP pin
**/
VOID
GpioGetSataGpPin (
  IN  UINT32                    SataCtrlIndex,
  IN  UINTN                     SataPort,
  OUT GPIO_PAD_NATIVE_FUNCTION  *NativePin
  )
{
  if (IsCdfPch ()) {
    if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
      if (SataPort < ARRAY_SIZE (mCdfPchSata1GpToGpioMap)) {
        *NativePin = mCdfPchSata1GpToGpioMap[SataPort];
        return;
      }
    } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
      if (SataPort < ARRAY_SIZE (mCdfPchSata2GpToGpioMap)) {
        *NativePin = mCdfPchSata2GpToGpioMap[SataPort];
        return;
      }
    } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
      if (SataPort < ARRAY_SIZE (mCdfPchSata3GpToGpioMap)) {
        *NativePin = mCdfPchSata3GpToGpioMap[SataPort];
        return;
      }
    }
  } else {
    if (IsPchLp ()) {
      if (SataPort < ARRAY_SIZE (mPchLpSataGpToGpioMap)) {
        *NativePin = mPchLpSataGpToGpioMap[SataPort];
        return;
      }
    } else {
      if (SataPort < ARRAY_SIZE (mPchHSataGpToGpioMap)) {
        *NativePin = mPchHSataGpToGpioMap[SataPort];
        return;
      }
    }
  }
  *NativePin = (GPIO_PAD_NATIVE_FUNCTION){0};
  ASSERT (FALSE);
}

//
// SATA LED pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpSataLedGpio = {GPIO_CNL_LP_GPP_E8, GpioPadModeNative1};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSataLedGpio = {GPIO_CNL_H_GPP_E8, GpioPadModeNative1};

/**
  This function sets SATA LED pin into native mode. SATA LED indicates
  SATA controller activity

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSataLed (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION  SataLedGpio;

  if (IsPchLp ()) {
    SataLedGpio = mPchLpSataLedGpio;
  } else {
    SataLedGpio = mPchHSataLedGpio;
  }

  return GpioSetPadMode (SataLedGpio.Pad, SataLedGpio.Mode);
}

//
// USB2 OC pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpUsbOcGpioPins[] =
{
  {GPIO_CNL_LP_GPP_E9,  GpioPadModeNative1},// USB_OC_0
  {GPIO_CNL_LP_GPP_E10, GpioPadModeNative1},// USB_OC_1
  {GPIO_CNL_LP_GPP_E11, GpioPadModeNative1},// USB_OC_2
  {GPIO_CNL_LP_GPP_E12, GpioPadModeNative1} // USB_OC_3
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHUsbOcGpioPins[] =
{
  {GPIO_CNL_H_GPP_E9,  GpioPadModeNative1},// USB_OC_0
  {GPIO_CNL_H_GPP_E10, GpioPadModeNative1},// USB_OC_1
  {GPIO_CNL_H_GPP_E11, GpioPadModeNative1},// USB_OC_2
  {GPIO_CNL_H_GPP_E12, GpioPadModeNative1},// USB_OC_3
  {GPIO_CNL_H_GPP_F15, GpioPadModeNative1},// USB_OC_4
  {GPIO_CNL_H_GPP_F16, GpioPadModeNative1},// USB_OC_5
  {GPIO_CNL_H_GPP_F17, GpioPadModeNative1},// USB_OC_6
  {GPIO_CNL_H_GPP_F18, GpioPadModeNative1} // USB_OC_7
};

/**
  This function enables USB OverCurrent pins by setting
  USB2 OCB pins into native mode

  @param[in]  OcPinNumber            USB OC pin number

  @retval Status
**/
EFI_STATUS
GpioEnableUsbOverCurrent (
  IN  UINTN   OcPinNumber
  )
{
  GPIO_PAD_NATIVE_FUNCTION  OcGpio;

  if (IsPchLp ()) {
    if (OcPinNumber >= ARRAY_SIZE (mPchLpUsbOcGpioPins)) {
      ASSERT(FALSE);
      return EFI_UNSUPPORTED;
    }
    OcGpio = mPchLpUsbOcGpioPins[OcPinNumber];
  } else {
    if (OcPinNumber >= ARRAY_SIZE (mPchHUsbOcGpioPins)) {
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    OcGpio = mPchHUsbOcGpioPins[OcPinNumber];
  }

  return GpioSetPadMode (OcGpio.Pad, OcGpio.Mode);
}

//
// GPIO pin for PCIE SCRCLKREQB
// SCRCLKREQB_x -> GPIO pin y
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpPcieSrcClkReqbPinToGpioMap[] =
{
  {GPIO_CNL_LP_GPP_B5,  GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_B6,  GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_B7,  GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_B8,  GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_B9,  GpioPadModeNative1},
  {GPIO_CNL_LP_GPP_B10, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHPcieSrcClkReqbPinToGpioMap[]  =
{
  {GPIO_CNL_H_GPP_B5,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_B6,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_B7,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_B8,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_B9,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_B10, GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H0,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H1,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H2,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H3,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H4,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H5,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H6,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H7,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H8,  GpioPadModeNative1},
  {GPIO_CNL_H_GPP_H9,  GpioPadModeNative1}
};

/**
  This function provides PCIe CLKREQ pin data

  @param[in]  ClkreqIndex        CLKREQ# number
  @param[out] NativePin          Native pin data
**/
VOID
GpioGetPcieClkReqPin (
  IN  UINT32                      ClkreqIndex,
  OUT GPIO_PAD_NATIVE_FUNCTION    *NativePin
  )
{
  if (IsPchLp ()) {
    if (ClkreqIndex < ARRAY_SIZE (mPchLpPcieSrcClkReqbPinToGpioMap)) {
      *NativePin = mPchLpPcieSrcClkReqbPinToGpioMap[ClkreqIndex];
      return;
    }
  } else {
    if (ClkreqIndex < ARRAY_SIZE (mPchHPcieSrcClkReqbPinToGpioMap)) {
      *NativePin = mPchHPcieSrcClkReqbPinToGpioMap[ClkreqIndex];
      return;
    }
  }
  *NativePin = (GPIO_PAD_NATIVE_FUNCTION){0};
  ASSERT (FALSE);
}

//
// PCHHOTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpPchHotbPin = {GPIO_CNL_LP_GPP_B23,  GpioPadModeNative2};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHPchHotbPin = {GPIO_CNL_H_GPP_B23,  GpioPadModeNative2};

/**
  This function sets PCHHOT pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnablePchHot (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION PchHotbPin;

  if (IsPchLp ()) {
    PchHotbPin = mPchLpPchHotbPin;
  } else {
    PchHotbPin = mPchHPchHotbPin;
  }

  return GpioSetPadMode (PchHotbPin.Pad, PchHotbPin.Mode);
}

//
// VRALERTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpVrAlertbPin = {GPIO_CNL_LP_GPP_B2, GpioPadModeNative1};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHVrAlertbPin = {GPIO_CNL_H_GPP_B2, GpioPadModeNative1};

//
// CPU_C10_GATE pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCpuC10GatePin = {GPIO_CNL_LP_GPP_H18, GpioPadModeNative1};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCpuC10GatePin = {GPIO_CNL_H_GPP_J1, GpioPadModeNative2};

/**
  This function sets VRALERTB pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableVrAlert (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION  VrAlertGpio;

  if (IsPchLp ()) {
    VrAlertGpio = mPchLpVrAlertbPin;
  } else {
    VrAlertGpio = mPchHVrAlertbPin;
  }

  return GpioSetPadMode (VrAlertGpio.Pad, VrAlertGpio.Mode);
}

/**
This function sets CPU C10 Gate pins into native mode

@retval Status
**/
EFI_STATUS
GpioEnableCpuC10GatePin (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION  CpuC10GateGpio;

  if (IsPchLp ()) {
    CpuC10GateGpio = mPchLpCpuC10GatePin;
  } else {
    CpuC10GateGpio = mPchHCpuC10GatePin;
  }

  return GpioSetPadMode (CpuC10GateGpio.Pad, CpuC10GateGpio.Mode);
}

//
// CPU GP pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCpuGpPinMap[PCH_GPIO_CPU_GP_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_E3, GpioPadModeNative1}, // CPU_GP_0
  {GPIO_CNL_LP_GPP_E7, GpioPadModeNative1}, // CPU_GP_1
  {GPIO_CNL_LP_GPP_B3, GpioPadModeNative1}, // CPU_GP_2
  {GPIO_CNL_LP_GPP_B4, GpioPadModeNative1}, // CPU_GP_3
};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCpuGpPinMap[PCH_GPIO_CPU_GP_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_E3, GpioPadModeNative1}, // CPU_GP_0
  {GPIO_CNL_H_GPP_E7, GpioPadModeNative1}, // CPU_GP_1
  {GPIO_CNL_H_GPP_B3, GpioPadModeNative1}, // CPU_GP_2
  {GPIO_CNL_H_GPP_B4, GpioPadModeNative1}, // CPU_GP_3
};

/**
  This function sets CPU GP pins into native mode

  @param[in]  CpuGpPinNum               CPU GP pin number

  @retval Status
**/
EFI_STATUS
GpioEnableCpuGpPin (
  IN  UINT32                            CpuGpPinNum
  )
{
  GPIO_PAD_NATIVE_FUNCTION CpuGpPin;

  if (IsPchLp ()) {
    if (CpuGpPinNum >= ARRAY_SIZE (mPchLpCpuGpPinMap)) {
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    CpuGpPin = mPchLpCpuGpPinMap[CpuGpPinNum];
  } else {
    if (CpuGpPinNum >= ARRAY_SIZE (mPchHCpuGpPinMap)) {
      ASSERT(FALSE);
      return EFI_UNSUPPORTED;
    }
    CpuGpPin = mPchHCpuGpPinMap[CpuGpPinNum];
  }

  return GpioSetPadMode (CpuGpPin.Pad, CpuGpPin.Mode);
}

//
// DDSP_HPD pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpDdspHpdPins[] =
{
  {GPIO_CNL_LP_GPP_E13, GpioPadModeNative1},// DDSP_HPD_0
  {GPIO_CNL_LP_GPP_E14, GpioPadModeNative1},// DDSP_HPD_1
  {GPIO_CNL_LP_GPP_E15, GpioPadModeNative1},// DDSP_HPD_2
  {GPIO_CNL_LP_GPP_E16, GpioPadModeNative1} // DDSP_HPD_3
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHDdspHpdPins[] =
{
  {GPIO_CNL_H_GPP_I0, GpioPadModeNative1},// DDSP_HPD_0
  {GPIO_CNL_H_GPP_I1, GpioPadModeNative1},// DDSP_HPD_1
  {GPIO_CNL_H_GPP_I2, GpioPadModeNative1},// DDSP_HPD_2
  {GPIO_CNL_H_GPP_I3, GpioPadModeNative1} // DDSP_HPD_3
};

/**
  This function sets DDSP_HPDx pin into native mode

  @param[in]  DdspHpdPin     DDSP_HPDx pin

  @retval Status
**/
EFI_STATUS
GpioEnableDpHotPlugDetect (
  IN GPIO_DDSP_HPD  DdspHpdPin
  )
{
  GPIO_PAD_NATIVE_FUNCTION  DdspHpdGpio;
  UINT32                    DdspHpdPinIndex;

  if (DdspHpdPin > GpioDdspHpd3) {
    return EFI_UNSUPPORTED;
  }

  DdspHpdPinIndex = DdspHpdPin - GpioDdspHpd0;

  if (IsPchLp ()) {
    if (DdspHpdPinIndex >= ARRAY_SIZE (mPchLpDdspHpdPins)) {
      goto Error;
    }
    DdspHpdGpio = mPchLpDdspHpdPins[DdspHpdPinIndex];
  } else {
    if (DdspHpdPinIndex >= ARRAY_SIZE (mPchHDdspHpdPins)) {
      goto Error;
    }
    DdspHpdGpio = mPchHDdspHpdPins[DdspHpdPinIndex];
  }

  return GpioSetPadMode (DdspHpdGpio.Pad, DdspHpdGpio.Mode);
Error:
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

//
// EDP HPD, VDD and BKLT pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpEdpPins[PCH_GPIO_EDP_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_E17,         GpioPadModeNative1},// EDP_HPD
  {GPIO_CNL_LP_HVMOS_L_VDDEN,   GpioPadModeNative1},// VDDEN
  {GPIO_CNL_LP_HVMOS_L_BKLTEN,  GpioPadModeNative1},// BKLTEN
  {GPIO_CNL_LP_HVMOS_L_BKLTCTL, GpioPadModeNative1} // BKLTCTL
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHEdpPins[PCH_GPIO_EDP_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_I4,  GpioPadModeNative1},// EDP_HPD
  {GPIO_CNL_H_GPP_F19, GpioPadModeNative1},// VDDEN
  {GPIO_CNL_H_GPP_F20, GpioPadModeNative1},// BKLTEN
  {GPIO_CNL_H_GPP_F21, GpioPadModeNative1} // BKLTCTL
};

/**
  This function provides eDP pins

  @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetEdpPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpEdpPins;
    *NoOfNativePins = ARRAY_SIZE (mPchLpEdpPins);
  } else {
    *NativePinsTable = mPchHEdpPins;
    *NoOfNativePins = ARRAY_SIZE (mPchHEdpPins);
  }
}

//
// DDPB/C/D/F  CTRLCLK and CTRLDATA pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpDdpInterfacePins[PCH_GPIO_DDP_NUMBER_OF_INTERFACES][PCH_GPIO_DDP_NUMBER_OF_PINS] =
{
  {// DDPB
    {GPIO_CNL_LP_GPP_E18, GpioPadModeNative1},// DDPB_CTRLCLK
    {GPIO_CNL_LP_GPP_E19, GpioPadModeNative1} // DDPB_CTRLDATA
  },
  {// DDPC
    {GPIO_CNL_LP_GPP_E20, GpioPadModeNative1},// DDPC_CTRLCLK
    {GPIO_CNL_LP_GPP_E21, GpioPadModeNative1} // DDPC_CTRLDATA
  },
  {// DDPD
    {GPIO_CNL_LP_GPP_E22, GpioPadModeNative1},// DDPD_CTRLCLK
    {GPIO_CNL_LP_GPP_E23, GpioPadModeNative1} // DDPD_CTRLDATA
  },
  {// DDPF
    {GPIO_CNL_LP_GPP_H16, GpioPadModeNative1},// DDPF_CTRLCLK
    {GPIO_CNL_LP_GPP_H17, GpioPadModeNative1} // DDPF_CTRLDATA
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHDdpInterfacePins[PCH_GPIO_DDP_NUMBER_OF_INTERFACES][PCH_GPIO_DDP_NUMBER_OF_PINS] =
{
  {// DDPB
    {GPIO_CNL_H_GPP_I5,  GpioPadModeNative1}, // DDPB_CTRLCLK
    {GPIO_CNL_H_GPP_I6,  GpioPadModeNative1}, // DDPB_CTRLDATA
  },
  {// DDPC
    {GPIO_CNL_H_GPP_I7,  GpioPadModeNative1}, // DDPC_CTRLCLK
    {GPIO_CNL_H_GPP_I8,  GpioPadModeNative1}, // DDPC_CTRLDATA
  },
  {// DDPD
    {GPIO_CNL_H_GPP_I9,  GpioPadModeNative1}, // DDPD_CTRLCLK
    {GPIO_CNL_H_GPP_I10, GpioPadModeNative1}, // DDPD_CTRLDATA
  },
  {// DDPF
    {GPIO_CNL_H_GPP_F22, GpioPadModeNative1}, // DDPF_CTRLCLK
    {GPIO_CNL_H_GPP_F23, GpioPadModeNative1}, // DDPF_CTRLDATA
  }
};

/**
  This function provides DDPx interface pins

  @param[in]  DdpInterface   DDPx interface

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetDdpPins (
  IN  GPIO_DDP                    DdpInterface,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  UINT32  DdpInterfaceIndex;

  switch (DdpInterface) {
    case GpioDdpB:
    case GpioDdpC:
    case GpioDdpD:
      DdpInterfaceIndex = DdpInterface - GpioDdpB;
      break;
    case GpioDdpF:
      DdpInterfaceIndex = 3;
      break;
    default:
      goto Error;
  }

  if (IsPchLp ()) {
    if (DdpInterfaceIndex < ARRAY_SIZE (mPchLpDdpInterfacePins)) {
      *NativePinsTable = mPchLpDdpInterfacePins[DdpInterfaceIndex];
      return;
    }
  } else {
    if (DdpInterfaceIndex < ARRAY_SIZE (mPchHDdpInterfacePins)) {
      *NativePinsTable = mPchHDdpInterfacePins[DdpInterfaceIndex];
      return;
    }
  }
Error:
  *NativePinsTable = NULL;
  ASSERT(FALSE);
}

/**
  This function enables CNVi RF Reset pin
**/
VOID
GpioEnableCnviRfResetPin (
  VOID
  )
{
  EFI_STATUS      Status;
  GPIO_PAD        GpioPad;
  GPIO_PAD_MODE   PadMode;

  if (IsPchLp ()) {
    GpioPad = GPIO_CNL_LP_GPP_H1;
    PadMode = GpioPadModeNative3;
  } else {
    GpioPad = GPIO_CNL_H_GPP_D5;
    PadMode = GpioPadModeNative3;
  }

  Status = GpioSetPadMode (GpioPad, PadMode);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function enables CNVi MODEM CLKREQ pin
**/
VOID
GpioEnableCnviModemClkReqPin (
  VOID
  )
{
  EFI_STATUS      Status;
  GPIO_PAD        GpioPad;
  GPIO_PAD_MODE   PadMode;

  if (IsPchLp ()) {
    GpioPad = GPIO_CNL_LP_GPP_H2;
    PadMode = GpioPadModeNative3;
  } else {
    GpioPad = GPIO_CNL_H_GPP_D6;
    PadMode = GpioPadModeNative3;
  }

  Status = GpioSetPadMode (GpioPad, PadMode);
  ASSERT_EFI_ERROR (Status);
}


/**
  This function provides CNVi BT interface select pin

  @retval GpioPad          GPIO pad for CNVi BT interface select
**/
GPIO_PAD
GpioGetCnviBtIfSelectPin (
  VOID
  )
{
  if (IsPchLp ()) {
    return GPIO_CNL_LP_VGPIO5;
  } else {
    return GPIO_CNL_H_VGPIO7;
  }
}

/**
  This function provides CNVi BT Charging pin

  @retval GpioPad          GPIO pad for CNVi BT Charging select
**/
GPIO_PAD
GpioGetCnviBtChargingPin (
  VOID
  )
{
  if (IsPchLp ()) {
    return GPIO_CNL_LP_VGPIO3;
  } else {
    return GPIO_CNL_H_VGPIO3;
  }
}

/**
  This function provides CNVi A4WP pin

  @param[out] GpioNativePad       GPIO native pad for CNVi A4WP
**/
VOID
GpioGetCnviA4WpPin (
  OUT GPIO_PAD_NATIVE_FUNCTION  *GpioNativePad
  )
{
  GpioNativePad->Mode = GpioPadModeNative1;
  if (IsPchLp ()) {
    GpioNativePad->Pad = GPIO_CNL_LP_GPP_F23;
  } else {
    GpioNativePad->Pad = GPIO_CNL_H_GPP_J11;
  }
}

/**
  This function provides CNVi BT host wake int pin

  @retval GpioPad          GPIO pad BT host wake int
**/
GPIO_PAD
GpioGetCnviBtHostWakeIntPin (
  VOID
  )
{
  if (IsPchLp ()) {
    return GPIO_CNL_LP_VGPIO4;
  } else {
    return GPIO_CNL_H_VGPIO4;
  }
}

//
// CNVi Bluetooth UART pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchLpVCnviBtUartGpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_LP_VGPIO6, // vCNV_BT_UART_TXD
  GPIO_CNL_LP_VGPIO7, // vCNV_BT_UART_RXD
  GPIO_CNL_LP_VGPIO8, // vCNV_BT_UART_CTS_B
  GPIO_CNL_LP_VGPIO9  // vCNV_BT_UART_RTS_B
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchHVCnviBtUartGpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_H_VGPIO8, // vCNV_BT_UART_TXD
  GPIO_CNL_H_VGPIO9, // vCNV_BT_UART_RXD
  GPIO_CNL_H_VGPIO10,// vCNV_BT_UART_CTS_B
  GPIO_CNL_H_VGPIO11 // vCNV_BT_UART_RTS_B
};

//
// vUART for Bluetooth
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchLpVUartForCnviBtGpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_LP_VGPIO18, // vUART0_TXD
  GPIO_CNL_LP_VGPIO19, // vUART0_RXD
  GPIO_CNL_LP_VGPIO20, // vUART0_CTS_B
  GPIO_CNL_LP_VGPIO21  // vUART0_RTS_B
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchHVUartForCnviBtGpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_H_VGPIO20, // vUART0_TXD
  GPIO_CNL_H_VGPIO21, // vUART0_RXD
  GPIO_CNL_H_VGPIO22, // vUART0_CTS_B
  GPIO_CNL_H_VGPIO23  // vUART0_RTS_B
};

/**
  This function provides CNVi BT UART pins

  @param[in]  ConnectionType           CNVi BT UART connection type
  @param[out] VCnviBtUartPad           Table with vCNV_BT_UARTx pads
  @param[out] VCnviBtUartPadMode       vCNV_BT_UARTx pad mode
  @param[out] VUartForCnviBtPad        Table with vUART0 pads
  @param[out] VUartForCnviBtPadMode    vUART0 pad mode
**/
VOID
GpioGetCnviBtUartPins (
  IN  VGPIO_CNVI_BT_UART_CONNECTION_TYPE  ConnectionType,
  OUT GPIO_PAD                            **VCnviBtUartPad,
  OUT GPIO_PAD_MODE                       *VCnviBtUartPadMode,
  OUT GPIO_PAD                            **VUartForCnviBtPad,
  OUT GPIO_PAD_MODE                       *VUartForCnviBtPadMode
  )
{
  if (IsPchLp ()) {
    *VCnviBtUartPad = mPchLpVCnviBtUartGpioPad;
    *VUartForCnviBtPad = mPchLpVUartForCnviBtGpioPad;
  } else {
    *VCnviBtUartPad = mPchHVCnviBtUartGpioPad;
    *VUartForCnviBtPad = mPchHVUartForCnviBtGpioPad;
  }

  switch (ConnectionType) {
    case GpioCnviBtUartToSerialIoUart0:
      *VCnviBtUartPadMode = GpioPadModeNative1;
      *VUartForCnviBtPadMode = GpioPadModeNative1;
      break;
    case GpioCnviBtUartToIshUart0:
      *VCnviBtUartPadMode = GpioPadModeNative2;
      *VUartForCnviBtPadMode = GpioPadModeNative1;
      break;
    case GpioCnviBtUartNotConnected:
    case GpioCnviBtUartToExternalPads:
      *VCnviBtUartPadMode = GpioPadModeGpio;
      *VUartForCnviBtPadMode = GpioPadModeGpio;
      break;
    default:
      ASSERT (FALSE);
      return;
  }
}

//
// CNVi Bluetooth UART external pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCnviBtUartExternalPads[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_C8,  GpioPadModeNative2}, // CNV_BT_UART_0_RXD
  {GPIO_CNL_LP_GPP_C9,  GpioPadModeNative2}, // CNV_BT_UART_0_TXD
  {GPIO_CNL_LP_GPP_C10, GpioPadModeNative2}, // CNV_BT_UART_0_RTS
  {GPIO_CNL_LP_GPP_C11, GpioPadModeNative2}  // CNV_BT_UART_0_CTS
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCnviBtUartExternalPads[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_C8,  GpioPadModeNative2}, // CNV_BT_UART_0_RXD
  {GPIO_CNL_H_GPP_C9,  GpioPadModeNative2}, // CNV_BT_UART_0_TXD
  {GPIO_CNL_H_GPP_C10, GpioPadModeNative2}, // CNV_BT_UART_0_RTS
  {GPIO_CNL_H_GPP_C11, GpioPadModeNative2}  // CNV_BT_UART_0_CTS
};

/**
  This function provides CNVi BT UART external pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnviBtUartExternalPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpCnviBtUartExternalPads;
  } else {
    *NativePinsTable = mPchHCnviBtUartExternalPads;
  }
}

//
// CNVi Bluetooth I2S pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchLpVCnviBtI2sGpioPad[PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  GPIO_CNL_LP_VGPIO30, // vCNV_BT_I2S_BCLK
  GPIO_CNL_LP_VGPIO31, // vCNV_BT_I2S_WS_SYNC
  GPIO_CNL_LP_VGPIO32, // vCNV_BT_I2S_SDO
  GPIO_CNL_LP_VGPIO33  // vCNV_BT_I2S_SDI
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchHVCnviBtI2sGpioPad[PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  GPIO_CNL_H_VGPIO32, // vCNV_BT_I2S_BCLK
  GPIO_CNL_H_VGPIO33, // vCNV_BT_I2S_WS_SYNC
  GPIO_CNL_H_VGPIO34, // vCNV_BT_I2S_SDO
  GPIO_CNL_H_VGPIO35  // vCNV_BT_I2S_SDI
};

//
// vSSP for Bluetooth
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchLpVSspForCnviBtGpioPad[PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  GPIO_CNL_LP_VGPIO34, // vSSP2_SCLK
  GPIO_CNL_LP_VGPIO35, // vSSP2_SFRM
  GPIO_CNL_LP_VGPIO36, // vSSP2_TXD
  GPIO_CNL_LP_VGPIO37  // vSSP2_RXD
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchHVSspForCnviBtGpioPad[PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  GPIO_CNL_H_VGPIO36, // vSSP2_SCLK
  GPIO_CNL_H_VGPIO37, // vSSP2_SFRM
  GPIO_CNL_H_VGPIO38, // vSSP2_TXD
  GPIO_CNL_H_VGPIO39  // vSSP2_RXD
};

/**
  This function provides CNVi BT I2S pins

  @param[in]  ConnectionType          CNVi BT I2S connection type
  @param[out] VCnviBtI2sPad           Table with vCNV_BT_I2Sx pads
  @param[out] VCnviBtI2sPadMode       vCNV_BT_I2Sx pad mode
  @param[out] VSspForCnviBtPad        Table with vSSP2 pads
  @param[out] VSspForCnviBtPadMode    vSSP2 pad mode
**/
VOID
GpioGetCnviBtI2sPins (
  IN  VGPIO_CNVI_BT_I2S_CONNECTION_TYPE  ConnectionType,
  OUT GPIO_PAD                 **VCnviBtI2sPad,
  OUT GPIO_PAD_MODE            *VCnviBtI2sPadMode,
  OUT GPIO_PAD                 **VSspForCnviBtPad,
  OUT GPIO_PAD_MODE            *VSspForCnviBtPadMode
  )
{
  if (IsPchLp ()) {
    *VCnviBtI2sPad = mPchLpVCnviBtI2sGpioPad;
    *VSspForCnviBtPad = mPchLpVSspForCnviBtGpioPad;
  } else {
    *VCnviBtI2sPad = mPchHVCnviBtI2sGpioPad;
    *VSspForCnviBtPad = mPchHVSspForCnviBtGpioPad;
  }

  switch (ConnectionType) {
    case GpioCnviBtI2sToSsp0:
      *VCnviBtI2sPadMode = GpioPadModeNative1;
      *VSspForCnviBtPadMode = GpioPadModeNative1;
      break;
    case GpioCnviBtI2sToSsp1:
      *VCnviBtI2sPadMode = GpioPadModeNative2;
      *VSspForCnviBtPadMode = GpioPadModeNative1;
      break;
    case GpioCnviBtI2sToSsp2:
      *VCnviBtI2sPadMode = GpioPadModeNative3;
      *VSspForCnviBtPadMode = GpioPadModeNative1;
      break;
    case GpioCnviBtI2sNotConnected:
    case GpioCnviBtI2sToExternalPads:
      *VCnviBtI2sPadMode = GpioPadModeGpio;
      *VSspForCnviBtPadMode = GpioPadModeGpio;
      break;
    default:
      ASSERT (FALSE);
      return;
  }
}

//
// CNVi Bluetooth I2S external pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCnviBtI2sExternalPads[PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_H0, GpioPadModeNative2}, // CNV_BT_I2S_WS_SYNC
  {GPIO_CNL_LP_GPP_H1, GpioPadModeNative2}, // CNV_BT_I2S_BCLK
  {GPIO_CNL_LP_GPP_H2, GpioPadModeNative2}, // CNV_BT_I2S_SDI
  {GPIO_CNL_LP_GPP_H3, GpioPadModeNative2}  // CNV_BT_I2S_SDO
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCnviBtI2sExternalPads[PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_D8, GpioPadModeNative2}, // CNV_BT_I2S_WS_SYNC
  {GPIO_CNL_H_GPP_D5, GpioPadModeNative2}, // CNV_BT_I2S_BCLK
  {GPIO_CNL_H_GPP_D6, GpioPadModeNative2}, // CNV_BT_I2S_SDI
  {GPIO_CNL_H_GPP_D7, GpioPadModeNative2}  // CNV_BT_I2S_SDO
};

/**
  This function provides CNVi BT I2S external pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnviBtI2sExternalPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpCnviBtI2sExternalPads;
  } else {
    *NativePinsTable = mPchHCnviBtI2sExternalPads;
  }
}

//
// CNVi MFUART1 pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchLpVCnviMfUart1GpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_LP_VGPIO10, // vCNV_MFUART1_TXD
  GPIO_CNL_LP_VGPIO11, // vCNV_MFUART1_RXD
  GPIO_CNL_LP_VGPIO12, // vCNV_MFUART1_CTS_B
  GPIO_CNL_LP_VGPIO13  // vCNV_MFUART1_RTS_B
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchHVCnviMfUart1GpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_H_VGPIO12, // vCNV_MFUART1_TXD
  GPIO_CNL_H_VGPIO13, // vCNV_MFUART1_RXD
  GPIO_CNL_H_VGPIO14, // vCNV_MFUART1_CTS_B
  GPIO_CNL_H_VGPIO15  // vCNV_MFUART1_RTS_B
};

//
// vUART for MFUART1
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchLpVUartForCnviMfUart1GpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_LP_VGPIO22, // vISH_UART0_TXD
  GPIO_CNL_LP_VGPIO23, // vISH_UART0_RXD
  GPIO_CNL_LP_VGPIO24, // vISH_UART0_CTS_B
  GPIO_CNL_LP_VGPIO25  // vISH_UART0_RTS_B
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mPchHVUartForCnviMfUart1GpioPad[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  GPIO_CNL_H_VGPIO24, // vISH_UART0_TXD
  GPIO_CNL_H_VGPIO25, // vISH_UART0_RXD
  GPIO_CNL_H_VGPIO26, // vISH_UART0_CTS_B
  GPIO_CNL_H_VGPIO27  // vISH_UART0_RTS_B
};

/**
  This function provides CNVi MFUART1 pins

  @param[in]  ConnectionType          CNVi MFUART1 connection type
  @param[out] VCnviBtI2sPad           Table with vCNV_MFUART1x pads
  @param[out] VCnviBtI2sPadMode       vCNV_MFUART1x pad mode
  @param[out] VSspForCnviBtPad        Table with vISH_UART0 pads
  @param[out] VSspForCnviBtPadMode    vISH_UART0 pad mode
**/
VOID
GpioGetCnviMfUart1Pins (
  IN  VGPIO_CNVI_MF_UART1_CONNECTION_TYPE  ConnectionType,
  OUT GPIO_PAD                 **VCnviMfUart1Pad,
  OUT GPIO_PAD_MODE            *VCnviMfUart1PadMode,
  OUT GPIO_PAD                 **VUartForCnviMfUart1Pad,
  OUT GPIO_PAD_MODE            *VUartForCnviMfUart1PadMode
  )
{
  if (IsPchLp ()) {
    *VCnviMfUart1Pad = mPchLpVCnviMfUart1GpioPad;
    *VUartForCnviMfUart1Pad = mPchLpVUartForCnviMfUart1GpioPad;
  } else {
    *VCnviMfUart1Pad = mPchHVCnviMfUart1GpioPad;
    *VUartForCnviMfUart1Pad = mPchHVUartForCnviMfUart1GpioPad;
  }

  switch (ConnectionType) {
    case GpioCnviMfUart1ToSerialIoUart2:
      *VCnviMfUart1PadMode = GpioPadModeNative2;
      *VUartForCnviMfUart1PadMode = GpioPadModeNative1;
      break;
    case GpioCnviMfUart1ToIshUart0:
      *VCnviMfUart1PadMode = GpioPadModeNative1;
      *VUartForCnviMfUart1PadMode = GpioPadModeNative1;
      break;
    case GpioCnviMfUart1NotConnected:
    case GpioCnviMfUart1ToExternalPads:
      *VCnviMfUart1PadMode = GpioPadModeGpio;
      *VUartForCnviMfUart1PadMode = GpioPadModeGpio;
      break;
    default:
      ASSERT (FALSE);
      return;
  }
}

//
// CNVi MFUART1 external pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCnviMfUart1ExternalPads[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_C12, GpioPadModeNative3}, // CNV_MFUART1_RXD
  {GPIO_CNL_LP_GPP_C13, GpioPadModeNative3}, // CNV_MFUART1_TXD
  {GPIO_CNL_LP_GPP_C14, GpioPadModeNative3}, // CNV_MFUART1_RTS
  {GPIO_CNL_LP_GPP_C15, GpioPadModeNative3}  // CNV_MFUART1_CTS
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCnviMfUart1ExternalPads[PCH_GPIO_CNVI_UART_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_C12, GpioPadModeNative3}, // CNV_MFUART1_RXD
  {GPIO_CNL_H_GPP_C13, GpioPadModeNative3}, // CNV_MFUART1_TXD
  {GPIO_CNL_H_GPP_C14, GpioPadModeNative3}, // CNV_MFUART1_RTS
  {GPIO_CNL_H_GPP_C15, GpioPadModeNative3}  // CNV_MFUART1_CTS
};

/**
  This function provides CNVi MFUART1 external pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnviMfUart1ExternalPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpCnviMfUart1ExternalPads;
  } else {
    *NativePinsTable = mPchHCnviMfUart1ExternalPads;
  }
}

/**
  This function provides CNVi Bluetooth Enable pad

  @retval GpioPad           CNVi Bluetooth Enable pad
**/
GPIO_PAD
GpioGetCnviBtEnablePin (
  VOID
  )
{
  if (IsPchLp ()) {
    return GPIO_CNL_LP_VGPIO0;
  } else {
    return GPIO_CNL_H_VGPIO0;
  }
}

//
// CNVi BRI (Bluetooth Radio Interface) and RGI (Radio Generic Interface) buses from Pulsar to CRF (Companion RF)
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpCnviBriRgiGpioPad[PCH_GPIO_CNVI_BRI_RGI_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_F4, GpioPadModeNative1}, // CNV_BRI_DT
  {GPIO_CNL_LP_GPP_F5, GpioPadModeNative1}, // CNV_BRI_RSP
  {GPIO_CNL_LP_GPP_F6, GpioPadModeNative1}, // CNV_RGI_DT
  {GPIO_CNL_LP_GPP_F7, GpioPadModeNative1}  // CNV_RGI_RSP
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCnviBriRgiGpioPad[PCH_GPIO_CNVI_BRI_RGI_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_J4, GpioPadModeNative1}, // CNV_BRI_DT
  {GPIO_CNL_H_GPP_J5, GpioPadModeNative1}, // CNV_BRI_RSP
  {GPIO_CNL_H_GPP_J6, GpioPadModeNative1}, // CNV_RGI_DT
  {GPIO_CNL_H_GPP_J7, GpioPadModeNative1}  // CNV_RGI_RSP
};

/**
  This function provides CNVi BRI RGI GPIO pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnvBriRgiPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpCnviBriRgiGpioPad;
  } else {
    *NativePinsTable = mPchHCnviBriRgiGpioPad;
  }
}


/**
  This function sets CNVi WiFi mode

  @param[in] Value                CNVi WiFi Mode value
                                  GpioCnviWiFiAuto: WiFi is automatically enabled/disabled by WiFi core
                                  GpioCnviWiFiEnabled: WiFi is enabled regardless of WiFi core decision
  @retval Status
**/
EFI_STATUS
GpioSetCnviWifiMode (
  IN  VGPIO_CNVI_WIFI_MODE  WiFiMode
  )
{
  EFI_STATUS  Status;
  GPIO_PAD    CnviWifiModePad;
  GPIO_CONFIG PadConfig;

  ZeroMem (&PadConfig, sizeof (PadConfig));

  PadConfig.PadMode = GpioPadModeGpio;
  PadConfig.Direction = GpioDirOut;
  if (WiFiMode == GpioCnviWiFiEnabled) {
    PadConfig.OutputState = GpioOutHigh;
  } else {
    PadConfig.OutputState = GpioOutLow;
  }

  if (IsPchLp ()) {
    CnviWifiModePad = GPIO_CNL_LP_VGPIO2;
  } else {
    CnviWifiModePad = GPIO_CNL_H_VGPIO2;
  }

  Status = GpioSetPadConfig (CnviWifiModePad, &PadConfig);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchLpImguClkOutGpioPad[SA_GPIO_IMGUCLK_NUMBER_OF_PINS] =
{
  {GPIO_CNL_LP_GPP_D4,  GpioPadModeNative1}, // IMGCLKOUT_0
  {GPIO_CNL_LP_GPP_H20, GpioPadModeNative1}, // IMGCLKOUT_1
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHImguClkOutGpioPad[SA_GPIO_IMGUCLK_NUMBER_OF_PINS] =
{
  {GPIO_CNL_H_GPP_K22, GpioPadModeNative1}, // IMGCLKOUT_0
  {GPIO_CNL_H_GPP_K23, GpioPadModeNative1}, // IMGCLKOUT_1
};

/**
  This function provides IMGCLKOUT pins

  @param[out] NativePinsTable          Table with pins
  @param[out] NoOfNativePins            Number of pins
**/
VOID
GpioGetImgClkOutPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable,
  OUT UINT32                   *NoOfNativePins
  )
{
  if (IsPchLp ()) {
    *NativePinsTable = mPchLpImguClkOutGpioPad;
    *NoOfNativePins = ARRAY_SIZE (mPchLpImguClkOutGpioPad);
  } else {
    *NativePinsTable = mPchHImguClkOutGpioPad;
    *NoOfNativePins = ARRAY_SIZE (mPchHImguClkOutGpioPad);
  }
}

/**
  This function provides PWRBTN pin

  @retval GpioPad          PWRTBTN pin
**/
GPIO_PAD
GpioGetPwrBtnPin (
  VOID
  )
{
  if (IsPchLp ()) {
    return GPIO_CNL_LP_GPD3;
  } else {
    return GPIO_CNL_H_GPD3;
  }
}

/**
  This function provides LPC pin

  @retval GpioPad          LPC pin
**/
GPIO_PAD
GpioGetLpcPin (
  VOID
  )
{
  if (PchGetLpcDid () == V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A305_SKU) {
    return GPIO_CNL_H_GPP_A8;
  } else {
    return 0;
  }
}
