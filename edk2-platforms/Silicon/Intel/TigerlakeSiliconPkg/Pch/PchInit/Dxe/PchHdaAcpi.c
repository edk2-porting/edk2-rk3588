
/** @file
  Initializes the PCH HD Audio ACPI Tables.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/AcpiTable.h>
#include <DxeHdaNhlt.h>
#include <Library/DxeHdaNhltLib.h>
#include "PchInit.h"
#include <HdAudioConfig.h>
#include <PchConfigHob.h>
#include <Library/PcdLib.h>
#include <Protocol/PchPolicy.h>
#include <HdAudioConfig.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>

#define XTAL_FREQ_38P4MHZ    1

/**
  Retrieves address of NHLT table from XSDT/RSDT.

  @retval NHLT_ACPI_TABLE*  Pointer to NHLT table if found
  @retval NULL              NHLT could not be found
**/
NHLT_ACPI_TABLE *
LocateNhltAcpiTable (
  VOID
  )
{
  EFI_ACPI_3_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;
  NHLT_ACPI_TABLE                               *Nhlt;
  UINTN                                         Index;
  UINT64                                        Data64;
  EFI_STATUS                                    Status;
  Rsdp  = NULL;
  Xsdt  = NULL;
  Nhlt  = NULL;

  ///
  /// Find the AcpiSupport protocol returns RSDP (or RSD PTR) address.
  ///
  DEBUG ((DEBUG_INFO, "LocateNhltAcpiTable() Start\n"));

  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID *) &Rsdp);
  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_ERROR, "EFI_ERROR or Rsdp == NULL\n"));
    return NULL;
  }

  Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->XsdtAddress;
  if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_5_0_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    // If XSDT has not been found, check RSDT
    Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->RsdtAddress;
    if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_5_0_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_ERROR, "XSDT/RSDT == NULL or wrong signature\n"));
      return NULL;
    }
  }

  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < Xsdt->Length; Index = Index + sizeof (UINT64)) {
    Data64  = *(UINT64 *) ((UINT8 *) Xsdt + Index);
    Nhlt    = (NHLT_ACPI_TABLE *) (UINTN) Data64;
    if (Nhlt != NULL && Nhlt->Header.Signature == NHLT_ACPI_TABLE_SIGNATURE) {
      break;
    }
    Nhlt = NULL;
  }

  if (Nhlt == NULL || Nhlt->Header.Signature != NHLT_ACPI_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Nhlt == NULL or wrong signature\n"));
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "Found NhltTable, Address = 0x%016x\n", Nhlt));

  return Nhlt;
}

/**
  Sets NVS ACPI variables for HDAS._DSM and SNDW._DSD accordingly to policy.

  @param[in]                NhltAcpiTableAddress
  @param[in]                NhltAcpiTableLength
  @param[in]                *HdAudioConfigHob
  @param[in]                *HdAudioDxeConfig
**/
VOID
UpdateHdaAcpiData (
  IN       UINT64                 NhltAcpiTableAddress,
  IN       UINT32                 NhltAcpiTableLength,
  IN CONST HDAUDIO_HOB            *HdAudioConfigHob,
  IN CONST HDAUDIO_DXE_CONFIG     *HdAudioDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "UpdateHdaAcpiData():\n NHLT Address = 0x%016x, Length = 0x%08x\n", NhltAcpiTableAddress, NhltAcpiTableLength));
  DEBUG ((DEBUG_INFO, " FeatureMask = 0x%08x\n", HdAudioDxeConfig->DspFeatureMask));

  mPchNvsAreaProtocol.Area->XTAL = XTAL_FREQ_38P4MHZ;
  mPchNvsAreaProtocol.Area->NHLA = NhltAcpiTableAddress;
  mPchNvsAreaProtocol.Area->NHLL = NhltAcpiTableLength;
  mPchNvsAreaProtocol.Area->ADFM = HdAudioDxeConfig->DspFeatureMask;

  if (HdAudioConfigHob->DspEnable || HdAudioConfigHob->DspUaaCompliance == FALSE) {
    mPchNvsAreaProtocol.Area->SWQ0 = HdAudioConfigHob->AudioLinkSndw1 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ1 = HdAudioConfigHob->AudioLinkSndw2 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ2 = HdAudioConfigHob->AudioLinkSndw3 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ3 = HdAudioConfigHob->AudioLinkSndw4 ? 0 : BIT1;
  } else {
    mPchNvsAreaProtocol.Area->SWQ0 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ1 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ2 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ3 = BIT1;
  }

  mPchNvsAreaProtocol.Area->SWMC = GetPchHdaMaxSndwLinkNum ();

  mPchNvsAreaProtocol.Area->ACS0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].AutonomousClockStop;

  mPchNvsAreaProtocol.Area->DAI0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnActiveIntervalSelect;

  mPchNvsAreaProtocol.Area->DOD0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnDelaySelect;
}

/**
  Initialize Intel High Definition Audio ACPI Tables

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_LOAD_ERROR          ACPI table cannot be installed
  @retval EFI_UNSUPPORTED         ACPI table not set because DSP is disabled
**/
EFI_STATUS
PchHdAudioAcpiInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT64                        HdaPciBase;
  CONST HDAUDIO_HOB             *HdAudioConfigHob;
  PCH_POLICY_PROTOCOL           *PchPolicy;
  HDAUDIO_DXE_CONFIG            *HdAudioDxeConfig;
  NHLT_ACPI_TABLE               *NhltTable;


  DEBUG ((DEBUG_INFO, "PchHdAudioAcpiInit() Start\n"));

  HdAudioConfigHob = &mPchConfigHob->HdAudio;

  ///
  /// Get PCH Policy Protocol
  ///
  Status = gBS->LocateProtocol (&gPchPolicyProtocolGuid, NULL, (VOID **)&PchPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Get HD Audio DXE Config Block
  ///
  Status = GetConfigBlock ((VOID *)PchPolicy, &gHdAudioDxeConfigGuid, (VOID *)&HdAudioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  HdaPciBase = HdaPciCfgBase ();

  if ((PciSegmentRead16 (HdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) || (HdAudioConfigHob->DspEnable == FALSE)) {
    // Do not set ACPI tables if HDAudio is Function disabled or DSP is disabled
    DEBUG ((DEBUG_INFO, "AudioDSP: Non-HDAudio ACPI Table (NHLT) not set!\n"));
    return EFI_UNSUPPORTED;
  }

  NhltTable = LocateNhltAcpiTable ();
  if (NhltTable == NULL) {
    return EFI_LOAD_ERROR;
  }

  UpdateHdaAcpiData ((UINT64) (UINTN) NhltTable, (UINT32) (NhltTable->Header.Length), HdAudioConfigHob, HdAudioDxeConfig);
  DEBUG_CODE ( NhltAcpiTableDump (NhltTable); );

  DEBUG ((DEBUG_INFO, "PchHdAudioAcpiInit() End - Status = %r\n", Status));
  return Status;
}

