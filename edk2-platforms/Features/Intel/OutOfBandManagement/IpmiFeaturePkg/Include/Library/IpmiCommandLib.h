/** @file
  This library abstract how to send/receive IPMI command.

Copyright (c) 2018-2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _IPMI_COMMAND_LIB_H_
#define _IPMI_COMMAND_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Ipmi.h>

//
// NetFnApp
//
EFI_STATUS
EFIAPI
IpmiGetDeviceId (
  OUT IPMI_GET_DEVICE_ID_RESPONSE  *DeviceId
  );

EFI_STATUS
EFIAPI
IpmiGetSelfTestResult (
  OUT IPMI_SELF_TEST_RESULT_RESPONSE   *SelfTestResult
  );

EFI_STATUS
EFIAPI
IpmiResetWatchdogTimer (
  OUT UINT8                            *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiSetWatchdogTimer (
  IN  IPMI_SET_WATCHDOG_TIMER_REQUEST  *SetWatchdogTimer,
  OUT UINT8                            *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiGetWatchdogTimer (
  OUT IPMI_GET_WATCHDOG_TIMER_RESPONSE *GetWatchdogTimer
  );

EFI_STATUS
EFIAPI
IpmiSetBmcGlobalEnables (
  IN  IPMI_SET_BMC_GLOBAL_ENABLES_REQUEST  *SetBmcGlobalEnables,
  OUT UINT8                                *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiGetBmcGlobalEnables (
  OUT IPMI_GET_BMC_GLOBAL_ENABLES_RESPONSE *GetBmcGlobalEnables
  );

EFI_STATUS
EFIAPI
IpmiClearMessageFlags (
  IN  IPMI_CLEAR_MESSAGE_FLAGS_REQUEST  *ClearMessageFlagsRequest,
  OUT UINT8                             *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiGetMessageFlags (
  OUT IPMI_GET_MESSAGE_FLAGS_RESPONSE *GetMessageFlagsResponse
  );

EFI_STATUS
EFIAPI
IpmiGetMessage (
  OUT IPMI_GET_MESSAGE_RESPONSE *GetMessageResponse,
  IN OUT UINT32                 *GetMessageResponseSize
  );

EFI_STATUS
EFIAPI
IpmiSendMessage (
  IN  IPMI_SEND_MESSAGE_REQUEST  *SendMessageRequest,
  IN  UINT32                     SendMessageRequestSize,
  OUT IPMI_SEND_MESSAGE_RESPONSE *SendMessageResponse,
  IN OUT UINT32                  *SendMessageResponseSize
  );

//
// NetFnTransport
//
EFI_STATUS
EFIAPI
IpmiSolActivating (
  IN  IPMI_SOL_ACTIVATING_REQUEST  *SolActivatingRequest,
  OUT UINT8                        *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiSetSolConfigurationParameters (
  IN  IPMI_SET_SOL_CONFIGURATION_PARAMETERS_REQUEST  *SetConfigurationParametersRequest,
  IN  UINT32                                         SetConfigurationParametersRequestSize,
  OUT UINT8                                          *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiGetSolConfigurationParameters (
  IN  IPMI_GET_SOL_CONFIGURATION_PARAMETERS_REQUEST  *GetConfigurationParametersRequest,
  OUT IPMI_GET_SOL_CONFIGURATION_PARAMETERS_RESPONSE *GetConfigurationParametersResponse,
  IN OUT UINT32                                      *GetConfigurationParametersResponseSize
  );

//
// NetFnChasis
//
EFI_STATUS
EFIAPI
IpmiGetChassisCapabilities (
  OUT IPMI_GET_CHASSIS_CAPABILITIES_RESPONSE  *GetChassisCapabilitiesResponse
  );

EFI_STATUS
EFIAPI
IpmiGetChassisStatus (
  OUT IPMI_GET_CHASSIS_STATUS_RESPONSE  *GetChassisStatusResponse
  );

EFI_STATUS
EFIAPI
IpmiChassisControl (
  IN IPMI_CHASSIS_CONTROL_REQUEST  *ChassisControlRequest,
  OUT UINT8                        *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiSetPowerRestorePolicy (
  IN  IPMI_SET_POWER_RESTORE_POLICY_REQUEST  *ChassisControlRequest,
  OUT IPMI_SET_POWER_RESTORE_POLICY_RESPONSE *ChassisControlResponse
  );

//
// NetFnStorage
//
EFI_STATUS
EFIAPI
IpmiSetSystemBootOptions (
  IN  IPMI_SET_BOOT_OPTIONS_REQUEST  *BootOptionsRequest,
  OUT IPMI_SET_BOOT_OPTIONS_RESPONSE *BootOptionsResponse
  );

EFI_STATUS
EFIAPI
IpmiGetSystemBootOptions (
  IN  IPMI_GET_BOOT_OPTIONS_REQUEST  *BootOptionsRequest,
  OUT IPMI_GET_BOOT_OPTIONS_RESPONSE *BootOptionsResponse
  );

//
// NetFnStorage
//
EFI_STATUS
EFIAPI
IpmiGetFruInventoryAreaInfo (
  IN  IPMI_GET_FRU_INVENTORY_AREA_INFO_REQUEST   *GetFruInventoryAreaInfoRequest,
  OUT IPMI_GET_FRU_INVENTORY_AREA_INFO_RESPONSE  *GetFruInventoryAreaInfoResponse
  );

EFI_STATUS
EFIAPI
IpmiReadFruData (
  IN  IPMI_READ_FRU_DATA_REQUEST        *ReadFruDataRequest,
  OUT IPMI_READ_FRU_DATA_RESPONSE       *ReadFruDataResponse,
  IN OUT UINT32                         *ReadFruDataResponseSize
  );

EFI_STATUS
EFIAPI
IpmiWriteFruData (
  IN  IPMI_WRITE_FRU_DATA_REQUEST       *WriteFruDataRequest,
  IN  UINT32                            WriteFruDataRequestSize,
  OUT IPMI_WRITE_FRU_DATA_RESPONSE      *WriteFruDataResponse
  );

EFI_STATUS
EFIAPI
IpmiGetSelInfo (
  OUT IPMI_GET_SEL_INFO_RESPONSE  *GetSelInfoResponse
  );

EFI_STATUS
EFIAPI
IpmiGetSelEntry (
  IN IPMI_GET_SEL_ENTRY_REQUEST    *GetSelEntryRequest,
  OUT IPMI_GET_SEL_ENTRY_RESPONSE  *GetSelEntryResponse,
  IN OUT UINT32                    *GetSelEntryResponseSize
  );

EFI_STATUS
EFIAPI
IpmiAddSelEntry (
  IN IPMI_ADD_SEL_ENTRY_REQUEST    *AddSelEntryRequest,
  OUT IPMI_ADD_SEL_ENTRY_RESPONSE  *AddSelEntryResponse
  );

EFI_STATUS
EFIAPI
IpmiPartialAddSelEntry (
  IN IPMI_PARTIAL_ADD_SEL_ENTRY_REQUEST    *PartialAddSelEntryRequest,
  IN UINT32                                PartialAddSelEntryRequestSize,
  OUT IPMI_PARTIAL_ADD_SEL_ENTRY_RESPONSE  *PartialAddSelEntryResponse
  );

EFI_STATUS
EFIAPI
IpmiClearSel (
  IN IPMI_CLEAR_SEL_REQUEST   *ClearSelRequest,
  OUT IPMI_CLEAR_SEL_RESPONSE *ClearSelResponse
  );

EFI_STATUS
EFIAPI
IpmiGetSelTime (
  OUT IPMI_GET_SEL_TIME_RESPONSE *GetSelTimeResponse
  );

EFI_STATUS
EFIAPI
IpmiSetSelTime (
  IN IPMI_SET_SEL_TIME_REQUEST  *SetSelTimeRequest,
  OUT UINT8                     *CompletionCode
  );

EFI_STATUS
EFIAPI
IpmiGetSdrRepositoryInfo (
  OUT IPMI_GET_SDR_REPOSITORY_INFO_RESPONSE  *GetSdrRepositoryInfoResp
  );

EFI_STATUS
EFIAPI
IpmiGetSdr (
  IN  IPMI_GET_SDR_REQUEST          *GetSdrRequest,
  OUT IPMI_GET_SDR_RESPONSE         *GetSdrResponse,
  IN OUT UINT32                     *GetSdrResponseSize
  );

#endif
