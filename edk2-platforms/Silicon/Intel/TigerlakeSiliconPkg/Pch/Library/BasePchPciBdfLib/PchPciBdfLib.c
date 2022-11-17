/** @file
  PCH PCIe Bus Device Function Library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Register/PchRegs.h>
#include <PchBdfAssignment.h>

/**
  Check if a Device is present for PCH FRU
  If the data is defined for PCH RFU return it
  If the data is not defined (Device is NOT present) assert.

  @param[in]  DataToCheck       Device or Function number to check

  @retval Device or Function number value if defined for PCH FRU
          0xFF if not present in PCH FRU
**/
UINT8
CheckAndReturn (
  UINT8 DataToCheck
  )
{
  if (DataToCheck == NOT_PRESENT) {
    ASSERT (FALSE);
  }
  return DataToCheck;
}

/**
  Get eSPI controller address that can be passed to the PCI Segment Library functions.

  @retval eSPI controller address in PCI Segment Library representation
**/
UINT64
EspiPciCfgBase (
  VOID
  )
{
  ASSERT (PCI_DEVICE_NUMBER_PCH_ESPI != NOT_PRESENT);

  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_ESPI,
           PCI_FUNCTION_NUMBER_PCH_ESPI,
           0
           );
}

/**
  Returns Gigabit Ethernet PCI Device Number

  @retval  GbE device number
**/
UINT8
GbeDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_GBE);
}

/**
  Returns Gigabit Ethernet PCI Function Number

  @retval  GbE function number
**/
UINT8
GbeFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_GBE);
}

/**
  Get GbE controller address that can be passed to the PCI Segment Library functions.

  @retval GbE controller address in PCI Segment Library representation
**/
UINT64
GbePciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            GbeDevNumber (),
            GbeFuncNumber (),
            0
            );
}

/**
  Get HDA PCI device number

  @retval PCI dev number
**/
UINT8
HdaDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_HDA);
}

/**
  Get HDA PCI function number

  @retval PCI fun number
**/
UINT8
HdaFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_HDA);
}

/**
  Get HDA controller address that can be passed to the PCI Segment Library functions.

  @retval HDA controller address in PCI Segment Library representation
**/
UINT64
HdaPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            HdaDevNumber (),
            HdaFuncNumber (),
            0
            );
}

/**
  Get P2SB PCI device number

  @retval PCI dev number
**/
UINT8
P2sbDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_P2SB);
}

/**
  Get P2SB PCI function number

  @retval PCI fun number
**/
UINT8
P2sbFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_P2SB);
}

/**
  Get P2SB controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sbPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           P2sbDevNumber (),
           P2sbFuncNumber (),
           0
           );
}

/**
  Returns PCH SPI Device number

  @retval UINT8   PCH SPI Device number
**/
UINT8
SpiDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SPI);
}

/**
  Returns PCH SPI Function number

  @retval UINT8   PCH SPI Function number
**/
UINT8
SpiFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SPI);
}

/**
  Returns PCH SPI PCI Config Space base address

  @retval  UINT64  PCH SPI Config Space base address
**/
UINT64
SpiPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SpiDevNumber (),
           SpiFuncNumber (),
           0
           );
}

/**
  Get XHCI controller PCIe Device Number

  @retval XHCI controller PCIe Device Number
**/
UINT8
PchXhciDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_XHCI);
}

/**
  Get XHCI controller PCIe Function Number

  @retval XHCI controller PCIe Function Number
**/
UINT8
PchXhciFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_XHCI);
}

/**
  Get XHCI controller address that can be passed to the PCI Segment Library functions.

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PchXhciPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchXhciDevNumber (),
          PchXhciFuncNumber (),
          0
          );
}

/**
  Get XDCI controller PCIe Device Number

  @retval XDCI controller PCIe Device Number
**/
UINT8
PchXdciDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_XDCI);
}

/**
  Get XDCI controller PCIe Function Number

  @retval XDCI controller PCIe Function Number
**/
UINT8
PchXdciFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_XDCI);
}

/**
  Get XDCI controller address that can be passed to the PCI Segment Library functions.

  @retval XDCI controller address in PCI Segment Library representation
**/
UINT64
PchXdciPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchXdciDevNumber (),
          PchXdciFuncNumber (),
          0
          );
}

/**
  Return Smbus Device Number

  @retval Smbus Device Number
**/
UINT8
SmbusDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SMBUS);
}

/**
  Return Smbus Function Number

  @retval  Smbus Function Number
**/
UINT8
SmbusFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SMBUS);
}

/**
  Get SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            SmbusDevNumber (),
            SmbusFuncNumber (),
            0
            );
}

/**
  Return DMA Smbus Device Number

  @retval DMA Smbus Device Number
**/
UINT8
SmbusDmaDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_DMA_SMBUS);
}

/**
  Return DMA Smbus Function Number

  @retval  DMA Smbus Function Number
**/
UINT8
SmbusDmaFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_DMA_SMBUS);
}

/**
  Get DMA SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval DMA SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusDmaPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            SmbusDmaDevNumber (),
            SmbusDmaFuncNumber (),
            0
            );
}

/**
  Get SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
SataDevNumber (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MAX_SATA_CONTROLLER);

  if (SataCtrlIndex == 0) {
    return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SATA_1);
  } else if (SataCtrlIndex == 1) {
    return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SATA_2);
  } else if (SataCtrlIndex == 2) {
    return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SATA_3);
  } else {
    ASSERT (FALSE);
    return 0xFF;
  }
}

/**
  Get SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
SataFuncNumber (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MAX_SATA_CONTROLLER);

  if (SataCtrlIndex == 0) {
    return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SATA_1);
  } else if (SataCtrlIndex == 1) {
    return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SATA_2);
  } else if (SataCtrlIndex == 2) {
    return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SATA_3);
  } else {
    ASSERT (FALSE);
    return 0xFF;
  }
}

/**
  Get SATA controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller address in PCI Segment Library representation
**/
UINT64
SataPciCfgBase (
  IN UINT32  SataCtrlIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SataDevNumber (SataCtrlIndex),
           SataFuncNumber (SataCtrlIndex),
           0
           );
}

/**
  Get LPC controller PCIe Device Number

  @retval LPC controller PCIe Device Number
**/
UINT8
LpcDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_LPC);
}

/**
  Get LPC controller PCIe Function Number

  @retval LPC controller PCIe Function Number
**/
UINT8
LpcFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_LPC);
}

/**
  Returns PCH LPC device PCI base address.

  @retval                   PCH LPC PCI base address.
**/
UINT64
LpcPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpcDevNumber (),
           LpcFuncNumber (),
           0
           );
}

/**
  Get Thermal Device PCIe Device Number

  @retval Thermal Device PCIe Device Number
**/
UINT8
ThermalDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_THERMAL);
}

/**
  Get Thermal Device PCIe Function Number

  @retval Thermal Device PCIe Function Number
**/
UINT8
ThermalFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_THERMAL);
}

/**
  Returns Thermal Device PCI base address.

  @retval                   Thermal Device PCI base address.
**/
UINT64
ThermalPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           ThermalDevNumber (),
           ThermalFuncNumber (),
           0
           );
}

/**
  Get Serial IO I2C controller PCIe Device Number

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller PCIe Device Number
**/
UINT8
SerialIoI2cDevNumber (
  IN UINT8       I2cNumber
  )
{
  if (GetPchMaxSerialIoI2cControllersNum () <= I2cNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (I2cNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C6);
    case 7:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C7);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO I2C controller PCIe Function Number

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller PCIe Function Number
**/
UINT8
SerialIoI2cFuncNumber (
  IN UINT8       I2cNumber
  )
{
  if (GetPchMaxSerialIoI2cControllersNum () <= I2cNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (I2cNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3);
    case 4:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4);
    case 5:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5);
    case 6:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C6);
    case 7:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C7);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO I2C controller address that can be passed to the PCI Segment Library functions.

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller address in PCI Segment Library representation
**/
UINT64
SerialIoI2cPciCfgBase (
  IN UINT8        I2cNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SerialIoI2cDevNumber (I2cNumber),
           SerialIoI2cFuncNumber (I2cNumber),
           0
           );
}

/**
  Get Serial IO SPI controller PCIe Device Number

  @param[in]  I2cNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller PCIe Device Number
**/
UINT8
SerialIoSpiDevNumber (
  IN UINT8       SpiNumber
  )
{
  if (GetPchMaxSerialIoSpiControllersNum () <= SpiNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }

  switch (SpiNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI2);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI3);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI4);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI5);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO SPI controller PCIe Function Number

  @param[in]  SpiNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller PCIe Function Number
**/
UINT8
SerialIoSpiFuncNumber (
  IN UINT8       SpiNumber
  )
{
  if (GetPchMaxSerialIoSpiControllersNum () <= SpiNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }

  switch (SpiNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI2);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI3);
    case 4:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI4);
    case 5:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI5);
    case 6:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO SPI controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SpiNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller address in PCI Segment Library representation
**/
UINT64
SerialIoSpiPciCfgBase (
  IN UINT8        SpiNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SerialIoSpiDevNumber (SpiNumber),
           SerialIoSpiFuncNumber (SpiNumber),
           0
           );
}

/**
  Get Serial IO UART controller PCIe Device Number

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller PCIe Device Number
**/
UINT8
SerialIoUartDevNumber (
  IN UINT8       UartNumber
  )
{
  if (GetPchMaxSerialIoUartControllersNum () <= UartNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (UartNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART3);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART4);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART5);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO UART controller PCIe Function Number

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller PCIe Function Number
**/
UINT8
SerialIoUartFuncNumber (
  IN UINT8       UartNumber
  )
{
  if (GetPchMaxSerialIoUartControllersNum () <= UartNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (UartNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART3);
    case 4:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART4);
    case 5:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART5);
    case 6:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO UART controller address that can be passed to the PCI Segment Library functions.

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller address in PCI Segment Library representation
**/
UINT64
SerialIoUartPciCfgBase (
  IN UINT8        UartNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SerialIoUartDevNumber (UartNumber),
           SerialIoUartFuncNumber (UartNumber),
           0
           );
}

/**
  Get PCH PCIe controller PCIe Device Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Device Number
**/
UINT8
PchPcieRpDevNumber (
  IN  UINTN   RpIndex
  )
{
  if (RpIndex >= GetPchMaxPciePortNum ()) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (RpIndex) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7);
    case 7:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8);
    case 8:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9);
    case 9:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10);
    case 10:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11);
    case 11:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12);
    case 12:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13);
    case 13:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14);
    case 14:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15);
    case 15:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16);
    case 16:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_17);
    case 17:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_18);
    case 18:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_19);
    case 19:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_20);
    case 20:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_21);
    case 21:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_22);
    case 22:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_23);
    case 23:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_24);
    case 24:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_25);
    case 25:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_26);
    case 26:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_27);
    case 27:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_28);

    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get PCH PCIe controller PCIe Function Number
  Note:
  For Client PCH generations Function Number can be various
  depending on "Root Port Function Swapping". For such cases
  Function Number  MUST be obtain from proper register.
  For Server PCHs we have no "Root Port Function Swapping"
  and we can return fixed Function Number.
  To address this difference in this, PCH generation independent,
  library we should call specific function in PchPcieRpLib.

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Function Number
**/
UINT8
PchPcieRpFuncNumber (
  IN  UINTN   RpIndex
  )
{
  UINTN   Device;
  UINTN   Function;

  GetPchPcieRpDevFun (RpIndex, &Device, &Function);

  return (UINT8)Function;
}

/**
  Get PCH PCIe controller address that can be passed to the PCI Segment Library functions.

  @param[in]  RpIndex       PCH PCIe Root Port physical number. (0-based)

  @retval PCH PCIe controller address in PCI Segment Library representation
**/
UINT64
PchPcieRpPciCfgBase (
  IN  UINT32   RpIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchPcieRpDevNumber (RpIndex),
          PchPcieRpFuncNumber (RpIndex),
          0
          );
}

/**
  Get HECI1 PCI device number

  @retval PCI dev number
**/
UINT8
PchHeci1DevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_HECI1);
}

/**
  Get HECI1 PCI function number

  @retval PCI fun number
**/
UINT8
PchHeci1FuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_HECI1);
}

/**
  Get HECI1 controller address that can be passed to the PCI Segment Library functions.

  @retval HECI1 controller address in PCI Segment Library representation
**/
UINT64
PchHeci1PciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PchHeci1DevNumber (),
            PchHeci1FuncNumber (),
            0
            );
}

/**
  Get HECI3 PCI device number

  @retval PCI dev number
**/
UINT8
PchHeci3DevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_HECI3);
}

/**
  Get HECI3 PCI function number

  @retval PCI fun number
**/
UINT8
PchHeci3FuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_HECI3);
}

/**
  Get HECI3 controller address that can be passed to the PCI Segment Library functions.

  @retval HECI3 controller address in PCI Segment Library representation
**/
UINT64
PchHeci3PciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PchHeci3DevNumber (),
            PchHeci3FuncNumber (),
            0
            );
}
