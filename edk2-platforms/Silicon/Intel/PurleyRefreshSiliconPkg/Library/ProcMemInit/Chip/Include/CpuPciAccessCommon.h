/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  _CPU_PCI_ACCESS_COMMON_H_
#define  _CPU_PCI_ACCESS_COMMON_H_

#include <SysHost.h>

/**

  Indetifies the bus number for given SocId & BoxType

  @param host            - Pointer to sysHost, the system host (root) structure
  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxType         - Box Type; values come from CpuPciAccess.h
  @param BoxInst         - IIO PCIE Box Instance
  @param FuncBlk         - Function Block within IIO
  @param CpuCsrAccessVar - Pointer to CSR access data

  @retval (UINT32) PCI bus number

**/
UINT32
GetBusNumber (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  );

/**

  Indetifies the device number for given SocId & BoxType

  @param host            - Pointer to sysHost, the system host (root) structure
  @param BoxType         - Box Type; values come from CpuPciAccess.h
  @param BoxInst         - IIO PCIE Box Instance
  @param FuncBlk         - Function Block within IIO
  @param CpuCsrAccessVar - Pointer to CSR access data

  @retval (UINT32) PCI device number

**/
UINT32
GetDeviceNumber (
  PSYSHOST host,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  );

/**

  Indetifies the function number for given SocId & BoxType

  @param host            - Pointer to sysHost, the system host (root) structure
  @param BoxType         - Box Type; values come from CpuPciAccess.h
  @param BoxInst         - IIO PCIE Box Instance
  @param FuncBlk         - Function Block within IIO
  @param CpuCsrAccessVar - Pointer to CSR access data

  @retval (UINT32) PCI function number

**/
UINT32
GetFunctionNumber (
  PSYSHOST host,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  );

#endif   // _CPU_PCI_ACCESS_COMMON_H_


