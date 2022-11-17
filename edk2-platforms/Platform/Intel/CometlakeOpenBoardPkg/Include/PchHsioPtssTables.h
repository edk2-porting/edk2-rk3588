/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef PCH_HSIO_PTSSTABLES_H_
#define PCH_HSIO_PTSSTABLES_H_

#include <PchAccess.h>

///
/// SATA PTSS Topology Types
///
typedef enum {
  PchSataTopoUnknown = 0x00,
  PchSataTopoIsata,
  PchSataTopoDirectConnect,
  PchSataTopoFlex,
  PchSataTopoM2
} PCH_SATA_TOPOLOGY;

///
/// PCIe PTSS Topology Types
///
typedef enum {
  PchPcieTopoUnknown = 0x00,
  PchPcieTopox1,
  PchPcieTopox4,
  PchPcieTopoSataE,
  PchPcieTopoM2
} PCH_PCIE_TOPOLOGY;

///
/// The PCH_SBI_PTSS_HSIO_TABLE block describes HSIO PTSS settings for PCH.
///
typedef struct {
  UINT8       LaneNum;
  UINT8       PhyMode;
  UINT16      Offset;
  UINT32      Value;
  UINT32      BitMask;
} PCH_SBI_PTSS_HSIO_TABLE;

typedef struct {
  PCH_SBI_PTSS_HSIO_TABLE   PtssTable;
  UINT16                    Topology;
} HSIO_PTSS_TABLES;

#endif // PCH_HSIO_PTSSTABLES_H_

