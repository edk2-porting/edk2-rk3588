/** @file

  Copyright (c) 2021, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

 **/
#ifndef __RPI_DEBUG_PORT_2_H__
#define __RPI_DEBUG_PORT_2_H__

#include <IndustryStandard/DebugPort2Table.h>

#define RPI_DBG2_NUM_DEBUG_PORTS                        1
#define RPI_DBG2_NUMBER_OF_GENERIC_ADDRESS_REGISTERS    1
#define RPI_DBG2_NAMESPACESTRING_FIELD_SIZE             15

#pragma pack(1)


typedef struct {
  EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT         Dbg2Device;
  EFI_ACPI_6_3_GENERIC_ADDRESS_STRUCTURE                BaseAddressRegister;
  UINT32                                                AddressSize;
  UINT8                                                 NameSpaceString[RPI_DBG2_NAMESPACESTRING_FIELD_SIZE];
} DBG2_DEBUG_DEVICE_INFORMATION;

typedef struct {
  EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE               Description;
  DBG2_DEBUG_DEVICE_INFORMATION                         Dbg2DeviceInfo[RPI_DBG2_NUM_DEBUG_PORTS];
} DBG2_TABLE;

#pragma pack()
#endif  //__RPI_DEBUG_PORT_2_H__
