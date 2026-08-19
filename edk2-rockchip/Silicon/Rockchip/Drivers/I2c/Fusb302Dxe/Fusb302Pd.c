/** @file
*
*  USB Power Delivery sink negotiation over the FUSB302.
*
*  This runs once, synchronously, while the port controller driver starts. A
*  background state machine would be the natural shape for an OS driver, but
*  firmware only needs to establish a contract the once, and a blocking
*  sequence avoids doing I2C from a timer callback -- every transfer on this
*  controller raises to TPL_HIGH_LEVEL, which would block the very timer
*  driving the machine.
*
*  The timing this has to meet is modest because the FUSB302 answers incoming
*  messages with GoodCRC in hardware (SWITCHES1.AUTO_GCRC), covering
*  tReceiverResponse of 15 ms. Firmware is left with tSenderResponse, 30 ms,
*  which a 500 us poll meets with room to spare.
*
*  Sink only. Sourcing power is deliberately not implemented: it would mean
*  this driver deciding to energise VBUS, which is a decision better left to
*  the OS, and no board here needs it during boot.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2c.h>
#include <Protocol/UsbTypeCPort.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Pi/PiI2c.h>

#include "Fusb302Dxe.h"

/**
  Prepare the PD transmitter and receiver on the CC pin the partner is on.
**/
STATIC
EFI_STATUS
Fusb302PdEnable (
  IN FUSB302_CONTEXT         *Context,
  IN USB_TYPE_C_ORIENTATION  Orientation
  )
{
  EFI_STATUS  Status;
  BOOLEAN     UseCc2;
  UINT8       Switches0;
  UINT8       Switches1;

  UseCc2 = (Orientation == UsbTypeCOrientationFlipped);

  //
  // Keep the sink terminations, and route the BMC receiver to the live pin.
  //
  Switches0 = FUSB302_SWITCHES0_CC1_PD_EN | FUSB302_SWITCHES0_CC2_PD_EN |
              (UseCc2 ? FUSB302_SWITCHES0_MEAS_CC2 : FUSB302_SWITCHES0_MEAS_CC1);

  Status = Fusb302RegWrite (Context, FUSB302_REG_SWITCHES0, Switches0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Transmit on the same pin, answer with GoodCRC in hardware, and speak
  // revision 2.0 as a sink and UFP. Revision 2.0 is understood by every
  // 3.0 source, which saves implementing the extended message support that
  // advertising 3.0 would oblige us to handle.
  //
  Switches1 = (UseCc2 ? FUSB302_SWITCHES1_TXCC2_EN : FUSB302_SWITCHES1_TXCC1_EN) |
              FUSB302_SWITCHES1_AUTO_GCRC |
              (PD_REV_2_0 << FUSB302_SWITCHES1_SPECREV_SHIFT) |
              (Context->DataRoleDfp ? FUSB302_SWITCHES1_DATAROLE : 0);

  Status = Fusb302RegWrite (Context, FUSB302_REG_SWITCHES1, Switches1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Discard anything left in the FIFOs from before we took over.
  //
  Status = Fusb302RegUpdate (
             Context,
             FUSB302_REG_CONTROL1,
             FUSB302_CONTROL1_RX_FLUSH,
             FUSB302_CONTROL1_RX_FLUSH
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302RegUpdate (
             Context,
             FUSB302_REG_CONTROL0,
             FUSB302_CONTROL0_TX_FLUSH,
             FUSB302_CONTROL0_TX_FLUSH
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->MessageId  = 0;
  Context->DataRoleDfp = FALSE;

  return EFI_SUCCESS;
}

/**
  Stop driving the CC line, leaving the part as the OS driver expects to find
  an idle port.
**/
STATIC
VOID
Fusb302PdDisable (
  IN FUSB302_CONTEXT  *Context
  )
{
  Fusb302RegWrite (Context, FUSB302_REG_SWITCHES1, PD_REV_2_0 << FUSB302_SWITCHES1_SPECREV_SHIFT);
  Fusb302RegWrite (
    Context,
    FUSB302_REG_SWITCHES0,
    FUSB302_SWITCHES0_CC1_PD_EN | FUSB302_SWITCHES0_CC2_PD_EN
    );
}

/**
  Send one PD message.

  The payload is framed with the SOP ordered set and handed to the transmitter;
  the controller appends the CRC itself in response to the JAM_CRC token.
**/
EFI_STATUS
Fusb302PdSend (
  IN FUSB302_CONTEXT  *Context,
  IN UINT16           Header,
  IN CONST UINT32     *Objects
  )
{
  EFI_STATUS  Status;
  //
  // 4 SOP tokens, PACKSYM, the header, the data objects, then JAM_CRC,
  // EOP, TXOFF and TXON.
  //
  UINT8       Frame[4 + 1 + sizeof (UINT16) + (PD_MAX_DATA_OBJECTS * sizeof (UINT32)) + 4];
  UINTN       Length;
  UINTN       Count;
  UINTN       Index;

  Count  = PD_HEADER_COUNT (Header);
  Length = 0;

  Frame[Length++] = FUSB302_TKN_SYNC1;
  Frame[Length++] = FUSB302_TKN_SYNC1;
  Frame[Length++] = FUSB302_TKN_SYNC1;
  Frame[Length++] = FUSB302_TKN_SYNC2;
  Frame[Length++] = FUSB302_TKN_PACKSYM | (UINT8)(sizeof (UINT16) + (Count * sizeof (UINT32)));

  Frame[Length++] = (UINT8)(Header & 0xFF);
  Frame[Length++] = (UINT8)(Header >> 8);

  for (Index = 0; Index < Count; Index++) {
    Frame[Length++] = (UINT8)(Objects[Index] & 0xFF);
    Frame[Length++] = (UINT8)((Objects[Index] >> 8) & 0xFF);
    Frame[Length++] = (UINT8)((Objects[Index] >> 16) & 0xFF);
    Frame[Length++] = (UINT8)((Objects[Index] >> 24) & 0xFF);
  }

  Frame[Length++] = FUSB302_TKN_JAMCRC;
  Frame[Length++] = FUSB302_TKN_EOP;
  Frame[Length++] = FUSB302_TKN_TXOFF;
  Frame[Length++] = FUSB302_TKN_TXON;

  //
  // The FIFO is a single register written repeatedly, so this cannot be one
  // block transfer.
  //
  for (Index = 0; Index < Length; Index++) {
    Status = Fusb302RegWrite (Context, FUSB302_REG_FIFOS, Frame[Index]);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Read one PD message if the receiver has one waiting.

  @retval EFI_SUCCESS     A message was read into Message.
  @retval EFI_NOT_READY   The receive FIFO is empty.

**/
STATIC
EFI_STATUS
Fusb302PdReceive (
  IN  FUSB302_CONTEXT  *Context,
  OUT PD_MESSAGE       *Message
  )
{
  EFI_STATUS  Status;
  UINT8       Status1;
  UINT8       Token;
  UINT8       Byte;
  UINTN       Count;
  UINTN       Index;
  UINTN       ByteIndex;

  Status = Fusb302RegRead (Context, FUSB302_REG_STATUS1, &Status1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((Status1 & FUSB302_STATUS1_RX_EMPTY) != 0) {
    return EFI_NOT_READY;
  }

  Status = Fusb302RegRead (Context, FUSB302_REG_FIFOS, &Token);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((Token & FUSB302_RX_TOKEN_MASK) != FUSB302_RX_TOKEN_SOP) {
    //
    // Something other than SOP: cable plug messages and hard reset signalling
    // land here. Drop it and let the caller poll again.
    //
    Fusb302RegUpdate (
      Context,
      FUSB302_REG_CONTROL1,
      FUSB302_CONTROL1_RX_FLUSH,
      FUSB302_CONTROL1_RX_FLUSH
      );
    return EFI_NOT_READY;
  }

  Message->Header = 0;
  for (ByteIndex = 0; ByteIndex < sizeof (UINT16); ByteIndex++) {
    Status = Fusb302RegRead (Context, FUSB302_REG_FIFOS, &Byte);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Message->Header |= (UINT16)Byte << (ByteIndex * 8);
  }

  Count = PD_HEADER_COUNT (Message->Header);
  if (Count > PD_MAX_DATA_OBJECTS) {
    Fusb302RegUpdate (
      Context,
      FUSB302_REG_CONTROL1,
      FUSB302_CONTROL1_RX_FLUSH,
      FUSB302_CONTROL1_RX_FLUSH
      );
    return EFI_NOT_READY;
  }

  for (Index = 0; Index < Count; Index++) {
    Message->Objects[Index] = 0;
    for (ByteIndex = 0; ByteIndex < sizeof (UINT32); ByteIndex++) {
      Status = Fusb302RegRead (Context, FUSB302_REG_FIFOS, &Byte);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Message->Objects[Index] |= (UINT32)Byte << (ByteIndex * 8);
    }
  }

  //
  // The trailing CRC is checked by the controller, but it still has to be
  // drained out of the FIFO.
  //
  for (ByteIndex = 0; ByteIndex < 4; ByteIndex++) {
    Fusb302RegRead (Context, FUSB302_REG_FIFOS, &Byte);
  }

  return EFI_SUCCESS;
}

/**
  Wait for a specific message, discarding anything else that arrives.

  GoodCRC is skipped throughout: the controller generates and consumes those
  itself, but one can still surface here.

  @param[in]  WantData    TRUE to match a data message, FALSE for a control one.
  @param[in]  Type        Message type to wait for.

**/
EFI_STATUS
Fusb302PdWaitFor (
  IN  FUSB302_CONTEXT  *Context,
  IN  BOOLEAN          WantData,
  IN  UINT8            Type,
  IN  UINTN            TimeoutUs,
  OUT PD_MESSAGE       *Message
  )
{
  EFI_STATUS  Status;
  UINTN       Waited;
  UINTN       Count;

  for (Waited = 0; Waited < TimeoutUs; Waited += PD_POLL_INTERVAL_US) {
    Status = Fusb302PdReceive (Context, Message);
    if (Status == EFI_NOT_READY) {
      MicroSecondDelay (PD_POLL_INTERVAL_US);
      continue;
    }

    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (PD_HEADER_EXTENDED (Message->Header)) {
      continue;
    }

    Count = PD_HEADER_COUNT (Message->Header);

    if (!WantData && (Count == 0) && (PD_HEADER_TYPE (Message->Header) == Type)) {
      return EFI_SUCCESS;
    }

    if (WantData && (Count > 0) && (PD_HEADER_TYPE (Message->Header) == Type)) {
      return EFI_SUCCESS;
    }

    //
    // A Reject or a Wait in answer to our Request is final; report it rather
    // than spinning until the timeout.
    //
    if (!WantData && (Count == 0) &&
        ((PD_HEADER_TYPE (Message->Header) == PD_CTRL_REJECT) ||
         (PD_HEADER_TYPE (Message->Header) == PD_CTRL_WAIT)))
    {
      return EFI_ABORTED;
    }
  }

  return EFI_TIMEOUT;
}

/**
  Pick the best fixed supply the source offers that stays within what the
  board allows.

  Highest voltage wins, since that is what reduces the current the board has
  to draw. Boards opt in to anything above 5 V explicitly, because the VBUS
  path has to be rated for it.

  @param[out] Position   1-based index of the chosen object.

**/
STATIC
EFI_STATUS
Fusb302PdSelectPdo (
  IN  CONST PD_MESSAGE  *SourceCap,
  IN  UINT32            MaxVoltageMv,
  IN  UINT32            MaxCurrentMa,
  OUT UINTN             *Position,
  OUT UINT32            *VoltageMv,
  OUT UINT32            *CurrentMa
  )
{
  UINTN   Count;
  UINTN   Index;
  UINT32  Pdo;
  UINT32  Voltage;
  UINT32  Current;

  Count      = PD_HEADER_COUNT (SourceCap->Header);
  *Position  = 0;
  *VoltageMv = 0;
  *CurrentMa = 0;

  for (Index = 0; Index < Count; Index++) {
    Pdo = SourceCap->Objects[Index];

    if (PD_PDO_TYPE (Pdo) != PD_PDO_TYPE_FIXED) {
      continue;
    }

    Voltage = PD_PDO_FIXED_VOLTAGE_MV (Pdo);
    Current = PD_PDO_FIXED_CURRENT_MA (Pdo);

    DEBUG ((
      DEBUG_INFO,
      "%a:   PDO %u: fixed %u mV, %u mA\n",
      __func__,
      (UINT32)Index + 1,
      Voltage,
      Current
      ));

    if ((Voltage == 0) || (Voltage > MaxVoltageMv)) {
      continue;
    }

    if (Voltage > *VoltageMv) {
      *Position  = Index + 1;
      *VoltageMv = Voltage;
      *CurrentMa = MIN (Current, MaxCurrentMa);
    }
  }

  return (*Position == 0) ? EFI_NOT_FOUND : EFI_SUCCESS;
}

EFI_STATUS
Fusb302PdNegotiateSink (
  IN OUT FUSB302_CONTEXT         *Context,
  IN     USB_TYPE_C_ORIENTATION  Orientation
  )
{
  EFI_STATUS  Status;
  PD_MESSAGE  Message;
  UINT16      Header;
  UINT32      Rdo;
  UINTN       Position;
  UINT32      VoltageMv;
  UINT32      CurrentMa;
  UINT32      MaxVoltageMv;
  UINT32      MaxCurrentMa;

  MaxVoltageMv = PcdGet32 (PcdFusb302MaxVoltageMv);
  MaxCurrentMa = PcdGet32 (PcdFusb302MaxCurrentMa);

  Status = Fusb302PdEnable (Context, Orientation);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // A source repeats Source_Capabilities until something answers, so listening
  // is usually enough to join the conversation.
  //
  Status = Fusb302PdWaitFor (
             Context,
             TRUE,
             PD_DATA_SOURCE_CAP,
             PD_SOURCE_CAP_TIMEOUT_US,
             &Message
             );
  if (EFI_ERROR (Status)) {
    //
    // Silence does not mean there is no source: once a contract exists the
    // source stops advertising, which is what we find on a warm reboot or
    // where an earlier boot stage already negotiated. Soft_Reset returns both
    // ends to a known protocol state and makes the source advertise again. It
    // does not disturb VBUS, unlike a hard reset, so it is safe to send even
    // when the board is running off this port.
    //
    DEBUG ((DEBUG_INFO, "%a: no advertisement; sending Soft_Reset\n", __func__));

    Header = PD_HEADER_BUILD (
               PD_CTRL_SOFT_RESET,
               0,
               0,
               Context->DataRoleDfp ? 1 : 0,
               0,
               PD_REV_2_0
               );

    Status = Fusb302PdSend (Context, Header, NULL);
    if (EFI_ERROR (Status)) {
      goto Disable;
    }

    //
    // Soft_Reset resets the message counters at both ends.
    //
    Context->MessageId = 0;

    Status = Fusb302PdWaitFor (Context, FALSE, PD_CTRL_ACCEPT, PD_SENDER_RESPONSE_TIMEOUT_US, &Message);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: Soft_Reset unanswered (%r); partner is not a PD source\n", __func__, Status));
      goto Disable;
    }

    Status = Fusb302PdWaitFor (
               Context,
               TRUE,
               PD_DATA_SOURCE_CAP,
               PD_SOURCE_CAP_RETRY_TIMEOUT_US,
               &Message
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: no Source_Capabilities after Soft_Reset (%r)\n", __func__, Status));
      goto Disable;
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: source offers %u power data object(s), board limit %u mV / %u mA\n",
    __func__,
    (UINT32)PD_HEADER_COUNT (Message.Header),
    MaxVoltageMv,
    MaxCurrentMa
    ));

  Status = Fusb302PdSelectPdo (&Message, MaxVoltageMv, MaxCurrentMa, &Position, &VoltageMv, &CurrentMa);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: no acceptable power data object offered\n", __func__));
    goto Disable;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: requesting object %u: %u mV, %u mA\n",
    __func__,
    (UINT32)Position,
    VoltageMv,
    CurrentMa
    ));

  Rdo = PD_RDO_FIXED (Position, CurrentMa, CurrentMa);

  Header = PD_HEADER_BUILD (
             PD_DATA_REQUEST,
             Context->MessageId,
             1,
             Context->DataRoleDfp ? 1 : 0,
             0,
             PD_REV_2_0
             );

  Status = Fusb302PdSend (Context, Header, &Rdo);
  if (EFI_ERROR (Status)) {
    goto Disable;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  Status = Fusb302PdWaitFor (Context, FALSE, PD_CTRL_ACCEPT, PD_SENDER_RESPONSE_TIMEOUT_US, &Message);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: request not accepted (%r)\n", __func__, Status));
    goto Disable;
  }

  //
  // The source now moves the supply; PS_RDY says it has arrived.
  //
  Status = Fusb302PdWaitFor (Context, FALSE, PD_CTRL_PS_RDY, PD_PS_TRANSITION_TIMEOUT_US, &Message);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: no PS_RDY after Accept (%r)\n", __func__, Status));
    goto Disable;
  }

  Context->Contract.PdNegotiated = TRUE;
  Context->Contract.VoltageMv    = VoltageMv;
  Context->Contract.CurrentMa    = CurrentMa;

  DEBUG ((
    DEBUG_INFO,
    "%a: contract established: %u mV, %u mA\n",
    __func__,
    VoltageMv,
    CurrentMa
    ));

  //
  // Deliberately leave the receiver running rather than tearing the link down:
  // dropping off the line after a contract would have the source treat us as
  // gone and drop back to 5 V, undoing the negotiation.
  //
  return EFI_SUCCESS;

Disable:
  Fusb302PdDisable (Context);
  return Status;
}

EFI_STATUS
Fusb302PdDataRoleSwapToDfp (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  PD_MESSAGE  Message;
  UINT16      Header;

  if (Context->DataRoleDfp) {
    return EFI_SUCCESS;
  }

  Header = PD_HEADER_BUILD (PD_CTRL_DR_SWAP, Context->MessageId, 0, 0, 0, PD_REV_2_0);

  Status = Fusb302PdSend (Context, Header, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  Status = Fusb302PdWaitFor (Context, FALSE, PD_CTRL_ACCEPT, PD_SENDER_RESPONSE_TIMEOUT_US, &Message);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: partner refused the data role swap (%r)\n", __func__, Status));
    return Status;
  }

  //
  // The controller builds its automatic GoodCRC replies from these bits, so it
  // has to be told about the new role as well; a GoodCRC disagreeing with our
  // own messages gets us ignored.
  //
  Status = Fusb302RegUpdate (
             Context,
             FUSB302_REG_SWITCHES1,
             FUSB302_SWITCHES1_DATAROLE,
             FUSB302_SWITCHES1_DATAROLE
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->DataRoleDfp = TRUE;

  DEBUG ((DEBUG_INFO, "%a: now the downstream facing port for data\n", __func__));

  return EFI_SUCCESS;
}
