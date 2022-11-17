/** @file
  GUID used for Cpu Nvram Data entries in the HOB list.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_NVRAM_DATA_GUID_H_
#define _CPU_NVRAM_DATA_GUID_H_

#include <CpuDataStruct.h>

#define EFI_CPU_NVRAM_DATA_GUID \
  { \
  0x184220a2, 0xe14c, 0x4497, { 0x85, 0xbb, 0x14, 0x90, 0xa9, 0xa1, 0xf0, 0xd3 }\
  }

#define EFI_CPU_NVRAM_DATA_VARIABLE_NAME L"CpuNvramData"

//
//  CPU_NVRAM
//  Data that need to be saved in NVRAM for S3 resume
//
typedef struct {                          // data that need to be saved in NVRAM for S3 resume
  UINT32 flexRatioCsr;                    //  Common for all sockets
  UINT64 DesiredCoresCsr[MAX_SOCKET];    //  One per socket 64bits
  UINT32 DesiredCoresCfg2Csr[MAX_SOCKET]; //  One per socket
} CPU_NVRAM;

extern EFI_GUID gEfiCpuNvramDataGuid;

#endif
