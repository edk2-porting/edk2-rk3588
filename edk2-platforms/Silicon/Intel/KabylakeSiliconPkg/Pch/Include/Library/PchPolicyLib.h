/** @file
  Prototype of the PeiPchPolicy library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PEI_PCH_POLICY_LIB_H_
#define _PEI_PCH_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print whole PCH_PREMEM_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
PchPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  );

/**
  Print whole SI_POLICY_PPI and serial out.

  @param[in] SiPolicyPpi               The RC Policy PPI instance
**/
VOID
EFIAPI
PchPrintPolicyPpi (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

/**
  Get PCH PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
PchGetPreMemConfigBlockTotalSize (
  VOID
  );

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
PchGetConfigBlockTotalSize (
  VOID
  );

/**
  PchAddPreMemConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  PchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/*
  Apply sample board PCH specific default settings

  @param[in] SiPreMemPolicy       The pointer to SI PREMEM Policy PPI instance
*/
VOID
EFIAPI
PchLoadSamplePreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicy
  );

/*
  Apply sample board PCH specific default settings

  @param[in] SiPolicyPpi      The pointer to SI Policy PPI instance
*/
VOID
EFIAPI
PchLoadSamplePolicy (
  IN  SI_POLICY_PPI           *SiPolicy
  );

#endif // _PEI_PCH_POLICY_LIB_H_
