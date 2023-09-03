## @file
#
#  Copyright (c) 2021-2022, Rockchip Limited. All rights reserved.
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  #
  # PI DXE Drivers producing Architectural Protocols (EFI Services)
  #
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  #
  # Non-volatile FVB support
  #
  # RkFvbDxe does not have a hard dependency on NorFlashDxe to provide
  # its services, but we want it to load earlier so the protocol can
  # get installed by the time RkFvbDxe tries to locate it on initialization.
  #
!if $(RK_NOR_FLASH_ENABLE) == TRUE
  INF Silicon/Rockchip/Drivers/NorFlashDxe/NorFlashDxe.inf
!endif
  INF Silicon/Rockchip/Drivers/RkFvbDxe/RkFvbDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  #
  # Multiple Console IO support
  #
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  INF MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  #
  # Arm GIC
  #
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  #
  # Timer
  #
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

  #
  # SCMI Support
  #
  INF ArmPkg/Drivers/ArmScmiDxe/ArmScmiDxe.inf

  #
  # ACPI Support
  #
  INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf

  #
  # DeviceTree Support
  #
  INF EmbeddedPkg/Drivers/DtPlatformDxe/DtPlatformDxe.inf

  #
  # SMBIOS Support
  #
  INF MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  INF Silicon/Rockchip/Drivers/PlatformSmbiosDxe/PlatformSmbiosDxe.inf

  #
  # I2C Bus & Devices
  #
  INF MdeModulePkg/Bus/I2c/I2cDxe/I2cDxe.inf
  INF Silicon/Rockchip/Drivers/I2c/I2cDxe/I2cDxe.inf
!if $(RK_860X_REGULATOR_ENABLE) == TRUE
  INF Silicon/Rockchip/Drivers/I2c/Rk860xRegulatorDxe/Rk860xRegulatorDxe.inf
!endif
!if $(RK_RTC8563_ENABLE) == TRUE
  INF Silicon/Rockchip/Drivers/Rtc8563PlatformDxe/Rtc8563PlatformDxe.inf
!endif
  # INF Silicon/Rockchip/Drivers/I2c/I2cDemoDxe/I2cDemoDxe.inf

  #
  # Display Support
  #
  INF Silicon/Rockchip/Drivers/Vop2Dxe/Vop2Dxe.inf
  # INF Silicon/Rockchip/Library/DisplayLib/AnalogixDpLib.inf
  INF Silicon/Rockchip/Library/DisplayLib/DwHdmiQpLib.inf
  INF Silicon/Rockchip/Library/DisplayLib/DwDpLib.inf
  INF Silicon/Rockchip/Drivers/LcdGraphicsOutputDxe/LcdGraphicsOutputDxe.inf

  #
  # USB Support
  #
  INF MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  INF Silicon/Rockchip/Drivers/OhciDxe/OhciDxe.inf

  # USB controllers installer
  INF Silicon/Rockchip/Drivers/UsbHcdInitDxe/UsbHcd.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf

  #
  # PCI Support
  #
  INF MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf

  #
  # SD/eMMC upper layer drivers
  #
!if $(RK_SD_ENABLE) == TRUE || $(RK_EMMC_ENABLE) == TRUE
  INF MdeModulePkg/Bus/Sd/SdDxe/SdDxe.inf
  INF MdeModulePkg/Bus/Sd/EmmcDxe/EmmcDxe.inf
!endif

  #
  # SD Support
  #
!if $(RK_SD_ENABLE) == TRUE
  INF Silicon/Synopsys/DesignWare/Drivers/DwMmcHcDxe/DwMmcHcDxe.inf
  INF Silicon/Rockchip/Drivers/RkSdmmcDxe/RkSdmmcDxe.inf
!endif

  #
  # eMMC Support
  #
!if $(RK_EMMC_ENABLE) == TRUE
  INF MdeModulePkg/Bus/Pci/SdMmcPciHcDxe/SdMmcPciHcDxe.inf
  INF Silicon/Rockchip/Drivers/DwcSdhciDxe/DwcSdhciDxe.inf
!endif

  #
  # AHCI Support
  #
!if $(RK_AHCI_ENABLE) == TRUE
  INF MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf
  INF MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  INF MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
!endif

  #
  # FAT filesystem + GPT/MBR partitioning
  #
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  #
  # UEFI Network Stack
  #
!include NetworkPkg/Network.fdf.inc

!if $(NETWORK_ENABLE) == TRUE
  #
  # Realtek PCIe UNDI Driver
  #
  INF Drivers/Realtek/Bus/Pcie/PcieNetworking/RtkUndiDxe.inf

  #
  # Realtek USB UNDI Driver
  #
  INF Drivers/Realtek/Bus/Usb/UsbNetworking/RtkUsbUndiDxe.inf

  #
  # AX88772 Ethernet Driver for Apple Ethernet Adapter
  #
  INF Drivers/ASIX/Bus/Usb/UsbNetworking/Ax88772c/Ax88772c.inf
!endif

  #
  # Bds
  #
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  INF MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Application/UiApp/UiApp.inf

  #
  # UEFI applications
  #
  INF ShellPkg/Application/Shell/Shell.inf
!ifdef $(INCLUDE_TFTP_COMMAND)
  INF ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf
!endif #$(INCLUDE_TFTP_COMMAND)

  # I2C Demo application
  # INF Silicon/Rockchip/Applications/I2cDemoTest/I2cDemoTest.inf

  # SPI Flash Cmd application
  # INF Silicon/Rockchip/Applications/SpiTool/SpiFlashCmd.inf
