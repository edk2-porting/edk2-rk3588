/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

///
/// Gbe Ethernet ASL methods and structures 
///

  //
  // GPE bit 13 indicates wake from this device, can wakeup from S4 state
  //
  Method(_PRW, 0) {
    Return(Package() {13, 4})
  }