/** @file
  Definitions for Sps Pei Library

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SPS_PEI_LIB_H
#define _SPS_PEI_LIB_H

// Available clocking modes
typedef enum _CLOCKING_MODES {
  InternalStandard        = 0,
  InternalAlternate,
  HybridStandard,
  HybridAlternate,
  External
} CLOCKING_MODES;

#endif // _SPS_PEI_LIB_H
