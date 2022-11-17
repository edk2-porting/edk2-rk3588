/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UP_XTREME_INIT_H_
#define _UP_XTREME_INIT_H_

#include <Uefi.h>
#include <IoExpander.h>
#include <PlatformBoardId.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <Ppi/SiPolicy.h>
#include <PchHsioPtssTables.h>

extern const UINT8 mUpXtremeSamsungDdr4Spd[];
extern const UINT16 mUpXtremeSamsungDdr4SpdSize;
extern const UINT8 mUpXtremeSkhynixDdr4Spd[];
extern const UINT16 mUpXtremeSkhynixDdr4SpdSize;

extern HSIO_PTSS_TABLES PchLpHsioPtss_Bx_UpXtreme[];
extern UINT16 PchLpHsioPtss_Bx_UpXtreme_Size;
extern HSIO_PTSS_TABLES PchLpHsioPtss_Cx_UpXtreme[];
extern UINT16 PchLpHsioPtss_Cx_UpXtreme_Size;

extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4PreMem[];
extern UINT16 mGpioTableWhlUDdr4PreMemSize;
extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4WwanOnEarlyPreMem[];
extern UINT16 mGpioTableWhlUDdr4WwanOnEarlyPreMemSize;
extern GPIO_INIT_CONFIG mGpioTableWhlUDdr4WwanOffEarlyPreMem[];
extern UINT16 mGpioTableWhlUDdr4WwanOffEarlyPreMemSize;

extern GPIO_INIT_CONFIG mGpioTableUpXtreme[];
extern UINT16 mGpioTableUpXtremeSize;
extern GPIO_INIT_CONFIG mGpioTableDefault[];
extern UINT16 mGpioTableDefaultSize;

#endif // _UP_XTREME_INIT_H_

