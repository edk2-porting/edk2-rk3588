/** @file
  This file is PeiCpuPolicy library.

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiCpuPolicyLibrary.h"
#include <Library/MmPciLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PostCodeLib.h>
#include <Library/SaPlatformLib.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuConfigLibPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_CONFIG_LIB_PREMEM_CONFIG  *CpuConfigLibPreMemConfig;
  CPU_FAMILY  CpuFamily;
  CPU_SKU     CpuSku;
  BOOLEAN     PegDisabled;
  UINT64      MchBar;

  CpuConfigLibPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuConfigLibPreMemConfig->Header.GuidHob.Name = %g\n", &CpuConfigLibPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuConfigLibPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuConfigLibPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Config Lib PreMem configuration
  ********************************/
  CpuConfigLibPreMemConfig->HyperThreading          = CPU_FEATURE_ENABLE;
  CpuConfigLibPreMemConfig->BootFrequency           = 1;    // Maximum non-turbo Performance
  CpuConfigLibPreMemConfig->ActiveCoreCount         = 0;    // All cores active
  CpuConfigLibPreMemConfig->VmxEnable               = CPU_FEATURE_ENABLE;
  CpuConfigLibPreMemConfig->CpuRatio = RShiftU64 (AsmReadMsr64 (MSR_PLATFORM_INFO), N_PLATFORM_INFO_MAX_RATIO) & B_PLATFORM_INFO_RATIO_MASK;

  ///
  /// FCLK Frequency
  ///
  CpuFamily = GetCpuFamily ();
  CpuSku    = GetCpuSku ();

  DEBUG_CODE_BEGIN ();
  ///
  /// Ensure the upper 7-bits [38:32] of MCHBAR are zero so we can access MCHBAR in 32-bit mode.
  ///
  MchBar = MmioRead32 (MmPciBase (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN) + R_SA_MCHBAR + 0x4) & 0x7F;
  if (MchBar != 0x0) {
    DEBUG ((
      DEBUG_ERROR,
      "Error: [%a]:[%dL] MCHBAR configured to >4GB\n",
      __FUNCTION__,
      __LINE__
      ));
  }
  ASSERT (MchBar == 0x0);
  DEBUG_CODE_END ();

  MchBar = MmioRead32 (MmPciBase (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN) + R_SA_MCHBAR) &~BIT0;
  if (IsPchLinkDmi (CpuFamily) && (MmioRead16 (MmPciBase (SA_PEG_BUS_NUM, SA_PEG_DEV_NUM, SA_PEG10_FUN_NUM) + PCI_VENDOR_ID_OFFSET) != 0xFFFF)) {
    PegDisabled = MmioRead32 ((UINTN) MchBar + R_SA_MCHBAR_BIOS_RESET_CPL_OFFSET) & BIT3;
  } else {
    PegDisabled = 1;
  }

  ///
  /// DT/Halo FCLK = 1GHz
  /// Ulx/Ult FCLK = 800MHz
  ///
  if (((CpuSku == EnumCpuHalo) && (!PegDisabled)) || (CpuSku == EnumCpuTrad)) {
    CpuConfigLibPreMemConfig->FClkFrequency = 1;  // 1Ghz
  } else {
    CpuConfigLibPreMemConfig->FClkFrequency = 0;  // 800MHz
  }

  CpuConfigLibPreMemConfig->PeciC10Reset = 1;  // Disables Peci Reset on C10 exit
}

/**
  Load Overclocking pre-mem Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuOverclockingPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_OVERCLOCKING_PREMEM_CONFIG  *CpuOverclockingPreMemConfig;
  CpuOverclockingPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuOverclockingPreMemConfig->Header.GuidHob.Name = %g\n", &CpuOverclockingPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuOverclockingPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuOverclockingPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Overclocking PreMem configuration
  ********************************/
}


static COMPONENT_BLOCK_ENTRY  mCpuIpBlocksPreMem [] = {
  {&gCpuConfigLibPreMemConfigGuid,    sizeof (CPU_CONFIG_LIB_PREMEM_CONFIG),    CPU_CONFIG_LIB_PREMEM_CONFIG_REVISION,  LoadCpuConfigLibPreMemConfigDefault},
  {&gCpuOverclockingPreMemConfigGuid, sizeof (CPU_OVERCLOCKING_PREMEM_CONFIG),  CPU_OVERCLOCKING_PREMEM_CONFIG_REVISION,LoadCpuOverclockingPreMemConfigDefault},
};

/**
  Get CPU PREMEM config block table total size.

  @retval Size of CPU PREMEM config block table
**/
UINT16
EFIAPI
CpuGetPreMemConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mCpuIpBlocksPreMem[0], sizeof (mCpuIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  CpuAddPreMemConfigBlocks add all CPU PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;
  DEBUG((DEBUG_INFO, "CPU Pre-Mem Entry \n"));
  PostCode (0xC00);

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mCpuIpBlocksPreMem[0], sizeof (mCpuIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG((DEBUG_INFO, "CpuAddPreMemConfigBlocks Done \n"));
  PostCode (0xC0F);

  return Status;
}

