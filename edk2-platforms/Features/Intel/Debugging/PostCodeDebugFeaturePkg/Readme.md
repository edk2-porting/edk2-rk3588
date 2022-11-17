# Overview
* **Feature Name:** Post Code Debug
* **PI Phase(s) Supported:** PEI, DXE, SMM
* **SMM Required?** Yes

More Information:

## Purpose
The PostCodeDebugFeaturePkg include some useful post code debug libraries, such as get post code from status code and show it.
This is an important capability in firmware development to get and analyze the post code.


# High-Level Theory of Operation
It provide a library PostCodeStatusCodeHandlerLib used by edk2 StatusCodeHandler.efi, used to show the post code.
It also provide a library of PostCodeMap lib, it map the status code to post code.
A library of PostCode lib is needed by platform.

In the library contstructor function, PostCodeStatusCodeHandlerLib register the call back function for ReportStatusCode.
When called, it call GetPostCodeFromStatusCode() in PostCodeMapLib to get post code from status code, and call PostCode() in PostCodeLib to show the post code.

PostCodeStatusCodeHandlerLib include 3 libraries for PEI, RuntimeDxe, SMM:
* PeiPostCodeStatusCodeHandlerLib
* RuntimeDxePostCodeStatusCodeHandlerLib
* SmmPostCodeStatusCodeHandlerLib

## Firmware Volumes
Linked with StatusCodeHandler.efi, and make sure put the StatusCodeHandler.efi after the ReportStatusCodeRouter.efi.

## Modules
* PostCodeStatusCodeHandlerLib
* PostCodeMapLib

## PostCodeStatusCodeHandlerLib
This library register the call back function for ReportStatusCode, and get post code from status code, and show post code.

## PostCodeMapLib
This library provide a function to get post code from status code.

## Key Functions
* In PeiPostCodeStatusCodeHandlerLib:
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

* In RuntimeDxePostCodeStatusCodeHandlerLib:
  EFI_STATUS
  EFIAPI
  PostCodeStatusCodeReportWorker (
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value,
    IN UINT32                         Instance,
    IN EFI_GUID                       *CallerId,
    IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  )

* In SmmPostCodeStatusCodeHandlerLib:
  EFI_STATUS
  EFIAPI
  PostCodeStatusCodeReportWorker (
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value,
    IN UINT32                         Instance,
    IN EFI_GUID                       *CallerId,
    IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
    )

* In PostCodeMapLib:
  UINT32
  EFIAPI
  GetPostCodeFromStatusCode (
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value
  )

## Configuration
* Link the library to StatusCodeHandler.efi.
  Example:
    MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf {
      <LibraryClasses>
        NULL|PostCodeDebugFeaturePkg/Library/PostCodeStatusCodeHandlerLib/RuntimeDxePostCodeStatusCodeHandlerLib.inf
    }
  Refer to PostCodeDebugFeature.dsc for other example.
* Config PCD gPostCodeDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUsePostCode.
  In platform .dsc file, need to config the type of gPostCodeDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUsePostCode.
  Use PcdsFixedAtBuild to save binary size, and use PcdsDynamic if want to enable/disable in runtime.
* Implemented platform's special PostCodeMapLib if needed.
* Provide the platform's special PostCodeLib.
* Make sure put the StatusCodeHandler.efi after the ReportStatusCodeRouter.efi.

## Data Flows
Status Code (ReportStatusCode) -> Post Code (GetPostCodeFromStatusCode).

## Control Flows
ReportStatusCode() -> PostCodeStatusCodeReportWorker() -> GetPostCodeFromStatusCode() -> PostCode()

## Build Flows
There is not special build flows.

## Test Point Results
Verify the post code shown is correct.

## Functional Exit Criteria
N/A

## Feature Enabling Checklist
* Set the PCD gPostCodeDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUsePostCode to TRUE.
* The post code can be shown.

## Common Optimizations
* Implemented platform's special PostCodeMapLib if needed.
