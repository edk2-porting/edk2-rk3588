/** @file
  Source code file for Report CPU HOB library.

Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Register/Intel/Cpuid.h>


/**
  Build a HOB for the CPU.
**/
VOID
EFIAPI
ReportCpuHob (
  VOID
  )
{
  UINT8                           PhysicalAddressBits;
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX  AddressSizeEax;

  AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &AddressSizeEax.Uint32, NULL, NULL, NULL);
  if (AddressSizeEax.Uint32 >= CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &AddressSizeEax.Uint32, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8)AddressSizeEax.Bits.PhysicalAddressBits;
  } else {
    PhysicalAddressBits = 36;
  }

  ///
  /// Create a CPU hand-off information
  ///
  BuildCpuHob (PhysicalAddressBits, 16);
}
