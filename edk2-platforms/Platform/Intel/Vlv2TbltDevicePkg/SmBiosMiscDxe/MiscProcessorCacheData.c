/*++

Copyright (c) 2012 - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscBiosProcessorCache.c

Abstract:

  Processor cache static data.
  Misc. subclass type 7.
  SMBIOS type 7.

--*/


#include "CommonHeader.h"

#include "MiscSubclassDriver.h"


//
// Static (possibly build generated) Processor cache data.
//
MISC_SMBIOS_TABLE_DATA(UINTN, MiscProcessorCache) = 0;
