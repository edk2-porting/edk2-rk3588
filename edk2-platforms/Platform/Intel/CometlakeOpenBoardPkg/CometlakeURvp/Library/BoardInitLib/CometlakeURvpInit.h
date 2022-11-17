/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _COMET_LAKE_U_RVP_INIT_H_
#define _COMET_LAKE_U_RVP_INIT_H_

#include <Uefi.h>
#include <IoExpander.h>
#include <PlatformBoardId.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <Library/SiliconInitLib.h>
#include <Ppi/SiPolicy.h>
#include <PchHsioPtssTables.h>

extern HSIO_PTSS_TABLES PchLpHsioPtss_Bx_CometlakeURvp[];
extern UINT16 PchLpHsioPtss_Bx_CometlakeURvp_Size;
extern HSIO_PTSS_TABLES PchLpHsioPtss_Cx_CometlakeURvp[];
extern UINT16 PchLpHsioPtss_Cx_CometlakeURvp_Size;


extern GPIO_INIT_CONFIG mGpioTableCmlULpddr3PreMem[];
extern UINT16 mGpioTableCmlULpddr3PreMemSize;
extern GPIO_INIT_CONFIG mGpioTableCmlULpddr3WwanOnEarlyPreMem[];
extern UINT16 mGpioTableCmlULpddr3WwanOnEarlyPreMemSize;
extern GPIO_INIT_CONFIG mGpioTableCmlULpddr3WwanOffEarlyPreMem[];
extern UINT16 mGpioTableCmlULpddr3WwanOffEarlyPreMemSize;

extern GPIO_INIT_CONFIG mGpioTableDefault[];
extern UINT16 mGpioTableDefaultSize;
extern GPIO_INIT_CONFIG mGpioTableCmlULpddr3[];
extern UINT16 mGpioTableCmlULpddr3Size;

#endif // _COMET_LAKE_U_LP3_INIT_H_

