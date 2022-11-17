/** @file
  This file is part of the IGD OpRegion Implementation.  The IGD OpRegion is
  an interface between system BIOS, ASL code, and Graphics drivers.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _IGD_OPREGION_PROTOCOL_H_
#define _IGD_OPREGION_PROTOCOL_H_

#include <IndustryStandard/IgdOpRegion.h>

extern EFI_GUID gIgdOpRegionProtocolGuid;

///
/// IGD OpRegion Protocol
///
typedef struct {
  IGD_OPREGION_STRUCTURE  *OpRegion; ///< IGD Operation Region Structure
} IGD_OPREGION_PROTOCOL;

#endif
