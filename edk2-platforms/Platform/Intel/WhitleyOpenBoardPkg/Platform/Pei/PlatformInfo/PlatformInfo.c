/** @file
  Platform Info PEIM.

  @copyright
  Copyright 1999 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PlatformInfo.h"
#include <GpioPinsSklH.h>
#include <Library/GpioLib.h>
#include <Library/PchInfoLib.h>

#include <Ppi/DynamicSiLibraryPpi.h>

#include <Library/UbaGpioPlatformConfig.h>
#include <UncoreCommonIncludes.h>
#include <PlatformInfoTypes.h>

#include <Library/PeiServicesLib.h>

#define  TEMP_BUS_NUMBER    (0x3F)


STATIC EFI_PEI_PPI_DESCRIPTOR       mPlatformInfoPpi = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPlatformInfoGuid,
    NULL
  };

#define  BOARD_ID_GPIO_PADS_NUMBER 6
#define  BOARD_REV_ID_GPIO_PADS_NUMBER 3

//
// These pads shall not be board specific as these are used for Board ID and Rev ID detection
// Therefore can not be moved to UBA and are common for all Purley boards
//
GPIO_PAD                 mBoardId [BOARD_ID_GPIO_PADS_NUMBER] = {
  // BoardId pads - PADCFG register  for GPIO G12
  // WARNING: The pad number must be obtained from board schematics
  GPIO_SKL_H_GPP_G12,
  GPIO_SKL_H_GPP_G13,
  GPIO_SKL_H_GPP_G14,
  GPIO_SKL_H_GPP_G15,
  GPIO_SKL_H_GPP_G16,
  GPIO_SKL_H_GPP_B19
};

GPIO_PAD                 mBoardRevId [BOARD_REV_ID_GPIO_PADS_NUMBER] = {
  // Board RevId pads - Start from pad C12
  // WARNING: This should be obtained from board schematics
  GPIO_SKL_H_GPP_C12,
  GPIO_SKL_H_GPP_C13,
  GPIO_SKL_H_GPP_B9
};

GPIO_CONFIG              mBoardAndRevIdConfig = {
  // Board and Revision ID pads configuration required for proper reading the values
  GpioPadModeGpio, GpioHostOwnDefault, GpioDirIn, GpioOutDefault, GpioIntDefault,
    GpioPlatformReset, GpioTermDefault, GpioLockDefault, GpioRxRaw1Default
};


VOID
GpioConfigForBoardId (
  VOID
  )
{
  UINT8                   i;
  EFI_STATUS              Status;
  GPIO_CONFIG             PadConfig;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

  PadConfig.PadMode          = mBoardAndRevIdConfig.PadMode;
  PadConfig.HostSoftPadOwn   = mBoardAndRevIdConfig.HostSoftPadOwn;
  PadConfig.Direction        = mBoardAndRevIdConfig.Direction;
  PadConfig.OutputState      = mBoardAndRevIdConfig.OutputState;
  PadConfig.InterruptConfig  = mBoardAndRevIdConfig.InterruptConfig;
  PadConfig.PowerConfig      = mBoardAndRevIdConfig.PowerConfig;
  PadConfig.ElectricalConfig = mBoardAndRevIdConfig.ElectricalConfig;
  PadConfig.LockConfig       = mBoardAndRevIdConfig.LockConfig;
  PadConfig.OtherSettings    = mBoardAndRevIdConfig.OtherSettings;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  for (i = 0; i < BOARD_ID_GPIO_PADS_NUMBER; i++) {
    Status = DynamicSiLibraryPpi->GpioSetPadConfig (mBoardId[i], &PadConfig);
    ASSERT_EFI_ERROR (Status);
  }
}


VOID
GpioConfigForBoardRevId (
  VOID
  )
{
  UINT8                   i;
  EFI_STATUS              Status;
  GPIO_CONFIG             PadConfig;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

  PadConfig.PadMode          = mBoardAndRevIdConfig.PadMode;
  PadConfig.HostSoftPadOwn   = mBoardAndRevIdConfig.HostSoftPadOwn;
  PadConfig.Direction        = mBoardAndRevIdConfig.Direction;
  PadConfig.OutputState      = mBoardAndRevIdConfig.OutputState;
  PadConfig.InterruptConfig  = mBoardAndRevIdConfig.InterruptConfig;
  PadConfig.PowerConfig      = mBoardAndRevIdConfig.PowerConfig;
  PadConfig.ElectricalConfig = mBoardAndRevIdConfig.ElectricalConfig;
  PadConfig.LockConfig       = mBoardAndRevIdConfig.LockConfig;
  PadConfig.OtherSettings    = mBoardAndRevIdConfig.OtherSettings;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  for (i = 0; i < BOARD_REV_ID_GPIO_PADS_NUMBER; i++) {
    Status = DynamicSiLibraryPpi->GpioSetPadConfig (mBoardRevId[i], &PadConfig);
    ASSERT_EFI_ERROR (Status);
  }
}

/**

    Reads GPIO pins to get Board ID value

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardId (
  OUT UINT32 *BoardId
  )
{
  EFI_STATUS              Status = EFI_DEVICE_ERROR;
  UINT32                  Data32;
  UINT8                   i;
  UINT32                  BdId;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

  if (BoardId == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  BdId = 0;

  GpioConfigForBoardId ();

  for (i = 0; i < BOARD_ID_GPIO_PADS_NUMBER; i++) {
    Status = DynamicSiLibraryPpi->GpioGetInputValue (mBoardId[i], &Data32);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (Data32) {
      BdId = BdId | (1 << i);
    }
  }
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  *BoardId = BdId;
  return EFI_SUCCESS;
}

/**

    Reads GPIO pins to get Board Revision ID value

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardRevId (
  OUT UINT32 *BoardRevId
  )
{
  EFI_STATUS              Status = EFI_DEVICE_ERROR;
  UINT32                  Data32;
  UINT8                   i;
  UINT32                  RevId;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

  if (BoardRevId == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  RevId = 0;

  GpioConfigForBoardRevId ();

  for (i = 0; i < BOARD_REV_ID_GPIO_PADS_NUMBER; i++){
    Status =  DynamicSiLibraryPpi->GpioGetInputValue (mBoardRevId[i], &Data32);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (Data32) {
      RevId = RevId | (1 << i);
    }
  }
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  *BoardRevId = RevId;
  return EFI_SUCCESS;

}

/**

   Returns the Model ID of the CPU.
   Model ID = EAX[7:4]

**/
VOID
GetCpuInfo (
  UINT32    *CpuType,
  UINT8     *CpuStepping
  )

{
  UINT32                    RegEax=0;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  *CpuStepping = (UINT8) (RegEax & 0x0F);
  *CpuType     = (UINT32) (RegEax >> 4);
}


/**

    GC_TODO: add routine description

    @param BAR         - GC_TODO: add arg description
    @param PeiServices - GC_TODO: add arg description

    @retval None

**/
VOID
InitGSX(
  UINT32                *BAR,
  IN EFI_PEI_SERVICES  **PeiServices
)
{
}

/**

    GC_TODO: add routine description

    @param Data        - GC_TODO: add arg description
    @param PeiServices - GC_TODO: add arg description

    @retval EFI_SUCCESS     - GC_TODO: add retval description
    @retval EFI_UNSUPPORTED - GC_TODO: add retval description

**/
EFI_STATUS
GsxRead(
   UINT32                *Data,
   IN EFI_PEI_SERVICES  **PeiServices
)
{
  return EFI_UNSUPPORTED;
}

/**

    GC_TODO: add routine description

    @param Data        - GC_TODO: add arg description
    @param PeiServices - GC_TODO: add arg description

    @retval None

**/
VOID
GetGsxBoardID(
   BOARD_ID             *Data,
   IN EFI_PEI_SERVICES  **PeiServices
)
{

  EFI_STATUS                Status;
  UINT32                    GSXIN[2];
  UINT32                    RetryCount;

  RetryCount = 0;
  GSXIN[0]   = 0;
  GSXIN[1]   = 0;

  do {
    Status     = GsxRead(GSXIN, PeiServices);

    if(Status){
      // if EFI_SUCCESS != Success then retry one more time
      RetryCount ++;
    }else{
      // if EFI_SUCCESS read Board ID and exit
      RetryCount = 0xFFFFFFFF;
    }

    if (GSXIN[0] & BIT0) {
      Data->BoardID.BoardID0 = 1;
    }

    if (GSXIN[0] & BIT1) {
      Data->BoardID.BoardID1 = 1;
    }

    if (GSXIN[0] & BIT2) {
      Data->BoardID.BoardID2 = 1;
    }

    if (GSXIN[0] & BIT3) {
      Data->BoardID.BoardID3 = 1;
    }

    if (GSXIN[0] & BIT4) {
      Data->BoardID.BoardID4 = 1;
    }

    if (GSXIN[0] & BIT5) {
      Data->BoardID.BoardRev0 = 1;
    }

    if (GSXIN[0] & BIT6) {
      Data->BoardID.BoardRev1 = 1;
    }

  } while(RetryCount < 1);

  if(Status){
    //
    // Unhable to read GSX HW error Hang the system
    //
    DEBUG ((EFI_D_ERROR, "ERROR: GSX HW is unavailable, SYSTEM HANG\n"));
    CpuDeadLoop ();
  }
}

/**
    Get Platform Type by read Platform Data Region in SPI flash.
    SPI Descriptor Mode Routines for Accessing Platform Info from Platform Data Region (PDR)

    @param PeiServices  -  General purpose services available to every PEIM.
    @param PlatformInfoHob - Platform Type is returned in PlatformInfoHob->BoardId

    @retval Status EFI_SUCCESS - PDR read success
    @retval Status EFI_INCOMPATIBLE_VERSION - PDR read but it is not valid Platform Type

**/
EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO         *PlatformInfoHob
  )
{
  EFI_STATUS              Status;
  PCH_SPI_PROTOCOL        *SpiPpi;
  UINTN                   Size;

  //
  // Locate the SPI PPI Interface
  //
  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gPchSpiPpiGuid,
                    0,
                    NULL,
                    &SpiPpi
                    );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Read the PIT (Platform Info Table) from the SPI Flash Platform Data Region
  //
  Size = sizeof (EFI_PLATFORM_INFO);
  Status = SpiPpi->FlashRead (
                              SpiPpi,
                              FlashRegionPlatformData,
                              PDR_REGION_START_OFFSET,
                              (UINT32) Size,
                              (UINT8 *) PlatformInfoHob
                             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if ((PlatformInfoHob->BoardId >= TypePlatformMin) && (PlatformInfoHob->BoardId <= TypePlatformMax)) {
    //
    // Valid Platform Identified
    //
    DEBUG ((DEBUG_INFO, "Platform Info from PDR: Type = %x\n",PlatformInfoHob->BoardId));
  } else {
    //
    // Reading PIT from SPI PDR Failed or a unknown platform identified
    //
    DEBUG ((EFI_D_ERROR, "PIT from SPI PDR reports Platform ID as %x. This is unknown ID. Assuming Greencity Platform!\n", PlatformInfoHob->BoardId));
    PlatformInfoHob->BoardId = TypePlatformUnknown;
    Status = EFI_INCOMPATIBLE_VERSION;
  }
  return Status;
}

VOID
GatherQATInfo(OUT EFI_PLATFORM_INFO   *PlatformInfoHob)
/**

    GC_TODO: add routine description

    @param None

    @ret None
**/
{
  EFI_STATUS              Status;
  GPIO_CONFIG             PadConfig;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

    // Gpio programming to QAT board detection
  PadConfig.PadMode          = GpioPadModeGpio;
  PadConfig.HostSoftPadOwn   = GpioHostOwnDefault;
  PadConfig.Direction        = GpioDirIn;
  PadConfig.OutputState      = GpioOutLow;
  PadConfig.InterruptConfig  = GpioIntDis;
  PadConfig.PowerConfig      = GpioResetPwrGood;
  PadConfig.ElectricalConfig = GpioTermNone;
  PadConfig.LockConfig       = GpioPadConfigLock;
  PadConfig.OtherSettings    = 00;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = DynamicSiLibraryPpi->GpioSetPadConfig (GPIO_SKL_H_GPP_B3, &PadConfig);
  Status = DynamicSiLibraryPpi->GpioGetInputValue (GPIO_SKL_H_GPP_B3, &PlatformInfoHob->QATDis);
  Status = DynamicSiLibraryPpi->GpioSetPadConfig (GPIO_SKL_H_GPP_B4, &PadConfig);
  Status = DynamicSiLibraryPpi->GpioGetInputValue (GPIO_SKL_H_GPP_B4, &PlatformInfoHob->QATSel);
}

EFI_STATUS
GetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  )
/**

    GC_TODO: add routine description

    @param PeiServices     - GC_TODO: add arg description
    @param PlatformInfoHob - GC_TODO: add arg description

    @retval EFI_UNSUPPORTED - GC_TODO: add retval description
    @retval EFI_SUCCESS     - GC_TODO: add retval description

**/
{


  UINT32                  BoardId;
  UINT32                  BoardRev;
  EFI_PEI_PCI_CFG2_PPI    *PciCfgPpi;
  EFI_STATUS              Status;

  PciCfgPpi = (**PeiServices).PciCfg;
  ASSERT (PciCfgPpi != NULL);

  PlatformInfoHob->BoardId = TypeNeonCityEPRP;
  DEBUG ((DEBUG_INFO, "Use GPIO to read Board ID\n"));

  Status = GpioGetBoardId (&BoardId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error: Can't read GPIO to get Board ID!\n"));
    return Status;
  }
  Status = GpioGetBoardRevId (&BoardRev);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Error: Can't read GPIO to get Board ID!\n"));
    return Status;
  }
  PlatformInfoHob->TypeRevisionId = BoardRev;

  switch (BoardId) {
    case 0x00:  // for Simics
      PlatformInfoHob->BoardId = TypeWilsonCityRP;
    break;
    case 0x01:
      PlatformInfoHob->BoardId = TypeWilsonCityRP;
      DEBUG ((DEBUG_INFO, "Board ID = TypeWilsonCityRP\n"));
      break;
    case 0x12:
      PlatformInfoHob->BoardId = TypeWilsonCityRP;
      DEBUG((DEBUG_INFO, "Board ID = TypeWilsonCityRP\n"));
      break;
    case 0x15:
      PlatformInfoHob->BoardId = TypeWilsonCitySMT;
      DEBUG((DEBUG_INFO, "Board ID = TypeWilsonCitySMT\n"));
      break;
    case 0x17:
    case 0x18:
      PlatformInfoHob->BoardId = TypeCooperCityRP;
      DEBUG((DEBUG_INFO, "Board ID = TypeCooperCityRP\n"));
      break;
    default:
      PlatformInfoHob->BoardId = TypePlatformDefault;
      DEBUG ((DEBUG_INFO, "Board ID = %2X Default set to TypePlatformDefault\n",BoardId));
      break;
  }

  GatherQATInfo(PlatformInfoHob);

  DEBUG ((DEBUG_INFO, "Board Rev.: %d\n", BoardRev));
  return EFI_SUCCESS;
}

/**

  This function initializes the board related flag to indicates if
  PCH and Lan-On-Motherboard (LOM) devices is supported.

**/
VOID
GetPchLanSupportInfo(
  IN EFI_PLATFORM_INFO   *PlatformInfoHob
  )
{
  PlatformInfoHob->PchData.LomLanSupported  = 0;
}

/**

    GC_TODO: add routine description

    @param PeiVariable     - GC_TODO: add arg description
    @param PlatformInfoHob - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
GetIioCommonRcPlatformSetupPolicy(
  OUT EFI_PLATFORM_INFO                     *PlatformInfoHob
  )
  {
  UINT8                                 IsocEn;

  CopyMem (&IsocEn, (UINT8 *)PcdGetPtr(PcdSocketCommonRcConfig) + OFFSET_OF(SOCKET_COMMONRC_CONFIGURATION, IsocEn), sizeof(UINT8));

  PlatformInfoHob->SysData.IsocEn    = IsocEn;       // ISOC enabled

  return EFI_SUCCESS;
}
/**

    GC_TODO: add routine description

    @param PeiVariable     - GC_TODO: add arg description
    @param PlatformInfoHob - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
GetIioPlatformSetupPolicy(
  OUT EFI_PLATFORM_INFO                     *PlatformInfoHob
  )
{
  return EFI_SUCCESS;
}


/**
  Platform Type detection. Because the PEI globle variable
  is in the flash, it could not change directly.So use
  2 PPIs to distinguish the platform type.

  @param FfsHeader    -  Pointer to Firmware File System file header.
  @param PeiServices  -  General purpose services available to every PEIM.

  @retval EFI_SUCCESS  -  Memory initialization completed successfully.
  @retval Others       -  All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
PlatformInfoInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PCI_CFG2_PPI    *PciCfgPpi;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  EFI_PLATFORM_INFO       PlatformInfoHob;
  EFI_PLATFORM_INFO       tempPlatformInfoHob;
  UINT8                   ChipId;
  UINT32                  Delay;
  UINT32                  CpuType;
  UINT8                   CpuStepping;
  DYNAMIC_SI_LIBARY_PPI   *DynamicSiLibraryPpi = NULL;

  PciCfgPpi = (**PeiServices).PciCfg;
  if (PciCfgPpi == NULL) {
    DEBUG ((EFI_D_ERROR, "\nError! PlatformInfoInit() - PeiServices is a NULL Pointer!!!\n"));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate Variable PPI
  //
  Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, &PeiVariable);

  (*PeiServices)->SetMem (&PlatformInfoHob, sizeof (PlatformInfoHob), 0);

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // --------------------------------------------------
  //
  // Detect the iBMC SIO for CV/CRB Platforms
  // 0x2E/0x2F decoding has been enabled in MonoStatusCode PEIM.
  //
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_SIO_UNLOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_CHIP_ID_REG);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  ChipId = IoRead8  (PILOTIV_SIO_DATA_PORT);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);
  IoWrite8 (PILOTIV_SIO_INDEX_PORT, PILOTIV_SIO_LOCK);
  for (Delay = 0; Delay < 40; Delay++) IoRead8  (0x61);

  if (EFI_ERROR (Status))
  {
        DEBUG((EFI_D_ERROR, "LocatePpi Error in PlatformInfo.c !\n"));
  }

  Status = GetIioPlatformSetupPolicy (&PlatformInfoHob);
  ASSERT_EFI_ERROR (Status);
  Status = GetIioCommonRcPlatformSetupPolicy (&PlatformInfoHob);
  ASSERT_EFI_ERROR (Status);

  //
  // Update PCH Type
  //
  PlatformInfoHob.PchType = DynamicSiLibraryPpi->GetPchSeries ();
  PlatformInfoHob.PchSku = DynamicSiLibraryPpi->GetPchLpcDeviceId ();
  PlatformInfoHob.PchRevision = (UINT8) DynamicSiLibraryPpi->PchStepping ();
  PlatformInfoHob.MaxNumOfPchs = 1;
  Status = EFI_SUCCESS;

    if(!EFI_ERROR(Status)) {
      Status = GetPlatformInfo (PeiServices, &PlatformInfoHob);
      if(EFI_ERROR (Status)) {
        Status = PdrGetPlatformInfo (PeiServices, &tempPlatformInfoHob);
        PlatformInfoHob.BoardId = tempPlatformInfoHob.BoardId;
        PlatformInfoHob.TypeRevisionId = tempPlatformInfoHob.TypeRevisionId;
        if (EFI_ERROR(Status)) {
          PlatformInfoHob.BoardId = TypePlatformUnknown;
        }
      }
    } else {
      PlatformInfoHob.BoardId = TypePlatformUnknown;
    }

  //
  // Update IIO Type
  //
  PlatformInfoHob.IioRevision = 0;


  //
  // Get Subtractive decode enable bit from descriptor
  //

  if (DynamicSiLibraryPpi->PchIsGbeRegionValid () == FALSE) {
    PlatformInfoHob.PchData.GbeRegionInvalid = 1;
  } else {
    PlatformInfoHob.PchData.GbeRegionInvalid = 0;
  }
  GetPchLanSupportInfo (&PlatformInfoHob);
  PlatformInfoHob.PchData.GbePciePortNum = 0xFF;
  PlatformInfoHob.PchData.GbePciePortNum = (UINT8) DynamicSiLibraryPpi->PchGetGbePortNumber ();
  PlatformInfoHob.PchData.GbeEnabled  = DynamicSiLibraryPpi->PchIsGbePresent ();
  PlatformInfoHob.PchData.PchStepping = (UINT8) DynamicSiLibraryPpi->PchStepping ();

  PlatformInfoHob.SysData.SysSioExist = (UINT8)IsSioExist();

  GetCpuInfo (&CpuType, &CpuStepping);
  PlatformInfoHob.CpuType     = CpuType;
  PlatformInfoHob.CpuStepping = CpuStepping;

  //
  // Set default memory topology to DaisyChainTopology. This should be modified in UBA board
  // specific file.
  //
  (*PeiServices)->SetMem (&PlatformInfoHob.MemoryTopology, sizeof (PlatformInfoHob.MemoryTopology), DaisyChainTopology);

  //
  // Set default memory type connector to DimmConnectorPth. This should be modified in UBA board
  // specific file.
  //
  (*PeiServices)->SetMem (&PlatformInfoHob.MemoryConnectorType, sizeof (PlatformInfoHob.MemoryConnectorType), DimmConnectorPth);

  //
  // Build HOB for setup memory information
  //
  BuildGuidDataHob (
      &gEfiPlatformInfoGuid,
      &(PlatformInfoHob),
      sizeof (EFI_PLATFORM_INFO)
      );

  Status = (**PeiServices).InstallPpi (PeiServices, &mPlatformInfoPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Save PlatformInfoHob.BoardId in CMOS
  //
  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_LO);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)PlatformInfoHob.BoardId);

  IoWrite8 (R_IOPORT_CMOS_UPPER_INDEX, CMOS_PLATFORM_ID_HI);
  IoWrite8 (R_IOPORT_CMOS_UPPER_DATA, (UINT8)((PlatformInfoHob.PcieRiser2Type << 4) + (PlatformInfoHob.PcieRiser1Type)));

  return EFI_SUCCESS;
}
