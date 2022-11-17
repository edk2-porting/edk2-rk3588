/** @file
*  FdtHelperLib.c
*
*  Copyright (c) 2021, NUVIA Inc. All rights reserved.
*  Copyright (c) 2020, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/FdtHelperLib.h>
#include <Library/PcdLib.h>
#include <libfdt.h>

STATIC INT32 mFdtFirstCpuOffset;
STATIC INT32 mFdtCpuNodeSize;

/**
  Get MPIDR for a given cpu from device tree passed by Qemu.

  @param [in]   CpuId    Index of cpu to retrieve MPIDR value for.

  @retval                MPIDR value of CPU at index <CpuId>
**/
UINT64
FdtHelperGetMpidr (
  IN UINTN   CpuId
  )
{
  VOID           *DeviceTreeBase;
  CONST UINT64   *RegVal;
  INT32          Len;

  DeviceTreeBase = (VOID *)(UINTN)PcdGet64 (PcdDeviceTreeBaseAddress);
  ASSERT (DeviceTreeBase != NULL);

  RegVal = fdt_getprop (DeviceTreeBase,
             mFdtFirstCpuOffset + (CpuId * mFdtCpuNodeSize),
             "reg",
             &Len);
  if (!RegVal) {
    DEBUG ((DEBUG_ERROR, "Couldn't find reg property for CPU:%d\n", CpuId));
    return 0;
  }

  return (fdt64_to_cpu (ReadUnaligned64 (RegVal)));
}

/** Walks through the Device Tree created by Qemu and counts the number
    of CPUs present in it.

    @return The number of CPUs present.
**/
EFIAPI
UINT32
FdtHelperCountCpus (
  VOID
  )
{
  VOID   *DeviceTreeBase;
  INT32  Node;
  INT32  Prev;
  INT32  CpuNode;
  UINT32 CpuCount;

  DeviceTreeBase = (VOID *)(UINTN)PcdGet64 (PcdDeviceTreeBaseAddress);
  ASSERT (DeviceTreeBase != NULL);

  // Make sure we have a valid device tree blob
  ASSERT (fdt_check_header (DeviceTreeBase) == 0);

  CpuNode = fdt_path_offset (DeviceTreeBase, "/cpus");
  if (CpuNode <= 0) {
    DEBUG ((DEBUG_ERROR, "Unable to locate /cpus in device tree\n"));
    return 0;
  }

  CpuCount = 0;

  // Walk through /cpus node and count the number of subnodes.
  // The count of these subnodes corresponds to the number of
  // CPUs created by Qemu.
  Prev = fdt_first_subnode (DeviceTreeBase, CpuNode);
  mFdtFirstCpuOffset = Prev;
  while (1) {
    CpuCount++;
    Node = fdt_next_subnode (DeviceTreeBase, Prev);
    if (Node < 0) {
      break;
    }
    mFdtCpuNodeSize = Node - Prev;
    Prev = Node;
  }

  return CpuCount;
}
