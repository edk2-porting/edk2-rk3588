/** @file
  PCH configuration based on PCH policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PREMEM_POLICY_COMMON_H_
#define _PCH_PREMEM_POLICY_COMMON_H_

#include <ConfigBlock.h>

#include "PchLimits.h"
#include "ConfigBlock/PchGeneralConfig.h"
#include "ConfigBlock/DciConfig.h"
#include "ConfigBlock/WatchDogConfig.h"
#include "ConfigBlock/PchTraceHubConfig.h"
#include "ConfigBlock/SmbusConfig.h"
#include "ConfigBlock/LpcConfig.h"
#include "ConfigBlock/HsioPcieConfig.h"
#include "ConfigBlock/HsioSataConfig.h"
#include "ConfigBlock/HsioConfig.h"

#pragma pack (push,1)

#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif

/**
  PCH Policy revision number
  Any backwards compatible changes to this structure will result in an update in the revision number
**/
#define PCH_PREMEM_POLICY_REVISION  1

/**
  PCH Policy PPI\n
  All PCH config block change history will be listed here\n\n

  - <b>Revision 1</b>:
    - Initial version.\n
**/
typedef struct _PCH_PREMEM_POLICY {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} PCH_PREMEM_POLICY;

#pragma pack (pop)

#endif // _PCH_PREMEM_POLICY_COMMON_H_
