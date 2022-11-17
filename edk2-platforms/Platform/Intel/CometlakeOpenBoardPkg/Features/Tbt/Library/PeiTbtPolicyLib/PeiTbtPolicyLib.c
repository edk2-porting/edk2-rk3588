/** @file
  This file is PeiTbtPolicyLib library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PeiServicesLib.h>
#include <Library/GpioLib.h>
#include <PiPei.h>
#include <PeiTbtPolicyLibrary.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/PeiTbtPolicy.h>
#include <Base.h>
#include <GpioConfig.h>

/**
  Update PEI TBT Policy Callback
**/
VOID
EFIAPI
UpdatePeiTbtPolicy (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  PEI_TBT_POLICY                   *PeiTbtConfig;

  PeiTbtConfig = NULL;
  Status = EFI_NOT_FOUND;

  DEBUG ((DEBUG_INFO, "UpdatePeiTbtPolicy \n"));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

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
  // Update DTBT Policy
  //
  PeiTbtConfig-> DTbtControllerConfig.DTbtControllerEn = PcdGet8 (PcdDTbtControllerEn);
  if (PcdGet8 (PcdDTbtControllerType) == TYPE_PEG)
  {
    PeiTbtConfig-> DTbtControllerConfig.Type = (UINT8) TYPE_PEG;
    PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber = 1; // PEG RP 1 (Function no. 0)
  }
  else {
    PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber = PcdGet8 (PcdDTbtPcieRpNumber);
    PeiTbtConfig-> DTbtControllerConfig.Type = PcdGet8 (PcdDTbtControllerType);
  }
  PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.GpioPad = (GPIO_PAD) PcdGet32 (PcdDTbtCioPlugEventGpioPad);
  if (GpioCheckFor2Tier(PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.GpioPad)) {
    PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting = 0;
    PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature = SIGNATURE_32('X', 'T', 'B', 'T');
  }
  else {
    PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting = 1;
    //
    // Update Signature based on platform GPIO.
    //
    PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature = SIGNATURE_32('X', 'T', 'B', 'T');
  }
  PeiTbtConfig->DTbtCommonConfig.TbtBootOn = PcdGet8 (PcdDTbtBootOn);
  PeiTbtConfig->DTbtCommonConfig.TbtUsbOn = PcdGet8 (PcdDTbtUsbOn);
  PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr = PcdGet8 (PcdDTbtGpio3ForcePwr);
  PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly = PcdGet16 (PcdDTbtGpio3ForcePwrDly);

  return;
}

/**
  Print PEI TBT Policy
**/
VOID
EFIAPI
TbtPrintPeiPolicyConfig (
  VOID
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                       Status;
  PEI_TBT_POLICY                   *PeiTbtConfig;

  PeiTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "TbtPrintPolicyConfig Start\n"));

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
  // Print DTBT Policy
  //
  DEBUG ((DEBUG_INFO, "\n------------------------ TBT Policy (PEI) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", PEI_TBT_POLICY_REVISION));
  DEBUG ((DEBUG_INFO, "------------------------ PEI_TBT_CONFIG  -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", PEI_TBT_POLICY_REVISION));

  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.DTbtControllerEn = %x\n", PeiTbtConfig-> DTbtControllerConfig.DTbtControllerEn));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.Type = %x\n", PeiTbtConfig-> DTbtControllerConfig.Type));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.PcieRpNumber = %x\n", PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.ForcePwrGpio.GpioPad = %x\n", PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioPad));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.ForcePwrGpio.GpioLevel = %x\n", PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioLevel));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.PcieRstGpio.GpioPad = %x\n", PeiTbtConfig-> DTbtControllerConfig.PcieRstGpio.GpioPad));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.PcieRstGpio.GpioLevel = %x\n", PeiTbtConfig-> DTbtControllerConfig.PcieRstGpio.GpioLevel));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.GpioPad = %x\n", PeiTbtConfig-> DTbtControllerConfig.CioPlugEventGpio.GpioPad));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature = %x\n", PeiTbtConfig-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignature));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting = %x\n", PeiTbtConfig-> DTbtControllerConfig.CioPlugEventGpio.AcpiGpeSignaturePorting));


  //
  // Print DTBT Common Policy
  //
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.TbtBootOn = %x\n", PeiTbtConfig->DTbtCommonConfig.TbtBootOn));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.TbtUsbOn = %x\n", PeiTbtConfig->DTbtCommonConfig.TbtUsbOn));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr = %x\n", PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly = %x\n", PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.DTbtSharedGpioConfiguration = %x\n", PeiTbtConfig->DTbtCommonConfig.DTbtSharedGpioConfiguration));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.PcieRstSupport = %x\n", PeiTbtConfig->DTbtCommonConfig.PcieRstSupport));

  DEBUG ((DEBUG_INFO, "\n------------------------ TBT Policy (PEI) Print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  Install Tbt Policy

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallPeiTbtPolicy (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_PPI_DESCRIPTOR        *PeiTbtPolicyPpiDesc;
  PEI_TBT_POLICY                *PeiTbtConfig;

  DEBUG ((DEBUG_INFO, "Install PEI TBT Policy\n"));

  PeiTbtConfig = NULL;

  //
  // Allocate memory for PeiTbtPolicyPpiDesc
  //
  PeiTbtPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (PeiTbtPolicyPpiDesc != NULL);
  if (PeiTbtPolicyPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate memory and initialize all default to zero for PeiTbtPolicy
  //
  PeiTbtConfig = (PEI_TBT_POLICY *) AllocateZeroPool (sizeof (PEI_TBT_POLICY));
  ASSERT (PeiTbtConfig != NULL);
  if (PeiTbtConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize PPI
  //
  PeiTbtPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PeiTbtPolicyPpiDesc->Guid = &gPeiTbtPolicyPpiGuid;
  PeiTbtPolicyPpiDesc->Ppi = PeiTbtConfig;

  Status = PeiServicesInstallPpi (PeiTbtPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install PEI TBT Policy failed\n"));
  }
  return Status;
}


