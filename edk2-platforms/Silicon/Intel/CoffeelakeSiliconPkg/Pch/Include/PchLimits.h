/** @file
  Build time limits of PCH resources.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_LIMITS_H_
#define _PCH_LIMITS_H_

//
// PCIe limits
//
#define PCH_MAX_PCIE_ROOT_PORTS             24
#define PCH_MAX_PCIE_CONTROLLERS            6

//
// PCIe clocks limits
//
#define PCH_MAX_PCIE_CLOCKS                 16
//
// RST PCIe Storage Cycle Router limits
//
#define PCH_MAX_RST_PCIE_STORAGE_CR         3

//
// SATA limits
//
#define PCH_MAX_SATA_CONTROLLERS            3
#define PCH_MAX_SATA_PORTS                  8

//
// USB limits
//
#define PCH_MAX_USB2_PORTS                  16
#define PCH_MAX_USB3_PORTS                  10

//
// SerialIo limits
//
#define PCH_MAX_SERIALIO_CONTROLLERS         12
#define PCH_MAX_SERIALIO_I2C_CONTROLLERS      6
#define PCH_MAX_SERIALIO_SPI_CONTROLLERS      3
#define PCH_MAX_SERIALIO_UART_CONTROLLERS     3

//
// Number of eSPI slaves
//
#define PCH_MAX_ESPI_SLAVES                  2

#endif // _PCH_LIMITS_H_

