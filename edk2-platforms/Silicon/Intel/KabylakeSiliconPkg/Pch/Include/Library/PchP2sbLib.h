/** @file
  Header file for PchP2sbLib.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_P2SB_LIB_H_
#define _PCH_P2SB_LIB_H_

/**
  Get P2SB pci configuration register.
  It returns register at Offset of P2SB controller and size in 4bytes.
  The Offset should not exceed 255 and must be aligned with size.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[in]  Offset                    Register offset of P2SB controller.
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchP2sbCfgGet32 (
  IN  UINTN                             Offset,
  OUT UINT32                            *OutData
  );

/**
  Get P2SB pci configuration register.
  It returns register at Offset of P2SB controller and size in 2bytes.
  The Offset should not exceed 255 and must be aligned with size.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[in]  Offset                    Register offset of P2SB controller.
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchP2sbCfgGet16 (
  IN  UINTN                             Offset,
  OUT UINT16                            *OutData
  );

/**
  Get P2SB pci configuration register.
  It returns register at Offset of P2SB controller and size in 1byte.
  The Offset should not exceed 255 and must be aligned with size.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[in]  Offset                    Register offset of P2SB controller.
  @param[out] OutData                   Buffer of Output Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchP2sbCfgGet8 (
  IN  UINTN                             Offset,
  OUT UINT8                             *OutData
  );

/**
  Set P2SB pci configuration register.
  It programs register at Offset of P2SB controller and size in 4bytes.
  The Offset should not exceed 255 and must be aligned with size.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[in]  Offset                    Register offset of P2SB controller.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchP2sbCfgSet32 (
  IN  UINTN                             Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Set P2SB pci configuration register.
  It programs register at Offset of P2SB controller and size in 2bytes.
  The Offset should not exceed 255 and must be aligned with size.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[in]  Offset                    Register offset of P2SB controller.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchP2sbCfgSet16 (
  IN  UINTN                             Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  );

/**
  Set P2SB pci configuration register.
  It programs register at Offset of P2SB controller and size in 1bytes.
  The Offset should not exceed 255 and must be aligned with size.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[in]  Offset                    Register offset of P2SB controller.
  @param[in]  AndData                   AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                    OR Data. Must be the same size as Size parameter.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchP2sbCfgSet8 (
  IN  UINTN                             Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  );

/**
  Hide P2SB device.

  @param[in]  P2sbBase                  Pci base address of P2SB controller.

  @retval EFI_SUCCESS                   Always return success.
**/
EFI_STATUS
PchHideP2sb (
  IN  UINTN                             P2sbBase
  );

/**
  Reveal P2SB device.
  Also return the original P2SB status which is for Hidding P2SB or not after.
  If OrgStatus is not NULL, then TRUE means P2SB is unhidden,
  and FALSE means P2SB is hidden originally.

  @param[in]  P2sbBase                  Pci base address of P2SB controller.
  @param[out] OrgStatus                 Original P2SB hidding/unhidden status

  @retval EFI_SUCCESS                   Always return success.
**/
EFI_STATUS
PchRevealP2sb (
  IN  UINTN                             P2sbBase,
  OUT BOOLEAN                           *OrgStatus
  );

#endif // _PCH_P2SB_LIB_H_
