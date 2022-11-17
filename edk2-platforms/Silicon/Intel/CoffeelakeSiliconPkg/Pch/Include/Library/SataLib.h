/** @file
  Header file for PchSataLib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SATA_LIB_H_
#define _PCH_SATA_LIB_H_

#define SATA_1_CONTROLLER_INDEX             0
#define SATA_2_CONTROLLER_INDEX             1
#define SATA_3_CONTROLLER_INDEX             2

/**
  Get Pch Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval Pch Maximum Sata Port Number
**/
UINT8
GetPchMaxSataPortNum (
  IN UINT32     SataCtrlIndex
  );

/**
  Gets Maximum Sata Controller Number

  @param[in] None

  @retval Maximum Sata Controller Number
**/
UINT8
GetPchMaxSataControllerNum (
  VOID
  );

/**
  Gets SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
GetSataPcieDeviceNum (
  IN UINT32 SataCtrlIndex
  );

/**
  Gets SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
GetSataPcieFunctionNum (
  IN UINT32 SataCtrlIndex
  );

/**
  Gets SATA controller PCIe config space base address

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe config space base address
**/
UINT64
GetSataRegBase (
  IN UINT32 SataCtrlIndex
  );

#endif // _PCH_SATA_LIB_H_
