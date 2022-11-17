/** @file
  This file defines the function to initialize default silicon policy PPI.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI_H_
#define _PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI_H_

//
// Forward declaration for the PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI.
//
typedef struct _PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI;

/**
  Initialize and install default silicon policy PPI
**/
typedef
EFI_STATUS
(EFIAPI *PEI_PREMEM_POLICY_INIT) (
  VOID
  );

///
/// This PPI provides function to install default silicon policy
///
struct _PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI {
  PEI_PREMEM_POLICY_INIT   PeiPreMemPolicyInit;   ///< PeiPreMemPolicyInit()
};

extern EFI_GUID gSiPreMemDefaultPolicyInitPpiGuid;

#endif // _PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI_H_
