/** @file
  Mailbox Library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Library/BaseLib.h>

/**
  Generic Mailbox function for mailbox write commands. This function will
  poll the mailbox interface for control, issue the write request, poll
  for completion, and verify the write was successful.

  @param[in]  MailboxType     The type of mailbox interface to read. The Overclocking mailbox is defined as MAILBOX_TYPE_OC = 2.
  @param[in]  MailboxCommand  Overclocking mailbox command data
  @param[in]  MailboxData     Overclocking mailbox interface data
  @param[out] *MailboxStatus  Pointer to the mailbox status returned from pcode. Possible mailbox status values are:
                              - SUCCESS (0)               Command succeeded.
                              - OC_LOCKED (1)             Overclocking is locked. Service is read-only.
                              - INVALID_DOMAIN (2)        Invalid Domain ID provided in command data.
                              - MAX_RATIO_EXCEEDED (3)    Ratio exceeds maximum overclocking limits.
                              - MAX_VOLTAGE_EXCEEDED (4)  Voltage exceeds input VR's max voltage.
                              - OC_NOT_SUPPORTED (5)      Domain does not support overclocking.

  @retval    EFI_SUCCESS           Command succeeded.
  @retval    EFI_INVALID_PARAMETER Invalid read data detected from pcode.
  @retval    EFI_UNSUPPORTED       Unsupported MailboxType parameter.
**/
EFI_STATUS
EFIAPI
MailboxWrite (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request from MailboxType, and populate the read results in the MailboxDataPtr.

  @param[in]  MailboxType     The type of mailbox interface to read. The Overclocking mailbox is defined as MAILBOX_TYPE_OC = 2.
  @param[in]  MailboxCommand  Overclocking mailbox command data
  @param[out] *MailboxDataPtr Pointer to the overclocking mailbox interface data
  @param[out] *MailboxStatus  Pointer to the mailbox status returned from pcode. Possible mailbox status are
                              - SUCCESS (0)               Command succeeded.
                              - OC_LOCKED (1)             Overclocking is locked. Service is read-only.
                              - INVALID_DOMAIN (2)        Invalid Domain ID provided in command data.
                              - MAX_RATIO_EXCEEDED (3)    Ratio exceeds maximum overclocking limits.
                              - MAX_VOLTAGE_EXCEEDED (4)  Voltage exceeds input VR's max voltage.
                              - OC_NOT_SUPPORTED (5)      Domain does not support overclocking.

  @retval EFI_SUCCESS           Command succeeded.
  @retval EFI_INVALID_PARAMETER Invalid read data detected from pcode.
  @retval EFI_UNSUPPORTED       Unsupported MailboxType parameter.
**/
EFI_STATUS
EFIAPI
MailboxRead (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @param[in]  MailboxType

  @retval EFI_SUCCESS           Command succeeded.
  @retval EFI_TIMEOUT           Command timeout.
**/
EFI_STATUS
EFIAPI
PollMailboxReady (
  IN UINT32 MailboxType
  )
{
  return EFI_SUCCESS;
}
