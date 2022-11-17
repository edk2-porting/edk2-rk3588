/** @file
  This file contains implementation of the GpioMemLib for DXE phase

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Private/Library/GpioNameBufferLib.h>

STATIC CHAR8 mGpioNameBuffer[GPIO_NAME_LENGTH_MAX];

CHAR8*
GpioGetStaticNameBuffer (
  VOID
  )
{
  return mGpioNameBuffer;
}

