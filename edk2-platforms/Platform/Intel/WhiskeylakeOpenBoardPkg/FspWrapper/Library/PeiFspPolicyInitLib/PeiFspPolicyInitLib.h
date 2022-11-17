/** @file
  Internal header file for Fsp Policy Initialization Library.


  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_FSP_POLICY_INIT_LIB_H_
#define _PEI_FSP_POLICY_INIT_LIB_H_

#include <PiPei.h>

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>

#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>
#include <Setup.h>

/**
  Performs FSP SI PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSiPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP SI PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSiPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  );

/**
  Performs FSP PCH PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyInit (
  IN OUT FSPS_UPD     *FspsUpd
  );

/**
  Performs FSP CPU PEI Policy initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspCpuPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
Performs FSP Security PEI Policy initialization.

@param[in][out]  FspmUpd             Pointer to FSP UPD Data.

@retval          EFI_SUCCESS         FSP UPD Data is updated.
@retval          EFI_NOT_FOUND       Fail to locate required PPI.
@retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSecurityPolicyInitPreMem(
IN OUT FSPM_UPD    *FspmUpd
);

/**
  Performs FSP ME PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspMePolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP ME PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspMePolicyInit (
  IN OUT FSPS_UPD     *FspsUpd
  );

/**
  Performs FSP SA PEI Policy initialization in pre-memory.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP SA PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  );

/**
  Performs FSP CPU PEI Policy post memory initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspCpuPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  );

/**
Performs FSP Security PEI Policy post memory initialization.

@param[in][out]  FspsUpd             Pointer to FSP UPD Data.

@retval          EFI_SUCCESS         FSP UPD Data is updated.
@retval          EFI_NOT_FOUND       Fail to locate required PPI.
@retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSecurityPolicyInit(
IN OUT FSPS_UPD    *FspsUpd
);

/**
  PeiGetSectionFromFv finds the file in FV and gets file Address and Size

  @param[in] NameGuid              - File GUID
  @param[out] Address              - Pointer to the File Address
  @param[out] Size                 - Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32               *Size
  );

/**
  Performs FSP Misc UPD initialization.

  @param[in][out]  FspmUpd             Pointer to FSPM_UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
**/
EFI_STATUS
EFIAPI
PeiFspMiscUpdInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

#endif // _PEI_FSP_POLICY_INIT_LIB_H_

