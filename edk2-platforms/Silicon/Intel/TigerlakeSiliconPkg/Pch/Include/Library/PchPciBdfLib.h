/** @file
  Header file for PchPciBdfLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PCI_BDF_LIB_H_
#define _PCH_PCI_BDF_LIB_H_


/**
  Get eSPI controller address that can be passed to the PCI Segment Library functions.

  @retval eSPI controller address in PCI Segment Library representation
**/
UINT64
EspiPciCfgBase (
  VOID
  );

/**
  Get GbE controller address that can be passed to the PCI Segment Library functions.

  @retval GbE controller address in PCI Segment Library representation
**/
UINT64
GbePciCfgBase (
  VOID
  );

/**
  Returns Gigabit Ethernet PCI Device Number

  @retval  GbE device number
**/
UINT8
GbeDevNumber (
  VOID
  );

/**
  Returns Gigabit Ethernet PCI Function Number

  @retval  GbE function number
**/
UINT8
GbeFuncNumber (
  VOID
  );

/**
  Get HDA controller address that can be passed to the PCI Segment Library functions.

  @retval HDA controller address in PCI Segment Library representation
**/
UINT64
HdaPciCfgBase (
  VOID
  );

/**
  Get HDA PCI device number

  @retval PCI dev number
**/
UINT8
HdaDevNumber (
  VOID
  );

/**
  Get HDA PCI function number

  @retval PCI fun number
**/
UINT8
HdaFuncNumber (
  VOID
  );


/**
  Get P2SB controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sbPciCfgBase (
  VOID
  );

/**
  Get P2SB PCI device number

  @retval PCI dev number
**/
UINT8
P2sbDevNumber (
  VOID
  );

/**
  Returns SPI PCI Config Space base address

  @retval  UINT64  SPI Config Space base address
**/
UINT64
SpiPciCfgBase (
  VOID
  );

/**
  Returns SPI Device number

  @retval UINT8   PCH SPI Device number
**/
UINT8
SpiDevNumber (
  VOID
  );

/**
  Returns SPI Function number

  @retval UINT8   PCH SPI Function number
**/
UINT8
SpiFuncNumber (
  VOID
  );

/**
  Get XHCI controller address that can be passed to the PCI Segment Library functions.

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PchXhciPciCfgBase (
  VOID
  );

/**
  Get XHCI controller PCIe Device Number

  @retval XHCI controller PCIe Device Number
**/
UINT8
PchXhciDevNumber (
  VOID
  );

/**
  Get XHCI controller PCIe Function Number

  @retval XHCI controller PCIe Function Number
**/
UINT8
PchXhciFuncNumber (
  VOID
  );

/**
  Get XDCI controller address that can be passed to the PCI Segment Library functions.

  @retval XDCI controller address in PCI Segment Library representation
**/
UINT64
PchXdciPciCfgBase (
  VOID
  );

/**
  Get XDCI controller PCIe Device Number

  @retval XDCI controller PCIe Device Number
**/
UINT8
PchXdciDevNumber (
  VOID
  );

/**
  Get XDCI controller PCIe Function Number

  @retval XDCI controller PCIe Function Number
**/
UINT8
PchXdciFuncNumber (
  VOID
  );

/**
  Get SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusPciCfgBase (
  VOID
  );

/**
  Return DMA Smbus Device Number

  @retval DMA Smbus Device Number
**/
UINT8
SmbusDmaDevNumber (
  VOID
  );

/**
  Return DMA Smbus Function Number

  @retval  DMA Smbus Function Number
**/
UINT8
SmbusDmaFuncNumber (
  VOID
  );

/**
  Get DMA SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval DMA SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusDmaPciCfgBase (
  VOID
  );

/**
  Return Smbus Device Number

  @retval Smbus Device Number
**/
UINT8
SmbusDevNumber (
  VOID
  );

/**
  Return Smbus Function Number

  @retval  Smbus Function Number
**/
UINT8
SmbusFuncNumber (
  VOID
  );

/**
  Gets SATA controller PCIe config space base address

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe config space base address
**/
UINT64
SataPciCfgBase (
  IN UINT32 SataCtrlIndex
  );

/**
  Gets SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
SataDevNumber (
  IN UINT32 SataCtrlIndex
  );

/**
  Gets SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
SataFuncNumber (
  IN UINT32 SataCtrlIndex
  );

/**
  Returns PCH LPC device PCI base address.

  @retval                   PCH LPC PCI base address.
**/
UINT64
LpcPciCfgBase (
  VOID
  );

/**
  Get LPC controller PCIe Device Number

  @retval LPC controller PCIe Device Number
**/
UINT8
LpcDevNumber (
  VOID
  );

/**
  Get Thermal Device PCIe Device Number

  @retval Thermal Device PCIe Device Number
**/
UINT8
ThermalDevNumber (
  VOID
  );

/**
  Get Thermal Device PCIe Function Number

  @retval Thermal Device PCIe Function Number
**/
UINT8
ThermalFuncNumber (
  VOID
  );

/**
  Returns Thermal Device PCI base address.

  @retval                   Thermal Device PCI base address.
**/
UINT64
ThermalPciCfgBase (
  VOID
  );

/**
  Get LPC controller PCIe Function Number

  @retval LPC controller PCIe Function Number
**/
UINT8
LpcFuncNumber (
  VOID
  );

/**
  Get Serial IO I2C controller PCIe Device Number

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller PCIe Device Number
**/
UINT8
SerialIoI2cDevNumber (
  IN UINT8       I2cNumber
  );

/**
  Get Serial IO I2C controller PCIe Function Number

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller PCIe Function Number
**/
UINT8
SerialIoI2cFuncNumber (
  IN UINT8       I2cNumber
  );

/**
  Get Serial IO I2C controller address that can be passed to the PCI Segment Library functions.

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller address in PCI Segment Library representation
**/
UINT64
SerialIoI2cPciCfgBase (
  IN UINT8        I2cNumber
  );

/**
  Get Serial IO SPI controller PCIe Device Number

  @param[in]  I2cNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller PCIe Device Number
**/
UINT8
SerialIoSpiDevNumber (
  IN UINT8       SpiNumber
  );

/**
  Get Serial IO SPI controller PCIe Function Number

  @param[in]  SpiNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller PCIe Function Number
**/
UINT8
SerialIoSpiFuncNumber (
  IN UINT8       SpiNumber
  );

/**
  Get Serial IO SPI controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SpiNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller address in PCI Segment Library representation
**/
UINT64
SerialIoSpiPciCfgBase (
  IN UINT8        SpiNumber
  );

/**
  Get Serial IO UART controller PCIe Device Number

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller PCIe Device Number
**/
UINT8
SerialIoUartDevNumber (
  IN UINT8       UartNumber
  );

/**
  Get Serial IO UART controller PCIe Function Number

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller PCIe Function Number
**/
UINT8
SerialIoUartFuncNumber (
  IN UINT8       UartNumber
  );

/**
  Get Serial IO UART controller address that can be passed to the PCI Segment Library functions.

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller address in PCI Segment Library representation
**/
UINT64
SerialIoUartPciCfgBase (
  IN UINT8        UartNumber
  );

/**
  Get PCH PCIe controller PCIe Device Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Device Number
**/
UINT8
PchPcieRpDevNumber (
  IN  UINTN   RpIndex
  );

/**
  Get PCH PCIe controller PCIe Function Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Function Number
**/
UINT8
PchPcieRpFuncNumber (
  IN  UINTN   RpIndex
  );

/**
  Get PCH PCIe controller address that can be passed to the PCI Segment Library functions.

  @param[in]  RpIndex       PCH PCIe Root Port physical number. (0-based)

  @retval PCH PCIe controller address in PCI Segment Library representation
**/
UINT64
PchPcieRpPciCfgBase (
  IN  UINT32   RpIndex
  );

/**
  Get HECI1 PCI device number

  @retval PCI dev number
**/
UINT8
PchHeci1DevNumber (
  VOID
  );

/**
  Get HECI1 PCI function number

  @retval PCI fun number
**/
UINT8
PchHeci1FuncNumber (
  VOID
  );

/**
  Get HECI1 controller address that can be passed to the PCI Segment Library functions.

  @retval HECI1 controller address in PCI Segment Library representation
**/
UINT64
PchHeci1PciCfgBase (
  VOID
  );

/**
  Get HECI3 PCI device number

  @retval PCI dev number
**/
UINT8
PchHeci3DevNumber (
  VOID
  );

/**
  Get HECI3 PCI function number

  @retval PCI fun number
**/
UINT8
PchHeci3FuncNumber (
  VOID
  );

/**
  Get HECI3 controller address that can be passed to the PCI Segment Library functions.

  @retval HECI3 controller address in PCI Segment Library representation
**/
UINT64
PchHeci3PciCfgBase (
  VOID
  );

#endif //_PCH_PCI_BDF_LIB_H_
