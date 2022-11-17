/** @file
  Header file for the Ps2 keyboard controller library.

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PS2_KBC_LIB_H_
#define _PS2_KBC_LIB_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootManagerLib.h>

//
// Below is the platform console device path
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      Keyboard;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_KEYBOARD_DEVICE_PATH;

typedef struct {
  UINT8 Segment;
  UINT8 Bus;
  UINT8 Device;
  UINT8 Funtion;
} SIO_PCI_ISA_BRIDGE_DEVICE_INFO;

#define gPciRootBridge \
  { \
    { \
      ACPI_DEVICE_PATH, \
      ACPI_DP, \
      { \
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), \
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8) \
      }, \
    }, \
    EISA_PNP_ID (0x0A03), \
    0 \
  }

#define gEndEntire \
  { \
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, { END_DEVICE_PATH_LENGTH, 0 } \
  }

#define KBC_INPBUF_VIA60_KBECHO             0xEE
#define KEYBOARD_8042_DATA_REGISTER         0x60
#define KEYBOARD_8042_STATUS_REGISTER       0x64

#define PS2_KEYBOARD_TIMEOUT                65536   // 0.07s
#define PS2_KEYBOARD_WAITFORVALUE_TIMEOUT   1000000 // 1s
#define PS2_KEYBOARD_KBEN                   0xF4
#define PS2_KEYBOARD_CMDECHO_ACK            0xFA

#endif
