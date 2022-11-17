/** @file
  DRAM info PPI to retrieve DRAM information from lower level firmware

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  _SYNQUACER_DRAMINFO_PPI_
#define  _SYNQUACER_DRAMINFO_PPI_

#define SYNQUACER_DRAMINFO_PPI_GUID \
  { 0x3e1d7356, 0xdda4, 0x4b1a, { 0x93, 0x46, 0xbf, 0x89, 0x1c, 0x86, 0x46, 0xcc } }

/**
  Retrieve the number of discontiguous DRAM regions

  @param[out] RegionCount       The number of available DRAM regions

  @retval EFI_SUCCESS           The data was successfully returned.
  @retval EFI_INVALID_PARAMETER RegionCount == NULL

**/
typedef
EFI_STATUS
(EFIAPI * DRAMINFO_GET_REGION_COUNT) (
  OUT   UINTN                 *RegionCount
  );

/**
  Retrieve the base and size of a DRAM region

  @param[in]  RegionIndex       The 0-based index of the region to retrieve
  @param[out] Base              The base of the requested region
  @param[out] Size              The size of the requested region

  @retval EFI_SUCCESS           The data was successfully returned.
  @retval EFI_INVALID_PARAMETER Base == NULL or Size == NULL
  @retval EFI_NOT_FOUND         No region exists with index >= RegionIndex

**/
typedef
EFI_STATUS
(EFIAPI * DRAMINFO_GET_REGION) (
  IN    UINTN                 RegionIndex,
  OUT   UINT64                *Base,
  OUT   UINT64                *Size
  );

typedef struct {
  DRAMINFO_GET_REGION_COUNT   GetRegionCount;
  DRAMINFO_GET_REGION         GetRegion;
} SYNQUACER_DRAM_INFO_PPI;

extern EFI_GUID gSynQuacerDramInfoPpiGuid;

#endif
