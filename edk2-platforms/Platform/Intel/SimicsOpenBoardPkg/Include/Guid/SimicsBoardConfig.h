/** @file
  GUID for UEFI variables that are specific to Simics Board configuration.

  Copyright (C) 2014, Red Hat, Inc.
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __SIMICS_BOARD_CONFIG_H__
#define __SIMICS_BOARD_CONFIG_H__

#define SIMICS_BOARD_CONFIG_GUID \
{0x8a318e00, 0xfaf5, 0x499f, { 0x91,0x75, 0xce, 0x4d, 0x8d, 0xa6, 0x70, 0xae}}

extern EFI_GUID gSimicsBoardConfigGuid;

#endif
