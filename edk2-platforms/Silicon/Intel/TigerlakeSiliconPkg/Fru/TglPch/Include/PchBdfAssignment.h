/** @file
  Header file for TigerLake PCH devices PCI Bus Device Function map.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_BDF_ASSIGNMENT_H_
#define _PCH_BDF_ASSIGNMENT_H_

#define NOT_PRESENT                     0xFF

#define MAX_SATA_CONTROLLER             1

//
// PCH PCIe Controllers
//
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9          29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_17         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_18         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_19         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_20         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_21         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_22         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_23         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_24         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_25         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_26         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_27         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_28         NOT_PRESENT

//
// USB3 (XHCI) Controller PCI config
//
#define PCI_DEVICE_NUMBER_PCH_XHCI                    20
#define PCI_FUNCTION_NUMBER_PCH_XHCI                  0

//
// xDCI (OTG) USB Device Controller
//
#define PCI_DEVICE_NUMBER_PCH_XDCI                    20
#define PCI_FUNCTION_NUMBER_PCH_XDCI                  1

//
//  Thermal Device
//
#define PCI_DEVICE_NUMBER_PCH_THERMAL                 NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_THERMAL               NOT_PRESENT

//
// CSME HECI #1
//
#define PCI_DEVICE_NUMBER_PCH_HECI1                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI1                 0

//
// CSME HECI #2
//
#define PCI_DEVICE_NUMBER_PCH_HECI2                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI2                 1

//
// CSME IDE-Redirection (IDE-R)
//
#define PCI_DEVICE_NUMBER_PCH_IDER                    22
#define PCI_FUNCTION_NUMBER_PCH_IDER                  2

//
// CSME Keyboard and Text (KT) Redirection
//
#define PCI_DEVICE_NUMBER_PCH_KTR                     22
#define PCI_FUNCTION_NUMBER_PCH_KTR                   3

//
// CSME HECI #3
//
#define PCI_DEVICE_NUMBER_PCH_HECI3                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI3                 4

//
// CSME HECI #4
//
#define PCI_DEVICE_NUMBER_PCH_HECI4                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI4                 5

//
// CSME MROM
//
#define PCI_DEVICE_NUMBER_PCH_MROM                    NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_MROM                  NOT_PRESENT

//
// CSME WLAN
//
#define PCI_DEVICE_NUMBER_PCH_WLAN                    22
#define PCI_FUNCTION_NUMBER_PCH_WLAN                  7

//
// SATA Controllers
//
#define PCI_DEVICE_NUMBER_PCH_SATA_1                  23
#define PCI_FUNCTION_NUMBER_PCH_SATA_1                0
#define PCI_DEVICE_NUMBER_PCH_SATA_2                  NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SATA_2                NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_SATA_3                  NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SATA_3                NOT_PRESENT

//
//  PCH LP Serial IO I2C #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0          21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0        0

//
//  PCH LP Serial IO I2C #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1          21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1        1

//
//  PCH LP Serial IO I2C #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2          21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2        2

//
//  PCH LP Serial IO I2C #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3          21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3        3

//
//  PCH LP Serial IO I2C #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4          25
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4        0

//
//  PCH LP Serial IO I2C #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5          25
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5        1

//
//  PCH LP Serial IO I2C #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C6          16
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C6        0

//
//  PCH LP Serial IO I2C #7 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C7          16
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C7        1

//
//  PCH LP Serial IO SPI #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0          30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0        2

//
//  PCH LP Serial IO SPI #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1          30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1        3

//
//  PCH LP Serial IO SPI #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI2          18
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI2        6

//
//  PCH LP Serial IO SPI #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI3          19
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI3        0

//
//  PCH LP Serial IO SPI #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI4          19
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI4        1

//
//  PCH LP Serial IO SPI #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI5          19
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI5        2

//
//  PCH LP Serial IO SPI #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI6          19
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI6        3

//
//  PCH LP Serial IO UART #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0         30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0       0

//
//  PCH LP Serial IO UART #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1         30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1       1

//
//  PCH LP Serial IO UART #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2         25
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2       2

//
//  PCH LP Serial IO UART #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART3         17
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART3       0

//
//  PCH LP Serial IO UART #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART4         17
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART4       1

//
//  PCH LP Serial IO UART #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART5         17
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART5       2

//
//  PCH LP Serial IO UART #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART6         17
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART6       3

//
// DMA-SMBus Controller
//
#define PCI_DEVICE_NUMBER_PCH_DMA_SMBUS               30
#define PCI_FUNCTION_NUMBER_PCH_DMA_SMBUS             4

//
// TSN GbE Controller #1
//
#define PCI_DEVICE_NUMBER_PCH_TSN0                    30
#define PCI_FUNCTION_NUMBER_PCH_TSN0                  4

//
// TSN GbE Controller #2
//
#define PCI_DEVICE_NUMBER_PCH_TSN1                    30
#define PCI_FUNCTION_NUMBER_PCH_TSN1                  5

//
// LPC Controller
//
#define PCI_DEVICE_NUMBER_PCH_LPC                     31
#define PCI_FUNCTION_NUMBER_PCH_LPC                   0

//
// eSPI Controller
//
#define PCI_DEVICE_NUMBER_PCH_ESPI                    31
#define PCI_FUNCTION_NUMBER_PCH_ESPI                  0

//
// Primary to Sideband (P2SB) Bridge
//
#define PCI_DEVICE_NUMBER_PCH_P2SB                    31
#define PCI_FUNCTION_NUMBER_PCH_P2SB                  1

//
// PMC (D31:F2)
//
#define PCI_DEVICE_NUMBER_PCH_PMC                     31
#define PCI_FUNCTION_NUMBER_PCH_PMC                   2

//
// PMC SSRAM Registers
//
#define PCI_DEVICE_NUMBER_PCH_PMC_SSRAM               20
#define PCI_FUNCTION_NUMBER_PCH_PMC_SSRAM             2

//
// HD-A Controller
//
#define PCI_DEVICE_NUMBER_PCH_HDA                     31
#define PCI_FUNCTION_NUMBER_PCH_HDA                   3

//
// SMBus Controller
//
#define PCI_DEVICE_NUMBER_PCH_SMBUS                   31
#define PCI_FUNCTION_NUMBER_PCH_SMBUS                 4

//
// SPI Controller (D31:F5)
//
#define PCI_DEVICE_NUMBER_PCH_SPI                     31
#define PCI_FUNCTION_NUMBER_PCH_SPI                   5

//
// Gigabit Ethernet LAN Controller (D31:F6)
//
#define PCI_DEVICE_NUMBER_GBE                         31
#define PCI_FUNCTION_NUMBER_GBE                       6

#endif // _PCH_BDF_ASSIGNMENT_H_
