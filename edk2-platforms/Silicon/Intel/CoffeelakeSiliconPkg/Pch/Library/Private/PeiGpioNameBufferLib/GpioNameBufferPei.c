/** @file
  This file contains GpioMemLib implementation for PEI phase

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Private/Library/GpioNameBufferLib.h>

STATIC CONST EFI_GUID mGpioNamesPrivateHobGuid = {0x9AE3138D, 0x4EBF, 0x4E90, {0x87, 0x96, 0x11, 0xD3, 0x10, 0x04, 0x60, 0x0A}};

STATIC volatile BOOLEAN mGlobalMemoryWorking = FALSE;

STATIC CHAR8 mGpioNameBuffer[GPIO_NAME_LENGTH_MAX];

/**
  Returns pointer to the buffer taken from GpioLib private HOB

  @retval CHAR8*  Pointer to the buffer
**/
STATIC
CHAR8*
GetBufferFromHob (
  VOID
  )
{
  VOID  *Hob;
  CHAR8 *GpioNameBuffer;

  Hob = NULL;
  GpioNameBuffer = NULL;

  Hob = GetFirstGuidHob (&mGpioNamesPrivateHobGuid);
  if (Hob != NULL){
    GpioNameBuffer = (CHAR8*) GET_GUID_HOB_DATA (Hob);
  } else {
    GpioNameBuffer = (CHAR8*) BuildGuidHob (&mGpioNamesPrivateHobGuid, GPIO_NAME_LENGTH_MAX);
    if (GpioNameBuffer == NULL){
      DEBUG ((DEBUG_ERROR, "Failed to setup HOB for GPIO names lib\n"));
      ASSERT (FALSE);
    }
  }
  return GpioNameBuffer;
}

/**
  Returns pointer to the global buffer to be used by GpioNamesLib

  @retval CHAR8*  Pointer to the buffer
**/
CHAR8*
GpioGetStaticNameBuffer (
  VOID
  )
{
  mGlobalMemoryWorking = TRUE;

  if (mGlobalMemoryWorking) {
    return mGpioNameBuffer;
  } else {
    return GetBufferFromHob ();
  }
}

