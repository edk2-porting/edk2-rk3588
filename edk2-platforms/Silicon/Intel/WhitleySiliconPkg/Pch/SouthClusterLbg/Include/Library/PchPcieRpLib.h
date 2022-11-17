/** @file
  Header file for PchPcieRpLib.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PCIERP_LIB_H_
#define _PCH_PCIERP_LIB_H_

/**
  PCIe controller bifurcation configuration.
**/
typedef enum {
  PcieBifurcation4x1      = 0,
  PcieBifurcation1x2_2x1  = 1,
  PcieBifurcation2x2      = 2,
  PcieBifurcation1x4      = 3,
  PcieUnknownConfig
} PCIE_BIFURCATION_CONFIG;

typedef struct {
  UINT8 DevNum;
  UINT8 Pid;
  UINT8 RpNumBase;
} PCH_PCIE_CONTROLLER_INFO;

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  PchId               The PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)
  @param[in]  RpNumber            Root port physical number. (0-based)
  @param[out] RpDev               Return corresponding root port device number.
  @param[out] RpFun               Return corresponding root port function number.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFunByPchId (
  IN  UINT8   PchId,
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber            Root port physical number. (0-based)
  @param[out] RpDev               Return corresponding root port device number.
  @param[out] RpFun               Return corresponding root port function number.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  Get Root Port physical Number by Pch Pcie Root Port Device and Function Number

  @param[in]  PchId                 The PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)
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
  IN  UINT8   PchId,
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  );

/**
  Gets base address of PCIe root port.

  @param[in]  PchId               The PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)
  @param RpIndex    Root Port Index (0 based)
  @return PCIe port base address.
**/
UINTN
PchPcieBase (
  IN  UINT8    PchId,
  IN  UINT32   RpIndex
  );

/**
  Determines whether L0s is supported on current stepping.

  @return TRUE if L0s is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieL0sSupported (
  VOID
  );

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
  );

/**
  Returns the maximum value of the "Payload" parameter of all available PchPcie Ports.

  @param[in]  PchId               The PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)
  @retval INT16     The value to assign in field max payload size field
**/
UINT16
PchPcieGetMaxPayloadSizeForAllPortsByPchId (
  IN  UINT8           PchId
  );

/**
  Returns the maximum value of the "Payload" parameter of all available PchPcie Ports.

  @param VOID
  @retval INT16     The value to assign in field max payload size field
**/
UINT16
PchPcieGetMaxPayloadSizeForAllPorts (
  VOID
  );

#endif // _PCH_PCIERP_LIB_H_
