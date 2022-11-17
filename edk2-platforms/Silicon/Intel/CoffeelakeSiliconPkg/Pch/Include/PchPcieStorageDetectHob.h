/** @file
  Definitions required to create PcieStorageInfoHob

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PCIE_STORAGE_DETECT_HOB_
#define _PCH_PCIE_STORAGE_DETECT_HOB_

#include "PchLimits.h"

#define PCIE_STORAGE_INFO_HOB_REVISION              1

extern EFI_GUID gPchPcieStorageDetectHobGuid;

typedef enum {
  RstLinkWidthX1 = 1,
  RstLinkWidthX2 = 2,
  RstLinkWidthX4 = 4
} RST_LINK_WIDTH;

//
//  Stores information about connected PCIe storage devices used later by BIOS setup and RST remapping
//
#pragma pack(1)
typedef struct {
  UINT8  Revision;

  //
  // Stores the number of root ports occupied by a connected storage device, values from RST_LINK_WIDTH are supported
  //
  UINT8  PcieStorageLinkWidth[PCH_MAX_PCIE_ROOT_PORTS];

  //
  // Programming interface value for a given device, 0x02 - NVMe or RAID, 0x1 - AHCI storage, 0x0 - no device connected
  //
  UINT8  PcieStorageProgrammingInterface[PCH_MAX_PCIE_ROOT_PORTS];

  //
  // Stores information about cycle router number under a given PCIe controller
  //
  UINT8  RstCycleRouterMap[PCH_MAX_PCIE_CONTROLLERS];
} PCIE_STORAGE_INFO_HOB;
#pragma pack()
#endif
