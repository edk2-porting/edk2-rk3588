/** @file
  Helper functions for PCH SMM

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef PCH_SMM_HELPERS_H
#define PCH_SMM_HELPERS_H

#include "PchSmm.h"
#include "PchxSmmHelpers.h"
//
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SUPPORT / HELPER FUNCTIONS (PCH version-independent)
//

/**
  Publish SMI Dispatch protocols.


**/
VOID
PchSmmPublishDispatchProtocols (
  VOID
  );

/**
  Compare 2 SMM source descriptors' enable settings.

  @param[in] Src1                 Pointer to the PCH SMI source description table 1
  @param[in] Src2                 Pointer to the PCH SMI source description table 2

  @retval TRUE                    The enable settings of the 2 SMM source descriptors are identical.
  @retval FALSE                   The enable settings of the 2 SMM source descriptors are not identical.
**/
BOOLEAN
CompareEnables (
  CONST IN PCH_SMM_SOURCE_DESC *Src1,
  CONST IN PCH_SMM_SOURCE_DESC *Src2
  );

/**
  Compare a bit descriptor to the enables of source descriptor. Includes null address type.

  @param[in] BitDesc              Pointer to the PCH SMI bit descriptor
  @param[in] Src                  Pointer to the PCH SMI source description table 2

  @retval TRUE                    The bit desc is equal to any of the enables in source descriptor
  @retval FALSE                   The bid desc is not equal to all of the enables in source descriptor
**/
BOOLEAN
IsBitEqualToAnySourceEn (
  CONST IN PCH_SMM_BIT_DESC    *BitDesc,
  CONST IN PCH_SMM_SOURCE_DESC *Src
  );

/**
  Compare 2 SMM source descriptors' statuses.

  @param[in] Src1                 Pointer to the PCH SMI source description table 1
  @param[in] Src2                 Pointer to the PCH SMI source description table 2

  @retval TRUE                    The statuses of the 2 SMM source descriptors are identical.
  @retval FALSE                   The statuses of the 2 SMM source descriptors are not identical.
**/
BOOLEAN
CompareStatuses (
  CONST IN PCH_SMM_SOURCE_DESC *Src1,
  CONST IN PCH_SMM_SOURCE_DESC *Src2
  );

/**
  Compare 2 SMM source descriptors, based on Enable settings and Status settings of them.

  @param[in] Src1                 Pointer to the PCH SMI source description table 1
  @param[in] Src2                 Pointer to the PCH SMI source description table 2

  @retval TRUE                    The 2 SMM source descriptors are identical.
  @retval FALSE                   The 2 SMM source descriptors are not identical.
**/
BOOLEAN
CompareSources (
  CONST IN PCH_SMM_SOURCE_DESC *Src1,
  CONST IN PCH_SMM_SOURCE_DESC *Src2
  );

/**
  Check if an SMM source is active.

  @param[in] Src                  Pointer to the PCH SMI source description table
  @param[in] SciEn                Indicate if SCI is enabled or not
  @param[in] SmiEnValue           Value from R_PCH_SMI_EN
  @param[in] SmiStsValue          Value from R_PCH_SMI_STS

  @retval TRUE                    It is active.
  @retval FALSE                   It is inactive.
**/
BOOLEAN
SourceIsActive (
  CONST IN PCH_SMM_SOURCE_DESC  *Src,
  CONST IN BOOLEAN              SciEn,
  CONST IN UINT32               SmiEnValue,
  CONST IN UINT32               SmiStsValue
  );

/**
  Enable the SMI source event by set the SMI enable bit, this function would also clear SMI
  status bit to make initial state is correct

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmEnableSource (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  );

/**
  Disable the SMI source event by clear the SMI enable bit

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmDisableSource (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  );

/**
  Clear the SMI status bit by set the source bit of SMI status register

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmClearSource (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  );

/**
  Sets the source to a 1 and then waits for it to clear.
  Be very careful when calling this function -- it will not
  ASSERT.  An acceptable case to call the function is when
  waiting for the NEWCENTURY_STS bit to clear (which takes
  3 RTCCLKs).

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmClearSourceAndBlock (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  );

#endif
