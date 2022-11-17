/** @file
  Sata policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SATA_CONFIG_H_
#define _SATA_CONFIG_H_

#include <PchLimits.h>

#define SATA_CONFIG_REVISION 1
extern EFI_GUID gSataConfigGuid;

#pragma pack (push,1)

typedef enum  {
  SataModeAhci,
  SataModeRaid,
  SataModeMax
} SATA_MODE;

typedef enum {
  SataSpeedDefault,
  SataSpeedGen1,
  SataSpeedGen2,
  SataSpeedGen3
} SATA_SPEED;

typedef enum {
  SataRstMsix,
  SataRstMsi,
  SataRstLegacy
} SATA_RST_INTERRUPT;

typedef enum {
  SataRaidClient,
  SataRaidAlternate,
  SataRaidServer
} SATA_RAID_DEV_ID;

/**
  This structure configures the features, property, and capability for each SATA port.
**/
typedef struct {
  /**
    Enable SATA port.
    It is highly recommended to disable unused ports for power savings
  **/
  UINT32  Enable           :  1;                  ///< 0: Disable; <b>1: Enable</b>
  UINT32  HotPlug          :  1;                  ///< <b>0: Disable</b>; 1: Enable
  UINT32  InterlockSw      :  1;                  ///< <b>0: Disable</b>; 1: Enable
  UINT32  External         :  1;                  ///< <b>0: Disable</b>; 1: Enable
  UINT32  SpinUp           :  1;                  ///< <b>0: Disable</b>; 1: Enable the COMRESET initialization Sequence to the device
  UINT32  SolidStateDrive  :  1;                  ///< <b>0: HDD</b>; 1: SSD
  UINT32  DevSlp           :  1;                  ///< <b>0: Disable</b>; 1: Enable DEVSLP on the port
  UINT32  EnableDitoConfig :  1;                  ///< <b>0: Disable</b>; 1: Enable DEVSLP Idle Timeout settings (DmVal, DitoVal)
  UINT32  DmVal            :  4;                  ///< DITO multiplier. Default is <b>15</b>.
  UINT32  DitoVal          : 10;                  ///< DEVSLP Idle Timeout (DITO), Default is <b>625</b>.
  /**
    Support zero power ODD <b>0: Disable</b>, 1: Enable.
    This is also used to disable ModPHY dynamic power gate.
  **/
  UINT32  ZpOdd             :  1;
  UINT32  DevSlpResetConfig :  4;                 ///< 0: Hardware default; <b>0x01: GpioResumeReset</b>; 0x03: GpioHostDeepReset; 0x05: GpioPlatformReset; 0x07: GpioDswReset
  UINT32  SataPmPtm         :  1;                 ///< Deprecated
  UINT32  RxPolarity        :  1;                 ///< <b>0: Disable</b>; 1: Enable; Rx Polarity
  UINT32  RsvdBits0         :  3;                 ///< Reserved fields for future expansion w/o protocol change
} PCH_SATA_PORT_CONFIG;

/**
  This structure lists PCH supported SATA thermal throttling register setting for customization.
  The settings is programmed through SATA Index/Data registers.
  When the SuggestedSetting is enabled, the customized values are ignored.
**/
typedef struct {
  UINT32  P0T1M                   :  2; ///< Port 0 T1 Multipler
  UINT32  P0T2M                   :  2; ///< Port 0 T2 Multipler
  UINT32  P0T3M                   :  2; ///< Port 0 T3 Multipler
  UINT32  P0TDisp                 :  2; ///< Port 0 Tdispatch

  UINT32  P1T1M                   :  2; ///< Port 1 T1 Multipler
  UINT32  P1T2M                   :  2; ///< Port 1 T2 Multipler
  UINT32  P1T3M                   :  2; ///< Port 1 T3 Multipler
  UINT32  P1TDisp                 :  2; ///< Port 1 Tdispatch

  UINT32  P0Tinact                :  2; ///< Port 0 Tinactive
  UINT32  P0TDispFinit            :  1; ///< Port 0 Alternate Fast Init Tdispatch
  UINT32  P1Tinact                :  2; ///< Port 1 Tinactive
  UINT32  P1TDispFinit            :  1; ///< Port 1 Alternate Fast Init Tdispatch
  UINT32  SuggestedSetting        :  1; ///< 0: Disable; <b>1: Enable</b> suggested representative values
  UINT32  RsvdBits0               :  9; ///< Reserved bits
} SATA_THERMAL_THROTTLING;

/**
   The SATA_CONFIG block describes the expected configuration of the SATA controllers.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                             ///< Config Block Header
  ///
  /// This member describes whether or not the SATA controllers should be enabled. 0: Disable; <b>1: Enable</b>.
  ///
  UINT8                         Enable;
  UINT8                         TestMode;       ///< <b>(Test)</b> <b>0: Disable</b>; 1: Allow entrance to the PCH SATA test modes
  UINT8                         SalpSupport;    ///< 0: Disable; <b>1: Enable</b> Aggressive Link Power Management
  UINT8                         PwrOptEnable;   ///< 0: Disable; <b>1: Enable</b> SATA Power Optimizer on PCH side.
  /**
    EsataSpeedLimit
    When enabled, BIOS will configure the PxSCTL.SPD to 2 to limit the eSATA port speed.
    Please be noted, this setting could be cleared by HBA reset, which might be issued
    by EFI AHCI driver when POST time, or by SATA inbox driver/RST driver after POST.
    To support the Speed Limitation when POST, the EFI AHCI driver should preserve the
    setting before and after initialization. For support it after POST, it's dependent on
    driver's behavior.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT8                         EsataSpeedLimit;
  UINT8                         LedEnable;       ///< SATA LED indicates SATA controller activity. 0: Disable; <b>1: Enable</b> SATA LED.
  /**
    This option allows to configure SATA controller device ID while in RAID mode.
    Refer to SATA_RAID_DEV_ID enumeration for supported options.
    Choosing Client will allow RST driver loading, RSTe driver will not be able to load
    Choosing Alternate will not allow RST inbox driver loading in Windows
    Choosing Server will allow RSTe driver loading, RST driver will not load
    <b>0: Client</b>; 1: Alternate; 2: Server
  **/
  UINT8                        RaidDeviceId;
  /**
    Controlls which interrupts will be linked to SATA controller CAP list
    This option will take effect only if SATA controller is in RAID mode
    Default: <b>PchSataMsix</b>
  **/
  UINT8                         SataRstInterrupt;

  /**
    Determines the system will be configured to which SATA mode.
    Refer to SATA_MODE enumeration for supported options. Default is <b>SataModeAhci</b>.
  **/
  UINT8                         SataMode;
  /**
    Indicates the maximum speed the SATA controller can support.
    Refer to SATA_SPEED enumeration for supported options.
    <b>0h: SataSpeedDefault</b>; 1h: 1.5 Gb/s (Gen 1); 2h: 3 Gb/s(Gen 2); 3h: 6 Gb/s (Gen 1)
  **/
  UINT8                         SpeedLimit;
  UINT8                         EnclosureSupport;   ///< Enclosure Management Support. 0: Disable; 1: Enable
  /**
    Controlls whenever Serial GPIO support is enabled for controller
    <b>0: Disable</b>; 1: Enable
  **/
  UINT8                         SgpioSupport;
  /**
    This member configures the features, property, and capability for each SATA port.
  **/
  PCH_SATA_PORT_CONFIG          PortSettings[PCH_MAX_SATA_PORTS];
  /**
    This field decides the settings of Sata thermal throttling. When the Suggested Setting
    is enabled, PCH RC will use the suggested representative values.
  **/
  SATA_THERMAL_THROTTLING       ThermalThrottling;
} SATA_CONFIG;

#pragma pack (pop)

#endif // _SATA_CONFIG_H_
