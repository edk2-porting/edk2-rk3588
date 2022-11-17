/** @file
  The GUID definition for PchConfigHob

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_CONFIG_HOB_H_
#define _PCH_CONFIG_HOB_H_

#include <ConfigBlock.h>
#include <SmbusConfig.h>
#include <InterruptConfig.h>
#include <PchPcieRpConfig.h>
#include <SataConfig.h>
#include <RstConfig.h>
#include <ConfigBlock/FlashProtectionConfig.h>
#include <FivrConfig.h>
#include <ThcConfig.h>
#include <SerialIoConfig.h>


extern EFI_GUID gPchConfigHobGuid;

#pragma pack (push,1)

/**
  This structure contains the HOB which are related to PCH general config.
**/
typedef struct {
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32 Crid      :  1;
  UINT32 RsvdBits0 : 31; ///< Reserved bits
} GENERAL_HOB;

/**
  The SMBUS_CONFIG block lists the reserved addresses for non-ARP capable devices in the platform.
**/
typedef struct {
  UINT8 RsvdBytes[3];          ///< Reserved bytes
  UINT8 NumRsvdSmbusAddresses; ///< The number of elements in the RsvdSmbusAddressTable.
  /**
    Array of addresses reserved for non-ARP-capable SMBus devices.
  **/
  UINT8 RsvdSmbusAddressTable[PCH_MAX_SMBUS_RESERVED_ADDRESS];
} SMBUS_HOB;

/**
  The INTERRUPT_HOB describes interrupt settings for PCH.
**/
typedef struct {
  UINT8                        NumOfDevIntConfig;                             ///< Number of entries in DevIntConfig table
  UINT8                        GpioIrqRoute;                                  ///< Interrupt routing for GPIO. Default is <b>14</b>.
  UINT8                        Rsvd0[2];                                      ///< Reserved bytes, align to multiple 4.
  PCH_DEVICE_INTERRUPT_CONFIG  DevIntConfig[PCH_MAX_DEVICE_INTERRUPT_CONFIG]; ///< Array which stores PCH devices interrupts settings
} INTERRUPT_HOB;

/**
  The SERIAL_IO block provides the configurations to set the Serial IO controllers
**/
typedef struct {
  SERIAL_IO_SPI_CONFIG  SpiDeviceConfig[PCH_MAX_SERIALIO_SPI_CONTROLLERS];   ///< SPI Configuration
  SERIAL_IO_I2C_CONFIG  I2cDeviceConfig[PCH_MAX_SERIALIO_I2C_CONTROLLERS];   ///< I2C Configuration
  SERIAL_IO_UART_CONFIG UartDeviceConfig[PCH_MAX_SERIALIO_UART_CONTROLLERS]; ///< UART Configuration
} SERIAL_IO_HOB;

/**
  The PCIERP_HOB block describes the expected configuration of the PCH PCI Express controllers
**/
typedef struct {
  ///
  /// These members describe the configuration of each PCH PCIe root port.
  ///
  PCH_PCIE_ROOT_PORT_CONFIG    RootPort[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_PCIE_CLOCK               PcieClock[PCH_MAX_PCIE_CLOCKS];
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
  UINT32  AllowNoLtrIccPllShutdown :  1;
  UINT32  RsvdBits0                : 31; ///< Reserved bits
} PCIERP_HOB;

/**
  The HDAUDIO_HOB block describes the configuration of the PCH cAVS controller
**/
typedef struct {
  UINT32  DspEnable             :  1; ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  UINT32  DspUaaCompliance      :  1; ///< UAA-mode Select: <b>0: Non-Uaa</b>; 1: UAA
  UINT32  CodecSxWakeCapability :  1; ///< Capability to detect wake initiated by a codec in Sx, <b>0: Disable</b>; 1: Enable
  UINT32  AudioLinkSndw1        :  1; ///< SoundWire1 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with HDA
  UINT32  AudioLinkSndw2        :  1; ///< SoundWire2 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with SSP1
  UINT32  AudioLinkSndw3        :  1; ///< SoundWire3 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with DMIC1
  UINT32  AudioLinkSndw4        :  1; ///< SoundWire4 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with DMIC0
  UINT32  Pme                   :  1; ///< Azalia wake-on-ring, <b>0: Disable</b>; 1: Enable
  UINT32  RsvdBits0             : 24; ///< Reserved bits
} HDAUDIO_HOB;

/**
  The SATA_HOB block describes the configuration of the PCH SATA controllers
**/
typedef struct {
  /**
    This member describes whether or not the SATA controllers should be enabled. 0: Disable; <b>1: Enable</b>.
  **/
  UINT32                        Enable          :  1;
  UINT32                        TestMode        :  1; ///< <b>(Test)</b> <b>0: Disable</b>; 1: Allow entrance to the PCH SATA test modes
  UINT32                        RsvdBits0       : 30; ///< Reserved bits
  /**
    This member configures the features, property, and capability for each SATA port.
  **/
  PCH_SATA_PORT_CONFIG          PortSettings[PCH_MAX_SATA_PORTS];
} SATA_HOB;

/**
  The RST block describes the configuration of the RST PCIE Cycle Routers
**/
typedef struct {
  /**
    This member describes the details of implementation of Intel RST for PCIe Storage remapping (Intel RST Driver is required)
  **/
  RST_HARDWARE_REMAPPED_STORAGE_CONFIG   RstHardwareRemappedStorageConfig[PCH_MAX_RST_PCIE_STORAGE_CR];
} RST_HOB;

typedef struct {
  UINT32  Enabled  : 1; ///< Indicates that SD card has been enabled
  UINT32  Reserved : 31;
} SD_CARD_HOB;

/**
  The EMMC_HOB block describes integrated eMMC settings for PCH.
**/
typedef struct {
  UINT32  Enabled                      :  1;  ///< Determine if eMMC is enabled - 0: Disabled, <b>1: Enabled</b>.
  UINT32  Hs400Enabled                 :  1;  ///< Determine eMMC HS400 Mode if EmmcEnabled - <b>0: Disabled</b>, 1: Enabled
  /**
    Determine if HS400 Training is required, set to FALSE if Hs400 Data is valid. <b>0: Disabled</b>, 1: Enabled.
    First Boot or CMOS clear, system boot with Default settings, set tuning required.
    Subsequent Boots, Get Variable 'Hs400TuningData'
      - if failed to get variable, set tuning required
      - if passed, retrieve Hs400DataValid, Hs400RxStrobe1Dll and Hs400TxDataDll from variable. Set tuning not required.
  **/
  UINT32  Hs400DllDataValid            :  1;  ///< Set if HS400 Tuning Data Valid
  UINT32  RsvdBits                     : 29;
} EMMC_HOB;

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
  UINT32  RsvdBits        : 29;
} LOCK_DOWN_HOB;

/**
  The PM_HOB block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.
**/
typedef struct {
  UINT32 PsOnEnable             :  1; ///< Indicates if PS_ON support has been enabled, <b>0: Disable</b>; 1: Enable.
  UINT32 EnableTimedGpio0       :  1; ///< Enable Bit for Timed GPIO 0 <b>0 = disable</b>; 1 = enable
  UINT32 EnableTimedGpio1       :  1; ///< Enable Bit for Timed GPIO 1 <b>0 = disable</b>; 1 = enable
  UINT32 RsvdBits1              : 29;
} PM_HOB;

/**
  FIVR_HOB block
**/
typedef struct {
  /**
    Additional External Vnn VR rail configuration dedicated for Sx.
    Required only if External Vnn VR needs different settings for Sx than
    those specified in ExtVnnRail (refer to PCH_FIVR_CONFIG.ExtVnnRail)
  **/
  FIVR_EXT_RAIL_CONFIG     ExtVnnRailSx;
} FIVR_HOB;

/**
  PCH Trace Hub HOB settings.
**/
typedef struct {
  UINT32  PchTraceHubMode       :  2; ///< <b>0 = Disable</b>; 1 = Target Debugger mode; 2 = Host Debugger mode
  UINT32  Rsvd1                 : 30; ///< Reserved bits
  /**
    Trace hub memory buffer region size policy.
    The avaliable memory size options are: 0:0MB (none), 1:1MB, <b>2:8MB</b>, 3:64MB, 4:128MB, 5:256MB, 6:512MB.
    Note : Limitation of total buffer size (CPU + PCH) is 512MB. If iTbt is enabled, the total size limits to 256 MB.
    Refer to TRACE_BUFFER_SIZE
  **/
  UINT32  MemReg0Size;
  UINT32  MemReg1Size;
} PCH_TRACEHUB_HOB;

/**
  PCH eSPI HOB settings.
**/
typedef struct {
  UINT32  BmeMasterSlaveEnabled :  1; ///< <b>0 = BME disable</b>; 1 = BME enable
  UINT32  RsvdBits              : 31; ///< Reserved bits
} PCH_ESPI_HOB;

/**
  THC HOB settings.
**/
typedef struct {
  THC_PORT  ThcPort[2]; ///< Port Configuration
} THC_HOB;


/**
  This structure contains the HOBs which are related to PCH controllers
**/
typedef struct {
  EFI_HOB_GUID_TYPE  EfiHobGuidType;     ///< GUID HOB type structure for gPchConfigHobGuid
  GENERAL_HOB        General;            ///< Pch general HOB definition
  INTERRUPT_HOB      Interrupt;          ///< Interrupt HOB definition
  SERIAL_IO_HOB      SerialIo;           ///< Serial io HOB definition
  PCIERP_HOB         PcieRp;             ///< PCIE root port HOB definition
  SD_CARD_HOB        SdCard;             ///< SD card HOB definition
  EMMC_HOB           Emmc;               ///< eMMC HOB definition
  LOCK_DOWN_HOB      LockDown;           ///< Lock down HOB definition
  PM_HOB             Pm;                 ///< PM HOB definition
  HDAUDIO_HOB        HdAudio;            ///< HD audio definition
  SATA_HOB           Sata[PCH_MAX_SATA_CONTROLLERS]; ///< SATA definition
  RST_HOB            Rst;                ///< RST definition
  PROTECTED_RANGE    ProtectRange[PCH_FLASH_PROTECTED_RANGES];
  SMBUS_HOB          Smbus;
  PCH_TRACEHUB_HOB   PchTraceHub;        ///< PCH Trace Hub definition
  PCH_ESPI_HOB       Espi;               ///< PCH eSPI definition
  THC_HOB            Thc;                ///< PCH Tocuh Host Controller definition
  FIVR_HOB           Fivr;               ///< PCH FIVR HOB definition


} PCH_CONFIG_HOB;
#pragma pack (pop)
#endif
