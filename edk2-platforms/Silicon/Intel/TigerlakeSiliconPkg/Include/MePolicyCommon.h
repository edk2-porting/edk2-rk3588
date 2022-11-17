/** @file
  Definition for ME common policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _ME_POLICY_COMMON_H_
#define _ME_POLICY_COMMON_H_

#include <ConfigBlock.h>

#include <MePeiConfig.h>

#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif
#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif

#endif // _ME_POLICY_COMMON_H_
