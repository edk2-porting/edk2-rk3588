/** @file
  Header file for PCH RESET Driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_RESET_LIB_H_
#define _PCH_RESET_LIB_H_


/**
  Initialize PCH Reset APIs

  @retval EFI_SUCCESS             APIs are installed successfully
  @retval EFI_OUT_OF_RESOURCES    Can't allocate pool
**/
EFI_STATUS
EFIAPI
PchInitializeReset (
  VOID
  );

#endif
