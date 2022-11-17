/** @file
  USB policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _USB_CONFIG_H_
#define _USB_CONFIG_H_

#define USB_CONFIG_REVISION 2
extern EFI_GUID gUsbConfigGuid;

#pragma pack (push,1)

///
/// Overcurrent pins, the values match the setting of PCH EDS, please refer to PCH EDS for more details
///
typedef enum {
  PchUsbOverCurrentPin0 = 0,
  PchUsbOverCurrentPin1,
  PchUsbOverCurrentPin2,
  PchUsbOverCurrentPin3,
  PchUsbOverCurrentPin4,
  PchUsbOverCurrentPin5,
  PchUsbOverCurrentPin6,
  PchUsbOverCurrentPin7,
  PchUsbOverCurrentPinSkip,
  PchUsbOverCurrentPinMax
} PCH_USB_OVERCURRENT_PIN;

///
/// The location of the USB connectors.  This information is use to decide eye diagram tuning value for Usb 2.0 motherboard trace.
///
enum PCH_USB_PORT_LOCATION{
  PchUsbPortLocationBackPanel = 0,
  PchUsbPortLocationFrontPanel,
  PchUsbPortLocationDock,
  PchUsbPortLocationMiniPciE,
  PchUsbPortLocationFlex,
  PchUsbPortLocationInternalTopology,
  PchUsbPortLocationSkip,
  PchUsbPortLocationMax
};


/**
  This structure configures per USB2 AFE settings.
  It allows to setup the port parameters.
**/
typedef struct {
/** Per Port HS Preemphasis Bias (PERPORTPETXISET)
  000b - 0mV
  001b - 11.25mV
  010b - 16.9mV
  011b - 28.15mV
  100b - 28.15mV
  101b - 39.35mV
  110b - 45mV
  111b - 56.3mV
**/
  UINT8   Petxiset;
/** Per Port HS Transmitter Bias (PERPORTTXISET)
  000b - 0mV
  001b - 11.25mV
  010b - 16.9mV
  011b - 28.15mV
  100b - 28.15mV
  101b - 39.35mV
  110b - 45mV
  111b - 56.3mV
**/
  UINT8   Txiset;
/**
  Per Port HS Transmitter Emphasis (IUSBTXEMPHASISEN)
  00b - Emphasis OFF
  01b - De-emphasis ON
  10b - Pre-emphasis ON
  11b - Pre-emphasis & De-emphasis ON
**/
  UINT8   Predeemp;
/**
  Per Port Half Bit Pre-emphasis (PERPORTTXPEHALF)
  1b - half-bit pre-emphasis
  0b - full-bit pre-emphasis
**/
  UINT8   Pehalfbit;
} PCH_USB20_AFE;

/**
  This structure configures per USB2 port physical settings.
  It allows to setup the port location and port length, and configures the port strength accordingly.
**/
typedef struct {
  UINT32        Enable             :  1;     ///< 0: Disable; <b>1: Enable</b>.
  UINT32        RsvdBits0          : 31;     ///< Reserved bits
  /**
    These members describe the specific over current pin number of USB 2.0 Port N.
    It is SW's responsibility to ensure that a given port's bit map is set only for
    one OC pin Description. USB2 and USB3 on the same combo Port must use the same
    OC pin (see: PCH_USB_OVERCURRENT_PIN).
  **/
  UINT8         OverCurrentPin;
  UINT8         Rsvd0[3];                    ///< Reserved bytes, align to multiple 4.
  PCH_USB20_AFE Afe;                         ///< USB2 AFE settings
  UINT32        Rsvd1[1];                    ///< Reserved bytes
} PCH_USB20_PORT_CONFIG;

/**
  This structure describes whether the USB3 Port N of PCH is enabled by platform modules.
**/
typedef struct {
  UINT32   Enable             :  1;     ///< 0: Disable; <b>1: Enable</b>.
  UINT32   RsvdBits0          : 31;     ///< Reserved bits
  /**
    These members describe the specific over current pin number of USB 3.0 Port N.
    It is SW's responsibility to ensure that a given port's bit map is set only for
    one OC pin Description. USB2 and USB3 on the same combo Port must use the same
    OC pin (see: PCH_USB_OVERCURRENT_PIN).
  **/
  UINT8    OverCurrentPin;
  UINT8    Rsvd0[3];                 ///< Reserved bytes, align to multiple 4

  UINT32 HsioTxDeEmphEnable           :  1; ///< Enable the write to USB 3.0 TX Output -3.5dB De-Emphasis Adjustment, <b>0: Disable</b>; 1: Enable.
  /**
    USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting (ow2tapgen2deemph3p5)
    HSIO_TX_DWORD5[21:16]
    <b>Default = 29h</b> (approximately -3.5dB De-Emphasis)
  **/
  UINT32 HsioTxDeEmph                 :  6;

  UINT32 HsioTxDownscaleAmpEnable     :  1; ///< Enable the write to USB 3.0 TX Output Downscale Amplitude Adjustment, <b>0: Disable</b>; 1: Enable.
  /**
    USB 3.0 TX Output Downscale Amplitude Adjustment (orate01margin)
    HSIO_TX_DWORD8[21:16]
    <b>Default = 00h</b>
  **/
  UINT32 HsioTxDownscaleAmp           :  6;

  UINT32 RsvdBits1                    : 18; ///< Reserved bits
  UINT32 Rsvd1[1];                          ///< Reserved bytes
} PCH_USB30_PORT_CONFIG;

#define PCH_XHCI_MODE_OFF         0
#define PCH_XHCI_MODE_ON          1

/**
  These members describe some settings which are related to the SSIC ports.
**/
typedef struct {
  /**
    0: Disable; <b>1: Enable</b> SSIC support.
  **/
  UINT32  Enable          : 1;
  UINT32  RsvdBits1       : 31;
} PCH_XHCI_SSIC_PORT;
/**
  These members describe some settings which are related to the SSIC ports.
**/
typedef struct {
  PCH_XHCI_SSIC_PORT  SsicPort[PCH_XHCI_MAX_SSIC_PORT_COUNT];
} PCH_SSIC_CONFIG;

/**
  The PCH_XDCI_CONFIG block describes the configurations
  of the xDCI Usb Device controller.
**/
typedef struct {
  /**
    This member describes whether or not the xDCI controller should be enabled.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32  Enable              :  1;
  UINT32  RsvdBits0            : 31;     ///< Reserved bits
} PCH_XDCI_CONFIG;


/**
  This member describes the expected configuration of the PCH USB controllers,
  Platform modules may need to refer Setup options, schematic, BIOS specification
  to update this field.
  The Usb20OverCurrentPins and Usb30OverCurrentPins field must be updated by referring
  the schematic.

  <b>Revision 1</b>:
    - Initial version.
  <b>Revision 2</b>:
    - Added DelayPdoProgramming policy option.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    This policy will disable XHCI compliance mode on all ports. Complicance Mode should be default enabled.
    For the platform that support USB Type-C, it can disable Compliance Mode, and enable Compliance Mode when testing.
    <b>0:Disable</b> , 1: Enable
  **/
  UINT32                      DisableComplianceMode        :  1;
  /**
    This policy option if set will move Port Disable Override register programming to ReadyToBoot event during DXE phase.
    If not enabled it will program Port Disable Override register in PEI phase during PCH initialization
    <b>0: Program in PEI phase </b>, 1: Program in DXE phase
  **/
  UINT32                      DelayPdoProgramming          :  1;
  UINT32                      RsvdBits0                    : 30;     ///< Reserved bits
  /**
    These members describe whether the USB2 Port N of PCH is enabled by platform modules.
    Panel and Dock are used to describe the layout of USB port. Panel is only available for Desktop PCH.
    Dock is only available for Mobile LPT.
  **/
  PCH_USB20_PORT_CONFIG       PortUsb20[PCH_MAX_USB2_PORTS];
  /**
    These members describe whether the USB3 Port N of PCH is enabled by platform modules.
  **/
  PCH_USB30_PORT_CONFIG       PortUsb30[PCH_MAX_USB3_PORTS];
  /**
    This member describes whether or not the xDCI controller should be enabled.
  **/
  PCH_XDCI_CONFIG             XdciConfig;
  /**
    These members describe some settings which are related to the SSIC ports.
  **/
  PCH_SSIC_CONFIG             SsicConfig;
} PCH_USB_CONFIG;

#pragma pack (pop)

#endif // _USB_CONFIG_H_
