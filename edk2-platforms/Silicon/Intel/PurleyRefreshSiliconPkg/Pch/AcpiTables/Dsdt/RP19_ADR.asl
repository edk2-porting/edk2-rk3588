/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Method (_ADR, 0) {
  If (LNotEqual(RPAJ,0)) {
    Return (RPAJ)
  } Else {
    Return (0x001B0002)
  }
}