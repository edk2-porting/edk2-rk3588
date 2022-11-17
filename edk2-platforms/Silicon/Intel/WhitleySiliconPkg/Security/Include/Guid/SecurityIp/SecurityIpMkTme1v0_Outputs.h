/** @file
  Provides data structure information used by SiliconIp MK-TME

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// MK-TME
//
// NAK (Not a knob) - Used just for indication
UINT8  TmeCapability;                           // NAK (Not a knob) - TME Capable
UINT8  TmeCrSupport;                            // NAK (Not a knob) - Flag used to check if Crystal Ridge is supported in UEFI
UINT8  MktmeCapability;                         // NAK (Not a knob) - MK-TME Capable
UINT16 MktmeMaxKeys;                            // NAK (Not a knob) - Max number of keys used for encryption
UINT8  MkTmeKeyIdBits;                          // NAK (Not a knob) - Used to suppress setup menu key-splits
