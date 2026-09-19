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
*  This is the sinking half. Sourcing lives in Fusb302Source.c, and the two
*  share the message handling here: replies take their power role from the
*  context, so the same code answers a partner whichever end is supplying.
*
*  Copyright (c) 2026 Corey Moyer <cronmod.dev@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/UsbTypeCPort.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
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
  // Enabling the transmitter is a fresh start for the protocol, and a reset
  // returns the data role to the default for our power role. Settle that
  // before the role reaches SWITCHES1 below, or the controller answers with
  // GoodCRC carrying a role we no longer believe we hold.
  //
  Context->DataRoleDfp = FALSE;

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

  Context->MessageId = 0;

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

  //
  // A header promising objects with none to send would walk off the end of
  // the caller's argument. Checked rather than asserted: the header and the
  // objects come from different places in the sourcing path, and a release
  // build should not frame a malformed message either.
  //
  ASSERT ((Count == 0) || (Objects != NULL));
  if ((Count != 0) && (Objects == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

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
  Send a control message carrying the current message ID, and step the ID on.

  Used for the short answers this driver owes a partner that asks something of
  it out of turn, where there is nothing to report but the answer itself.

**/
STATIC
EFI_STATUS
Fusb302PdSendControl (
  IN OUT FUSB302_CONTEXT  *Context,
  IN     UINT8            Type
  )
{
  EFI_STATUS  Status;
  UINT16      Header;

  Header = PD_HEADER_BUILD (
             Type,
             Context->MessageId,
             0,
             Context->DataRoleDfp ? 1 : 0,
             FUSB302_POWER_ROLE (Context),
             PD_REV_2_0
             );

  Status = Fusb302PdSend (Context, Header, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  return EFI_SUCCESS;
}

/**
  Accept a Soft_Reset the partner asked for.

  Soft_Reset puts both ends back to a known protocol state without disturbing
  VBUS, and the specification has the receiver answer Accept using message ID
  zero, both counters having been reset. A partner that gets no answer escalates
  to a Hard Reset, which drops the rail and costs us the contract and any
  alternate mode along with it.

  Whatever exchange was in flight is over either way; the caller sees its own
  wait time out and decides what to do next.

**/
STATIC
VOID
Fusb302PdAcceptSoftReset (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;

  //
  // Both counters restart, and the Accept itself carries the first ID.
  //
  Context->MessageId = 0;

  Status = Fusb302PdSendControl (Context, PD_CTRL_ACCEPT);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: could not accept Soft_Reset (%r)\n", __func__, Status));
    return;
  }

  Context->Contract.PdNegotiated = FALSE;

  DEBUG ((DEBUG_INFO, "%a: accepted Soft_Reset; message IDs restart\n", __func__));
}

/**
  Turn down a request this driver cannot satisfy.

  Under the 2.0 protocol the answer to a request that cannot be met is Reject,
  and saying so is what lets the partner move on. Silence would strand it
  exactly as an unanswered Get_Sink_Cap does.

  Used for the swaps this driver has no path for, and for a capability query
  it has no answer to.

**/
STATIC
VOID
Fusb302PdRejectRequest (
  IN OUT FUSB302_CONTEXT  *Context,
  IN     UINT8            Type
  )
{
  EFI_STATUS  Status;

  Status = Fusb302PdSendControl (Context, PD_CTRL_REJECT);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: could not reject message type %u (%r)\n", __func__, (UINT32)Type, Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "%a: rejected message type %u\n", __func__, (UINT32)Type));
}

/**
  Answer a Get_Sink_Cap with what this board is prepared to draw.

  A source may ask at any point, and the question is its own atomic message
  sequence: a source left waiting on the answer stays part way through an
  exchange and ignores everything that follows it, alternate mode entry
  included. That is the same failure a repeated Source_Capabilities caused.

  Failure to reply is logged rather than propagated. The question is the
  partner's, not ours, and losing the answer costs no less than never having
  sent one.

**/
STATIC
VOID
Fusb302PdAnswerGetSinkCap (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS    Status;
  UINT16        Header;
  CONST UINT32  *Pdos;
  UINTN         Count;

  Pdos  = Context->Platform->SinkPdos;
  Count = Fusb302PdoCount (Pdos);

  //
  // Nothing to describe. Say so rather than staying quiet, for the same
  // reason the question is answered at all.
  //
  if (Count == 0) {
    Fusb302PdRejectRequest (Context, PD_CTRL_GET_SINK_CAP);
    return;
  }

  Header = PD_HEADER_BUILD (
             PD_DATA_SINK_CAP,
             Context->MessageId,
             Count,
             Context->DataRoleDfp ? 1 : 0,
             FUSB302_POWER_ROLE (Context),
             PD_REV_2_0
             );

  Status = Fusb302PdSend (Context, Header, Pdos);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: could not answer Get_Sink_Cap (%r)\n", __func__, Status));
    return;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  DEBUG ((
    DEBUG_INFO,
    "%a: answered Get_Sink_Cap with %u object(s)\n",
    __func__,
    (UINT32)Count
    ));
}

/**
  Answer a Get_Source_Cap.

  While this board supplies the port it has a real answer to give, and it is
  the same single object the source path advertises. Sinking, there is nothing
  to offer and Reject is what lets the partner move on.

**/
STATIC
VOID
Fusb302PdAnswerGetSourceCap (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  UINT16      Header;
  UINT32      Pdo;

  Pdo = Fusb302SourcePdo (Context);

  if (Context->PartnerIsSource || (Pdo == 0)) {
    Fusb302PdRejectRequest (Context, PD_CTRL_GET_SOURCE_CAP);
    return;
  }

  Header = PD_HEADER_BUILD (
             PD_DATA_SOURCE_CAP,
             Context->MessageId,
             1,
             Context->DataRoleDfp ? 1 : 0,
             FUSB302_POWER_ROLE (Context),
             PD_REV_2_0
             );

  Status = Fusb302PdSend (Context, Header, &Pdo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: could not answer Get_Source_Cap (%r)\n", __func__, Status));
    return;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  DEBUG ((DEBUG_INFO, "%a: answered Get_Source_Cap with 0x%08x\n", __func__, Pdo));
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
  IN OUT FUSB302_CONTEXT  *Context,
  IN     BOOLEAN          WantData,
  IN     UINT8            Type,
  IN     UINTN            TimeoutUs,
  OUT    PD_MESSAGE       *Message
  )
{
  EFI_STATUS  Status;
  UINT64      Start;
  UINT64      TimeoutNs;
  UINTN       Count;

  //
  // The timeout is a bound on time, not on how many messages arrive. Most of
  // the paths below answer the partner and come back around without delaying
  // at all, so counting poll intervals would cut the wait short exactly when
  // the partner is chatty -- the case where the wait is worth the most.
  //
  Start     = GetPerformanceCounter ();
  TimeoutNs = MultU64x32 (TimeoutUs, 1000);

  while (GetTimeInNanoSecond (GetPerformanceCounter () - Start) < TimeoutNs) {
    Status = Fusb302PdReceive (Context, Message);
    if (Status == EFI_NOT_READY) {
      MicroSecondDelay (PD_POLL_INTERVAL_US);
      continue;
    }

    if (EFI_ERROR (Status)) {
      return Status;
    }

    Count = PD_HEADER_COUNT (Message->Header);

    //
    // Log everything that arrives, matching or not. What a partner sends
    // unprompted between exchanges is the only way to tell a message we got
    // wrong from one the partner never sent.
    //
    DEBUG ((
      DEBUG_INFO,
      "%a: rx %a type %u id %u objects %u rev %u%a\n",
      __func__,
      Count > 0 ? "data" : "ctrl",
      (UINT32)PD_HEADER_TYPE (Message->Header),
      (UINT32)PD_HEADER_ID (Message->Header),
      (UINT32)Count,
      (UINT32)PD_HEADER_REV (Message->Header),
      PD_HEADER_EXTENDED (Message->Header) ? " extended" : ""
      ));

    if (PD_HEADER_EXTENDED (Message->Header)) {
      continue;
    }

    //
    // Answer what the partner asks for in passing, whatever we happen to be
    // waiting for. A request left hanging strands the partner part way through
    // an exchange, after which it ignores everything that follows.
    //
    // Every request the 2.0 protocol defines is answered here, in whichever
    // role we are in: the replies take their power role from the context, so
    // the same handling serves the sourcing path as well as the sinking one.
    // Anything this driver cannot do is turned down rather than ignored --
    // Reject moves the partner on, silence does not.
    //
    if (Count == 0) {
      switch (PD_HEADER_TYPE (Message->Header)) {
        case PD_CTRL_GET_SINK_CAP:
          Fusb302PdAnswerGetSinkCap (Context);
          continue;

        case PD_CTRL_GET_SOURCE_CAP:
          Fusb302PdAnswerGetSourceCap (Context);
          continue;

        case PD_CTRL_SOFT_RESET:
          Fusb302PdAcceptSoftReset (Context);
          continue;

        //
        // Swaps this driver has no path for. The power role is fixed by which
        // end of the cable supplies the other, the data role is swapped only
        // when alternate mode needs it, and VCONN is not ours to move.
        //
        case PD_CTRL_PR_SWAP:
        case PD_CTRL_DR_SWAP:
        case PD_CTRL_VCONN_SWAP:
          Fusb302PdRejectRequest (Context, (UINT8)PD_HEADER_TYPE (Message->Header));
          continue;

        //
        // Neither of these asks for a reply. Ping is a source keeping the
        // link alive; GotoMin asks a sink to draw less, which this board is
        // already doing by not drawing anything much.
        //
        case PD_CTRL_PING:
        case PD_CTRL_GOTO_MIN:
          continue;

        default:
          break;
      }
    }

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
  Whether the board will take a supply at this voltage, and how much of it.

  A fixed sink object names one voltage; a variable one names a range, which
  is how a board says it accepts anything in between. Either way the object's
  current is the most the board will draw there.

  @param[out] LimitMa   Current the matching object allows.

  @retval TRUE   The board accepts this voltage.

**/
STATIC
BOOLEAN
Fusb302SinkAccepts (
  IN  CONST UINT32  *SinkPdos,
  IN  UINT32        VoltageMv,
  OUT UINT32        *LimitMa
  )
{
  UINTN   Index;
  UINTN   Count;
  UINT32  Pdo;

  Count    = Fusb302PdoCount (SinkPdos);
  *LimitMa = 0;

  for (Index = 0; Index < Count; Index++) {
    Pdo = SinkPdos[Index];

    if ((PD_PDO_TYPE (Pdo) == PD_PDO_TYPE_FIXED) &&
        (PD_PDO_FIXED_VOLTAGE_MV (Pdo) == VoltageMv))
    {
      *LimitMa = PD_PDO_FIXED_CURRENT_MA (Pdo);
      return TRUE;
    }

    if ((PD_PDO_TYPE (Pdo) == PD_PDO_TYPE_VARIABLE) &&
        (VoltageMv >= PD_PDO_VAR_MIN_VOLTAGE_MV (Pdo)) &&
        (VoltageMv <= PD_PDO_VAR_MAX_VOLTAGE_MV (Pdo)))
    {
      *LimitMa = PD_PDO_VAR_MAX_CURRENT_MA (Pdo);
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Pick the best fixed supply the source offers that the board will take.

  Highest voltage wins, since that is what reduces the current the board has
  to draw. Anything above 5 V is reached only where the board listed it, the
  VBUS path having to be rated for it.

  @param[out] Position   1-based index of the chosen object.

**/
STATIC
EFI_STATUS
Fusb302PdSelectPdo (
  IN  CONST PD_MESSAGE  *SourceCap,
  IN  CONST UINT32      *SinkPdos,
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
  UINT32  LimitMa;

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

    if ((Voltage == 0) || !Fusb302SinkAccepts (SinkPdos, Voltage, &LimitMa)) {
      continue;
    }

    if (Voltage > *VoltageMv) {
      *Position  = Index + 1;
      *VoltageMv = Voltage;
      *CurrentMa = MIN (Current, LimitMa);
    }
  }

  return (*Position == 0) ? EFI_NOT_FOUND : EFI_SUCCESS;
}

EFI_STATUS
Fusb302PdHardReset (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  UINT8       Status0;
  UINTN       Waited;
  BOOLEAN     Gone;

  DEBUG ((DEBUG_INFO, "%a: sending Hard Reset\n", __func__));

  Status = Fusb302RegUpdate (
             Context,
             FUSB302_REG_CONTROL3,
             FUSB302_CONTROL3_SEND_HARD_RESET,
             FUSB302_CONTROL3_SEND_HARD_RESET
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Both ends start counting from zero again.
  //
  Context->MessageId             = 0;
  Context->Contract.PdNegotiated = FALSE;

  //
  // Watch VBUS fall and rise again. Seeing it go is what tells us the source
  // acted on the reset rather than ignoring it, but a source that recovers
  // quickly can be back before we look, so a rail that never appears to drop
  // is not treated as a failure.
  //
  Gone = FALSE;

  for (Waited = 0; Waited < PD_HARD_RESET_RECOVER_US; Waited += PD_POLL_INTERVAL_US) {
    Status = Fusb302RegRead (Context, FUSB302_REG_STATUS0, &Status0);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ((Status0 & FUSB302_STATUS0_VBUSOK) == 0) {
      Gone = TRUE;
    } else if (Gone) {
      break;
    }

    MicroSecondDelay (PD_POLL_INTERVAL_US);
  }

  Status = Fusb302RegRead (Context, FUSB302_REG_STATUS0, &Status0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((Status0 & FUSB302_STATUS0_VBUSOK) == 0) {
    DEBUG ((DEBUG_ERROR, "%a: VBUS did not return after Hard Reset\n", __func__));
    return EFI_TIMEOUT;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: VBUS %a; port is back\n",
    __func__,
    Gone ? "cycled" : "never seen to drop"
    ));

  //
  // Anything the controller buffered belongs to the conversation we just tore
  // down.
  //
  Fusb302RegUpdate (Context, FUSB302_REG_CONTROL1, FUSB302_CONTROL1_RX_FLUSH, FUSB302_CONTROL1_RX_FLUSH);
  Fusb302RegUpdate (Context, FUSB302_REG_CONTROL0, FUSB302_CONTROL0_TX_FLUSH, FUSB302_CONTROL0_TX_FLUSH);

  return EFI_SUCCESS;
}

EFI_STATUS
Fusb302PdNegotiateSink (
  IN OUT FUSB302_CONTEXT         *Context,
  IN     USB_TYPE_C_ORIENTATION  Orientation
  )
{
  EFI_STATUS    Status;
  PD_MESSAGE    Message;
  UINT16        Header;
  UINT32        Rdo;
  UINTN         Position;
  UINT32        VoltageMv;
  UINT32        CurrentMa;
  CONST UINT32  *SinkPdos;
  UINTN         Attempt;

  SinkPdos = Context->Platform->SinkPdos;

  if (Fusb302PdoCount (SinkPdos) == 0) {
    DEBUG ((DEBUG_WARN, "%a: board lists nothing this port will accept\n", __func__));
    return EFI_UNSUPPORTED;
  }

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
    // where an earlier boot stage already negotiated. Try Soft_Reset first,
    // since it returns both ends to a known protocol state without disturbing
    // VBUS.
    //
    DEBUG ((DEBUG_INFO, "%a: no advertisement; sending Soft_Reset\n", __func__));

    //
    // Soft_Reset always carries message ID zero: both counters restart with it.
    //
    Header = PD_HEADER_BUILD (
               PD_CTRL_SOFT_RESET,
               0,
               0,
               Context->DataRoleDfp ? 1 : 0,
               FUSB302_POWER_ROLE (Context),
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
    if (!EFI_ERROR (Status)) {
      Status = Fusb302PdWaitFor (
                 Context,
                 TRUE,
                 PD_DATA_SOURCE_CAP,
                 PD_SOURCE_CAP_RETRY_TIMEOUT_US,
                 &Message
                 );
    } else {
      DEBUG ((DEBUG_INFO, "%a: Soft_Reset unanswered (%r)\n", __func__, Status));
    }

    if (EFI_ERROR (Status)) {
      //
      // A source holding a contract agreed before this firmware started may
      // refuse to answer Soft_Reset at all, because from its point of view
      // nothing is wrong. Hard Reset is what the specification leaves for
      // that: it tears the contract down at both ends and the source starts
      // advertising from scratch. It costs a momentary loss of VBUS, which is
      // why it is tried only once Soft_Reset has failed.
      //
      DEBUG ((DEBUG_INFO, "%a: falling back to Hard Reset\n", __func__));

      Status = Fusb302PdHardReset (Context);
      if (EFI_ERROR (Status)) {
        goto Disable;
      }

      //
      // The terminations and the receiver have to be put back afterwards; a
      // Hard Reset returns the controller to its unattached state.
      //
      Status = Fusb302PdEnable (Context, Orientation);
      if (EFI_ERROR (Status)) {
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
        DEBUG ((
          DEBUG_WARN,
          "%a: no Source_Capabilities after Hard Reset (%r); "
          "partner is not a PD source\n",
          __func__,
          Status
          ));
        goto Disable;
      }
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: source offers %u power data object(s); board accepts %u\n",
    __func__,
    (UINT32)PD_HEADER_COUNT (Message.Header),
    (UINT32)Fusb302PdoCount (SinkPdos)
    ));

  //
  // A source is entitled to advertise again after a contract, and a sink has
  // to answer every Source_Capabilities with a Request. Ignoring a repeat
  // leaves the source part way through an exchange, after which it ignores
  // everything else we send -- which looks exactly like a partner that does
  // not support alternate mode.
  //
  for (Attempt = 0; Attempt < PD_SOURCE_CAP_REPEAT_LIMIT; Attempt++) {
    Status = Fusb302PdSelectPdo (&Message, SinkPdos, &Position, &VoltageMv, &CurrentMa);
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
               FUSB302_POWER_ROLE (Context),
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

    //
    // Settled, unless the source immediately offers again.
    //
    Status = Fusb302PdWaitFor (
               Context,
               TRUE,
               PD_DATA_SOURCE_CAP,
               PD_SOURCE_CAP_REPEAT_US,
               &Message
               );
    if (EFI_ERROR (Status)) {
      break;
    }

    DEBUG ((DEBUG_INFO, "%a: source advertised again; answering\n", __func__));
  }

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

  Header = PD_HEADER_BUILD (
             PD_CTRL_DR_SWAP,
             Context->MessageId,
             0,
             Context->DataRoleDfp ? 1 : 0,
             FUSB302_POWER_ROLE (Context),
             PD_REV_2_0
             );

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
