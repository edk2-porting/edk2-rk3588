/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _WHISKEY_LAKE_U_RVP_INIT_H_
#define _WHISKEY_LAKE_U_RVP_INIT_H_

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

extern HSIO_PTSS_TABLES PchLpHsioPtss_Bx_WhiskeylakeURvp[];
extern UINT16 PchLpHsioPtss_Bx_WhiskeylakeURvp_Size;
extern HSIO_PTSS_TABLES PchLpHsioPtss_Cx_WhiskeylakeURvp[];
extern UINT16 PchLpHsioPtss_Cx_WhiskeylakeURvp_Size;

extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4PreMem[];
extern UINT16 mGpioTableWhlUDdr4PreMemSize;
extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4WwanOnEarlyPreMem[];
extern UINT16 mGpioTableWhlUDdr4WwanOnEarlyPreMemSize;
extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4WwanOffEarlyPreMem[];
extern UINT16 mGpioTableWhlUDdr4WwanOffEarlyPreMemSize;

extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4_0[];
extern UINT16 mGpioTableWhlUDdr4_0Size;
extern GPIO_INIT_CONFIG mGpioTableDefault[];
extern UINT16 mGpioTableDefaultSize;
extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4[];
extern UINT16 mGpioTableWhlUDdr4Size;

#endif // _WHISKEY_LAKE_U_RVP_INIT_H_

