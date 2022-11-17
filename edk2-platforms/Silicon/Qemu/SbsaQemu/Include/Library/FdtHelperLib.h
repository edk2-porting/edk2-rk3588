/** @file
*  FdtHelperLib.h
*
*  Copyright (c) 2021, NUVIA Inc. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef FDT_HELPER_LIB_
#define FDT_HELPER_LIB_

/**
  Get MPIDR for a given cpu from device tree passed by Qemu.

  @param [in]   CpuId    Index of cpu to retrieve MPIDR value for.

  @retval                MPIDR value of CPU at index <CpuId>
**/
UINT64
FdtHelperGetMpidr (
  IN UINTN   CpuId
  );

/** Walks through the Device Tree created by Qemu and counts the number
    of CPUs present in it.

    @return The number of CPUs present.
**/
EFIAPI
UINT32
FdtHelperCountCpus (
  VOID
  );

#endif /* FDT_HELPER_LIB_ */
