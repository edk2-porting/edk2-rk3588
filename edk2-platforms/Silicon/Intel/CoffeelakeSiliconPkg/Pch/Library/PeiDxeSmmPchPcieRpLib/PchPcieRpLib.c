/** @file
  PCH PCIE root port library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPcieRpLib.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsPcie.h>
#include <Register/PchRegsPcr.h>

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_PCIE_CONTROLLER_INFO mPchPcieControllerInfo[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, PID_SPA,  0 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, PID_SPB,  4 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, PID_SPC,  8 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, PID_SPD, 12 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, PID_SPE, 16 }, // PCH-H only
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, PID_SPF, 20 }  // PCH-H only
};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 mPchPcieControllerInfoSize = sizeof (mPchPcieControllerInfo) / sizeof (mPchPcieControllerInfo[0]);

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mPchLpRstPcieStorageSupportedPort[] = {
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP1..RP4
  RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,         // RP5..RP8
  RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,         // RP9..RP12
  RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3          // RP13..RP16
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mPchHRstPcieStorageSupportedPort[] = {
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP1..RP4
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP5..RP8
  RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,         // RP9..RP12
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP13..RP16
  RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,         // RP17..RP20
  RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2          // RP21..RP24
};

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  UINTN       Index;
  UINTN       FuncIndex;
  UINT32      PciePcd;

  if (RpNumber >= GetPchMaxPciePortNum ()) {
    DEBUG ((DEBUG_ERROR, "GetPchPcieRpDevFun invalid RpNumber %x", RpNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Index = RpNumber / PCH_PCIE_CONTROLLER_PORTS;
  FuncIndex = RpNumber - mPchPcieControllerInfo[Index].RpNumBase;
  *RpDev = mPchPcieControllerInfo[Index].DevNum;
  PciePcd = PchPcrRead32 (mPchPcieControllerInfo[Index].Pid, R_SPX_PCR_PCD);
  *RpFun = (PciePcd >> (FuncIndex * S_SPX_PCR_PCD_RP_FIELD)) & B_SPX_PCR_PCD_RP1FN;

  return EFI_SUCCESS;
}

/**
  Get Root Port physical Number by Pch Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)

  @retval     EFI_SUCCESS           Physical root port is retrieved
**/
EFI_STATUS
EFIAPI
GetPchPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  )
{
  UINT64 RpBase;

  RpBase = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, RpDev, RpFun, 0);
  *RpNumber = (PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LCAP) >> N_PCH_PCIE_CFG_LCAP_PN) -1;
  return EFI_SUCCESS;
}

/**
  Gets pci segment base address of PCIe root port.

  @param RpIndex    Root Port Index (0 based)
  @return PCIe port base address.
**/
UINT64
PchPcieBase (
  IN  UINT32   RpIndex
  )
{
  UINTN       RpDevice;
  UINTN       RpFunction;

  GetPchPcieRpDevFun (RpIndex, &RpDevice, &RpFunction);

  return PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDevice, (UINT32) RpFunction, 0);
}

/**
  Determines whether L0s is supported on current stepping.

  @return TRUE if L0s is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieL0sSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Some early PCH steppings require Native ASPM to be disabled due to hardware issues:
   - RxL0s exit causes recovery
   - Disabling PCIe L0s capability disables L1
  Use this function to determine affected steppings.

  @return TRUE if Native ASPM is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieNativeAspmSupported (
  VOID
  )
{
  return PchIsPcieL0sSupported ();
}

/**
  Check the RST PCIe Storage Cycle Router number according to the root port number and PCH type

  @param[in] RootPortNum  Root Port Number

  @return  The RST PCIe Storage Cycle Router Number
**/
UINT8
RstGetCycleRouterNumber (
  IN  UINT32                   RootPortNum
  )
{
  if (IsPchLp ()) {
    if (RootPortNum < ARRAY_SIZE (mPchLpRstPcieStorageSupportedPort)) {
      return mPchLpRstPcieStorageSupportedPort[RootPortNum];
    }
  } else if (IsPchH ()) {
    if (RootPortNum < ARRAY_SIZE (mPchHRstPcieStorageSupportedPort)) {
      return mPchHRstPcieStorageSupportedPort[RootPortNum];
    }
  }
  return RST_PCIE_STORAGE_CR_INVALID;
}

