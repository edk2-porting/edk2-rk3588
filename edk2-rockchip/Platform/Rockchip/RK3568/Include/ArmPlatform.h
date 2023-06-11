/** @file
*
*  Copyright (c) 2014-2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

//
// We don't care about this value, but the PL031 driver depends on the macro
// to exist: it will pass it on to our ArmPlatformSysConfigLib:ConfigGet()
// function, which just returns EFI_UNSUPPORTED.
//
//
#define SYS_CFG_RTC                             0

#endif /* __PLATFORM_H__ */
