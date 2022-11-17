/** @file

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_HSIO_PTSSTABLES_H_
#define _PCH_HSIO_PTSSTABLES_H_

#include <Platform.h>
#include <Ppi/PchHsioPtssTable.h>
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
/// DMI PTSS Topology Types
///
typedef enum {
  PchDmiTopoUnknown = 0x00,
} PCH_DMI_TOPOLOGY;

typedef struct {
  PCH_SBI_PTSS_HSIO_TABLE   PtssTable;
  UINT16                    Topology;
  UINT16                    BoardId;
} HSIO_PTSS_TABLES;

#endif // _PCH_HSIO_PTSSTABLES_H_
