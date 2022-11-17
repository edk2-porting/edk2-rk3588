/** @file
  Struct definition for CpuPrivateData.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CPU_PRIVATE_DATA_H_
#define _CPU_PRIVATE_DATA_H_

#include <CpuInitDataHob.h>

///
/// CPU Private Data saved and restored for S3.
///
typedef struct {
  UINT64                    ProcessorTraceAddress[MAX_PROCESSOR_THREADS];
  UINT32                    S3BspMtrrTablePointer;
} CPU_PRIVATE_DATA;

#endif
