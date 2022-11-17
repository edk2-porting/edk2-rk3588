/** @file
  Build time limits of PCH resources.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_LIMITS_H_
#define _PCH_LIMITS_H_
/*
 * Defines povided in this file are indended to be used only where static value
 * is needed. They are set to values which allow to accomodate multiple projects
 * needs. Where runtime usage is possible please used dedicated functions from
 * PchInfoLib to retrieve accurate values
 */

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
// SerialIo limits
//
#define PCH_MAX_SERIALIO_I2C_CONTROLLERS      8
#define PCH_MAX_SERIALIO_SPI_CONTROLLERS      7
#define PCH_MAX_SERIALIO_SPI_CHIP_SELECTS     2
#define PCH_MAX_SERIALIO_UART_CONTROLLERS     7

//
// ISH limits
//
#define PCH_MAX_ISH_GP_PINS                   8
#define PCH_MAX_ISH_UART_CONTROLLERS          2
#define PCH_MAX_ISH_I2C_CONTROLLERS           3
#define PCH_MAX_ISH_SPI_CONTROLLERS           1
#define PCH_MAX_ISH_SPI_CS_PINS               1
//
// HDA limits
//
#define PCH_MAX_HDA_SDI                       2
#define PCH_MAX_HDA_SSP_LINK_NUM              6
#define PCH_MAX_HDA_SNDW_LINK_NUM             4

//
// Number of eSPI slaves
//
#define PCH_MAX_ESPI_SLAVES                  2

#endif // _PCH_LIMITS_H_

