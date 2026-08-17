/** @file
*
*  Everything Fusb302Dxe needs to know about one Type-C port: which I2C
*  device the controller answers on, which combo PHY it belongs to, what the
*  port may source and will sink, and how to switch its supply.
*
*  One instance is produced per port and consumed by Fusb302Dxe. The split
*  keeps the controller driver free of any knowledge of the board it happens
*  to be wired to -- it never touches a GPIO or a PCD, so a different
*  platform can carry it by producing this protocol alone.
*
*  Copyright (c) 2026 Corey Moyer <cronmod.dev@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _FUSB302_PLATFORM_DEVICE_PROTOCOL_H_
#define _FUSB302_PLATFORM_DEVICE_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

#define FUSB302_PLATFORM_DEVICE_PROTOCOL_GUID   \
    { 0x8afa635c, 0x2704, 0x4528, { 0xbf, 0xf4, 0xe5, 0xc0, 0xa4, 0x59, 0xbe, 0xc5 } }

//
// Power data objects a board may list for one port. Four is more than any
// board here needs and keeps the whole table small.
//
#define FUSB302_MAX_PDOS  4

typedef struct _FUSB302_PLATFORM_DEVICE_PROTOCOL FUSB302_PLATFORM_DEVICE_PROTOCOL;

/**
  Switch this port's VBUS supply on or off.

  Produced only by a board that can actually energise the rail. Where the
  port cannot source at all the member is NULL, which the controller driver
  reads as "never offer power here".

  @param[in] This     Protocol instance.
  @param[in] Enable   TRUE to energise the rail, FALSE to drop it.

  @retval EFI_SUCCESS       The supply was switched.
  @retval EFI_DEVICE_ERROR  The supply could not be reached.

**/
typedef
EFI_STATUS
(EFIAPI *FUSB302_PLATFORM_SET_VBUS)(
  IN FUSB302_PLATFORM_DEVICE_PROTOCOL   *This,
  IN BOOLEAN                            Enable
  );

struct _FUSB302_PLATFORM_DEVICE_PROTOCOL {
  //
  // Which I2C device the controller answers on, in the terms the bus driver
  // publishes it under. Stated this way rather than as a bus and address so
  // that the consumer needs no knowledge of how a platform numbers its I2C.
  //
  EFI_GUID                     DeviceGuid;
  UINT32                       DeviceIndex;
  //
  // The USBDP combo PHY this port is wired to, matching DP_PHY_PROTOCOL.Id.
  //
  UINT32                       PhyId;
  //
  // What the port may offer an attached sink. All zero means it never
  // supplies power. Only the first object is advertised: the VBUS switch is
  // a plain on/off, so the rail cannot move to a second voltage on request.
  //
  UINT32                       SourcePdos[FUSB302_MAX_PDOS];
  //
  // What the port will accept from an attached source. The 5 V fixed supply
  // comes first, as the specification requires of any sink. A source's offer
  // is requested only where one of these covers it.
  //
  UINT32                       SinkPdos[FUSB302_MAX_PDOS];
  //
  // NULL where the board has no supply switch this driver can reach, which
  // stops the port sourcing whatever it offers above.
  //
  FUSB302_PLATFORM_SET_VBUS    SetVbus;
};

extern EFI_GUID  gFusb302PlatformDeviceProtocolGuid;

#endif // _FUSB302_PLATFORM_DEVICE_PROTOCOL_H_
