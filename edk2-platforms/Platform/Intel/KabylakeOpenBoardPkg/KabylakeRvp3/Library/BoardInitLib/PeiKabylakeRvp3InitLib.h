/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_KABYLAKE_RVP3_BOARD_INIT_LIB_H_
#define _PEI_KABYLAKE_RVP3_BOARD_INIT_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <Ppi/SiPolicy.h>
#include <PchHsioPtssTables.h>
#include <IoExpander.h>

#include <PlatformBoardId.h>

extern const UINT8 mDqByteMapSklRvp3[2][6][2];
extern const UINT8 mDqsMapCpu2DramSklRvp3[2][8];
extern const UINT8 mSkylakeRvp3Spd110[];
extern const UINT16 mSkylakeRvp3Spd110Size;
extern const UINT8 mSkylakeRvp3Spd[];
extern const UINT16 mSkylakeRvp3SpdSize;
extern HSIO_PTSS_TABLES PchLpHsioPtss_Bx_KabylakeRvp3[];
extern UINT16 PchLpHsioPtss_Bx_KabylakeRvp3_Size;
extern HSIO_PTSS_TABLES PchLpHsioPtss_Cx_KabylakeRvp3[];
extern UINT16 PchLpHsioPtss_Cx_KabylakeRvp3_Size;

extern HDAUDIO_VERB_TABLE HdaVerbTableAlc286Rvp3;
extern GPIO_INIT_CONFIG mGpioTableLpddr3Rvp3UcmcDevice[];
extern UINT16 mGpioTableLpddr3Rvp3UcmcDeviceSize;

extern IO_EXPANDER_GPIO_CONFIG mGpioTableIoExpander[];
extern UINT16 mGpioTableIoExpanderSize;
extern GPIO_INIT_CONFIG mGpioTableLpDdr3Rvp3Touchpanel;
extern GPIO_INIT_CONFIG mGpioTableLpDdr3Rvp3[];
extern UINT16 mGpioTableLpDdr3Rvp3Size;

#endif // _PEI_KABYLAKE_RVP3_BOARD_INIT_LIB_H_
