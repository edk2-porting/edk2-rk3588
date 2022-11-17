/** @file
  This file contains internal header for PSF lib usage

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PSF_PRIVATE_LIB_INTERNAL_H_
#define _PCH_PSF_PRIVATE_LIB_INTERNAL_H_

#include <Private/Library/PchPsfPrivateLib.h>
#include <Register/PchRegsPcr.h>

#define PSF_PORT_NULL ((PSF_PORT){0,0})
#define PSF_IS_PORT_NULL(PsfPort) ((PsfPort.PsfPid == 0) && (PsfPort.RegBase == 0))

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableBridge (
  IN PSF_PORT  PsfPort
  );

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level in RS3

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfRs3DisableBridge (
  IN PSF_PORT  PsfPort
  );

/**
  Check if bridge (e.g. PCIe Root Port) is enabled at PSF level

  @param[in] PsfPort  PSF PORT data structure

  @retval TRUE        Bridge behind PSF Port is enabled
          FALSE       Bridge behind PSF Port is disabled
**/
BOOLEAN
PsfIsBridgeEnabled (
  IN PSF_PORT  PsfPort
  );

/**
  Disable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfDisableDeviceIoSpace (
  IN PSF_PORT  PsfPort
  );

/**
  Enable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfEnableDeviceIoSpace (
  IN PSF_PORT  PsfPort
  );

/**
  Disable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfDisableDeviceMemSpace (
  IN PSF_PORT  PsfPort
  );

/**
  Enable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfEnableDeviceMemSpace (
  IN PSF_PORT  PsfPort
  );

/**
  Set device BARx address at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PsfSetDeviceBarValue (
  IN PSF_PORT  PsfPort,
  IN UINT8     BarNum,
  IN UINT32    BarValue
  );

/**
  Return PSF_PORT for TraceHub device

  @retval    PsfPort         PSF PORT structure for TraceHub device
**/
PSF_PORT
PsfTraceHubPort (
  VOID
  );

/**
  This procedure will return PSF_PORT for TraceHub ACPI device

  @retval    PsfPort         PSF PORT structure for TraceHub ACPI device
**/
PSF_PORT
PsfTraceHubAcpiDevPort (
  VOID
  );

/**
  Return PSF_PORT for HECI device

  @param[in] HeciDevice      HECIx Device (HECI1-4)

  @retval    PsfPort         PSF PORT structure for HECI device
**/
PSF_PORT
PsfHeciPort (
  IN UINT8      HeciDevice
  );

/**
  This procedure will return PSF_PORT for SOL device

  @retval    PsfPort         PSF PORT structure for SOL device
**/
PSF_PORT
PsfSolPort (
  VOID
  );

/**
  Return PSF_PORT for ISH device

  @retval    PsfPort         PSF PORT structure for ISH device
**/
PSF_PORT
PsfIshPort (
  VOID
  );

/**
  Return PSF_PORT for CNVi device

  @retval    PsfPort         PSF PORT structure for CNVi device
**/
PSF_PORT
PsfCnviPort (
  VOID
  );

/**
  Return PSF_PORT for PMC device

  @retval    PsfPort         PSF PORT structure for PMC device
**/
PSF_PORT
PsfPmcPort (
  VOID
  );

/**
  Return second level PSF_PORT to which PCIE Root Port device is connected (directly)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe
**/
PSF_PORT
PsfPcieSecondLevelPort (
  IN UINT32  RpIndex
  );

/**
  Return PSF_PORT at root PSF level to which PCIe Root Port device is connected

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe

**/
PSF_PORT
PsfRootPciePort (
  IN UINT32  RpIndex
  );

/**
  Return RS3 PSF_PORT at root PSF level to which PCIe Root Port device is connected

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe
**/
PSF_PORT
PsfRootRs3PciePort (
  IN UINT32  RpIndex
  );

/**
  Check if PCIe Root Port is enabled

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval TRUE              PCIe Root Port is enabled
          FALSE             PCIe Root Port is disabled
**/
BOOLEAN
PsfIsPcieRootPortEnabled (
  IN UINT32  RpIndex
  );

//
// Type of enpoint connected to PSF port.
// PsfNullPort is used for ports which do not exist
//
typedef enum {
  PsfNullPort,
  PsfToPsfPort,
  PsfPcieCtrlPort
} PSF_TOPO_PORT_TYPE;

//
// Structure for storing information on location in PSF topology
// Every PSF node is identified by PsfID and PsfPortId
//
typedef struct {
  UINT8         PsfId;
  UINT8         PortId;
} PSF_TOPO_PORT;

#define PSF_TOPO_PORT_NULL ((PSF_TOPO_PORT){0, 0})
#define PSF_IS_TOPO_PORT_NULL(PsfTopoPort) (((PsfTopoPort).PsfId == 0) && ((PsfTopoPort).PortId == 0))

//
// This is optional field containing PSF port specific data
//
typedef union {
  UINT32  PcieCtrlIndex;
} PSF_TOPO_PORT_DATA;

//
// Structure representing PSF port in PSF topology
// If port is of PsfToPsfPort type Child will point to the first
// port of sub PSF segment.
//
typedef struct PSF_TOPOLOGY {
  PSF_TOPO_PORT              PsfPort;
  PSF_TOPO_PORT_TYPE         PortType;
  CONST struct PSF_TOPOLOGY  *Child;
  PSF_TOPO_PORT_DATA         PortData;
} PSF_TOPOLOGY;

//
// Tag for identifying last element of PSF_TOPOLOGY type array
//
#define PSF_TOPOLOGY_END   {{0, 0}, PsfNullPort, NULL}

/**
  Get PSF Pcie Tree topology

  @param[in] PsfTopology          PSF Port from PSF PCIe tree topology

  @retval PsfTopology             PSF PCIe tree topology
**/
CONST PSF_TOPOLOGY*
PsfGetRootPciePsfTopology (
  VOID
  );

//
// Structure for storing data on PCIe controller to PSF assignment and GrantCount register offsets
//
typedef struct {
  PCH_SBI_PID  PsfPid;
  UINT16       DevGntCnt0Base;
  UINT16       TargetGntCntPg1Tgt0Base;
} PSF_GRANT_COUNT_REG;

/**
  Grant count regs data for PSF that is directly connected to PCIe Root Ports

  @param[in]  Controller     PCIe Root Port Controller index (0 based)
  @param[out] GrantCountReg  Structure with PSF Grant Count register data
**/
VOID
PsfPcieGrantCountBaseReg (
  IN  UINT8                Controller,
  OUT PSF_GRANT_COUNT_REG  *GrantCountReg
  );

/**
  Get Grant Count number (Device Grant Count and Target Grant Count)
  for PSF that is directly connected to PCIe Root Ports

  @param[in]  Controller    PCIe Root Port Controller index
  @param[in]  Channel       PCIe Root Port Channel index
  @param[out] DgcrNo        Device Grant Count number
  @param[out] PgTgtNo       Target Grant Count number
**/
VOID
PsfPcieGrantCountNumber (
  IN  UINT8 Controller,
  IN  UINT8 Channel,
  OUT UINT8 *DgcrNo,
  OUT UINT8 *PgTgtNo
  );

/**
  Grant count regs data for a given PSF-to-PSF port.

  @param[in] PsfTopoPort         PSF-to-PSF port

  @param[out] GrantCountReg      Structure with PSF Grant Count register data
**/
VOID
PsfSegmentGrantCountBaseReg (
  IN PSF_TOPO_PORT         PsfTopoPort,
  OUT PSF_GRANT_COUNT_REG  *GrantCountReg
  );

/**
  Grant Count number (Device Grant Count and Target Grant Count) for a given PSF-to-PSF port.

  @param[in] PsfTopoPort         PSF-to-PSF port
  @param[out] DgcrNo             Device Grant Count number
  @param[out] PgTgtNo            Target Grant Count number
**/
VOID
PsfSegmentGrantCountNumber (
  IN PSF_TOPO_PORT PsfTopoPort,
  OUT UINT8        *DgcrNo,
  OUT UINT8        *PgTgtNo
  );

//
// Do not override PSF Grant Count value and leave HW default setting
//
#define DEFAULT_PCIE_GRANT_COUNT 0xFF

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

/**
  Get PSF SideBand Port ID from PSF ID (1 - PSF1, 2 - PSF2, ...)

  @param[in] PsfId             PSF ID (1 - PSF1, 2 - PSF2, ...)

  @retval PSF SideBand Port ID
**/
PCH_SBI_PID
PsfSbPortId (
  UINT32        PsfId
  );

/**
  Get EOI register data for given PSF ID

  @param[in]  PsfId           PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[out] EoiTargetBase   EOI Target register
  @param[out] EoiControlBase  EOI Control register

  @retval MaxTargets          Number of supported targets

**/
UINT8
PsfEoiRegData (
  UINT32        PsfId,
  UINT16        *EoiTargetBase,
  UINT16        *EoiControlBase
  );

/**
  Get MCTP register data for given PSF ID

  @param[in]  PsfId            PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[out] MctpTargetBase   MCTP Target register
  @param[out] MctpControlBase  MCTP Control register

  @retval MaxTargets           Number of supported targets

**/
UINT8
PsfMctpRegData (
  UINT32        PsfId,
  UINT16        *MctpTargetBase,
  UINT16        *MctpControlBase
  );

/**
  P2SB PSF port Destination ID (psf_id:port_group_id:port_id:channel_id)

  @retval P2SB Destination ID
**/
PSF_PORT_DEST_ID
PsfP2sbDestinationId (
  VOID
  );

/**
  DMI PSF port Destination ID (psf_id:port_group_id:port_id:channel_id)

  @retval DMI Destination ID
**/
PSF_PORT_DEST_ID
PsfDmiDestinationId (
  VOID
  );

/**
  Check if MCTP is supported

  @retval TRUE              MCTP is supported
          FALSE             MCTP is not supported
**/
BOOLEAN
PsfIsMctpSupported (
  VOID
  );

/**
  Return the PSF (Root level) Function Config PSF_PORT for PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe Function Config
**/
PSF_PORT
PsfRootPcieFunctionConfigPort (
  IN UINT32  RpIndex
  );

/**
  Return the PSF (Root level) RS3 Function Config PSF_PORT for PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe Function Config
**/
PSF_PORT
PsfRootRs3PcieFunctionConfigPort (
  IN UINT32  RpIndex
  );

/**
  Return the PSF Function Config Second Level PSF_PORT for PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe Function Config
**/
PSF_PORT
PsfPcieFunctionConfigSecondLevelPort (
  IN UINT32  RpIndex
  );

#endif
