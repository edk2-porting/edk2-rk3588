/** @file
  UBS silicon access PPI

  This PPI abstracts all UBA silicon accesses

  @copyright
  Copyright 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DYNAMIC_SI_LIBARY_PPI_H_
#define _DYNAMIC_SI_LIBARY_PPI_H_

// {C1176733-159F-42d5-BCB9-320660B17310}
#define DYNAMIC_SI_LIBARY_PPI_GUID \
  { 0x4e18e22b, 0x5034, 0x4512, { 0xb7, 0xe5, 0x0b, 0xf1, 0x9d, 0xe3, 0x59, 0x8c } }

#define UBA_ACCESS_PPI_VERSION    01
#define UBA_ACCESS_PPI_SIGNATURE  SIGNATURE_32('D', 'S', 'L', 'P')

#include <Library/SpsPeiLib.h>
#include <IioPlatformData.h>
#include <GpioConfig.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>

//
// Functions
//

typedef
EFI_STATUS
(EFIAPI *PEI_GET_GPIO_INPUT_VALUE) (
  IN  GPIO_PAD                            GpioPad,
  OUT UINT32                              *InputVal
  );

typedef
EFI_STATUS
(EFIAPI *PEI_SET_GPIO_OUTPUT_VALUE) (
  IN  GPIO_PAD                            GpioPad,
  IN  UINT32                              Value
  );

typedef
EFI_STATUS
(EFIAPI *PEI_GPIO_SET_PAD) (
  IN  GPIO_PAD                            GpioPad,
  IN  GPIO_CONFIG                         *GpioData
  );

typedef
UINT16
(EFIAPI *PEI_GET_PCH_DEVICE_ID) (
  VOID
  );

typedef
PCH_SERIES
(EFIAPI *PEI_GET_PCH_SERIES) (
  VOID
  );

typedef
PCH_STEPPING
(EFIAPI *PEI_GET_PCH_STEPPING) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_IS_PCH_GBE_REGION_VALID) (
  VOID
  );

typedef
UINT32
(EFIAPI *PEI_GET_PCH_GBE_PORT_NUMBER) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_IS_PCH_GBE_PRESENT) (
  VOID
  );

typedef
VOID
(EFIAPI *PEI_PchDisableGbe) (
  VOID
  );

typedef
VOID
(EFIAPI *PEI_PchDisableGbeByPchId) (
  IN  UINT8           PchId
  );

typedef
EFI_STATUS
(EFIAPI *PEI_GpioConfigurePadsByPchId) (
  IN UINT8                     PchId,
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  );

typedef
EFI_STATUS
(EFIAPI *PEI_GpioGetInputValueByPchId) (
  IN UINT8                     PchId,
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *InputVal
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PeiGetCurrenClockingMode) (
   OUT CLOCKING_MODES *ClockingMode
  );

typedef
EFI_STATUS
(EFIAPI *PEI_GetPchPcieRpDevFunByPchId) (
  IN  UINT8   PchId,
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

typedef
BOOLEAN
(EFIAPI *PEI_IsSimicsEnvironment) (
  VOID
  );

typedef
UINT8
(EFIAPI *PEI_GetPchMaxSataPortNum) (
  VOID
  );

typedef
UINT8
(EFIAPI *PEI_GetPchMaxsSataPortNum) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *PEI_GpioSetPadConfigByPchId) (
  IN UINT8                     PchId,
  IN GPIO_PAD                  GpioPad,
  IN GPIO_CONFIG               *GpioData
  );

typedef
UINT8
(EFIAPI *PEI_GetPchMaxPciePortNum) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchGetSataLaneNumByPchId) (
  IN  UINT8           PchId,
  UINT32              SataLaneIndex,
  UINT8               *LaneNum
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchGetPcieLaneNumByPchId) (
  IN  UINT8           PchId,
  UINT32              PcieLaneIndex,
  UINT8               *LaneNum
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchGetsSataLaneNumByPchId) (
  IN  UINT8           PchId,
  UINT32              SataLaneIndex,
  UINT8               *LaneNum
  );

typedef
UINTN
(EFIAPI *PEI_MmPciBase) (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
  );

typedef
BOOLEAN
(EFIAPI *PEI_HybridSystemLevelEmulationEnabled) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_IioIsSocketPresent) (
  IN  UINT8  IioIndex
  );

typedef
UINT8
(EFIAPI *PEI_GetMaxPortNumPerSocket) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_IsVMDEnabledForPort) (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  );

typedef
BOOLEAN
(EFIAPI *PEI_IioAreLanesAssignedToPort) (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  );

typedef
UINT8
(EFIAPI *PEI_GetPortIndexbyStack) (
  IN UINT8    StackIndex,
  IN UINT8    PortIndex
  );

typedef
BOOLEAN
(EFIAPI *PEI_IsDmiStack) (
  IN  UINT8             Stack
  );

typedef
BOOLEAN
(EFIAPI *PEI_IsCpuAndRevision) (
  IN UINT8  CpuType,
  IN UINT16 Revision
  );

typedef
UINT8
(EFIAPI *PEI_GetMaxStackNumPerSocket) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_IioIsStackPresent) (
  IN UINT8     IioIndex,
  IN UINT8     StackIndex
  );

typedef
UINT8
(EFIAPI *PEI_GetMaxPortNumPerStack) (
  IN  UINT8   Stack
  );

//
// From KtiApi.h
//
typedef
BOOLEAN
(EFIAPI *PEI_SocketPresent) (
  IN UINT32     SocId
  );

/**
  Get SVID Mapping from Socket and MCID

  @param[in] Socket    - Socket Id - 0 based
  @param[in] McId      - Memory controller 0 based
  @param[in] SvidValue - SVID Value

  @retval EFI_SUCCESS   - Value found
  @retval EFI_NOT_FOUND - Value not found

**/
typedef
EFI_STATUS
(EFIAPI *PEI_GetSvidMap) (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 *SvidValue
  );

typedef
UINT16
(EFIAPI *PEI_PmcGetAcpiBase) (
  VOID
  );

typedef
UINT16
(EFIAPI *PEI_PmcGetAcpiBaseByPchId) (
  IN  UINT8           PchId
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchLpcIoDecodeRangesSet) (
  IN  UINT16                            LpcIoDecodeRanges
  );

typedef
VOID
(EFIAPI *PEI_CheckPowerOffNow) (
  VOID
  );

typedef
VOID
(EFIAPI *PEI_PmcSetPlatformStateAfterPowerFailure) (
  IN UINT8 PowerStateAfterG3
  );

typedef
VOID
(EFIAPI *PEI_PmcClearPowerFailureStatus) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_PmcIsPowerFailureDetected) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchLpcGenIoRangeSet) (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchLpcIoEnableDecodingSet) (
  IN  UINT16                            LpcIoEnableDecoding
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchPcrAndThenOr32) (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

typedef
EFI_STATUS
(EFIAPI *PEI_PchPcrRead32ByPchId) (
  IN  UINT8                             PchId,
  IN  PCH_SBI_PID                       Pid,
  IN  UINT16                            Offset,
  OUT UINT32                            *OutData
  );

typedef
UINT8
(EFIAPI *PEI_ReadNmiEn) (
  VOID
  );

typedef
UINT8
(EFIAPI *PEI_GetPchXhciMaxUsb2PortNum) (
  VOID
  );

typedef
UINT8
(EFIAPI *PEI_GetPchXhciMaxUsb3PortNum) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_PchIsGbeAvailable) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *PEI_EnableMcaOnCacheableMmio) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *PEI_X2ApicIdDetect) (
  IN VOID  *Host
  );

//
// Abstracting silicon functional implementations from OpenBoardPkg code
//
typedef struct {
  UINT32                                    Signature;
  UINT32                                    Version;

  PEI_GET_GPIO_INPUT_VALUE                  GpioGetInputValue;
  PEI_SET_GPIO_OUTPUT_VALUE                 GpioSetOutputValue;
  PEI_GPIO_SET_PAD                          GpioSetPadConfig;

  PEI_GET_PCH_DEVICE_ID                     GetPchLpcDeviceId;
  PEI_GET_PCH_SERIES                        GetPchSeries;
  PEI_GET_PCH_STEPPING                      PchStepping;
  PEI_IS_PCH_GBE_REGION_VALID               PchIsGbeRegionValid;
  PEI_GET_PCH_GBE_PORT_NUMBER               PchGetGbePortNumber;
  PEI_IS_PCH_GBE_PRESENT                    PchIsGbePresent;

  PEI_PchDisableGbeByPchId                  PchDisableGbeByPchId;
  PEI_GpioConfigurePadsByPchId              GpioConfigurePadsByPchId;
  PEI_GpioSetPadConfigByPchId               GpioSetPadConfigByPchId;
  PEI_GpioGetInputValueByPchId              GpioGetInputValueByPchId;
  PEI_PeiGetCurrenClockingMode              PeiGetCurrenClockingMode;
  PEI_GetPchPcieRpDevFunByPchId             GetPchPcieRpDevFunByPchId;
  PEI_IsSimicsEnvironment                   IsSimicsEnvironment;
  PEI_GetPchMaxSataPortNum                  GetPchMaxSataPortNum;
  PEI_GetPchMaxsSataPortNum                 GetPchMaxsSataPortNum;
  PEI_GetPchMaxPciePortNum                  GetPchMaxPciePortNum;
  PEI_PchGetSataLaneNumByPchId              PchGetSataLaneNumByPchId;
  PEI_PchGetsSataLaneNumByPchId             PchGetsSataLaneNumByPchId;
  PEI_PchGetPcieLaneNumByPchId              PchGetPcieLaneNumByPchId;
  PEI_MmPciBase                             MmPciBase;
  PEI_HybridSystemLevelEmulationEnabled     HybridSystemLevelEmulationEnabled;
  PEI_IioIsSocketPresent                    IioIsSocketPresent;
  PEI_GetMaxPortNumPerSocket                GetMaxPortNumPerSocket;
  PEI_IsVMDEnabledForPort                   IsVMDEnabledForPort;
  PEI_IioAreLanesAssignedToPort             IioAreLanesAssignedToPort;
  PEI_GetPortIndexbyStack                   GetPortIndexbyStack;
  PEI_IsDmiStack                            IsDmiStack;
  PEI_IsCpuAndRevision                      IsCpuAndRevision;
  PEI_PchDisableGbe                         PchDisableGbe;
  PEI_GetMaxStackNumPerSocket               GetMaxStackNumPerSocket;
  PEI_IioIsStackPresent                     IioIsStackPresent;
  PEI_GetMaxPortNumPerStack                 GetMaxPortNumPerStack;
  PEI_SocketPresent                         SocketPresent;
  PEI_GetSvidMap                            GetSvidMap;

  PEI_PmcGetAcpiBase                        PmcGetAcpiBase;
  PEI_PmcGetAcpiBaseByPchId                 PmcGetAcpiBaseByPchId;
  PEI_PchLpcIoDecodeRangesSet               PchLpcIoDecodeRangesSet;
  PEI_CheckPowerOffNow                      CheckPowerOffNow;
  PEI_PmcSetPlatformStateAfterPowerFailure  PmcSetPlatformStateAfterPowerFailure;
  PEI_PmcClearPowerFailureStatus            PmcClearPowerFailureStatus;
  PEI_PmcIsPowerFailureDetected             PmcIsPowerFailureDetected;
  PEI_PchLpcGenIoRangeSet                   PchLpcGenIoRangeSet;
  PEI_PchLpcIoEnableDecodingSet             PchLpcIoEnableDecodingSet;
  PEI_PchPcrAndThenOr32                     PchPcrAndThenOr32;
  PEI_PchPcrRead32ByPchId                   PchPcrRead32ByPchId;
  PEI_ReadNmiEn                             ReadNmiEn;

  PEI_GetPchXhciMaxUsb2PortNum              GetPchXhciMaxUsb2PortNum;
  PEI_GetPchXhciMaxUsb3PortNum              GetPchXhciMaxUsb3PortNum;
  PEI_PchIsGbeAvailable                     PchIsGbeAvailable;
  PEI_EnableMcaOnCacheableMmio              EnableMcaOnCacheableMmio;
  PEI_X2ApicIdDetect                        X2ApicIdDetect;
} DYNAMIC_SI_LIBARY_PPI;

extern EFI_GUID gDynamicSiLibraryPpiGuid;

#endif // _DYNAMIC_SI_LIBARY_PPI_H_
