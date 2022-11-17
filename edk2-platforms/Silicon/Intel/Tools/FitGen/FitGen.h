/**@file
Definitions for the FitGen utility.

Copyright (c) 2010-2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _FIT_GEN_H
#define _FIT_GEN_H

#include <stdio.h>
#include <stdlib.h>
#define PI_SPECIFICATION_VERSION  0x00010000
#define EFI_FVH_PI_REVISION       EFI_FVH_REVISION
#include <Common/UefiBaseTypes.h>
#include <Common/PiFirmwareFile.h>
#include <Common/PiFirmwareVolume.h>
#include <Guid/PiFirmwareFileSystem.h>
#include "EfiUtilityMsgs.c"
#include "CommonLib.h"
#include "ParseInf.h"
#include "FvLib.h"

//
// Utility Name
//
#define UTILITY_NAME  "FitGen"

//
// Utility version information
//
#define UTILITY_MAJOR_VERSION 0
#define UTILITY_MINOR_VERSION 64
#define UTILITY_DATE          __DATE__

//
// The minimum number of arguments accepted from the command line.
//
#define MIN_VIEW_ARGS   3
#define MIN_ARGS        4
#define BUF_SIZE        (8 * 1024)

#define GETOCCUPIEDSIZE(ActualSize, Alignment) \
  (ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1))
;

#define ROUNDUP(Size, Alignment) (((Size) + (Alignment) - 1) / (Alignment) * (Alignment))

#endif
