/** @file
  Serial IO policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SERIAL_IO_CONFIG_H_
#define _SERIAL_IO_CONFIG_H_

#define SERIAL_IO_CONFIG_REVISION 2
extern EFI_GUID gSerialIoConfigGuid;

#pragma pack (push,1)

/**
  The PCH_SERIAL_IO_CONFIG block provides the configurations to set the Serial IO controllers

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add I2cPadsTermination
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                             ///< Config Block Header
  /**
       0: Disabled;
          - Device is placed in D3
          - Gpio configuration is skipped
          - Device will be disabled in PSF
          - !important! If given device is Function 0 and not all other LPSS functions on given device
                        are disabled, then PSF disabling is skipped.
                        PSF default will remain and device PCI CFG Space will still be visible.
                        This is needed to allow PCI enumerator access functions above 0 in a multifunction device.
    <b>1: Pci</b>;
          - Gpio pin configuration in native mode for each assigned pin
            RX/TX or RX/TX/CTS/RTS in case of UART depending UartHwFlowCtrl
          - Device will be enabled in PSF
          - Only Bar 0 will be enabled
       2: Acpi;
          - Gpio pin configuration in native mode for each assigned pin
            RX/TX or RX/TX/CTS/RTS in case of UART depending UartHwFlowCtrl
          - Device will be hidden in PSF and not available to PCI enumerator
          - Both BARs are enabled, BAR1 becomes devices Pci config Space
    @note Intel does not provide Windows SerialIo drivers for this mode
       3: Hidden;
          Designated for Kernel Debug and Legacy UART configuartion, might also be used for IO Expander on I2C
          - Device is placed in D0
          - Gpio pin configuration in native mode for each assigned pin
            RX/TX or RX/TX/CTS/RTS in case of UART depending UartHwFlowCtrl
          - Device will be hidden in PSF and not available to PCI enumerator
          - Both BARs are enabled, BAR1 becomes devices Pci config Space
          - !important! In this mode UART will work in 16550 Legacy 8BIT Mode, it's resources will be assigned to mother board through ACPI (PNP0C02)
    @note Considering the PcdSerialIoUartDebugEnable and PcdSerialIoUartNumber for all SerialIo UARTx,
          the PCD is more meaningful to represent the board design. It means, if PcdSerialIoUartDebugEnable is not 0,
          the board is designed to use the SerialIo UART for debug message and the PcdSerialIoUartNumber is dedicated
          to be Debug UART usage. Therefore, it should grayout the option from setup menu since no other options
          available for this UART controller on this board, and also override the policy default accordingly.
          While PcdSerialIoUartDebugEnable is 0, then it's allowed to configure the UART controller by policy.
  **/
  UINT8  DevMode[PCH_MAX_SERIALIO_CONTROLLERS];
  UINT8  SpiCsPolarity[PCH_MAX_SERIALIO_SPI_CONTROLLERS];   ///< Selects SPI ChipSelect signal polarity, <b>0=active low</b>.
  UINT8  UartHwFlowCtrl[PCH_MAX_SERIALIO_UART_CONTROLLERS]; ///< Enables UART hardware flow control, CTS and RTS lines, <b>0:disabled</b>, 1:enabled
  /**
    I2C Pads Internal Termination.
    For more information please see Platform Design Guide.
    Supported values (check GPIO_ELECTRICAL_CONFIG for reference):
    <b>GpioTermNone: No termination</b>,
    GpioTermWpu1K: 1kOhm weak pull-up,
    GpioTermWpu5K: 5kOhm weak pull-up,
    GpioTermWpu20K: 20kOhm weak pull-up
  **/
  UINT8  I2cPadsTermination[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
  /**
    UART device for debug purpose. 0:UART0, 1: UART1, <b>2:UART2</b>
    @note If CNVi solution is on the platform and UART0 is selected as BT Core interface,
          UART0 cannot be used for debug purpose.
  **/
  UINT32 DebugUartNumber           :  2;
  UINT32 EnableDebugUartAfterPost  :  1;                    ///< Enable debug UART controller after post. 0: diabled, <b>1: enabled</b>
  /**
    <b>0: default pins</b>; 1: pins muxed with CNV_BRI/RGI
    UART0 can be configured to use two different sets of pins:
    This setting gives flexibility to use UART0 functionality on other pins when
    default ones are used for a different purpose.
    @note Since the second pin set contains pads which are also used for CNVi purpose, setting Uart0PinMuxing
    is exclusive with CNVi being enabled.
  **/
  UINT32 Uart0PinMuxing            :  1;
  UINT32 RsvdBits0                 : 28;
} PCH_SERIAL_IO_CONFIG;

#pragma pack (pop)

#endif // _SERIAL_IO_CONFIG_H_
