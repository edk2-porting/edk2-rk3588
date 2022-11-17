/** @file
  Implementation of Fsp SA Policy Initialization.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <ConfigBlock/MemoryConfig.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmbusLib.h>
#include <Library/MmPciLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PcdLib.h>

#include <IndustryStandard/Pci.h>
#include <MrcSpdData.h>
#include <PchAccess.h>
#include <Ppi/FirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiPeiCis.h>

#define MAX_SPD_PAGE_COUNT           (2)
#define MAX_SPD_PAGE_SIZE            (256)
#define MAX_SPD_SIZE                 (MAX_SPD_PAGE_SIZE * MAX_SPD_PAGE_COUNT)
#define SPD_PAGE_ADDRESS_0           (0x6C)
#define SPD_PAGE_ADDRESS_1           (0x6E)
#define SPD_DDR3_SDRAM_TYPE_OFFSET   (0x02)
#define SPD_DDR3_SDRAM_TYPE_NUMBER   (0x0B)
#define SPD_DDR4_SDRAM_TYPE_NUMBER   (0x0C)
#define SPD_LPDDR3_SDRAM_TYPE_NUMBER (0xF1)
#define SPD_JEDEC_LPDDR3_SDRAM_TYPE_NUMBER (0x0F)
#define XMP_ID_STRING                (0x4A0C)
#define SPD3_MANUF_START             (117)
#define SPD3_MANUF_END               (127)
#define SPD4_MANUF_START             (320)
#define SPD4_MANUF_END               (328)
#define SPDLP_MANUF_START            (320)
#define SPDLP_MANUF_END              (328)

GLOBAL_REMOVE_IF_UNREFERENCED const SPD_OFFSET_TABLE mSpdDdr3Table[] = {
  {   0,               1,             (1 << SpdCold),},
  {   2,               2,             (1 << SpdCold) | (1 << SpdFast),},
  {   3,              41,             (1 << SpdCold),},
  {  60,              63,             (1 << SpdCold),},
  { SPD3_MANUF_START, SPD3_MANUF_END, (1 << SpdCold) | (1 << SpdFast),},
  { 128,             145,             (1 << SpdCold),},
#ifdef SUPPORT_SPD_CRC
  {  39,              59,             (1 << SpdCold),},
  {  64,             125,             (1 << SpdCold),},
#endif
  { 176,             179,             (1 << SpdCold),},
  { 180,             184,             (1 << SpdCold),},
  { 185,             215,             (1 << SpdCold),},
  { 220,             250,             (1 << SpdCold),},
};

GLOBAL_REMOVE_IF_UNREFERENCED const SPD_OFFSET_TABLE mSpdDdr4Table[] = {
  {   0,               1,             (1 << SpdCold),},
  {   2,               2,             (1 << SpdCold) | (1 << SpdFast),},
  {   3,              40,             (1 << SpdCold),},
  { 117,             131,             (1 << SpdCold),},
  { SPD4_MANUF_START, SPD4_MANUF_END, (1 << SpdCold) | (1 << SpdFast),},
  { 329,             348,             (1 << SpdCold),},
#ifdef SUPPORT_SPD_CRC
  {  32,             119,             (1 << SpdCold),},
  { 126,             255,             (1 << SpdCold),},
  { 349,             383,             (1 << SpdCold),},
#endif
  { 384,             387,             (1 << SpdCold),},
  { 388,             389,             (1 << SpdCold),},
  { 393,             431,             (1 << SpdCold),},
  { 440,             478,             (1 << SpdCold),},
};

GLOBAL_REMOVE_IF_UNREFERENCED const SPD_OFFSET_TABLE mSpdLpddrTable[] = {
  {   0,               1,               (1 << SpdCold),},
  {   2,               2,               (1 << SpdCold) | (1 << SpdFast),},
  {   3,              32,               (1 << SpdCold),},
  { 120,             130,               (1 << SpdCold),},
  { SPDLP_MANUF_START, SPDLP_MANUF_END, (1 << SpdCold) | (1 << SpdFast),},
  { 329,             348,               (1 << SpdCold),},
#ifdef SUPPORT_SPD_CRC
  {  31,             121,               (1 << SpdCold),},
  { 126,             255,               (1 << SpdCold),},
  { 349,             383,               (1 << SpdCold),},
#endif
  { 384,             387,               (1 << SpdCold),},
  { 388,             389,               (1 << SpdCold),},
  { 393,             431,               (1 << SpdCold),},
  { 440,             478,               (1 << SpdCold),},
};


/**
  Read the SPD data over the SMBus, at the specified SPD address, starting at
  the specified starting offset and read the given amount of data.

  @param[in] SpdAddress  - SPD SMBUS address
  @param[in, out] Buffer - Buffer to store the data.
  @param[in] Start       - Starting SPD offset
  @param[in] Size        - The number of bytes of data to read and also the size of the buffer.
  @param[in, out] Page   - The final page that is being pointed to.

  @retval EFI_SUCCESS if the read is successful, otherwise error status.
**/
static
EFI_STATUS
InternalDoSpdRead (
  IN     const UINT8  SpdAddress,
  IN OUT UINT8        *const Buffer,
  IN     const UINT16 Start,
  IN           UINT16 Size,
  IN OUT UINT8        *const Page
  )
{
  EFI_STATUS    EfiStatus;
  BOOLEAN       PageUpdate;
  UINT16        Count;
  UINT16        Index;

  EfiStatus = EFI_DEVICE_ERROR;
  if ((Buffer != NULL) && (Start < MAX_SPD_SIZE) && ((Start + Size) < MAX_SPD_SIZE)) {
    Count = 0;
    PageUpdate = FALSE;
    while (Size--) {
      Index = Start + Count;
      if ((Index / MAX_SPD_PAGE_SIZE) != *Page) {
        *Page = (UINT8) (Index / MAX_SPD_PAGE_SIZE);
        PageUpdate = TRUE;
      }
      Index %= MAX_SPD_PAGE_SIZE;
      if (PageUpdate == TRUE) {
        PageUpdate = FALSE;
        SmBusWriteDataByte ((*Page == 0) ? SPD_PAGE_ADDRESS_0 : SPD_PAGE_ADDRESS_1, 0, &EfiStatus);
      }
      Buffer[Count] = SmBusReadDataByte (SpdAddress | ((UINT32) Index << 8), &EfiStatus);
      if (EFI_SUCCESS != EfiStatus) {
        Buffer[Count] = 0;
        break;
      }
      Count++;
    }
    EfiStatus = EFI_SUCCESS;
  }
  return (EfiStatus);
}

/**
  See if there is valid XMP SPD data.

  @param[in] Debug    - Mrc debug structure.
  @param[in, out] Spd - Mrc SPD structure.
  @param[in] XmpStart - The current offset in the SPD.

  @retval TRUE if valid, FALSE in not.
**/
static
BOOLEAN
InternalVerifyXmp (
  IN OUT MrcSpd *const Spd,
  IN const UINT16 XmpStart
  )
{
  SPD_EXTREME_MEMORY_PROFILE_HEADER      *Header1;
  SPD_EXTREME_MEMORY_PROFILE_HEADER_2_0  *Header2;
  BOOLEAN                                 Xmp;

  Xmp = FALSE;

  switch (((UINT8 *)Spd)[2]) {
    case SPD_DDR3_SDRAM_TYPE_NUMBER:
      Header1 = &Spd->Ddr3.Xmp.Header;
      if (XmpStart == ((UINT32) (Header1) - (UINT32) Spd)) {
        Xmp = TRUE;
        if ((Header1->XmpRevision.Data & 0xFE) == 0x12) {
          return (TRUE);
        } else {
          Header1->XmpId            = 0;
          Header1->XmpOrgConf.Data  = 0;
          Header1->XmpRevision.Data = 0;
        }
      }
      break;
    case SPD_DDR4_SDRAM_TYPE_NUMBER:
      Header2 = &Spd->Ddr4.EndUser.Xmp.Header;
      if (XmpStart == ((UINT32) (Header2) - (UINT32) Spd)) {
        Xmp = TRUE;
        if ((Header2->XmpRevision.Data) == 0x20) {
          return (TRUE);
        } else {
          Header2->XmpId            = 0;
          Header2->XmpOrgConf.Data  = 0;
          Header2->XmpRevision.Data = 0;
        }
      }
      break;
    case SPD_JEDEC_LPDDR3_SDRAM_TYPE_NUMBER:
    case SPD_LPDDR3_SDRAM_TYPE_NUMBER:
      return (TRUE);
    default:
      return (FALSE);
  }
  if (!Xmp) {
    return (TRUE);
  }
  return (FALSE);
}

/**
  Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
  The SPD data locations read is controlled by the current boot mode.

  @param[in] BootMode          - The current MRC boot mode.
  @param[in] Address           - SPD SmBus address offset.
  @param[in] Buffer            - Buffer that contains the data read from the SPD.
  @param[in] SpdDdr3Table      - Indicates which SPD bytes to read.
  @param[in] SpdDdr3TableSize  - Size of SpdDdr3Table in bytes.
  @param[in] SpdDdr4Table      - Indicates which SPD bytes to read.
  @param[in] SpdDdr4TableSize  - Size of SpdDdr4Table in bytes.
  @param[in] SpdLpddrTable     - Indicates which SPD bytes to read.
  @param[in] SpdLpddrTableSize - Size of SpdLpddrTable in bytes.

  @retval TRUE if the read is successful, otherwise FALSE on error.
**/
BOOLEAN
InternalGetSpdData (
  IN SPD_BOOT_MODE BootMode,
  IN UINT8         Address,
  IN OUT   UINT8   *Buffer,
  IN UINT8         *SpdDdr3Table,
  IN UINT32        SpdDdr3TableSize,
  IN UINT8         *SpdDdr4Table,
  IN UINT32        SpdDdr4TableSize,
  IN UINT8         *SpdLpddrTable,
  IN UINT32        SpdLpddrTableSize
  )
{
  const SPD_OFFSET_TABLE *Tbl;
  const SPD_OFFSET_TABLE *TableSelect;
  EFI_STATUS             Status;
  UINT32                 Byte;
  UINT32                 Stop;
  UINT8                  Page;

  Page   = (UINT8) (~0);
  Status = InternalDoSpdRead (Address, &Buffer[SPD_DDR3_SDRAM_TYPE_OFFSET], 2, 1, &Page);
  if (EFI_SUCCESS == Status) {
    switch (Buffer[SPD_DDR3_SDRAM_TYPE_OFFSET]) {
      case SPD_DDR3_SDRAM_TYPE_NUMBER:
      case SPD_LPDDR3_SDRAM_TYPE_NUMBER:
      default:
        TableSelect = (SPD_OFFSET_TABLE *) SpdDdr3Table;
        Stop = (SpdDdr3TableSize / sizeof (SPD_OFFSET_TABLE));
        break;
      case SPD_DDR4_SDRAM_TYPE_NUMBER:
        TableSelect = (SPD_OFFSET_TABLE *) SpdDdr4Table;
        Stop = (SpdDdr4TableSize / sizeof (SPD_OFFSET_TABLE));
        break;
      case SPD_JEDEC_LPDDR3_SDRAM_TYPE_NUMBER:
        TableSelect = (SPD_OFFSET_TABLE *) SpdLpddrTable;
        Stop = (SpdLpddrTableSize / sizeof (SPD_OFFSET_TABLE));
        break;
    }
    for (Byte = 0; (EFI_SUCCESS == Status) && (Byte < Stop); Byte++) {
      Tbl = &TableSelect[Byte];
      if ((1 << BootMode) & Tbl->BootMode) {
        Status = InternalDoSpdRead (Address, &Buffer[Tbl->Start], Tbl->Start, Tbl->End - Tbl->Start + 1, &Page);
        if (Status == EFI_SUCCESS) {
          if (SpdCold == BootMode) {
            if (FALSE == InternalVerifyXmp ((MrcSpd *) Buffer, Tbl->Start)) {
              break;
            }
          }
        } else {
          break;
        }
      }
    }
  }

  return ((EFI_SUCCESS == Status) ? TRUE : FALSE);
}

/**
  Initialize the Smbus PPI and program the Smbus BAR

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
**/
EFI_STATUS
InternalInitializePchSmbus (
  VOID
  )
{
  UINTN          SmbusRegBase;
  SmbusRegBase = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SMBUS,
                   PCI_FUNCTION_NUMBER_PCH_SMBUS
                   );

  ///
  /// Since PEI has no PCI enumerator, set the BAR & I/O space enable ourselves
  ///
  MmioAndThenOr32 (SmbusRegBase + R_PCH_SMBUS_BASE, B_PCH_SMBUS_BASE_BAR, PcdGet16 (PcdSmbusBaseAddress));

  MmioOr8 (SmbusRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE);

  ///
  /// Reset the SMBus host controller
  ///
  MmioOr8 (SmbusRegBase + R_PCH_SMBUS_HOSTC, B_PCH_SMBUS_HOSTC_SSRESET);

  ///
  /// Enable the SMBus host controller
  ///
  MmioAndThenOr8 (
    SmbusRegBase + R_PCH_SMBUS_HOSTC,
    (UINT8)(~(B_PCH_SMBUS_HOSTC_SMI_EN | B_PCH_SMBUS_HOSTC_I2C_EN)),
    B_PCH_SMBUS_HOSTC_HST_EN
    );

  ///
  /// Clear Status Register before anyone uses the interfaces
  ///
  IoWrite8 (PcdGet16(PcdSmbusBaseAddress) + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);

  return EFI_SUCCESS;
}

/**
  Update Spd Data

  @param[in][out] FspmUpd              Pointer to FSP UPD Data.
  @param[in]      MemConfigNoCrc       Pointer to Mem Config No Crc.
  @param[in]      MiscPeiPreMemConfig  Pointer to Misc Config.

  @retval         EFI_SUCCESS          The function completes successfully
  @retval         Other                The function fail
**/
VOID
EFIAPI
InternalUpdateSpdData (
  IN OUT FSPM_UPD               *FspmUpd,
  IN MEMORY_CONFIG_NO_CRC       *MemConfigNoCrc,
  IN SA_MISC_PEI_PREMEM_CONFIG  *MiscPeiPreMemConfig
  )
{
  UINT8                     Socket;
  UINT8                     *SpdData;

  InternalInitializePchSmbus ();

  DEBUG ((DEBUG_INFO, "Updating UPD:Memory Spd Pointers...\n"));
  if (FspmUpd == NULL || MemConfigNoCrc == NULL) {
    DEBUG ((DEBUG_ERROR, "EFI_INVALID_PARAMETER.\n"));
    DEBUG ((DEBUG_ERROR, "Fail to access SPD from SiPolicyPpi\n"));
    return;
  }

  if (*((UINT32 *)MiscPeiPreMemConfig->SpdAddressTable) != 0x0) {
    //
    // Update MemConfigNoCrc->SpdData->SpdData
    //
    for (Socket = 0; Socket < SA_MC_MAX_SOCKETS; Socket++) {
      SpdData = (UINT8 *)((UINT32)MemConfigNoCrc->SpdData->SpdData + (Socket * SA_MC_MAX_SPD_SIZE));
      InternalGetSpdData (
        0,
        MiscPeiPreMemConfig->SpdAddressTable[Socket],
        (UINT8 *)SpdData,
        (UINT8 *)&mSpdDdr3Table,
        sizeof(mSpdDdr3Table),
        (UINT8 *)&mSpdDdr4Table,
        sizeof(mSpdDdr4Table),
        (UINT8 *)&mSpdLpddrTable,
        sizeof(mSpdLpddrTable)
        );
    }
  }

  FspmUpd->FspmConfig.MemorySpdPtr00 = (UINT32)MemConfigNoCrc->SpdData->SpdData;
  FspmUpd->FspmConfig.MemorySpdPtr01 = (UINT32)MemConfigNoCrc->SpdData->SpdData + (1 * SA_MC_MAX_SPD_SIZE);
  FspmUpd->FspmConfig.MemorySpdPtr10 = (UINT32)MemConfigNoCrc->SpdData->SpdData + (2 * SA_MC_MAX_SPD_SIZE);
  FspmUpd->FspmConfig.MemorySpdPtr11 = (UINT32)MemConfigNoCrc->SpdData->SpdData + (3 * SA_MC_MAX_SPD_SIZE);

  DEBUG ((DEBUG_INFO, "UPD:MemorySpdPtr Updated\n"));
}

/**
  Performs FSP SA PEI Policy initialization in pre-memory.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                  Status;
  SA_MISC_PEI_PREMEM_CONFIG   *MiscPeiPreMemConfig;
  MEMORY_CONFIGURATION        *MemConfig;
  MEMORY_CONFIG_NO_CRC        *MemConfigNoCrc;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;

  //
  // Locate SiPreMemPolicyPpi
  //
  SiPreMemPolicyPpi = NULL;
  MiscPeiPreMemConfig = NULL;
  MemConfig = NULL;
  MemConfigNoCrc = NULL;


  Status = PeiServicesLocatePpi(
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) == FALSE) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
    ASSERT_EFI_ERROR (Status);
 }

  //
  // Update UPD:DqPinsInterleaved
  //
  FspmUpd->FspmConfig.DqPinsInterleaved = (UINT8)MemConfig->DqPinsInterleaved;

  //
  // Update UPD:DqPinsInterleaved
  //
  FspmUpd->FspmConfig.CaVrefConfig  = MemConfig->CaVrefConfig;

  //
  // Update UPD:MemorySpdPtrXX
  //
  InternalUpdateSpdData (FspmUpd, MemConfigNoCrc, MiscPeiPreMemConfig);

  //
  // Update UPD:MemorySpdDataLen
  //
  FspmUpd->FspmConfig.MemorySpdDataLen = SA_MC_MAX_SPD_SIZE;

  //
  // Update UPD:PlatformMemorySize
  //
  //
  // @todo: This value is used since #183932. Revisit.
  //
  FspmUpd->FspmConfig.PlatformMemorySize = MemConfigNoCrc->PlatformMemorySize;
  FspmUpd->FspmConfig.SaGv = MemConfig->SaGv;
  FspmUpd->FspmConfig.RMT = (UINT8) MemConfig->RMT;
  FspmUpd->FspmConfig.DdrFreqLimit = MemConfig->DdrFreqLimit;

  FspmUpd->FspmConfig.SpdProfileSelected = MemConfig->SpdProfileSelected;
  FspmUpd->FspmConfig.VddVoltage         = MemConfig->VddVoltage;
  FspmUpd->FspmConfig.RefClk             = MemConfig->RefClk;
  FspmUpd->FspmConfig.Ratio              = MemConfig->Ratio;
  FspmUpd->FspmConfig.OddRatioMode       = (UINT8) MemConfig->OddRatioMode;
  FspmUpd->FspmConfig.tCL                = (UINT8) MemConfig->tCL;
  FspmUpd->FspmConfig.tCWL               = (UINT8) MemConfig->tCWL;
  FspmUpd->FspmConfig.tFAW               = MemConfig->tFAW;
  FspmUpd->FspmConfig.tRAS               = MemConfig->tRAS;
  FspmUpd->FspmConfig.tRCDtRP            = (UINT8) MemConfig->tRCDtRP;
  FspmUpd->FspmConfig.tREFI              = MemConfig->tREFI;
  FspmUpd->FspmConfig.tRFC               = MemConfig->tRFC;
  FspmUpd->FspmConfig.tRRD               = (UINT8) MemConfig->tRRD;
  FspmUpd->FspmConfig.tRTP               = (UINT8) MemConfig->tRTP;
  FspmUpd->FspmConfig.tWR                = (UINT8) MemConfig->tWR;
  FspmUpd->FspmConfig.tWTR               = (UINT8) MemConfig->tWTR;
  FspmUpd->FspmConfig.NModeSupport       = MemConfig->NModeSupport;
  FspmUpd->FspmConfig.DllBwEn0           = MemConfig->DllBwEn0;
  FspmUpd->FspmConfig.DllBwEn1           = MemConfig->DllBwEn1;
  FspmUpd->FspmConfig.DllBwEn2           = MemConfig->DllBwEn2;
  FspmUpd->FspmConfig.DllBwEn3           = MemConfig->DllBwEn3;
  FspmUpd->FspmConfig.EvLoader           = (UINT8) MemConfig->EvLoader;

  //
  // Update UPD:SmramMask
  //
  if (MemConfig != NULL) {
    FspmUpd->FspmConfig.SmramMask = MemConfig->SmramMask;
  }

  return EFI_SUCCESS;
}

/**
  Performs FSP SA PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
  GRAPHICS_PEI_CONFIG       *GtConfig;
  VTD_CONFIG                *Vtd;
  //
  // Locate SiPolicyPpi
  //
  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi(
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiPolicyPpi
             );
  if ((Status == EFI_SUCCESS) && (SiPolicyPpi != NULL)) {
    GtConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &GtConfig);
    ASSERT_EFI_ERROR (Status);

    Vtd = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gVtdConfigGuid, (VOID *) &Vtd);
    ASSERT_EFI_ERROR (Status);

  }

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiSaPolicy\n"));
  if (GtConfig != NULL) {
    //
    // For FSP, FspsUpd->FspsConfig.PeiGraphicsPeimInit is always enabled as default.
    //
    FspsUpd->FspsConfig.PeiGraphicsPeimInit = (UINT8) GtConfig->PeiGraphicsPeimInit; // SA: InternalOnly: For Internal validation we still need to enable both Enable/Disable Cases

    //
    // Update UPD: VBT & LogoPtr
    //
    FspsUpd->FspsConfig.GraphicsConfigPtr = (UINT32) GtConfig->GraphicsConfigPtr;
    DEBUG(( DEBUG_INFO, "VbtPtr from GraphicsPeiConfig is 0x%x\n", FspsUpd->FspsConfig.GraphicsConfigPtr));

    FspsUpd->FspsConfig.LogoPtr  = (UINT32) GtConfig->LogoPtr;
    FspsUpd->FspsConfig.LogoSize = GtConfig->LogoSize;
    DEBUG(( DEBUG_INFO, "LogoPtr from PeiFspSaPolicyInit GraphicsPeiConfig is 0x%x\n", FspsUpd->FspsConfig.LogoPtr));
    DEBUG(( DEBUG_INFO, "LogoSize from PeiFspSaPolicyInit GraphicsPeiConfig is 0x%x\n", FspsUpd->FspsConfig.LogoSize));



  }
  if (Vtd != NULL) {
    FspsUpd->FspsConfig.X2ApicOptOut = (UINT8) Vtd->X2ApicOptOut;
    FspsUpd->FspsConfig.VtdBaseAddress[0] = Vtd->BaseAddress[0];
    FspsUpd->FspsConfig.VtdBaseAddress[1] = Vtd->BaseAddress[1];
    FspsUpd->FspsTestConfig.VtdDisable = (UINT8) Vtd->VtdDisable;
  }
  return EFI_SUCCESS;
}
