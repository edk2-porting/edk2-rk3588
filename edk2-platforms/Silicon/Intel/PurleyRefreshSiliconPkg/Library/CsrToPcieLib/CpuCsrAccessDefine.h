/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  _CPU_CSR_ACCESS_DEFINE_H_
#define  _CPU_CSR_ACCESS_DEFINE_H_

#include <SysHost.h>
#include <MemHost.h>
#include <Library/PcieAddress.h>

#include <CsrToPcieAddress.h>
#include <CpuPciAccessCommon.h>


typedef enum {
  BUS_CLASS = 0,
  DEVICE_CLASS = 1,
  FUNCTION_CLASS = 2
} BDF_CLASS;

UINT32
GetSegmentNumber (
  IN USRA_ADDRESS    *Address
  );

UINT32
GetBDFNumber (
  IN USRA_ADDRESS    *Address,
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVar,
  IN UINT8                    BDFType
  );

UINT32
GetCpuCsrAddress (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT8    *Size
  );

UINT32
GetMmcfgAddress(
  PSYSHOST host
  );

VOID
GetCpuCsrAccessVar_RC (
  PSYSHOST host,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  );

#endif   // _CPU_CSR_ACCESS_DEFINE_H_
