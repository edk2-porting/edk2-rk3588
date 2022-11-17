/** @file
  Platform Hook Library instances

@copyright
Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <PiPei.h>
#include <Platform.h>
#include <Guid/SetupVariable.h>
#include <Guid/SocketVariable.h>
#include <PchSetupVariable.h>
#include <Register/PchRegsPmc.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <Guid/GlobalVariable.h>
#include <Guid/PlatformInfo.h>
#include <Guid/FirmwareFileSystem3.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiPlatformHooklib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/BoardInitLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <SioRegs.h>

EFI_STATUS
UpdatePlatformInfo (
  IN   SYSTEM_CONFIGURATION                             *SystemConfiguration,
  IN   SOCKET_PROCESSORCORE_CONFIGURATION               *SocketProcessorCoreConfig,
  IN   SOCKET_IIO_CONFIGURATION                         *SocketIioConfig
  );

EFI_STATUS
EFIAPI
BoardDetect (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardDebugInit (
  VOID
  )
{

  return EFI_SUCCESS;
}

EFI_BOOT_MODE
EFIAPI
BoardBootModeDetect (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

VOID
EarlyPlatformPchInit (
  VOID
  )
{
  UINT16                          Data16;
  UINT8                           Data8;
  UINTN                           LpcBaseAddress;
  UINT8                           TcoRebootHappened;
  UINTN                           SpiBaseAddress;
  UINTN                           P2sbBase;
  EFI_STATUS                      Status = EFI_SUCCESS;
  DYNAMIC_SI_LIBARY_PPI           *DynamicSiLibraryPpi = NULL;

  DEBUG((DEBUG_INFO, "EarlyPlatformPchInit - Start\n"));

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  LpcBaseAddress = DynamicSiLibraryPpi->MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );
  SpiBaseAddress = DynamicSiLibraryPpi->MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI
                     );

  //
  // Program bar
  //
  P2sbBase = DynamicSiLibraryPpi->MmPciBase (
               DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_P2SB,
               PCI_FUNCTION_NUMBER_PCH_P2SB
               );

  MmioWrite32 (P2sbBase + R_P2SB_CFG_SBREG_BAR, PCH_PCR_BASE_ADDRESS);
  MmioOr8 (P2sbBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // LPC I/O Configuration
  //
  DynamicSiLibraryPpi->PchLpcIoDecodeRangesSet (
    (V_LPC_CFG_IOD_LPT_378  << N_LPC_CFG_IOD_LPT)  |
    (V_LPC_CFG_IOD_COMB_3E8 << N_LPC_CFG_IOD_COMB) |
    (V_LPC_CFG_IOD_COMA_3F8 << N_LPC_CFG_IOD_COMA)
    );

  DynamicSiLibraryPpi->PchLpcIoEnableDecodingSet (
    B_LPC_CFG_IOE_ME2  |
    B_LPC_CFG_IOE_SE   |
    B_LPC_CFG_IOE_ME1  |
    B_LPC_CFG_IOE_KE   |
    B_LPC_CFG_IOE_HGE  |
    B_LPC_CFG_IOE_LGE  |
    B_LPC_CFG_IOE_FDE  |
    B_LPC_CFG_IOE_PPE  |
    B_LPC_CFG_IOE_CBE  |
    B_LPC_CFG_IOE_CAE
    );
  //
  // Enable the upper 128-byte bank of RTC RAM
  //
  DynamicSiLibraryPpi->PchPcrAndThenOr32 (PID_RTC_HOST, R_RTC_PCR_CONF, (UINT32)~0, B_RTC_PCR_CONF_UCMOS_EN);

  //
  // Disable the Watchdog timer expiration from causing a system reset
  //
  DynamicSiLibraryPpi->PchPcrAndThenOr32 (PID_ITSS, R_ITSS_PCR_GIC, (UINT32)~0, B_ITSS_PCR_GIC_AME);
  //
  // Halt the TCO timer
  //
  Data16 = IoRead16 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO1_CNT);
  Data16 |= B_TCO_IO_TCO1_CNT_TMR_HLT;
  IoWrite16 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO1_CNT, Data16);

  //
  // Read the Second TO status bit
  //
  Data8 = IoRead8 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO2_STS);
  if ((Data8 & B_TCO_IO_TCO2_STS_SECOND_TO) == B_TCO_IO_TCO2_STS_SECOND_TO) {
    TcoRebootHappened = 1;
    DEBUG ((EFI_D_INFO, "EarlyPlatformPchInit - TCO Second TO status bit is set. This might be a TCO reboot\n"));
  } else {
    TcoRebootHappened = 0;
  }

  //
  // Clear the Second TO status bit
  //
  Data8 |= (UINT8) B_TCO_IO_TCO2_STS_SECOND_TO;
  Data8 &= (UINT8) ~B_TCO_IO_TCO2_STS_INTRD_DET;
  IoWrite8 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO2_STS, Data8);

  //
  // Disable SERR NMI and IOCHK# NMI in port 61
  //
  Data8 = IoRead8 (R_PCH_IO_NMI_SC);
  Data8 |= (B_PCH_IO_NMI_SC_PCI_SERR_EN | B_PCH_IO_NMI_SC_IOCHK_NMI_EN);
  IoWrite8 (R_PCH_IO_NMI_SC, Data8);

  DynamicSiLibraryPpi->PchPcrAndThenOr32 (PID_ITSS, R_ITSS_PCR_GIC, (UINT32)~B_ITSS_PCR_GIC_AME, 0);

  //
  // Clear EISS bit to allow for SPI use
  //
  MmioAnd8 (SpiBaseAddress + R_SPI_CFG_BC, (UINT8)~B_SPI_CFG_BC_EISS);

  //
  // Enable LPC decode at 0xCA0 for BMC
  //
  DynamicSiLibraryPpi->PchLpcGenIoRangeSet ((IPMI_DEFAULT_SMM_IO_BASE & 0xFF0), 0x10);
  DEBUG ((EFI_D_INFO, "[IPMI_DEBUG]: PchLpcGenIoRangeSet 0x%x!\n", IPMI_DEFAULT_SMM_IO_BASE));

  DEBUG((DEBUG_INFO, "EarlyPlatformPchInit - End\n"));
}

EFI_STATUS
EFIAPI
BoardInitBeforeMemoryInit (
  VOID
  )
{
  SYSTEM_CONFIGURATION                  SysSetupData;
  SOCKET_PROCESSORCORE_CONFIGURATION    SocketProcessorCoreSetupData;
  SOCKET_IIO_CONFIGURATION              SocketIioSetupData;
  PCH_SETUP                             PchSetupData;
  UINT16                                ABase;
  UINT16                                Pm1Sts = 0;
  UINT32                                Pm1Cnt;
  EFI_STATUS                            Status;
  DYNAMIC_SI_LIBARY_PPI                 *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Get Setup Data
  //

  ZeroMem (&SysSetupData, sizeof(SYSTEM_CONFIGURATION));
  ZeroMem (&SocketProcessorCoreSetupData, sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  ZeroMem (&SocketIioSetupData, sizeof(SOCKET_IIO_CONFIGURATION));
  ZeroMem (&PchSetupData, sizeof(PCH_SETUP));

  CopyMem (&PchSetupData, PcdGetPtr(PcdPchSetup), sizeof(PCH_SETUP));
  CopyMem (&SysSetupData, PcdGetPtr(PcdSetup), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&SocketProcessorCoreSetupData, PcdGetPtr(PcdSocketProcessorCoreConfig), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&SocketIioSetupData, PcdGetPtr(PcdSocketIioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
  //
  // Configure GPIO
  //
  Status = BoardInit ();

  EarlyPlatformPchInit ();

  ///
  /// Set what state (S0/S5) to go to when power is re-applied after a power failure (G3 state)
  ///
  DynamicSiLibraryPpi->PmcSetPlatformStateAfterPowerFailure (PchSetupData.StateAfterG3);

  //
  // Check PWR FLR
  //
  if (DynamicSiLibraryPpi->PmcIsPowerFailureDetected ()) {
    DynamicSiLibraryPpi->PmcClearPowerFailureStatus ();
  }
  ///----------------------------------------------------------------------------------
  ///
  /// Clear PWR_BTN_STS if set. BIOS should check the WAK_STS bit in PM1_STS[15] (PCH register ABASE+00h) before memory
  /// initialization to determine if ME has reset the system while the Host was in a sleep state.
  /// If WAK_STS is not set, BIOS should ensure a non-sleep exit path is taken by overwriting
  /// PM1_CNT[12:10] (PCH register ABASE+04h) to 111b to force an s5 exit.
  ///
  ABase = DynamicSiLibraryPpi->PmcGetAcpiBase ();

  Pm1Sts = IoRead16 (ABase + R_ACPI_IO_PM1_STS);
  if ((Pm1Sts & B_ACPI_IO_PM1_STS_PWRBTN) == B_ACPI_IO_PM1_STS_PWRBTN) {
    IoWrite16 (ABase + R_ACPI_IO_PM1_STS, B_ACPI_IO_PM1_STS_PWRBTN);
  }

  if ((Pm1Sts & B_ACPI_IO_PM1_STS_WAK) == 0) {
    Pm1Cnt = IoRead32 (ABase + R_ACPI_IO_PM1_CNT);
    Pm1Cnt |= V_ACPI_IO_PM1_CNT_S5;
    IoWrite32 (ABase + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }

  //
  // Update Platform Info
  //
  UpdatePlatformInfo (&SysSetupData,&SocketProcessorCoreSetupData,&SocketIioSetupData);


  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardInitAfterMemoryInit (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_BOOT_MODE               BootMode;
  UINT16                      Pm1Cnt;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Check if user wants to turn off in PEI phase
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    DynamicSiLibraryPpi->CheckPowerOffNow ();
  } else {
    Pm1Cnt  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_CNT);
    Pm1Cnt &= ~B_ACPI_IO_PM1_CNT_SLP_TYP;
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardInitBeforeTempRamExit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardInitAfterTempRamExit (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**

  Initialize POC register by Variable.

  @param *SystemConfiguration  -  Pointer to SystemConfiguration variables.

  @retval EFI_SUCCESS  -  Success.

**/
EFI_STATUS
UpdatePlatformInfo (
  IN   SYSTEM_CONFIGURATION                             *SystemConfiguration,
  IN   SOCKET_PROCESSORCORE_CONFIGURATION               *SocketProcessorCoreConfig,
  IN   SOCKET_IIO_CONFIGURATION                         *SocketIioConfig
  )
{
  EFI_PLATFORM_INFO *PlatformInfo;
  EFI_HOB_GUID_TYPE *GuidHob;
  //
  // Update the PCIE base and 32/64bit PCI resource support
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  PlatformInfo->SysData.SysIoApicEnable       = PCH_IOAPIC;
#if MAX_SOCKET <= 4
  if (SocketIioConfig->DevPresIoApicIio[0]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC00_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[1]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC01_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[2]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC02_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[3]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC03_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[4]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC04_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[5]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC05_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[6]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC06_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[7]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC07_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[8]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC08_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[9]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC09_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[10]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC10_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[11]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC11_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[12]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC12_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[13]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC13_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[14]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC14_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[15]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC15_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[16]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC16_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[17]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC17_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[18]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC18_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[19]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC19_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[20]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC20_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[21]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC21_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[22]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC22_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[23]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC23_IOAPIC);
  }
#else
  // Enable all 32 IOxAPIC
  PlatformInfo->SysData.SysIoApicEnable = 0xFFFFFFFF;
#endif
  //
  // Check to make sure TsegSize is in range, if not use default.
  //
  if (SocketProcessorCoreConfig->TsegSize > MAX_PROCESSOR_TSEG) {
    SocketProcessorCoreConfig->TsegSize = MAX_PROCESSOR_TSEG; // if out of range make default 64M
  }
  PlatformInfo->MemData.MemTsegSize  = (0x400000 << SocketProcessorCoreConfig->TsegSize);
  PlatformInfo->MemData.MemIedSize = PcdGet32 (PcdCpuIEDRamSize);

  //
  // Minimum SMM range in TSEG should be larger than 3M
  //
  ASSERT (PlatformInfo->MemData.MemTsegSize - PlatformInfo->MemData.MemIedSize >= 0x300000);

  return EFI_SUCCESS;
}