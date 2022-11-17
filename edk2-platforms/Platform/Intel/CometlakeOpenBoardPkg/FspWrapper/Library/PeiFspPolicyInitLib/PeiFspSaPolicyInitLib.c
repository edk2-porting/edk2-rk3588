/** @file
  Implementation of Fsp SA Policy Initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <ConfigBlock/MemoryConfig.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmbusLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PcdLib.h>

#include <IndustryStandard/Pci.h>
#include <PchAccess.h>
#include <Ppi/FirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiPeiCis.h>
#include <Core/Pei/PeiMain.h>
#include <Library/ConfigBlockLib.h>

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
  {  39,              59,             (1 << SpdCold),},
  {  64,             125,             (1 << SpdCold),},
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
  {  32,             119,             (1 << SpdCold),},
  { 126,             255,             (1 << SpdCold),},
  { 349,             383,             (1 << SpdCold),},
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
  {  31,             121,               (1 << SpdCold),},
  { 126,             255,               (1 << SpdCold),},
  { 349,             383,               (1 << SpdCold),},
  { 384,             387,               (1 << SpdCold),},
  { 388,             389,               (1 << SpdCold),},
  { 393,             431,               (1 << SpdCold),},
  { 440,             478,               (1 << SpdCold),},
};


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
InternalUpdateSpdInfo (
  IN OUT FSPM_UPD               *FspmUpd,
  IN MEMORY_CONFIG_NO_CRC       *MemConfigNoCrc,
  IN SA_MISC_PEI_PREMEM_CONFIG  *MiscPeiPreMemConfig
  )
{

  DEBUG ((DEBUG_INFO, "Updating UPD:Memory Spd Pointers...\n"));
  if ((FspmUpd == NULL) || (MemConfigNoCrc == NULL) || (MiscPeiPreMemConfig == NULL)) {
    DEBUG ((DEBUG_ERROR, "EFI_INVALID_PARAMETER.\n"));
    DEBUG ((DEBUG_ERROR, "Fail to access SPD from SiPolicyPpi\n"));
    return;
  }

  //
  // Update MemorySpdPtrXX if SpdAddressTable is zero
  //
  if (MiscPeiPreMemConfig->SpdAddressTable[0] == 0x0) {
    FspmUpd->FspmConfig.MemorySpdPtr00 = (UINT32)MemConfigNoCrc->SpdData->SpdData;
  } else {
    FspmUpd->FspmConfig.SpdAddressTable[0] = MiscPeiPreMemConfig->SpdAddressTable[0];
  }

  if (MiscPeiPreMemConfig->SpdAddressTable[1] == 0x0) {
    FspmUpd->FspmConfig.MemorySpdPtr01 = (UINT32)MemConfigNoCrc->SpdData->SpdData + (1 * SA_MC_MAX_SPD_SIZE);
  } else {
    FspmUpd->FspmConfig.SpdAddressTable[1] = MiscPeiPreMemConfig->SpdAddressTable[1];
  }

  if (MiscPeiPreMemConfig->SpdAddressTable[2] == 0x0) {
    FspmUpd->FspmConfig.MemorySpdPtr10 = (UINT32)MemConfigNoCrc->SpdData->SpdData + (2 * SA_MC_MAX_SPD_SIZE);
  } else {
    FspmUpd->FspmConfig.SpdAddressTable[2] = MiscPeiPreMemConfig->SpdAddressTable[2];
  }

  if (MiscPeiPreMemConfig->SpdAddressTable[3] == 0x0) {
    FspmUpd->FspmConfig.MemorySpdPtr11 = (UINT32)MemConfigNoCrc->SpdData->SpdData + (3 * SA_MC_MAX_SPD_SIZE);
  } else {
    FspmUpd->FspmConfig.SpdAddressTable[3] = MiscPeiPreMemConfig->SpdAddressTable[3];
  }

  DEBUG ((DEBUG_INFO, "UPD:MemorySpdPtr Updated\n"));
}

/**
  PeiGetSectionFromFv finds the file in FV and gets file Address and Size

  @param[in] NameGuid              - File GUID
  @param[out] Address              - Pointer to the File Address
  @param[out] Size                 - Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32               *Size
  )
{
  EFI_STATUS  Status;
  EFI_PEI_FIRMWARE_VOLUME_PPI          *FvPpi;
  EFI_FV_FILE_INFO                     FvFileInfo;
  PEI_CORE_INSTANCE                    *PrivateData;
  UINTN                                CurrentFv;
  PEI_CORE_FV_HANDLE                   *CoreFvHandle;
  EFI_PEI_FILE_HANDLE                  VbtFileHandle;
  EFI_GUID                             *VbtGuid;
  EFI_COMMON_SECTION_HEADER            *Section;
  CONST EFI_PEI_SERVICES               **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  PrivateData = PEI_CORE_INSTANCE_FROM_PS_THIS(PeiServices);

  Status = PeiServicesLocatePpi (
            &gEfiFirmwareFileSystem2Guid,
            0,
            NULL,
            (VOID **) &FvPpi
            );
  ASSERT_EFI_ERROR (Status);

  CurrentFv = PrivateData->CurrentPeimFvCount;
  CoreFvHandle = &(PrivateData->Fv[CurrentFv]);

  Status = FvPpi->FindFileByName (FvPpi, &NameGuid, &CoreFvHandle->FvHandle, &VbtFileHandle);
  if (!EFI_ERROR(Status) && VbtFileHandle != NULL) {

  DEBUG ((DEBUG_INFO, "Find SectionByType \n"));

    Status = FvPpi->FindSectionByType (FvPpi, EFI_SECTION_RAW, VbtFileHandle, (VOID **) &VbtGuid);
    if (!EFI_ERROR (Status)) {

    DEBUG ((DEBUG_INFO, "GetFileInfo \n"));

      Status = FvPpi->GetFileInfo (FvPpi, VbtFileHandle, &FvFileInfo);
      Section = (EFI_COMMON_SECTION_HEADER *)FvFileInfo.Buffer;

      if (IS_SECTION2 (Section)) {
        ASSERT (SECTION2_SIZE (Section) > 0x00FFFFFF);
        *Size = SECTION2_SIZE (Section) - sizeof (EFI_COMMON_SECTION_HEADER2);
        *Address = ((UINT8 *)Section + sizeof (EFI_COMMON_SECTION_HEADER2));
      } else {
        *Size = SECTION_SIZE (Section) - sizeof (EFI_COMMON_SECTION_HEADER);
        *Address = ((UINT8 *)Section + sizeof (EFI_COMMON_SECTION_HEADER));
      }
    }
  }

  return EFI_SUCCESS;
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
  PCIE_PEI_PREMEM_CONFIG      *PciePeiPreMemConfig;
  SWITCHABLE_GRAPHICS_CONFIG  *SgGpioData;
  GRAPHICS_PEI_PREMEM_CONFIG  *GtPreMemConfig;
  OVERCLOCKING_PREMEM_CONFIG  *OcPreMemConfig;
  VTD_CONFIG                  *Vtd;
  IPU_PREMEM_CONFIG           *IpuPreMemPolicy;
  UINT8                       Index;
  VOID                        *Buffer;

  SiPreMemPolicyPpi   = NULL;
  MiscPeiPreMemConfig = NULL;
  MemConfig           = NULL;
  MemConfigNoCrc      = NULL;
  PciePeiPreMemConfig = NULL;
  SgGpioData          = NULL;
  GtPreMemConfig      = NULL;
  OcPreMemConfig      = NULL;
  Vtd                 = NULL;
  IpuPreMemPolicy     = NULL;



  //
  // Locate SiPreMemPolicyPpi
  //
  Status = PeiServicesLocatePpi(
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status == EFI_SUCCESS) && (SiPreMemPolicyPpi != NULL)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &GtPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaPciePeiPreMemConfigGuid, (VOID *) &PciePeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSwitchableGraphicsConfigGuid, (VOID *) &SgGpioData);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &Vtd);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaOverclockingPreMemConfigGuid, (VOID *) &OcPreMemConfig);
    ASSERT_EFI_ERROR (Status);

  }

  DEBUG((DEBUG_INFO, "Updating Dq Byte Map and DQS Byte Swizzling Settings...\n"));
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqByteMap);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqByteMapCh0, Buffer, 12);
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqByteMapCh1, (UINT8*) Buffer + 12, 12);
  }
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqsMapCpu2Dram);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqsMapCpu2DramCh0, Buffer, 8);
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqsMapCpu2DramCh1, (UINT8*) Buffer + 8, 8);
  }

  DEBUG((DEBUG_INFO, "Updating Dq Pins Interleaved,Rcomp Resistor & Rcomp Target Settings...\n"));
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompResistor);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.RcompResistor, Buffer, 6);
  }
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompTarget);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.RcompTarget, Buffer, 10);
  }

  //
  // Update UPD:MemorySpdPtrXX and SpdAddressTable
  //
  InternalUpdateSpdInfo (FspmUpd, MemConfigNoCrc, MiscPeiPreMemConfig);

  //
  // Update UPD:MemorySpdDataLen
  //
  FspmUpd->FspmConfig.MemorySpdDataLen = SA_MC_MAX_SPD_SIZE;

  if (MemConfigNoCrc != NULL) {
    //
    // Update UPD:PlatformMemorySize
    //
    //
    // @todo: This value is used since #183932. Revisit.
    //
    FspmUpd->FspmConfig.PlatformMemorySize  = MemConfigNoCrc->PlatformMemorySize;
    FspmUpd->FspmConfig.CleanMemory         = (UINT8) MemConfigNoCrc->CleanMemory;
    FspmUpd->FspmConfig.MemTestOnWarmBoot   = (UINT8) MemConfigNoCrc->MemTestOnWarmBoot;
  }

  if (MemConfig != NULL) {
    //
    // Update UPD:DqPinsInterleaved
    //
    FspmUpd->FspmConfig.DqPinsInterleaved     = (UINT8) MemConfig->DqPinsInterleaved;

    FspmUpd->FspmConfig.ProbelessTrace        = MemConfig->ProbelessTrace;
    FspmUpd->FspmConfig.GdxcIotSize           = MemConfig->GdxcIotSize;
    FspmUpd->FspmConfig.GdxcMotSize           = MemConfig->GdxcMotSize;
    FspmUpd->FspmConfig.DualDimmPerChannelBoardType =(UINT8) MemConfig->DualDimmPerChannelBoardType;
    FspmUpd->FspmConfig.Ddr4Mixed2DpcLimit    =(UINT8) MemConfig->Ddr4MixedUDimm2DpcLimit;
    //
    // Update UPD:CaVrefConfig
    //
    FspmUpd->FspmConfig.CaVrefConfig          = MemConfig->CaVrefConfig;
    FspmUpd->FspmConfig.SaGv                  = MemConfig->SaGv;
    FspmUpd->FspmConfig.FreqSaGvLow           = MemConfig->FreqSaGvLow;
    FspmUpd->FspmConfig.RMT                   = (UINT8) MemConfig->RMT;
    FspmUpd->FspmConfig.DdrFreqLimit          = MemConfig->DdrFreqLimit;

    FspmUpd->FspmConfig.SpdProfileSelected    = MemConfig->SpdProfileSelected;
    FspmUpd->FspmConfig.VddVoltage            = MemConfig->VddVoltage;
    FspmUpd->FspmConfig.RefClk                = MemConfig->RefClk;
    FspmUpd->FspmConfig.Ratio                 = MemConfig->Ratio;
    FspmUpd->FspmConfig.OddRatioMode          = (UINT8) MemConfig->OddRatioMode;
    FspmUpd->FspmConfig.tCL                   = (UINT8) MemConfig->tCL;
    FspmUpd->FspmConfig.tCWL                  = (UINT8) MemConfig->tCWL;
    FspmUpd->FspmConfig.tFAW                  = MemConfig->tFAW;
    FspmUpd->FspmConfig.tRAS                  = MemConfig->tRAS;
    FspmUpd->FspmConfig.tRCDtRP               = (UINT8) MemConfig->tRCDtRP;
    FspmUpd->FspmConfig.tREFI                 = MemConfig->tREFI;
    FspmUpd->FspmConfig.tRFC                  = MemConfig->tRFC;
    FspmUpd->FspmConfig.tRRD                  = (UINT8) MemConfig->tRRD;
    FspmUpd->FspmConfig.tRTP                  = (UINT8) MemConfig->tRTP;
    FspmUpd->FspmConfig.tWR                   = (UINT8) MemConfig->tWR;
    FspmUpd->FspmConfig.tWTR                  = (UINT8) MemConfig->tWTR;
    FspmUpd->FspmConfig.NModeSupport          = MemConfig->NModeSupport;
    FspmUpd->FspmConfig.DllBwEn0              = MemConfig->DllBwEn0;
    FspmUpd->FspmConfig.DllBwEn1              = MemConfig->DllBwEn1;
    FspmUpd->FspmConfig.DllBwEn2              = MemConfig->DllBwEn2;
    FspmUpd->FspmConfig.DllBwEn3              = MemConfig->DllBwEn3;
    FspmUpd->FspmConfig.MrcSafeConfig         = (UINT8) MemConfig->MrcSafeConfig; // Typecasting as MrcSafeConfig is of UINT32 in MEMORY_CONFIGURATION
    FspmUpd->FspmConfig.LpDdrDqDqsReTraining  = (UINT8) MemConfig->Lp4DqsOscEn;
    FspmUpd->FspmConfig.RmtPerTask            = (UINT8) MemConfig->RmtPerTask;
    FspmUpd->FspmConfig.TrainTrace            = (UINT8) MemConfig->TrainTrace;
    FspmUpd->FspmConfig.ScramblerSupport      = (UINT8) MemConfig->ScramblerSupport;
    FspmUpd->FspmConfig.SafeMode              = (UINT8) MemConfig->SafeMode;

    //
    // Update UPD:SmramMask and DisableDimmChannel
    //
    FspmUpd->FspmConfig.SmramMask               = MemConfig->SmramMask;
    FspmUpd->FspmConfig.DisableDimmChannel0     = MemConfig->DisableDimmChannel[0];
    FspmUpd->FspmConfig.DisableDimmChannel1     = MemConfig->DisableDimmChannel[1];
    FspmUpd->FspmConfig.HobBufferSize           = MemConfig->HobBufferSize;

    FspmUpd->FspmConfig.ECT                     = (UINT8) MemConfig->ECT;
    FspmUpd->FspmConfig.SOT                     = (UINT8) MemConfig->SOT;
    FspmUpd->FspmConfig.ERDMPRTC2D              = (UINT8) MemConfig->ERDMPRTC2D;
    FspmUpd->FspmConfig.RDMPRT                  = (UINT8) MemConfig->RDMPRT;
    FspmUpd->FspmConfig.RCVET                   = (UINT8) MemConfig->RCVET;
    FspmUpd->FspmConfig.JWRL                    = (UINT8) MemConfig->JWRL;
    FspmUpd->FspmConfig.EWRTC2D                 = (UINT8) MemConfig->EWRTC2D;
    FspmUpd->FspmConfig.ERDTC2D                 = (UINT8) MemConfig->ERDTC2D;
    FspmUpd->FspmConfig.WRTC1D                  = (UINT8) MemConfig->WRTC1D;
    FspmUpd->FspmConfig.WRVC1D                  = (UINT8) MemConfig->WRVC1D;
    FspmUpd->FspmConfig.RDTC1D                  = (UINT8) MemConfig->RDTC1D;
    FspmUpd->FspmConfig.DIMMODTT                = (UINT8) MemConfig->DIMMODTT;
    FspmUpd->FspmConfig.DIMMRONT                = (UINT8) MemConfig->DIMMRONT;
    FspmUpd->FspmConfig.WRSRT                   = (UINT8) MemConfig->WRSRT;
    FspmUpd->FspmConfig.RDODTT                  = (UINT8) MemConfig->RDODTT;
    FspmUpd->FspmConfig.RDEQT                   = (UINT8) MemConfig->RDEQT;
    FspmUpd->FspmConfig.RDAPT                   = (UINT8) MemConfig->RDAPT;
    FspmUpd->FspmConfig.WRTC2D                  = (UINT8) MemConfig->WRTC2D;
    FspmUpd->FspmConfig.RDTC2D                  = (UINT8) MemConfig->RDTC2D;
    FspmUpd->FspmConfig.WRVC2D                  = (UINT8) MemConfig->WRVC2D;
    FspmUpd->FspmConfig.RDVC2D                  = (UINT8) MemConfig->RDVC2D;
    FspmUpd->FspmConfig.CMDVC                   = (UINT8) MemConfig->CMDVC;
    FspmUpd->FspmConfig.LCT                     = (UINT8) MemConfig->LCT;
    FspmUpd->FspmConfig.RTL                     = (UINT8) MemConfig->RTL;
    FspmUpd->FspmConfig.TAT                     = (UINT8) MemConfig->TAT;
    FspmUpd->FspmConfig.RCVENC1D                = (UINT8) MemConfig->RCVENC1D;
    FspmUpd->FspmConfig.RMT                     = (UINT8) MemConfig->RMT;
    FspmUpd->FspmConfig.MEMTST                  = (UINT8) MemConfig->MEMTST;
    FspmUpd->FspmConfig.ALIASCHK                = (UINT8) MemConfig->ALIASCHK;
    FspmUpd->FspmConfig.RMC                     = (UINT8) MemConfig->RMC;
    FspmUpd->FspmConfig.WRDSUDT                 = (UINT8) MemConfig->WRDSUDT;
    FspmUpd->FspmConfig.EnBER                   = (UINT8) MemConfig->EnBER;
    FspmUpd->FspmConfig.EccSupport              = (UINT8) MemConfig->EccSupport;
    FspmUpd->FspmConfig.RemapEnable             = (UINT8) MemConfig->RemapEnable;
    FspmUpd->FspmConfig.ScramblerSupport        = (UINT8) MemConfig->ScramblerSupport;
    FspmUpd->FspmConfig.MrcFastBoot             = (UINT8) MemConfig->MrcFastBoot;
    FspmUpd->FspmConfig.RankInterleave          = (UINT8) MemConfig->RankInterleave;
    FspmUpd->FspmConfig.EnhancedInterleave      = (UINT8) MemConfig->EnhancedInterleave;
    FspmUpd->FspmConfig.MemoryTrace             = (UINT8) MemConfig->MemoryTrace;
    FspmUpd->FspmConfig.ChHashEnable            = (UINT8) MemConfig->ChHashEnable;
    FspmUpd->FspmConfig.EnableExtts             = (UINT8) MemConfig->EnableExtts;
    FspmUpd->FspmConfig.EnableCltm              = (UINT8) MemConfig->EnableCltm;
    FspmUpd->FspmConfig.EnableOltm              = (UINT8) MemConfig->EnableOltm;
    FspmUpd->FspmConfig.EnablePwrDn             = (UINT8) MemConfig->EnablePwrDn;
    FspmUpd->FspmConfig.EnablePwrDnLpddr        = (UINT8) MemConfig->EnablePwrDnLpddr;
    FspmUpd->FspmConfig.UserPowerWeightsEn      = (UINT8) MemConfig->UserPowerWeightsEn;
    FspmUpd->FspmConfig.RaplLim2Lock            = (UINT8) MemConfig->RaplLim2Lock;
    FspmUpd->FspmConfig.RaplLim2Ena             = (UINT8) MemConfig->RaplLim2Ena;
    FspmUpd->FspmConfig.RaplLim1Ena             = (UINT8) MemConfig->RaplLim1Ena;
    FspmUpd->FspmConfig.SrefCfgEna              = (UINT8) MemConfig->SrefCfgEna;
    FspmUpd->FspmConfig.ThrtCkeMinDefeatLpddr   = (UINT8) MemConfig->ThrtCkeMinDefeatLpddr;
    FspmUpd->FspmConfig.ThrtCkeMinDefeat        = (UINT8) MemConfig->ThrtCkeMinDefeat;
    FspmUpd->FspmConfig.RhPrevention            = (UINT8) MemConfig->RhPrevention;
    FspmUpd->FspmConfig.ExitOnFailure           = (UINT8) MemConfig->ExitOnFailure;
    FspmUpd->FspmConfig.DdrThermalSensor        = (UINT8) MemConfig->DdrThermalSensor;
    FspmUpd->FspmConfig.Ddr4DdpSharedClock      = (UINT8) MemConfig->Ddr4DdpSharedClock;
    FspmUpd->FspmConfig.Ddr4DdpSharedZq         = (UINT8) MemConfig->SharedZqPin;
    FspmUpd->FspmConfig.BClkFrequency           = MemConfig->BClkFrequency;
    FspmUpd->FspmConfig.ChHashInterleaveBit     = MemConfig->ChHashInterleaveBit;
    FspmUpd->FspmConfig.ChHashMask              = MemConfig->ChHashMask;
    FspmUpd->FspmConfig.EnergyScaleFact         = MemConfig->EnergyScaleFact;
    FspmUpd->FspmConfig.Idd3n                   = MemConfig->Idd3n;
    FspmUpd->FspmConfig.Idd3p                   = MemConfig->Idd3p;
    FspmUpd->FspmConfig.CMDSR                   = (UINT8) MemConfig->CMDSR;
    FspmUpd->FspmConfig.CMDDSEQ                 = (UINT8) MemConfig->CMDDSEQ;
    FspmUpd->FspmConfig.CMDNORM                 = (UINT8) MemConfig->CMDNORM;
    FspmUpd->FspmConfig.EWRDSEQ                 = (UINT8) MemConfig->EWRDSEQ;
    FspmUpd->FspmConfig.FreqSaGvLow             = MemConfig->FreqSaGvLow;
    FspmUpd->FspmConfig.RhActProbability        = MemConfig->RhActProbability;
    FspmUpd->FspmConfig.RaplLim2WindX           = MemConfig->RaplLim2WindX;
    FspmUpd->FspmConfig.RaplLim2WindY           = MemConfig->RaplLim2WindY;
    FspmUpd->FspmConfig.RaplLim1WindX           = MemConfig->RaplLim1WindX;
    FspmUpd->FspmConfig.RaplLim1WindY           = MemConfig->RaplLim1WindY;
    FspmUpd->FspmConfig.RaplLim2Pwr             = MemConfig->RaplLim2Pwr;
    FspmUpd->FspmConfig.RaplLim1Pwr             = MemConfig->RaplLim1Pwr;
    FspmUpd->FspmConfig.WarmThresholdCh0Dimm0   = MemConfig->WarmThresholdCh0Dimm0;
    FspmUpd->FspmConfig.WarmThresholdCh0Dimm1   = MemConfig->WarmThresholdCh0Dimm1;
    FspmUpd->FspmConfig.WarmThresholdCh1Dimm0   = MemConfig->WarmThresholdCh1Dimm0;
    FspmUpd->FspmConfig.WarmThresholdCh1Dimm1   = MemConfig->WarmThresholdCh1Dimm1;
    FspmUpd->FspmConfig.HotThresholdCh0Dimm0    = MemConfig->HotThresholdCh0Dimm0;
    FspmUpd->FspmConfig.HotThresholdCh0Dimm1    = MemConfig->HotThresholdCh0Dimm1;
    FspmUpd->FspmConfig.HotThresholdCh1Dimm0    = MemConfig->HotThresholdCh1Dimm0;
    FspmUpd->FspmConfig.HotThresholdCh1Dimm1    = MemConfig->HotThresholdCh1Dimm1;
    FspmUpd->FspmConfig.WarmBudgetCh0Dimm0      = MemConfig->WarmBudgetCh0Dimm0;
    FspmUpd->FspmConfig.WarmBudgetCh0Dimm1      = MemConfig->WarmBudgetCh0Dimm1;
    FspmUpd->FspmConfig.WarmBudgetCh1Dimm0      = MemConfig->WarmBudgetCh1Dimm0;
    FspmUpd->FspmConfig.WarmBudgetCh1Dimm1      = MemConfig->WarmBudgetCh1Dimm1;
    FspmUpd->FspmConfig.HotBudgetCh0Dimm0       = MemConfig->HotBudgetCh0Dimm0;
    FspmUpd->FspmConfig.HotBudgetCh0Dimm1       = MemConfig->HotBudgetCh0Dimm1;
    FspmUpd->FspmConfig.HotBudgetCh1Dimm0       = MemConfig->HotBudgetCh1Dimm0;
    FspmUpd->FspmConfig.HotBudgetCh1Dimm1       = MemConfig->HotBudgetCh1Dimm1;
    FspmUpd->FspmConfig.IdleEnergyCh0Dimm0      = MemConfig->IdleEnergyCh0Dimm0;
    FspmUpd->FspmConfig.IdleEnergyCh0Dimm1      = MemConfig->IdleEnergyCh0Dimm1;
    FspmUpd->FspmConfig.IdleEnergyCh1Dimm0      = MemConfig->IdleEnergyCh1Dimm0;
    FspmUpd->FspmConfig.IdleEnergyCh1Dimm1      = MemConfig->IdleEnergyCh1Dimm1;
    FspmUpd->FspmConfig.PdEnergyCh0Dimm0        = MemConfig->PdEnergyCh0Dimm0;
    FspmUpd->FspmConfig.PdEnergyCh0Dimm1        = MemConfig->PdEnergyCh0Dimm1;
    FspmUpd->FspmConfig.PdEnergyCh1Dimm0        = MemConfig->PdEnergyCh1Dimm0;
    FspmUpd->FspmConfig.PdEnergyCh1Dimm1        = MemConfig->PdEnergyCh1Dimm1;
    FspmUpd->FspmConfig.ActEnergyCh0Dimm0       = MemConfig->ActEnergyCh0Dimm0;
    FspmUpd->FspmConfig.ActEnergyCh0Dimm1       = MemConfig->ActEnergyCh0Dimm1;
    FspmUpd->FspmConfig.ActEnergyCh1Dimm0       = MemConfig->ActEnergyCh1Dimm0;
    FspmUpd->FspmConfig.ActEnergyCh1Dimm1       = MemConfig->ActEnergyCh1Dimm1;
    FspmUpd->FspmConfig.RdEnergyCh0Dimm0        = MemConfig->RdEnergyCh0Dimm0;
    FspmUpd->FspmConfig.RdEnergyCh0Dimm1        = MemConfig->RdEnergyCh0Dimm1;
    FspmUpd->FspmConfig.RdEnergyCh1Dimm0        = MemConfig->RdEnergyCh1Dimm0;
    FspmUpd->FspmConfig.RdEnergyCh1Dimm1        = MemConfig->RdEnergyCh1Dimm1;
    FspmUpd->FspmConfig.WrEnergyCh0Dimm0        = MemConfig->WrEnergyCh0Dimm0;
    FspmUpd->FspmConfig.WrEnergyCh0Dimm1        = MemConfig->WrEnergyCh0Dimm1;
    FspmUpd->FspmConfig.WrEnergyCh1Dimm0        = MemConfig->WrEnergyCh1Dimm0;
    FspmUpd->FspmConfig.WrEnergyCh1Dimm1        = MemConfig->WrEnergyCh1Dimm1;
    FspmUpd->FspmConfig.ThrtCkeMinTmr           = MemConfig->ThrtCkeMinTmr;
    FspmUpd->FspmConfig.CkeRankMapping          = MemConfig->CkeRankMapping;
    FspmUpd->FspmConfig.CaVrefConfig            = MemConfig->CaVrefConfig;
    FspmUpd->FspmConfig.RaplPwrFlCh1            = MemConfig->RaplPwrFlCh1;
    FspmUpd->FspmConfig.RaplPwrFlCh0            = MemConfig->RaplPwrFlCh0;
    FspmUpd->FspmConfig.EnCmdRate               = MemConfig->EnCmdRate;
    FspmUpd->FspmConfig.Refresh2X               = MemConfig->Refresh2X;
    FspmUpd->FspmConfig.EpgEnable               = MemConfig->EpgEnable;
    FspmUpd->FspmConfig.RhSolution              = MemConfig->RhSolution;
    FspmUpd->FspmConfig.UserThresholdEnable     = MemConfig->UserThresholdEnable;
    FspmUpd->FspmConfig.UserBudgetEnable        = MemConfig->UserBudgetEnable;
    FspmUpd->FspmConfig.TsodTcritMax            = MemConfig->TsodTcritMax;
    FspmUpd->FspmConfig.TsodEventMode           = MemConfig->TsodEventMode;
    FspmUpd->FspmConfig.TsodEventPolarity       = MemConfig->TsodEventPolarity;
    FspmUpd->FspmConfig.TsodCriticalEventOnly   = MemConfig->TsodCriticalEventOnly;
    FspmUpd->FspmConfig.TsodEventOutputControl  = MemConfig->TsodEventOutputControl;
    FspmUpd->FspmConfig.TsodAlarmwindowLockBit  = MemConfig->TsodAlarmwindowLockBit;
    FspmUpd->FspmConfig.TsodCriticaltripLockBit = MemConfig->TsodCriticaltripLockBit;
    FspmUpd->FspmConfig.TsodShutdownMode        = MemConfig->TsodShutdownMode;
    FspmUpd->FspmConfig.TsodThigMax             = MemConfig->TsodThigMax;
    FspmUpd->FspmConfig.TsodManualEnable        = MemConfig->TsodManualEnable;
    FspmUpd->FspmConfig.IsvtIoPort              = MemConfig->IsvtIoPort;
    FspmUpd->FspmConfig.ForceOltmOrRefresh2x    = MemConfig->ForceOltmOrRefresh2x;
    FspmUpd->FspmConfig.PwdwnIdleCounter        = MemConfig->PwdwnIdleCounter;
    FspmUpd->FspmConfig.CmdRanksTerminated      = MemConfig->CmdRanksTerminated;
    FspmUpd->FspmConfig.GdxcEnable              = MemConfig->GdxcEnable;
    FspmUpd->FspmConfig.RMTLoopCount            = MemConfig->RMTLoopCount;

    // DDR4 Memory Timings
    FspmUpd->FspmTestConfig.tRRD_L = (UINT8) MemConfig->tRRD_L;
    FspmUpd->FspmTestConfig.tRRD_S = (UINT8) MemConfig->tRRD_S;
    FspmUpd->FspmTestConfig.tWTR_L = (UINT8) MemConfig->tWTR_L;
    FspmUpd->FspmTestConfig.tWTR_S = (UINT8) MemConfig->tWTR_S;

    // TurnAround Timing
    // Read-to-Read
    FspmUpd->FspmTestConfig.tRd2RdSG = MemConfig->tRd2RdSG;
    FspmUpd->FspmTestConfig.tRd2RdDG = MemConfig->tRd2RdDG;
    FspmUpd->FspmTestConfig.tRd2RdDR = MemConfig->tRd2RdDR;
    FspmUpd->FspmTestConfig.tRd2RdDD = MemConfig->tRd2RdDD;
    // Write-to-Read
    FspmUpd->FspmTestConfig.tWr2RdSG = MemConfig->tWr2RdSG;
    FspmUpd->FspmTestConfig.tWr2RdDG = MemConfig->tWr2RdDG;
    FspmUpd->FspmTestConfig.tWr2RdDR = MemConfig->tWr2RdDR;
    FspmUpd->FspmTestConfig.tWr2RdDD = MemConfig->tWr2RdDD;
    // Write-to-Write
    FspmUpd->FspmTestConfig.tWr2WrSG = MemConfig->tWr2WrSG;
    FspmUpd->FspmTestConfig.tWr2WrDG = MemConfig->tWr2WrDG;
    FspmUpd->FspmTestConfig.tWr2WrDR = MemConfig->tWr2WrDR;
    FspmUpd->FspmTestConfig.tWr2WrDD = MemConfig->tWr2WrDD;
    // Read-to-Write
    FspmUpd->FspmTestConfig.tRd2WrSG = MemConfig->tRd2WrSG;
    FspmUpd->FspmTestConfig.tRd2WrDG = MemConfig->tRd2WrDG;
    FspmUpd->FspmTestConfig.tRd2WrDR = MemConfig->tRd2WrDR;
    FspmUpd->FspmTestConfig.tRd2WrDD = MemConfig->tRd2WrDD;
  }

  if (MiscPeiPreMemConfig != NULL) {
    FspmUpd->FspmConfig.IedSize               = MiscPeiPreMemConfig->IedSize;
    FspmUpd->FspmConfig.UserBd                = MiscPeiPreMemConfig->UserBd;
    FspmUpd->FspmConfig.SgDelayAfterPwrEn     = MiscPeiPreMemConfig->SgDelayAfterPwrEn;
    FspmUpd->FspmConfig.SgDelayAfterHoldReset = MiscPeiPreMemConfig->SgDelayAfterHoldReset;
    FspmUpd->FspmConfig.MmioSize              = MiscPeiPreMemConfig->MmioSize;
    FspmUpd->FspmConfig.MmioSizeAdjustment    = MiscPeiPreMemConfig->MmioSizeAdjustment;
    FspmUpd->FspmConfig.TsegSize              = MiscPeiPreMemConfig->TsegSize;

    FspmUpd->FspmTestConfig.SkipExtGfxScan           = (UINT8) MiscPeiPreMemConfig->SkipExtGfxScan;
    FspmUpd->FspmTestConfig.BdatEnable               = (UINT8) MiscPeiPreMemConfig->BdatEnable;
    FspmUpd->FspmTestConfig.BdatTestType             = (UINT8) MiscPeiPreMemConfig->BdatTestType;
    FspmUpd->FspmTestConfig.ScanExtGfxForLegacyOpRom = (UINT8) MiscPeiPreMemConfig->ScanExtGfxForLegacyOpRom;
    FspmUpd->FspmTestConfig.LockPTMregs              = (UINT8) MiscPeiPreMemConfig->LockPTMregs;
  }

  if (Vtd != NULL) {
    FspmUpd->FspmConfig.X2ApicOptOut = (UINT8) Vtd->X2ApicOptOut;
    FspmUpd->FspmConfig.VtdBaseAddress[0] = Vtd->BaseAddress[0];
    FspmUpd->FspmConfig.VtdBaseAddress[1] = Vtd->BaseAddress[1];
    FspmUpd->FspmConfig.VtdBaseAddress[2] = Vtd->BaseAddress[2];
    FspmUpd->FspmTestConfig.VtdDisable = (UINT8) Vtd->VtdDisable;
  }

  if (PciePeiPreMemConfig != NULL) {
    FspmUpd->FspmConfig.DmiGen3ProgramStaticEq = (UINT8) PciePeiPreMemConfig->DmiGen3ProgramStaticEq;
    FspmUpd->FspmConfig.Peg0Enable = (UINT8) PciePeiPreMemConfig->Peg0Enable;
    FspmUpd->FspmConfig.Peg1Enable = (UINT8) PciePeiPreMemConfig->Peg1Enable;
    FspmUpd->FspmConfig.Peg2Enable = (UINT8) PciePeiPreMemConfig->Peg2Enable;
    FspmUpd->FspmConfig.Peg3Enable = (UINT8) PciePeiPreMemConfig->Peg3Enable;
    FspmUpd->FspmConfig.Peg0MaxLinkSpeed = (UINT8) PciePeiPreMemConfig->Peg0MaxLinkSpeed;
    FspmUpd->FspmConfig.Peg1MaxLinkSpeed = (UINT8) PciePeiPreMemConfig->Peg1MaxLinkSpeed;
    FspmUpd->FspmConfig.Peg2MaxLinkSpeed = (UINT8) PciePeiPreMemConfig->Peg2MaxLinkSpeed;
    FspmUpd->FspmConfig.Peg3MaxLinkSpeed = (UINT8) PciePeiPreMemConfig->Peg3MaxLinkSpeed;
    FspmUpd->FspmConfig.Peg0MaxLinkWidth = (UINT8) PciePeiPreMemConfig->Peg0MaxLinkWidth;
    FspmUpd->FspmConfig.Peg1MaxLinkWidth = (UINT8) PciePeiPreMemConfig->Peg1MaxLinkWidth;
    FspmUpd->FspmConfig.Peg2MaxLinkWidth = (UINT8) PciePeiPreMemConfig->Peg2MaxLinkWidth;
    FspmUpd->FspmConfig.Peg3MaxLinkWidth = (UINT8) PciePeiPreMemConfig->Peg3MaxLinkWidth;
    FspmUpd->FspmConfig.Peg0PowerDownUnusedLanes = (UINT8) PciePeiPreMemConfig->Peg0PowerDownUnusedLanes;
    FspmUpd->FspmConfig.Peg1PowerDownUnusedLanes = (UINT8) PciePeiPreMemConfig->Peg1PowerDownUnusedLanes;
    FspmUpd->FspmConfig.Peg2PowerDownUnusedLanes = (UINT8) PciePeiPreMemConfig->Peg2PowerDownUnusedLanes;
    FspmUpd->FspmConfig.Peg3PowerDownUnusedLanes = (UINT8) PciePeiPreMemConfig->Peg3PowerDownUnusedLanes;
    FspmUpd->FspmConfig.InitPcieAspmAfterOprom = (UINT8) PciePeiPreMemConfig->InitPcieAspmAfterOprom;
    FspmUpd->FspmConfig.PegDisableSpreadSpectrumClocking = (UINT8) PciePeiPreMemConfig->PegDisableSpreadSpectrumClocking;
    for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
      FspmUpd->FspmConfig.DmiGen3RootPortPreset[Index] = PciePeiPreMemConfig->DmiGen3RootPortPreset[Index];
      FspmUpd->FspmConfig.DmiGen3EndPointPreset[Index] = PciePeiPreMemConfig->DmiGen3EndPointPreset[Index];
      FspmUpd->FspmConfig.DmiGen3EndPointHint[Index] = PciePeiPreMemConfig->DmiGen3EndPointHint[Index];
    }
    for (Index = 0; Index < SA_DMI_MAX_BUNDLE; Index++) {
      FspmUpd->FspmConfig.DmiGen3RxCtlePeaking[Index] = PciePeiPreMemConfig->DmiGen3RxCtlePeaking[Index];
    }
    for (Index = 0; Index < SA_PEG_MAX_BUNDLE ; Index++) {
      FspmUpd->FspmConfig.PegGen3RxCtlePeaking[Index] = PciePeiPreMemConfig->PegGen3RxCtlePeaking[Index];
    }
    FspmUpd->FspmConfig.PegDataPtr = (UINT32) PciePeiPreMemConfig->PegDataPtr;
    CopyMem((VOID *)FspmUpd->FspmConfig.PegGpioData, &PciePeiPreMemConfig->PegGpioData, sizeof (PEG_GPIO_DATA));
    FspmUpd->FspmConfig.DmiDeEmphasis = PciePeiPreMemConfig->DmiDeEmphasis;

    for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
      FspmUpd->FspmConfig.PegRootPortHPE[Index] = PciePeiPreMemConfig->PegRootPortHPE[Index];
    }
    FspmUpd->FspmTestConfig.DmiMaxLinkSpeed     = (UINT8) PciePeiPreMemConfig->DmiMaxLinkSpeed;
    FspmUpd->FspmTestConfig.DmiGen3EqPh2Enable  = (UINT8) PciePeiPreMemConfig->DmiGen3EqPh2Enable;
    FspmUpd->FspmTestConfig.DmiGen3EqPh3Method  = (UINT8) PciePeiPreMemConfig->DmiGen3EqPh3Method;
    FspmUpd->FspmTestConfig.Peg0Gen3EqPh2Enable = (UINT8) PciePeiPreMemConfig->Peg0Gen3EqPh2Enable;
    FspmUpd->FspmTestConfig.Peg1Gen3EqPh2Enable = (UINT8) PciePeiPreMemConfig->Peg1Gen3EqPh2Enable;
    FspmUpd->FspmTestConfig.Peg2Gen3EqPh2Enable = (UINT8) PciePeiPreMemConfig->Peg2Gen3EqPh2Enable;
    FspmUpd->FspmTestConfig.Peg3Gen3EqPh2Enable = (UINT8) PciePeiPreMemConfig->Peg3Gen3EqPh2Enable;
    FspmUpd->FspmTestConfig.Peg0Gen3EqPh3Method = (UINT8) PciePeiPreMemConfig->Peg0Gen3EqPh3Method;
    FspmUpd->FspmTestConfig.Peg1Gen3EqPh3Method = (UINT8) PciePeiPreMemConfig->Peg1Gen3EqPh3Method;
    FspmUpd->FspmTestConfig.Peg2Gen3EqPh3Method = (UINT8) PciePeiPreMemConfig->Peg2Gen3EqPh3Method;
    FspmUpd->FspmTestConfig.Peg3Gen3EqPh3Method = (UINT8) PciePeiPreMemConfig->Peg3Gen3EqPh3Method;
    FspmUpd->FspmTestConfig.PegGen3ProgramStaticEq = (UINT8) PciePeiPreMemConfig->PegGen3ProgramStaticEq;
    FspmUpd->FspmTestConfig.Gen3SwEqAlwaysAttempt = (UINT8) PciePeiPreMemConfig->Gen3SwEqAlwaysAttempt;
    FspmUpd->FspmTestConfig.Gen3SwEqNumberOfPresets = (UINT8) PciePeiPreMemConfig->Gen3SwEqNumberOfPresets;
    FspmUpd->FspmTestConfig.Gen3SwEqEnableVocTest = (UINT8) PciePeiPreMemConfig->Gen3SwEqEnableVocTest;
    FspmUpd->FspmTestConfig.PegRxCemTestingMode = (UINT8) PciePeiPreMemConfig->PegRxCemTestingMode;
    FspmUpd->FspmTestConfig.PegRxCemLoopbackLane = (UINT8) PciePeiPreMemConfig->PegRxCemLoopbackLane;
    FspmUpd->FspmTestConfig.PegGenerateBdatMarginTable = (UINT8) PciePeiPreMemConfig->PegGenerateBdatMarginTable;
    FspmUpd->FspmTestConfig.PegRxCemNonProtocolAwareness = (UINT8) PciePeiPreMemConfig->PegRxCemNonProtocolAwareness;
    FspmUpd->FspmTestConfig.PegGen3RxCtleOverride = (UINT8) PciePeiPreMemConfig->PegGen3RxCtleOverride;
    for (Index = 0; Index < SA_PEG_MAX_LANE; Index++) {
      FspmUpd->FspmTestConfig.PegGen3RootPortPreset[Index] = PciePeiPreMemConfig->PegGen3RootPortPreset[Index];
      FspmUpd->FspmTestConfig.PegGen3EndPointPreset[Index] = PciePeiPreMemConfig->PegGen3EndPointPreset[Index];
      FspmUpd->FspmTestConfig.PegGen3EndPointHint[Index] = PciePeiPreMemConfig->PegGen3EndPointHint[Index];
    }
    FspmUpd->FspmTestConfig.Gen3SwEqJitterDwellTime = PciePeiPreMemConfig->Gen3SwEqJitterDwellTime;
    FspmUpd->FspmTestConfig.Gen3SwEqJitterErrorTarget = PciePeiPreMemConfig->Gen3SwEqJitterErrorTarget;
    FspmUpd->FspmTestConfig.Gen3SwEqVocDwellTime = PciePeiPreMemConfig->Gen3SwEqVocDwellTime;
    FspmUpd->FspmTestConfig.Gen3SwEqVocErrorTarget = PciePeiPreMemConfig->Gen3SwEqVocErrorTarget;
  }

  if (GtPreMemConfig != NULL) {
    FspmUpd->FspmConfig.PrimaryDisplay = (UINT8) GtPreMemConfig->PrimaryDisplay;
    FspmUpd->FspmConfig.InternalGfx = (UINT8) GtPreMemConfig->InternalGraphics;
    FspmUpd->FspmConfig.IgdDvmt50PreAlloc = (UINT8) GtPreMemConfig->IgdDvmt50PreAlloc;
    FspmUpd->FspmConfig.ApertureSize = (UINT8) GtPreMemConfig->ApertureSize;
    FspmUpd->FspmConfig.GttMmAdr = GtPreMemConfig->GttMmAdr;
    FspmUpd->FspmConfig.GmAdr = GtPreMemConfig->GmAdr;
    FspmUpd->FspmConfig.GttSize = GtPreMemConfig->GttSize;
    FspmUpd->FspmConfig.PsmiRegionSize = (UINT8) GtPreMemConfig->PsmiRegionSize;
    FspmUpd->FspmConfig.GtPsmiSupport = (UINT8)GtPreMemConfig->GtPsmiSupport;
    FspmUpd->FspmTestConfig.PanelPowerEnable = (UINT8) GtPreMemConfig->PanelPowerEnable;
    FspmUpd->FspmTestConfig.DeltaT12PowerCycleDelayPreMem = (UINT16) GtPreMemConfig->DeltaT12PowerCycleDelayPreMem;
  }

  if (SgGpioData != NULL) {
    CopyMem((VOID *) FspmUpd->FspmConfig.SaRtd3Pcie0Gpio, &SgGpioData->SaRtd3Pcie0Gpio, sizeof (SA_PCIE_RTD3_GPIO));
    CopyMem((VOID *) FspmUpd->FspmConfig.SaRtd3Pcie1Gpio, &SgGpioData->SaRtd3Pcie1Gpio, sizeof (SA_PCIE_RTD3_GPIO));
    CopyMem((VOID *) FspmUpd->FspmConfig.SaRtd3Pcie2Gpio, &SgGpioData->SaRtd3Pcie2Gpio, sizeof (SA_PCIE_RTD3_GPIO));
    FspmUpd->FspmConfig.RootPortIndex = SgGpioData->RootPortIndex;
  }

  if (IpuPreMemPolicy != NULL) {
    FspmUpd->FspmConfig.SaIpuEnable = (UINT8) IpuPreMemPolicy->SaIpuEnable;
    FspmUpd->FspmConfig.SaIpuImrConfiguration = (UINT8) IpuPreMemPolicy->SaIpuImrConfiguration;
  }

  if (OcPreMemConfig != NULL) {
    FspmUpd->FspmConfig.SaOcSupport = (UINT8) OcPreMemConfig->OcSupport;
    FspmUpd->FspmConfig.RealtimeMemoryTiming = (UINT8) OcPreMemConfig->RealtimeMemoryTiming;
    FspmUpd->FspmConfig.GtVoltageMode = (UINT8) OcPreMemConfig->GtVoltageMode;
    FspmUpd->FspmConfig.GtMaxOcRatio = OcPreMemConfig->GtMaxOcRatio;
    FspmUpd->FspmConfig.GtVoltageOffset = OcPreMemConfig->GtVoltageOffset;
    FspmUpd->FspmConfig.GtVoltageOverride = OcPreMemConfig->GtVoltageOverride;
    FspmUpd->FspmConfig.GtExtraTurboVoltage = OcPreMemConfig->GtExtraTurboVoltage;
    FspmUpd->FspmConfig.SaVoltageOffset = OcPreMemConfig->SaVoltageOffset;
    FspmUpd->FspmConfig.GtusMaxOcRatio = OcPreMemConfig->GtusMaxOcRatio;
    FspmUpd->FspmConfig.GtusVoltageMode = (UINT8) OcPreMemConfig->GtusVoltageMode;
    FspmUpd->FspmConfig.GtusVoltageOffset = OcPreMemConfig->GtusVoltageOffset;
    FspmUpd->FspmConfig.GtusVoltageOverride = OcPreMemConfig->GtusVoltageOverride;
    FspmUpd->FspmConfig.GtusExtraTurboVoltage = OcPreMemConfig->GtusExtraTurboVoltage;
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
  SA_MISC_PEI_CONFIG        *MiscPeiConfig;
  GRAPHICS_PEI_CONFIG       *GtConfig;
  PCIE_PEI_CONFIG           *PciePeiConfig;
  GNA_CONFIG                *GnaConfig;
  UINT8                     Index;
  EFI_BOOT_MODE             BootMode;

  MiscPeiConfig = NULL;
  GtConfig      = NULL;
  PciePeiConfig = NULL;
  GnaConfig     = NULL;

  //
  // @todo This could be cleared up after FSP provides ExitBootServices NotifyPhase.
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

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
    MiscPeiConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaMiscPeiConfigGuid, (VOID *) &MiscPeiConfig);
    ASSERT_EFI_ERROR (Status);

    GtConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &GtConfig);
    ASSERT_EFI_ERROR (Status);

    GnaConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGnaConfigGuid, (VOID *) &GnaConfig);
    ASSERT_EFI_ERROR (Status);

    PciePeiConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaPciePeiConfigGuid, (VOID *) &PciePeiConfig);
    ASSERT_EFI_ERROR (Status);

  }

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Wrapper UpdatePeiSaPolicy\n"));


  if (MiscPeiConfig != NULL) {
    FspsUpd->FspsConfig.Device4Enable = (UINT8) MiscPeiConfig->Device4Enable;
    FspsUpd->FspsConfig.CridEnable = (UINT8) MiscPeiConfig->CridEnable;
    FspsUpd->FspsTestConfig.ChapDeviceEnable = (UINT8) MiscPeiConfig->ChapDeviceEnable;
    FspsUpd->FspsTestConfig.SkipPamLock = (UINT8) MiscPeiConfig->SkipPamLock;
    FspsUpd->FspsTestConfig.EdramTestMode = (UINT8) MiscPeiConfig->EdramTestMode;
  }

  if (PciePeiConfig != NULL) {
    FspsUpd->FspsConfig.DmiAspm = (UINT8) PciePeiConfig->DmiAspm;
    FspsUpd->FspsTestConfig.DmiExtSync = (UINT8) PciePeiConfig->DmiExtSync;
    FspsUpd->FspsTestConfig.DmiIot = (UINT8) PciePeiConfig->DmiIot;
    for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
      FspsUpd->FspsConfig.PegDeEmphasis[Index] = PciePeiConfig->PegDeEmphasis[Index];
      FspsUpd->FspsConfig.PegSlotPowerLimitValue[Index] = PciePeiConfig->PegSlotPowerLimitValue[Index];
      FspsUpd->FspsConfig.PegSlotPowerLimitScale[Index] = PciePeiConfig->PegSlotPowerLimitScale[Index];
      FspsUpd->FspsConfig.PegPhysicalSlotNumber[Index] = PciePeiConfig->PegPhysicalSlotNumber[Index];
      FspsUpd->FspsTestConfig.PegMaxPayload[Index] = PciePeiConfig->PegMaxPayload[Index];
    }
  }

  if (GtConfig != NULL) {
    FspsUpd->FspsConfig.PavpEnable = (UINT8) GtConfig->PavpEnable;
    FspsUpd->FspsConfig.CdClock = (UINT8) GtConfig->CdClock;
    FspsUpd->FspsTestConfig.RenderStandby = (UINT8) GtConfig->RenderStandby;
    FspsUpd->FspsTestConfig.PmSupport = (UINT8) GtConfig->PmSupport;
    FspsUpd->FspsTestConfig.CdynmaxClampEnable = (UINT8) GtConfig->CdynmaxClampEnable;
    FspsUpd->FspsTestConfig.GtFreqMax = (UINT8) GtConfig->GtFreqMax;
    FspsUpd->FspsTestConfig.DisableTurboGt = (UINT8) GtConfig->DisableTurboGt;
    FspsUpd->FspsConfig.SkipS3CdClockInit = (UINT8)GtConfig->SkipS3CdClockInit;

    //
    // For FSP, FspsUpd->FspsConfig.PeiGraphicsPeimInit is always enabled as default.
    //
    FspsUpd->FspsConfig.PeiGraphicsPeimInit = (UINT8) GtConfig->PeiGraphicsPeimInit; // SA: InternalOnly: For Internal validation we still need to enable both Enable/Disable Cases

    //
    // Update UPD: VBT & LogoPtr
    //
    if (BootMode == BOOT_ON_S3_RESUME) {
      FspsUpd->FspsConfig.GraphicsConfigPtr = (UINT32) NULL;
    } else {
      FspsUpd->FspsConfig.GraphicsConfigPtr = (UINT32) GtConfig->GraphicsConfigPtr;
    }
    DEBUG(( DEBUG_INFO, "VbtPtr from GraphicsPeiConfig is 0x%x\n", FspsUpd->FspsConfig.GraphicsConfigPtr));

    FspsUpd->FspsConfig.LogoPtr  = (UINT32) GtConfig->LogoPtr;
    FspsUpd->FspsConfig.LogoSize = GtConfig->LogoSize;
    DEBUG(( DEBUG_INFO, "LogoPtr from PeiFspSaPolicyInit GraphicsPeiConfig is 0x%x\n", FspsUpd->FspsConfig.LogoPtr));
    DEBUG(( DEBUG_INFO, "LogoSize from PeiFspSaPolicyInit GraphicsPeiConfig is 0x%x\n", FspsUpd->FspsConfig.LogoSize));

    FspsUpd->FspsConfig.BltBufferAddress  = (UINT32) GtConfig->BltBufferAddress;
    FspsUpd->FspsConfig.BltBufferSize     = (UINT32) GtConfig->BltBufferSize;

    //
    // Update DDI/DDC configuration
    //
    FspsUpd->FspsConfig.DdiPortEdp = GtConfig->DdiConfiguration.DdiPortEdp;
    FspsUpd->FspsConfig.DdiPortBHpd = GtConfig->DdiConfiguration.DdiPortBHpd;
    FspsUpd->FspsConfig.DdiPortCHpd = GtConfig->DdiConfiguration.DdiPortCHpd;
    FspsUpd->FspsConfig.DdiPortDHpd = GtConfig->DdiConfiguration.DdiPortDHpd;
    FspsUpd->FspsConfig.DdiPortFHpd = GtConfig->DdiConfiguration.DdiPortFHpd;
    FspsUpd->FspsConfig.DdiPortBDdc = GtConfig->DdiConfiguration.DdiPortBDdc;
    FspsUpd->FspsConfig.DdiPortCDdc = GtConfig->DdiConfiguration.DdiPortCDdc;
    FspsUpd->FspsConfig.DdiPortDDdc = GtConfig->DdiConfiguration.DdiPortDDdc;
    FspsUpd->FspsConfig.DdiPortFDdc = GtConfig->DdiConfiguration.DdiPortFDdc;

  }

  if (GnaConfig != NULL) {
    FspsUpd->FspsConfig.GnaEnable = (UINT8) GnaConfig->GnaEnable;
#ifdef TESTMENU_FLAG
#endif // TESTMENU_FLAG
  }


  return EFI_SUCCESS;
}

