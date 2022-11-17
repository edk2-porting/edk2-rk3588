/** @file
  Print whole PCH_PREMEM_POLICY_PPI

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiPchPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/**
  Print PCH_GENERAL_PREMEM_CONFIG and serial out.

  @param[in] PchGeneralPreMemConfig     Pointer to a PCH_GENERAL_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintGeneralPreMemConfig (
  IN CONST PCH_GENERAL_PREMEM_CONFIG    *PchGeneralPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH General PreMem Config -----------\n"));
  DEBUG ((DEBUG_INFO, " AcpiBase= %x\n", PchGeneralPreMemConfig->AcpiBase));
  DEBUG ((DEBUG_INFO, " Port80Route= %x\n", PchGeneralPreMemConfig->Port80Route));
}

/**
  Print PCH_DCI_PREMEM_CONFIG and serial out.

  @param[in] DciPreMemConfig            Pointer to a PCH_DCI_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintDciPreMemConfig (
  IN CONST PCH_DCI_PREMEM_CONFIG        *DciPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH DCI PreMem Config ---------------\n"));
  DEBUG ((DEBUG_INFO, "DciEn= %x\n", DciPreMemConfig->DciEn));
}

/**
  Print PCH_WDT_PREMEM_CONFIG and serial out.

  @param[in] WdtPreMemConfig            Pointer to a PCH_WDT_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintWdtPreMemConfig (
  IN CONST PCH_WDT_PREMEM_CONFIG               *WdtPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH WDT PreMem Config ---------------\n"));
  DEBUG ((DEBUG_INFO, "DisableAndLock= %x\n", WdtPreMemConfig->DisableAndLock));
}

/**
  Print PCH_TRACE_HUB_CONFIG and serial out.

  @param[in] TraceHubConfig             Pointer to a PCH_TRACE_HUB_CONFIG that provides the platform setting

**/
VOID
PchPrintTraceHubPreMemConfig (
  IN CONST PCH_TRACE_HUB_PREMEM_CONFIG         *TraceHubPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH TraceHub PreMem Config ----------\n"));
  DEBUG ((DEBUG_INFO, "EnableMode= %x\n", TraceHubPreMemConfig->EnableMode));
  DEBUG ((DEBUG_INFO, "MemReg0Size= %x\n", TraceHubPreMemConfig->MemReg0Size));
  DEBUG ((DEBUG_INFO, "MemReg1Size= %x\n", TraceHubPreMemConfig->MemReg1Size));
}

/**
  Print PCH_HPET_PREMEM_CONFIG and serial out.

  @param[in] HpetPreMemConfig       Pointer to a PCH_HPET_CONFIG that provides the platform setting

**/
VOID
PchPrintHpetPreMemConfig (
  IN CONST PCH_HPET_PREMEM_CONFIG   *HpetPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH HPET PreMem Config --------------\n"));
  DEBUG ((DEBUG_INFO, " Enable %x\n", HpetPreMemConfig->Enable));
  DEBUG ((DEBUG_INFO, " BdfValid %x\n", HpetPreMemConfig->BdfValid));
  DEBUG ((DEBUG_INFO, " BusNumber %x\n",  HpetPreMemConfig->BusNumber));
  DEBUG ((DEBUG_INFO, " DeviceNumber %x\n", HpetPreMemConfig->DeviceNumber));
  DEBUG ((DEBUG_INFO, " FunctionNumber %x\n", HpetPreMemConfig->FunctionNumber));
  DEBUG ((DEBUG_INFO, " Base %x\n", HpetPreMemConfig->Base));
}

/**
  Print PCH_SMBUS_CONFIG and serial out.

  @param[in] SmbusConfig         Pointer to a PCH_SMBUS_CONFIG that provides the platform setting

**/
VOID
PchPrintSmbusPreMemConfig (
  IN CONST PCH_SMBUS_PREMEM_CONFIG    *SmbusPreMemConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH SMBUS PreMem Config -------------\n"));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", SmbusPreMemConfig->Enable));
  DEBUG ((DEBUG_INFO, " ArpEnable= %x\n", SmbusPreMemConfig->ArpEnable));
  DEBUG ((DEBUG_INFO, " DynamicPowerGating= %x\n", SmbusPreMemConfig->DynamicPowerGating));
  DEBUG ((DEBUG_INFO, " SpdWriteDisable= %x\n", SmbusPreMemConfig->SpdWriteDisable));
  DEBUG ((DEBUG_INFO, " SmbusIoBase= %x\n", SmbusPreMemConfig->SmbusIoBase));
  DEBUG ((DEBUG_INFO, " NumRsvdSmbusAddresses= %x\n", SmbusPreMemConfig->NumRsvdSmbusAddresses));
  DEBUG ((DEBUG_INFO, " RsvdSmbusAddressTable= {"));
  for (Index = 0; Index < SmbusPreMemConfig->NumRsvdSmbusAddresses; ++Index) {
    DEBUG ((DEBUG_INFO, " %02xh", SmbusPreMemConfig->RsvdSmbusAddressTable[Index]));
  }
  DEBUG ((DEBUG_INFO, " }\n"));
}

/**
  Print PCH_LPC_PREMEM_CONFIG and serial out.

  @param[in] LpcPreMemConfig                  Pointer to a PCH_LPC_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintLpcPreMemConfig (
  IN CONST PCH_LPC_PREMEM_CONFIG              *LpcPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH LPC PreMem Config ---------------\n"));
  DEBUG ((DEBUG_INFO, "EnhancePort8xhDecoding= %x\n", LpcPreMemConfig->EnhancePort8xhDecoding));
}

/**
  Print PCH_HSIO_PCIE_PREMEM_CONFIG and serial out.

  @param[in] HsioPciePreMemConfig     Pointer to a PCH_HSIO_PCIE_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintHsioPciePreMemConfig (
  IN CONST PCH_HSIO_PCIE_PREMEM_CONFIG *HsioPciePreMemConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ HSIO PCIE PreMem Config -------------\n"));
  DEBUG ((DEBUG_INFO, " PciePllSsc = %x\n", HsioPciePreMemConfig->PciePllSsc));
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioRxSetCtleEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioRxSetCtle= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen1DownscaleAmpEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen1DownscaleAmp= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmp));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen2DownscaleAmpEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen2DownscaleAmp= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmp));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen3DownscaleAmpEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen3DownscaleAmp= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmp));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen1DeEmphEnable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmphEnable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen1DeEmph= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmph));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen2DeEmph3p5Enable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5Enable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen2DeEmph3p5= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen2DeEmph6p0Enable= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0Enable));
    DEBUG ((DEBUG_INFO, " Rp[%d] HsioTxGen2DeEmph6p0= %x\n", Index, HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0));
  }
}

/**
  Print PCH_HSIO_SATA_PREMEM_CONFIG and serial out.

  @param[in] HsioSataPreMemConfig     Pointer to a PCH_HSIO_SATA_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintHsioSataPreMemConfig (
  IN CONST PCH_HSIO_SATA_PREMEM_CONFIG *HsioSataPreMemConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ HSIO SATA PreMem Config -------------\n"));
  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen1EqBoostMagEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMagEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen1EqBoostMag= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMag));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen2EqBoostMagEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMagEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen2EqBoostMag= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMag));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen3EqBoostMagEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMagEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen3EqBoostMag= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMag));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DownscaleAmpEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DownscaleAmp= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DownscaleAmpEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DownscaleAmp= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DownscaleAmpEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DownscaleAmp= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DeEmphEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DeEmph= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmph));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DeEmphEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DeEmph= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmph));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DeEmphEnable= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DeEmph= %x\n", Index, HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmph));
  }
}

/**
  Print PCH_HSIO_PREMEM_CONFIG and serial out.

  @param[in] HsioPreMemConfig     Pointer to a PCH_HSIO_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintHsioPreMemConfig (
  IN CONST PCH_HSIO_PREMEM_CONFIG *HsioPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ HSIO PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " ChipsetInitMessage : 0x%x\n", HsioPreMemConfig->ChipsetInitMessage));
  DEBUG ((DEBUG_INFO, " BypassPhySyncReset : 0x%x\n", HsioPreMemConfig->BypassPhySyncReset));
}

/**
  Print PCH_PCIE_RP_PREMEM_CONFIG and serial out.

  @param[in] PcieRpPreMemConfig        Pointer to a PCH_PCIE_RP_PREMEM_CONFIG that provides the platform setting

**/
VOID
PchPrintPcieRpPreMemConfig (
  IN CONST PCH_PCIE_RP_PREMEM_CONFIG    *PcieRpPreMemConfig
  )
{
  UINT32 Index;
  DEBUG ((DEBUG_INFO, "------------------ PCH PCIe RP PreMem Config -----------\n"));

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Port[%d] RpEnabled= %x\n", Index, (PcieRpPreMemConfig->RpEnabledMask & (UINT32) (1 << Index)) != 0 ));
  }
}


/**
  Print whole PCH_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance

**/
VOID
PchPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                      Status;
  PCH_GENERAL_PREMEM_CONFIG       *PchGeneralPreMemConfig;
  PCH_DCI_PREMEM_CONFIG           *DciPreMemConfig;
  PCH_WDT_PREMEM_CONFIG           *WdtPreMemConfig;
  PCH_TRACE_HUB_PREMEM_CONFIG     *TraceHubPreMemConfig;
  PCH_HPET_PREMEM_CONFIG          *HpetPreMemConfig;
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;
  PCH_LPC_PREMEM_CONFIG           *LpcPreMemConfig;
  PCH_HSIO_PCIE_PREMEM_CONFIG     *HsioPciePreMemConfig;
  PCH_HSIO_SATA_PREMEM_CONFIG     *HsioSataPreMemConfig;
  PCH_HSIO_PREMEM_CONFIG          *HsioPreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gWatchDogPreMemConfigGuid, (VOID *) &WdtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHpetPreMemConfigGuid, (VOID *) &HpetPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHsioSataPreMemConfigGuid, (VOID *) &HsioSataPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHsioPreMemConfigGuid, (VOID *) &HsioPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "------------------------ PCH Print PreMemPolicy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));

  PchPrintGeneralPreMemConfig (PchGeneralPreMemConfig);
  PchPrintDciPreMemConfig (DciPreMemConfig);
  PchPrintWdtPreMemConfig (WdtPreMemConfig);
  PchPrintTraceHubPreMemConfig (TraceHubPreMemConfig);
  PchPrintHpetPreMemConfig (HpetPreMemConfig);
  PchPrintSmbusPreMemConfig (SmbusPreMemConfig);
  PchPrintLpcPreMemConfig (LpcPreMemConfig);
  PchPrintHsioPciePreMemConfig (HsioPciePreMemConfig);
  PchPrintHsioSataPreMemConfig (HsioSataPreMemConfig);
  PchPrintHsioPreMemConfig (HsioPreMemConfig);
  PchPrintPcieRpPreMemConfig (PcieRpPreMemConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print PreMemPolicy End --------------------------\n"));
  DEBUG_CODE_END ();
}

