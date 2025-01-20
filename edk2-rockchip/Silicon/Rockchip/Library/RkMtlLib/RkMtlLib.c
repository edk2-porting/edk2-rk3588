/** @file

  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>
  Copyright (c) 2017-2018, Arm Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  System Control and Management Interface V1.0
    http://infocenter.arm.com/help/topic/com.arm.doc.den0056a/
    DEN0056A_System_Control_and_Management_Interface.pdf
**/

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/ArmLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/ArmMtlLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "RkMtlPrivateLib.h"

// Shared mailbox
STATIC CONST MTL_CHANNEL  Channels[NUM_CHANNELS] = {
  {
    MTL_CHANNEL_TYPE_LOW,
    (MTL_MAILBOX *)(MTL_MAILBOX_BASE)
  },
};

/** Wait until channel is free.

  @param[in] Channel                Pointer to a channel.
  @param[in] TimeOutInMicroSeconds  Time out in micro seconds.

  @retval EFI_SUCCESS               Channel is free.
  @retval EFI_TIMEOUT               Time out error.
**/
EFI_STATUS
MtlWaitUntilChannelFree (
  IN MTL_CHANNEL  *Channel,
  IN UINTN        TimeOutInMicroSeconds
  )
{
  while (TimeOutInMicroSeconds != 0) {
    ArmDataSynchronizationBarrier ();

    // If channel is free then we have received the reply.
    if (Channel->MailBox->ChannelStatus == MTL_CHANNEL_FREE) {
      return EFI_SUCCESS;
    }

    if (TimeOutInMicroSeconds < MTL_POLL_WAIT_TIME) {
      gBS->Stall (TimeOutInMicroSeconds);
      break;
    }

    // Wait for some arbitrary time.
    gBS->Stall (MTL_POLL_WAIT_TIME);
    TimeOutInMicroSeconds -= MTL_POLL_WAIT_TIME;
  }

  // No response from SCP.
  if (Channel->MailBox->ChannelStatus != MTL_CHANNEL_FREE) {
    ASSERT (FALSE);
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/** Return the address of the message payload.

  @param[in] Channel   Pointer to a channel.

  @retval UINT32*      Pointer to the payload.
**/
UINT32 *
MtlGetChannelPayload (
  IN  MTL_CHANNEL  *Channel
  )
{
  return Channel->MailBox->Payload;
}

/** Return pointer to a channel for the requested channel type.

  @param[in] ChannelType        ChannelType, only supports Low priority channel.
                                MTL_CHANNEL_TYPE_LOW

  @param[out] Channel           Holds pointer to the channel.

  @retval EFI_SUCCESS           Pointer to channel is returned.
  @retval EFI_UNSUPPORTED       Requested channel type not supported.
**/
EFI_STATUS
MtlGetChannel (
  IN  MTL_CHANNEL_TYPE  ChannelType,
  OUT MTL_CHANNEL       **Channel
  )
{
  if (ChannelType != MTL_CHANNEL_TYPE_LOW) {
    return EFI_UNSUPPORTED;
  }

  *Channel = (MTL_CHANNEL *)&Channels[ChannelType];

  return EFI_SUCCESS;
}

/** Mark the channel busy and ring the doorbell.

  @param[in] Channel               Pointer to a channel.
  @param[in] MessageHeader         Message header.

  @param[out] PayloadLength        Message length.

  @retval EFI_SUCCESS              Message sent successfully.
  @retval EFI_DEVICE_ERROR         Channel is busy.
**/
EFI_STATUS
MtlSendMessage (
  IN  MTL_CHANNEL  *Channel,
  IN  UINT32       MessageHeader,
  OUT UINT32       PayloadLength
  )
{
  MTL_MAILBOX   *MailBox = Channel->MailBox;
  ARM_SMC_ARGS  SmcRegs  = { 0 };

  ArmDataSynchronizationBarrier ();
  if (Channel->MailBox->ChannelStatus != MTL_CHANNEL_FREE) {
    DEBUG ((DEBUG_WARN, "Mailbox is busy\n"));
    return EFI_DEVICE_ERROR;
  }

  // Mark the channel busy before ringing doorbell.
  Channel->MailBox->ChannelStatus = MTL_CHANNEL_BUSY;
  ArmDataSynchronizationBarrier ();

  MailBox->Flags         = MTL_POLL;
  MailBox->MessageHeader = MessageHeader;

  // Add length of message header.
  MailBox->Length = PayloadLength + sizeof (MessageHeader);

  ArmDataSynchronizationBarrier ();

  DEBUG ((
    DEBUG_INFO,
    "MtlSendMessage ringing doorbell 0x%08X with message header 0x%08X length 0x%08X\n",
    FixedPcdGet32 (PcdRkMtlMailBoxSmcId),
    MailBox->MessageHeader,
    MailBox->Length
    ));

  // Ring the doorbell.
  SmcRegs.Arg0 = FixedPcdGet32 (PcdRkMtlMailBoxSmcId);
  ArmCallSmc (&SmcRegs);

  if (SmcRegs.Arg0 != 0) {
    DEBUG ((DEBUG_WARN, "SMC doorbell call failed: 0x%lX\n", FixedPcdGet32 (PcdRkMtlMailBoxSmcId), SmcRegs.Arg0));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/** Wait for a response on a channel.

  If channel is free after sending message, it implies SCP responded
  with a response on the channel.

  @param[in] Channel               Pointer to a channel.

  @retval EFI_SUCCESS              Message received successfully.
  @retval EFI_TIMEOUT              Time out error.
**/
EFI_STATUS
MtlReceiveMessage (
  IN  MTL_CHANNEL  *Channel,
  OUT UINT32       *MessageHeader,
  OUT UINT32       *PayloadLength
  )
{
  EFI_STATUS  Status;

  MTL_MAILBOX  *MailBox = Channel->MailBox;

  Status = MtlWaitUntilChannelFree (Channel, RESPONSE_TIMEOUT);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *MessageHeader = MailBox->MessageHeader;

  // Deduct message header length.
  *PayloadLength = MailBox->Length - sizeof (*MessageHeader);

  return EFI_SUCCESS;
}
