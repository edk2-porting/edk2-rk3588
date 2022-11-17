/** @file
  CPU PCIe information library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/CpuPcieRegs.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <Library/CpuPcieInitCommon.h>
#include <CpuPcieInfo.h>
#include <Register/SaRegsHostBridge.h>
#include <PcieRegs.h>

/**
  Get Maximum CPU Pcie Root Port Number

  @retval Maximum CPU Pcie Root Port Number
**/
UINT8
GetMaxCpuPciePortNum (
  VOID
  )
{
  return CPU_PCIE_ULT_ULX_MAX_ROOT_PORT;
}

/**
  Get CPU Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetCpuPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  if (RpNumber > GetMaxCpuPciePortNum ()) {
    DEBUG ((DEBUG_ERROR, "GetCpuPcieRpDevFun invalid RpNumber %x", RpNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  //  For TGL - U/Y only one CPU PCIE Root port is present
  //
  *RpDev = 6;
  *RpFun = 0;
  return EFI_SUCCESS;
}
/**

  Gets pci segment base address of PCIe root port.

  @param RpIndex    Root Port Index (0 based)

  @return PCIe port base address.
**/
UINT64
CpuPcieBase (
  IN  UINT32   RpIndex
  )
{
  UINTN   RpDevice;
  UINTN   RpFunction;
  GetCpuPcieRpDevFun (RpIndex, &RpDevice, &RpFunction);
  return PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, (UINT32) RpDevice, (UINT32) RpFunction, 0);
}
