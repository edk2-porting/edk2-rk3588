/** @file
  IPMI Command - NetFnTransport.

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
IpmiSolActivating (
  IN  IPMI_SOL_ACTIVATING_REQUEST  *SolActivatingRequest,
  OUT UINT8                        *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_TRANSPORT,
             IPMI_TRANSPORT_SOL_ACTIVATING,
             (VOID *)SolActivatingRequest,
             sizeof(*SolActivatingRequest),
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiSetSolConfigurationParameters (
  IN  IPMI_SET_SOL_CONFIGURATION_PARAMETERS_REQUEST  *SetConfigurationParametersRequest,
  IN  UINT32                                         SetConfigurationParametersRequestSize,
  OUT UINT8                                          *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_TRANSPORT,
             IPMI_TRANSPORT_SET_SOL_CONFIG_PARAM,
             (VOID *)SetConfigurationParametersRequest,
             SetConfigurationParametersRequestSize,
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSolConfigurationParameters (
  IN  IPMI_GET_SOL_CONFIGURATION_PARAMETERS_REQUEST  *GetConfigurationParametersRequest,
  OUT IPMI_GET_SOL_CONFIGURATION_PARAMETERS_RESPONSE *GetConfigurationParametersResponse,
  IN OUT UINT32                                      *GetConfigurationParametersResponseSize
  )
{
  EFI_STATUS                   Status;

  Status = IpmiSubmitCommand (
             IPMI_NETFN_TRANSPORT,
             IPMI_TRANSPORT_GET_SOL_CONFIG_PARAM,
             (VOID *)GetConfigurationParametersRequest,
             sizeof(*GetConfigurationParametersRequest),
             (VOID *)GetConfigurationParametersResponse,
             GetConfigurationParametersResponseSize
             );
  return Status;
}
