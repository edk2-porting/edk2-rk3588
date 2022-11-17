/** @file
  Header file for CpuPcieRpLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PCIERP_LIB_H_
#define _CPU_PCIERP_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/HobLib.h>

#pragma pack(1)

typedef struct {
  UINT8   Segment;
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  BOOLEAN Enable;
} CPU_PCIE_RP_INFO;

#pragma pack()

/**
  Determines whether PCIe link is active

  @param[in] RpBase    Root Port base address
  @retval Link Active state
**/
BOOLEAN
CpuPcieIsLinkActive (
  UINT64  RpBase
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port pci segment base address
  @return    Max link speed
**/
UINT32
CpuPcieGetMaxLinkSpeed (
  UINT64 RpBase
  );

#endif // _CPU_PCIERP_LIB_H_
