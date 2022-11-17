/** @file
  GUID used for Platform Info Data entries in the HOB list.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_INFO_H_
#define _PLATFORM_INFO_H_

#pragma pack(1)

///
/// PCH_GPIO_PAD is equivalent to GPIO_PAD which is defined in GpioConfig.h
///
typedef UINT32 PCH_GPIO_PAD; //Copied from GpioConfig.h (need to change it based on include)

typedef struct {
UINT8    Expander;
UINT8    Pin;
UINT16   Reserved; // Reserved for future use
} IO_EXPANDER_PAD;

typedef union {
PCH_GPIO_PAD       PchGpio;
IO_EXPANDER_PAD    IoExpGpio;
} GPIO_PAD_CONFIG;

typedef struct {
UINT8                GpioType;    // 0: Disabled (no GPIO support), 1: PCH, 2: I/O Expander
UINT8                Reserved[3]; // Reserved for future use
GPIO_PAD_CONFIG      GpioData;
} PACKED_GPIO_CONFIG;

typedef union {
PACKED_GPIO_CONFIG    PackedGpio;
UINT64                Data64;
} COMMON_GPIO_CONFIG;

#pragma pack()

#endif

