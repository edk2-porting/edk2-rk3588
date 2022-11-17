/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TbtCommonLib.h>
#include <Library/DxeTbtPolicyLib.h>
#include <TbtBoardInfo.h>
#include <Protocol/DxeTbtPolicy.h>
#include <Protocol/TbtNvsArea.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiSpec.h>
#include <Library/PcdLib.h>
#include <Library/AslUpdateLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED TBT_NVS_AREA_PROTOCOL                     mTbtNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED TBT_INFO_HOB                              *gTbtInfoHob = NULL;

/**
  TBT NVS Area Initialize

**/

VOID
TbtNvsAreaInit (
  IN  VOID              **mTbtNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;
  TBT_NVS_AREA_PROTOCOL         *TbtNvsAreaProtocol;
  DXE_TBT_POLICY_PROTOCOL       *DxeTbtConfig;

  DEBUG ((DEBUG_INFO, "TbtNvsAreaInit Start\n"));
  Status = gBS->LocateProtocol (
              &gDxeTbtPolicyProtocolGuid,
              NULL,
              (VOID **) &DxeTbtConfig
              );
  ASSERT_EFI_ERROR (Status);

  Pages = EFI_SIZE_TO_PAGES (sizeof (TBT_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  *mTbtNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mTbtNvsAreaPtr, sizeof (TBT_NVS_AREA), 0);

  //
  // TBTNvsAreaProtocol default value init here
  //
  TbtNvsAreaProtocol = (TBT_NVS_AREA_PROTOCOL *) &Address;

  //
  // Initialize default values
  //
  TbtNvsAreaProtocol->Area->WAKFinished             = 0;
  TbtNvsAreaProtocol->Area->DiscreteTbtSupport      = ((gTbtInfoHob-> DTbtControllerConfig.DTbtControllerEn == 1 ) ? TRUE : FALSE);
  TbtNvsAreaProtocol->Area->TbtAcpiRemovalSupport   = 0;
  TbtNvsAreaProtocol->Area->TbtGpioFilter           = (UINT8) DxeTbtConfig->TbtCommonConfig.Gpio5Filter;
//  TbtNvsAreaProtocol->Area->TrOsup                  = (UINT8) DxeTbtConfig->TbtCommonConfig.TrA0OsupWa;
  TbtNvsAreaProtocol->Area->TbtFrcPwrEn             = gTbtInfoHob->DTbtCommonConfig.Gpio3ForcePwr;
  TbtNvsAreaProtocol->Area->TbtAspm                 = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtAspm;
//  TbtNvsAreaProtocol->Area->TbtL1SubStates          = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtL1SubStates;
  TbtNvsAreaProtocol->Area->TbtSetClkReq            = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtSetClkReq;
  TbtNvsAreaProtocol->Area->TbtLtr                  = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtLtr;
//  TbtNvsAreaProtocol->Area->TbtPtm                  = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtPtm;
  TbtNvsAreaProtocol->Area->TbtWakeupSupport        = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport;
  TbtNvsAreaProtocol->Area->TbtAcDcSwitch           = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch;
  TbtNvsAreaProtocol->Area->Rtd3TbtSupport          = (UINT8) DxeTbtConfig->TbtCommonConfig.Rtd3Tbt;             // TBT RTD3 Enable.
  TbtNvsAreaProtocol->Area->Rtd3TbtOffDelay         = (UINT16) DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay;    // TBT RTD3 Off delay in ms.
  TbtNvsAreaProtocol->Area->Rtd3TbtClkReq           = (UINT8) DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq;       // TBT RTD3 ClkReq Mask Enable.
  TbtNvsAreaProtocol->Area->Rtd3TbtClkReqDelay      = (UINT16) DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReqDelay; // TBT RTD3 ClkReq mask delay in ms.
  TbtNvsAreaProtocol->Area->TbtWin10Support         = (UINT8) DxeTbtConfig->TbtCommonConfig.Win10Support; // TBT FW Execution Mode

  //
  // DTBT Controller 1
  //
  TbtNvsAreaProtocol->Area->DTbtControllerEn0       = gTbtInfoHob-> DTbtControllerConfig.DTbtControllerEn;
  TbtNvsAreaProtocol->Area->RootportSelected0       = gTbtInfoHob-> DTbtControllerConfig.PcieRpNumber;
  TbtNvsAreaProtocol->Area->RootportSelected0Type   = gTbtInfoHob-> DTbtControllerConfig.Type;
  TbtNvsAreaProtocol->Area->RootportEnabled0        = gTbtInfoHob-> DTbtControllerConfig.DTbtControllerEn;
  TbtNvsAreaProtocol->Area->TbtFrcPwrGpioNo0        = gTbtInfoHob-> DTbtControllerConfig.ForcePwrGpio.GpioPad;
  TbtNvsAreaProtocol->Area->TbtFrcPwrGpioLevel0     = gTbtInfoHob-> DTbtControllerConfig.ForcePwrGpio.GpioLevel;
  TbtNvsAreaProtocol->Area->TbtCioPlugEventGpioNo0  = gTbtInfoHob-> DTbtControllerConfig.CioPlugEventGpio.GpioPad;
  TbtNvsAreaProtocol->Area->TbtPcieRstGpioNo0       = gTbtInfoHob-> DTbtControllerConfig.PcieRstGpio.GpioPad;
  TbtNvsAreaProtocol->Area->TbtPcieRstGpioLevel0    = gTbtInfoHob-> DTbtControllerConfig.PcieRstGpio.GpioLevel;

  TbtNvsAreaProtocol->Area->TBtCommonGpioSupport    = gTbtInfoHob->DTbtCommonConfig.DTbtSharedGpioConfiguration;

  DEBUG ((DEBUG_INFO, "TbtNvsAreaInit End\n"));
}

/**
  This function gets registered as a callback to patch TBT ASL code

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
  can we put this also in read me
**/
VOID
EFIAPI
TbtAcpiEndOfDxeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  UINT32                                Signature;

  Address = (UINT32) (UINTN) mTbtNvsAreaProtocol.Area;
  Length  = (UINT16) sizeof (TBT_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch TBT NvsAreaAddress: TBT NVS Address %x Length %x\n", Address, Length));
  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','N','V','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','N','V','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  if (gTbtInfoHob != NULL) {
    if (gTbtInfoHob-> DTbtControllerConfig.DTbtControllerEn == 1) {
      if (gTbtInfoHob-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting == TRUE) {
        DEBUG ((DEBUG_INFO, "Patch ATBT Method Name\n"));
        Signature = gTbtInfoHob-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature;
        Status  = UpdateNameAslCode (SIGNATURE_32 ('A','T','B','T'), &Signature, sizeof (Signature));
        ASSERT_EFI_ERROR (Status);
      }
    }
  }

  return;
}

/**
  Initialize Thunderbolt(TM) SSDT ACPI tables

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/

EFI_STATUS
EFIAPI
TbtDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
 // EFI_EVENT               EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "TbtDxeEntryPoint \n"));

  //
  // Get TBT INFO HOB
  //
  gTbtInfoHob = (TBT_INFO_HOB *) GetFirstGuidHob (&gTbtInfoHobGuid);
  if (gTbtInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }
  InstallTbtPolicy (ImageHandle);
  //
  // Update DXE TBT Policy
  //
  UpdateTbtPolicyCallback ();

  //
  // Print DXE TBT Policy
  //
  TbtPrintDxePolicyConfig ();

  //
  // Initialize Tbt Nvs Area
  //
  TbtNvsAreaInit ((VOID **) &mTbtNvsAreaProtocol.Area);


  //
  // [ACPI] Thunderbolt ACPI table
  //


  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gTbtNvsAreaProtocolGuid,
                  &mTbtNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register an end of DXE event for TBT ACPI to do some patch can be put as description
  //
  /**
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  TbtAcpiEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
**/
  return EFI_SUCCESS;
}

