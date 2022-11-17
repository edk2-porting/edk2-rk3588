/** @file
  PCH Serial IO Lib implementation Cannon Lake specific.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Library/PchSerialIoLib.h>
#include <Private/Library/PchPsfPrivateLib.h>

#include <PchLimits.h>
#include <Register/PchRegsSerialIoCnl.h>
#include "PchSerialIoLibInternal.h"

GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 mCnlAcpiHid[PCH_MAX_SERIALIO_CONTROLLERS][SERIALIO_HID_LENGTH] =
{
  "INT34B2",
  "INT34B3",
  "INT34B4",
  "INT34B5",
  "INT34B6",
  "INT34B7",
  "INT34B0",
  "INT34B1",
  "INT34BC",
  "INT34B8",
  "INT34B9",
  "INT34BA"
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mCnlPchLpSerialIoId [PCH_MAX_SERIALIO_CONTROLLERS] =
{
  V_CNL_PCH_LP_SERIAL_IO_CFG_I2C0_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_I2C1_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_I2C2_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_I2C3_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_I2C4_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_I2C5_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_SPI0_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_SPI1_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_UART0_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_UART1_DEVICE_ID,
  V_CNL_PCH_LP_SERIAL_IO_CFG_UART2_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mCnlPchHSerialIoId [PCH_MAX_SERIALIO_CONTROLLERS] =
{
  V_CNL_PCH_H_SERIAL_IO_CFG_I2C0_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_I2C1_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_I2C2_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_I2C3_DEVICE_ID,
                                         0,
                                         0,
  V_CNL_PCH_H_SERIAL_IO_CFG_SPI0_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_SPI1_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_UART0_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_UART1_DEVICE_ID,
  V_CNL_PCH_H_SERIAL_IO_CFG_UART2_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_BDF_NUMBERS mSerialIoBdf [PCH_MAX_SERIALIO_CONTROLLERS] =
{
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0, PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1, PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2, PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2}
};


/**
  Returns index of the last i2c controller

  @param[in] Number                     Number of SerialIo controller

  @retval                               Index of I2C controller
**/
PCH_SERIAL_IO_CONTROLLER
GetMaxI2cNumber (
  VOID
  )
{
  if (IsPchH ()) {
    return PchSerialIoIndexI2C3;
  } else {
    return PchSerialIoIndexI2C5;
  }
}

/**
  Checks if Device with given PciDeviceId is one of SerialIo controllers
  If yes, its number is returned through Number parameter, otherwise Number is not updated

  @param[in]  PciDevId                  Device ID
  @param[out] Number                    Number of SerialIo controller

  @retval TRUE                          Yes it is a SerialIo controller
  @retval FALSE                         No it isn't a SerialIo controller
**/
BOOLEAN
IsSerialIoPciDevId (
  IN  UINT16                    PciDevId,
  OUT PCH_SERIAL_IO_CONTROLLER  *Number
  )
{
  PCH_SERIAL_IO_CONTROLLER Controller;

  for (Controller = 0; Controller < GetPchMaxSerialIoControllersNum (); Controller++) {
    if ((IsPchLp () && (PciDevId == mCnlPchLpSerialIoId[Controller])) ||
        (IsPchH () && (PciDevId == mCnlPchHSerialIoId[Controller])))
    {
      *Number = Controller;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Finds PCI Device Number of SerialIo devices.

  @param[in] SerialIoNumber             Serial IO device

  @retval                               SerialIo device number
**/
UINT8
GetSerialIoDeviceNumber (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  )
{
  return mSerialIoBdf[SerialIoNumber].DevNum;
}

/**
  Finds PCI Function Number of SerialIo devices.

  @param[in] SerialIoNumber             Serial IO device

  @retval                               SerialIo funciton number
**/
UINT8
GetSerialIoFunctionNumber (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  )
{
  return mSerialIoBdf[SerialIoNumber].FuncNum;
}

/**
  Returns string with AcpiHid assigned to selected SerialIo controller

  @param[in] Number                     Number of SerialIo controller

  @retval                               pointer to 8-byte string
**/
CHAR8*
GetSerialIoAcpiHid (
  IN PCH_SERIAL_IO_CONTROLLER Number
  )
{
  return mCnlAcpiHid[Number];
}
