/** @file
  PostCode status code implementation.

  Copyright (c) 2010 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Ppi/ReportStatusCodeHandler.h>

#include <Library/PostCodeMapLib.h>
#include <Library/PostCodeLib.h>

/**
  Convert status code value and write data to post code.

  @param  PeiServices      An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to post code successfully.

**/
EFI_STATUS
EFIAPI
PostCodeStatusCodeReportWorker (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN CONST EFI_GUID                 *CallerId,
  IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )
{
  UINT32 PostCodeValue;

  PostCodeValue = GetPostCodeFromStatusCode (CodeType, Value);
  if (PostCodeValue != 0) {
    DEBUG ((EFI_D_INFO, "POSTCODE=<%02x>\n", PostCodeValue));
    PostCode (PostCodeValue);
  }

  return EFI_SUCCESS;
}

/**
  Constructor function of PeiPostCodeStatusCodeHandlerLib.

  This function is the constructor function of this PostCode Status Code Handler Library for PEI Phase.
  It check whether need post code, and register it to gEfiPeiRscHandlerPpiGuid.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCESS  The entry point of DXE IPL PEIM executes successfully.

**/
RETURN_STATUS
EFIAPI
PeiPostCodeStatusCodeHandlerLibConstructor (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_RSC_HANDLER_PPI     *RscHandlerPpi;

  if (!PcdGetBool (PcdStatusCodeUsePostCode)) {
    return RETURN_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = RscHandlerPpi->Register (PostCodeStatusCodeReportWorker);
  ASSERT_EFI_ERROR (Status);

  return RETURN_SUCCESS;
}
