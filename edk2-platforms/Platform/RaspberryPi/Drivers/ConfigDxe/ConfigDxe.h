/** @file
 *
 *  Copyright (c) 2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef _CONFIG_DXE_H_
#define _CONFIG_DXE_H_

#include <Uefi.h>
#include <Protocol/RpiFirmware.h>

VOID RegisterXhciQuirkHandler (
  IN RASPBERRY_PI_FIRMWARE_PROTOCOL *FwProtocol
  );

#endif /* _CONFIG_DXE_H_ */
