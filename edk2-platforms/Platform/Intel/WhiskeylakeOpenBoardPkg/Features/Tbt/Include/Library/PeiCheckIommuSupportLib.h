/** @file
  Header file for the PeiCheckIommuSupport library.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_CHECK_IOMMU_SUPPORT_LIBRARY_H_
#define _PEI_CHECK_IOMMU_SUPPORT_LIBRARY_H_

/**
  Check Iommu Ability base on SKU type, CSME FW type, Vtd and setup options.
**/
VOID
PeiCheckIommuSupport (
  VOID
  );

#endif // _PEI_CHECK_IOMMU_SUPPORT_LIBRARY_H_

