/** @file
  Header file for PchPsfPrivateLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PSF_PRIVATE_LIB_H_
#define _PCH_PSF_PRIVATE_LIB_H_

#include <Library/PchPcrLib.h>
#include <Register/PchPcrRegs.h>

typedef struct {
  UINT32       Id;
  PCH_SBI_PID  SbPid;
} PSF_SEGMENT;

/**
  Get list of supported PSF segments.

  @param[out] PsfTable        Array of supported PSF segments
  @param[out] PsfTableLength  Length of PsfTable
**/
VOID
PsfSegments (
  OUT PSF_SEGMENT  **PsfTable,
  OUT UINT32       *PsfTableLength
  );

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
  Disable Dsp bar at PSF level
**/
VOID
PsfDisableDspBar (
  VOID
  );

/**
  Disable THC device at PSF level

  @param[in]  ThcNumber                Touch Host Controller Number THC0 or THC1
**/
VOID
PsfDisableThcDevice (
  IN  UINT32        ThcNumber
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
  Disable FPAK device at PSF level
**/
VOID
PsfDisableFpakDevice (
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

/**
  Specifies the root port configuration of the
  PCIe controller. The number on the left of x
  signifies the number of root ports in the controller
  while value on the right is link width. N stands for
  the number of PCIe lanes per root port instance.
**/
typedef enum {
  PsfPcieCtrl4xn,
  PsfPcieCtrl1x2n_2xn,
  PsfPcieCtrl2xn_1x2n,
  PsfPcieCtrl2x2n,
  PsfPcieCtrl1x4n,
  PsfPcieCtrlUndefined
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
