/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SILICON_INIT_LIB_H_
#define _SILICON_INIT_LIB_H_

///@todo it should be moved to Si Pkg.

VOID
EarlySiliconInit (
  VOID
  );

VOID
SiliconInit (
  VOID
  );

VOID
LateSiliconInit (
  VOID
  );

#endif