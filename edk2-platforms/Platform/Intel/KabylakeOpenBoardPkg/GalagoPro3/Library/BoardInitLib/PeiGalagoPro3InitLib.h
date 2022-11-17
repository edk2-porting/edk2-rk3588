/** @file

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_N1_XX_WU_BOARD_INIT_LIB_H_
#define _PEI_N1_XX_WU_BOARD_INIT_LIB_H_

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
extern HSIO_PTSS_TABLES PchLpHsioPtss_Bx_GalagoPro3[];
extern UINT16 PchLpHsioPtss_Bx_GALAGO_PRO_3_Size;
extern HSIO_PTSS_TABLES PchLpHsioPtss_Cx_GalagoPro3[];
extern UINT16 PchLpHsioPtss_Cx_GALAGO_PRO_3_Size;

extern HDAUDIO_VERB_TABLE HdaVerbTableAlc286Rvp3;
extern GPIO_INIT_CONFIG mGpioTableGalagoPro3UcmcDevice[];
extern UINT16 mGpioTableGalagoPro3UcmcDeviceSize;

extern IO_EXPANDER_GPIO_CONFIG mGpioTableIoExpander[];
extern UINT16 mGpioTableIoExpanderSize;
extern GPIO_INIT_CONFIG mGpioTableGalagoPro3Touchpanel;
extern GPIO_INIT_CONFIG mGpioTableGalagoPro3[];
extern UINT16 mGpioTableGalagoPro3Size;

#endif // _PEI_N1_XX_WU_BOARD_INIT_LIB_H_
