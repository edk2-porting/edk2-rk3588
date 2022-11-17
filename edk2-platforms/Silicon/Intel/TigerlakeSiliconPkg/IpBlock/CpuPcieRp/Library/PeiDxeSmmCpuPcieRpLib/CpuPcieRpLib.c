/** @file
  CPU PCIe root port library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <PcieRegs.h>
#include <Register/CpuPcieRegs.h>
#include "CpuPcieInfo.h"
#include <Library/CpuPcieInitCommon.h>
#include <Library/PciLib.h>
#include <IndustryStandard/Pci22.h>

/**
  Determines whether PCIe link is active

  @param[in] RpBase    Root Port base address
  @retval Link Active state
**/
BOOLEAN
CpuPcieIsLinkActive (
  UINT64  RpBase
  )
{
  return !! (PegPciSegmentRead16 (RpBase + R_PCIE_LSTS) & B_PCIE_LSTS_LA);
}

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port base address
  @return    Max link speed
**/
UINT32
CpuPcieGetMaxLinkSpeed (
  UINT64 RpBase
  )
{
  return PegPciSegmentRead32 (RpBase + R_PCIE_LCAP) & B_PCIE_LCAP_MLS;
}
