/** @file
  Dynamic link silicon library service access Protocol

  This protocol abstracts silicon static library accesses via a protocol

  @copyright
  Copyright 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DYNAMIC_SI_LIBARY_PROTOCOL_H_
#define _DYNAMIC_SI_LIBARY_PROTOCOL_H_

#include <Library/CompressedVariableLib.h>
#include <IioPlatformData.h>
#include <Include/SystemInfoVar.h>
#include <Guid/MemoryMapData.h>
#include <Library/CpuEarlyDataLib.h>
#include <UsraAccessType.h>
#include <IioUniversalData.h>

#define DYNAMIC_SI_LIBARY_PROTOCOL_GUID \
  { 0xb235fbed, 0x3b25, 0x4cb3, { 0x98, 0x9c, 0x8c, 0xe7, 0xec, 0x49, 0x8b, 0x7e } }

#define DYNAMIC_SI_LIBARY_PROTOCOL_SIGNATURE  SIGNATURE_32('D', 'S', 'L', 'P')
#define DYNAMIC_SI_LIBARY_PROTOCOL_VERSION    0x01

//
// Functions
//

typedef
EFI_STATUS
(EFIAPI *DXE_SET_GPIO_OUTPUT_VALUE) (
  IN  UINT32                              GPioPad,
  IN  UINT32                              Value
  );

typedef
BOOLEAN
(EFIAPI *DXE_IsCpuAndRevision) (
  IN UINT8  CpuType,
  IN UINT16 Revision
  );

typedef
CPU_VAR_DATA *
(EFIAPI *DXE_GetCpuVarData) (
  );

typedef
UINT8
(EFIAPI *DXE_MaxSataControllerNum) (
  VOID
  );

typedef
UINTN
(EFIAPI *DXE_MmPciBase) (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
  );

typedef
CPU_CSR_ACCESS_VAR *
(EFIAPI *DXE_GetSysCpuCsrAccessVar) (
  VOID
  );

typedef
VOID
(EFIAPI *DXE_IioPciHookBeforeEnumeration) (
  IN  UINT8          Segment,
  IN  UINT8          Bus,
  IN  UINT8          Device,
  IN  UINT8          Function,
  IN  UINT32         DidVid
  );

typedef
CHAR8*
(EFIAPI *DXE_PchGetSeriesStr) (
  );

typedef
EFI_STATUS
(EFIAPI *DXE_PchGetSteppingStr) (
  OUT    CHAR8                          *Buffer,
  IN     UINT32                         BufferSize
  );

typedef
CHAR8*
(EFIAPI *DXE_PchGetSkuStr) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *DXE_SaveVariableFromHob) (
  IN EFI_GUID HobGuid,
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid
  );

typedef
VOID
(EFIAPI *DXE_SetColdBootSlowRequired) (
  IN  BOOLEAN ColdBootSlowRequired
  );

typedef
SYS_INFO_VAR_NVRAM *
(EFIAPI *DXE_GetSysInfoVarNvramPtr) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *DXE_HybridSystemLevelEmulationEnabled) (
  VOID
  );

typedef
SYSTEM_MEMORY_MAP_HOB *
(EFIAPI *DXE_GetSystemMemoryMapData) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *DXE_X2ApicIdDetect) (
  IN VOID  *Host
  );

typedef
RETURN_STATUS
(EFIAPI *DXE_RegisterRead) (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  );

typedef
RETURN_STATUS
(EFIAPI *DXE_RegisterWrite) (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  );

typedef
UINT8
(EFIAPI *DXE_SataDevNumber) (
  IN UINT32 SataCtrlIndex
  );

typedef
UINT8
(EFIAPI *DXE_SataFuncNumber) (
  IN UINT32 SataCtrlIndex
  );

typedef
UINT16
(EFIAPI *DXE_PmcGetAcpiBase) (
  VOID
  );

typedef
UINTN
(EFIAPI *DXE_PchGetPmcBaseByPchId) (
  IN  UINT8             PchId
  );

typedef
VOID
(EFIAPI *DXE_SetBiosInfoFlagWpe) (
  VOID
  );

typedef
VOID
(EFIAPI *DXE_ProgramGenProtRangeRegs) (
  IIO_UDS             *IioUds
  );

typedef
VOID
(EFIAPI *DXE_ProgramImrRegs) (
  IIO_UDS             *IioUds
  );

typedef
VOID
(EFIAPI *DXE_ProgramImr2Regs) (
  IIO_UDS             *IioUds
  );

typedef
VOID
(EFIAPI *DXE_CheckAndPopulateIedTraceMemory) (
  UINTN       IedTraceSize,
  IIO_UDS     *IioUds
  );

typedef
UINT32
(EFIAPI *DXE_ReadScratchpad7) (
  VOID
  );

//
// UBA specific silicon abstraction protocol
//
typedef struct {
  UINT32                                  Signature;
  UINT32                                  Version;

  DXE_GetCpuVarData                       GetCpuVarData;
  DXE_IsCpuAndRevision                    IsCpuAndRevision;
  DXE_MaxSataControllerNum                MaxSataControllerNum;
  DXE_MmPciBase                           MmPciBase;
  DXE_GetSysCpuCsrAccessVar               GetSysCpuCsrAccessVar;
  DXE_IioPciHookBeforeEnumeration         IioPciHookBeforeEnumeration;
  DXE_SET_GPIO_OUTPUT_VALUE               GpioSetOutputValue;
  DXE_PchGetSeriesStr                     PchGetSeriesStr;
  DXE_PchGetSteppingStr                   PchGetSteppingStr;
  DXE_PchGetSkuStr                        PchGetSkuStr;
  DXE_SaveVariableFromHob                 SaveVariableFromHob;
  DXE_SetColdBootSlowRequired             SetColdBootSlowRequired;
  DXE_GetSysInfoVarNvramPtr               GetSysInfoVarNvramPtr;
  DXE_X2ApicIdDetect                      X2ApicIdDetect;
  DXE_GetSystemMemoryMapData              GetSystemMemoryMapData;
  DXE_RegisterRead                        RegisterRead;
  DXE_RegisterWrite                       RegisterWrite;
  DXE_HybridSystemLevelEmulationEnabled   HybridSystemLevelEmulationEnabled;
  DXE_SataDevNumber                       SataDevNumber;
  DXE_SataFuncNumber                      SataFuncNumber;
  DXE_PmcGetAcpiBase                      PmcGetAcpiBase;
  DXE_PchGetPmcBaseByPchId                PchGetPmcBaseByPchId;
  DXE_SetBiosInfoFlagWpe                  SetBiosInfoFlagWpe;
  DXE_ProgramGenProtRangeRegs             ProgramGenProtRangeRegs;
  DXE_ProgramImrRegs                      ProgramImrRegs;
  DXE_ProgramImr2Regs                     ProgramImr2Regs;
  DXE_CheckAndPopulateIedTraceMemory      CheckAndPopulateIedTraceMemory;
  DXE_ReadScratchpad7                     ReadScratchpad7;
} DYNAMIC_SI_LIBARY_PROTOCOL;

extern EFI_GUID gDynamicSiLibraryProtocolGuid;

#endif // _DYNAMIC_SI_LIBARY_PROTOCOL_H_
