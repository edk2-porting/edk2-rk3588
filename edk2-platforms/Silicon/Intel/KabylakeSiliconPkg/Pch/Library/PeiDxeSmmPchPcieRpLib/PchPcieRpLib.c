/** @file
  PCH PCIE root port library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciLib.h>
#include <PchAccess.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPcieRpLib.h>

//
// For SKL PCH-LP, it supports three express port controllers:
//   Controller 1:
//     Port 1-4, Device 28, function 0-3
//   Controller 2:
//     Port 5-8, Device 28, function 4-7
//   Controller 3:
//     port 9-12, Device 29, function 0-3
// For SKL PCH-H, it supports five express port controllers:
//   Controller 1:
//     Port 1-4, Device 28, function 0-3
//   Controller 2:
//     Port 5-8, Device 28, function 4-7
//   Controller 3:
//     port 9-12, Device 29, function 0-3
//   Controller 4:
//     Port 13-16, Device 29, function 4-7
//   Controller 5:
//     port 17-20, Device 27, function 0-3
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_PCIE_CONTROLLER_INFO mPchPcieControllerInfo[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, PID_SPA,  0 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, PID_SPB,  4 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, PID_SPC,  8 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, PID_SPD, 12 }, // SKL-H and KBL-H only
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, PID_SPE, 16 }, // SKL-H and KBL-H only
  { PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, PID_SPF, 20 }  // KBL-H only
};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 mPchPcieControllerInfoSize = sizeof (mPchPcieControllerInfo) / sizeof (mPchPcieControllerInfo[0]);

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mPchLpRstPcieStorageSupportedPort[] = {
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP1..RP4
  RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,         // RP5..RP8
  RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3          // RP9..RP12
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSklPchHRstPcieStorageSupportedPort[] = {
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP1..RP4
  RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID, RST_PCIE_STORAGE_CR_INVALID,   // RP5..RP8
  RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,       RST_PCIE_STORAGE_CR_1,         // RP9..RP12
  RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,       RST_PCIE_STORAGE_CR_2,         // RP13..RP16
  RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3,       RST_PCIE_STORAGE_CR_3          // RP17..RP20
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mKblPchHRstPcieStorageSupportedPort[] = {
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

  //
  // if SKL PCH-LP, RpNumber must be < 12.
  // if SKL PCH-H , RpNumber must be < 20.
  // if KBL PCH-H , RpNumber must be < 24.
  //
  if (RpNumber >= GetPchMaxPciePortNum ()) {
    DEBUG ((DEBUG_ERROR, "GetPchPcieRpDevFun invalid RpNumber %x", RpNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Index = RpNumber / PCH_PCIE_CONTROLLER_PORTS;
  FuncIndex = RpNumber - mPchPcieControllerInfo[Index].RpNumBase;
  *RpDev = mPchPcieControllerInfo[Index].DevNum;
  PchPcrRead32 (mPchPcieControllerInfo[Index].Pid, R_PCH_PCR_SPX_PCD, &PciePcd);
  *RpFun = (PciePcd >> (FuncIndex * S_PCH_PCR_SPX_PCD_RP_FIELD)) & B_PCH_PCR_SPX_PCD_RP1FN;

  return EFI_SUCCESS;
}

/**
  Get Root Port physical Number by Pch Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)

  @retval     EFI_SUCCESS           Physical root port is retrieved
  @retval     EFI_INVALID_PARAMETER RpDev and/or RpFun are invalid
  @retval     EFI_UNSUPPORTED       Root port device and function is not assigned to any physical root port
**/
EFI_STATUS
EFIAPI
GetPchPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  )
{
  UINTN       Index;
  UINTN       FuncIndex;
  UINT32      PciePcd;
  UINT8       MaxPorts;

  MaxPorts = GetPchMaxPciePortNum ();
  if ( !((RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1) && (RpFun < 8) && (RpFun < MaxPorts)) &&
       !((RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2) && (RpFun < 8) && (RpFun + 8 < MaxPorts)) &&
       !((RpDev == PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3) && (RpFun < 8) && (RpFun + 16 < MaxPorts)) ) {
    DEBUG ((DEBUG_ERROR, "GetPchPcieRpNumber invalid RpDev %x RpFun %x", RpDev, RpFun));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < mPchPcieControllerInfoSize; Index++) {
    if (mPchPcieControllerInfo[Index].DevNum == RpDev) {
      PchPcrRead32 (mPchPcieControllerInfo[Index].Pid, R_PCH_PCR_SPX_PCD, &PciePcd);
      for (FuncIndex = 0; FuncIndex < 4; FuncIndex ++) {
        if (RpFun == ((PciePcd >> (FuncIndex * S_PCH_PCR_SPX_PCD_RP_FIELD)) & B_PCH_PCR_SPX_PCD_RP1FN)) {
          break;
        }
      }
      if (FuncIndex < 4) {
        *RpNumber = mPchPcieControllerInfo[Index].RpNumBase + FuncIndex;
        break;
      }
    }
  }
  if (Index >= mPchPcieControllerInfoSize) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Gets base address of PCIe root port.

  @param RpIndex    Root Port Index (0 based)
  @return PCIe port base address.
**/
UINTN
PchPcieBase (
  IN  UINT32   RpIndex
  )
{
  UINTN   RpDevice;
  UINTN   RpFunction;
  GetPchPcieRpDevFun (RpIndex, &RpDevice, &RpFunction);
  return MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDevice, (UINT32) RpFunction);
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
  if (GetPchGeneration () == SklPch) {
    if (GetPchSeries () == PchLp) {
      if (PchStepping () < PchLpC0) {
        return FALSE;
      }
    } else {
      if (PchStepping () < PchHD0) {
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
  Some early SKL PCH steppings require Native ASPM to be disabled due to hardware issues:
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

  @param[in] RootPortNum            Root Port Number

  @retval UINT32                    The RST PCIe Storage Cycle Router Number
**/
UINT32
RstGetCycleRouterNumber (
  IN  UINT32                   RootPortNum
  )
{
  PCH_SERIES        PchSeries;

  PchSeries         = GetPchSeries ();

  if (PchSeries == PchLp) {
    if (RootPortNum < PCH_LP_PCIE_MAX_ROOT_PORTS) {
      return mPchLpRstPcieStorageSupportedPort[RootPortNum];
    }
  } else {
    if (GetPchGeneration () == KblPch) {
      if (RootPortNum < KBL_PCH_H_PCIE_MAX_ROOT_PORTS) {
        return mKblPchHRstPcieStorageSupportedPort[RootPortNum];
      }
    } else {
      if (RootPortNum < SKL_PCH_H_PCIE_MAX_ROOT_PORTS) {
        return mSklPchHRstPcieStorageSupportedPort[RootPortNum];
      }
    }
  }
  return RST_PCIE_STORAGE_CR_INVALID;
}

