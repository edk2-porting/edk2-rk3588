/** @file
  The DXE CPU PCIE RP Library Implements After Memory PEIM

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/SaPolicy.h>
#include <Protocol/SaNvsArea.h>
#include <Library/PcdLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/BaseMemoryLib.h>
#include <ConfigBlock/PcieDxeConfig.h>
#include <Register/SaRegsHostBridge.h>

/**
Update CPU PCIE RP NVS AREA tables

**/
VOID
UpdateCpuPcieNVS (
  VOID
  )
{
  EFI_STATUS                       Status;
  PCIE_DXE_CONFIG                  *PcieDxeConfig;
  SA_POLICY_PROTOCOL               *SaPolicy;
  SYSTEM_AGENT_NVS_AREA_PROTOCOL   *SaNvsAreaProtocol;

  DEBUG ((DEBUG_INFO, "Update Cpu Pcie NVS Area.\n"));

  Status = gBS->LocateProtocol (&gSaPolicyProtocolGuid, NULL, (VOID **) &SaPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SaPolicy, &gPcieDxeConfigGuid, (VOID *)&PcieDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gSaNvsAreaProtocolGuid, NULL, (VOID **) &SaNvsAreaProtocol);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Locate SA NVS Area failed.\n"));
    return;
  }

  SaNvsAreaProtocol->Area->Peg0LtrEnable  = PcieDxeConfig->PegPwrOpt[0].LtrEnable;
  SaNvsAreaProtocol->Area->Peg0ObffEnable = PcieDxeConfig->PegPwrOpt[0].ObffEnable;
  SaNvsAreaProtocol->Area->Peg1LtrEnable  = PcieDxeConfig->PegPwrOpt[1].LtrEnable;
  SaNvsAreaProtocol->Area->Peg1ObffEnable = PcieDxeConfig->PegPwrOpt[1].ObffEnable;
  SaNvsAreaProtocol->Area->Peg2LtrEnable  = PcieDxeConfig->PegPwrOpt[2].LtrEnable;
  SaNvsAreaProtocol->Area->Peg2ObffEnable = PcieDxeConfig->PegPwrOpt[2].ObffEnable;
  SaNvsAreaProtocol->Area->Peg3LtrEnable  = PcieDxeConfig->PegPwrOpt[3].LtrEnable;
  SaNvsAreaProtocol->Area->Peg3ObffEnable = PcieDxeConfig->PegPwrOpt[3].ObffEnable;
  SaNvsAreaProtocol->Area->PegLtrMaxSnoopLatency = V_SA_LTR_MAX_SNOOP_LATENCY_VALUE;
  SaNvsAreaProtocol->Area->PegLtrMaxNoSnoopLatency = V_SA_LTR_MAX_NON_SNOOP_LATENCY_VALUE;
}
