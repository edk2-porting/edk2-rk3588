/** @file
*
*  Copyright (c) 2017, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _BMC_CONFIG_BOOT_LIB_H_
#define _BMC_CONFIG_BOOT_LIB_H_

VOID
EFIAPI
RestoreBootOrder (
  VOID
  );

VOID
EFIAPI
HandleBmcBootType (
  VOID
  );

#endif
