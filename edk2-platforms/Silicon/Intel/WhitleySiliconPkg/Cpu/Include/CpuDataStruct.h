/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_DATA_STRUCT_H
#define _CPU_DATA_STRUCT_H

#define inline          __inline

#ifndef MAX_SOCKET
#define MAX_SOCKET (FixedPcdGet32 (PcdMaxCpuSocketCount))
#endif

#ifndef MAX_CORE
#define MAX_CORE (FixedPcdGet32 (PcdMaxCpuCoreCount))
#endif

//
// Total TDP levels for Config TDP + Speed Select (ISS/SST)
//
#define CONFIG_TDP_TOTAL_LEVEL  5

#endif
