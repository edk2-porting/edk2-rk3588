/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Method (_ADR, 0) {
  If (LNotEqual(RPA9,0)) {
    Return (RPA9)
  } Else {
    Return (0x001D0000)
  }
}