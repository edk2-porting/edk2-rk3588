/** @file
  This file defines the PPI function for installing PostMem silicon policy
  PPI with default settings.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_SI_DEFAULT_POLICY_INIT_PPI_H_
#define _PEI_SI_DEFAULT_POLICY_INIT_PPI_H_

//
// Forward declaration for the PEI_SI_DEFAULT_POLICY_INIT_PPI.
//
typedef struct _PEI_SI_DEFAULT_POLICY_INIT_PPI PEI_SI_DEFAULT_POLICY_INIT_PPI;

/**
  Initialize and install default silicon policy PPI
**/
typedef
EFI_STATUS
(EFIAPI *PEI_POLICY_INIT) (
  VOID
  );

///
/// This PPI provides function to install default silicon policy
///
struct _PEI_SI_DEFAULT_POLICY_INIT_PPI {
  PEI_POLICY_INIT   PeiPolicyInit;
};

extern EFI_GUID gSiDefaultPolicyInitPpiGuid;

#endif // _PEI_SI_DEFAULT_POLICY_INIT_PPI_H_
