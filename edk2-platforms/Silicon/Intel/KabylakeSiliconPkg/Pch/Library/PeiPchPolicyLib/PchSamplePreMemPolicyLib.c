/** @file
  This file is to load sample board policy.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiPchPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/*
  Apply sample board PCH specific default settings

  @param[in] SiPreMemPolicy      The pointer to SI PREMEM Policy PPI instance
*/
VOID
EFIAPI
PchLoadSamplePreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicy
  )
{
  EFI_STATUS                      Status;
  PCH_TRACE_HUB_PREMEM_CONFIG     *TraceHubPreMemConfig;
  PCH_HSIO_PCIE_PREMEM_CONFIG     *HsioPciePreMemConfig;
  PCH_HSIO_SATA_PREMEM_CONFIG     *HsioSataPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioSataPreMemConfigGuid, (VOID *) &HsioSataPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // TRACEHUB
  //
  TraceHubPreMemConfig->MemReg0Size = 0x100000;  // 1MB
  TraceHubPreMemConfig->MemReg1Size = 0x100000;  // 1MB

  //
  // HSIO PCIE
  //
  HsioPciePreMemConfig->Lane[0].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[0].HsioRxSetCtle = 6;
  HsioPciePreMemConfig->Lane[1].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[1].HsioRxSetCtle = 6;
  HsioPciePreMemConfig->Lane[2].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[2].HsioRxSetCtle = 6;
  HsioPciePreMemConfig->Lane[3].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[3].HsioRxSetCtle = 6;
  HsioPciePreMemConfig->Lane[5].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[5].HsioRxSetCtle = 8;
  HsioPciePreMemConfig->Lane[7].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[7].HsioRxSetCtle = 8;
  HsioPciePreMemConfig->Lane[8].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[8].HsioRxSetCtle = 8;
  HsioPciePreMemConfig->Lane[9].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[9].HsioRxSetCtle = 8;
  HsioPciePreMemConfig->Lane[10].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[10].HsioRxSetCtle = 8;
  HsioPciePreMemConfig->Lane[11].HsioRxSetCtleEnable = TRUE;
  HsioPciePreMemConfig->Lane[11].HsioRxSetCtle = 8;
  HsioPciePreMemConfig->PciePllSsc = 0xFF;

  //
  // HSIO SATA
  //
  HsioSataPreMemConfig->PortLane[0].HsioRxGen3EqBoostMagEnable = TRUE;
  HsioSataPreMemConfig->PortLane[0].HsioRxGen3EqBoostMag = 4;
  HsioSataPreMemConfig->PortLane[0].HsioTxGen1DownscaleAmpEnable = TRUE;
  HsioSataPreMemConfig->PortLane[0].HsioTxGen1DownscaleAmp = 0x2C;
  HsioSataPreMemConfig->PortLane[0].HsioTxGen2DownscaleAmpEnable = 0;
  HsioSataPreMemConfig->PortLane[0].HsioTxGen2DownscaleAmp = 0x38;

}
