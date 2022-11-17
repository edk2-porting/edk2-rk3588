/** @file
  This file contains PSF routines for RC usage

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/SaPlatformLib.h>
#include <Private/Library/PchPsfPrivateLib.h>
#include <PchLimits.h>
#include <Register/PchRegsPsf.h>
#include <Register/PchRegsPcie.h>
#include "PchPsfPrivateLibInternal.h"

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
    ~0u,
    B_PCH_PSFX_PCR_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // Read back is needed to enforce the sideband and primary ordering.
  // If there is PCI access right after the PSF hide device, the device might
  // still be accessible since the PSF cycle is not completed yet, and causes
  // the race condition between sideband and primary cycles.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_CFG_DIS,
    ~0u,
    B_PCH_PSFX_PCR_T0_SHDW_CFG_DIS_CFGDIS
    );
}

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_CFG_DIS,
    (UINT32) ~(B_PCH_PSFX_PCR_T0_SHDW_CFG_DIS_CFGDIS),
    0
    );
}

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
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // BAR0-5 supported
  //
  ASSERT (BarDisMask < BIT6);

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
    ~0u,
    BarDisMask << N_PCH_PSFX_PCR_T0_SHDW_PCIEN_BARXDIS
    );
}

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
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // BAR0-5 supported
  //
  ASSERT (BarEnMask < BIT6);

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
    (UINT32)~(BarEnMask << N_PCH_PSFX_PCR_T0_SHDW_PCIEN_BARXDIS),
    0
    );
}

/**
  Disable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfDisableDeviceIoSpace (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
    ~(UINT32)(B_PCH_PSFX_PCR_T0_SHDW_PCIEN_IOEN),
    0
    );
}

/**
  Enable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfEnableDeviceIoSpace (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
    ~0u,
    B_PCH_PSFX_PCR_T0_SHDW_PCIEN_IOEN
    );
}

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
  )
{
  ASSERT (BarNum < 6);

  if (PSF_IS_PORT_NULL (PsfPort)) {
    ASSERT (FALSE);
    return;
  }

  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  PchPcrAndThenOr32WithReadback (
    PsfPort.PsfPid,
    PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_BAR0 + BarNum * 0x4,
    0,
    BarValue
    );
}

/**
  Hide PMC device at PSF level
**/
VOID
PsfHidePmcDevice (
  VOID
  )
{
  PsfHideDevice (PsfPmcPort ());
}

/**
  Set PMC ABASE value in PSF

  @param[in] Address     Address for ACPI base address.
**/
VOID
PsfSetPmcAbase (
  IN  UINT16       Address
  )
{
  PSF_PORT PsfPort;

  PsfPort = PsfPmcPort ();

  ASSERT (PchPcrRead32 (PsfPort.PsfPid, PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_BAR4) != 0xFFFFFFFF);

  //
  // Disable IOSpace before changing the address
  //
  PsfDisableDeviceIoSpace (PsfPort);

  //
  // Program ABASE in PSF PMC space BAR4
  //
  PsfSetDeviceBarValue (PsfPort, 4, Address);

  //
  // Enable IOSpace
  //
  PsfEnableDeviceIoSpace (PsfPort);
}

/**
  Get PMC ABASE value from PSF

  @retval Address     Address for ACPI base.
**/
UINT16
PsfGetPmcAbase (
  VOID
  )
{
  UINT16    Address;
  PSF_PORT  PsfPort;

  PsfPort = PsfPmcPort ();
  //
  // Read ABASE from PSF PMC space BAR4
  //
  Address = PchPcrRead16 (
              PsfPort.PsfPid,
              PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_BAR4
              );

  ASSERT (Address != 0xFFFF);

  return Address;
}

/**
  Get PMC PWRMBASE value from PSF

  @retval Address     Address for PWRM base.
**/
UINT32
PsfGetPmcPwrmBase (
  VOID
  )
{
  UINT32    Address;
  PSF_PORT  PsfPort;

  PsfPort = PsfPmcPort ();
  //
  // Read PWRMBASE from PSF PMC space BAR0
  //
  Address = PchPcrRead32 (
              PsfPort.PsfPid,
              PsfPort.RegBase + R_PCH_PSFX_PCR_T0_SHDW_BAR0
              );

  ASSERT (Address != 0xFFFFFFFF);

  return Address;
}

/**
  Get PSF SideBand Port ID from PSF ID (1 - PSF1, 2 - PSF2, ...)

  @param[in] PsfId             PSF ID (1 - PSF1, 2 - PSF2, ...)

  @retval PSF SideBand Port ID
**/
PCH_SBI_PID
PsfSbPortId (
  UINT32        PsfId
  )
{
  UINT32          PsfTableIndex;
  PSF_SEGMENT     *PsfTable;
  UINT32          PsfTableSize;

  PsfSegments (&PsfTable, &PsfTableSize);

  for (PsfTableIndex = 0; PsfTableIndex < PsfTableSize; PsfTableIndex++) {
    if (PsfTable[PsfTableIndex].Id == PsfId) {
      return PsfTable[PsfTableIndex].SbPid;
    }
  }

  ASSERT (FALSE);
  return 0;
}


/**
  Get PCH Root PSF ID. This is the PSF segment to which OPDMI/DMI is connected.

  @retval PsfId             Root PSF ID
**/
UINT32
PsfRootId (
  VOID
  )
{
  PSF_SEGMENT     *PsfTable;
  UINT32          PsfTableSize;

  PsfSegments (&PsfTable, &PsfTableSize);

  return PsfTable[0].Id;
}

/**
  Add EOI Target in a given PSF

  @param[in] PsfId             PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[in] TargetId          EOI Target ID
**/
STATIC
VOID
PsfAddEoiTarget (
  UINT32           PsfId,
  PSF_PORT_DEST_ID TargetId
  )
{
  UINT16      EoiTargetBase;
  UINT16      EoiControlBase;
  UINT8       NumOfEnabledTargets;
  UINT8       MaximalNumberOfTargets;
  PCH_SBI_PID PsfSbiPortId;
  UINT32      Data32;
  UINT8       TargetIndex;

  MaximalNumberOfTargets = PsfEoiRegData (PsfId, &EoiTargetBase, &EoiControlBase);
  PsfSbiPortId = PsfSbPortId (PsfId);

  //
  // Get number of enabled agents from PSF_x_PSF_MC_CONTROL_MCAST0_RS0_EOI register
  //
  Data32 = PchPcrRead32 (PsfSbiPortId, EoiControlBase);
  NumOfEnabledTargets = (UINT8) (Data32 >> N_PCH_PSFX_PCR_MC_CONTROL_MCASTX_NUMMC);

  //
  // Check if target was not already enabled
  // Targets from a different PSF segment are aggregated into single destination on
  // current PSF segment.
  //
  for (TargetIndex = 0; TargetIndex < NumOfEnabledTargets; TargetIndex++) {
    Data32 = PchPcrRead32 (PsfSbiPortId, EoiTargetBase + TargetIndex * 4);
    //
    // If target already added don't add it again
    //
    if (Data32 == TargetId.RegVal) {
      ASSERT (FALSE);
      return;
    }
    //
    // If target is from different PSF segment than currently being analyzed
    // it is enough that its PsfID is matching
    //
    if ((Data32 & B_PCH_PSFX_PCR_TARGET_PSFID) >> N_PCH_PSFX_PCR_TARGET_PSFID == TargetId.Fields.PsfId) {
      return;
    }
  }

  //
  // Check if next one can be added
  //
  if (NumOfEnabledTargets >= MaximalNumberOfTargets) {
    ASSERT (FALSE);
    return;
  }

  //
  // Add next target
  // Configure Multicast Destination ID register with target device on PSF.
  // Configuration must be done in next available PSF_MC_AGENT_MCAST0_RS0_TGT<x>_EOI register
  // so that other targets  are not overridden. <x> is known from the number of multicast agents
  // in Multicast Control Register. Value programmed is based on
  // PsfID, PortGroupID, PortID and ChannelID of the target
  //
  PchPcrWrite32 (PsfSbiPortId, EoiTargetBase + NumOfEnabledTargets * 4, TargetId.RegVal);

  //
  // Enable new target
  // Configure PSF_x_PSF_MC_CONTROL_MCAST0_RS0_EOI, increase NumMc and set MultCEn
  //
  NumOfEnabledTargets++;
  Data32 = (NumOfEnabledTargets << N_PCH_PSFX_PCR_MC_CONTROL_MCASTX_NUMMC) | B_PCH_PSFX_PCR_MC_CONTROL_MCASTX_MULTCEN;
  PchPcrWrite32 (PsfSbiPortId, EoiControlBase, Data32);
}

/**
  Enable EOI Target

  @param[in] TargetId  Target ID
**/
STATIC
VOID
PsfEnableEoiTarget (
  PSF_PORT_DEST_ID     TargetId
  )
{
  UINT32 RootLevelPsf;

  RootLevelPsf = PsfRootId ();

  //
  // Enable EOI target in root PSF
  //
  PsfAddEoiTarget (RootLevelPsf, TargetId);

  //
  // Enable EOI target on other PSF segment if target
  // is not located on root PSF
  //
  if (TargetId.Fields.PsfId != RootLevelPsf) {
    PsfAddEoiTarget (TargetId.Fields.PsfId, TargetId);
  }
}

/**
  This function enables EOI message forwarding in PSF for PCIe ports
  for cases where IOAPIC is present behind this root port.

  @param[in] RpIndex        Root port index (0 based)

  @retval Status
**/
EFI_STATUS
PsfConfigurEoiForPciePort (
  IN  UINT32  RpIndex
  )
{
  ASSERT (RpIndex < GetPchMaxPciePortNum ());

  //
  // If there is an IOAPIC discovered behind root port program PSF Multicast registers
  // accordingly to PCH BWG PSF EOI Multicast Configuration
  // Since there is a device behind RootPort to which EOI needs to be forwarded
  // enable multicast (MULTCEN) and increase the number of multicast agents (NUMMC)
  // in Multicast Control Register.
  //
  PsfEnableEoiTarget (PsfPcieDestinationId (RpIndex));

  return EFI_SUCCESS;
}
