/** @file
*
*  DisplayPort Alternate Mode entry over the FUSB302.
*
*  Over a Type-C connector there is no hot-plug detect wire: whether a display
*  is present downstream is carried in a DisplayPort Status vendor defined
*  message. The DisplayPort controller's own HPD input has nothing driving it
*  on these boards, so without this the firmware cannot tell a connected
*  monitor from an empty port and has to be told to force output by hand.
*
*  Entering the mode also tells us the pin assignment, which says whether the
*  link carries four DisplayPort lanes or two alongside USB 3. That would
*  otherwise be a fixed guess from the board description.
*
*  This is the data role half of the port manager, and unlike sourcing power it
*  does not depend on the power role: a board drawing power from a dock is
*  still the upstream facing port for data and drives this exchange.
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
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Pi/PiI2c.h>

#include "Fusb302Dxe.h"

/**
  Send a structured VDM and wait for the matching response.

  @param[in]  Svid        SVID the command belongs to.
  @param[in]  Command     Structured VDM command.
  @param[in]  Position    Object position, zero except for mode commands.
  @param[in]  Objects     Additional data objects, or NULL.
  @param[in]  Count       Number of additional objects.
  @param[out] Response    Receives the response message.

  @retval EFI_SUCCESS       The partner acknowledged the command.
  @retval EFI_UNSUPPORTED   The partner replied NAK.
  @retval EFI_TIMEOUT       No response, or it stayed busy.

**/
STATIC
EFI_STATUS
Fusb302VdmRequest (
  IN  FUSB302_CONTEXT  *Context,
  IN  UINT16           Svid,
  IN  UINT8            Command,
  IN  UINT8            Position,
  IN  CONST UINT32     *Objects,
  IN  UINTN            Count,
  OUT PD_MESSAGE       *Response
  )
{
  EFI_STATUS  Status;
  UINT32      Payload[PD_MAX_DATA_OBJECTS];
  UINT16      Header;
  UINTN       Index;

  if ((Count + 1) > PD_MAX_DATA_OBJECTS) {
    return EFI_INVALID_PARAMETER;
  }

  Payload[0] = PD_VDM_HEADER (Svid, Position, PD_VDM_TYPE_REQ, Command);
  for (Index = 0; Index < Count; Index++) {
    Payload[Index + 1] = Objects[Index];
  }

  Header = PD_HEADER_BUILD (
             PD_DATA_VENDOR_DEFINED,
             Context->MessageId,
             Count + 1,
             Context->DataRoleDfp ? 1 : 0,
             0,  // sink
             PD_REV_2_0
             );

  Status = Fusb302PdSend (Context, Header, Payload);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  //
  // A partner may answer BUSY while it gets ready; the specification allows
  // retrying, so give it a few attempts before concluding it will not answer.
  //
  for (Index = 0; Index < 3; Index++) {
    Status = Fusb302PdWaitFor (
               Context,
               TRUE,
               PD_DATA_VENDOR_DEFINED,
               PD_VDM_RESPONSE_TIMEOUT_US,
               Response
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (!PD_VDM_IS_STRUCTURED (Response->Objects[0]) ||
        (PD_VDM_SVID (Response->Objects[0]) != Svid) ||
        (PD_VDM_CMD (Response->Objects[0]) != Command))
    {
      continue;
    }

    switch (PD_VDM_CMD_TYPE (Response->Objects[0])) {
      case PD_VDM_TYPE_ACK:
        return EFI_SUCCESS;

      case PD_VDM_TYPE_NAK:
        return EFI_UNSUPPORTED;

      case PD_VDM_TYPE_BUSY:
      default:
        MicroSecondDelay (5 * 1000);
        continue;
    }
  }

  return EFI_TIMEOUT;
}

/**
  Look through the Discover SVIDs response for the DisplayPort SVID.

  The SVIDs come packed two per data object, terminated by a zero entry.
**/
STATIC
BOOLEAN
Fusb302VdmHasDisplayPort (
  IN CONST PD_MESSAGE  *Response
  )
{
  UINTN   Count;
  UINTN   Index;
  UINT16  Svid;

  Count = PD_HEADER_COUNT (Response->Header);

  //
  // Object 0 is the VDM header, the SVIDs follow.
  //
  for (Index = 1; Index < Count; Index++) {
    Svid = (UINT16)(Response->Objects[Index] >> 16);
    if (Svid == PD_VDM_SVID_DISPLAYPORT) {
      return TRUE;
    }

    if (Svid == 0) {
      break;
    }

    Svid = (UINT16)(Response->Objects[Index] & 0xFFFF);
    if (Svid == PD_VDM_SVID_DISPLAYPORT) {
      return TRUE;
    }

    if (Svid == 0) {
      break;
    }
  }

  return FALSE;
}

/**
  Choose a pin assignment from what the display offers.

  Assignment D is preferred: it carries two DisplayPort lanes alongside a live
  USB 3 link, which keeps boot devices on the port working, and two lanes are
  ample for anything firmware puts on screen. It is also the mapping the lane
  mux in UsbDpPhyDxe has always been set up for on these boards.

  C and E give four lanes at the cost of USB 3, and are taken only where D is
  not on offer.

  @param[out] Lanes   DisplayPort lanes the choice provides.

  @return The chosen pin assignment, or zero if none is usable.

**/
STATIC
UINT8
Fusb302VdmSelectPinAssignment (
  IN  UINT32  DpCapVdo,
  OUT UINT8   *Lanes
  )
{
  UINT8  Offered;

  //
  // The partner is the display, so it is the UFP_D and fills in that field.
  //
  Offered = (UINT8)DP_CAP_PIN_ASSIGN_UFP_D (DpCapVdo);

  if ((Offered & USB_TYPE_C_DP_PIN_ASSIGN_D) != 0) {
    *Lanes = 2;
    return USB_TYPE_C_DP_PIN_ASSIGN_D;
  }

  if ((Offered & USB_TYPE_C_DP_PIN_ASSIGN_C) != 0) {
    *Lanes = 4;
    return USB_TYPE_C_DP_PIN_ASSIGN_C;
  }

  if ((Offered & USB_TYPE_C_DP_PIN_ASSIGN_E) != 0) {
    *Lanes = 4;
    return USB_TYPE_C_DP_PIN_ASSIGN_E;
  }

  *Lanes = 0;
  return 0;
}

EFI_STATUS
Fusb302DpAltModeEnter (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  PD_MESSAGE  Response;
  UINT32      DpCapVdo;
  UINT32      Config;
  UINT32      Zero = 0;
  UINT8       PinAssignment;
  UINT8       Lanes;
  UINTN       Count;

  ZeroMem (&Context->DpAltMode, sizeof (Context->DpAltMode));

  //
  // Alternate modes are discovered by the downstream facing port. Attaching as
  // a sink leaves us upstream facing, so ask to swap before anything else; a
  // partner that refuses cannot be driven as a display from here.
  //
  Status = Fusb302PdDataRoleSwapToDfp (Context);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Discover Identity first: its header says whether the partner does any
  // alternate mode at all, so a plain charger or cable costs one exchange
  // rather than three.
  //
  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_PD,
             PD_VDM_DISCOVER_IDENTITY,
             0,
             NULL,
             0,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Discover Identity failed (%r)\n", __func__, Status));
    return Status;
  }

  if (PD_HEADER_COUNT (Response.Header) < 2) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a: partner identity header 0x%08x\n", __func__, Response.Objects[1]));

  if (!PD_ID_HEADER_MODAL_OPERATION (Response.Objects[1])) {
    DEBUG ((DEBUG_INFO, "%a: partner supports no alternate modes\n", __func__));
    return EFI_UNSUPPORTED;
  }

  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_PD,
             PD_VDM_DISCOVER_SVIDS,
             0,
             NULL,
             0,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Discover SVIDs failed (%r)\n", __func__, Status));
    return Status;
  }

  if (!Fusb302VdmHasDisplayPort (&Response)) {
    DEBUG ((DEBUG_INFO, "%a: partner does not advertise DisplayPort\n", __func__));
    return EFI_UNSUPPORTED;
  }

  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_DISPLAYPORT,
             PD_VDM_DISCOVER_MODES,
             0,
             NULL,
             0,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Discover Modes failed (%r)\n", __func__, Status));
    return Status;
  }

  Count = PD_HEADER_COUNT (Response.Header);
  if (Count < 2) {
    return EFI_UNSUPPORTED;
  }

  //
  // Only the first mode is considered. The DisplayPort specification defines
  // exactly one, and nothing observed in practice offers more.
  //
  DpCapVdo = Response.Objects[1];

  DEBUG ((
    DEBUG_INFO,
    "%a: DisplayPort capability 0x%08x (port capability %u, UFP_D pin assignments 0x%02x)\n",
    __func__,
    DpCapVdo,
    (UINT32)DP_CAP_PORT_CAPABILITY (DpCapVdo),
    (UINT32)DP_CAP_PIN_ASSIGN_UFP_D (DpCapVdo)
    ));

  //
  // We drive a display, so the partner has to be willing to be one.
  //
  if ((DP_CAP_PORT_CAPABILITY (DpCapVdo) != DP_CAP_PORT_UFP_D) &&
      (DP_CAP_PORT_CAPABILITY (DpCapVdo) != DP_CAP_PORT_BOTH))
  {
    DEBUG ((DEBUG_INFO, "%a: partner is not a DisplayPort sink\n", __func__));
    return EFI_UNSUPPORTED;
  }

  PinAssignment = Fusb302VdmSelectPinAssignment (DpCapVdo, &Lanes);
  if (PinAssignment == 0) {
    DEBUG ((DEBUG_WARN, "%a: no usable pin assignment offered\n", __func__));
    return EFI_UNSUPPORTED;
  }

  //
  // Object position 1, matching the mode taken from Discover Modes.
  //
  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_DISPLAYPORT,
             PD_VDM_ENTER_MODE,
             1,
             NULL,
             0,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: Enter Mode failed (%r)\n", __func__, Status));
    return Status;
  }

  Context->DpAltMode.Entered       = TRUE;
  Context->DpAltMode.PinAssignment = PinAssignment;
  Context->DpAltMode.DpLanes       = Lanes;

  //
  // Ask for status before configuring, which is the order the DisplayPort Alt
  // Mode specification lays out and the order a display expects.
  //
  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_DISPLAYPORT,
             PD_VDM_DP_STATUS,
             1,
             &Zero,
             1,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: DisplayPort Status failed (%r)\n", __func__, Status));
    return Status;
  }

  Config = DP_CONFIG_BUILD (PinAssignment);

  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_DISPLAYPORT,
             PD_VDM_DP_CONFIGURE,
             1,
             &Config,
             1,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: DisplayPort Configure failed (%r)\n", __func__, Status));
    return Status;
  }

  //
  // Read the status again now the link is configured, so the hot-plug state we
  // report describes the link we actually set up.
  //
  Status = Fusb302VdmRequest (
             Context,
             PD_VDM_SVID_DISPLAYPORT,
             PD_VDM_DP_STATUS,
             1,
             &Zero,
             1,
             &Response
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: DisplayPort Status failed (%r)\n", __func__, Status));
    return Status;
  }

  if (PD_HEADER_COUNT (Response.Header) >= 2) {
    Context->DpAltMode.HpdAsserted = DP_STATUS_HPD_STATE (Response.Objects[1]) != 0;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: entered DisplayPort Alt Mode, pin assignment %a, %u lanes, display %a\n",
    __func__,
    (PinAssignment == USB_TYPE_C_DP_PIN_ASSIGN_C) ? "C" :
    (PinAssignment == USB_TYPE_C_DP_PIN_ASSIGN_D) ? "D" : "E",
    Lanes,
    Context->DpAltMode.HpdAsserted ? "present" : "absent"
    ));

  return EFI_SUCCESS;
}
