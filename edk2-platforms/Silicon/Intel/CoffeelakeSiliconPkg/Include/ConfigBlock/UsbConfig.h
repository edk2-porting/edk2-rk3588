/** @file
  Common USB policy shared between PCH and CPU
  Contains general features settings for xHCI and xDCI

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _USB_CONFIG_H_
#define _USB_CONFIG_H_

#define USB_CONFIG_REVISION 3
extern EFI_GUID gUsbConfigGuid;

#define MAX_USB2_PORTS  16
#define MAX_USB3_PORTS  10

#pragma pack (push,1)

#define PCH_USB_OC_PINS_MAX  8  ///< Maximal possible number of USB Over Current pins

///
/// Overcurrent pins, the values match the setting of EDS, please refer to EDS for more details
///
typedef enum {
  UsbOverCurrentPin0 = 0,
  UsbOverCurrentPin1,
  UsbOverCurrentPin2,
  UsbOverCurrentPin3,
  UsbOverCurrentPin4,
  UsbOverCurrentPin5,
  UsbOverCurrentPin6,
  UsbOverCurrentPin7,
  UsbOverCurrentPinMax,
  UsbOverCurrentPinSkip = 0xFF
} USB_OVERCURRENT_PIN;

/**
  This structure configures per USB2 AFE settings.
  It allows to setup the port electrical parameters.
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
} USB20_AFE;

/**
  This structure configures per USB2 port physical settings.
  It allows to setup the port location and port length, and configures the port strength accordingly.
**/
typedef struct {
  /**
    These members describe the specific over current pin number of USB 2.0 Port N.
    It is SW's responsibility to ensure that a given port's bit map is set only for
    one OC pin Description. USB2 and USB3 on the same combo Port must use the same
    OC pin (see: USB_OVERCURRENT_PIN).
  **/
  UINT32     OverCurrentPin     :  8;
  UINT32     Enable             :  1;     ///< 0: Disable; <b>1: Enable</b>.
  UINT32     RsvdBits0          : 23;     ///< Reserved bits
  /**
    Changing this policy values from default ones may require disabling USB2 PHY Sus Well Power Gating
    through Usb2PhySusPgEnable on PCH-LP
  **/
  USB20_AFE  Afe;                         ///< USB2 AFE settings
} USB20_PORT_CONFIG;

/**
  This structure describes whether the USB3 Port N is enabled by platform modules.
**/
typedef struct {
  /**
    These members describe the specific over current pin number of USB 3.x Port N.
    It is SW's responsibility to ensure that a given port's bit map is set only for
    one OC pin Description. USB2 and USB3 on the same combo Port must use the same
    OC pin (see: USB_OVERCURRENT_PIN).
  **/
  UINT32  OverCurrentPin            :  8;

  /**
    USB 3.0 TX Output Downscale Amplitude Adjustment (orate01margin)
    HSIO_TX_DWORD8[21:16]
    <b>Default = 00h</b>
  **/
  UINT32  HsioTxDownscaleAmp        :  8;
  /**
    USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting (ow2tapgen2deemph3p5)
    HSIO_TX_DWORD5[21:16]
    <b>Default = 29h</b> (approximately -3.5dB De-Emphasis)
  **/
  UINT32  HsioTxDeEmph              :  8;

  UINT32  Enable                    :  1; ///< 0: Disable; <b>1: Enable</b>.
  UINT32  HsioTxDeEmphEnable        :  1; ///< Enable the write to USB 3.0 TX Output -3.5dB De-Emphasis Adjustment, <b>0: Disable</b>; 1: Enable.
  UINT32  HsioTxDownscaleAmpEnable  :  1; ///< Enable the write to USB 3.0 TX Output Downscale Amplitude Adjustment, <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0                 :  5; ///< Reserved bits
} USB30_PORT_CONFIG;

/**
  The XDCI_CONFIG block describes the configurations
  of the xDCI Usb Device controller.
**/
typedef struct {
  /**
    This member describes whether or not the xDCI controller should be enabled.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32  Enable              :  1;
  UINT32  RsvdBits0           : 31;     ///< Reserved bits
} XDCI_CONFIG;

//
// Below defines are for proper UPD construction and values syncing between UPD and policy
//
#define B_XHCI_HSIO_CTRL_ADAPT_OFFSET_CFG_EN      BIT0  ///< Enable the write to Signed Magnatude number added to the CTLE code bit
#define B_XHCI_HSIO_FILTER_SELECT_N_EN            BIT1  ///< Enable the write to LFPS filter select for n
#define B_XHCI_HSIO_FILTER_SELECT_P_EN            BIT2  ///< Enable the write to LFPS filter select for p
#define B_XHCI_HSIO_LFPS_CFG_PULLUP_DWN_RES_EN    BIT3  ///< Enable the write to olfpscfgpullupdwnres
#define N_XHCI_UPD_HSIO_CTRL_ADAPT_OFFSET_CFG     3
#define N_XHCI_UPD_HSIO_LFPS_CFG_PULLUP_DWN_RES   0
#define N_XHCI_UPD_HSIO_FILTER_SELECT_P           0
#define N_XHCI_UPD_HSIO_FILTER_SELECT_N           4

typedef struct {
  /**
    Signed Magnatude number added to the CTLE code.(ctle_adapt_offset_cfg_4_0)
    HSIO_RX_DWORD25 [20:16]
    Ex: -1 -- 1_0001. +1: 0_0001
    <b>Default = 0h</b>
  **/
  UINT32  HsioCtrlAdaptOffsetCfg      :  5;
  /**
    LFPS filter select for n (filter_sel_n_2_0)
    HSIO_RX_DWORD51 [29:27]
    0h:1.6ns
    1h:2.4ns
    2h:3.2ns
    3h:4.0ns
    4h:4.8ns
    5h:5.6ns
    6h:6.4ns
    <b>Default = 0h</b>
  **/
  UINT32  HsioFilterSelN              :  3;
  /**
    LFPS filter select for p (filter_sel_p_2_0)
    HSIO_RX_DWORD51 [26:24]
    0h:1.6ns
    1h:2.4ns
    2h:3.2ns
    3h:4.0ns
    4h:4.8ns
    5h:5.6ns
    6h:6.4ns
    <b>Default = 0h</b>
  **/
  UINT32  HsioFilterSelP              :  3;
  /**
    Controls the input offset (olfpscfgpullupdwnres_sus_usb_2_0)
    HSIO_RX_DWORD51 [2:0]
    000 Prohibited
    001 45K
    010 Prohibited
    011 31K
    100 36K
    101 36K
    110 36K
    111 36K
    <b>Default = 3h</b>
  **/
  UINT32  HsioOlfpsCfgPullUpDwnRes    :  3;

  UINT32  HsioCtrlAdaptOffsetCfgEnable    :  1; ///< Enable the write to Signed Magnatude number added to the CTLE code, <b>0: Disable</b>; 1: Enable.
  UINT32  HsioFilterSelNEnable            :  1; ///< Enable the write to LFPS filter select for n, <b>0: Disable</b>; 1: Enable.
  UINT32  HsioFilterSelPEnable            :  1; ///< Enable the write to LFPS filter select for p, <b>0: Disable</b>; 1: Enable.
  UINT32  HsioOlfpsCfgPullUpDwnResEnable  :  1; ///< Enable the write to olfpscfgpullupdwnres, <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0                       : 14; ///< Reserved bits
} USB30_HSIO_RX_CONFIG;


/**
  This member describes the expected configuration of the USB controller,
  Platform modules may need to refer Setup options, schematic, BIOS specification to update this field.
  The Usb20OverCurrentPins and Usb30OverCurrentPins field must be updated by referring the schematic.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added Usb2PhySusPgEnable - for enabling/disabling USB2 PHY SUS Well Power Gating
  <b>Revision 3</b>:
    Added HSIO Rx tuning policy options structure USB30_HSIO_RX_CONFIG
**/
typedef struct {
  CONFIG_BLOCK_HEADER     Header;                   ///< Config Block Header
  /**
    This policy setting controls state of Compliance Mode enabling.
    Compliance Mode can be enabled for testing through this option but defualt setting is Disabled.
    <b>0:Disable</b>, 1: Enable
  **/
  UINT32                  EnableComplianceMode         :  1;
  /**
    This policy option when set will make BIOS program Port Disable Override register during PEI phase.
    When disabled BIOS will not program the PDO during PEI phase and leave PDO register unlocked for later programming.
    If this is disabled, platform code MUST set it before booting into OS.
    <b>1: Enable</b>, 0: Disable
  **/
  UINT32                  PdoProgramming               :  1;
  /**
    This option allows for control whether USB should program the Overcurrent Pins mapping into xHCI.
    Disabling this feature will disable overcurrent detection functionality.
    Overcurrent Pin mapping data is contained in respective port structures (i.e. USB30_PORT_CONFIG) in OverCurrentPin field.
    By default this Overcurrent functionality should be enabled and disabled only for OBS debug usage.
    <b>1: Will program USB OC pin mapping in respective xHCI controller registers</b>
    0: Will clear OC pin mapping allow for OBS usage of OC pins
  **/
  UINT32                  OverCurrentEnable            :  1;
  /**
    <b>(Test)</b>
    If this policy option is enabled then BIOS will program OCCFDONE bit in xHCI meaning that OC mapping data will be
    consumed by xHCI and OC mapping registers will be locked. OverCurrent mapping data is taken from respective port data
    structure from OverCurrentPin field.
    If EnableOverCurrent policy is enabled this also should be enabled, otherwise xHCI won't consume OC mapping data.
    <b>1: Program OCCFDONE bit and make xHCI consume OverCurrent mapping data</b>
    0: Do not program OCCFDONE bit making it possible to use OBS debug on OC pins.
  **/
  UINT32                  XhciOcLock                   :  1;
  /**
    <b>(Test)</b>
    This policy option enables USB2 PHY SUS Well Power Gating functionality.
    Please note this is ignored on PCH H
    <b>0: disable USB2 PHY SUS Well Power Gating</b>
    1: enable USB2 PHY SUS Well Power Gating
  **/
  UINT32                  Usb2PhySusPgEnable           :  1;
  UINT32                  RsvdBits0                    : 27;     ///< Reserved bits
  /**
    These members describe whether the USB2 Port N of PCH is enabled by platform modules.
  **/
  USB20_PORT_CONFIG       PortUsb20[MAX_USB2_PORTS];
  /**
    These members describe whether the USB3 Port N of PCH is enabled by platform modules.
  **/
  USB30_PORT_CONFIG       PortUsb30[MAX_USB3_PORTS];
  /**
    This member describes whether or not the xDCI controller should be enabled.
  **/
  XDCI_CONFIG             XdciConfig;
  /**
    This member describes policy options for RX signal tuning in ModPHY
  **/
  USB30_HSIO_RX_CONFIG    PortUsb30HsioRx[MAX_USB3_PORTS];
} USB_CONFIG;

#pragma pack (pop)

#endif // _USB_CONFIG_H_
