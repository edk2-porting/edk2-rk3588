/** @file

  Copyright (c) 2018, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __LSCONNECTOR_DXE_H__
#define __LSCONNECTOR_DXE_H__

#include <Guid/HiiPlatformSetupFormset.h>
#include <Guid/FormSet.h>

#define MEZZANINE_NONE                            0x0
#define MEZZANINE_SECURE96                        0x1
#define MEZZANINE_MAX                             0x2

#define NINETY_SIX_BOARDS_CONFIG_VARIABLE_NAME    L"NinetySixBoardsConfig"

typedef struct {
  UINT8         MezzanineType;
  UINT8         Reserved[7];
} NINETY_SIX_BOARDS_CONFIG_DATA;

#endif // __LSCONNECTOR_DXE_H__
