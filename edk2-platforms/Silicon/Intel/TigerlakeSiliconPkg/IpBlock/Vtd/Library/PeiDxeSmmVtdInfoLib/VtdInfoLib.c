/** @file
  VTD Info library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Ppi/SiPolicy.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/VtdInfoLib.h>
#include <Register/VtdRegs.h>

/**
  Get VTD Engine Base Address from PCD value

  @param[in]  VtdEngineNumber        - Engine number for which VTD Base Adderess is required.

  @retval   VTD Engine Base Address
**/
UINT32
GetVtdBaseAddress (
  IN UINT8        VtdEngineNumber
  )
{
  switch (VtdEngineNumber) {
    case 0:
      return PcdGet32(VtdEngine1BaseAddeess);
      break;
    case 2:
      return PcdGet32(VtdEngine3BaseAddeess);
      break;
    default:
      return 0x0;
      break;
  }
}


/**
  Read VTD Engine Base Address from VTD BAR Offsets.

  @param[in]  VtdEngineNumber        - Engine number for which VTD Base Adderess is required.

  @retval   VTD Engine Base Address
**/
UINT32
ReadVtdBaseAddress (
  IN UINT8        VtdEngineNumber
  )
{
  UINT64              McD0BaseAddress;
  UINTN               MchBar;

  McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  MchBar          = PciSegmentRead32 (McD0BaseAddress + R_SA_MCHBAR) & (~BIT0);

  switch (VtdEngineNumber) {
    case 0:
      return (MmioRead32 (MchBar + R_MCHBAR_VTD1_OFFSET) & (~BIT0));
      break;
    case 2:
      return (MmioRead32 (MchBar + R_MCHBAR_VTD3_OFFSET) & (~BIT0));
      break;
    default:
      return 0x0;
      break;
  }
}

/**
  GetMaxVtdEngineNumber: Get Maximum Vtd Engine Number

  @retval Vtd Engine Number
**/
UINT8
GetMaxVtdEngineNumber(
  VOID
)
{
  return (UINT8)VTD_ENGINE_NUMBER;
}
