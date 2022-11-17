/** @file
  This file contains routines for GPIO native and chipset specific purpose
  used by Reference Code only.

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
#include <Library/PchInfoLib.h>
#include <Pch/Library/PeiDxeSmmGpioLib/GpioLibrary.h>

//
// Chipset specific data
//
//ISH
extern GPIO_PAD_NATIVE_FUNCTION mPchLpIshGPGpio[PCH_ISH_MAX_GP_PINS];
extern GPIO_PAD_NATIVE_FUNCTION mPchHIshGPGpio[PCH_ISH_MAX_GP_PINS];
extern GPIO_PAD_NATIVE_FUNCTION mPchLpIshI2cGpio[PCH_ISH_MAX_I2C_CONTROLLERS][PCH_ISH_PINS_PER_I2C_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchHIshI2cGpio[PCH_ISH_MAX_I2C_CONTROLLERS][PCH_ISH_PINS_PER_I2C_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchLpIshUartGpio[PCH_ISH_MAX_UART_CONTROLLERS][PCH_ISH_PINS_PER_UART_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchHIshUartGpio[PCH_ISH_MAX_UART_CONTROLLERS][PCH_ISH_PINS_PER_UART_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchLpIshSpiGpio[PCH_ISH_PINS_PER_SPI_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchHIshSpiGpio[PCH_ISH_PINS_PER_SPI_CONTROLLER];


//PCIe SRCCLKREQB
extern GPIO_PAD_NATIVE_FUNCTION mPchLpPcieSrcClkReqbPinToGpioMap[PCH_LP_PCIE_MAX_CLK_REQ];
extern GPIO_PAD_NATIVE_FUNCTION mPchHPcieSrcClkReqbPinToGpioMap[PCH_H_PCIE_MAX_CLK_REQ];

//PCH_HOT
extern GPIO_PAD_NATIVE_FUNCTION mPchLpPchHotbPin;
extern GPIO_PAD_NATIVE_FUNCTION mPchHPchHotbPin;

//CPU GP
extern GPIO_PAD_NATIVE_FUNCTION mPchLpCpuGpPinMap[4];
extern GPIO_PAD_NATIVE_FUNCTION mPchHCpuGpPinMap[4];

/**
  This function sets ISH I2C controller pins into native mode

  @param[in]  IshI2cControllerNumber   I2C controller

  @retval Status
**/
EFI_STATUS
GpioSetIshI2cPinsIntoNativeMode (
  IN  UINT32            IshI2cControllerNumber
  )
{
  EFI_STATUS               Status;
  UINTN                    Index;
  GPIO_PAD_NATIVE_FUNCTION (*I2cGpio) [PCH_ISH_PINS_PER_I2C_CONTROLLER];

  Status = EFI_SUCCESS;

  if (GetPchSeries () == PchLp) {
    I2cGpio = mPchLpIshI2cGpio;
  } else {
    I2cGpio = mPchHIshI2cGpio;
  }

  ASSERT (IshI2cControllerNumber < PCH_ISH_MAX_I2C_CONTROLLERS);

  for (Index = 0; Index < PCH_ISH_PINS_PER_I2C_CONTROLLER; Index++) {
    Status = SetGpioPadMode (I2cGpio[IshI2cControllerNumber][Index].Pad, I2cGpio[IshI2cControllerNumber][Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }
  return Status;
}

/**
  This function sets ISH UART controller pins into native mode

  @param[in]  IshUartControllerNumber   UART controller

  @retval Status
**/
EFI_STATUS
GpioSetIshUartPinsIntoNativeMode (
  IN  UINT32            IshUartControllerNumber
  )
{
  EFI_STATUS    Status;
  UINTN         Index;
  GPIO_PAD_NATIVE_FUNCTION (*UartGpio) [PCH_ISH_PINS_PER_UART_CONTROLLER];

  Status = EFI_SUCCESS;

  if (GetPchSeries () == PchLp) {
    UartGpio = mPchLpIshUartGpio;
  } else {
    UartGpio = mPchHIshUartGpio;
  }

  ASSERT (IshUartControllerNumber < PCH_ISH_MAX_UART_CONTROLLERS);

  for (Index = 0; Index < PCH_ISH_PINS_PER_UART_CONTROLLER; Index++) {
    Status = SetGpioPadMode (UartGpio[IshUartControllerNumber][Index].Pad, UartGpio[IshUartControllerNumber][Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }
  return Status;
}

/**
  This function sets ISH SPI controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioSetIshSpiPinsIntoNativeMode (
  VOID
  )
{
  EFI_STATUS    Status;
  UINTN         Index;
  GPIO_PAD_NATIVE_FUNCTION *SpiGpio;

  Status = EFI_SUCCESS;

  if (GetPchSeries () == PchLp) {
    SpiGpio = mPchLpIshSpiGpio;
  } else {
    SpiGpio = mPchHIshSpiGpio;
  }

  for (Index = 0; Index < PCH_ISH_PINS_PER_SPI_CONTROLLER; Index++) {
    Status = SetGpioPadMode (SpiGpio[Index].Pad, SpiGpio[Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }
  return Status;
}

/**
  This function sets ISH GP pins into native mode

  @param[in]  IshGpPinNumber   ISH GP pin number

  @retval Status
**/
EFI_STATUS
GpioSetIshGpPinsIntoNativeMode (
  IN  UINT32            IshGpPinNumber
  )
{
  EFI_STATUS               Status;
  GPIO_PAD_NATIVE_FUNCTION *IshGp;

  Status = EFI_SUCCESS;

  if (GetPchSeries () == PchLp) {
    IshGp = mPchLpIshGPGpio;
  } else {
    IshGp = mPchHIshGPGpio;
  }
  ASSERT (IshGpPinNumber < PCH_ISH_MAX_GP_PINS);

  Status = SetGpioPadMode (IshGp[IshGpPinNumber].Pad, IshGp[IshGpPinNumber].Mode);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Returns a pad for given CLKREQ# index.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return CLKREQ# pad.
**/
GPIO_PAD
GpioGetClkreqPad (
  IN     UINT32   ClkreqIndex
  )
{
  if (GetPchSeries () == PchLp) {
    ASSERT (ClkreqIndex < PCH_LP_PCIE_MAX_CLK_REQ);
    return mPchLpPcieSrcClkReqbPinToGpioMap[ClkreqIndex].Pad;
  } else {
    ASSERT (ClkreqIndex < PCH_H_PCIE_MAX_CLK_REQ);
    return mPchHPcieSrcClkReqbPinToGpioMap[ClkreqIndex].Pad;
  }
}

/**
  Enables CLKREQ# pad in native mode.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return none
**/
VOID
GpioEnableClkreq (
  IN     UINT32   ClkreqIndex
  )
{
  GPIO_CONFIG        PadConfig;
  GPIO_PAD           ClkreqPad;
  GPIO_PAD_MODE      PadMode;

  ZeroMem (&PadConfig, sizeof (PadConfig));

  if (GetPchSeries () == PchLp) {
    ASSERT (ClkreqIndex < PCH_LP_PCIE_MAX_CLK_REQ);
    ClkreqPad = mPchLpPcieSrcClkReqbPinToGpioMap[ClkreqIndex].Pad;
    PadMode = mPchLpPcieSrcClkReqbPinToGpioMap[ClkreqIndex].Mode;
  } else {
    ASSERT (ClkreqIndex < PCH_H_PCIE_MAX_CLK_REQ);
    ClkreqPad = mPchHPcieSrcClkReqbPinToGpioMap[ClkreqIndex].Pad;
    PadMode = mPchHPcieSrcClkReqbPinToGpioMap[ClkreqIndex].Mode;
  }

  PadConfig.PadMode      = PadMode;
  PadConfig.Direction    = GpioDirNone;
  PadConfig.PowerConfig  = GpioHostDeepReset;
  DEBUG ((DEBUG_INFO, "Enabling CLKREQ%d\n", ClkreqIndex));
  GpioSetPadConfig (ClkreqPad, &PadConfig);
}

/**
  This function checks if GPIO pin for PCHHOTB is in NATIVE MODE

  @param[in]  none

  @retval TRUE                    Pin is in PCHHOTB native mode
          FALSE                   Pin is in gpio mode or is not owned by HOST
**/
BOOLEAN
GpioIsPchHotbPinInNativeMode (
  VOID
  )
{
  EFI_STATUS               Status;
  GPIO_PAD_NATIVE_FUNCTION PchHotbPin;
  GPIO_PAD_MODE            GpioMode;


  if (GetPchSeries () == PchLp) {
    PchHotbPin = mPchLpPchHotbPin;
  } else {
    PchHotbPin = mPchHPchHotbPin;
  }

  Status =  GetGpioPadMode (PchHotbPin.Pad, &GpioMode);

  if ((EFI_ERROR (Status)) || (GpioMode != PchHotbPin.Mode)) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  This function sets CPU GP pins into native mode

  @param[in]  CpuGpPinNum               CPU GP pin number

  @retval Status
**/
EFI_STATUS
GpioSetCpuGpPinsIntoNativeMode (
  IN  UINT32                            CpuGpPinNum
  )
{
  EFI_STATUS               Status;
  GPIO_PAD_NATIVE_FUNCTION *CpuGpPins;

  if (GetPchSeries () == PchLp) {
    CpuGpPins = mPchLpCpuGpPinMap;
  } else {
    CpuGpPins = mPchHCpuGpPinMap;
  }

  ASSERT (CpuGpPinNum < 4);

  Status = SetGpioPadMode (CpuGpPins[CpuGpPinNum].Pad, CpuGpPins[CpuGpPinNum].Mode);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}
