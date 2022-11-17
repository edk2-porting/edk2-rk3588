/** @file
  EFI PEIM for Emulation Platform Initial

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/EmulationConfigurationLib.h>
#include <EmulationConfiguration.h>
#include <Guid/EmulationDfxVariable.h>
#include <PlatformHost.h>
#include <Cpu/CpuIds.h>

EFI_PEI_PPI_DESCRIPTOR     mPpiListEmulation = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEmulationHobGuid,
  NULL
};

#define CSR_EMULATION_FLAG_OFFSET  0xFC

EFI_STATUS
EFIAPI
EmulationPlatformInitEntry (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{

  EFI_STATUS                           Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI      *PeiVariable;
  EMULATION_DFX_CONFIGURATION          EmulationVariable;
  UINTN                                BufferSize = sizeof(EMULATION_DFX_CONFIGURATION);
  EMULATION_SETTING                    *EmulationSetting;
  UINT32                               RegEax;
  UINT16                               CpuFamily;
  UINTN                                PciLibAddress;
  UINT32                               EmulationType;

  //
  // Build the Emulation Hob
  //
  EmulationSetting = BuildGuidHob (&gEmulationHobGuid, sizeof (EMULATION_SETTING));
  if (EmulationSetting == NULL) {
    DEBUG((EFI_D_ERROR, "Emulation BuildGuidDataHob fail!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  EmulationSetting->UbiosGenerationSetting = EMULATION_DISABLE;
  EmulationSetting->HybridSystemLevelEmulationSetting = EMULATION_DISABLE;
  EmulationSetting->UbiosOutputMode = ASM_OUTPUT_ENABLE;
  EmulationSetting->LoopBackLabelNumber = 0;
  EmulationSetting->FnvAccessValue = FNV_ACCESS_DISABLE;
  EmulationSetting->MsrTraceEnable = MSR_OUTPUT_DISABLE;

  //
  // Store variable into the hob.
  //
  (*PeiServices)->LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  Status = PeiVariable->GetVariable (PeiVariable, EMULATION_DFX_CONFIGURATION_NAME, &gEmulationDfxVariableGuid, NULL, &BufferSize, &EmulationVariable);
  DEBUG ((DEBUG_INFO, "Emulation GetVariable status = %r !\n", Status));
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  EmulationSetting->UbiosGenerationSetting = EmulationVariable.DfxUbiosGeneration;
  EmulationSetting->HybridSystemLevelEmulationSetting = EmulationVariable.DfxHybridSystemLevelEmulation;
  EmulationSetting->MsrTraceEnable = EmulationVariable.DfxPmMsrTrace;

  //
  // Check the override value
  //
  AsmCpuid (1, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT16) (RegEax >> 4);

  if (CpuFamily == CPU_FAMILY_SKX) {
    //
    // Simics flag is at B0:D0:F0 offset 0xFC for SKX.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 0, 0, CSR_EMULATION_FLAG_OFFSET);
  } else {
    //
    // Simics flag is at B0:D3:F0 offset 0xFC for 10nm.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 3, 0, CSR_EMULATION_FLAG_OFFSET);
  }

  EmulationType = PciRead32 (PciLibAddress);

  if (EmulationType != 0xFFFFFFFF) {
    if ((EmulationType & UBIOS_GENERATION_EN) != 0) {
      EmulationSetting->UbiosGenerationSetting = EMULATION_ENABLE;
      DEBUG ((DEBUG_INFO, "EmulationVariable.DfxUbiosGeneration = %d\n", EmulationSetting->UbiosGenerationSetting));
    }

    if ((EmulationType & HYBRID_SYSTEM_LEVEL_EMULATION_EN) != 0) {
      EmulationSetting->HybridSystemLevelEmulationSetting = EMULATION_ENABLE;
      DEBUG ((DEBUG_INFO, "EmulationSetting->HybridSystemLevelEmulationSetting = %d\n", EmulationSetting->HybridSystemLevelEmulationSetting));
    }
  }
  //
  // Install gEmulationHobGuid PPI to inform the Emulation Hob is ready.
  //
  Status = PeiServicesInstallPpi (&mPpiListEmulation);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
