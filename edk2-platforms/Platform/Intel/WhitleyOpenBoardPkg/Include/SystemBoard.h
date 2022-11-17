/** @file
  This protocol is EFI compatible.

  @copyright
  Copyright 2005 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_SYSTEM_BOARD_H_
#define _DXE_SYSTEM_BOARD_H_

#include <PlatPirqData.h>
#include <PlatDevData.h>
#include <Ppi/PchPolicy.h>

#define PCI_DEVICE_NUMBER_IMC0_CH_0     0x08
#define PCI_FUNCTION_NUMBER_IMC0_CH_0   0
#define PCI_DEVICE_ID_IMC0_CH_0         0x2014
#define BIOSGUARD_SUPPORT_ENABLED BIT0
#define OC_SUPPORT_ENABLED   BIT1

#ifndef __AML_OFFSET_TABLE_H
#define __AML_OFFSET_TABLE_H

typedef struct {
    char                   *Pathname;      /* Full pathname (from root) to the object */
    unsigned short         ParentOpcode;   /* AML opcode for the parent object */
    unsigned long          NamesegOffset;  /* Offset of last nameseg in the parent namepath */
    unsigned char          Opcode;         /* AML opcode for the data */
    unsigned long          Offset;         /* Offset for the data */
    unsigned long long     Value;          /* Original value of the data (as applicable) */
} AML_OFFSET_TABLE_ENTRY;
#endif

//
// Global variables for Option ROMs
//
#define NULL_ROM_FILE_GUID \
  { \
    0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } \
  }

typedef struct {
  EFI_GUID  FileName;
  UINTN     Segment;
  UINTN     Bus;
  UINTN     Device;
  UINTN     Function;
  UINT16    VendorId;
  UINT16    DeviceId;
} PCI_OPTION_ROM_TABLE;

//
// System board information table
//
typedef struct {
  //
  // Pci option ROM data
  //
  PCI_OPTION_ROM_TABLE          *PciOptionRomTable;

  //
  // System CPU data
  //
  UINT32                        CpuSocketCount;

  //
  // System device and irq routing data
  //
  DEVICE_DATA                   *DeviceData;
  PLATFORM_PIRQ_DATA            *SystemPirqData;
} DXE_SYSTEM_BOARD_INFO;

#endif
