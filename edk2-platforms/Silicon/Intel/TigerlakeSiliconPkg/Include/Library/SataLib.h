/** @file
  Header file for SataLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SATA_LIB_H_
#define _SATA_LIB_H_

#define SATA_1_CONTROLLER_INDEX             0
#define SATA_2_CONTROLLER_INDEX             1
#define SATA_3_CONTROLLER_INDEX             2

/**
  Get Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Maximum Sata Port Number
**/
UINT8
MaxSataPortNum (
  IN UINT32     SataCtrlIndex
  );

/**
  Gets Maximum Sata Controller Number

  @retval Maximum Sata Controller Number
**/
UINT8
MaxSataControllerNum (
  VOID
  );

/**
  Get SATA controller's Port Present Status

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Port Present Status
**/
UINT8
GetSataPortPresentStatus (
  IN UINT32  SataCtrlIndex
  );

/**
  Get SATA controller Function Disable Status

  @param[in]  SataCtrlIndex       SATA controller index

  @retval 0 SATA Controller is not Function Disabled
  @retval 1 SATA Controller is Function Disabled
**/
BOOLEAN
SataControllerFunctionDisableStatus (
  IN UINT32  SataCtrlIndex
  );

/**
  Get SATA controller ABAR size

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller ABAR size
**/
UINT32
GetSataAbarSize (
  IN UINT32  SataCtrlIndex
  );

/**
  Get SATA controller AHCI base address

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller AHCI base address
**/
UINT32
GetSataAhciBase (
  IN UINT32  SataCtrlIndex
  );

/**
  Check if SATA controller supports RST remapping

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     TRUE                Controller supports remapping
  @retval     FALSE               Controller does not support remapping
**/
BOOLEAN
IsRemappingSupportedOnSata (
  IN UINT32  SataCtrlIndex
  );

/**
  Checks if SoC supports the SATA PGD power down on given
  SATA controller.

  @param[in] SataCtrlIndex  SATA controller index

  @retval TRUE   SATA PGD power down supported
  @retval FALSE  SATA PGD power down not supported
**/
BOOLEAN
IsSataPowerGatingSupported (
  IN UINT32 SataCtrlIndex
  );

#endif // _SATA_LIB_H_
