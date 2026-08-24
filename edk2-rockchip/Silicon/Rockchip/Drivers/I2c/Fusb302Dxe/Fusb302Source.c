/** @file
*
*  Sourcing power over USB-C from the FUSB302.
*
*  Advertises a fixed 5 V supply to an attached sink and completes the contract
*  it asks for. This is the one part of the port manager that does something
*  physical to the board rather than just describing it, so it is gated three
*  ways: the board has to opt in through PcdFusb302SourceEnable and name the
*  GPIO that switches the supply, the partner has to actually be a sink, and
*  VBUS has to be absent before we drive it. That last check matters most --
*  switching our supply onto a VBUS something else is already driving puts two
*  sources in contention.
*
*  Only 5 V is offered. Anything higher needs a supply that can produce it and
*  a VBUS path rated for it, neither of which this driver can discover, and
*  offering a voltage the hardware cannot deliver is worse than offering none.
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
#include <Library/GpioLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Pi/PiI2c.h>

#include "Fusb302Dxe.h"

#define FUSB302_SOURCE_VOLTAGE_MV  5000

/**
  Switch the board's USB-C supply on or off.

  @retval EFI_SUCCESS       The supply was switched.
  @retval EFI_UNSUPPORTED   No supply GPIO was configured for this board.

**/
EFI_STATUS
Fusb302SourceSetVbus (
  IN BOOLEAN  Enable
  )
{
  UINT8    Bank;
  UINT8    Pin;
  BOOLEAN  ActiveHigh;
  BOOLEAN  Level;

  Bank       = PcdGet8 (PcdFusb302VbusGpioBank);
  Pin        = PcdGet8 (PcdFusb302VbusGpioPin);
  ActiveHigh = PcdGetBool (PcdFusb302VbusGpioActiveHigh);

  if ((Bank == 0xFF) || (Pin == 0xFF)) {
    return EFI_UNSUPPORTED;
  }

  Level = ActiveHigh ? Enable : (BOOLEAN)!Enable;

  GpioPinWrite (Bank, Pin, Level);
  GpioPinSetDirection (Bank, Pin, GPIO_PIN_OUTPUT);

  DEBUG ((
    DEBUG_INFO,
    "%a: USB-C VBUS %a via GPIO%u_%c%u\n",
    __func__,
    Enable ? "on" : "off",
    Bank,
    'A' + (Pin / 8),
    Pin % 8
    ));

  return EFI_SUCCESS;
}

/**
  Apply the Rp that matches what we are about to advertise, so that a sink
  which does not speak PD still reads the right current from the CC pin alone.
**/
STATIC
EFI_STATUS
Fusb302SourceSetRp (
  IN FUSB302_CONTEXT         *Context,
  IN USB_TYPE_C_ORIENTATION  Orientation,
  IN UINT32                  CurrentMa
  )
{
  EFI_STATUS  Status;
  BOOLEAN     UseCc2;
  UINT8       HostCurrent;
  UINT8       Switches0;

  UseCc2 = (Orientation == UsbTypeCOrientationFlipped);

  if (CurrentMa >= 3000) {
    HostCurrent = FUSB302_CONTROL0_HOST_CUR_HIGH;
  } else if (CurrentMa >= 1500) {
    HostCurrent = FUSB302_CONTROL0_HOST_CUR_MED;
  } else {
    HostCurrent = FUSB302_CONTROL0_HOST_CUR_DEF;
  }

  Status = Fusb302RegUpdate (
             Context,
             FUSB302_REG_CONTROL0,
             FUSB302_CONTROL0_HOST_CUR_MASK,
             HostCurrent
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Pull up both pins and measure the live one, which is what a source
  // presents.
  //
  Switches0 = FUSB302_SWITCHES0_CC1_PU_EN | FUSB302_SWITCHES0_CC2_PU_EN |
              (UseCc2 ? FUSB302_SWITCHES0_MEAS_CC2 : FUSB302_SWITCHES0_MEAS_CC1);

  return Fusb302RegWrite (Context, FUSB302_REG_SWITCHES0, Switches0);
}

/**
  Bring up the PD transmitter as a source on the live CC pin.
**/
STATIC
EFI_STATUS
Fusb302SourcePdEnable (
  IN FUSB302_CONTEXT         *Context,
  IN USB_TYPE_C_ORIENTATION  Orientation
  )
{
  EFI_STATUS  Status;
  BOOLEAN     UseCc2;
  UINT8       Switches1;

  UseCc2 = (Orientation == UsbTypeCOrientationFlipped);

  //
  // Announce ourselves as the source and the downstream facing port. These
  // bits also build the controller's automatic GoodCRC replies, so they have
  // to agree with the headers we send.
  //
  Switches1 = (UseCc2 ? FUSB302_SWITCHES1_TXCC2_EN : FUSB302_SWITCHES1_TXCC1_EN) |
              FUSB302_SWITCHES1_AUTO_GCRC |
              FUSB302_SWITCHES1_POWERROLE |
              FUSB302_SWITCHES1_DATAROLE |
              (PD_REV_2_0 << FUSB302_SWITCHES1_SPECREV_SHIFT);

  Status = Fusb302RegWrite (Context, FUSB302_REG_SWITCHES1, Switches1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

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

  Context->MessageId   = 0;
  Context->DataRoleDfp = TRUE;

  return EFI_SUCCESS;
}

/**
  Wait for VBUS to be seen by the controller.
**/
STATIC
BOOLEAN
Fusb302SourceVbusPresent (
  IN FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  UINT8       Status0;

  Status = Fusb302RegRead (Context, FUSB302_REG_STATUS0, &Status0);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return (Status0 & FUSB302_STATUS0_VBUSOK) != 0;
}

EFI_STATUS
Fusb302SourceIdle (
  IN OUT FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  UINT32      CurrentMa;

  if (!PcdGetBool (PcdFusb302SourceEnable)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Same rule as the negotiating path: never add our supply to a rail that
  // already has one.
  //
  if (Fusb302SourceVbusPresent (Context)) {
    return EFI_ALREADY_STARTED;
  }

  CurrentMa = PcdGet32 (PcdFusb302SourceCurrentMa);

  //
  // Orientation is unknown with nothing attached, so this pulls up both pins
  // and measures CC1 arbitrarily. It is the Rp itself, on both pins, that a
  // later attachment reads.
  //
  Status = Fusb302SourceSetRp (Context, UsbTypeCOrientationNormal, CurrentMa);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302SourceSetVbus (TRUE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->Contract.PdNegotiated = FALSE;
  Context->Contract.VoltageMv    = FUSB302_SOURCE_VOLTAGE_MV;
  Context->Contract.CurrentMa    = CurrentMa;

  DEBUG ((
    DEBUG_INFO,
    "%a: empty port left live at %u mV %u mA\n",
    __func__,
    FUSB302_SOURCE_VOLTAGE_MV,
    CurrentMa
    ));

  return EFI_SUCCESS;
}

EFI_STATUS
Fusb302PdSourceRun (
  IN OUT FUSB302_CONTEXT         *Context,
  IN     USB_TYPE_C_ORIENTATION  Orientation
  )
{
  EFI_STATUS  Status;
  PD_MESSAGE  Message;
  UINT32      Pdo;
  UINT32      CurrentMa;
  UINT16      Header;
  UINTN       Waited;
  UINTN       Position;

  if (!PcdGetBool (PcdFusb302SourceEnable)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Nothing to supply if the partner is supplying us.
  //
  if (Context->PartnerIsSource) {
    return EFI_UNSUPPORTED;
  }

  //
  // Refuse to add our supply to a VBUS that already has one. This is the check
  // that keeps a charger and this board from fighting over the rail.
  //
  if (Fusb302SourceVbusPresent (Context)) {
    DEBUG ((
      DEBUG_WARN,
      "%a: VBUS is already driven by something else, not sourcing\n",
      __func__
      ));
    return EFI_ALREADY_STARTED;
  }

  CurrentMa = PcdGet32 (PcdFusb302SourceCurrentMa);

  Status = Fusb302SourceSetRp (Context, Orientation, CurrentMa);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302SourceSetVbus (TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: no VBUS supply GPIO configured for this board\n", __func__));
    return Status;
  }

  for (Waited = 0; Waited < PD_VBUS_ON_TIMEOUT_US; Waited += PD_POLL_INTERVAL_US) {
    if (Fusb302SourceVbusPresent (Context)) {
      break;
    }

    MicroSecondDelay (PD_POLL_INTERVAL_US);
  }

  if (!Fusb302SourceVbusPresent (Context)) {
    DEBUG ((DEBUG_ERROR, "%a: VBUS did not come up, switching it back off\n", __func__));
    Fusb302SourceSetVbus (FALSE);
    return EFI_DEVICE_ERROR;
  }

  //
  // From here a sink is powered whatever happens with the messaging below, so
  // failures leave VBUS alone: Type-C current advertisement through Rp is a
  // valid contract on its own, and dropping the rail under a device that has
  // started drawing from it would be worse than not negotiating.
  //
  Context->Contract.PdNegotiated = FALSE;
  Context->Contract.VoltageMv    = FUSB302_SOURCE_VOLTAGE_MV;
  Context->Contract.CurrentMa    = CurrentMa;

  Status = Fusb302SourcePdEnable (Context, Orientation);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Pdo = PD_PDO_FIXED_BUILD (FUSB302_SOURCE_VOLTAGE_MV, CurrentMa);

  DEBUG ((
    DEBUG_INFO,
    "%a: advertising %u mV %u mA (power data object 0x%08x)\n",
    __func__,
    FUSB302_SOURCE_VOLTAGE_MV,
    CurrentMa,
    Pdo
    ));

  //
  // Advertise until the sink asks for something. A sink that never does is
  // either not a PD device or is happy with what Rp alone told it, and either
  // way it is already powered.
  //
  for (Waited = 0; Waited < PD_SOURCE_ATTACH_TIMEOUT_US; Waited += PD_SOURCE_CAP_INTERVAL_US) {
    Header = PD_HEADER_BUILD (PD_DATA_SOURCE_CAP, Context->MessageId, 1, 1, 1, PD_REV_2_0);

    Status = Fusb302PdSend (Context, Header, &Pdo);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Context->MessageId = (Context->MessageId + 1) & 0x7;

    Status = Fusb302PdWaitFor (
               Context,
               TRUE,
               PD_DATA_REQUEST,
               PD_SOURCE_CAP_INTERVAL_US,
               &Message
               );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_INFO,
      "%a: sink did not request a contract; it stays on Type-C current\n",
      __func__
      ));

    //
    // Still worth asking what it is: a display adapter that took power without
    // negotiating for it will happily talk alternate mode.
    //
    Fusb302DpAltModeEnter (Context);

    return EFI_SUCCESS;
  }

  Position = PD_RDO_OBJECT_POSITION (Message.Objects[0]);

  //
  // Exactly one object is on offer, so anything else is a sink asking for
  // something we never advertised.
  //
  if (Position != 1) {
    DEBUG ((DEBUG_WARN, "%a: sink asked for object %u, rejecting\n", __func__, (UINT32)Position));

    Header = PD_HEADER_BUILD (PD_CTRL_REJECT, Context->MessageId, 0, 1, 1, PD_REV_2_0);
    Fusb302PdSend (Context, Header, NULL);
    Context->MessageId = (Context->MessageId + 1) & 0x7;

    return EFI_SUCCESS;
  }

  Header = PD_HEADER_BUILD (PD_CTRL_ACCEPT, Context->MessageId, 0, 1, 1, PD_REV_2_0);

  Status = Fusb302PdSend (Context, Header, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  //
  // The supply is already at the voltage asked for, since only one was
  // offered, so it is ready as soon as the request is accepted.
  //
  Header = PD_HEADER_BUILD (PD_CTRL_PS_RDY, Context->MessageId, 0, 1, 1, PD_REV_2_0);

  Status = Fusb302PdSend (Context, Header, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context->MessageId = (Context->MessageId + 1) & 0x7;

  Context->Contract.PdNegotiated = TRUE;

  DEBUG ((
    DEBUG_INFO,
    "%a: supplying %u mV %u mA under a power delivery contract\n",
    __func__,
    FUSB302_SOURCE_VOLTAGE_MV,
    CurrentMa
    ));

  //
  // A display reached through a bus powered adapter is attached this way
  // round, so alternate mode has to be looked for here as well as when the
  // partner supplies us. We are already the downstream facing port, so the
  // data role swap inside is a no-op.
  //
  Fusb302DpAltModeEnter (Context);

  return EFI_SUCCESS;
}
