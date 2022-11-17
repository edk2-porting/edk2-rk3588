/** @file
  This is the Common driver that initializes the Intel System Agent.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "SaInit.h"
#include <Library/PciSegmentLib.h>
#include <HostBridgeDataHob.h>
#include <SaConfigHob.h>
#include <Protocol/PciEnumerationComplete.h>

///
/// Global Variables
///
GLOBAL_REMOVE_IF_UNREFERENCED SA_CONFIG_HOB                          *mSaConfigHob;
BOOLEAN                                                              mSkipPamLock = FALSE;

/*
  Intel(R) Core Processor Skylake BWG version 0.4.0

  18.6 System Agent Configuration Locking
   For reliable operation and security, System BIOS must set the following bits:
   1. For all modern Intel processors, Intel strongly recommends that BIOS should set
       the D_LCK bit. Set B0:D0:F0.R088h [4] = 1b to lock down SMRAM space.
  BaseAddr values for mSaSecurityRegisters that uses PciExpressBaseAddress will be initialized at
  Runtime inside function CpuPcieInitPolicy().
*/
GLOBAL_REMOVE_IF_UNREFERENCED BOOT_SCRIPT_REGISTER_SETTING mSaSecurityRegisters[] = {
  {0,  R_SA_SMRAMC,  0xFFFFFFFF,  BIT4}
};

/**
  SystemAgent Initialization Common Function.

  @retval EFI_SUCCESS   - Always.
**/

VOID
SaInitEntryPoint (
  VOID
  )
{
  HOST_BRIDGE_DATA_HOB        *HostBridgeDataHob;

  ///
  /// Get Host Bridge Data HOB
  ///
  HostBridgeDataHob = NULL;
  HostBridgeDataHob = (HOST_BRIDGE_DATA_HOB *) GetFirstGuidHob (&gHostBridgeDataHobGuid);
  if (HostBridgeDataHob != NULL) {
    mSkipPamLock = HostBridgeDataHob->SkipPamLock;
  }
  return;
}

/**
  This function does SA security lock
**/
VOID
SaSecurityLock (
  VOID
  )
{
  UINT8           Index;
  UINT64          BaseAddress;
  UINT32          RegOffset;
  UINT32          Data32And;
  UINT32          Data32Or;

  ///
  /// 17.2 System Agent Security Lock configuration
  ///
  DEBUG ((DEBUG_INFO, "DXE SaSecurityLock\n"));
  for (Index = 0; Index < (sizeof (mSaSecurityRegisters) / sizeof (BOOT_SCRIPT_REGISTER_SETTING)); Index++) {
    BaseAddress = mSaSecurityRegisters[Index].BaseAddr;
    RegOffset   = mSaSecurityRegisters[Index].Offset;
    Data32And   = mSaSecurityRegisters[Index].AndMask;
    Data32Or    = mSaSecurityRegisters[Index].OrMask;

    if (RegOffset == R_SA_SMRAMC) {
      ///
      /// SMRAMC LOCK must use CF8/CFC access
      ///
      PciCf8Or8 (PCI_CF8_LIB_ADDRESS (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_SMRAMC), (UINT8) Data32Or);
      BaseAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_SMRAMC);
      S3BootScriptSavePciCfgReadWrite (
        S3BootScriptWidthUint8,
        (UINTN) BaseAddress,
        &Data32Or,
        &Data32And
        );
    }
  }

}

/**
  This function performs SA Security locking in EndOfDxe callback

  @retval EFI_SUCCESS     - Security lock has done
  @retval EFI_UNSUPPORTED - Security lock not done successfully
**/
EFI_STATUS
SaSecurityInit (
  VOID
  )
{

  UINT8                     Index;

  for (Index = 0; Index < (sizeof (mSaSecurityRegisters) / sizeof (BOOT_SCRIPT_REGISTER_SETTING)); Index++) {
    if (mSaSecurityRegisters[Index].BaseAddr != PcdGet64 (PcdMchBaseAddress)) {
      mSaSecurityRegisters[Index].BaseAddr = PcdGet64 (PcdSiPciExpressBaseAddress);
    }
  }
  SaSecurityLock ();

  return EFI_SUCCESS;
}
