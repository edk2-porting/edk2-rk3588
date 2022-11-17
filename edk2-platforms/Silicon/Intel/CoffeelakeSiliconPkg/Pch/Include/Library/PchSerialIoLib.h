/** @file
  Header file for PCH Serial IO Lib implementation.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SERIAL_IO_LIB_H_
#define _PCH_SERIAL_IO_LIB_H_

typedef enum {
  PchSerialIoIndexI2C0,
  PchSerialIoIndexI2C1,
  PchSerialIoIndexI2C2,
  PchSerialIoIndexI2C3,
  PchSerialIoIndexI2C4,
  PchSerialIoIndexI2C5,
  PchSerialIoIndexSpi0,
  PchSerialIoIndexSpi1,
  PchSerialIoIndexSpi2,
  PchSerialIoIndexUart0,
  PchSerialIoIndexUart1,
  PchSerialIoIndexUart2,
  PchSerialIoIndexMax
} PCH_SERIAL_IO_CONTROLLER;

typedef enum {
  PchSerialIoDisabled,
  PchSerialIoPci,
  PchSerialIoAcpi,
  PchSerialIoHidden
} PCH_SERIAL_IO_MODE;

typedef enum  {
  SERIAL_IO_UNKNOWN = 0,
  SERIAL_IO_I2C,
  SERIAL_IO_SPI,
  SERIAL_IO_UART
} PCH_SERIAL_IO_CONTROLLER_TYPE;

enum PCH_LP_SERIAL_IO_CS_POLARITY {
  PchSerialIoCsActiveLow = 0,
  PchSerialIoCsActiveHigh = 1
};
enum PCH_LP_SERIAL_IO_HW_FLOW_CTRL {
  PchSerialIoHwFlowCtrlDisabled = 0,
  PchSerialIoHwFlowControlEnabled = 1
};

#define SERIALIO_HID_LENGTH 8 // including null terminator
#define SERIALIO_UID_LENGTH 1
#define SERIALIO_CID_LENGTH 1
#define SERIALIO_TOTAL_ID_LENGTH SERIALIO_HID_LENGTH+SERIALIO_UID_LENGTH+SERIALIO_CID_LENGTH

/**
  Returns index of the last i2c controller

  @param[in] Number  Number of SerialIo controller

  @retval            Index of I2C controller
**/
PCH_SERIAL_IO_CONTROLLER
GetMaxI2cNumber (
  VOID
  );

/**
  Returns string with AcpiHID assigned to selected SerialIo controller

  @param[in] Number  Number of SerialIo controller

  @retval            pointer to 8-byte string
**/
CHAR8*
GetSerialIoAcpiHid (
  IN PCH_SERIAL_IO_CONTROLLER Number
  );

/**
  Checks if given Serial IO Controller Function equals 0

  @param[in] SerialIoNumber             Serial IO device

  @retval                               TRUE if SerialIO Function is equal to 0
                                        FALSE if Function is higher then 0
**/
BOOLEAN
IsSerialIoFunctionZero (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  );

/**
  Checks if Device with given PciDeviceId is one of SerialIo controllers
  If yes, its number is returned through Number parameter, otherwise Number is not updated

  @param[in]  PciDevId  Device ID
  @param[out] Number    Number of SerialIo controller

  @retval TRUE          Yes it is a SerialIo controller
  @retval FALSE         No it isn't a SerialIo controller
**/
BOOLEAN
IsSerialIoPciDevId (
  IN  UINT16                    PciDevId,
  OUT PCH_SERIAL_IO_CONTROLLER  *Number
  );

/**
  Checks if Device with given AcpiHID string is one of SerialIo controllers
  If yes, its number is returned through Number parameter, otherwise Number is not updated

  @param[in]  AcpiHid   String
  @param[out] Number    Number of SerialIo controller

  @retval TRUE          yes it is a SerialIo controller
  @retval FALSE         no it isn't a SerialIo controller
**/
BOOLEAN
IsSerialIoAcpiHid (
  IN CHAR8                      *AcpiHid,
  OUT PCH_SERIAL_IO_CONTROLLER  *Number
  );

/**
  Configures Serial IO Controller

  @param[in] Controller    Serial IO controller number
  @param[in] DeviceMode    Device operation mode
  @param[in] PsfDisable    Disable device at PSF level

  @retval None
**/
VOID
ConfigureSerialIoController (
  IN PCH_SERIAL_IO_CONTROLLER Controller,
  IN PCH_SERIAL_IO_MODE       DeviceMode,
  IN BOOLEAN                  PsfDisable
  );

/**
  Returns Serial IO Controller Type I2C, SPI or UART

  @param[in] Number  Number of SerialIo controller

  @retval            I2C, SPI or UART
  @retval            UNKNOWN - in case if undefined controller
**/
PCH_SERIAL_IO_CONTROLLER_TYPE
GetSerialIoControllerType (
  IN PCH_SERIAL_IO_CONTROLLER  Controller
  );

/**
  Finds PCI Device Number of SerialIo devices.
  SerialIo devices' BDF is configurable

  @param[in] SerialIoNumber             Serial IO device

  @retval                               SerialIo device number
**/
UINT8
GetSerialIoDeviceNumber (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  );

/**
  Finds PCI Function Number of SerialIo devices.
  SerialIo devices' BDF is configurable

  @param[in] SerialIoNumber             Serial IO device

  @retval                               SerialIo funciton number
**/
UINT8
GetSerialIoFunctionNumber (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  );

/**
  Finds BAR values of SerialIo devices.
  SerialIo devices can be configured to not appear on PCI so traditional method of reading BAR might not work.

  @param[in] SerialIoDevice             Serial IO device
  @param[in] BarNumber                  0=BAR0, 1=BAR1

  @retval                               SerialIo Bar value
**/
UINTN
FindSerialIoBar (
  IN PCH_SERIAL_IO_CONTROLLER           SerialIoDevice,
  IN UINT8                              BarNumber
  );

/**
  Checks if given device corresponds to any of LPSS Devices

  @param[in] DeviceNumber               device number
  @param[in] FunctionNumber             function number

  @retval                               TRUE if SerialIO Device/Function Number is equal to any of LPSS devices
                                        FALSE Device/Function is not in Serial IO scope
**/
BOOLEAN
IsSerialIoDevice (
  IN UINT8  DeviceNumber,
  IN UINT8  FunctionNumber
  );

/**
  Checks if given Serial IO Controller is enabled or not

  @param[in] DeviceNumber               device number
  @param[in] FunctionNumber             function number

  @retval TRUE                          TRUE if given serial io device is enabled.
  @retval FALSE                         FALSE if given serial io device is disabled.
**/
BOOLEAN
IsSerialIoDeviceEnabled (
  IN UINT8  DeviceNumber,
  IN UINT8  FunctionNumber
  );

/**
  Gets Pci Config control offset

  @param[in] DeviceNumber               device number
  @param[in] FunctionNumber             function number

  @retval    CfgCtrAddr                 Offset of Pci config control
                                        0 if Device and Function do not correspond to Serial IO
**/
UINT16
GetSerialIoConfigControlOffset (
  IN UINT8  DeviceNumber,
  IN UINT8  FunctionNumber
  );

#endif // _PEI_DXE_SMM_PCH_SERIAL_IO_LIB_H_
