# Overview
* **Feature Name:** Beep Debug
* **PI Phase(s) Supported:** PEI, DXE, SMM
* **SMM Required?** Yes

More Information:

## Purpose
The BeepDebugFeaturePkg include some useful beep debug libraries, such as get beep value from status code and beep.
This is an important capability in firmware development to get and analyze the early error when there is not serial port.


# High-Level Theory of Operation
It provide a library BeepStatusCodeHandlerLib used by edk2 StatusCodeHandler.efi, used to do beep if needed.
It also provide a library of BeepMap lib, it map the status code to beep value.
A library of Beep lib is needed by platform, and this pkg has a Null implementation.

In the library contstructor function, BeepStatusCodeHandlerLib register the call back function for ReportStatusCode.
When called, it call GetBeepFromStatusCode() in BeepMapLib to get beep value from status code, and call Beep() in BeepLib to beep.

BeepStatusCodeHandlerLib include 3 libraries for PEI, RuntimeDxe, SMM:
* PeiBeepStatusCodeHandlerLib
* RuntimeDxeBeepStatusCodeHandlerLib
* SmmBeepStatusCodeHandlerLib

## Firmware Volumes
Linked with StatusCodeHandler.efi, and make sure put the StatusCodeHandler.efi after the ReportStatusCodeRouter.efi.

## Modules
* BeepStatusCodeHandlerLib
* BeepMapLib
* BeepLibNull

## BeepStatusCodeHandlerLib
This library register the call back function for ReportStatusCode, and get beep valude from status code, and do beep.

## BeepMapLib
This library provide a function to get beep value from status code.

## Key Functions
* In PeiBeepStatusCodeHandlerLib:
  EFI_STATUS
  EFIAPI
  BeepStatusCodeReportWorker (
    IN CONST  EFI_PEI_SERVICES        **PeiServices,
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value,
    IN UINT32                         Instance,
    IN CONST EFI_GUID                 *CallerId,
    IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )

* In RuntimeDxeBeepStatusCodeHandlerLib:
  EFI_STATUS
  EFIAPI
  BeepStatusCodeReportWorker (
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value,
    IN UINT32                         Instance,
    IN EFI_GUID                       *CallerId,
    IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  )

* In SmmBeepStatusCodeHandlerLib:
  EFI_STATUS
  EFIAPI
  BeepStatusCodeReportWorker (
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value,
    IN UINT32                         Instance,
    IN EFI_GUID                       *CallerId,
    IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
    )

* In BeepMapLib:
  UINT32
  EFIAPI
  GetBeepValueFromStatusCode (
    IN EFI_STATUS_CODE_TYPE           CodeType,
    IN EFI_STATUS_CODE_VALUE          Value
  )

* In BeepLib:
  VOID
  EFIAPI
  Beep (
    IN UINT32  Value
  )

## Configuration
* Link the library to StatusCodeHandler.efi.
  Example:
    MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf {
    <LibraryClasses>
      NULL|BeepDebugFeaturePkg/Library/BeepStatusCodeHandlerLib/RuntimeDxeBeepStatusCodeHandlerLib.inf
    }
  Refer to BeepDebugFeature.dsc for other example.
* Config PCD gBeepDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUseBeep.
  In platform .dsc file, need to config the type of gBeepDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUseBeep.
  Use PcdsFixedAtBuild to save binary size, and use PcdsDynamic if want to enable/disable in runtime.
* Implemented platform's special BeepMapLib if needed.
* Provide the platform's special BeepLib.
* Make sure put the StatusCodeHandler.efi after the ReportStatusCodeRouter.efi.

## Data Flows
Status Code (ReportStatusCode) -> Beep Value (GetBeepValueFromStatusCode).

## Control Flows
ReportStatusCode() -> BeepStatusCodeReportWorker() -> GetBeepValueFromStatusCode() -> Beep()

## Build Flows
There is not special build flows.

## Test Point Results
Verify the post code shown is correct.

## Functional Exit Criteria
N/A

## Feature Enabling Checklist
* Set the PCD gBeepDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUseBeep to TRUE.
* Plug out all the memory, check can here the beep.

## Common Optimizations
* Implemented platform's special BeepMapLib if needed.
