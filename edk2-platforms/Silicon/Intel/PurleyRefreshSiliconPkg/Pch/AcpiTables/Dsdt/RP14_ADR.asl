/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Method (_ADR, 0) {
  If (LNotEqual(RPAE,0)) {
    Return (RPAE)
  } Else {
    Return (0x001D0005)
  }
}