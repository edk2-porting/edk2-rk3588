/** @file

Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/BoardInitLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include "PeiMtOlympusInitLib.h"

#include <Ppi/BootInRecoveryMode.h>
#include <Ppi/PchPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/Reset.h>
#include <Ppi/Smbus2.h>
#include <Platform.h>
#include <GpioPinsSklH.h>
#include <Library/GpioLib.h>

#include <Guid/SetupVariable.h>
#include <Guid/PchRcVariable.h>
#include <Guid/MemoryTypeInformation.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/PrintLib.h>
#include <SetupTable.h>
#include <Library/PchCycleDecodingLib.h>

#include <Platform.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>
#include <PchAccess.h>
#include <Library/MmPciBaseLib.h>
#include <IndustryStandard/Pci22.h>

#include <Protocol/IioUds.h>

#include <Ppi/MasterBootMode.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/SiliconPolicyInitLib.h>
#include <Library/SiliconPolicyUpdateLib.h>
#include <Library/GpioLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/TimerLib.h>
#include <Register/PchRegsPmc.h>
#include <Library/PchCycleDecodingLib.h>
#include <Register/PchRegsLpc.h>
#include <Guid/FirmwareFileSystem3.h>
#include <Guid/SetupVariable.h>
#include <Guid/PchRcVariable.h>
#include <Guid/SocketVariable.h>
#include <Library/PchPcrLib.h>
#include <IioBifurcationSlotTable.h>
#include <KtiHost.h>

#include "SioRegs.h"

#define LEGACY_8259_MASK_REGISTER_MASTER    0x21
#define LEGACY_8259_MASK_REGISTER_SLAVE     0xA1

extern GPIO_INIT_CONFIG mGpioTableMicrosoftWcs[];
extern UINTN mGpioTableSizeMicrosoftWcs;

extern PCH_USB_OVERCURRENT_PIN Usb20OverCurrentMappings[PCH_MAX_USB2_PORTS];
extern PCH_USB_OVERCURRENT_PIN Usb30OverCurrentMappings[PCH_MAX_USB3_PORTS];

extern IIO_BIFURCATION_ENTRY   mIioBifurcationTable[];
extern UINT8 mIioBifurcationTableEntries;
extern IIO_SLOT_CONFIG_ENTRY   mIioSlotTable[];
extern UINT8 mIioSlotTableEntries;
extern ALL_LANES_EPARAM_LINK_INFO  KtiMtOlympusAllLanesEparamTable[];
extern UINT32 KtiMtOlympusAllLanesEparamTableSize;

/**

  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers.

  @param PeiServices  -  PeiService point.
  @param CpuIo        -  CpuIo PPI to read/write IO ports.

  @retval EFI_SUCCESS  -  Init succeed.

**/
VOID
LpcSioEarlyInit (
  VOID
  )
{
    PchLpcGenIoRangeSet ((0x600  & 0xFF0), 0x10, LPC_ESPI_FIRST_SLAVE);

    IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);
    IoWrite8 (SIO_INDEX_PORT, SIO_UNLOCK);

    //
    //mailbox
    //
    IoWrite8 (SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
    IoWrite8 (SIO_DATA_PORT, SIO_MAILBOX);

    IoWrite8 (SIO_INDEX_PORT, BASE_ADDRESS_HIGH0);
    IoWrite8 (SIO_DATA_PORT, (UINT8)(0x600 >> 8));

    IoWrite8 (SIO_INDEX_PORT, BASE_ADDRESS_LOW0);
    IoWrite8 (SIO_DATA_PORT, (UINT8)(0x600 & 0xFF));
    //
    //active mailbox
    //
    IoWrite8 (SIO_INDEX_PORT, ACTIVATE);
    IoWrite8 (SIO_DATA_PORT, 1);

    IoWrite8 (SIO_INDEX_PORT, SIO_LOCK);
}


VOID
EarlyPlatformPchInit (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN PCH_RC_CONFIGURATION        *PchRcConfiguration
  )
{
  UINT16                          Data16;
  UINT8                           Data8;
  UINT8                           TcoRebootHappened;
  UINTN                           SpiBaseAddress;
  UINTN                           P2sbBase;

  DEBUG((DEBUG_ERROR, "EarlyPlatformPchInit - Start\n"));

  SpiBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI
                     );

  //
  // Program bar
  //
  P2sbBase = MmPciBase (
               DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_P2SB,
               PCI_FUNCTION_NUMBER_PCH_P2SB
               );

  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBREG_BAR, PCH_PCR_BASE_ADDRESS);
  MmioOr8 (P2sbBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // LPC I/O Configuration
  //
  PchLpcIoDecodeRangesSet (
    (V_PCH_LPC_IOD_LPT_378  << N_PCH_LPC_IOD_LPT)  |
    (V_PCH_LPC_IOD_COMB_3E8 << N_PCH_LPC_IOD_COMB) |
    (V_PCH_LPC_IOD_COMA_3F8 << N_PCH_LPC_IOD_COMA)
    );

  PchLpcIoEnableDecodingSet (
    B_PCH_LPC_IOE_ME2  |
    B_PCH_LPC_IOE_SE   |
    B_PCH_LPC_IOE_ME1  |
    B_PCH_LPC_IOE_KE   |
    B_PCH_LPC_IOE_HGE  |
    B_PCH_LPC_IOE_LGE  |
    B_PCH_LPC_IOE_FDE  |
    B_PCH_LPC_IOE_PPE  |
    B_PCH_LPC_IOE_CBE  |
    B_PCH_LPC_IOE_CAE,
    LPC_ESPI_FIRST_SLAVE
    );

  //
  // Enable the upper 128-byte bank of RTC RAM
  //
  PchPcrAndThenOr32 (PID_RTC, R_PCH_PCR_RTC_CONF, (UINT32)~0, B_PCH_PCR_RTC_CONF_UCMOS_EN);

  //
  // Disable the Watchdog timer expiration from causing a system reset
  //
  PchPcrAndThenOr32 (PID_ITSS, R_PCH_PCR_ITSS_GIC, (UINT32)~0, B_PCH_PCR_ITSS_GIC_AME);

  //
  // Halt the TCO timer
  //
  Data16 = IoRead16 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO1_CNT);
  Data16 |= B_PCH_TCO_CNT_TMR_HLT;
  IoWrite16 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO1_CNT, Data16);

  //
  // Read the Second TO status bit
  //
  Data8 = IoRead8 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO2_STS);
  DEBUG((EFI_D_ERROR, "pre read:%x\n", Data8));

  Data8 = IoRead8 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO2_STS);
  DEBUG((EFI_D_ERROR, "read:%x\n", Data8));
  if ((Data8 & B_PCH_TCO2_STS_SECOND_TO) == B_PCH_TCO2_STS_SECOND_TO) {
    TcoRebootHappened = 1;
  } else {
    TcoRebootHappened = 0;
  }
  if (TcoRebootHappened) {
    DEBUG ((EFI_D_ERROR, "EarlyPlatformPchInit - TCO Second TO status bit is set. This might be a TCO reboot\n"));
  }

  //
  // Clear the Second TO status bit
  //
  Data8 |= B_PCH_TCO2_STS_SECOND_TO;
  IoWrite8 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO2_STS, Data8);

  //
  // Disable SERR NMI and IOCHK# NMI in port 61
  //
  Data8 = IoRead8 (R_PCH_NMI_SC);
  Data8 |= (B_PCH_NMI_SC_PCI_SERR_EN | B_PCH_NMI_SC_IOCHK_NMI_EN);
  IoWrite8 (R_PCH_NMI_SC, Data8);

  PchPcrAndThenOr32 (PID_ITSS, R_PCH_PCR_ITSS_GIC, (UINT32)~B_PCH_PCR_ITSS_GIC_AME, 0);

  //
  // Clear EISS bit to allow for SPI use
  //
  MmioAnd8 (SpiBaseAddress + R_PCH_SPI_BC, (UINT8)~B_PCH_SPI_BC_EISS);

  DEBUG((DEBUG_ERROR, "EarlyPlatformPchInit - End\n"));
}


/**

  Initialize POC register by Variable.

  @param *SystemConfiguration  -  Pointer to SystemConfiguration variables.

  @retval EFI_SUCCESS  -  Success.

**/
EFI_STATUS
UpdatePlatformInfo (
  IN   SYSTEM_CONFIGURATION               *SystemConfiguration,
  IN   SOCKET_CONFIGURATION               *SocketConfiguration
  )
{
  SOCKET_PROCESSORCORE_CONFIGURATION *SocketProcessorCoreConfig;
  SOCKET_IIO_CONFIGURATION           *SocketIioConfig;
  EFI_STATUS                         Status;
  UINT32                             PcIoApicEnable;
#if MAX_SOCKET <= 4
  UINTN                              Index;
#endif

  DEBUG((EFI_D_ERROR, "platform update platform info entry\n"));

  SocketProcessorCoreConfig = &SocketConfiguration->SocketProcessorCoreConfiguration;
  SocketIioConfig = &SocketConfiguration->IioConfig;

#if MAX_SOCKET <= 4
  for (Index = 0; Index < 24; Index++) {
    if (SocketIioConfig->DevPresIoApicIio[Index]) {
      PcIoApicEnable |= (1 << Index);
    }
  }

#else
  // Enable all 32 IOxAPIC
  PcIoApicEnable = 0xFFFFFFFF;
#endif
  Status = PcdSet32S (PcdPcIoApicEnable, PcIoApicEnable);
  ASSERT_EFI_ERROR (Status);
  //
  // Check to make sure TsegSize is in range, if not use default.
  //
  if (SocketProcessorCoreConfig->TsegSize > MAX_PROCESSOR_TSEG) {
    SocketProcessorCoreConfig->TsegSize = MAX_PROCESSOR_TSEG; // if out of range make default 64M
  }
  Status = PcdSet32S (PcdMemTsegSize, (0x400000 << SocketProcessorCoreConfig->TsegSize));
  ASSERT_EFI_ERROR (Status);
  if (SocketProcessorCoreConfig->IedSize > 0) {
    Status = PcdSet32S (PcdMemIedSize, (0x400000 << (SocketProcessorCoreConfig->IedSize - 1)));
    ASSERT_EFI_ERROR (Status);
  } else {
    Status = PcdSet32S (PcdMemIedSize, 0);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Minimum SMM range in TSEG should be larger than 3M
  //
  ASSERT (PcdGet32 (PcdMemTsegSize) - PcdGet32 (PcdMemIedSize) >= 0x300000);

  return EFI_SUCCESS;
}

/**
  Clear any SMI status or wake status left from boot.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
ClearPchSmiAndWake (
  VOID
  )
{
  UINT16              ABase;
  UINT16              Pm1Sts = 0;


  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts |=
    (
      B_PCH_ACPI_PM1_STS_PWRBTN
    );
  PchAcpiBaseGet (&ABase);
  //
  // Write them back
  //
  IoWrite16 (ABase + R_PCH_ACPI_PM1_STS,  Pm1Sts);

  //
  // Clear the GPE and PM enable
  //
  IoWrite16 (ABase + R_PCH_ACPI_PM1_EN, 0);
  IoWrite32 (ABase + R_PCH_ACPI_GPE0_EN_127_96, 0);

  return EFI_SUCCESS;
}

EFI_STATUS
PlatformInitGpios (
  VOID
  )
{
  EFI_STATUS                   Status;
  GPIO_INIT_CONFIG             *GpioTable;
  UINTN                        TableSize;

  TableSize = mGpioTableSizeMicrosoftWcs;
  DEBUG ((DEBUG_ERROR, "UBA:Size of GpioTable 0x%X, blocks: 0x%X.\n", TableSize, (TableSize/sizeof (GPIO_INIT_CONFIG)) ));

  GpioTable = mGpioTableMicrosoftWcs;
  DEBUG ((DEBUG_ERROR, "UBA: ConfigureGpio() MtOlympus Start.\n"));
  Status = GpioConfigurePads (TableSize/sizeof (GPIO_INIT_CONFIG), GpioTable);
  DEBUG ((DEBUG_ERROR, "UBA: ConfigureGpio() MtOlympus End. Status = %r\n", Status));

  return EFI_SUCCESS;
}

VOID
SetUsbConfig (
  VOID
  )
{
  EFI_STATUS                   Status;

  Status = PcdSet64S (PcdUsb20OverCurrentMappings, (UINT64)(UINTN)Usb20OverCurrentMappings);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet64S (PcdUsb30OverCurrentMappings, (UINT64)(UINTN)Usb30OverCurrentMappings);
  ASSERT_EFI_ERROR (Status);
}

VOID
IioPortBifurcationConfig (
  VOID
  )
{
  EFI_STATUS                   Status;

  Status = PcdSet64S (PcdIioBifurcationTable, (UINT64)(UINTN)mIioBifurcationTable);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet8S (PcdIioBifurcationTableEntries, mIioBifurcationTableEntries);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet64S (PcdIioSlotTable, (UINT64)(UINTN)mIioSlotTable);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet8S (PcdIioSlotTableEntries, mIioSlotTableEntries);
  ASSERT_EFI_ERROR (Status);
}

VOID
AllLanesEparamTableConfig (
  VOID
  )
{
  EFI_STATUS                   Status;

  Status = PcdSet64S (PcdAllLanesEparamTable, (UINT64)(UINTN)KtiMtOlympusAllLanesEparamTable);
  ASSERT_EFI_ERROR (Status);
  Status = PcdSet32S (PcdAllLanesEparamTableSize, KtiMtOlympusAllLanesEparamTableSize);
  ASSERT_EFI_ERROR (Status);
}

EFI_STATUS
PchLanConfig (
  IN SYSTEM_CONFIGURATION         *SystemConfig
  )
{
  GpioSetOutputValue (GPIO_SKL_H_GPP_I9, (UINT32)SystemConfig->LomDisableByGpio);

  return EFI_SUCCESS;
}

/**
  Write to mask registers of master and slave 8259 PICs.

**/
VOID
STATIC
Mask8259Interrupts (
  VOID
  )
{
  IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER, 0xFF);
  IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE, 0xFF);
}

EFI_STATUS
EFIAPI
MtOlympusBoardInitBeforeMemoryInit (
  VOID
  )
{
  EFI_STATUS                            Status;
  SETUP_DATA                            SetupData;
  SYSTEM_CONFIGURATION                  SystemConfiguration;
  PCH_RC_CONFIGURATION                  PchRcConfiguration;
  SOCKET_CONFIGURATION                  SocketConfiguration;
  UINT16                                ABase;
  UINT16                                Pm1Sts;
  UINT32                                Pm1Cnt;
  CONST EFI_PEI_SERVICES **             PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  ZeroMem (&SetupData, sizeof(SETUP_DATA));
  CopyMem (&SetupData.SocketConfig.IioConfig, PcdGetPtr(PcdSocketIioConfigData), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.CommonRcConfig, PcdGetPtr(PcdSocketCommonRcConfigData), sizeof(SOCKET_COMMONRC_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.CsiConfig, PcdGetPtr(PcdSocketMpLinkConfigData), sizeof(SOCKET_MP_LINK_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.MemoryConfig, PcdGetPtr(PcdSocketMemoryConfigData), sizeof(SOCKET_MEMORY_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.PowerManagementConfig, PcdGetPtr(PcdSocketPowerManagementConfigData), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.SocketProcessorCoreConfiguration, PcdGetPtr(PcdSocketProcessorCoreConfigData), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&SetupData.SystemConfig, PcdGetPtr(PcdSetupData), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&SetupData.PchRcConfig, PcdGetPtr(PcdPchRcConfigurationData), sizeof(PCH_RC_CONFIGURATION));

  CopyMem (&SocketConfiguration, &(SetupData.SocketConfig), sizeof (SOCKET_CONFIGURATION));
  CopyMem (&PchRcConfiguration, &(SetupData.PchRcConfig), sizeof (PCH_RC_CONFIGURATION));
  CopyMem (&SystemConfiguration, &(SetupData.SystemConfig), sizeof (SYSTEM_CONFIGURATION));

  ///
  /// Set LPC SIO
  ///
  MmioOr16(
    (MmPciBase(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC) + R_PCH_LPC_IOE),
    B_PCH_LPC_IOE_SE
    );

  LpcSioEarlyInit ();

  Status = PlatformInitGpios ();
  ASSERT_EFI_ERROR (Status);

  SetUsbConfig ();
  IioPortBifurcationConfig ();
  AllLanesEparamTableConfig ();

  ///
  /// Do Early PCH init
  ///
  EarlyPlatformPchInit ((EFI_PEI_SERVICES**)PeiServices, &SystemConfiguration, &PchRcConfiguration);

  ///
  /// Clear PCH SMI and Wake
  /// Clear all pending SMI. On S3 clear power button enable so it will not generate an SMI.
  ///
  Status = ClearPchSmiAndWake();
  ASSERT_EFI_ERROR (Status);
  ///----------------------------------------------------------------------------------
  ///
  /// BIOS should check the WAK_STS bit in PM1_STS[15] (PCH register ABASE+00h) before memory
  /// initialization to determine if ME has reset the system while the host was in a sleep state.
  /// If WAK_STS is not set, BIOS should ensure a non-sleep exit path is taken by overwriting
  /// PM1_CNT[12:10] (PCH register ABASE+04h) to 111b to force an s5 exit.
  ///
  PchAcpiBaseGet (&ABase);
  Pm1Sts = IoRead16 (ABase + R_PCH_ACPI_PM1_STS);
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_WAK) == 0) {
    Pm1Cnt = IoRead32 (ABase + R_PCH_ACPI_PM1_CNT);
    Pm1Cnt |= V_PCH_ACPI_PM1_CNT_S5;
    IoWrite32 (ABase + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  }

  UpdatePlatformInfo (&SystemConfiguration, &SocketConfiguration);

  //
  // Do platform specific on-board Zoar init
  //
  PchLanConfig (&SystemConfiguration);

  //
  // The 8259 PIC is still functional and not masked by default even if APIC is
  // enabled. So need to disable all 8259 interrupts.
  //
  Mask8259Interrupts ();

  return EFI_SUCCESS;
}

/**

  Turn off system if needed.

  @param PeiServices Pointer to PEI Services
  @param CpuIo       Pointer to CPU I/O Protocol

  @retval None.

**/
VOID
CheckPowerOffNow (
  VOID
  )
{

  UINT16  Pm1Sts;

  //
  // Read and check the ACPI registers
  //
  Pm1Sts = IoRead16 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_ACPI_PM1_STS);
  DEBUG ((EFI_D_ERROR, "CheckPowerOffNow()- Pm1Sts= 0x%04x\n", Pm1Sts ));

  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_PWRBTN) == B_PCH_ACPI_PM1_STS_PWRBTN) {
    IoWrite16 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_PWRBTN);
    IoWrite16 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5);
    IoWrite16 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5 + B_PCH_ACPI_PM1_CNT_SLP_EN);
  }
}

EFI_STATUS
EFIAPI
MtOlympusBoardInitAfterMemoryInit (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_BOOT_MODE               BootMode;
  UINT16                      Pm1Cnt;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Check if user wants to turn off in PEI phase
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    CheckPowerOffNow ();
  } else {
    Pm1Cnt  = IoRead16 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_ACPI_PM1_CNT);
    Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SLP_TYP;
    IoWrite16 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MtOlympusBoardDebugInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_BOOT_MODE
EFIAPI
MtOlympusBoardBootModeDetect (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

