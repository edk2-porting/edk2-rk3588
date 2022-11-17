/** @file
  Header file for VtdInfoLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _VTD_INFO_LIB_H_
#define _VTD_INFO_LIB_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/HobLib.h>

#define VTD_ENGINE_NUMBER             7

#pragma pack(1)

/**
  Get VTD Engine Base Address from PCD values.

  @param[in]  VtdEngineNumber        - Engine number for which VTD Base Adderess is required.

  @retval   VTD Engine Base Address
**/
UINT32
GetVtdBaseAddress (
  IN UINT8        VtdEngineNumber
  );

/**
  Read VTD Engine Base Address from VTD BAR Offsets.

  @param[in]  VtdEngineNumber        - Engine number for which VTD Base Adderess is required.

  @retval   VTD Engine Base Address
**/
UINT32
ReadVtdBaseAddress (
  IN UINT8        VtdEngineNumber
  );

/**
  GetMaxVtdEngineNumber: Get Maximum Vtd Engine Number

  @retval Vtd Engine Number
**/
UINT8
GetMaxVtdEngineNumber(
  VOID
);

#pragma pack()
#endif // _VTD_INFO_LIB_H_
