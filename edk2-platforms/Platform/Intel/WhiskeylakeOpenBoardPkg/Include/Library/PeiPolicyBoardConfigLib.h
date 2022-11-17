/** @file
  Header file for the PeiPolicyBoardConfig Library.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_POLICY_BOARD_CONFIG_LIB_H_
#define _PEI_POLICY_BOARD_CONFIG_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  This function performs PEI CPU Pre-Memory Policy update by board configuration.

  @param[in, out] SiPolicy        The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfigPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  This function performs PEI ME Pre-Memory Policy update by board configuration.

  @param[in, out] SiPolicy        The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyBoardConfigPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  This function performs PEI PCH Pre-Memory Policy update by board configuration.

  @param[in, out] SiPolicy        The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfigPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  This function performs PEI SA Pre-Memory Policy update by board configuration.

  @param[in, out] SiPolicy        The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfigPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  This function performs PEI CPU Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  );

/**
  This function performs PEI ME Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  );

/**
  This function performs PEI PCH Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  );

/**
  This function performs PEI SA Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  );

/**
  This function performs PEI SI Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  );

#endif

