/** @file
  IPMI Command - NetFnStorage.

  Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IpmiBaseLib.h>

#include <IndustryStandard/Ipmi.h>


EFI_STATUS
EFIAPI
IpmiGetFruInventoryAreaInfo (
  IN  IPMI_GET_FRU_INVENTORY_AREA_INFO_REQUEST   *GetFruInventoryAreaInfoRequest,
  OUT IPMI_GET_FRU_INVENTORY_AREA_INFO_RESPONSE  *GetFruInventoryAreaInfoResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetFruInventoryAreaInfoResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_FRU_INVENTORY_AREAINFO,
             (VOID *)GetFruInventoryAreaInfoRequest,
             sizeof(*GetFruInventoryAreaInfoRequest),
             (VOID *)GetFruInventoryAreaInfoResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiReadFruData (
  IN  IPMI_READ_FRU_DATA_REQUEST        *ReadFruDataRequest,
  OUT IPMI_READ_FRU_DATA_RESPONSE       *ReadFruDataResponse,
  IN OUT UINT32                         *ReadFruDataResponseSize
  )
{
  EFI_STATUS                   Status;

  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_READ_FRU_DATA,
             (VOID *)ReadFruDataRequest,
             sizeof(*ReadFruDataRequest),
             (VOID *)ReadFruDataResponse,
             ReadFruDataResponseSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiWriteFruData (
  IN  IPMI_WRITE_FRU_DATA_REQUEST       *WriteFruDataRequest,
  IN  UINT32                            WriteFruDataRequestSize,
  OUT IPMI_WRITE_FRU_DATA_RESPONSE      *WriteFruDataResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*WriteFruDataResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_WRITE_FRU_DATA,
             (VOID *)WriteFruDataRequest,
             WriteFruDataRequestSize,
             (VOID *)WriteFruDataResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSelInfo (
  OUT IPMI_GET_SEL_INFO_RESPONSE  *GetSelInfoResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetSelInfoResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SEL_INFO,
             NULL,
             0,
             (VOID *)GetSelInfoResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSelEntry (
  IN IPMI_GET_SEL_ENTRY_REQUEST    *GetSelEntryRequest,
  OUT IPMI_GET_SEL_ENTRY_RESPONSE  *GetSelEntryResponse,
  IN OUT UINT32                    *GetSelEntryResponseSize
  )
{
  EFI_STATUS                   Status;

  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SEL_ENTRY,
             (VOID *)GetSelEntryRequest,
             sizeof(*GetSelEntryRequest),
             (VOID *)GetSelEntryResponse,
             GetSelEntryResponseSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiAddSelEntry (
  IN IPMI_ADD_SEL_ENTRY_REQUEST    *AddSelEntryRequest,
  OUT IPMI_ADD_SEL_ENTRY_RESPONSE  *AddSelEntryResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*AddSelEntryResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_ADD_SEL_ENTRY,
             (VOID *)AddSelEntryRequest,
             sizeof(*AddSelEntryRequest),
             (VOID *)AddSelEntryResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiPartialAddSelEntry (
  IN IPMI_PARTIAL_ADD_SEL_ENTRY_REQUEST    *PartialAddSelEntryRequest,
  IN UINT32                                PartialAddSelEntryRequestSize,
  OUT IPMI_PARTIAL_ADD_SEL_ENTRY_RESPONSE  *PartialAddSelEntryResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*PartialAddSelEntryResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_PARTIAL_ADD_SEL_ENTRY,
             (VOID *)PartialAddSelEntryRequest,
             PartialAddSelEntryRequestSize,
             (VOID *)PartialAddSelEntryResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiClearSel (
  IN IPMI_CLEAR_SEL_REQUEST   *ClearSelRequest,
  OUT IPMI_CLEAR_SEL_RESPONSE *ClearSelResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*ClearSelResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_CLEAR_SEL,
             (VOID *)ClearSelRequest,
             sizeof(*ClearSelRequest),
             (VOID *)ClearSelResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSelTime (
  OUT IPMI_GET_SEL_TIME_RESPONSE *GetSelTimeResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetSelTimeResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SEL_TIME,
             NULL,
             0,
             (VOID *)GetSelTimeResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiSetSelTime (
  IN IPMI_SET_SEL_TIME_REQUEST  *SetSelTimeRequest,
  OUT UINT8                     *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_SET_SEL_TIME,
             (VOID *)SetSelTimeRequest,
             sizeof(*SetSelTimeRequest),
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSdrRepositoryInfo (
  OUT IPMI_GET_SDR_REPOSITORY_INFO_RESPONSE  *GetSdrRepositoryInfoResp
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetSdrRepositoryInfoResp);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SDR_REPOSITORY_INFO,
             NULL,
             0,
             (VOID *)GetSdrRepositoryInfoResp,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSdr (
  IN  IPMI_GET_SDR_REQUEST          *GetSdrRequest,
  OUT IPMI_GET_SDR_RESPONSE         *GetSdrResponse,
  IN OUT UINT32                     *GetSdrResponseSize
  )
{
  EFI_STATUS                   Status;

  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SDR,
             (VOID *)GetSdrRequest,
             sizeof(*GetSdrRequest),
             (VOID *)GetSdrResponse,
             GetSdrResponseSize
             );
  return Status;
}
