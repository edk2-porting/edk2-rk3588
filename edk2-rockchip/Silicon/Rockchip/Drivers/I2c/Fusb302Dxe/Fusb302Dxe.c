/** @file
*
*  ON Semiconductor FUSB302 USB Type-C port controller.
*
*  Determines which CC pin the cable is using, so that the USBDP combo PHY can
*  map its lanes to the orientation the plug is actually in rather than to a
*  fixed guess from the board configuration.
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
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Pi/PiI2c.h>

#include "Fusb302Dxe.h"

STATIC CONST EFI_GUID  I2cGuid = I2C_GUID;

EFI_DRIVER_BINDING_PROTOCOL  gFusb302DriverBinding = {
  Fusb302Supported,
  Fusb302Start,
  Fusb302Stop
};

/**
  Read a single FUSB302 register.
**/
EFI_STATUS
Fusb302RegRead (
  IN  FUSB302_CONTEXT  *Context,
  IN  UINT8            Register,
  OUT UINT8            *Value
  )
{
  EFI_I2C_REQUEST_PACKET  *RequestPacket;
  UINTN                   RequestPacketSize;
  EFI_STATUS              Status;

  RequestPacketSize = sizeof (UINTN) + sizeof (EFI_I2C_OPERATION) * 2;
  RequestPacket     = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  RequestPacket->OperationCount = 2;

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = sizeof (Register);
  RequestPacket->Operation[0].Buffer        = &Register;

  RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = sizeof (*Value);
  RequestPacket->Operation[1].Buffer        = Value;

  Status = Context->I2cIo->QueueRequest (Context->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to read register 0x%02x. Status=%r\n",
      __func__,
      Register,
      Status
      ));
  }

  FreePool (RequestPacket);

  return Status;
}

/**
  Write a single FUSB302 register.
**/
EFI_STATUS
Fusb302RegWrite (
  IN FUSB302_CONTEXT  *Context,
  IN UINT8            Register,
  IN UINT8            Value
  )
{
  EFI_I2C_REQUEST_PACKET  *RequestPacket;
  UINTN                   RequestPacketSize;
  EFI_STATUS              Status;
  UINT8                   Buffer[2];

  Buffer[0] = Register;
  Buffer[1] = Value;

  RequestPacketSize = sizeof (UINTN) + sizeof (EFI_I2C_OPERATION);
  RequestPacket     = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  RequestPacket->OperationCount = 1;

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = sizeof (Buffer);
  RequestPacket->Operation[0].Buffer        = Buffer;

  Status = Context->I2cIo->QueueRequest (Context->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to write register 0x%02x. Status=%r\n",
      __func__,
      Register,
      Status
      ));
  }

  FreePool (RequestPacket);

  return Status;
}

EFI_STATUS
Fusb302RegUpdate (
  IN FUSB302_CONTEXT  *Context,
  IN UINT8            Register,
  IN UINT8            Mask,
  IN UINT8            Value
  )
{
  EFI_STATUS  Status;
  UINT8       Current;

  Status = Fusb302RegRead (Context, Register, &Current);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Current &= (UINT8) ~Mask;
  Current |= (Value & Mask);

  return Fusb302RegWrite (Context, Register, Current);
}

/**
  Apply the CC terminations for the role we are about to probe as.

  Sink presents Rd on both pins; source presents Rp on both, with the pull-up
  current left at the USB default.
**/
STATIC
EFI_STATUS
Fusb302SetTermination (
  IN FUSB302_CONTEXT     *Context,
  IN FUSB302_PROBE_ROLE  Role
  )
{
  EFI_STATUS  Status;
  UINT8       Switches0;

  if (Role == Fusb302RoleSink) {
    Switches0 = FUSB302_SWITCHES0_CC1_PD_EN | FUSB302_SWITCHES0_CC2_PD_EN;
  } else {
    Switches0 = FUSB302_SWITCHES0_CC1_PU_EN | FUSB302_SWITCHES0_CC2_PU_EN;
  }

  Status = Fusb302RegWrite (Context, FUSB302_REG_SWITCHES0, Switches0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Rp magnitude only matters in source mode, but setting it unconditionally
  // keeps the part in a known state.
  //
  return Fusb302RegUpdate (
           Context,
           FUSB302_REG_CONTROL0,
           FUSB302_CONTROL0_HOST_CUR_MASK,
           FUSB302_CONTROL0_HOST_CUR_DEF
           );
}

/**
  Measure the BC_LVL reported for one CC pin.

  Only one CC pin can be routed into the measure block at a time, so the caller
  gets one pin per call. The terminations must already have been applied by
  Fusb302SetTermination(); note that BC_LVL is meaningless unless one of the
  MEAS bits is set and the comparator has been given time to settle.

  @param[in]  Context   Driver context.
  @param[in]  Role      Role whose terminations are currently applied.
  @param[in]  UseCc2    Measure CC2 when TRUE, CC1 when FALSE.
  @param[out] Level     Receives the BC_LVL field of STATUS0.

**/
STATIC
EFI_STATUS
Fusb302MeasureCc (
  IN  FUSB302_CONTEXT     *Context,
  IN  FUSB302_PROBE_ROLE  Role,
  IN  BOOLEAN             UseCc2,
  OUT UINT8               *Level
  )
{
  EFI_STATUS  Status;
  UINT8       Status0;
  UINT8       Keep;

  Keep = (Role == Fusb302RoleSink)
         ? (FUSB302_SWITCHES0_CC1_PD_EN | FUSB302_SWITCHES0_CC2_PD_EN)
         : (FUSB302_SWITCHES0_CC1_PU_EN | FUSB302_SWITCHES0_CC2_PU_EN);

  Status = Fusb302RegWrite (
             Context,
             FUSB302_REG_SWITCHES0,
             Keep | (UseCc2 ? FUSB302_SWITCHES0_MEAS_CC2 : FUSB302_SWITCHES0_MEAS_CC1)
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (FUSB302_MEASURE_SETTLE_US);

  Status = Fusb302RegRead (Context, FUSB302_REG_STATUS0, &Status0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Level = Status0 & FUSB302_STATUS0_BC_LVL_MASK;

  return EFI_SUCCESS;
}

/**
  Probe both CC pins in one role and work out which pin the partner is on.

  As a sink, the pin reached through the cable is pulled up by the source and
  reads a higher BC_LVL than the open pin.

  As a source, it is the other way round: our own Rp holds an unconnected pin
  at the top of the range, and the partner's Rd drags the connected pin down.
  An electronically marked cable also presents Ra on the VCONN pin, which sits
  lower still, so where both pins read below the top band the higher of the two
  is the one carrying Rd.

  @retval EFI_SUCCESS     A partner was found and Orientation was set.
  @retval EFI_NOT_READY   Nothing is attached in this role.

**/
STATIC
EFI_STATUS
Fusb302ProbeRole (
  IN  FUSB302_CONTEXT         *Context,
  IN  FUSB302_PROBE_ROLE      Role,
  OUT USB_TYPE_C_ORIENTATION  *Orientation
  )
{
  EFI_STATUS  Status;
  UINT8       Cc1Level;
  UINT8       Cc2Level;

  Status = Fusb302SetTermination (Context, Role);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302MeasureCc (Context, Role, FALSE, &Cc1Level);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302MeasureCc (Context, Role, TRUE, &Cc2Level);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: as %a: CC1 BC_LVL %u, CC2 BC_LVL %u\n",
    __func__,
    Role == Fusb302RoleSink ? "sink" : "source",
    Cc1Level,
    Cc2Level
    ));

  if (Role == Fusb302RoleSink) {
    //
    // Both pins open: no source is driving either of them.
    //
    if ((Cc1Level == FUSB302_BC_LVL_RA) && (Cc2Level == FUSB302_BC_LVL_RA)) {
      return EFI_NOT_READY;
    }

    if (Cc1Level == Cc2Level) {
      return EFI_NOT_READY;
    }

    *Orientation = (Cc1Level > Cc2Level) ? UsbTypeCOrientationNormal
                                         : UsbTypeCOrientationFlipped;
    return EFI_SUCCESS;
  }

  //
  // Source: both pins still at the top of the range means nothing is pulling
  // them down, so the port is empty.
  //
  if ((Cc1Level == FUSB302_BC_LVL_3A) && (Cc2Level == FUSB302_BC_LVL_3A)) {
    return EFI_NOT_READY;
  }

  if (Cc1Level == Cc2Level) {
    return EFI_NOT_READY;
  }

  //
  // Whichever pin sits higher carries Rd; a lower one is Ra, or simply open.
  //
  *Orientation = (Cc1Level > Cc2Level) ? UsbTypeCOrientationFlipped
                                       : UsbTypeCOrientationNormal;

  return EFI_SUCCESS;
}

/**
  Work out which CC pin the cable is on.

  The port is dual-role on the boards this runs on, and which role applies
  depends on what got plugged in: a charger or a host makes us the sink, while
  a DisplayPort monitor makes us the source. Try sink first, since that is the
  role these boards prefer, and fall back to source.

**/
STATIC
EFI_STATUS
Fusb302DetectOrientation (
  IN  FUSB302_CONTEXT         *Context,
  OUT USB_TYPE_C_ORIENTATION  *Orientation
  )
{
  EFI_STATUS  Status;

  *Orientation             = UsbTypeCOrientationNone;
  Context->PartnerIsSource = FALSE;

  Status = Fusb302ProbeRole (Context, Fusb302RoleSink, Orientation);
  if (!EFI_ERROR (Status)) {
    //
    // We matched while presenting Rd, so the partner is driving Rp: it is the
    // source and we are the sink.
    //
    Context->PartnerIsSource = TRUE;
    goto Restore;
  }

  if (Status != EFI_NOT_READY) {
    return Status;
  }

  Status = Fusb302ProbeRole (Context, Fusb302RoleSource, Orientation);

Restore:
  //
  // Leave the part presenting a sink termination either way. Holding Rp would
  // advertise us as a source offering VBUS that this driver never turns on.
  //
  Fusb302SetTermination (Context, Fusb302RoleSink);

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
Fusb302GetDpAltMode (
  IN  USB_TYPE_C_PORT_PROTOCOL  *This,
  OUT USB_TYPE_C_DP_ALT_MODE    *AltMode
  )
{
  FUSB302_CONTEXT  *Context;

  if ((This == NULL) || (AltMode == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Context = FUSB302_SC_FROM_TYPEC_PORT (This);

  if (Context->Orientation == UsbTypeCOrientationNone) {
    return EFI_NOT_READY;
  }

  if (!Context->DpAltMode.Entered) {
    return EFI_UNSUPPORTED;
  }

  CopyMem (AltMode, &Context->DpAltMode, sizeof (*AltMode));

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Fusb302GetPowerContract (
  IN  USB_TYPE_C_PORT_PROTOCOL   *This,
  OUT USB_TYPE_C_POWER_CONTRACT  *Contract
  )
{
  FUSB302_CONTEXT  *Context;

  if ((This == NULL) || (Contract == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Context = FUSB302_SC_FROM_TYPEC_PORT (This);

  if (Context->Orientation == UsbTypeCOrientationNone) {
    return EFI_NOT_READY;
  }

  CopyMem (Contract, &Context->Contract, sizeof (*Contract));

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Fusb302GetOrientation (
  IN  USB_TYPE_C_PORT_PROTOCOL  *This,
  OUT USB_TYPE_C_ORIENTATION    *Orientation
  )
{
  FUSB302_CONTEXT  *Context;
  EFI_STATUS       Status;

  if ((This == NULL) || (Orientation == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Context = FUSB302_SC_FROM_TYPEC_PORT (This);

  //
  // Measuring means taking the CC pins back to plain terminations, which would
  // drop the BMC receiver and tear down a contract we have already negotiated.
  // Once PD is up the orientation cannot have changed underneath us anyway, so
  // report what was found then.
  //
  if (Context->Contract.PdNegotiated) {
    *Orientation = Context->Orientation;
    return (*Orientation == UsbTypeCOrientationNone) ? EFI_NOT_READY : EFI_SUCCESS;
  }

  //
  // Otherwise re-measure rather than returning the cached value: a cable may
  // well have been plugged in since we started.
  //
  Status = Fusb302DetectOrientation (Context, Orientation);
  if (!EFI_ERROR (Status)) {
    Context->Orientation = *Orientation;
  }

  return Status;
}

/**
  Reset the controller and bring up the analog blocks needed for measurement.
**/
STATIC
EFI_STATUS
Fusb302HwInit (
  IN FUSB302_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  UINT8       DeviceId;

  Status = Fusb302RegRead (Context, FUSB302_REG_DEVICE_ID, &DeviceId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: FUSB302 device ID 0x%02x (version %u)\n",
    __func__,
    DeviceId,
    DeviceId >> FUSB302_DEVICE_ID_VERSION_SHIFT
    ));

  Status = Fusb302RegWrite (Context, FUSB302_REG_RESET, FUSB302_RESET_SW_RESET);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (10 * 1000);

  //
  // Mask every interrupt source and keep the INT pin quiet. We poll, and the
  // OS driver will reconfigure the part from scratch when it takes over.
  //
  Status = Fusb302RegWrite (Context, FUSB302_REG_MASK, 0xFF);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302RegWrite (Context, FUSB302_REG_MASKA, 0xFF);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302RegWrite (Context, FUSB302_REG_MASKB, 0xFF);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Fusb302RegUpdate (
             Context,
             FUSB302_REG_CONTROL0,
             FUSB302_CONTROL0_INT_MASK,
             FUSB302_CONTROL0_INT_MASK
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Toggling would have the part drive its own connection state machine and
  // fight the measurements below, so make sure it is off.
  //
  Status = Fusb302RegUpdate (Context, FUSB302_REG_CONTROL2, FUSB302_CONTROL2_TOGGLE, 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Power up the bandgap, measure block and receiver.
  //
  Status = Fusb302RegWrite (Context, FUSB302_REG_POWER, FUSB302_POWER_PWR_ALL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Present a sink termination on both CC pins and leave VCONN off.
  //
  Status = Fusb302RegWrite (
             Context,
             FUSB302_REG_SWITCHES0,
             FUSB302_SWITCHES0_CC1_PD_EN | FUSB302_SWITCHES0_CC2_PD_EN
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Compare against the lowest threshold, which is what distinguishes an open
  // or Ra pin from one pulled up through the cable.
  //
  return Fusb302RegWrite (Context, FUSB302_REG_MEASURE, 0);
}

EFI_STATUS
EFIAPI
Fusb302Supported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS           Status;
  EFI_I2C_IO_PROTOCOL  *TmpI2cIo;
  UINT8                *Addresses;
  UINT8                *Buses;
  UINTN                Count;
  UINTN                Index;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&TmpI2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Addresses = PcdGetPtr (PcdFusb302Addresses);
  Buses     = PcdGetPtr (PcdFusb302Buses);
  Count     = PcdGetSize (PcdFusb302Addresses);

  Status = EFI_UNSUPPORTED;

  if ((Addresses == NULL) || (Buses == NULL)) {
    goto Exit;
  }

  for (Index = 0; Index < Count; Index++) {
    if (Addresses[Index] == 0) {
      continue;
    }

    if (CompareGuid (TmpI2cIo->DeviceGuid, &I2cGuid) &&
        (TmpI2cIo->DeviceIndex == I2C_DEVICE_INDEX (Buses[Index], Addresses[Index])))
    {
      Status = EFI_SUCCESS;
      break;
    }
  }

Exit:
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );

  return Status;
}

EFI_STATUS
EFIAPI
Fusb302Start (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS             Status;
  EFI_I2C_IO_PROTOCOL    *I2cIo;
  FUSB302_CONTEXT        *Context;
  UINT8                  *Addresses;
  UINT8                  *Buses;
  UINT8                  *PhyIds;
  UINTN                  Count;
  UINTN                  Index;
  USB_TYPE_C_ORIENTATION Orientation;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&I2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Context = AllocateZeroPool (sizeof (FUSB302_CONTEXT));
  if (Context == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto CloseProtocol;
  }

  Context->Signature = FUSB302_SIGNATURE;
  Context->I2cIo     = I2cIo;
  Context->Handle    = ControllerHandle;

  Context->TypeCPort.GetOrientation   = Fusb302GetOrientation;
  Context->TypeCPort.GetPowerContract = Fusb302GetPowerContract;
  Context->TypeCPort.GetDpAltMode     = Fusb302GetDpAltMode;
  Context->TypeCPort.PhyId          = 0;

  //
  // Find which combo PHY this controller belongs to.
  //
  Addresses = PcdGetPtr (PcdFusb302Addresses);
  Buses     = PcdGetPtr (PcdFusb302Buses);
  PhyIds    = PcdGetPtr (PcdFusb302PhyIds);
  Count     = PcdGetSize (PcdFusb302Addresses);

  if ((Addresses != NULL) && (Buses != NULL) && (PhyIds != NULL)) {
    for (Index = 0; Index < Count; Index++) {
      if (I2cIo->DeviceIndex == I2C_DEVICE_INDEX (Buses[Index], Addresses[Index])) {
        if (Index < PcdGetSize (PcdFusb302PhyIds)) {
          Context->TypeCPort.PhyId = PhyIds[Index];
        }

        break;
      }
    }
  }

  Status = Fusb302HwInit (Context);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to initialise FUSB302. Status=%r\n", __func__, Status));
    goto FreeContext;
  }

  //
  // Take an initial reading so that the log shows the port state even when
  // nothing goes on to ask for it.
  //
  Status = Fusb302DetectOrientation (Context, &Orientation);
  if (!EFI_ERROR (Status)) {
    Context->Orientation = Orientation;
    DEBUG ((
      DEBUG_INFO,
      "%a: PHY %u Type-C plug orientation: %a\n",
      __func__,
      Context->TypeCPort.PhyId,
      Orientation == UsbTypeCOrientationFlipped ? "flipped" : "normal"
      ));

    //
    // Only worth attempting where the partner is the source. Where this board
    // is the source there is nothing to ask for, and this driver does not
    // offer power itself.
    //
    if (Context->PartnerIsSource) {
      Status = Fusb302PdNegotiateSink (Context, Orientation);

      //
      // Alternate mode needs the PD link that negotiation leaves running. It
      // is the only way to learn whether a display is attached over a Type-C
      // connector, there being no hot-plug detect wire to read.
      //
      if (!EFI_ERROR (Status)) {
        Fusb302DpAltModeEnter (Context);
      }
    }

    if (!Context->Contract.PdNegotiated) {
      //
      // Fall back to describing what Type-C advertisement alone permits. The
      // conservative 5 V / 500 mA is what any port guarantees without PD.
      //
      Context->Contract.VoltageMv = 5000;
      Context->Contract.CurrentMa = 500;
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: PHY %u Type-C port is empty\n", __func__, Context->TypeCPort.PhyId));
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gUsbTypeCPortProtocolGuid,
                  &Context->TypeCPort,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto FreeContext;
  }

  return EFI_SUCCESS;

FreeContext:
  FreePool (Context);

CloseProtocol:
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );

  return Status;
}

EFI_STATUS
EFIAPI
Fusb302Stop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer
  )
{
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
Fusb302Initialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  gFusb302DriverBinding.ImageHandle       = ImageHandle;
  gFusb302DriverBinding.DriverBindingHandle = ImageHandle;

  return gBS->InstallMultipleProtocolInterfaces (
                &ImageHandle,
                &gEfiDriverBindingProtocolGuid,
                &gFusb302DriverBinding,
                NULL
                );
}
