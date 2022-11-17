/** @file
  Header file for the DxeCheckIommuSupport library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_CHECK_IOMMU_SUPPORT_LIBRARY_H_
#define _DXE_CHECK_IOMMU_SUPPORT_LIBRARY_H_

/**
  Detect ME FW and Board Type and return the result via IommuSkuCheck.

  IommuSkuCheck
  BIT0: Indicate system has a Corporate CSME firmware
  BIT1: Indicate BIOS is running on a CML RVP
  BIT2: Indicate BIOS is running on a CFL-H RVP
  BIT3: Indicate BIOS is running on a CFL-S 8+2 RVP

  @retval Return 0 means not support, otherwise value is defined by IommuSkuCheck
**/
UINT8
DetectMeAndBoard (
  VOID
  );

/**
  DxeCheckIommuSupport

  Only WHL/CFL-H/CFL-S 8+2 Crop SKUs support Iommu.
  This function will save sku information to PcdIommuSkuCheck.
  BIOS will use PcdIommuSkuCheck and other factors to set PcdVTdPolicyPropertyMask on the next boot in PEI phase

  This function might perform a system reset.
**/
EFI_STATUS
EFIAPI
DxeCheckIommuSupport (
  VOID
  );
#endif // _DXE_CHECK_IOMMU_SUPPORT_LIBRARY_H_

