/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _HEST_H_
#define _HEST_H_

#include "Apei.h"


typedef struct _HEST_CONTEXT {
  EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_HEADER  *HestHeader;         // pointer to hest header
  UINT32                                           OccupiedMemorySize; // allocated memory size for hest
  VOID                                             *KeyErrorSource;     // key error source, valtile
} HEST_CONTEXT;

EFI_STATUS
HestAddErrorSourceDescriptor (
  IN OUT HEST_CONTEXT  *Context,
  IN VOID              *ErrorSourceDescriptor,
  IN UINT32            SizeOfDescriptor
);
VOID
HestSetAcpiTable (
  IN HEST_CONTEXT *Context
);
EFI_STATUS
HestHeaderCreator (
  HEST_CONTEXT  *Context,
  UINT32        PreAllocatedHestSize
);

/**
* OEM Interface declaration
* 1.Interface is not realized default
* 2.OEM should implement this interface
*/
extern
VOID
OemHestInitialNotification (VOID);

extern
EFI_STATUS
OemInitHestTable(
  IN EFI_HANDLE ImageHandle
);

#endif    // _HEST_H_
