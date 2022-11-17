/** @file
  This file contains PEI DMI methods

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_PCH_DMI_LIB_H_
#define _PEI_PCH_DMI_LIB_H_

#include <Ppi/SiPolicy.h>

//
// Data structure definitions
//
typedef enum {
  DmiVcTypeVc0,
  DmiVcTypeVc1,
  DmiVcTypeVcm,
  DmiVcTypeMax
} PCH_DMI_VC_TYPE;


#endif
