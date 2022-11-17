/** @file
  Source code file for TBT Init PEI module


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiTbtPolicyLib.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/PeiTbtPolicy.h>
#include <Ppi/EndOfPeiPhase.h>
#include <TbtBoardInfo.h>
#include <Private/Library/PeiDTbtInitLib.h>
/*
/**
  This function Update and Print PEI TBT Policy after TbtPolicyBoardInitDone

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/


/**
  This function pass PEI TBT Policy to Hob at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/


EFI_STATUS
EFIAPI
PassTbtPolicyToHob (
VOID
  )
{
  EFI_STATUS            Status;
  EFI_BOOT_MODE         BootMode;
  TBT_INFO_HOB          *TbtInfoHob;
  PEI_TBT_POLICY        *PeiTbtConfig;

  DEBUG ((DEBUG_INFO, "PassTbtPolicyToHob\n"));

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  if (BootMode == BOOT_ON_S3_RESUME ) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);

  //
  // Create HOB for TBT Data
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TBT_INFO_HOB),
             (VOID **) &TbtInfoHob
             );
  DEBUG ((DEBUG_INFO, "TbtInfoHob Created \n"));
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize the TBT INFO HOB data.
  //
  TbtInfoHob->EfiHobGuidType.Name = gTbtInfoHobGuid;

  //
  // Update DTBT Policy
  //
  TbtInfoHob-> DTbtControllerConfig.DTbtControllerEn = PeiTbtConfig-> DTbtControllerConfig.DTbtControllerEn;
  TbtInfoHob-> DTbtControllerConfig.Type = PeiTbtConfig-> DTbtControllerConfig.Type;
  TbtInfoHob-> DTbtControllerConfig.PcieRpNumber = PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber;
  TbtInfoHob-> DTbtControllerConfig.ForcePwrGpio.GpioPad = PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioPad;
  TbtInfoHob-> DTbtControllerConfig.ForcePwrGpio.GpioLevel = PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioLevel;
  TbtInfoHob-> DTbtControllerConfig.CioPlugEventGpio.GpioPad = PeiTbtConfig-> DTbtControllerConfig.CioPlugEventGpio.GpioPad;
  TbtInfoHob-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature = PeiTbtConfig-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature;
  TbtInfoHob-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting = PeiTbtConfig-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting;
  TbtInfoHob-> DTbtControllerConfig.PcieRstGpio.GpioPad = PeiTbtConfig-> DTbtControllerConfig.PcieRstGpio.GpioPad;
  TbtInfoHob-> DTbtControllerConfig.PcieRstGpio.GpioLevel = PeiTbtConfig-> DTbtControllerConfig.PcieRstGpio.GpioLevel;

  TbtInfoHob->DTbtCommonConfig.TbtBootOn = PeiTbtConfig->DTbtCommonConfig.TbtBootOn;
  TbtInfoHob->DTbtCommonConfig.TbtUsbOn = PeiTbtConfig->DTbtCommonConfig.TbtUsbOn;
  TbtInfoHob->DTbtCommonConfig.Gpio3ForcePwr = PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr;
  TbtInfoHob->DTbtCommonConfig.Gpio3ForcePwrDly = PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly;
  TbtInfoHob->DTbtCommonConfig.DTbtSharedGpioConfiguration = PeiTbtConfig->DTbtCommonConfig.DTbtSharedGpioConfiguration;
  TbtInfoHob->DTbtCommonConfig.PcieRstSupport = PeiTbtConfig->DTbtCommonConfig.PcieRstSupport;

  return EFI_SUCCESS;
}


/**
  This function handles TbtInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/

EFI_STATUS
EFIAPI
TbtInitEndOfPei (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         DTbtExisted;
  PEI_TBT_POLICY  *PeiTbtConfig;

  DEBUG ((DEBUG_INFO, "TbtInitEndOfPei Entry\n"));

  Status       = EFI_SUCCESS;
  PeiTbtConfig = NULL;
  DTbtExisted  = FALSE;

  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);

    if (PeiTbtConfig-> DTbtControllerConfig.DTbtControllerEn == 1) {
      DTbtExisted = TRUE;
  }

  if (DTbtExisted == TRUE) {
    //
    // Call Init function
    //
   Status = TbtInit ();
  }

  return EFI_SUCCESS;
}

/**
  TBT Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
TbtInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  DEBUG ((DEBUG_INFO, "TBT PEI EntryPoint\n"));

  //
  // Install PEI TBT Policy
  //
  Status = InstallPeiTbtPolicy ();
  ASSERT_EFI_ERROR (Status);


  UpdatePeiTbtPolicy ();

  TbtPrintPeiPolicyConfig ();
  //
  // Performing PassTbtPolicyToHob and TbtInitEndOfPei
  //
  Status = PassTbtPolicyToHob ();

  Status = TbtInitEndOfPei ();

  return Status;
}

