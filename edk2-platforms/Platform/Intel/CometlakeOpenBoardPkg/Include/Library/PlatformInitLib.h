/** @file
  Function prototype of PlatformInitLib.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_INIT_LIB_H_
#define _PLATFORM_INIT_LIB_H_

VOID
PlatformLateInit (
  VOID
  );

VOID
InitSerialPort (
  VOID
  );

#endif // _PLATFORM_INIT_LIB_H_

