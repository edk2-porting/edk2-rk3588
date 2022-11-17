/** @file
  Get the global VTd PMR alignment information library.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef __GET_VTD_PMR_ALIGN_LIB_H__
#define __GET_VTD_PMR_ALIGN_LIB_H__
#include <Library/BaseLib.h>

/**
  Get the global VT-d protected memory alignment.
  @return The maximum protected memory alignment. Ex: 0x100000
**/
UINTN
EFIAPI
GetGlobalVtdPmrAlignment (
);

#endif // __GET_VTD_PMR_ALIGN_LIB_H__
