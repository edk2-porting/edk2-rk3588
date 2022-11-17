/** @file
  Header file for Serial IO Private Lib implementation.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SERIAL_IO_PRIVATE_LIB_H_
#define _SERIAL_IO_PRIVATE_LIB_H_

#include <SerialIoDevices.h>
#include <Library/SerialIoAccessLib.h>

/**
  Serial Io Pci Device State structure.
  Used to preserve current information about the device when it is configured in Pci mode prior to Pch Initialization.
**/
typedef struct {
  UINT64 PciCfgBar0;       ///< Pci Config Space Base Address Register
  UINT8  PciCfgCommand;    ///< Pci Config Space Command Register
  UINT8  PciCfgPmeCtrlSts; ///< Pci Config Space Pme Control Status
  UINT8  PprReset;         ///< MMIO Proprietary Reset Register
} SERIAL_IO_PCI_DEVICE_STATE;

/**
  Checks if higher functions are enabled.
  Used for Function 0 Serial Io Device disabling

  @param[in] DeviceNum       Device Number

  @retval TRUE               At least one higher function device is enabled
          FALSE              Higher functions are disabled
**/
BOOLEAN
SerialIoHigherFunctionsEnabled (
  IN UINT8                    DeviceNum
  );

/**
  Places SerialIo device in D3

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoSetD3 (
  IN UINT64                    PciCfgBase
  );

/**
  Places SerialIo device in D0

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoSetD0 (
  IN UINT64                    PciCfgBase
  );

/**
  Allows Memory Access

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] Hidden           Mode that determines access type

**/
VOID
SerialIoEnableMse (
  IN UINT64                    PciCfgBase,
  IN BOOLEAN                   Hidden
  );

/**
  Disable SerialIo memory access

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoDisableMse (
  IN UINT64                    PciCfgBase
  );

/**
  Disable SerialIo memory encoding
  Designated for Pci modes

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] RemoveTempBar    Remove temporary mem base address or not

**/
VOID
SerialIoMmioDisable (
  IN UINT64                    PciCfgBase,
  IN BOOLEAN                   RemoveBar
  );

/**
  Gets Fixed Base Address used for BAR0

  @param[in] SpiNumber               Serial IO device SPI number

  @retval                            Config control offset
**/
UINT32
GetSerialIoSpiFixedMmioAddress (
  IN UINT8       SpiNumber
  );

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] SpiNumber               Serial IO device SPI number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoSpiFixedPciCfgAddress (
  IN UINT8       SpiNumber
  );

/**
  Gets Spi Device Id

  @param[in] SpiNumber               Serial IO device SPI number

  @retval                            Device Id
**/
UINT16
GetSerialIoSpiDeviceId (
  IN UINT8       SpiNumber
  );

/**
  Checks if SPI is Hidden, and it's Pci Config space available

  @param[in] SpiNumber       Selects Serial IO SPI device

  @retval   TRUE             SPI is in hidden mode
  @retval   FALSE            SPI is not in hidden mode
**/
BOOLEAN
IsSerialIoSpiHidden (
  IN UINT8               SpiNumber
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config

**/
VOID
SerialIoSpiBootHandler (
  IN UINT8                      SpiNumber,
  IN SERIAL_IO_SPI_CONFIG       *SpiDeviceConfig
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     SpiNumber         SPI Number
  @param[in]     SpiDeviceConfig   SerialIo SPI Config
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoSpiS3Handler (
  IN     UINT8                 SpiNumber,
  IN     SERIAL_IO_SPI_CONFIG  *SpiDeviceConfig,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  );

/**
  Gets Pci Config control offset

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT16
GetSerialIoUartConfigControlOffset (
  IN UINT8       UartNumber
  );

/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT32
GetSerialIoUartFixedMmioAddress (
  IN UINT8       UartNumber
  );

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoUartFixedPciCfgAddress (
  IN UINT8       UartNumber
  );

/**
  Returns UART S3 boot script PCI address

  @param[in] UartNumber         UART Number

  @retval    UART  S3 boot script PCI address
**/
UINT64
GetSerialIoUartS3PciBase (
  IN UINT8        UartNumber
  );

/**
  Returns SPI S3 boot script PCI address

  @param[in] UartNumber         UART Number

  @retval    SPI S3 boot script PCI address
**/
UINT64
GetSerialIoSpiS3PciBase (
  IN UINT8        SpiNumber
  );

/**
  Returns I2C S3 boot script PCI address

  @param[in] I2cNumber         I2C Number

  @retval    I2C  S3 boot script PCI address
**/
UINT64
GetSerialIoI2cS3PciBase (
  IN UINT8        I2cNumber
  );

/**
  Gets Uarts Device Id

  @param[in] UartNumbe               Serial IO device UART number

  @retval                            Device Id
**/
UINT16
GetSerialIoUartDeviceId (
  IN UINT8       UartNumber
  );

/**
  Checks if UART is Hidden, and it's Pci Config space available

  @param[in]      UartNumber     Selects Serial IO UART device

  @retval   TRUE             UART is in hidden mode
  @retval   FALSE            UART is not in hidden mode
**/
BOOLEAN
IsSerialIoUartHidden (
  IN UINT8               UartNumber
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config

**/
VOID
SerialIoUartBootHandler (
  IN UINT8                      UartNumber,
  IN SERIAL_IO_UART_CONFIG      *UartDeviceConfig
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     UartNumber         UART Number
  @param[in]     UartDeviceConfig   SerialIo UART Config
  @param[in/out] S3PciCfgBase       S3 Boot Script Pci Config Base
  @param[in/out] Command            Pci Command register data to save
  @param[in/out] Pme                Pci Pme Control register data to save

**/
VOID
SerialIoUartS3Handler (
  IN     UINT8                  UartNumber,
  IN     SERIAL_IO_UART_CONFIG  *UartDeviceConfig,
  IN OUT UINT64                 *S3PciCfgBase,
  IN OUT UINT32                 *Command,
  IN OUT UINT32                 *Pme
  );

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] I2cNumber               Serial IO device I2C number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoI2cFixedPciCfgAddress (
  IN UINT8       I2cNumber
  );

/**
  Gets I2C Device Id

  @param[in] I2cNumber               Serial IO device I2C number

  @retval                            Device Id
**/
UINT16
GetSerialIoI2cDeviceId (
  IN UINT8       I2cNumber
  );

/**
  Checks if I2C is Hidden, and it's Pci Config space available

  @param[in] 2cNumber        Selects Serial IO I2C device

  @retval   TRUE             I2C is in hidden mode
  @retval   FALSE            I2C is not in hidden mode
**/
BOOLEAN
IsSerialIoI2cHidden (
  IN UINT8               I2cNumber
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   SerialIo I2C Config

**/
VOID
SerialIoI2cBootHandler (
  IN UINT8                      I2cNumber,
  IN SERIAL_IO_I2C_CONFIG       *I2cDeviceConfig
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I2cNumber         I2C Number
  @param[in]     I2cDeviceConfig   SerialIo I2C Config
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoI2cS3Handler (
  IN     UINT8                 I2cNumber,
  IN     SERIAL_IO_I2C_CONFIG  *I2cDeviceConfig,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  );

#endif // _SERIAL_IO_PRIVATE_LIB_H_
