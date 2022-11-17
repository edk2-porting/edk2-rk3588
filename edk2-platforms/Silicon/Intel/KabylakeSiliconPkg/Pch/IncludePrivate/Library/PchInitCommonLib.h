/** @file
  Header file for PCH Init Common Lib

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_INIT_COMMON_LIB_H_
#define _PCH_INIT_COMMON_LIB_H_

#include <PchPolicyCommon.h>
#include <PchAccess.h>

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieControllerSbiPid (
  IN  UINT32  ControllerIndex
  );

/**
  This function returns PID according to Root Port Number

  @param[in] RpPort             Root Port Number

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
GetRpSbiPid (
  IN  UINTN  RpPort
  );

/**
  Calculate root port device number based on physical port index.

  @param[in]  RpIndex              Root port index (0-based).

  @retval     Root port device number.
**/
UINT32
PchGetPcieRpDevice (
  IN  UINT32   RpIndex
  );

/**
  This function reads Pci Config register via SBI Access

  @param[in]  RpIndex             Root Port Index (0-based)
  @param[in]  Offset              Offset of Config register
  @param[out] *Data32             Value of Config register

  @retval EFI_SUCCESS             SBI Read successful.
**/
EFI_STATUS
PchSbiRpPciRead32 (
  IN    UINT32  RpIndex,
  IN    UINT32  Offset,
  OUT   UINT32  *Data32
  );

/**
  This function And then Or Pci Config register via SBI Access

  @param[in]  RpIndex             Root Port Index (0-based)
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciAndThenOr32 (
  IN  UINT32  RpIndex,
  IN  UINT32  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  );

/**
  Configure root port function number mapping

  @retval EFI_SUCCESS                   The function completed successfully
**/
EFI_STATUS
PchConfigureRpfnMapping (
  VOID
  );

/**
  This function lock down the P2sb SBI before going into OS.

  @param[in] SbiUnlock
**/
VOID
ConfigureP2sbSbiLock (
  IN  CONST BOOLEAN             SbiUnlock
  );

/**
  Bios will remove the host accessing right to Sideband register range
  prior to any 3rd party code execution.

  1) Set EPMASK5 Offset C4 (bits 29, 28, 27, 26, 17, 16, 10, 1) to disable Sideband access for PSF and MIPI controller
  2) Set EPMASK7 Offset CC (bits 6, 5) to disable Sideband access for XHCI controller
  3) Set the "Endpoint Mask Lock!", P2SB PCI offset E2h bit[1] to 1.
**/
VOID
RemoveSidebandAccess(
  VOID
  );

/**
  Configure PMC static function disable lock
**/
VOID
ConfigurePmcStaticFunctionDisableLock (
  VOID
  );

/**
  Print registers value

  @param[in] PrintMmioBase       Mmio base address
  @param[in] PrintSize           Number of registers
  @param[in] OffsetFromBase      Offset from mmio base address

  @retval None
**/
VOID
PrintRegisters (
  IN  UINTN        PrintMmioBase,
  IN  UINT32       PrintSize,
  IN  UINT32       OffsetFromBase
  );

VOID
PrintPchPciConfigSpace (
  VOID
  );

/**
  Check if RST PCIe Storage Remapping is enabled based on policy

  @param[in] RstPcieStorageRemap    The PCH PCIe Storage remapping

  @retval TRUE                      RST PCIe Storage Remapping is enabled
  @retval FALSE                     RST PCIe Storage Remapping is disabled
**/
BOOLEAN
IsRstPcieStorageRemapEnabled (
  IN  CONST PCH_RST_PCIE_STORAGE_CONFIG   *RstPcieStorageRemap
  );

/**
  Disable the RST remap address decoding range while RST is disabled.

  @param[in] AhciBar                ABAR address
**/
VOID
DisableRstRemapDecoding (
  UINT32                            AhciBar
  );

/**
  Program Xhci Port Disable Override

  @param[in] XhciMmioBase         xHCI controller MBAR0 address
  @param[in] Usb2DisabledPorts    Disabled USB2 ports where each port has its disabling bit
  @param[in] Usb3DisabledPorts    Disabled USB3 ports where each port has its disabling bit

  @retval TRUE if platform reset is needed, otherwise FALSE is returned
**/
BOOLEAN
UsbPdoProgramming (
  IN  UINTN   XhciMmioBase,
  IN  UINT32  Usb2DisabledPorts,
  IN  UINT32  Usb3DisabledPorts
  );

#endif
