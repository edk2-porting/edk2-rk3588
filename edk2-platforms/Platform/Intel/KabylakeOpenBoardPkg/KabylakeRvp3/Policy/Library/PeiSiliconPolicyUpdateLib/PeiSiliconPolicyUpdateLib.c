/** @file
  Provides silicon policy update library functions.

Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <ConfigBlock.h>
#include <SaPolicyCommon.h>
#include <CpuPolicyCommon.h>
#include <PchPreMemPolicyCommon.h>
#include <Pi/PiFirmwareFile.h>
#include <Register/Cpuid.h>
#include <PchHsioPtssTables.h>
#include <Library/PchInfoLib.h>
#include <Library/SiliconPolicyUpdateLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PchHsioLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/MmPciLib.h>
#include <Library/IoLib.h>

/**
  Get the next microcode patch pointer.

  @param[in, out] MicrocodeData - Input is a pointer to the last microcode patch address found,
                                  and output points to the next patch address found.

  @retval EFI_SUCCESS           - Patch found.
  @retval EFI_NOT_FOUND         - Patch not found.
**/
EFI_STATUS
EFIAPI
RetrieveMicrocode (
  IN OUT CPU_MICROCODE_HEADER **MicrocodeData
  )
{
  UINTN                MicrocodeStart;
  UINTN                MicrocodeEnd;
  UINTN                TotalSize;

  if ((FixedPcdGet32 (PcdFlashMicrocodeFvBase) == 0) || (FixedPcdGet32 (PcdFlashMicrocodeFvSize) == 0)) {
    return EFI_NOT_FOUND;
  }

  ///
  /// Microcode binary in SEC
  ///
  MicrocodeStart = (UINTN) FixedPcdGet32 (PcdFlashMicrocodeFvBase) +
          ((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) FixedPcdGet32 (PcdFlashMicrocodeFvBase))->HeaderLength +
          sizeof (EFI_FFS_FILE_HEADER);

  MicrocodeEnd = (UINTN) FixedPcdGet32 (PcdFlashMicrocodeFvBase) + (UINTN) FixedPcdGet32 (PcdFlashMicrocodeFvSize);

  if (*MicrocodeData == NULL) {
    *MicrocodeData = (CPU_MICROCODE_HEADER *) (UINTN) MicrocodeStart;
  } else {
    if (*MicrocodeData < (CPU_MICROCODE_HEADER *) (UINTN) MicrocodeStart) {
      DEBUG ((DEBUG_INFO, "[CpuPolicy]*MicrocodeData < MicrocodeStart \n"));
      return EFI_NOT_FOUND;
    }

    TotalSize = (UINTN) ((*MicrocodeData)->TotalSize);
    if (TotalSize == 0) {
      TotalSize = 2048;
    }

    *MicrocodeData = (CPU_MICROCODE_HEADER *) ((UINTN)*MicrocodeData + TotalSize);
    if (*MicrocodeData >= (CPU_MICROCODE_HEADER *) (UINTN) (MicrocodeEnd) || (*MicrocodeData)->TotalSize == (UINT32) -1) {
      DEBUG ((DEBUG_INFO, "[CpuPolicy]*MicrocodeData >= MicrocodeEnd \n"));
      return EFI_NOT_FOUND;
    }
  }
  return EFI_SUCCESS;
}

/**
  Get the microcode patch pointer.

  @retval EFI_PHYSICAL_ADDRESS - Address of the microcode patch, or NULL if not found.
**/
EFI_PHYSICAL_ADDRESS
PlatformCpuLocateMicrocodePatch (
  VOID
  )
{
  EFI_STATUS           Status;
  CPU_MICROCODE_HEADER *MicrocodeData;
  EFI_CPUID_REGISTER   Cpuid;
  UINT32               UcodeRevision;
  UINTN                MicrocodeBufferSize;
  VOID                 *MicrocodeBuffer = NULL;

  AsmCpuid (
    CPUID_VERSION_INFO,
    &Cpuid.RegEax,
    &Cpuid.RegEbx,
    &Cpuid.RegEcx,
    &Cpuid.RegEdx
    );

  UcodeRevision = GetCpuUcodeRevision ();
  MicrocodeData = NULL;
  while (TRUE) {
    ///
    /// Find the next patch address
    ///
    Status = RetrieveMicrocode (&MicrocodeData);
    DEBUG ((DEBUG_INFO, "MicrocodeData = %x\n", MicrocodeData));

    if (Status != EFI_SUCCESS) {
      break;
    } else if (CheckMicrocode (Cpuid.RegEax, MicrocodeData, &UcodeRevision)) {
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    return (EFI_PHYSICAL_ADDRESS) (UINTN) NULL;
  }

  ///
  /// Check that microcode patch size is <= 128K max size,
  /// then copy the patch from FV to temp buffer for faster access.
  ///
  MicrocodeBufferSize = (UINTN) MicrocodeData->TotalSize;

  if (MicrocodeBufferSize <= MAX_MICROCODE_PATCH_SIZE) {
    MicrocodeBuffer = AllocatePages (EFI_SIZE_TO_PAGES (MicrocodeBufferSize));
    if (MicrocodeBuffer != NULL) {
      DEBUG(( DEBUG_INFO, "Copying Microcode to temp buffer.\n"));
      CopyMem (MicrocodeBuffer, MicrocodeData, MicrocodeBufferSize);

      return (EFI_PHYSICAL_ADDRESS) (UINTN) MicrocodeBuffer;
    } else {
      DEBUG(( DEBUG_ERROR, "Failed to allocate enough memory for Microcode Patch.\n"));
    }
  } else {
    DEBUG(( DEBUG_ERROR, "Microcode patch size is greater than max allowed size of 128K.\n"));
  }
  return (EFI_PHYSICAL_ADDRESS) (UINTN) NULL;
}

/**
  Update HSIO policy per board.

  @param[in] Policy - Policy PPI pointer (caller should ensure it is valid pointer)

**/
VOID
InstallPlatformHsioPtssTable (
  IN VOID *Policy
  )
{
  HSIO_PTSS_TABLES            *UnknowPtssTables;
  HSIO_PTSS_TABLES            *SpecificPtssTables;
  HSIO_PTSS_TABLES            *PtssTables;
  UINT8                       PtssTableIndex;
  UINT32                      UnknowTableSize;
  UINT32                      SpecificTableSize;
  UINT32                      TableSize;
  UINT32                      Entry;
  UINT8                       LaneNum;
  UINT8                       Index;
  UINT8                       MaxSataPorts;
  UINT8                       MaxPciePorts;
  UINT8                       PcieTopologyReal[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8                       PciePort;
  UINTN                       RpBase;
  UINTN                       RpDevice;
  UINTN                       RpFunction;
  UINT32                      StrapFuseCfg;
  UINT8                       PcieControllerCfg;
  PCH_HSIO_PCIE_PREMEM_CONFIG     *HsioPciePreMemConfig;
  PCH_HSIO_SATA_PREMEM_CONFIG     *HsioSataPreMemConfig;
  EFI_STATUS                  Status;

  Status = GetConfigBlock (Policy, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock (Policy, &gHsioSataPreMemConfigGuid, (VOID *) &HsioSataPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  UnknowPtssTables = NULL;
  UnknowTableSize = 0;
  SpecificPtssTables = NULL;
  SpecificTableSize = 0;

  if (GetPchGeneration () == SklPch) {
    switch (PchStepping ()) {
      case PchLpB0:
      case PchLpB1:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowLpHsioPtssTable1);
        UnknowTableSize = PcdGet16 (PcdUnknowLpHsioPtssTable1Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificLpHsioPtssTable1);
        SpecificTableSize = PcdGet16 (PcdSpecificLpHsioPtssTable1Size);
        break;
      case PchLpC0:
      case PchLpC1:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowLpHsioPtssTable2);
        UnknowTableSize = PcdGet16 (PcdUnknowLpHsioPtssTable2Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificLpHsioPtssTable2);
        SpecificTableSize = PcdGet16 (PcdSpecificLpHsioPtssTable2Size);
        break;
      case PchHB0:
      case PchHC0:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowHHsioPtssTable1);
        UnknowTableSize = PcdGet16 (PcdUnknowHHsioPtssTable1Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificHHsioPtssTable1);
        SpecificTableSize = PcdGet16 (PcdSpecificHHsioPtssTable1Size);
        break;
      case PchHD0:
      case PchHD1:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowHHsioPtssTable2);
        UnknowTableSize = PcdGet16 (PcdUnknowHHsioPtssTable2Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificHHsioPtssTable2);
        SpecificTableSize = PcdGet16 (PcdSpecificHHsioPtssTable2Size);
        break;
      default:
        UnknowPtssTables = NULL;
        UnknowTableSize = 0;
        SpecificPtssTables = NULL;
        SpecificTableSize = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }
  } else {
    switch (PchStepping ()) {
      case KblPchHA0:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowHHsioPtssTable2);
        UnknowTableSize = PcdGet16 (PcdUnknowHHsioPtssTable2Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificHHsioPtssTable2);
        SpecificTableSize = PcdGet16 (PcdSpecificHHsioPtssTable2Size);
        break;
      default:
        UnknowPtssTables = NULL;
        UnknowTableSize = 0;
        SpecificPtssTables = NULL;
        SpecificTableSize = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }
  }

  PtssTableIndex = 0;
  MaxSataPorts = GetPchMaxSataPortNum ();
  MaxPciePorts = GetPchMaxPciePortNum ();
  ZeroMem (PcieTopologyReal, sizeof (PcieTopologyReal));
  //
  //Populate PCIe topology based on lane configuration
  //
  for (PciePort = 0; PciePort < MaxPciePorts; PciePort += 4) {
    Status = GetPchPcieRpDevFun (PciePort, &RpDevice, &RpFunction);
    ASSERT_EFI_ERROR (Status);

    RpBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDevice, (UINT32) RpFunction);
    StrapFuseCfg = MmioRead32 (RpBase + R_PCH_PCIE_STRPFUSECFG);
    PcieControllerCfg = (UINT8) ((StrapFuseCfg & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
    DEBUG ((DEBUG_INFO, "PCIE Port %d StrapFuseCfg Value = %d\n", PciePort, PcieControllerCfg));
  }
  for (Index = 0; Index < MaxPciePorts; Index++) {
    DEBUG ((DEBUG_INFO, "PCIE PTSS Assigned RP %d Topology = %d\n", Index, PcieTopologyReal[Index]));
  }
  //
  //Case 1: BoardId is known, Topology is known/unknown
  //Case 1a: SATA
  //
  PtssTables = SpecificPtssTables;
  TableSize = SpecificTableSize;
  for (Index = 0; Index < MaxSataPorts; Index++) {
    if (PchGetSataLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA)
          )
        {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD20) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
            HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMagEnable    = TRUE;
            HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMag          =  (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_TX_DWORD8)) {
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) {
              HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmpEnable  = TRUE;
              HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmp        = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0);
            }
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) {
              HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmpEnable  = TRUE;
              HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmp        = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0);
            }
          } else {
            ASSERT (FALSE);
          }
        }
      }
    }
  }
  //
  //Case 1b: PCIe
  //
  for (Index = 0; Index < MaxPciePorts; Index++) {
    if (PchGetPcieLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
            (PcieTopologyReal[Index] == PtssTables[Entry].Topology)) {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD25) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
            HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable           = TRUE;
            HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle                 = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0);

          } else {
            ASSERT (FALSE);
          }
        }
      }
    }
  }
  //
  //Case 2: BoardId is unknown, Topology is known/unknown
  //
  if (PtssTableIndex == 0) {
    DEBUG ((DEBUG_INFO, "PTSS Settings for unknown board will be applied\n"));

    PtssTables = UnknowPtssTables;
    TableSize = UnknowTableSize;

    for (Index = 0; Index < MaxSataPorts; Index++) {
      if (PchGetSataLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
              (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA)
             )
          {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD20) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
              HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMagEnable    = TRUE;
              HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMag          = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;

            } else if (PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_TX_DWORD8) {
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) {
                HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmpEnable  = TRUE;
                HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmp        = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0);

              }
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) {
                HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmpEnable  = TRUE;
                HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmp        = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0);
              }
            } else {
              ASSERT (FALSE);
            }
          }
        }
      }
    }
    for (Index = 0; Index < MaxPciePorts; Index++) {
      if (PchGetPcieLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
              (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
              (PcieTopologyReal[Index] == PtssTables[Entry].Topology)) {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD25) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
              HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable           = TRUE;
              HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle                 = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0);
            } else {
              ASSERT (FALSE);
            }
          }
        }
      }
    }
  }
}

/**
  Update PreMem phase silicon policy per board.

  @param[in] Policy - Policy PPI pointer.

  @retval Policy - Policy PPI pointer.

**/
VOID *
EFIAPI
SiliconPolicyUpdatePreMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                    Status;
  SA_MISC_PEI_PREMEM_CONFIG     *MiscPeiPreMemConfig;
  MEMORY_CONFIG_NO_CRC          *MemConfigNoCrc;
  VOID                          *Buffer;
  UINTN                         VariableSize;
  VOID                          *MemorySavedData;
  UINT8                         SpdAddressTable[4];

  DEBUG((DEBUG_INFO, "\nUpdating Policy in Pre-Mem\n"));

  if (Policy != NULL) {
    SpdAddressTable[0] = PcdGet8 (PcdMrcSpdAddressTable0);
    SpdAddressTable[1] = PcdGet8 (PcdMrcSpdAddressTable1);
    SpdAddressTable[2] = PcdGet8 (PcdMrcSpdAddressTable2);
    SpdAddressTable[3] = PcdGet8 (PcdMrcSpdAddressTable3);

    MiscPeiPreMemConfig = NULL;
    Status = GetConfigBlock (Policy, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);

    if (MiscPeiPreMemConfig != NULL) {
      //
      // Pass board specific SpdAddressTable to policy
      //
      CopyMem ((VOID *) MiscPeiPreMemConfig->SpdAddressTable, (VOID *) SpdAddressTable, (sizeof (UINT8) * 4));

      //
      // Set size of SMRAM
      //
      MiscPeiPreMemConfig->TsegSize = PcdGet32 (PcdTsegSize);

      //
      // Initialize S3 Data variable (S3DataPtr). It may be used for warm and fast boot paths.
      // Note: AmberLake FSP does not implement the FSPM_ARCH_CONFIG_PPI added in FSP 2.1, hence
      // the platform specific S3DataPtr must be used instead.
      //
      VariableSize = 0;
      MemorySavedData = NULL;
      Status = PeiGetVariable (
                L"MemoryConfig",
                &gFspNonVolatileStorageHobGuid,
                &MemorySavedData,
                &VariableSize
                );
      DEBUG ((DEBUG_INFO, "Get L\"MemoryConfig\" gFspNonVolatileStorageHobGuid - %r\n", Status));
      DEBUG ((DEBUG_INFO, "MemoryConfig Size - 0x%x\n", VariableSize));
      if (!EFI_ERROR (Status)) {
        MiscPeiPreMemConfig->S3DataPtr = MemorySavedData;
      }

      //
      // In FSP Dispatch Mode these BAR values are initialized by SiliconPolicyInitPreMem() in
      // KabylakeSiliconPkg/Library/PeiSiliconPolicyInitLib/PeiPolicyInitPreMem.c; this function calls
      // PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI->PeiPreMemPolicyInit() to initialize all Config Blocks
      // with default policy values (including these BAR values.) PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI
      // is implemented in the FSP. Make sure the value that FSP is using matches the value we are using.
      //
      ASSERT (PcdGet64 (PcdMchBaseAddress)  <= 0xFFFFFFFF);
      ASSERT (MiscPeiPreMemConfig->MchBar   == (UINT32) PcdGet64 (PcdMchBaseAddress));
      ASSERT (MiscPeiPreMemConfig->SmbusBar == PcdGet16 (PcdSmbusBaseAddress));
    }
    MemConfigNoCrc = NULL;
    Status = GetConfigBlock (Policy, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    ASSERT_EFI_ERROR (Status);

    if (MemConfigNoCrc != NULL) {
      MemConfigNoCrc->PlatformMemorySize = PcdGet32 (PcdPeiMinMemorySize);

      //
      // Only if SpdAddressTables are all zero we need to pass hard-coded SPD data buffer.
      // Otherwise FSP will retrieve SPD from DIMM basing on SpdAddressTables policy.
      //
      if (*((UINT32 *) (UINTN) SpdAddressTable) == 0) {
        DEBUG((DEBUG_INFO, "Override MemorySpdPtr...\n"));
        CopyMem((VOID *) MemConfigNoCrc->SpdData->SpdData[0][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16 (PcdMrcSpdDataSize));
        CopyMem((VOID *) MemConfigNoCrc->SpdData->SpdData[1][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16 (PcdMrcSpdDataSize));
      }

      DEBUG((DEBUG_INFO, "Updating Dq Byte Map and DQS Byte Swizzling Settings...\n"));
      Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqByteMap);
      if (Buffer) {
        CopyMem ((VOID *) MemConfigNoCrc->DqByteMap->DqByteMap[0], Buffer, 12);
        CopyMem ((VOID *) MemConfigNoCrc->DqByteMap->DqByteMap[1], (UINT8*) Buffer + 12, 12);
      }
      Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqsMapCpu2Dram);
      if (Buffer) {
        CopyMem ((VOID *) MemConfigNoCrc->DqsMap->DqsMapCpu2Dram[0], Buffer, 8);
        CopyMem ((VOID *) MemConfigNoCrc->DqsMap->DqsMapCpu2Dram[1], (UINT8*) Buffer + 8, 8);
      }

      DEBUG((DEBUG_INFO, "Updating Dq Pins Interleaved,Rcomp Resistor & Rcomp Target Settings...\n"));
      Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompResistor);
      if (Buffer) {
        CopyMem ((VOID *) &(MemConfigNoCrc->RcompData->RcompResistor[0]), Buffer, 6);
      }
      Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompTarget);
      if (Buffer) {
        CopyMem ((VOID *) &(MemConfigNoCrc->RcompData->RcompTarget[0]), Buffer, 10);
      }
    }
    //
    // Update PCD policy
    //
    InstallPlatformHsioPtssTable (Policy);
  }

  return Policy;
}

/**
  Update PostMem phase silicon policy per board.

  @param[in] Policy - Policy PPI pointer.

  @retval Policy - Policy PPI pointer.

**/
VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                      Status;
  VOID                            *Buffer;
  VOID                            *MemBuffer;
  UINT32                          Size;
  GRAPHICS_PEI_CONFIG             *GtConfig;
  CPU_CONFIG                      *CpuConfig;

  DEBUG((DEBUG_INFO, "\nUpdating Policy in Post Mem\n"));

  GtConfig = NULL;
  Status = GetConfigBlock ((VOID *) Policy, &gGraphicsPeiConfigGuid, (VOID *)&GtConfig);
  ASSERT_EFI_ERROR (Status);

  if (GtConfig != NULL) {
    //
    // Always enable PEI graphics initialization.
    //
    GtConfig->PeiGraphicsPeimInit = 1;
    Size   = 0;
    Buffer = NULL;
    PeiGetSectionFromAnyFv (PcdGetPtr (PcdGraphicsVbtGuid), EFI_SECTION_RAW, 0, &Buffer, &Size);
    if (Buffer == NULL) {
      DEBUG((DEBUG_WARN, "Could not locate VBT\n"));
    } else {
      MemBuffer = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)Size));
      if ((MemBuffer != NULL) && (Buffer != NULL)) {
        CopyMem (MemBuffer, (VOID *)Buffer, (UINTN)Size);
        GtConfig->GraphicsConfigPtr = MemBuffer;
      } else {
        DEBUG((DEBUG_WARN, "Error in locating / copying VBT.\n"));
        GtConfig->GraphicsConfigPtr = 0;
      }
    }
    DEBUG((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromFv is 0x%x\n", GtConfig->GraphicsConfigPtr));
    DEBUG((DEBUG_INFO, "Vbt Size from PeiGetSectionFromFv is 0x%x\n", Size));
    Size   = 0;
    Buffer = NULL;
    PeiGetSectionFromAnyFv (&gTianoLogoGuid, EFI_SECTION_RAW, 0, &Buffer, &Size);
    if (Buffer == NULL) {
      DEBUG((DEBUG_WARN, "Could not locate Logo\n"));
    } else {
      MemBuffer = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)Size));
      if ((MemBuffer != NULL) && (Buffer != NULL)) {
        CopyMem (MemBuffer, (VOID *)Buffer, (UINTN)Size);
        GtConfig->LogoPtr = MemBuffer;
        GtConfig->LogoSize = Size;
      } else {
        DEBUG((DEBUG_WARN, "Error in locating / copying LogoPtr.\n"));
        GtConfig->LogoPtr = 0;
        GtConfig->LogoSize = 0;
      }
    }
    DEBUG((DEBUG_INFO, "LogoPtr from PeiGetSectionFromFv is 0x%x\n", GtConfig->LogoPtr));
    DEBUG((DEBUG_INFO, "LogoSize from PeiGetSectionFromFv is 0x%x\n", GtConfig->LogoSize));
  }

  CpuConfig = NULL;
  Status = GetConfigBlock ((VOID *) Policy, &gCpuConfigGuid, (VOID *)&CpuConfig);
  ASSERT_EFI_ERROR (Status);

  if (CpuConfig != NULL) {
    CpuConfig->MicrocodePatchAddress = PlatformCpuLocateMicrocodePatch ();
  }
  return Policy;
}

/**
  Update late phase silicon policy per board.

  @param[in] Policy - Policy PPI pointer.

  @retval Policy - Policy PPI pointer.

**/
VOID *
EFIAPI
SiliconPolicyUpdateLate (
  IN VOID *Policy
  )
{
  return Policy;
}
