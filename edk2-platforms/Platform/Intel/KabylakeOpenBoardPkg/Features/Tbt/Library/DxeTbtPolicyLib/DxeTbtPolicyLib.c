/** @file
  This file is DxeTbtPolicyLib library.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <DxeTbtPolicyLibrary.h>
#include <TbtBoardInfo.h>
#include <Protocol/DxeTbtPolicy.h>
#include <Guid/HobList.h>
#include <Library/HobLib.h>


/**
Update Tbt Policy Callback 
Need to add PCDs for setup options
**/

VOID
EFIAPI
UpdateTbtPolicyCallback (
  VOID
  )
{

  EFI_STATUS                     Status;
  DXE_TBT_POLICY_PROTOCOL        *DxeTbtConfig;

  DxeTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "UpdateTbtPolicyCallback\n"));

  Status = gBS->LocateProtocol (
                  &gDxeTbtPolicyProtocolGuid,
                  NULL,
                  (VOID **) &DxeTbtConfig
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " gDxeTbtPolicyProtocolGuid Not installed!!!\n"));
  } else {
    DxeTbtConfig->DTbtResourceConfig.DTbtPcieExtraBusRsvd   = PcdGet8 (PcdDTbtPcieExtraBusRsvd);
    DxeTbtConfig->DTbtResourceConfig.DTbtPcieMemRsvd        = PcdGet16 (PcdDTbtPcieMemRsvd);
    DxeTbtConfig->DTbtResourceConfig.DTbtPcieMemAddrRngMax  = PcdGet8 (PcdDTbtPcieMemAddrRngMax);
    DxeTbtConfig->DTbtResourceConfig.DTbtPciePMemRsvd       = PcdGet16 (PcdDTbtPciePMemRsvd);
    DxeTbtConfig->DTbtResourceConfig.DTbtPciePMemAddrRngMax = PcdGet8 (PcdDTbtPciePMemAddrRngMax);

    DxeTbtConfig->TbtCommonConfig.TbtAspm          = PcdGet8 (PcdDTbtAspm);
    DxeTbtConfig->TbtCommonConfig.TbtHotNotify     = PcdGet8 (PcdDTbtHotNotify);
    DxeTbtConfig->TbtCommonConfig.TbtHotSMI        = PcdGet8 (PcdDTbtHotSMI);
    DxeTbtConfig->TbtCommonConfig.TbtSetClkReq     = PcdGet8 (PcdDTbtSetClkReq);
    DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport = PcdGet8 (PcdDTbtWakeupSupport);
    DxeTbtConfig->TbtCommonConfig.SecurityMode     = PcdGet8 (PcdDTbtSecurityMode);

    DxeTbtConfig->TbtCommonConfig.Gpio5Filter      = PcdGet8 (PcdDTbtGpio5Filter);
    DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch    = PcdGet8 (PcdDTbtAcDcSwitch);

    DxeTbtConfig->TbtCommonConfig.Rtd3Tbt          = PcdGet8 (PcdRtd3Tbt);
    DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay  = PcdGet16 (PcdRtd3TbtOffDelay);
    DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq    = PcdGet8 (PcdRtd3TbtClkReq);
    DxeTbtConfig->TbtCommonConfig.Win10Support    = PcdGet8 (PcdDTbtWin10Support);
  }

  return;
}

/**
  Print DXE TBT Policy
**/
VOID
TbtPrintDxePolicyConfig (
  VOID
  )
{
  EFI_STATUS                       Status;
  DXE_TBT_POLICY_PROTOCOL          *DxeTbtConfig;

  DEBUG ((DEBUG_INFO, "TbtPrintDxePolicyConfig Start\n"));

  DxeTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  Status = gBS->LocateProtocol (
                  &gDxeTbtPolicyProtocolGuid,
                  NULL,
                  (VOID **) &DxeTbtConfig
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " gDxeTbtPolicyProtocolGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);
  //
  // Print DTBT Policy
  //
  DEBUG ((DEBUG_ERROR, " ========================= DXE TBT POLICY ========================= \n"));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig.DTbtPcieExtraBusRsvd = %x\n", DxeTbtConfig->DTbtResourceConfig.DTbtPcieExtraBusRsvd));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig.DTbtPcieMemRsvd = %x\n", DxeTbtConfig->DTbtResourceConfig.DTbtPcieMemRsvd));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig.DTbtPcieMemAddrRngMax = %x\n", DxeTbtConfig->DTbtResourceConfig.DTbtPcieMemAddrRngMax));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig.DTbtPciePMemRsvd = %x\n", DxeTbtConfig->DTbtResourceConfig.DTbtPciePMemRsvd));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig.DTbtPciePMemAddrRngMax = %x\n", DxeTbtConfig->DTbtResourceConfig.DTbtPciePMemAddrRngMax));


  //
  // Print TBT Common Policy
  //
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtAspm = %x\n", DxeTbtConfig->TbtCommonConfig.TbtAspm));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtHotNotify = %x\n", DxeTbtConfig->TbtCommonConfig.TbtHotNotify));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtHotSMI = %x\n", DxeTbtConfig->TbtCommonConfig.TbtHotSMI));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtLtr = %x\n", DxeTbtConfig->TbtCommonConfig.TbtLtr));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtSetClkReq = %x\n", DxeTbtConfig->TbtCommonConfig.TbtSetClkReq));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport = %x\n", DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.SecurityMode = %x\n", DxeTbtConfig->TbtCommonConfig.SecurityMode));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Gpio5Filter = %x\n", DxeTbtConfig->TbtCommonConfig.Gpio5Filter));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch = %x\n", DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3Tbt = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3Tbt));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReqDelay = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReqDelay));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Win10Support = %x\n", DxeTbtConfig->TbtCommonConfig.Win10Support));

  return;
}

/**
  Install Tbt Policy

  @param[in] ImageHandle                Image handle of this driver.

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallTbtPolicy (
  IN  EFI_HANDLE                    ImageHandle
  )
{
  EFI_STATUS                    Status;
  DXE_TBT_POLICY_PROTOCOL       *DxeTbtPolicy;

  DEBUG ((DEBUG_INFO, "Install DXE TBT Policy\n"));

  DxeTbtPolicy = NULL;
  //Alloc memory for DxeTbtPolicy
  DxeTbtPolicy = (DXE_TBT_POLICY_PROTOCOL *) AllocateZeroPool (sizeof (DXE_TBT_POLICY_PROTOCOL));
  if (DxeTbtPolicy == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gDxeTbtPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  DxeTbtPolicy
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install Tbt Secure Boot List protocol failed\n"));
  }
  return Status;
}

