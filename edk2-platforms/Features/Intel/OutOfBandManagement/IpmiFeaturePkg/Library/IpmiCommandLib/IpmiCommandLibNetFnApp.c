/** @file
  IPMI Command - NetFnApp.

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
IpmiGetDeviceId (
  OUT IPMI_GET_DEVICE_ID_RESPONSE  *DeviceId
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*DeviceId);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_GET_DEVICE_ID,
             NULL,
             0,
             (VOID *)DeviceId,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetSelfTestResult (
  OUT IPMI_SELF_TEST_RESULT_RESPONSE   *SelfTestResult
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*SelfTestResult);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_GET_SELFTEST_RESULTS,
             NULL,
             0,
             (VOID *)SelfTestResult,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiResetWatchdogTimer (
  OUT UINT8                            *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_RESET_WATCHDOG_TIMER,
             NULL,
             0,
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiSetWatchdogTimer (
  IN  IPMI_SET_WATCHDOG_TIMER_REQUEST  *SetWatchdogTimer,
  OUT UINT8                            *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_SET_WATCHDOG_TIMER,
             (VOID *)SetWatchdogTimer,
             sizeof(*SetWatchdogTimer),
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetWatchdogTimer (
  OUT IPMI_GET_WATCHDOG_TIMER_RESPONSE *GetWatchdogTimer
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetWatchdogTimer);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_GET_WATCHDOG_TIMER,
             NULL,
             0,
             (VOID *)GetWatchdogTimer,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiSetBmcGlobalEnables (
  IN  IPMI_SET_BMC_GLOBAL_ENABLES_REQUEST  *SetBmcGlobalEnables,
  OUT UINT8                                *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_SET_BMC_GLOBAL_ENABLES,
             (VOID *)SetBmcGlobalEnables,
             sizeof(*SetBmcGlobalEnables),
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetBmcGlobalEnables (
  OUT IPMI_GET_BMC_GLOBAL_ENABLES_RESPONSE *GetBmcGlobalEnables
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetBmcGlobalEnables);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_GET_BMC_GLOBAL_ENABLES,
             NULL,
             0,
             (VOID *)GetBmcGlobalEnables,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiClearMessageFlags (
  IN  IPMI_CLEAR_MESSAGE_FLAGS_REQUEST  *ClearMessageFlagsRequest,
  OUT UINT8                             *CompletionCode
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*CompletionCode);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_CLEAR_MESSAGE_FLAGS,
             (VOID *)ClearMessageFlagsRequest,
             sizeof(*ClearMessageFlagsRequest),
             (VOID *)CompletionCode,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetMessageFlags (
  OUT IPMI_GET_MESSAGE_FLAGS_RESPONSE *GetMessageFlagsResponse
  )
{
  EFI_STATUS                   Status;
  UINT32                       DataSize;

  DataSize = sizeof(*GetMessageFlagsResponse);
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_GET_MESSAGE_FLAGS,
             NULL,
             0,
             (VOID *)GetMessageFlagsResponse,
             &DataSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiGetMessage (
  OUT IPMI_GET_MESSAGE_RESPONSE *GetMessageResponse,
  IN OUT UINT32                 *GetMessageResponseSize
  )
{
  EFI_STATUS                   Status;

  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_GET_MESSAGE,
             NULL,
             0,
             (VOID *)GetMessageResponse,
             GetMessageResponseSize
             );
  return Status;
}

EFI_STATUS
EFIAPI
IpmiSendMessage (
  IN  IPMI_SEND_MESSAGE_REQUEST  *SendMessageRequest,
  IN  UINT32                     SendMessageRequestSize,
  OUT IPMI_SEND_MESSAGE_RESPONSE *SendMessageResponse,
  IN OUT UINT32                  *SendMessageResponseSize
  )
{
  EFI_STATUS                   Status;

  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_SEND_MESSAGE,
             (VOID *)SendMessageRequest,
             SendMessageRequestSize,
             (VOID *)SendMessageResponse,
             SendMessageResponseSize
             );
  return Status;
}
