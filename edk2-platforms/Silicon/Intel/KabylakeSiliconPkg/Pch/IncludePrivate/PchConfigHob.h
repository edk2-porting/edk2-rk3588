/** @file
  The GUID definition for PchConfigHob

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_CONFIG_HOB_H_
#define _PCH_CONFIG_HOB_H_

#include <ConfigBlock.h>
#include <PchAccess.h>
#include <ConfigBlock/SmbusConfig.h>
#include <ConfigBlock/InterruptConfig.h>
#include <ConfigBlock/PcieRpConfig.h>
#include <ConfigBlock/SataConfig.h>
#include <ConfigBlock/FlashProtectionConfig.h>

extern EFI_GUID gPchConfigHobGuid;

#pragma pack (push,1)

///
/// This structure contains the HOB which are related to PCH general config.
///
typedef struct {
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32    Crid            :  1;
  UINT32    RsvdBits0       : 31;       ///< Reserved bits
} GENERAL_HOB;

///
/// The SMBUS_CONFIG block lists the reserved addresses for non-ARP capable devices in the platform.
///
typedef struct {
  UINT8   RsvdBytes[3];
  UINT8   NumRsvdSmbusAddresses;        ///< The number of elements in the RsvdSmbusAddressTable.
  /**
    Array of addresses reserved for non-ARP-capable SMBus devices.
  **/
  UINT8   RsvdSmbusAddressTable[PCH_MAX_SMBUS_RESERVED_ADDRESS];
} SMBUS_HOB;

///
/// The INTERRUPT describes interrupt settings for PCH HOB.
///
typedef struct {
  UINT8                        NumOfDevIntConfig;                               ///< Number of entries in DevIntConfig table
  UINT8                        GpioIrqRoute;                                    ///< Interrupt routing for GPIO. Default is <b>14</b>.
  UINT8                        Rsvd0[2];                                        ///< Reserved bytes, align to multiple 4.
  PCH_DEVICE_INTERRUPT_CONFIG  DevIntConfig[PCH_MAX_DEVICE_INTERRUPT_CONFIG];   ///< Array which stores PCH devices interrupts settings
} INTERRUPT_HOB;

///
/// The CIO2_HOB block describes SkyCam CIO2 device.
///
typedef struct {
  /**
    NOTE: For SKL PCH, while CIO2 is enabled,
    RC will configure CIO2 controller as ACPI mode when PCH stepping < C0, and configure to PCI mode for C0 onwards.
  **/
  UINT32 DeviceEnable              :  2;          ///< 0: Disabled, <b>1: Enabled</b>
  UINT32 RsvdBits0                 : 30;
} CIO2_HOB;

/**
  The SERIAL_IO block provides the configurations to set the Serial IO controllers
  to Acpi devices or Pci controllers, and also set the interrupt type to Acpi or Pci
  through Policy. It also provides to configure the I2c0 and I2c1 voltage
  to 1.8v or 3.3v by platform setting.
  Please refer to PeiDxeSmmPchSerialIoLib.h for definition of device numbers and enum values for the below fields
**/
typedef struct {
  /**
    0: Disabled; 1: ACPI Mode; <b>2: PCI Mode</b>; 3: Hidden mode, 4: Legacy UART mode, 5: Skip Init
    NOTE: Considering the PcdSerialIoUartDebugEnable and PcdSerialIoUartNumber for all SerialIo UARTx,
          the PCD is more meaningful to represent the board design. It means, if PcdSerialIoUartDebugEnable is not 0,
          the board is designed to use the SerialIo UART for debug message and the PcdSerialIoUartNumber is dedicated
          to be Debug UART usage. Therefore, it should grayout the option from setup menu since no other options
          available for this UART controller on this board, and also override the policy default accordingly.
          While PcdSerialIoUartDebugEnable is 0, then it's allowed to configure the UART controller by policy.
    NOTE: While DevMode is set to 5 (Skip Init), BIOS will not initialize this controller. Platform is resonsible for
          configuring this controller. If platform initializes the Serial IO controller in Hidden Mode, it MUST
          follow the predefined BAR address for the controller.
  **/
  UINT8  DevMode[PCH_SERIALIO_MAX_CONTROLLERS];             ///< serial io max controllers default is <b>11</b>.
  UINT8  Gpio;                                              ///< 0: Disabled; <b>1: Enabled</b>.
  UINT32 DebugUartNumber           :  2;                    ///< UART number for debug purpose. 0:UART0, 1: UART1, <b>2:UART2</b>
  UINT32 EnableDebugUartAfterPost  :  1;                    ///< Enable debug UART controller after post. 0: diabled, <b>1: enabled</b>
  UINT32 RsvdBits0                 : 29;
} SERIAL_IO_HOB;


///
/// The PCH_PCIE_CONFIG block describes the expected configuration of the PCH PCI Express controllers
///
typedef struct {
  ///
  /// These members describe the configuration of each PCH PCIe root port.
  ///
  PCH_PCIE_ROOT_PORT_CONFIG         RootPort[PCH_MAX_PCIE_ROOT_PORTS];
  /**
    This member allows BIOS to control ICC PLL Shutdown by determining PCIe devices are LTR capable
    or leaving untouched.
    - <b>0: Disable, ICC PLL Shutdown is determined by PCIe device LTR capablility.</b>
      - To allow ICC PLL shutdown if all present PCIe devices are LTR capable or if no PCIe devices are
        presented for maximum power savings where possible.
      - To disable ICC PLL shutdown when BIOS detects any non-LTR capable PCIe device for ensuring device
        functionality.
    - 1: Enable, To allow ICC PLL shutdown even if some devices do not support LTR capability.
  **/
  UINT32  AllowNoLtrIccPllShutdown         :  1;
  UINT32  RsvdBits0                        : 31;
} PCIERP_HOB;

typedef struct {
  UINT32  DspEnable            :  1;    ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  /**
    Bitmask of supported DSP endpoint configuration exposed via NHLT ACPI table:
  **/
  UINT32  DspEndpointDmic      :  2;    ///< DMIC Select (PCH_HDAUDIO_DMIC_TYPE enum): 0: Disable; 1: 2ch array; <b>2: 4ch array</b>; 3: 1ch array
  UINT32  DspEndpointBluetooth :  1;    ///< Bluetooth enablement: <b>0: Disable</b>; 1: Enable
  UINT32  DspEndpointI2s       :  1;    ///< I2S enablement: <b>0: Disable</b>; 1: Enable
  UINT32  RsvdBits0            : 27;    ///< Reserved bits
  /**
    Bitmask of supported DSP features:
    [BIT0] - WoV; [BIT1] - BT Sideband; [BIT2] - Codec VAD; [BIT5] - BT Intel HFP; [BIT6] - BT Intel A2DP
    [BIT7] - DSP based speech pre-processing disabled; [BIT8] - 0: Intel WoV, 1: Windows Voice Activation
    Default is <b>zero</b>.
  **/
  UINT32  DspFeatureMask;
} HDAUDIO_HOB;

typedef struct {
  ///
  /// This member describes whether or not the SATA controllers should be enabled. 0: Disable; <b>1: Enable</b>.
  ///
  UINT32                        Enable          :  1;
  UINT32                        TestMode        :  1;       ///< <b>(Test)</b> <b>0: Disable</b>; 1: Allow entrance to the PCH SATA test modes
  UINT32                        RsvdBits0       : 30;       ///< Reserved bits
  /**
    This member configures the features, property, and capability for each SATA port.
  **/
  PCH_SATA_PORT_CONFIG          PortSettings[PCH_MAX_SATA_PORTS];
  /**
    This member describes the details of implementation of Intel RST for PCIe Storage remapping (Intel RST Driver is required)
  **/
  PCH_RST_PCIE_STORAGE_CONFIG   RstPcieStorageRemap[PCH_MAX_RST_PCIE_STORAGE_CR];
} SATA_HOB;

/**
  The THERMAL_HOB block describes the expected configuration of the PCH for Thermal.
**/
typedef struct {
  UINT32  ThermalDeviceEnable     :  2; ///< 0: Disabled, <b>1: Enabled in PCI mode</b>, 2: Enabled in ACPI mode
  UINT32  RsvdBits0               : 30; ///< Reserved bits
} THERMAL_HOB;

/**
  This structure is used for the emulation feature for Port61h read. The port is trapped
  and the SMI handler will toggle bit4 according to the handler's internal state.
**/
typedef struct {
  UINT32                Enable                    :  1;     ///< 0: Disable; <b>1: Enable</b> the emulation
  UINT32                RsvdBits0                 : 31;    ///< Reserved bits
} PORT61H_SMM_HOB;

///
/// The SCS_HOB block describes Storage and Communication Subsystem (SCS) settings for PCH.
///
typedef struct {
  UINT32  ScsEmmcEnabled                      :  2;  ///< Determine if eMMC is enabled - 0: Disabled, <b>1: Enabled</b>.
  UINT32  ScsEmmcHs400Enabled                 :  1;  ///< Determine eMMC HS400 Mode if ScsEmmcEnabled - <b>0: Disabled</b>, 1: Enabled
  /**
    Determine if HS400 Training is required, set to FALSE if Hs400 Data is valid. <b>0: Disabled</b>, 1: Enabled.
    First Boot or CMOS clear, system boot with Default settings, set tuning required.
    Subsequent Boots, Get Variable 'Hs400TuningData'
      - if failed to get variable, set tuning required
      - if passed, retrieve Hs400DataValid, Hs400RxStrobe1Dll and Hs400TxDataDll from variable. Set tuning not required.
  **/
  UINT32  ScsEmmcHs400TuningRequired          :  1;
  UINT32  ScsEmmcHs400DllDataValid            :  1;  ///< Set if HS400 Tuning Data Valid
  UINT32  ScsEmmcHs400DriverStrength          :  3;  ///< I/O driver strength: <b>0 - 33 Ohm</b>, 1 - 40 Ohm, 2 - 50 Ohm
  UINT32  RsvdBits                            : 24;

} SCS_HOB;

/**
  The PCH_LOCK_DOWN_CONFIG block describes the expected configuration of the PCH
  for security requirement.
**/
typedef struct {
  UINT32  GlobalSmi      :  1;
  /**
    <b>(Test)</b> Enable BIOS Interface Lock Down bit to prevent writes to the Backup Control Register
    Top Swap bit and the General Control and Status Registers Boot BIOS Straps. 0: Disable; <b>1: Enable</b>.
  **/
  UINT32  BiosInterface  :  1;
  /**
    <b>(Test)</b> Enable RTC lower and upper 128 byte Lock bits to lock Bytes 38h-3Fh in the upper
    and lower 128-byte bank of RTC RAM. 0: Disable; <b>1: Enable</b>.
  **/
  UINT32  RtcLock        :  1;
  /**
    Enable the BIOS Lock Enable (BLE) feature and set EISS bit (D31:F5:RegDCh[5])
    for the BIOS region protection. When it is enabled, the BIOS Region can only be
    modified from SMM after EndOfDxe protocol is installed.
    Note: When BiosLock is enabled, platform code also needs to update to take care
    of BIOS modification (including SetVariable) in DXE or runtime phase after
    EndOfDxe protocol is installed. <b>0: Disable</b>; 1: Enable.
  **/
  UINT32  BiosLock       :  1;
  UINT32  RsvdBits       : 28;
} LOCK_DOWN_HOB;

/**
  The PM_HOB block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.
**/
typedef struct {
  /**
    <b>(Test)</b>
    When set to true, this bit disallows host reads to PMC XRAM.
    BIOS must set this bit (to disable and lock the feature) prior to passing control to OS
    0:Disable, <b>1:Enable</b>
  **/
  UINT32                  PmcReadDisable               :  1;
  UINT32                  RsvdBits1                    : 31;
} PM_HOB;

/**
  This structure contains the HOB which are related to P2SB device.
**/
typedef struct {
  /**
    <b>(Test)</b>
    This unlock the SBI lock bit to allow SBI after post time. <b>0: Disable</b>; 1: Enable.
    NOTE: Do not set this policy "SbiUnlock" unless necessary.
  **/
  UINT32    SbiUnlock         :  1;
  /**
    <b>(Test)</b>
    The PSF registers will be locked before 3rd party code execution.
    This policy unlock the PSF space. <b>0: Disable</b>; 1: Enable.
    NOTE: Do not set this policy "SbAccessUnlock" unless necessary.
  **/
  UINT32    PsfUnlock        : 1; //@deprecated
  UINT32    SbAccessUnlock   : 1;
  UINT32    RsvdBits         : 29;
} P2SB_HOB;

/**
  This structure holds information about disabled USB2 and USB3 ports through Port Disable Override functionality
**/
typedef struct {
  UINT32  DelayPdoProgramming   :  1; ///< Flag informing about delaying PDO programming to DXE phase
  UINT32  Usb2DisabledPorts     : 16; ///< USB2 Disabled ports
  UINT32  Usb3DisabledPorts     : 10; ///< USB3 Disabled ports
  UINT32  RsvdBits              :  5;
} USB_HOB;


///
/// Pch Config Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE  EfiHobGuidType;     ///< GUID HOB type structure for gPchConfigHobGuid
  GENERAL_HOB        General;            ///< Pch general HOB definition
  INTERRUPT_HOB      Interrupt;          ///< Interrupt HOB definition
  SERIAL_IO_HOB      SerialIo;           ///< Serial io HOB definition
  PCIERP_HOB         PcieRp;             ///< PCIE root port HOB definition
  THERMAL_HOB        Thermal;            ///< Thermal HOB definition
  SCS_HOB            Scs;                ///< Scs HOB definition
  CIO2_HOB           Cio2;               ///< CIO2 HOB definition
  LOCK_DOWN_HOB      LockDown;           ///< Lock down HOB definition
  PM_HOB             Pm;                 ///< PM HOB definition
  P2SB_HOB           P2sb;               ///< P2sb HOB definition
  HDAUDIO_HOB        HdAudio;            ///< HD audio definition
  SATA_HOB           Sata;               ///< SATA definition
  PROTECTED_RANGE    ProtectRange[PCH_FLASH_PROTECTED_RANGES];
  PORT61H_SMM_HOB    Port61hSmm;
  SMBUS_HOB          Smbus;
  USB_HOB            Usb;

} PCH_CONFIG_HOB;
#pragma pack (pop)
#endif
