/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_SECURITY_LIB_H_
#define _TBT_SECURITY_LIB_H_

#include <Protocol/Tcg2Protocol.h>
#include <Protocol/AcpiTable.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>
#include <SetupVariable.h>
#include <OemSetup.h>
#include <DmaRemappingTable.h>
#include <PcieRegs.h>
#include <Tcg2ConfigNvData.h>
#include <TbtPolicyCommonDefinition.h>
#include <Library/TbtCommonLib.h>

#define TBT_SECURITY_EVENT_STRING                 "DMA Protection Disabled"
#define TBT_SECURITY_EVENT_STRING_LEN             (sizeof (TBT_SECURITY_EVENT_STRING) - 1)

#define TBT_SECURITY_LEVEL_DOWNGRADED_STRING      "Security Level is Downgraded to 0"
#define TBT_SECURITY_LEVEL_DOWNGRADED_STRING_LEN  (sizeof (TBT_SECURITY_LEVEL_DOWNGRADED_STRING) - 1)

#define GET_TBT_SECURITY_MODE    0
#define SET_TBT_SECURITY_MODE    1

typedef struct {
  UINT8       EnableVtd;
  BOOLEAN     SLDowngrade;
} PCR7_DATA;

/**
  TBT Security ExtendPCR7 CallBackFunction
  If the firmware/BIOS has an option to enable and disable DMA protections via a VT-d switch in BIOS options, then the shipping configuration must be with VT-d protection enabled.
  On every boot where VT-d/DMA protection is disabled, or will be disabled, or configured to a lower security state, and a platform has a TPM enabled, then the platform SHALL extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA.
  The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement in the event log using the string "DMA Protection Disabled" for the Event Data.
  Measure and log launch of TBT Security, and extend the measurement result into a specific PCR.
  Extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA. The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement in the event log using the string "DMA Protection Disabled" for the Event Data.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
ExtendPCR7CallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  TBT Security DisableBme CallBackFunction

  BIOS will disable BME and tear down the Thunderbolt DMAR tables at ExitBootServices
  in order to hand off security of TBT hierarchies to the OS.
  The BIOS is expected to either: Disable BME from power on till the OS starts configuring the devices and enabling BME Enable BME only for devices that can be protected by VT-d in preboot environment,
  but disable BME and tear down any Thunderbolt DMAR tables at ExitBootServices()

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
TbtDisableBmeCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  TBT Security SetDmarOptIn CallBackFunction

  A new security feature will be supported to protect against Physical DMA attacks over Thunderbolt connects.
  In order to do this, they need a new flag added to the DMAR tables that a DMA is only permitted into RMRR at ExitBootServices().  With this flag available, OS can then Bug Check if any DMA is requested outside of the RMRR before OS supported device drivers are started.
  ReadyToBoot callback routine to update DMAR BIT2
  Bit definition: DMA_CONTROL_GUARANTEE
  If Set, the platform supports blocking all DMA outside of the regions defined in the RMRR structures from ExitBootServices() until OS supported device drivers are started.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SetDmarOptInCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


/**
  The function install DisableBme protocol for TBT Shell validation
**/
VOID
InstallDisableBmeProtocol (
  VOID
  );

/**
  Get or set Thunderbolt(TM) security mode

  @param[in]  DelayTime           - The delay time after do ForcePwr
  @param[in]  SecurityMode        - TBT Security Level
  @param[in]  Gpio3ForcePwrEn     - Force GPIO to power on or not
  @param[in]  DTbtController      - Enable/Disable DTbtController
  @param[in]  MaxControllerNumber - Number of contorller
  @param[in]  Action              - 0 = get, 1 = set

  @retval                         - Return security level
**/
UINT8
EFIAPI
GetSetSecurityMode (
  IN UINTN                       DelayTime,
  IN UINT8                       SecurityMode,
  IN UINT8                       Gpio3ForcePwrEn,
  IN UINT8                       *DTbtController,
  IN UINT8                       MaxControllerNumber,
  IN UINT8                       Action
);
#endif

