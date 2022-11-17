/** @file
  Header file for PchPsfPrivateLib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PSF_PRIVATE_LIB_H_
#define _PCH_PSF_PRIVATE_LIB_H_

#include <Library/PchPcrLib.h>
#include <Register/PchRegsPcr.h>

//
// Structure for storing data on both PSF SideBand Port ID and
// PSF port register offset for specific device
//
typedef struct {
  PCH_SBI_PID  PsfPid;
  UINT16       RegBase;
} PSF_PORT;

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Enable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfEnableDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Disable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarDisMask  BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfDisableDeviceBar (
  IN PSF_PORT  PsfPort,
  IN UINT32    BarDisMask
  );

/**
  Enable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarEnMask   BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfEnableDeviceBar (
  IN PSF_PORT  PsfPort,
  IN UINT32    BarEnMask
  );

/**
  Return PSF_PORT for SerialIO I2C device

  @param[in] I2cNum  Serial IO I2C device (I2C0, I2C1, ....)

  @retval  PsfPort   PSF PORT structure for SerialIO I2C device
**/
PSF_PORT
PsfSerialIoI2cPort (
  IN UINT32  I2cNum
  );

/**
  Return PSF_PORT for SerialIO SPI device

  @param[in] SpiNum  Serial IO SPI device (SPI0, SPI1, ....)

  @retval  PsfPort   PSF PORT structure for SerialIO SPI device
**/
PSF_PORT
PsfSerialIoSpiPort (
  IN UINT32  SpiNum
  );

/**
  Return PSF_PORT for SerialIO UART device

  @param[in] UartNum  Serial IO UART device (UART0, UART1, ....)

  @retval  PsfPort    PSF PORT structure for SerialIO UART device
**/
PSF_PORT
PsfSerialIoUartPort (
  IN UINT32  UartNum
  );

/**
  This procedure will set BARx value for TraceHub ACPI device at PSF level

  @param[in] BarNum          BAR Number (0:BAR0, 1:BAR1)
  @param[in] BarValue        32bit BAR value
**/
VOID
PsfSetTraceHubAcpiDeviceBarValue (
  IN UINT8   BarNum,
  IN UINT32  BarValue
  );

/**
  This procedure will enable MSE for TraceHub ACPI device at PSF level
**/
VOID
PsfEnableTraceHubAcpiDeviceMemorySpace (
  VOID
  );

/**
  Enable HECI device at PSF level

  @param[in] HeciDevice       HECIx Device (HECI1-4)
**/
VOID
PsfEnableHeciDevice (
  IN UINT8      HeciDevice
  );

/**
  Disable HECI device at PSF level

  @param[in] HeciDevice       HECIx Device (HECI1-4)
**/
VOID
PsfDisableHeciDevice (
  IN UINT8      HeciDevice
  );

/**
  Disable IDER device at PSF level
**/
VOID
PsfDisableIderDevice (
  VOID
  );

/**
  Enable SOL device at PSF level
**/
VOID
PsfEnableSolDevice (
  VOID
  );

/**
  Disable SOL device at PSF level
**/
VOID
PsfDisableSolDevice (
  VOID
  );

/**
  Set PMC ABASE value in PSF

  @param[in] Address     Address for ACPI base.
**/
VOID
PsfSetPmcAbase (
  IN  UINT16       Address
  );

/**
  Get PMC ABASE value from PSF

  @retval Address     Address for ACPI base.
**/
UINT16
PsfGetPmcAbase (
  VOID
  );

/**
  Get PMC PWRMBASE value from PSF

  @retval Address     Address for PWRM base.
**/
UINT32
PsfGetPmcPwrmBase (
  VOID
  );

/**
  Hide Cnvi WiFi device's PciCfgSpace at PSF level
**/
VOID
PsfHideCnviWifiDevice (
  VOID
  );

/**
  Disable Cnvi Wifi device at PSF level
**/
VOID
PsfDisableCnviWifiDevice (
  VOID
  );

/**
  Disable HDAudio device at PSF level
**/
VOID
PsfDisableHdaDevice (
  VOID
  );

/**
  Disable xDCI device at PSF level
**/
VOID
PsfDisableXdciDevice (
  VOID
  );

/**
  Disable xHCI device at PSF level
**/
VOID
PsfDisableXhciDevice (
  VOID
  );

/**
  Disable xHCI VTIO Phantom device at PSF level
**/
VOID
PsfDisableXhciVtioDevice (
  VOID
  );

/**
  Disable SATA device at PSF level

  @param[in]  SataCtrlIndex     SATA controller index
**/
VOID
PsfDisableSataDevice (
  IN UINT32     SataCtrlIndex
  );

/**
  Return PSF_PORT for SCS eMMC device

  @retval    PsfPort      PSF PORT structure for SCS eMMC device
**/
PSF_PORT
PsfScsEmmcPort (
  VOID
  );

/**
  Return PSF_PORT for SCS SD Card device

  @retval    PsfPort      PSF PORT structure for SCS SD Card device
**/
PSF_PORT
PsfScsSdCardPort (
  VOID
  );

/**
  Return PSF_PORT for SCS UFS device

  @param[in] UfsNum       UFS Device

  @retval    PsfPort      PSF PORT structure for SCS UFS device
**/
PSF_PORT
PsfScsUfsPort (
  IN UINT32  UfsNum
  );

/**
  Disable ISH device at PSF level
**/
VOID
PsfDisableIshDevice (
  VOID
  );

/**
  Disable ISH BAR1 at PSF level
**/
VOID
PsfDisableIshBar1 (
  VOID
  );

/**
  Disable GbE device at PSF level
**/
VOID
PsfDisableGbeDevice (
  VOID
  );

/**
  Disable SMBUS device at PSF level
**/
VOID
PsfDisableSmbusDevice (
  VOID
  );

/**
  Disable TraceHub ACPI devices at PSF level
**/
VOID
PsfDisableTraceHubAcpiDevice (
  VOID
  );

/**
  Hide TraceHub ACPI devices PciCfgSpace at PSF level
**/
VOID
PsfHideTraceHubAcpiDevice (
  VOID
  );

/**
  This procedure will hide TraceHub PciCfgSpace at PSF level
**/
VOID
PsfHideTraceHubDevice (
  VOID
  );

/**
  This procedure will unhide TraceHub PciCfgSpace at PSF level
**/
VOID
PsfUnhideTraceHubDevice (
  VOID
  );

/**
  This procedure will disable TraceHub device at PSF level
**/
VOID
PsfDisableTraceHubDevice (
  VOID
  );

/**
  Configures rootspace 3 bus number for PCIe IMR use

  @param[in] Rs3Bus        bus number
**/
VOID
PsfSetRs3Bus (
  UINT8 Rs3Bus
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PsfDisablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  Program PSF grant counts for SATA
  Call this before SATA ports are accessed for enumeration
**/
VOID
PsfConfigureSataGrantCounts (
  VOID
  );

typedef enum {
  PsfPcieCtrl4x1,
  PsfPcieCtrl1x2_2x1,
  PsfPcieCtrl2x2,
  PsfPcieCtrl1x4
} PSF_PCIE_CTRL_CONFIG;

/**
  Program PSF grant counts for PCI express depending on controllers configuration

  @param[in] PsfPcieCtrlConfigTable   Table with PCIe controllers configuration
  @param[in] NumberOfPcieControllers  Number of PCIe controllers. This is also the size of PsfPcieCtrlConfig table
**/
VOID
PsfConfigurePcieGrantCounts (
  IN PSF_PCIE_CTRL_CONFIG  *PsfPcieCtrlConfigTable,
  IN UINT32                NumberOfPcieControllers
  );

/**
  Program PSF EOI Multicast configuration for ITSS
**/
VOID
PsfConfigurEoiForItss (
  VOID
  );

/**
  This function enables EOI message forwarding in PSF for PCIe ports
  for cases where IOAPIC is present behind this root port.

  @param[in] RpIndex        Root port index (0 based)

  @retval Status
**/
EFI_STATUS
PsfConfigurEoiForPciePort (
  IN  UINT32   RpIndex
  );

//
// Structure for PSF Port Destination ID
//
typedef union {
  UINT32 RegVal;
  struct {
    UINT32  ChannelId   : 8;  // Channel ID
    UINT32  PortId      : 7;  // Port ID
    UINT32  PortGroupId : 1;  // Port Group ID
    UINT32  PsfId       : 8;  // PSF ID
    UINT32  Rsvd        : 7;  // Reserved
    UINT32  ChanMap     : 1;  // Channel map
  } Fields;
} PSF_PORT_DEST_ID;

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PsfPcieDestinationId (
  IN UINT32 RpIndex
  );

/**
  PSF early initialization.
**/
VOID
PsfEarlyInit (
  VOID
  );

/**
  Assign new function number for PCIe Port Number.

  @param[in] RpIndex        PCIe Root Port Index (0 based)
  @param[in] NewFunction    New Function number
**/
VOID
PsfSetPcieFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  );

/**
  This function enables PCIe Relaxed Order in PSF
**/
VOID
PsfEnablePcieRelaxedOrder (
  VOID
  );

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.
**/
VOID
PsfConfigurePowerManagement (
  VOID
  );

/**
  Enable VTd support in PSF.
**/
VOID
PchPsfEnableVtd (
  VOID
  );

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PchPsfDisableP2pDecoding (
  VOID
  );

/**
  Perform registers programming required for
  Management Component Transport Protocol Broadcast Cycle.

  Agent Destination Addresses are being programmed only when adequate
  PCIe root port controllers are function enabled.

  Function sets CSME PMT as a message broadcaster and programs the targets
  of the message in registers only if adequate PCIe root port controllers
  are function enabled. Conditionally, if the CPU PEG exist and is function
  enabled, DMI is also a target.
**/
VOID
PsfConfigureMctpCycle (
  VOID
  );

/**
  This procedure will hide PMC device at PSF level
**/
VOID
PsfHidePmcDevice (
  VOID
  );

/**
  This procedure will disable D3:F0 device at PSF level for PCH-LP
**/
VOID
PsfDisableD3F0 (
  VOID
  );

/**
  This procedure will disable PSF upstream completion tracking for HDAudio on PCH-LP
**/
VOID
PsfDisableUpstreamCompletionTrackingForHda (
  VOID
  );

#endif // _PCH_PSF_PRIVATE_LIB_H_
