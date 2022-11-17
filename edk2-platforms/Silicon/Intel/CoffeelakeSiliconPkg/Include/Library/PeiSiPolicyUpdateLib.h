/** @file
  Header file for PEI SiPolicyUpdate Library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_SI_POLICY_UPDATE_LIB_H_
#define _PEI_SI_POLICY_UPDATE_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  This function performs Silicon PEI Policy initialization.

  @param[in, out] SiPolicy The Silicon Policy PPI instance

  @retval EFI_SUCCESS      The function completed successfully
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicy (
  IN OUT SI_POLICY_PPI *SiPolicy
  );

/**
  This function performs CPU PEI Policy initialization in Post-memory.

  @param[in, out] SiPolicyPpi     The SI Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicy (
  IN OUT  SI_POLICY_PPI *SiPolicyPpi
  );

/**
  This function performs SI PEI Policy initialization.

  @param[in, out] SiPolicyPpi     The SA Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicy (
  IN OUT   SI_POLICY_PPI  *SiPolicyPpi
  );


/**
This function performs SA PEI Policy initialization for PreMem.

@param[in, out] SiPreMemPolicyPpi   The SI PreMem Policy PPI instance

@retval EFI_SUCCESS             Update complete.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyPreMem (
IN OUT   SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
);

/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  IN OUT  SI_POLICY_PPI         *SiPolicy
  );

/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPreMemPolicy  The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  Update the ME Policy Library

  @param[in, out] SiPolicy       The SI Policy PPI instance

  @retval EFI_SUCCESS            Update complete.
**/
EFI_STATUS
UpdatePeiMePolicy (
  IN OUT  SI_POLICY_PPI         *SiPolicy
  );

/**
  Update the ME Policy Library

  @param[in, out] SiPreMemPolicy The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS            Update complete.
**/
EFI_STATUS
UpdatePeiMePolicyPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

#endif
