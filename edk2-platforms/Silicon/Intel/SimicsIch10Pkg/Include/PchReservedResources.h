/** @file
  PCH preserved MMIO resource definitions.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PRESERVED_RESOURCES_H_
#define _PCH_PRESERVED_RESOURCES_H_

/**
  PCH preserved MMIO range, 24 MB, from 0xFD000000 to 0xFE7FFFFF

  Detailed recommended static allocation
  +-------------------------------------------------------------------------+
  | Size        | Start       | End         | Usage                         |
  | 16 MB       | 0xFD000000  | 0xFDFFFFFF  | SBREG                         |
  | 64 KB       | 0xFE000000  | 0xFE00FFFF  | PMC MBAR                      |
  | 4 KB        | 0xFE010000  | 0xFE010FFF  | SPI BAR0                      |
  | 88 KB       | 0xFE020000  | 0xFE035FFF  | SerialIo BAR in ACPI mode     |
  | 24 KB       | 0xFE036000  | 0xFE03BFFF  | Unused                        |
  | 4 KB        | 0xFE03C000  | 0xFE03CFFF  | Thermal Device in ACPI mode   |
  | 524 KB      | 0xFE03D000  | 0xFE0BFFFF  | Unused                        |
  | 256 KB      | 0xFE0C0000  | 0xFE0FFFFF  | TraceHub FW BAR               |
  | 1 MB        | 0xFE100000  | 0xFE1FFFFF  | TraceHub MTB BAR              |
  | 2 MB        | 0xFE200000  | 0xFE3FFFFF  | TraceHub SW BAR               |
  | 64 KB       | 0xFE400000  | 0xFE40FFFF  | CIO2 MMIO BAR in ACPI mode    |
  | 2 MB - 64KB | 0xFE410000  | 0xFE5FFFFF  | Unused                        |
  | 2 MB        | 0xFE600000  | 0xFE7FFFFF  | Temp address                  |
  +-------------------------------------------------------------------------+
**/
#define PCH_PRESERVED_BASE_ADDRESS      0xFD000000     ///< Pch preserved MMIO base address
#define PCH_PRESERVED_MMIO_SIZE         0x01800000     ///< 24MB
#define PCH_PCR_BASE_ADDRESS            0xFD000000     ///< SBREG MMIO base address
#define PCH_PCR_MMIO_SIZE               0x01000000     ///< 16MB
#define PCH_PWRM_BASE_ADDRESS           0xFE000000     ///< PMC MBAR MMIO base address
#define PCH_PWRM_MMIO_SIZE              0x00010000     ///< 64KB
#define PCH_SPI_BASE_ADDRESS            0xFED1C000 + 0x3800     ///< SPI MBAR MMIO base address
#define PCH_SPI_MMIO_SIZE               0x00001000     ///< 4KB
#define PCH_SERIAL_IO_BASE_ADDRESS      0xFE020000     ///< SerialIo MMIO base address
#define PCH_SERIAL_IO_MMIO_SIZE         0x00016000     ///< 88KB
#define PCH_THERMAL_BASE_ADDRESS        0xFE03C000     ///< Thermal Device in ACPI mode
#define PCH_THERMAL_MMIO_SIZE           0x00001000     ///< 4KB
#define PCH_TRACE_HUB_FW_BASE_ADDRESS   0xFE0C0000     ///< TraceHub FW MMIO base address
#define PCH_TRACE_HUB_FW_MMIO_SIZE      0x00040000     ///< 256KB
#define PCH_TRACE_HUB_MTB_BASE_ADDRESS  0xFE100000     ///< TraceHub MTB MMIO base address
#define PCH_TRACE_HUB_MTB_MMIO_SIZE     0x00100000     ///< 1MB
#define PCH_TRACE_HUB_SW_BASE_ADDRESS   0xFE200000     ///< TraceHub SW MMIO base address
#define PCH_TRACE_HUB_SW_MMIO_SIZE      0x00200000     ///< 2MB
#define PCH_CIO2_BASE_ADDRESS           0xFE400000     ///< CIO2 MMIO BAR in ACPI mode
#define PCH_CIO2_MMIO_SIZE              0x00010000     ///< 64KB
#define PCH_TEMP_BASE_ADDRESS           0xFE600000     ///< preserved temp address for misc usage
#define PCH_TEMP_MMIO_SIZE              0x00200000     ///< 2MB

#define RCRB                            0xFED1C000
#define SPIBAR                            0x3800

#endif // _PCH_PRESERVED_RESOURCES_H_

