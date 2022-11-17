/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_ASPIRE_VN7_572G_BOARD_INIT_LIB_H_
#define _PEI_ASPIRE_VN7_572G_BOARD_INIT_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <Ppi/SiPolicy.h>
#include <PchHsioPtssTables.h>

#include <PlatformBoardId.h>

extern HSIO_PTSS_TABLES PchLpHsioPtss_AspireVn7Dash572G[];
extern UINT16 PchLpHsioPtss_AspireVn7Dash572G_Size;

extern HDAUDIO_VERB_TABLE HdaVerbTableAlc255AspireVn7Dash572G;
extern HDAUDIO_VERB_TABLE HdaVerbTableDisplayAudio;

extern GPIO_INIT_CONFIG mGpioTableAspireVn7Dash572G[];
extern UINT16 mGpioTableAspireVn7Dash572GSize;
extern GPIO_INIT_CONFIG mGpioTableAspireVn7Dash572G_early[];
extern UINT16 mGpioTableAspireVn7Dash572G_earlySize;

#endif // _PEI_ASPIRE_VN7_572G_BOARD_INIT_LIB_H_
