/** @file
  This file contains GPIO name library implementation

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "GpioLibrary.h"
#include <Library/PrintLib.h>

/**
  Generates GPIO group name from GpioPad

  @param[in] GpioPad  GpioPad

  @retval CHAR8*  Pointer to the GPIO group name
**/
CONST
CHAR8*
GpioGetGroupName (
  IN UINT32  GroupIndex
  )
{
  CONST GPIO_GROUP_NAME_INFO*  GroupNameInfo;

  GroupNameInfo = GpioGetGroupNameInfo (GroupIndex);
  if (GroupNameInfo == NULL) {
    return NULL;
  } else {
    return GroupNameInfo->GpioGroupPrefix;
  }
}

/**
  Generates GPIO name from GpioPad

  @param[in]  GpioPad             GpioPad
  @param[out] GpioNameBuffer      Caller allocated buffer of GPIO_NAME_LENGTH_MAX size
  @param[in]  GpioNameBufferSize  Size of the buffer

  @retval CHAR8*  Pointer to the GPIO name
**/
CHAR8*
GpioGetPadName (
  IN GPIO_PAD  GpioPad,
  OUT CHAR8*   GpioNameBuffer,
  IN UINT32    GpioNameBufferSize
  )
{
  UINT32                       GroupIndex;
  UINT32                       PadNumber;
  UINT32                       FirstUniquePadNumber;
  CONST GPIO_GROUP_NAME_INFO*  GroupNameInfo;

  if (GpioNameBuffer == NULL) {
    ASSERT (FALSE);
    return NULL;
  }
  if ((GpioNameBufferSize < GPIO_NAME_LENGTH_MAX) || !GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    *GpioNameBuffer = 0;
    return NULL;
  }

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  GroupNameInfo = GpioGetGroupNameInfo (GroupIndex);
  if (GroupNameInfo == NULL) {
    return NULL;
  }

  FirstUniquePadNumber = GpioGetPadNumberFromGpioPad (GroupNameInfo->FirstUniqueGpio);
  if ((PadNumber < FirstUniquePadNumber) || (GroupNameInfo->GroupUniqueNames == NULL)) {
    AsciiSPrint (GpioNameBuffer, GPIO_NAME_LENGTH_MAX, "GPIO_%a%d", GpioGetGroupName (GroupIndex), PadNumber);
  } else {
    if (PadNumber - FirstUniquePadNumber < GroupNameInfo->UniqueNamesTableSize) {
      AsciiSPrint (GpioNameBuffer, GPIO_NAME_LENGTH_MAX, "GPIO_%a", GroupNameInfo->GroupUniqueNames[PadNumber - FirstUniquePadNumber]);
    } else {
      AsciiSPrint (GpioNameBuffer, GPIO_NAME_LENGTH_MAX, "GPIO_%08X", GpioPad);
      ASSERT (FALSE);
    }
  }

  return GpioNameBuffer;
}

