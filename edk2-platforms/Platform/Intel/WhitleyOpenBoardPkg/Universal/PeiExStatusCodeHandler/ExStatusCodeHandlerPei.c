/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "ExStatusCodeHandlerPei.h"

EFI_PEI_EX_RSC_HANDLER_PPI  mExStatusCodeHandlerPpi = {
  RegisterExStatusCodeHandler
  };

EFI_PEI_PPI_DESCRIPTOR   mExStatusCodeHandlerPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiExStatusCodeHandlerPpiGuid,
    &mExStatusCodeHandlerPpi
  }
};

/**
  Registers ExSerialStatusCodeReportWorker as callback function for ReportStatusCode() notification.


  @param[in] PeiServices        Pointer to PEI Services Table.

  @retval EFI_SUCCESS           Function was successfully registered.
  @retval EFI_INVALID_PARAMETER The callback function was NULL.
  @retval EFI_OUT_OF_RESOURCES  The internal buffer ran out of space. No more functions can be
                                registered.
  @retval EFI_ALREADY_STARTED   The function was already registered. It can't be registered again.

**/
EFI_STATUS
EFIAPI
RegisterExStatusCodeHandler (
  IN CONST  EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_RSC_HANDLER_PPI     *RscHandlerPpi;

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );
  ASSERT_EFI_ERROR (Status);

  if (PcdGetBool (PcdStatusCodeUseSerial)) {
    Status = RscHandlerPpi->Register (ExSerialStatusCodeReportWorker);
    if (Status != EFI_ALREADY_STARTED) {
      ASSERT_EFI_ERROR (Status);
    }
  }
  return Status;
}

/**
  Entry point of EX Status Code PEIM.

  This function is the entry point of this EX Status Code PEIM.
  It initializes serial port status code handler with policy features.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCESS  The entry point of PEIM executes successfully.

**/
EFI_STATUS
EFIAPI
ExStatusCodeHandlerPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  UINT32                      DebugPrintErrorLevel;

  //
  // Dispatch initialization request to sub-statuscode-devices.
  //
  Status = SerialPortInitialize();
  ASSERT_EFI_ERROR (Status);

  //
  // If serial logging is disabled. set PcdStatusCodeUseSerial to FALSE.
  //

  DebugPrintErrorLevel = GetDebugPrintErrorLevel ();
  if (DebugPrintErrorLevel == 0) {
    Status = PcdSetBoolS (PcdStatusCodeUseSerial, FALSE);
    ASSERT_EFI_ERROR (Status);
  }

  Status = RegisterExStatusCodeHandler (PeiServices);
  ASSERT_EFI_ERROR (Status);

  //
  // Install Report Status Code Handler PPI
  //
  Status = PeiServicesInstallPpi (mExStatusCodeHandlerPpiList);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

