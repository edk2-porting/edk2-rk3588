/** @file
  Private header for PCH Info Lib.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

/**
  Structure for PCH SKU string mapping
**/
struct PCH_SKU_STRING {
  UINT16        Id;
  CHAR8         *String;
};

extern struct PCH_SKU_STRING mSkuStrs[];

/**
  Determine Pch Series based on Device Id

  @param[in] LpcDeviceId      Lpc Device Id

  @retval PCH_SERIES          Pch Series
**/
PCH_SERIES
PchSeriesFromLpcDid (
  IN UINT16 LpcDeviceId
  );

/**
Determine Pch Generation based on Device Id

@param[in] LpcDeviceId            Lpc Device Id

@retval PCH_GENERATION            Pch Generation
**/
PCH_GENERATION
PchGenerationFromDid (
  IN UINT16 LpcDeviceId
  );

