/** @file

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// This defines the maximum number of cores supported by some modules.
// It is generally better to use a dynamic solution.
// This is also defined by build tools for some special build
// environments used in validation that do not support EDK II build
// and thus can't use PCD.
//

#ifndef MAX_CORE
#define MAX_CORE (FixedPcdGet32 (PcdMaxCpuCoreCount))
#endif

