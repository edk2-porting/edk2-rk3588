/** @file
  Sata policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SATA_CONFIG_H_
#define _SATA_CONFIG_H_

#include <PchLimits.h>

#define SATA_CONFIG_REVISION 2
extern EFI_GUID gSataConfigGuid;

#pragma pack (push,1)

typedef enum  {
  PchSataModeAhci,
  PchSataModeRaid,
  PchSataModeMax
} PCH_SATA_MODE;

typedef enum {
  PchSataOromDelay2sec,
  PchSataOromDelay4sec,
  PchSataOromDelay6sec,
  PchSataOromDelay8sec
} PCH_SATA_OROM_DELAY;

typedef enum {
  PchSataSpeedDefault,
  PchSataSpeedGen1,
  PchSataSpeedGen2,
  PchSataSpeedGen3
} PCH_SATA_SPEED;

typedef enum {
  PchSataRstMsix,
  PchSataRstMsi,
  PchSataRstLegacy
} PCH_SATA_RST_INTERRUPT;

typedef enum {
  PchSataRaidClient,
  PchSataRaidAlternate,
  PchSataRaidServer
} PCH_SATA_RAID_DEV_ID;

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
  UINT32  ZpOdd            :  1;
  UINT32  RsvdBits0        :  9;                  ///< Reserved fields for future expansion w/o protocol change
} PCH_SATA_PORT_CONFIG;

/**
  Rapid Storage Technology settings.
**/
typedef struct {
  UINT32  Raid0              :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID0
  UINT32  Raid1              :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID1
  UINT32  Raid10             :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID10
  UINT32  Raid5              :  1;         ///< 0  : Disable; <b>1  : Enable</b> RAID5
  UINT32  Irrt               :  1;         ///< 0  : Disable; <b>1  : Enable</b> Intel Rapid Recovery Technology
  UINT32  OromUiBanner       :  1;         ///< 0  : Disable; <b>1  : Enable</b> OROM UI and BANNER
  UINT32  OromUiDelay        :  2;         ///< <b>00b  : 2 secs</b>; 01b  : 4 secs; 10b  : 6 secs; 11  : 8 secs (see  : PCH_SATA_OROM_DELAY)
  UINT32  HddUnlock          :  1;         ///< 0  : Disable; <b>1  : Enable</b>. Indicates that the HDD password unlock in the OS is enabled
  UINT32  LedLocate          :  1;         ///< 0  : Disable; <b>1  : Enable</b>. Indicates that the LED/SGPIO hardware is attached and ping to locate feature is enabled on the OS
  UINT32  IrrtOnly           :  1;         ///< 0  : Disable; <b>1  : Enable</b>. Allow only IRRT drives to span internal and external ports
  UINT32  SmartStorage       :  1;         ///< 0  : Disable; <b>1  : Enable</b> RST Smart Storage caching Bit
  UINT32  LegacyOrom         :  1;         ///< <b>0  : Disable</b>; 1  : Enable RST Legacy OROM
  UINT32  OptaneMemory       :  1;         ///< 0: Disable; <b>1: Enable</b> RST Optane(TM) Memory
  UINT32  CpuAttachedStorage :  1;         ///< 0: Disable; <b>1: Enable</b> CPU Attached Storage
  /**
    This option allows to configure SATA controller device ID while in RAID mode.
    Refer to PCH_SATA_RAID_DEV_ID enumeration for supported options.
    Choosing Client will allow RST driver loading, RSTe driver will not be able to load
    Choosing Alternate will not allow RST inbox driver loading in Windows
    Choosing Server will allow RSTe driver loading, RST driver will not load
    <b>0: Client</b>; 1: Alternate; 2: Server
  **/
  UINT32  RaidDeviceId       :  2;
  /**
    Controlls which interrupts will be linked to SATA controller CAP list
    This option will take effect only if SATA controller is in RAID mode
    Default: <b>PchSataMsix</b>
  **/
  UINT32  SataRstInterrupt   :  2;
  UINT32  RsvdBits0          : 13;         ///< Reserved Bits
} PCH_SATA_RST_CONFIG;

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
  This structure describes the details of Intel RST for PCIe Storage remapping
  Note: In order to use this feature, Intel RST Driver is required
**/
typedef struct {
  /**
    This member describes whether or not the Intel RST for PCIe Storage remapping should be enabled. <b>0: Disable</b>; 1: Enable.
    Note 1: If Sata Controller is disabled, PCIe Storage Remapping should be disabled as well
    Note 2: If PCIe Storage remapping is enabled, the PCH integrated AHCI controllers Class Code is configured as RAID
  **/
  UINT32   Enable                 :  1;
  /**
    Intel RST for PCIe Storage remapping - PCIe Port Selection (1-based, <b>0 = autodetect</b>)
    The supported ports for PCIe Storage remapping is different depend on the platform and cycle router, the assignments are as below:
    i.)   RST PCIe Storage Cycle Router 2 -> RP5 - RP8
    ii.)  RST PCIe Storage Cycle Router 3 -> RP9 - RP12

    i.)   RST PCIe Storage Cycle Router 1 -> RP9  - RP12
    ii.)  RST PCIe Storage Cycle Router 2 -> RP13 - RP16
    iii.) RST PCIe Storage Cycle Router 3 -> RP17 - RP20
  **/
  UINT32   RstPcieStoragePort     :  5;
  UINT32   RsvdBits0              :  2; ///< Reserved bit
  /**
    PCIe Storage Device Reset Delay in milliseconds (ms), which it guarantees such delay gap is fulfilled
    before PCIe Storage Device configuration space is accessed after an reset caused by the link disable and enable step.
    Default value is <b>100ms</b>.
  **/
  UINT32   DeviceResetDelay       :  8;
  UINT32   RsvdBits1              : 16; ///< Reserved bits

  UINT32   Rsvd0[2];                    ///< Reserved bytes
} PCH_RST_PCIE_STORAGE_CONFIG;

/**
  The PCH_SATA_CONFIG block describes the expected configuration of the SATA controllers.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add CpuAttachedStorage in PCH_SATA_RST_CONFIG.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                             ///< Config Block Header
  ///
  /// This member describes whether or not the SATA controllers should be enabled. 0: Disable; <b>1: Enable</b>.
  ///
  UINT32                        Enable          :  1;
  UINT32                        TestMode        :  1;       ///< <b>(Test)</b> <b>0: Disable</b>; 1: Allow entrance to the PCH SATA test modes
  UINT32                        SalpSupport     :  1;       ///< 0: Disable; <b>1: Enable</b> Aggressive Link Power Management
  UINT32                        PwrOptEnable    :  1;       ///< 0: Disable; <b>1: Enable</b> SATA Power Optimizer on PCH side.
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
  UINT32                        EsataSpeedLimit :  1;
  UINT32                        LedEnable       :  1;       ///< SATA LED indicates SATA controller activity. 0: Disable; <b>1: Enable</b> SATA LED.
  UINT32                        RsvdBits0       : 26;       ///< Reserved bits

  /**
    Determines the system will be configured to which SATA mode (PCH_SATA_MODE). Default is <b>PchSataModeAhci</b>.
  **/
  PCH_SATA_MODE                 SataMode;
  /**
    Indicates the maximum speed the SATA controller can support
    <b>0h: PchSataSpeedDefault</b>; 1h: 1.5 Gb/s (Gen 1); 2h: 3 Gb/s(Gen 2); 3h: 6 Gb/s (Gen 1)
  **/
  PCH_SATA_SPEED                SpeedLimit;
  /**
    This member configures the features, property, and capability for each SATA port.
  **/
  PCH_SATA_PORT_CONFIG          PortSettings[PCH_MAX_SATA_PORTS];
  PCH_SATA_RST_CONFIG           Rst;                        ///< Setting applicable to Rapid Storage Technology
  /**
    This member describes the details of implementation of Intel RST for PCIe Storage remapping (Intel RST Driver is required)
    Note: RST for PCIe Sorage remapping is supported only for first SATA controller if more controllers are available
  **/
  PCH_RST_PCIE_STORAGE_CONFIG   RstPcieStorageRemap[PCH_MAX_RST_PCIE_STORAGE_CR];
  /**
    This field decides the settings of Sata thermal throttling. When the Suggested Setting
    is enabled, PCH RC will use the suggested representative values.
  **/
  SATA_THERMAL_THROTTLING       ThermalThrottling;
} PCH_SATA_CONFIG;

#pragma pack (pop)

#endif // _SATA_CONFIG_H_
