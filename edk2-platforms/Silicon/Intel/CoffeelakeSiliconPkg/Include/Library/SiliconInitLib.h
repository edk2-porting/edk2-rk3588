/** @file

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SILICON_INIT_LIB_H_
#define _SILICON_INIT_LIB_H_

#include <PiPei.h>

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