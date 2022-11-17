/** @file
  The GUID definition for PchConfigHob

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_CONFIG_HOB_H_
#define _PCH_CONFIG_HOB_H_

#include <ConfigBlock.h>
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
  ///
  ///
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
  UINT8                        PxRcConfig[PCH_MAX_PXRC_CONFIG];                 ///< PCI interrupt routing for 8259 PIC controller
} INTERRUPT_HOB;

///
/// The CNVI_HOB block describes CNVi device.
///
typedef struct {
  UINT32 Mode                      :  1;         ///< 0: Disabled, <b>1: Auto</b>
  UINT32 RsvdBits0                 : 31;
} CNVI_HOB;

/**
  The SERIAL_IO block provides the configurations to set the Serial IO controllers
**/
typedef struct {
  /**
       0: Disabled;
          - Device is placed in D3
          - Gpio configuration is skipped
          - Device will be disabled in PSF
          - !important! If given device is Function 0 and not all other LPSS functions on given device
                        are disabled, then PSF disabling is skipped.
                        PSF default will remain and device PCI CFG Space will still be visible.
                        This is needed to allow PCI enumerator access functions above 0 in a multifunction device.
    <b>1: Pci</b>;
          - Gpio pin configuration in native mode for each assigned pin
            RX/TX or RX/TX/CTS/RTS in case of UART depending UartHwFlowCtrl
          - Device will be enabled in PSF
          - Only Bar 0 will be enabled
       2: Acpi;
          - Gpio pin configuration in native mode for each assigned pin
            RX/TX or RX/TX/CTS/RTS in case of UART depending UartHwFlowCtrl
          - Device will be hidden in PSF and not available to PCI enumerator
          - Both BARs are enabled, BAR1 becomes devices Pci config Space
    @note Intel does not provide Windows SerialIo drivers for this mode
       3: Hidden;
          Designated for Kernel Debug and Legacy UART configuartion, might also be used for IO Expander on I2C
          - Device is placed in D0
          - Gpio pin configuration in native mode for each assigned pin
            RX/TX or RX/TX/CTS/RTS in case of UART depending UartHwFlowCtrl
          - Device will be hidden in PSF and not available to PCI enumerator
          - Both BARs are enabled, BAR1 becomes devices Pci config Space
          - !important! In this mode UART will work in 16550 Legacy 8BIT Mode, it's resources will be assigned to mother board through ACPI (PNP0C02)
    @note Considering the PcdSerialIoUartDebugEnable and PcdSerialIoUartNumber for all SerialIo UARTx,
          the PCD is more meaningful to represent the board design. It means, if PcdSerialIoUartDebugEnable is not 0,
          the board is designed to use the SerialIo UART for debug message and the PcdSerialIoUartNumber is dedicated
          to be Debug UART usage. Therefore, it should grayout the option from setup menu since no other options
          available for this UART controller on this board, and also override the policy default accordingly.
          While PcdSerialIoUartDebugEnable is 0, then it's allowed to configure the UART controller by policy.
  **/
  UINT8  DevMode[PCH_MAX_SERIALIO_CONTROLLERS];
  UINT32 DebugUartNumber           :  2;                    ///< UART number for debug purpose. 0:UART0, 1: UART1, <b>2:UART2</b>
  UINT32 EnableDebugUartAfterPost  :  1;                    ///< Enable debug UART controller after post. 0: disabled, <b>1: enabled</b>
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
  UINT32  DspEnable             :  1;    ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  UINT32  CodecSxWakeCapability :  1;    ///< Capability to detect wake initiated by a codec in Sx, <b>0: Disable</b>; 1: Enable
  UINT32  AudioLinkSndw1        :  1;    ///< SoundWire1 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with HDA
  UINT32  AudioLinkSndw2        :  1;    ///< SoundWire2 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with SSP1
  UINT32  AudioLinkSndw3        :  1;    ///< SoundWire3 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with DMIC1
  UINT32  AudioLinkSndw4        :  1;    ///< SoundWire4 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with DMIC0
  UINT32  RsvdBits0             : 26;    ///< Reserved bits
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
  UINT32  ScsSdPowerEnableActiveHigh          :  1;  ///< Sd PWREN# active high
  UINT32  ScsSdCardEnabled                    :  1;  ///< Sd card enabled
  UINT32  RsvdBits                            : 22;
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
    Enable RTC lower and upper 128 byte Lock bits to lock Bytes 38h-3Fh in the upper
    and lower 128-byte bank of RTC RAM. 0: Disable; <b>1: Enable</b>.
  **/
  UINT32  RtcMemoryLock   :  1;
  /**
    Enable the BIOS Lock Enable (BLE) feature and set EISS bit (D31:F5:RegDCh[5])
    for the BIOS region protection. When it is enabled, the BIOS Region can only be
    modified from SMM after EndOfDxe protocol is installed.
    Note: When BiosLock is enabled, platform code also needs to update to take care
    of BIOS modification (including SetVariable) in DXE or runtime phase after
    EndOfDxe protocol is installed.
    Enable InSMM.STS (EISS) in SPI
    If this EISS bit is set, then WPD must be a '1' and InSMM.STS must be '1' also
    in order to write to BIOS regions of SPI Flash. If this EISS bit is clear,
    then the InSMM.STS is a don't care.
    The BIOS must set the EISS bit while BIOS Guard support is enabled.
    In recovery path, platform can temporary disable EISS for SPI programming in
    PEI phase or early DXE phase.
    0: Disable; <b>1: Enable.</b>
  **/
  UINT32  BiosLock        :  1;
  UINT32  RsvdBits        : 28;
} LOCK_DOWN_HOB;

/**
  The PM_HOB block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.
**/
typedef struct {
  UINT32                  SlpS0VmRuntimeControl        :  1;     /// < SLP_S0 Voltage Margining Runtime Control. <b>0: Disable</b>; 1: Enable.
  UINT32                  SlpS0Vm070VSupport           :  1;     /// < SLP_S0 0.70V Voltage Margining Support. <b>0: Disable</b>; 1: Enable.
  UINT32                  SlpS0Vm075VSupport           :  1;     /// < SLP_S0 0.75V Voltage Margining Support. <b>0: Disable</b>; 1: Enable.
  UINT32                  PsOnEnable                   :  1;     /// < Indicates if PS_ON support has been enabled, <b>0: Disable</b>; 1: Enable.
  UINT32                  RsvdBits1                    : 28;
} PM_HOB;

/**
  PCH Trace Hub HOB settings.
**/
typedef struct {
  UINT32  PchTraceHubMode       :  2; // <b>0 = Disable</b>; 1 = Target Debugger mode; 2 = Host Debugger mode
  UINT32  Rsvd1                 : 30; // Reserved bytes
} PCH_TRACEHUB_HOB;

/**
  PCH eSPI HOB settings.
**/
typedef struct {
  UINT32  BmeMasterSlaveEnabled :  1; // <b>0 = BME disable</b>; 1 = BME enable
  UINT32  RsvdBits              : 31;
} PCH_ESPI_HOB;


///
/// Pch Config Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE  EfiHobGuidType;     ///< GUID HOB type structure for gPchConfigHobGuid
  GENERAL_HOB        General;            ///< Pch general HOB definition
  INTERRUPT_HOB      Interrupt;          ///< Interrupt HOB definition
  SERIAL_IO_HOB      SerialIo;           ///< Serial io HOB definition
  PCIERP_HOB         PcieRp;             ///< PCIE root port HOB definition
  SCS_HOB            Scs;                ///< Scs HOB definition
  CNVI_HOB           Cnvi;               ///< Cnvi Hob definition
  LOCK_DOWN_HOB      LockDown;           ///< Lock down HOB definition
  PM_HOB             Pm;                 ///< PM HOB definition
  HDAUDIO_HOB        HdAudio;            ///< HD audio definition
  SATA_HOB           Sata[PCH_MAX_SATA_CONTROLLERS]; ///< SATA definition
  PROTECTED_RANGE    ProtectRange[PCH_FLASH_PROTECTED_RANGES];
  SMBUS_HOB          Smbus;
  PCH_TRACEHUB_HOB   PchTraceHub;        ///< PCH Trace Hub definition
  PCH_ESPI_HOB       Espi;               ///< PCH eSPI definition

} PCH_CONFIG_HOB;
#pragma pack (pop)
#endif
